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

#include "stromx/runtime/InputProvider.h"
#include "stromx/runtime/OutputProvider.h"
#include "stromx/runtime/String.h"

namespace stromx
{
    namespace runtime
    {
        const std::string String::TYPE = "String";
        const std::string String::PACKAGE = STROMX_RUNTIME_PACKAGE_NAME;
        const Version String::VERSION = Version(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        const DataVariant & String::variant() const 
        {
            return DataVariant::STRING;
        }
        
        void String::serialize(OutputProvider & out) const
        {
            out.text() << m_value;
        }
        
        void String::deserialize(InputProvider & in, const Version &)
        {
            in.text() >> m_value;
        }
        
        bool operator==(const stromx::runtime::String& lhs, const stromx::runtime::String& rhs)
        {
            return lhs.m_value == rhs.m_value;
        }
        
        bool operator!=(const stromx::runtime::String& lhs, const stromx::runtime::String& rhs)
        {
            return lhs.m_value != rhs.m_value;
        }
    }
}

