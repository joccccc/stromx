#include "stromx/cvimgproc/test/UndistortTest.h"

#include <boost/lexical_cast.hpp>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/ReadAccess.h>
#include "stromx/cvsupport/Image.h"
#include "stromx/cvimgproc/Undistort.h"

CPPUNIT_TEST_SUITE_REGISTRATION (stromx::cvimgproc::UndistortTest);

namespace stromx
{
    namespace cvimgproc
    {
        void UndistortTest::setUp()
        {
            m_operator = new stromx::runtime::OperatorTester(new Undistort);
        }
        
        void UndistortTest::tearDown()
        {
            delete m_operator;
        }
        
        void UndistortTest::testManual0()
        {
            m_operator->setParameter(Undistort::DATA_FLOW, runtime::Enum(Undistort::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg", cvsupport::Image::GRAYSCALE));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            cvsupport::Matrix cameraMatrix("camera_matrix.npy");
            cvsupport::Matrix distCoeffs("dist_coeffs.npy");
            
            m_operator->setInputData(Undistort::SRC, src);
            m_operator->setInputData(Undistort::DST, dst);
            m_operator->setParameter(Undistort::CAMERA_MATRIX, cameraMatrix);
            m_operator->setParameter(Undistort::DIST_COEFFS, distCoeffs);
            
            runtime::DataContainer result = m_operator->getOutputData(Undistort::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("UndistortTest_testManual0.png", access());
        }
        
        void UndistortTest::testManual1()
        {
            m_operator->setParameter(Undistort::DATA_FLOW, runtime::Enum(Undistort::MANUAL));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            runtime::DataContainer dst(new cvsupport::Image(1000000));
            
            m_operator->setInputData(Undistort::SRC, src);
            m_operator->setInputData(Undistort::DST, dst);
            
            runtime::DataContainer result = m_operator->getOutputData(Undistort::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("UndistortTest_testManual1.png", access());
        }
        
        void UndistortTest::testAllocate0()
        {
            m_operator->setParameter(Undistort::DATA_FLOW, runtime::Enum(Undistort::ALLOCATE));
            m_operator->initialize();
            m_operator->activate();
            
            runtime::DataContainer src(new cvsupport::Image("lenna.jpg"));
            cvsupport::Matrix cameraMatrix("camera_matrix.npy");
            cvsupport::Matrix distCoeffs("dist_coeffs.npy");
            
            m_operator->setInputData(Undistort::SRC, src);
            m_operator->setParameter(Undistort::CAMERA_MATRIX, cameraMatrix);
            m_operator->setParameter(Undistort::DIST_COEFFS, distCoeffs);
            
            runtime::DataContainer result = m_operator->getOutputData(Undistort::DST);
            
            runtime::ReadAccess<runtime::Image> access(result);
            cvsupport::Image::save("UndistortTest_testAllocate0.png", access());
        }
        
    } // cvimgproc
} // stromx

