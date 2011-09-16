/* 
*  Copyright 2011 Thomas Fidler
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

#ifndef STROM_STROM_H
#define STROM_STROM_H

#include <vector> 
#include <string>

#include "impl/Network.h"

namespace strom
{
    class Thread;
    class Operator;
    class Registry;
    
    class Stream
    {
    public:
        enum Status
        {
            INACTIVE,
            ACTIVE,
            DEACTIVATING,
        };
        
        Stream();
        ~Stream();
        const std::string& name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }
        const Status status() const { return m_status; }
        const std::vector<Operator*>& operators() const { return m_network->operators(); }
        
        
        void addOperator(Operator* const op);
        void removeOperator(Operator* const op);
        const Node source(const Operator* const targetOp, const unsigned int inputId) const;
        
        void connect(Operator* const sourceOp, const unsigned int outputId, 
                     Operator* const targetOp, const unsigned int inputId);
        void disconnect(Operator* const targetOp, const unsigned int inputId);
        
        Thread* const addThread();
        void removeThread(Thread* const thr);
        const std::vector<Thread*> & threads() const;
        
        void start();
        void join();
        void stop();
        
    private:
        std::string m_name; 
        impl::Network* const m_network;
        std::vector<Thread*> m_threads;
        Status m_status;
    };
}
    
extern "C"
{
    void registerStream(strom::Registry* const registry);
}


#endif // STROM_STROM_H