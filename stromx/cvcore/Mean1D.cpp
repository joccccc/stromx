#include "stromx/cvcore/Mean1D.h"

#include "stromx/cvcore/Locale.h"
#include "stromx/cvcore/Utility.h"
#include <stromx/cvsupport/Image.h>
#include <stromx/cvsupport/Matrix.h>
#include <stromx/cvsupport/Utilities.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataComposite.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/VariantComposite.h>
#include <stromx/runtime/WriteAccess.h>
#include <opencv2/core/core.hpp>

namespace stromx
{
    namespace cvcore
    {
        const std::string Mean1D::PACKAGE(STROMX_CVCORE_PACKAGE_NAME);
        const runtime::Version Mean1D::VERSION(STROMX_CVCORE_VERSION_MAJOR, STROMX_CVCORE_VERSION_MINOR, STROMX_CVCORE_VERSION_PATCH);
        const std::string Mean1D::TYPE("Mean1D");
        
        Mean1D::Mean1D()
          : runtime::OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters()),
            m_dataFlow()
        {
        }
        
        const runtime::DataRef Mean1D::getParameter(unsigned int id) const
        {
            switch(id)
            {
            case DATA_FLOW:
                return m_dataFlow;
            default:
                throw runtime::WrongParameterId(id, *this);
            }
        }
        
        void Mean1D::setParameter(unsigned int id, const runtime::Data& value)
        {
            try
            {
                switch(id)
                {
                case DATA_FLOW:
                    {
                        const runtime::Enum & castedValue = runtime::data_cast<runtime::Enum>(value);
                        if(! castedValue.variant().isVariant(runtime::Variant::ENUM))
                        {
                            throw runtime::WrongParameterType(parameter(id), *this);
                        }
                        cvsupport::checkEnumValue(castedValue, m_dataFlowParameter, *this);
                        m_dataFlow = castedValue;
                    }
                    break;
                default:
                    throw runtime::WrongParameterId(id, *this);
                }
            }
            catch(runtime::BadCast&)
            {
                throw runtime::WrongParameterType(parameter(id), *this);
            }
        }
        
        const std::vector<const runtime::Parameter*> Mean1D::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            return parameters;
        }
        
        const std::vector<const runtime::Parameter*> Mean1D::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                }
                break;
            }
            
            return parameters;
        }
        
        const std::vector<const runtime::Description*> Mean1D::setupInputs()
        {
            std::vector<const runtime::Description*> inputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    m_srcDescription = new runtime::Description(SRC, runtime::Variant::MATRIX);
                    m_srcDescription->setTitle(L_("Source"));
                    inputs.push_back(m_srcDescription);
                    
                }
                break;
            }
            
            return inputs;
        }
        
        const std::vector<const runtime::Description*> Mean1D::setupOutputs()
        {
            std::vector<const runtime::Description*> outputs;
            
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Description* value = new runtime::Description(VALUE, runtime::Variant::FLOAT_64);
                    value->setTitle(L_("Value"));
                    outputs.push_back(value);
                    
                }
                break;
            }
            
            return outputs;
        }
        
        void Mean1D::initialize()
        {
            runtime::OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        void Mean1D::execute(runtime::DataProvider & provider)
        {
            switch(int(m_dataFlow))
            {
            case(ALLOCATE):
                {
                    runtime::Id2DataPair srcInMapper(SRC);
                    
                    provider.receiveInputData(srcInMapper);
                    
                    const runtime::Data* srcData = 0;
                    
                    runtime::ReadAccess srcReadAccess;
                    
                    srcReadAccess = runtime::ReadAccess(srcInMapper.data());
                    srcData = &srcReadAccess.get();
                    
                    if(! srcData->variant().isVariant(m_srcDescription->variant()))
                    {
                        throw runtime::InputError(SRC, *this, "Wrong input data variant.");
                    }
                    
                    const runtime::Matrix* srcCastedData = runtime::data_cast<runtime::Matrix>(srcData);
                    
                    cv::Mat srcCvData = cvsupport::getOpenCvMat(*srcCastedData);
                    double valueCvData;
                    
                    valueCvData = mean1D(srcCvData);
                    
                    runtime::Float64* valueCastedData = new runtime::Float64(valueCvData);
                    runtime::DataContainer valueOutContainer = runtime::DataContainer(valueCastedData);
                    runtime::Id2DataPair valueOutMapper(VALUE, valueOutContainer);
                    
                    provider.sendOutputData(valueOutMapper);
                }
                break;
            }
        }
        
    } // cvcore
} // stromx

