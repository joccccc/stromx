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

#ifndef STROMX_RUNTIME_NUMERICPARAMETER_H
#define STROMX_RUNTIME_NUMERICPARAMETER_H

#include "stromx/runtime/Parameter.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
        
        /** \brief %Description of a numeric parameter */
        template<typename data_t>
        class NumericParameter : public Parameter
        {
        public:
            /** Constructs a numeric parameter. */
            NumericParameter(const unsigned int id, ParameterGroup* const group = 0)
              : Parameter(id, data_traits<data_t>::variant(), group),
                m_min(data_t::MIN),
                m_max(data_t::MAX),
                m_step(data_t(1))
            {}
            
            /** Constructs a numeric parameter. */
            NumericParameter(const unsigned int id, const data_t min, const data_t max)
              : Parameter(id, data_traits<data_t>::variant()),
                m_min(min), 
                m_max(max)
            {}
            
            /** Constructs a numeric parameter. */
            NumericParameter(const unsigned int id, const data_t min, const data_t max,
                             const data_t step)
              : Parameter(id, data_traits<data_t>::variant()),
                m_min(min),
                m_max(max)
            {}
            
            virtual const Data& max() const { return m_max; }
            virtual const Data& min() const { return m_min; }
            virtual const Data& step() const { return m_step; }
            
            /** Sets the maximal value of the parameter. */
            virtual void setMax(const data_t& value)
            {
                m_max = value;
            }
            
            /** Sets the minimal value of the parameter. */
            virtual void setMin(const data_t& value)
            {
                m_min = value;
            }
            
            /** Sets the step value of the parameter. */
            virtual void setStep(const data_t& value)
            {
                m_step = value;
            }
            
        private:
            data_t m_min;
            data_t m_max;
            data_t m_step;
        };
    }
}

#endif // STROMX_RUNTIME_NUMERICPARAMETER_H
