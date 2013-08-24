#ifndef STROMX_CVIMGPROC_CALCHIST1D_H
#define STROMX_CVIMGPROC_CALCHIST1D_H

#include "stromx/cvimgproc/Config.h"
#include <stromx/cvsupport/Matrix.h>
#include <stromx/runtime/Enum.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/MatrixDescription.h>
#include <stromx/runtime/MatrixParameter.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorKernel.h>
#include <stromx/runtime/Primitive.h>

namespace stromx
{
    namespace cvimgproc
    {
        class STROMX_CVIMGPROC_API CalcHist1D : public runtime::OperatorKernel
        {
        public:
            enum DataFlowId
            {
                ALLOCATE
            };
            enum ConnectorId
            {
                SRC,
                DST
            };
            enum ParameterId
            {
                HIST_MIN,
                HIST_SIZE,
                DATA_FLOW,
                HIST_MAX
            };
            CalcHist1D();
            virtual OperatorKernel* clone() const { return new CalcHist1D; }
            virtual void setParameter(const unsigned int id, const runtime::Data& value);
            virtual const runtime::DataRef getParameter(const unsigned int id) const;
            void initialize();
            virtual void execute(runtime::DataProvider& provider);
            
        private:
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            static const std::string TYPE;
            
            const std::vector<const runtime::Parameter*> setupInitParameters();
            const std::vector<const runtime::Parameter*> setupParameters();
            const std::vector<const runtime::Description*> setupInputs();
            const std::vector<const runtime::Description*> setupOutputs();
            
            runtime::Float32 m_histMax;
            runtime::Float32 m_histMin;
            runtime::UInt32 m_histSize;
            runtime::Enum m_dataFlow;
            runtime::NumericParameter<runtime::Float32>* m_histMaxParameter;
            runtime::NumericParameter<runtime::Float32>* m_histMinParameter;
            runtime::NumericParameter<runtime::UInt32>* m_histSizeParameter;
            runtime::EnumParameter* m_dataFlowParameter;
        };
    }
}

#endif // STROMX_CVIMGPROC_CALCHIST1D_H
