#include "basics.h"

#include "constants.h"
#include "error.h"
#include "helpers.h"
#include "object.h"
#include "scheme.h"
#include "heap.h"

#include <algorithm>
#include <cstdint>
#include <memory>

namespace basics {

using Signature = Object*(Object*, Object*);

// ---- functions ----

//   -- unary --
Object* FBoolFunctor(Object* obj, std::function<bool(Object*)> func, const std::string& context,
                     Object* scope) {
    auto args = GetProperList(obj, context);
    if (args.size() != 1) {
        throw RuntimeError(context + kMustOneArg);
    }
    return Heap::GetHeap().Make<Bool>(func(Eval(args[0], scope)));
}

// - special
Object* FQuote(Object* obj, Object*) {
    auto args = GetProperList(obj, kQuote);
    if (args.size() != 1) {
        throw RuntimeError(kQuote + kMustOneArg);
    }
    return args[0];
}

// - integer
Object* FAbs(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kAbs);
    if (args.size() != 1) {
        throw RuntimeError(kAbs + kMustOneArg);
    }
    auto res = Eval(args[0], scope);
    if (!Is<Number>(res)) {
        throw RuntimeError(kAbs + kMustBeNum);
    }
    return Heap::GetHeap().Make<Number>(std::abs(As<Number>(res)->GetValue()));
}

Object* FIsNumber(Object* obj, Object* scope) {
    return FBoolFunctor(
        obj, [](Object* obj) { return Is<Number>(obj); }, kIsNumber, scope);
}

// - boolean
Object* FIsBool(Object* obj, Object* scope) {
    return FBoolFunctor(
        obj, [](Object* obj) { return Is<Bool>(obj); }, kIsBool, scope);
}

Object* FNot(Object* obj, Object* scope) {
    return FBoolFunctor(
        obj,
        [](Object* obj) {
            bool res = false;
            if (Is<Bool>(obj)) {
                res = (res || !As<Bool>(obj)->GetValue());
            }
            return res;
        },
        kNot, scope);
}

// - list
Object* FIsPair(Object* obj, Object* scope) {
    return FBoolFunctor(
        obj, [](Object* obj) { return CheckPair(obj); }, kIsPair, scope);
}

Object* FIsNull(Object* obj, Object* scope) {
    return FBoolFunctor(
        obj, [](Object* obj) { return CheckNull(obj); }, kIsNull, scope);
}

Object* FIsList(Object* obj, Object* scope) {
    return FBoolFunctor(
        obj, [](Object* obj) { return CheckProperList(obj); }, kIsList, scope);
}

Object* FCar(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kCar);
    if (args.size() != 1) {
        throw RuntimeError(kCar + kMustOneArg);
    }
    auto res = Eval(args[0], scope);
    if (!Is<Cell>(res)) {
        throw RuntimeError(kCar + kMustBeList);
    }
    if (CheckNull(res)) {
        throw RuntimeError(kCar + kMustNotNull);
    }
    return As<Cell>(res)->GetFirst();
}

Object* FCdr(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kCdr);
    if (args.size() != 1) {
        throw RuntimeError(kCdr + kMustOneArg);
    }
    auto res = Eval(args[0], scope);
    if (!Is<Cell>(res)) {
        throw RuntimeError(kCdr + kMustBeList);
    }
    if (CheckNull(res)) {
        throw RuntimeError(kCdr + kMustNotNull);
    }
    return As<Cell>(res)->GetSecond();
}

// - other
Object* FIsSymbol(Object* obj, Object* scope) {
    return FBoolFunctor(
        obj, [](Object* obj) { return Is<Symbol>(obj); }, kIsSymbol, scope);
}

//   -- binary --

// - number
Object* FMonotone(Object* obj, std::function<bool(int64_t, int64_t)> comp,
                  const std::string& context, Object* scope) {
    auto args = GetProperList(obj, context);
    if (args.empty()) {
        return Heap::GetHeap().Make<Bool>(true);
    }
    auto last = Eval(args[0], scope);
    if (!Is<Number>(last)) {
        throw RuntimeError(context + kMustBeNum);
    }
    for (size_t id = 1; id < args.size(); ++id) {
        auto cur = Eval(args[id], scope);
        if (!Is<Number>(cur)) {
            throw RuntimeError(context + kMustBeNum);
        }
        if (!comp(As<Number>(last)->GetValue(), As<Number>(cur)->GetValue())) {
            return Heap::GetHeap().Make<Bool>(false);
        }
        last = cur;
    }
    return Heap::GetHeap().Make<Bool>(true);
}

Object* FBaseFunc(Object* obj, std::function<int64_t(int64_t, int64_t)> func, int64_t base,
                  const std::string& context, Object* scope) {
    auto args = GetProperList(obj, context);
    int64_t last = base;
    for (size_t id = 0; id < args.size(); ++id) {
        auto tmp = Eval(args[id], scope);
        if (!Is<Number>(tmp)) {
            throw RuntimeError(context + kMustBeNum);
        }
        last = func(last, As<Number>(tmp)->GetValue());
    }
    return Heap::GetHeap().Make<Number>(last);
}

Object* FAtLeastOne(Object* obj, std::function<int64_t(int64_t, int64_t)> func,
                    const std::string& context, Object* scope) {
    auto args = GetProperList(obj, context);
    if (args.empty()) {
        throw RuntimeError(context + kMustOneMoreArg);
    }
    auto cur = Eval(args[0], scope);
    if (!Is<Number>(cur)) {
        throw RuntimeError(context + kMustBeNum);
    }
    int64_t last = As<Number>(cur)->GetValue();
    if (context == kMinus && args.size() == 1) {
        return Heap::GetHeap().Make<Number>(-last);
    }
    for (size_t id = 1; id < args.size(); ++id) {
        auto tmp = Eval(args[id], scope);
        if (!Is<Number>(tmp)) {
            throw RuntimeError(context + kMustBeNum);
        }
        last = func(last, As<Number>(tmp)->GetValue());
    }
    return Heap::GetHeap().Make<Number>(last);
}

