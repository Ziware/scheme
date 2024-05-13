#pragma once

#include "constants.h"
#include "object.h"
#include "heap.h"

#include <functional>
#include <map>
#include <memory>

namespace basics {

using Signature = Object*(Object*, Object*);

// ---- functions ----

//   -- unary --
Object* FBoolFunctor(Object* obj, std::function<bool(Object*)> func, const std::string& context,
                     Object* scope);

// - special
Object* FQuote(Object* obj, Object* scope);

// - integer
Object* FAbs(Object* obj, Object* scope);

Object* FIsNumber(Object* obj, Object* scope);

// - boolean
Object* FIsBool(Object* obj, Object* scope);

Object* FNot(Object* obj, Object* scope);

// - list
Object* FIsPair(Object* obj, Object* scope);

Object* FIsNull(Object* obj, Object* scope);

Object* FIsList(Object* obj, Object* scope);

Object* FCar(Object* obj, Object* scope);

Object* FCdr(Object* obj, Object* scope);

// - other

Object* FIsSymbol(Object* obj, Object* scope);

//   -- binary --

// - number
Object* FMonotone(Object* obj, std::function<bool(int64_t, int64_t)> comp,
                  const std::string& context, Object* scope);

Object* FBaseFunc(Object* obj, std::function<int64_t(int64_t, int64_t)> func, int64_t base,
                  const std::string& context, Object* scope);

Object* FAtLeastOne(Object* obj, std::function<int64_t(int64_t, int64_t)> func,
                    const std::string& context, Object* scope);

Object* FEqual(Object* obj, Object* scope);

Object* FLess(Object* obj, Object* scope);

Object* FGreater(Object* obj, Object* scope);

Object* FLEqual(Object* obj, Object* scope);

Object* FGEqual(Object* obj, Object* scope);

Object* FPlus(Object* obj, Object* scope);

Object* FMultiply(Object* obj, Object* scope);

Object* FMinus(Object* obj, Object* scope);

Object* FDivide(Object* obj, Object* scope);

Object* FMax(Object* obj, Object* scope);

Object* FMin(Object* obj, Object* scope);

// - boolean
Object* FBoolOp(Object* obj, std::function<bool(bool, bool)> func, bool base,
                const std::string& context, Object* scope);

Object* FAnd(Object* obj, Object* scope);

Object* FOr(Object* obj, Object* scope);

// - list
Object* FCons(Object* obj, Object* scope);

Object* FList(Object* obj, Object* scope);

Object* FListRef(Object* obj, Object* scope);

Object* FListTail(Object* obj, Object* scope);

}  // namespace basics

const std::vector<std::pair<std::string, Object*>> kBasicFunctions = {
    {kQuote, Heap::GetHeap().Make<Reserved>(kQuote, basics::FQuote)},
    {kAbs, Heap::GetHeap().Make<Reserved>(kAbs, basics::FAbs)},
    {kIsNumber, Heap::GetHeap().Make<Reserved>(kIsNumber, basics::FIsNumber)},
    {kIsBool, Heap::GetHeap().Make<Reserved>(kIsBool, basics::FIsBool)},
    {kNot, Heap::GetHeap().Make<Reserved>(kNot, basics::FNot)},
    {kIsPair, Heap::GetHeap().Make<Reserved>(kIsPair, basics::FIsPair)},
    {kIsNull, Heap::GetHeap().Make<Reserved>(kIsNull, basics::FIsNull)},
    {kIsList, Heap::GetHeap().Make<Reserved>(kIsList, basics::FIsList)},
    {kCar, Heap::GetHeap().Make<Reserved>(kCar, basics::FCar)},
    {kCdr, Heap::GetHeap().Make<Reserved>(kCdr, basics::FCdr)},
    {kIsSymbol, Heap::GetHeap().Make<Reserved>(kIsSymbol, basics::FIsSymbol)},
    {kEqual, Heap::GetHeap().Make<Reserved>(kEqual, basics::FEqual)},
    {kLess, Heap::GetHeap().Make<Reserved>(kLess, basics::FLess)},
    {kGreater, Heap::GetHeap().Make<Reserved>(kGreater, basics::FGreater)},
    {kLEqual, Heap::GetHeap().Make<Reserved>(kLEqual, basics::FLEqual)},
    {kGEqual, Heap::GetHeap().Make<Reserved>(kGEqual, basics::FGEqual)},
    {kPlus, Heap::GetHeap().Make<Reserved>(kPlus, basics::FPlus)},
    {kMultiply, Heap::GetHeap().Make<Reserved>(kMultiply, basics::FMultiply)},
    {kMinus, Heap::GetHeap().Make<Reserved>(kMinus, basics::FMinus)},
    {kDivide, Heap::GetHeap().Make<Reserved>(kDivide, basics::FDivide)},
    {kMax, Heap::GetHeap().Make<Reserved>(kMax, basics::FMax)},
    {kMin, Heap::GetHeap().Make<Reserved>(kMin, basics::FMin)},
    {kAnd, Heap::GetHeap().Make<Reserved>(kAnd, basics::FAnd)},
    {kOr, Heap::GetHeap().Make<Reserved>(kOr, basics::FOr)},
    {kCons, Heap::GetHeap().Make<Reserved>(kCons, basics::FCons)},
    {kList, Heap::GetHeap().Make<Reserved>(kList, basics::FList)},
    {kListRef, Heap::GetHeap().Make<Reserved>(kListRef, basics::FListRef)},
    {kListTail, Heap::GetHeap().Make<Reserved>(kListTail, basics::FListTail)},
};
