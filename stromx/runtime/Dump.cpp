/* 
 *  Copyright 2011 Matthias Fuchs
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

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Dump.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/Variant.h"

namespace stromx
{
    namespace runtime
    {
        const std::string Dump::TYPE("Dump");
        const std::string Dump::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Dump::VERSION(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        Dump::Dump()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(),
                           setupOutputs(), setupParameters(), setupProperties())
        {
        }
        void Dump::setParameter(unsigned int id, const runtime::Data&)
        {
            throw WrongParameterId(id, *this);
        }

        const DataRef Dump::getParameter(const unsigned int id) const
        {
            throw WrongParameterId(id, *this);
        }  
        
        void Dump::execute(DataProvider& provider)
        {
            Id2DataPair input(INPUT);
            
            provider.receiveInputData(input);
        }
        
        const std::vector<const Input*> Dump::setupInputs()
        {
            std::vector<const Input*> inputs;
            Input* input = new Input(INPUT, Variant::DATA);
            input->setTitle("Input");
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Output*> Dump::setupOutputs()
        {
            return std::vector<const Output*>();
        }
        
        const std::vector<const Parameter*> Dump::setupParameters()
        {
            return std::vector<const Parameter*>();
        }
        
        const OperatorProperties Dump::setupProperties()
        {
            OperatorProperties properties;
            properties.isGreedy = true;
            return properties;
        }
    }
}
