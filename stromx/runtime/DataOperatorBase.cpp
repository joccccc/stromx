/* 
*  Copyright 2015 Matthias Fuchs
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

#include "stromx/runtime/DataOperatorBase.h"

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/EnumParameter.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/String.h"
#include "stromx/runtime/TriggerData.h"
#include "stromx/runtime/Variant.h"

#include <boost/assert.hpp>

namespace stromx
{
    using namespace runtime;

    namespace runtime
    {
        namespace
        {
            VariantHandle typeToVariant(const unsigned int type)
            {
                if (type == Variant::BOOL.id())
                    return Variant::BOOL;
                
                if (type == Variant::TRIGGER.id())
                    return Variant::TRIGGER;
                
                if (type == Variant::INT_32.id())
                    return Variant::INT_32;
                
                if (type == Variant::UINT_32.id())
                    return Variant::UINT_32;
                
                if (type == Variant::STRING.id())
                    return Variant::STRING;
                
                if (type == Variant::FLOAT_32.id())
                    return Variant::FLOAT_32;
                
                if (type == Variant::FLOAT_32_MATRIX.id())
                    return Variant::FLOAT_32_MATRIX;
                    
                throw InternalError("Unhandled data type");
            }
        }
        
        DataOperatorBase::DataOperatorBase(const std::string & type,
            const std::string & package, const Version & version,
            const std::vector<const Parameter*>& parameters)
          : OperatorKernel(type, package, version, parameters),
            m_type(Variant::BOOL.id()),
            m_value(new Bool())
        {
        }
        
        DataOperatorBase::~DataOperatorBase()
        {
            delete m_value;
        }

        void DataOperatorBase::setParameter(unsigned int id, const Data& value)
        {
            try
            {
                switch(id)
                {
                case DATA_TYPE:
                    setDataType(data_cast<Enum>(value));
                    break;
                case VALUE:
                    delete m_value;
                    m_value = value.clone();
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

        const DataRef DataOperatorBase::getParameter(const unsigned int id) const
        {
            switch(id)
            {
            case DATA_TYPE:
                return m_type;
            case VALUE:
                if (! m_value)
                    throw InternalError("Value has not been set");
                return *m_value;
            default:
                throw WrongParameterId(id, *this);
            }
        }  
        
        void DataOperatorBase::initialize()
        {
            OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
        }
        
        const std::vector<const Input*> DataOperatorBase::setupInputs()
        {
            return std::vector<const Input*>();
        }
        
        const std::vector<const Output*> DataOperatorBase::setupOutputs()
        {
            std::vector<const Output*> outputs;
            
            VariantHandle variant = typeToVariant(m_type);
            
            Output* output = new Output(OUTPUT, variant);
            output->setTitle("Output");
            outputs.push_back(output);
            
            return outputs;
        }
        
        const std::vector<const Parameter*> DataOperatorBase::setupInitParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            EnumParameter* type = new EnumParameter(DATA_TYPE);
            type->setTitle("Data type");
            type->setAccessMode(runtime::Parameter::NONE_WRITE);
            type->add(EnumDescription(Enum(Variant::BOOL.id()),
                                      Variant::BOOL.title()));
            type->add(EnumDescription(Enum(Variant::TRIGGER.id()),
                                      Variant::TRIGGER.title()));
            type->add(EnumDescription(Enum(Variant::INT_32.id()),
                                      Variant::INT_32.title()));
            type->add(EnumDescription(Enum(Variant::UINT_32.id()),
                                      Variant::UINT_32.title()));
            type->add(EnumDescription(Enum(Variant::STRING.id()),
                                      Variant::STRING.title()));
            type->add(EnumDescription(Enum(Variant::FLOAT_32.id()),
                                      Variant::FLOAT_32.title()));
            type->add(EnumDescription(Enum(Variant::FLOAT_32_MATRIX.id()),
                                      Variant::FLOAT_32_MATRIX.title()));
            parameters.push_back(type);
                                        
            return parameters;
        }
        
        const std::vector<const Parameter*> DataOperatorBase::setupParameters()
        {
            std::vector<const runtime::Parameter*> parameters;
            
            VariantHandle variant = typeToVariant(m_type);
            
            Parameter* value = new Parameter(VALUE, variant);
            value->setTitle("Value");
            value->setAccessMode(runtime::Parameter::ACTIVATED_WRITE);
            parameters.push_back(value);
                                        
            return parameters;
        }
        
        void DataOperatorBase::setDataType(const Enum& value)
        {
            if (m_type == value)
                return;
            
            // update the type
            m_type = value;
            
            // set the value to a null pointer
            delete m_value;
            m_value = 0;
        } 

        Data* DataOperatorBase::resetValuePtr(Data* const value) 
        {
            Data* oldValue = m_value;
            m_value = value;
            return oldValue;
        }
    } 
}
