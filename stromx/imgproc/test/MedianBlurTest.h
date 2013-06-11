#ifndef STROMX_IMGPROC_MEDIANBLURTEST_H
#define STROMX_IMGPROC_MEDIANBLURTEST_H

#include "stromx/imgproc/Config.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "stromx/runtime/OperatorTester.h"

namespace stromx
{
    namespace imgproc
    {
        class MedianBlurTest : public CPPUNIT_NS::TestFixture
        {
            CPPUNIT_TEST_SUITE(MedianBlurTest);
            CPPUNIT_TEST(testManual0);
            CPPUNIT_TEST(testManual1);
            CPPUNIT_TEST(testAllocate0);
            CPPUNIT_TEST(testInPlace0);
            CPPUNIT_TEST_SUITE_END();
            
        public:
            MedianBlurTest() : m_operator(0) {}
            void setUp();
            void tearDown();
            
        protected:
            void testManual0();
            void testManual1();
            void testAllocate0();
            void testInPlace0();
            
        private:
            runtime::OperatorTester* m_operator;
        };
    }
}

#endif // STROMX_IMGPROC_MEDIANBLURTEST_H