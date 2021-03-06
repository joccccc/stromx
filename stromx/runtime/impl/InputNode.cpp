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

#include "stromx/runtime/DataContainer.h"
#include "stromx/runtime/Operator.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/impl/InputNode.h"
#include "stromx/runtime/impl/OutputNode.h"

namespace stromx
{
    namespace runtime
    {
        namespace impl
        {
            InputNode::InputNode(Operator* const op, const unsigned int inputId)
              : m_source(0),
                m_inputId(inputId),
                m_operator(op)
            {

            }

            void InputNode::connect(OutputNode* const output)
            {
                if(m_source)
                    throw WrongOperatorState(m_operator->info(), "Input node has already been connected.");
                
                m_source = output;
                m_source->addConnectedInput(this);
            }
            
            const OutputNode& InputNode::source() const
            {
                if(! m_source)
                    throw WrongOperatorState(m_operator->info(), "Input node is not connected.");
                else
                    return *m_source;
            }
            
            void InputNode::setInputData()
            {
                if(! m_source)
                    throw WrongOperatorState(m_operator->info(), "Input node has not been connected.");
                
                DataContainer inputData = m_source->getOutputData();
                
                try
                {
                    m_operator->setInputData(m_inputId, inputData);
                }
                catch(OperatorError & ex)
                {
                    ex.setName(m_operator->name());
                    throw;
                }
            }
            
            void InputNode::disconnect()
            {
                if(m_source)
                    m_source->removeConnectedInput(this);
                
                m_source = 0;
            }
        }
    }
}