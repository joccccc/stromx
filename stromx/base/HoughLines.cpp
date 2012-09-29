/* 
*  Copyright 2012 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include "HoughLines.h"

#include "Image.h"
#include "Matrix.h"
#include "Utilities.h"
#include <boost/math/constants/constants.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stromx/core/DataContainer.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/NumericParameter.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ReadAccess.h>
#include <stromx/core/WriteAccess.h>


namespace stromx
{
    using namespace core;

    namespace base
    {
        const std::string HoughLines::PACKAGE(STROMX_BASE_PACKAGE_NAME);
        const Version HoughLines::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR, BASE_VERSION_PATCH);
        const std::string HoughLines::TYPE("HoughLines");
        
        HoughLines::HoughLines()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
            m_transform(PROBALISTIC_HOUGH),
            m_rho(1.0),
            m_theta(boost::math::constants::pi<double>() / 180.0),
            m_threshold(100),
            m_minLineLength(50),
            m_maxLineGap(5)
        {
        }

        void HoughLines::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case TRANSFORM:
                    m_transform = data_cast<Enum>(value);
                    break;
                case RHO:
                    if(data_cast<Double>(value) <= 0.0)
                    {
                        throw WrongParameterValue(parameter(RHO), *this,
                                                  "Rho must be positive.");
                    }
                    m_rho = data_cast<Double>(value);
                    break;
                case THETA:
                    if(data_cast<Double>(value) <= 0.0)
                    {
                        throw WrongParameterValue(parameter(THETA), *this,
                                                  "Theta must be positive.");
                    }
                    m_theta = data_cast<Double>(value);
                    break;
                case THRESHOLD:
                    m_threshold = data_cast<UInt32>(value);
                    break;
                case MIN_LINE_LENGTH:
                    if(data_cast<Double>(value) < 0.0)
                    {
                        throw WrongParameterValue(parameter(MIN_LINE_LENGTH), *this,
                                                  "Minimal line length must not be negative.");
                    }
                    m_minLineLength = data_cast<Double>(value);
                    break;
                case MAX_LINE_GAP:
                    if(data_cast<Double>(value) < 0.0)
                    {
                        throw WrongParameterValue(parameter(MAX_LINE_GAP), *this,
                                                  "Maximal line gap must not be negative.");
                    }
                    m_maxLineGap = data_cast<Double>(value);
                    break;
                default:
                    throw WrongParameterId(id, *this);
                }
            }
            catch(std::bad_cast&)
            {
                throw WrongParameterType(parameter(id), *this);
            }
        }

        const Data& HoughLines::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case RHO:
                return m_rho;
            case THETA:
                return m_theta;
            case THRESHOLD:
                return m_threshold;
            case TRANSFORM:
                return m_transform;
            case MIN_LINE_LENGTH:
                return m_minLineLength;
            case MAX_LINE_GAP:
                return m_maxLineGap;
            default:
                throw WrongParameterId(id, *this);
            }
        }    
                
        void HoughLines::execute(DataProvider& provider)
        {
            Id2DataPair srcMapper(IMAGE);
            provider.receiveInputData(srcMapper);
            
            WriteAccess<Image> src(srcMapper.data());
            Image& image = src();
            
            if(image.pixelType() != Image::MONO_8)
                throw InputError(HoughLines::IMAGE, *this, "Source image must be an 8-bit monochrome image.");
            
            // apply the transform
            cv::Mat cvImage = getOpenCvMat(image);
            cv::Mat cvLines;
            cv::HoughLinesP(cvImage, cvLines, m_rho, m_theta, m_threshold, m_minLineLength, m_maxLineGap);
            cvLines = cvLines.reshape(1, cvLines.cols);
            
            Matrix* lines = new Matrix(cvLines);
            Id2DataPair outputMapper(LINES, DataContainer(lines));
            provider.sendOutputData(outputMapper);
        }
        
        const std::vector<const core::Description*> HoughLines::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* image = new Description(IMAGE, DataVariant::MONO_IMAGE);
            image->setDoc("Image");
            inputs.push_back(image);
            
            return inputs;
        }
        
        const std::vector<const Description*> HoughLines::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* lines = new Description(LINES, DataVariant::DOUBLE_MATRIX);
            lines->setDoc("Lines");
            outputs.push_back(lines);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> HoughLines::setupParameters()
        {
            std::vector<const core::Parameter*> parameters;
            
            EnumParameter* transform = new EnumParameter(TRANSFORM);
            transform->setDoc("Transform");
            transform->add(EnumDescription(HOUGH, "Hough transform"));
            transform->add(EnumDescription(PROBALISTIC_HOUGH, "Probalistic Hough transform"));
            transform->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(transform);
            
            NumericParameter<core::Double>* rho = new NumericParameter<core::Double>(RHO);
            rho->setMin(Double(0.0));
            rho->setDoc("Rho");
            rho->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(rho);
            
            NumericParameter<core::Double>* theta = new NumericParameter<core::Double>(THETA);
            theta->setMin(Double(0.0));
            theta->setDoc("Theta");
            theta->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(theta);
            
            NumericParameter<core::UInt32>* threshold = new NumericParameter<core::UInt32>(THRESHOLD);
            threshold->setDoc("Threshold");
            threshold->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(threshold);
            
            NumericParameter<core::Double>* minLineLength = new NumericParameter<core::Double>(MIN_LINE_LENGTH);
            minLineLength->setMin(Double(0.0));
            minLineLength->setDoc("Minimal line length");
            minLineLength->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(minLineLength);
            
            NumericParameter<core::Double>* maxLineGap = new NumericParameter<core::Double>(MAX_LINE_GAP);
            maxLineGap->setMin(Double(0.0));
            maxLineGap->setDoc("Maximal line gap");
            maxLineGap->setAccessMode(core::Parameter::ACTIVATED_WRITE);
            parameters.push_back(maxLineGap);
                                        
            return parameters;
        }
    } 
}