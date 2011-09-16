#include "Counter.h"

#include "Config.h"

#include <strom/OperatorException.h>
#include <strom/DataProvider.h>
#include <strom/Id2DataPair.h>

#include <typeinfo>


using namespace strom;

namespace base
{
    const std::string Counter::TYPE("Counter");
    const std::string Counter::PACKAGE(PACKAGE_NAME);
    const Version Counter::VERSION(BASE_VERSION_MAJOR, BASE_VERSION_MINOR);
    
    Counter::Counter()
      : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters()),
        m_counter(0)
    {
    }

    void Counter::setParameter(unsigned int id, const Data& value)
    {
        try
        {
            switch(id)
            {
            default:
                throw WrongParameterId(id, *this);
            }
        }
        catch(std::bad_cast&)
        {
            throw WrongParameterType(*parameters()[id], *this);
        }
    }

    const Data& Counter::getParameter(unsigned int id)
    {
        switch(id)
        {
        default:
            throw WrongParameterId(id, *this);
        }
    } 
    
    void Counter::activate()
    {
        m_counter = 0;
    }
    
    void Counter::execute(DataProvider& provider)
    {
        Data* outData = new UInt32(m_counter);
        
        m_counter++;
        
        Id2DataPair outputDataMapper(OUTPUT, DataContainer(outData));
        provider.sendOutputData(outputDataMapper);
    }
    
    const std::vector<const Description*> Counter::setupInputs()
    {
        std::vector<const Description*> inputs;

        return inputs;
    }
    
    const std::vector<const Description*> Counter::setupOutputs()
    {
        std::vector<const Description*> outputs;
        
        Description* output = new Description(OUTPUT, DataVariant::UINT_32);
        output->setName("Output");
        outputs.push_back(output);
        
        return outputs;
    }
    
    const std::vector<const Parameter*> Counter::setupParameters()
    {
        std::vector<const Parameter*> parameters;
        
        return parameters;
    }
} 