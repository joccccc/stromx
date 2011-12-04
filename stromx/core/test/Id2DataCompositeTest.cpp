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
#include "Id2DataCompositeTest.h"
#include <stromx/core/DataContainer.h>
#include <stromx/core/Exception.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/impl/Id2DataMap.h>
#include <stromx/core/None.h>

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::core::Id2DataCompositeTest);

namespace stromx
{
    namespace core
    {
        void Id2DataCompositeTest::setUp ( void )
        {
            std::vector<const Description*> descriptions;
            descriptions.push_back(new Description(0, DataVariant::NONE));
            descriptions.push_back(new Description(1, DataVariant::NONE));
            descriptions.push_back(new Description(2, DataVariant::NONE));
            m_map = new impl::Id2DataMap(descriptions);
            m_dataContainer = DataContainer(new None());
        }

        void Id2DataCompositeTest::testSet ( void )
        {
            {
                (*m_map)[0] = m_dataContainer;
                Id2DataPair pair0(0, m_dataContainer);
                Id2DataPair pair1(1, m_dataContainer);
                Id2DataPair pair2(2, m_dataContainer);
                
                CPPUNIT_ASSERT_THROW((pair0 && (pair1 || pair2)).set(*m_map), WrongState);
            }          
            
            {
                (*m_map)[0] = m_dataContainer;
                (*m_map)[1] = DataContainer();
                (*m_map)[2] = DataContainer();
                Id2DataPair pair0(0, m_dataContainer);
                Id2DataPair pair1(1, m_dataContainer);
                Id2DataPair pair2(2, m_dataContainer);
                
                CPPUNIT_ASSERT_NO_THROW((pair0 || (pair1 && pair2)).set(*m_map));
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[1]);
                CPPUNIT_ASSERT(pair1.data().empty());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[2]);
                CPPUNIT_ASSERT(pair2.data().empty());
            }         
            
            {
                (*m_map)[0] = DataContainer();
                (*m_map)[1] = m_dataContainer;
                (*m_map)[2] = DataContainer();
                Id2DataPair pair0(0, m_dataContainer);
                Id2DataPair pair1(1, m_dataContainer);
                Id2DataPair pair2(2, m_dataContainer);
                
                CPPUNIT_ASSERT_NO_THROW((pair0 || (pair1 && pair2)).set(*m_map));
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[0]);
                CPPUNIT_ASSERT(pair0.data().empty());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[1]);
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair1.data());
                CPPUNIT_ASSERT((*m_map)[2].empty());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair2.data());
            }
        }
        
        void Id2DataCompositeTest::testTryGet()
        {
            Id2DataPair pair0(0);
            Id2DataPair pair1(1);
            Id2DataPair pair2(2);
            (*m_map)[1] = m_dataContainer;
            (*m_map)[2] = m_dataContainer;
            
            CPPUNIT_ASSERT_EQUAL(false, (pair0 && pair1 && pair2).tryGet(*m_map));
            CPPUNIT_ASSERT_EQUAL(true, ((pair0 && pair1) || pair2).tryGet(*m_map));
            CPPUNIT_ASSERT_EQUAL(false, (pair0 && (pair1 || pair2)).tryGet(*m_map));
        }
        
        void Id2DataCompositeTest::testTrySet()
        {
            Id2DataPair pair0(0, m_dataContainer);
            Id2DataPair pair1(1, m_dataContainer);
            Id2DataPair pair2(2, m_dataContainer);
            (*m_map)[0] = m_dataContainer;
            
            CPPUNIT_ASSERT_EQUAL(false, (pair0 && pair1 && pair2).trySet(*m_map));
            CPPUNIT_ASSERT_EQUAL(true, ((pair0 && pair1) || pair2).trySet(*m_map));
            CPPUNIT_ASSERT_EQUAL(false, (pair0 && (pair1 || pair2)).trySet(*m_map));
        }

        void Id2DataCompositeTest::testGet ( void )
        {
            {
                (*m_map)[1] = m_dataContainer;
                (*m_map)[2] = m_dataContainer;
                Id2DataPair pair0(0);
                Id2DataPair pair1(1);
                Id2DataPair pair2(2);   
                
                CPPUNIT_ASSERT_THROW((pair0 && (pair1 || pair2)).set(*m_map), WrongState);
            }
            
            {
                (*m_map)[0] = DataContainer();
                (*m_map)[1] = m_dataContainer;
                (*m_map)[2] = m_dataContainer;
                Id2DataPair pair0(0);
                Id2DataPair pair1(1);
                Id2DataPair pair2(2);
                
                CPPUNIT_ASSERT_NO_THROW(((pair0 && pair1) || pair2).get(*m_map));
                CPPUNIT_ASSERT((*m_map)[0].empty());
                CPPUNIT_ASSERT(pair0.data().empty());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, (*m_map)[1]);
                CPPUNIT_ASSERT(pair1.data().empty());
                CPPUNIT_ASSERT((*m_map)[2].empty());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair2.data());
            }
            
            {
                (*m_map)[0] = m_dataContainer;
                (*m_map)[1] = m_dataContainer;
                (*m_map)[2] = DataContainer();
                Id2DataPair pair0(0);
                Id2DataPair pair1(1);
                Id2DataPair pair2(2);
                
                CPPUNIT_ASSERT_NO_THROW(((pair0 && pair1) || pair2).get(*m_map));
                CPPUNIT_ASSERT((*m_map)[0].empty());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair0.data());
                CPPUNIT_ASSERT((*m_map)[1].empty());
                CPPUNIT_ASSERT_EQUAL(m_dataContainer, pair1.data());
                CPPUNIT_ASSERT((*m_map)[2].empty());
                CPPUNIT_ASSERT(pair2.data().empty());
            }
        }

        void Id2DataCompositeTest::tearDown ( void )
        {
            delete m_map;
        }
    }
}
