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

#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/DataVariant.h>
#include <stromx/runtime/Data.h>
#include <stromx/runtime/Image.h>

#include <stromx/imgutil/Matrix.h>

#include <memory>
#include <boost/python.hpp>

using namespace boost::python;
using namespace stromx::runtime;

namespace
{  
    std::auto_ptr<stromx::imgutil::Matrix> allocateFromDimension(const unsigned int rows,
                                                                 const unsigned int cols,
                                                                 const stromx::runtime::Matrix::ValueType valueType)
    {
        return std::auto_ptr<stromx::imgutil::Matrix>(new stromx::imgutil::Matrix(rows, cols, valueType));
    }
    
    std::auto_ptr<stromx::imgutil::Matrix> allocateFromFile(const std::string & filename)
    {
        return std::auto_ptr<stromx::imgutil::Matrix>(new stromx::imgutil::Matrix(filename));
    } 
}

void exportMatrix()
{
    class_<stromx::imgutil::Matrix, bases<stromx::runtime::Matrix>, std::auto_ptr<stromx::imgutil::Matrix> >("Matrix", no_init)
        .def("__init__", make_constructor(&allocateFromFile))
        .def("__init__", make_constructor(&allocateFromDimension))
        .def<void (stromx::imgutil::Matrix::*)(const std::string &) const>("save", &stromx::imgutil::Matrix::save)
        .def<void (stromx::imgutil::Matrix::*)(const std::string &)>("open", &stromx::imgutil::Matrix::open)
        .def<void (stromx::imgutil::Matrix::*)(const unsigned int, const unsigned int, const Matrix::ValueType)>("resize", &stromx::imgutil::Matrix::resize)
     ;
     
    implicitly_convertible< std::auto_ptr<stromx::imgutil::Matrix>, std::auto_ptr<Data> >();
}
