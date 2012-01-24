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

#include "TestData.h"

#include <fstream>

namespace stromx
{
    namespace core
    {
        bool TestData::wasDestructed = false;
        
        const std::string TestData::TYPE = "TestData";
        const std::string TestData::PACKAGE = "TestPackage";
        const Version TestData::VERSION = Version(0, 1, 0);
        
        void TestData::serialize(std::ostream & textData, std::ostream & binData, std::string & ext) const
        {
            binData << m_value;;
            ext = "txt";
        }


        void TestData::deserialize(std::istream & textData, std::istream & binData)
        {
            binData >> m_value;
        }
    }
}