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

#ifndef STREAM_PRIMITIVE_H
#define STREAM_PRIMITIVE_H

#include "Description.h"
#include "Data.h"
#include "Version.h"
#include "Config.h"

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <float.h>

namespace stream
{
    template<typename data_t>
    class NumericParameter;
    
    template <class repr_t, class val_t>
    class Primitive : public Data
    {
    public:
        explicit Primitive() : m_value(0) {}
        explicit Primitive(const repr_t value) : m_value(value) {}
        
        virtual const std::string & name() const { return NAME; }
        virtual const Version & version() const { return VERSION; }
        virtual const std::string & package() const { return PACKAGE; }
        
        virtual const DataType & type() const;
        
        operator repr_t() const { return m_value; }
        
        static const Primitive MIN;
        static const Primitive MAX;
        
    private:
        static const std::string NAME;
        static const std::string PACKAGE;
        static const Version VERSION;
        
        val_t m_value;
    };
    
    typedef Primitive<bool, bool> Bool;
        
    typedef Primitive<int, int8_t> Int8;
    typedef Primitive<unsigned int, uint8_t> UInt8;
        
    typedef Primitive<int, int16_t> Int16;
    typedef Primitive<unsigned int, uint16_t> UInt16;
    
    typedef Primitive<int, int32_t> Int32;
    typedef Primitive<unsigned int, uint32_t> UInt32;
    
    typedef Primitive<double, float> Float;
    typedef Primitive<double, double> Double;
}

#endif // STREAM_PRIMITIVE_H