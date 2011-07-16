/* 
*  Copyright 2011 XXX
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

#ifndef STREAM_XMLREADER_H
#define STREAM_XMLREADER_H

#include <string>

namespace stream
{
    class OperatorFactory;
    class Stream;
    
    class XmlReader
    {
    public:
        XmlReader(OperatorFactory* const factory)
          : m_factory(factory)
        {}
        
        Stream* const read(const std::string & filename);        
             
        
    private:
        OperatorFactory* m_factory;
    };
}

#endif // STREAM_XMLREADER_H