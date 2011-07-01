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

#ifndef STREAM_TRIGGER_H
#define STREAM_TRIGGER_H

#include "Data.h"

namespace stream
{
    class Trigger : public Data
    {
    public:
        Trigger() {}
        
        virtual const DataType & type() const { return DataType::TRIGGER; }
    };
}

#endif // STREAM_ENUM_H