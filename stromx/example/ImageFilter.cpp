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

#include <opencv2/core/core.hpp>
#include "stromx/example/Image.h"
#include "stromx/example/ImageFilter.h"
#include "stromx/example/Utilities.h"
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/WriteAccess.h>

namespace stromx
{
    using namespace runtime;

    namespace example
    {
        const std::string ImageFilter::PACKAGE(STROMX_EXAMPLE_PACKAGE_NAME);
        const Version ImageFilter::VERSION(0, 1, 0);
        
        ImageFilter::ImageFilter(const std::string & type)
          : OperatorKernel(type, PACKAGE, VERSION, setupInitParameters()),
            m_inPlace(true)
        {
        }

        void ImageFilter::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case IN_PLACE:
                    m_inPlace = stromx::runtime::data_cast<Bool>(value);
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

        const DataRef ImageFilter::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case IN_PLACE:
                return m_inPlace;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
    
        void ImageFilter::initialize()
        {
            stromx::runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void ImageFilter::execute(DataProvider& provider)
        {
            if(m_inPlace)
            {
                Id2DataPair srcMapper(SOURCE);
                provider.receiveInputData(srcMapper);
                
                WriteAccess<runtime::Image> src(srcMapper.data());
                runtime::Image& image = src();
                
                validateSourceImage(image);
                cv::Mat cvImage = getOpenCvMat(image);
                
                applyFilter(cvImage, cvImage);
                
                Id2DataPair outputMapper(OUTPUT, srcMapper.data());
                provider.sendOutputData(outputMapper);
            }
            else
            {
                Id2DataPair srcMapper(SOURCE);
                Id2DataPair destMapper(DESTINATION);
                provider.receiveInputData(srcMapper && destMapper);
                
                // check whether source and destination contain different data objects
                if(srcMapper.data() != destMapper.data())
                {
                    ReadAccess<runtime::Image> src(srcMapper.data());
                    WriteAccess<runtime::Image> dest(destMapper.data());
                    
                    const runtime::Image& srcImage = src();
                    runtime::Image& destImage = dest();
                    
                    validateSourceImage(srcImage);
                    if(destImage.bufferSize() < computeDestinationSize(srcImage))
                        throw InputError(DESTINATION, *this, "Destination image is too small.");
                    
                    destImage.initializeImage(srcImage.width(), srcImage.height(), srcImage.width() * srcImage.pixelSize(), destImage.buffer(), srcImage.pixelType());
                    
                    cv::Mat inCvImage = getOpenCvMat(srcImage);
                    cv::Mat outCvImage = getOpenCvMat(destImage);
                    
                    applyFilter(inCvImage, outCvImage);
                    
                    Id2DataPair outputMapper(OUTPUT, destMapper.data());
                    provider.sendOutputData( outputMapper);
                }
                else
                {
                    // If source and destination contain the same data object a read and write access
                    // can not be obtained at the same time. Instead, we get a write access to the
                    // source and treat it as in the single input case.
                    WriteAccess<runtime::Image> src(srcMapper.data());
                    runtime::Image& image = src();
                    
                    validateSourceImage(image);
                    cv::Mat cvImage = getOpenCvMat(image);
                    
                    applyFilter(cvImage, cvImage);
                    
                    Id2DataPair outputMapper(OUTPUT, srcMapper.data());
                    provider.sendOutputData(outputMapper);
                }
            }
        }
        
        const std::vector<const runtime::Description*> ImageFilter::setupInputs()
        {
            std::vector<const Description*> inputs;
            
            Description* source = new Description(SOURCE, DataVariant::MONO_IMAGE);
            source->setTitle("Source");
            inputs.push_back(source);
            
            if(! m_inPlace)
            {
                Description* destination = new Description(DESTINATION, DataVariant::IMAGE);
                destination->setTitle("Destination");
                inputs.push_back(destination);
            }
            
            return inputs;
        }
        
        const std::vector<const Description*> ImageFilter::setupOutputs()
        {
            std::vector<const Description*> outputs;
            
            Description* output = new Description(OUTPUT, DataVariant::MONO_IMAGE);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> ImageFilter::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            Parameter* inPlace = new Parameter(IN_PLACE, DataVariant::BOOL);
            inPlace->setTitle("Process in place");
            inPlace->setAccessMode(runtime::Parameter::NONE_WRITE);
            parameters.push_back(inPlace);
                                        
            return parameters;
        }
    } 
}