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

#include "Dump.h"
#include "OperatorException.h"
#include "Id2DataPair.h"
#include "DataProvider.h"

namespace stromx
{
    namespace core
    {
        const std::string Dump::TYPE("Dump");
        const std::string Dump::PACKAGE(STROMX_CORE_PACKAGE_NAME);
        const Version Dump::VERSION(STROMX_VERSION_MAJOR, STROMX_VERSION_MINOR, STROMX_VERSION_PATCH);
        
        Dump::Dump()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
        {
        }
        void Dump::setParameter(unsigned int id, const core::Data& value)
        {
            throw WrongParameterId(id, *this);
        }

        const Data& Dump::getParameter(const unsigned int id) const
        {
            throw WrongParameterId(id, *this);
        }  
        
        void Dump::execute(DataProvider& provider)
        {
            Id2DataPair input(INPUT);
            
            provider.receiveInputData(input);
        }
        
        const std::vector<const Description*> Dump::setupInputs()
        {
            std::vector<const Description*> inputs;
            inputs.push_back(new Description(INPUT, DataVariant::DATA));
            
            return inputs;
        }
        
        const std::vector<const Description*> Dump::setupOutputs()
        {
            return std::vector<const Description*>();
        }
        
        const std::vector<const Parameter*> Dump::setupParameters()
        {
            return std::vector<const Parameter*>();
        }
    }
}