Object* FEqual(Object* obj, Object* scope) {
    return FMonotone(
        obj, [](int64_t a, int64_t b) { return a == b; }, kEqual, scope);
}

Object* FLess(Object* obj, Object* scope) {
    return FMonotone(
        obj, [](int64_t a, int64_t b) { return a < b; }, kLess, scope);
}

Object* FGreater(Object* obj, Object* scope) {
    return FMonotone(
        obj, [](int64_t a, int64_t b) { return a > b; }, kGreater, scope);
}

Object* FLEqual(Object* obj, Object* scope) {
    return FMonotone(
        obj, [](int64_t a, int64_t b) { return a <= b; }, kLEqual, scope);
}

Object* FGEqual(Object* obj, Object* scope) {
    return FMonotone(
        obj, [](int64_t a, int64_t b) { return a >= b; }, kGEqual, scope);
}

Object* FPlus(Object* obj, Object* scope) {
    return FBaseFunc(
        obj, [](int64_t a, int64_t b) { return a + b; }, 0, kPlus, scope);
}

Object* FMultiply(Object* obj, Object* scope) {
    return FBaseFunc(
        obj, [](int64_t a, int64_t b) { return a * b; }, 1, kMultiply, scope);
}

Object* FMinus(Object* obj, Object* scope) {
    return FAtLeastOne(
        obj, [](int64_t a, int64_t b) { return a - b; }, kMinus, scope);
}

Object* FDivide(Object* obj, Object* scope) {
    return FAtLeastOne(
        obj,
        [](int64_t a, int64_t b) {
            if (b == 0) {
                throw RuntimeError(kDivide + kZeroDivision);
            }
            return a / b;
        },
        kDivide, scope);
}

Object* FMax(Object* obj, Object* scope) {
    return FAtLeastOne(
        obj, [](int64_t a, int64_t b) { return std::max(a, b); }, kMax, scope);
}

Object* FMin(Object* obj, Object* scope) {
    return FAtLeastOne(
        obj, [](int64_t a, int64_t b) { return std::min(a, b); }, kMin, scope);
}

// - boolean
Object* FBoolOp(Object* obj, std::function<bool(bool, bool)> func, bool base,
                const std::string& context, Object* scope) {
    auto args = GetProperList(obj, context);
    Object* last = Heap::GetHeap().Make<Bool>(base);
    for (size_t id = 0; id < args.size(); ++id) {
        last = Eval(args[id], scope);
        bool cur = base;
        if (Is<Bool>(last)) {
            cur = func(cur, As<Bool>(last)->GetValue());
        }
        if (func(base, cur) != base) {
            break;
        }
    }
    return last;
}

Object* FAnd(Object* obj, Object* scope) {
    return FBoolOp(
        obj, [](bool a, bool b) { return (a && b); }, true, kAnd, scope);
}

Object* FOr(Object* obj, Object* scope) {
    return FBoolOp(
        obj, [](bool a, bool b) { return (a || b); }, false, kOr, scope);
}

// - list
Object* FCons(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kCons);
    if (args.size() != 2) {
        throw RuntimeError(kCons + kMustTwoArg);
    }
    auto to_retern = Heap::GetHeap().Make<Cell>();
    As<Cell>(to_retern)->SetFirst(Eval(args[0], scope));
    As<Cell>(to_retern)->SetSecond(Eval(args[1], scope));
    return to_retern;
}

Object* FList(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kList);
    if (args.empty()) {
        return nullptr;
    }
    auto to_retern = Heap::GetHeap().Make<Cell>();
    As<Cell>(to_retern)->SetFirst(Eval(args[0], scope));
    auto cur = to_retern;
    for (size_t id = 1; id < args.size(); ++id) {
        As<Cell>(cur)->SetSecond(Heap::GetHeap().Make<Cell>());
        cur = As<Cell>(cur)->GetSecond();
        As<Cell>(cur)->SetFirst(Eval(args[id], scope));
    }
    return to_retern;
}

Object* FListRef(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kListRef);
    if (args.size() != 2) {
        throw RuntimeError(kCons + kMustTwoArg);
    }
    auto list = GetProperList(Eval(args[0], scope), kListRef);
    auto res = Eval(args[1], scope);
    if (!Is<Number>(res)) {
        throw RuntimeError(kListRef + kSMustBeNum);
    }
    int64_t id = As<Number>(res)->GetValue();
    if (id >= static_cast<int64_t>(list.size()) || id < 0) {
        throw RuntimeError(kListRef + kOutOfRange);
    }
    return list[id];
}

Object* FListTail(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kListTail);
    if (args.size() != 2) {
        throw RuntimeError(kListTail + kMustTwoArg);
    }
    auto list = Eval(args[0], scope);
    auto res = Eval(args[1], scope);
    if (!Is<Number>(res)) {
        throw RuntimeError(kListTail + kSMustBeNum);
    }
    int64_t id = As<Number>(res)->GetValue();
    if (id < 0) {
        throw RuntimeError(kListTail + kOutOfRange);
    }
    while (id-- > 0) {
        if (!Is<Cell>(list)) {
            throw RuntimeError(kListTail + kOutOfRange);
        }
        list = As<Cell>(list)->GetSecond();
    }
    return list;
}

}  // namespace basics