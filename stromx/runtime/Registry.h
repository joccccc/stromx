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

#ifndef STROMX_RUNTIME_REGISTRY_H
#define STROMX_RUNTIME_REGISTRY_H

#include "stromx/runtime/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Data;
        class OperatorKernel;
        
        /** \brief Abstract registry of operator and data objects. */
        class Registry
        {
        public:
            /** Registers an operator kernel with the registry. The registry assumes
             *  ownership of \c op. */
            virtual void registerOperator(const OperatorKernel* const op) = 0; 
            
            /** Registers a data object with the registry. The registry assumes 
             *  ownership of \c data. */
            virtual void registerData(const Data* data) = 0;
        };
    }
}

#endif // STROMX_RUNTIME_REGISTRY_H
