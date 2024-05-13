#pragma once

#include "constants.h"
#include "object.h"
#include "scope.h"
#include "heap.h"

#include <memory>

namespace advanced {

using Signature = Object*(Object*, Object*);

Object* FDefine(Object* obj, Object* scope);

Object* FSet(Object* obj, Object* scope);

Object* FIf(Object* obj, Object* scope);

Object* FSetCar(Object* obj, Object* scope);

Object* FSetCdr(Object* obj, Object* scope);

Object* FLambda(Object* obj, Object* scope);

// obj holds ((arg_names) (args) body ...)
Object* FEvalLambda(const std::vector<Object*>& names, const std::vector<Object*>& args,
                    const std::vector<Object*>& body, Object* lambda_scope, Object* eval_scope);

}  // namespace advanced

const std::vector<std::pair<std::string, Object*>> kAdvancedFunctions = {
    {kDefine, Heap::GetHeap().Make<Reserved>(kDefine, advanced::FDefine)},
    {kSet, Heap::GetHeap().Make<Reserved>(kSet, advanced::FSet)},
    {kIf, Heap::GetHeap().Make<Reserved>(kIf, advanced::FIf)},
    {kSetCar, Heap::GetHeap().Make<Reserved>(kSetCar, advanced::FSetCar)},
    {kSetCdr, Heap::GetHeap().Make<Reserved>(kSetCdr, advanced::FSetCdr)},
    {kLambda, Heap::GetHeap().Make<Reserved>(kLambda, advanced::FLambda)},
};