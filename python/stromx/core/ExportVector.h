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

#include <boost/python.hpp>
#include <vector>

using namespace boost::python;

namespace stromx
{
    namespace python
    {
        template <class T>
        typename std::vector<T>::reference getItem(std::vector<T> v, typename std::vector<T>::size_type i)
        {
            return v.at(i);
        }
        
        template <class T>
        void exportVector(const char* const name)
        {
            class_< std::vector<T> >(name, no_init)
                .def("__len__", &std::vector<T>::size)
                .def("__getitem__", &getItem<T>, return_value_policy<copy_non_const_reference>())
            ;
        }
    }
}
