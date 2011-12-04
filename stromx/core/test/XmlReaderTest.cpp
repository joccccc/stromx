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

#include <cppunit/TestAssert.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Stream.h>
#include <stromx/core/XmlReader.h>
#include "TestOperator.h"
#include "XmlReaderTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::XmlReaderTest);

namespace stromx
{
    namespace core
    {
        void XmlReaderTest::setUp()
        {
            m_factory = new Factory;
            m_factory->registerOperator(new TestOperator());
            m_factory->registerData(new UInt32());
        }
        
        void XmlReaderTest::testRead()
        {
            Stream* stream = XmlReader().read("stream.xml", *m_factory);
            delete stream;
        }
        
        void XmlReaderTest::testReadWrongFile()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().read("wrongfile.xml", *m_factory), FileAccessFailed);
        }
        
        void XmlReaderTest::testReadInvalidFile()
        {
            CPPUNIT_ASSERT_THROW(XmlReader().read("invalid.xml", *m_factory), FileAccessFailed);
        }

        void XmlReaderTest::tearDown()
        {
            delete m_factory;
        }
    }
}
