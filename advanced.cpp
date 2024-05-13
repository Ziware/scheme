#include "advanced.h"

#include "constants.h"
#include "error.h"
#include "heap.h"
#include "helpers.h"
#include "object.h"
#include "scheme.h"
#include "scope.h"

#include <memory>
#include <string>
#include <vector>

namespace advanced {

using Signature = Object*(Object*, Object*);

Object* FDefine(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kDefine);
    if (args.size() <= 1) {
        throw SyntaxError(kDefine + kMustTwoArg);
    }
    if (Is<Cell>(args[0])) {
        auto name = As<Cell>(args[0])->GetFirst();
        auto arg = As<Cell>(args[0])->GetSecond();

        auto& heap = Heap::GetHeap();

        As<Cell>(args[0])->SetFirst(heap.Make<Symbol>(kLambda));
        As<Cell>(args[0])->SetSecond(heap.Make<Cell>());
        As<Cell>(As<Cell>(args[0])->GetSecond())->SetFirst(arg);
        As<Cell>(As<Cell>(args[0])->GetSecond())->SetSecond(As<Cell>(obj)->GetSecond());

        args[1] = args[0];
        args[0] = name;
        args.resize(2);
    }
    if (args.size() != 2) {
        throw SyntaxError(kDefine + kMustTwoArg);
    }
    if (!Is<Symbol>(args[0])) {
        throw SyntaxError(kDefine + " first argument must be a sybmol or lambda prototype");
    }
    As<Scope>(scope)->AddObject(As<Symbol>(args[0])->GetName(), Eval(args[1], scope));
    return nullptr;
}

Object* FSet(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kSet);
    if (args.size() != 2) {
        throw SyntaxError(kSet + kMustTwoArg);
    }
    if (!Is<Symbol>(args[0])) {
        throw SyntaxError(kSet + " first argument must be a sybmol");
    }
    As<Scope>(scope)->SetObject(As<Symbol>(args[0])->GetName(), Eval(args[1], scope));
    return nullptr;
}

Object* FIf(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kIf);
    if (args.size() != 2 && args.size() != 3) {
        throw SyntaxError(kIf + kMustTwoThreeArg);
    }
    auto res = Eval(args[0], scope);
    bool branch = true;
    if (Is<Bool>(res)) {
        branch &= As<Bool>(res)->GetValue();
    }
    if (branch) {
        return Eval(args[1], scope);
    }
    if (args.size() == 3) {
        return Eval(args[2], scope);
    }
    return nullptr;
}

Object* FSetCar(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kSetCar);
    if (args.size() != 2) {
        throw SyntaxError(kSetCar + kMustTwoArg);
    }
    auto res = Eval(args[0], scope);
    if (!Is<Cell>(res)) {
        throw SyntaxError(kSetCar + " first argument must hold list or pair");
    }
    As<Cell>(res)->SetFirst(Eval(args[1], scope));
    return nullptr;
}

Object* FSetCdr(Object* obj, Object* scope) {
    auto args = GetProperList(obj, kSetCdr);
    if (args.size() != 2) {
        throw SyntaxError(kSetCdr + kMustTwoArg);
    }
    auto res = Eval(args[0], scope);
    if (!Is<Cell>(res)) {
        throw SyntaxError(kSetCdr + " first argument must hold list or pair");
    }
    As<Cell>(res)->SetSecond(Eval(args[1], scope));
    return nullptr;
}

Object* FLambda(Object* obj, Object* scope) {
    // TODO: pass lambda name there
    if (!Is<Cell>(obj)) {
        throw SyntaxError(kLambda + kMustTwoMoreArg);
    }
    if (As<Cell>(obj)->GetFirst() != nullptr && !Is<Cell>(As<Cell>(obj)->GetFirst())) {
        throw SyntaxError(kLambda + " first argument must be a list");
    }
    auto largs = GetProperList(As<Cell>(obj)->GetFirst());
    for (const auto& arg : largs) {
        if (!Is<Symbol>(arg)) {
            throw SyntaxError(kLambda + " first argument must hold only symbols");
        }
    }
    if (!Is<Cell>(As<Cell>(obj)->GetSecond())) {
        throw SyntaxError(kLambda + " ill format body");
    }
    return Heap::GetHeap().Make<Lambda>(kLambda, As<Cell>(obj)->GetFirst(),
                                        As<Cell>(obj)->GetSecond(), scope);
}

Object* FEvalLambda(const std::vector<Object*>& names, const std::vector<Object*>& args,
                    const std::vector<Object*>& body, Object* lambda_scope, Object* eval_scope) {
    auto& heap = Heap::GetHeap();

    Object* new_scope = heap.Make<Scope>(lambda_scope);
    // prepare new scope
    if (args.size() != names.size()) {
        throw RuntimeError(kLambda + " must have as much arguments as prototype has");
    }
    for (size_t id = 0; id < names.size(); ++id) {
        As<Scope>(new_scope)->AddObject(As<Symbol>(names[id])->GetName(),
                                        Eval(args[id], eval_scope));
    }
    // evaluate
    Object* res;
    for (const auto& elem : body) {
        res = Eval(elem, new_scope);
    }
    return res;
}

}  // namespace advanced