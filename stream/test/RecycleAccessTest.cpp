#include "RecycleAccessTest.h"

#include "TestData.h"

#include <stream/DataContainer.h>
#include <stream/RecycleAccess.h>
#include <stream/Exception.h>

#include <cppunit/TestAssert.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (stream::RecycleAccessTest);

namespace stream
{
    void RecycleAccessTest::testRecycle()
    {
        Data* data = new TestData();
        
        RecycleAccess access;
        CPPUNIT_ASSERT_EQUAL((Data*)(0), access());
        
        {
            DataContainer container = DataContainer(data);
            access = RecycleAccess(container);
        }
        
        CPPUNIT_ASSERT_EQUAL(data, access());
        CPPUNIT_ASSERT(! TestData::wasDestructed);
        CPPUNIT_ASSERT_EQUAL((Data*)(0), access());
        
        delete data;
    }
    
    void RecycleAccessTest::testCopiedRecycle()
    {
        Data* data = new TestData();
        
        RecycleAccess access;
        
        {
            DataContainer container = DataContainer(data);
            RecycleAccess localAccess = RecycleAccess(container);
            access = localAccess;
        }
        
        CPPUNIT_ASSERT_EQUAL(data, access());
        CPPUNIT_ASSERT(! TestData::wasDestructed);
        CPPUNIT_ASSERT_EQUAL((Data*)(0), access());
        
        delete data;
    }
    
    void RecycleAccessTest::testNoRecycle()
    {
        Data* data = new TestData();
        
        {
            RecycleAccess access;
            DataContainer container = DataContainer(data);
            access = RecycleAccess(container);
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);
    }
    
    void RecycleAccessTest::testDelayedRecycle()
    {
        Data* data = new TestData();
        RecycleAccess access;
        
        {
            DataContainer container = DataContainer(data);
            access = RecycleAccess(container);
            boost::thread t(boost::bind(&RecycleAccessTest::destroyDelayed, this, _1), container);
        }
        
        Data* recycledData = access();
        CPPUNIT_ASSERT_EQUAL(data, recycledData);
    }
    
    void RecycleAccessTest::destroyDelayed(DataContainer& container)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
    
    void RecycleAccessTest::recycleInterrupt(RecycleAccess& access)
    {
        CPPUNIT_ASSERT_THROW(access(), InterruptException);
    }
    
    void RecycleAccessTest::testRecycleInterrupt()
    {
        {
            DataContainer container = DataContainer(new TestData());
            RecycleAccess access(container);
            boost::thread t(boost::bind(&RecycleAccessTest::recycleInterrupt, this, _1), access);
            
            t.interrupt();
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            t.join();
        }
        
        CPPUNIT_ASSERT(TestData::wasDestructed);
    }
    
    void RecycleAccessTest::getAccessInterrupt(DataContainer& container)
    {
        CPPUNIT_ASSERT_THROW(RecycleAccess access(container), InterruptException);
    }
    
    void RecycleAccessTest::releaseAccessDelayed(RecycleAccess& access)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
    
    void RecycleAccessTest::testGetRecycleAccessDelayed()
    {
        DataContainer container = DataContainer(new TestData());
        {
            RecycleAccess access(container);
            boost::thread t(boost::bind(&RecycleAccessTest::releaseAccessDelayed, this, _1), access);
        }
        
        RecycleAccess access(container);
    }
    
    void RecycleAccessTest::testGetRecycleAccessInterrupt()
    {
        DataContainer container = DataContainer(new TestData());
        RecycleAccess access(container);
        
        boost::thread t(boost::bind(&RecycleAccessTest::getAccessInterrupt, this, _1), container);
        t.interrupt();
        t.join();
    }
    
    void RecycleAccessTest::testAdd()
    {
        Data* data = new TestData();
        
        RecycleAccess access;
        CPPUNIT_ASSERT_EQUAL((Data*)(0), access());
        
        {
            DataContainer container = DataContainer(data);
            access.add(container);
        }
        
        CPPUNIT_ASSERT_EQUAL(data, access());
        CPPUNIT_ASSERT(! TestData::wasDestructed);
        CPPUNIT_ASSERT_EQUAL((Data*)(0), access());
        
        delete data;
    }
    
    void RecycleAccessTest::testRecycleMultiple()
    {
        Data* data1 = new TestData();
        Data* data2 = new TestData();
        
        RecycleAccess access;
        {
            DataContainer container1 = DataContainer(data1);
            DataContainer container2 = DataContainer(data2);
            
            access.add(container1);
            access.add(container2);
        }

        CPPUNIT_ASSERT_EQUAL(data2, access());
        CPPUNIT_ASSERT_EQUAL(data1, access());
        CPPUNIT_ASSERT_EQUAL((Data*)(0), access());
        
        delete data1;
        delete data2;
    }
}