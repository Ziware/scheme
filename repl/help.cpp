#include "help.h"

#include "assertions.h"
#include "constants.h"
#include "error.h"

#include <sys/ioctl.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

// TODO: fix bad description

Operation::Operation() {
    args_.push_back("Name");
    args_.push_back("ArgCnt");
    args_.push_back("Example = Result");
    args_.push_back("Description");
    ASSERT(args_.size() == kOpArgs,
           ("Operations should holds " + std::to_string(kOpArgs) + " fields").data());
}

Operation::Operation(const std::string& name, const std::string& example,
                     const std::string& arg_cnt, const std::string& description) {
    args_.push_back(name);
    args_.push_back(arg_cnt);
    args_.push_back(example);
    args_.push_back(description);
    ASSERT(args_.size() == kOpArgs,
           ("Operations should holds " + std::to_string(kOpArgs) + " fields").data());
}

bool Operation::operator<(const Operation& other) {
    return args_[0] < other.args_[0];
}

void BeautyHelp::AddOperation() {
    operations_.push_back(Operation());
    size_t id = 0;
    for (auto str : operations_.back().args_) {
        sizes_[id] = std::max(sizes_[id], str.size());
        ++id;
    }
}

void BeautyHelp::AddOperation(const std::string& name, const std::string& example,
                              const std::string& arg_cnt, const std::string& description) {
    operations_.push_back(Operation(name, example, arg_cnt, description));
    size_t id = 0;
    for (auto str : operations_.back().args_) {
        sizes_[id] = std::max(sizes_[id], str.size());
        ++id;
    }
}

std::string BeautyHelp::GetHelp() {
    std::string ans;
    ans += kDelim;
    for (auto op : operations_) {
        ans += kTab;
        size_t id = 0;
        for (auto str : op.args_) {
            ans += str;
            if (id + 1 != Operation::kOpArgs) {
                ans += std::string(sizes_[id] - str.size() + 2, ' ');
            }
            ++id;
        }
        ans += '\n';
        ans += kDelim;
    }
    return ans;
}

BeautyHelp::BeautyHelp() {
    sizes_.assign(Operation::kOpArgs, 0);

    AddOperation();
    AddOperation(kQuote, "(quote (1 2 3)) = (1 2 3)", "1",
                 "Returs argument. Short form: (quote 5) = \'5 = 5");
    AddOperation(kAbs, "(abs -2) = 2", "1", "Returns absolute value");
    AddOperation(kIsNumber, "(number? (1 . 2)) = #f", "1",
                 "Returns \'#t\' if argument is number, \'#f\' otherwise");
    AddOperation(kIsBool, "(boolean? #f) = #t", "1",
                 "Returns \'#t\' if argument is boolean, \'#f\' otherwise");
    AddOperation(kNot, "(not #t) = #f", "1", "Returns \'#f\' if \'#t\' given, \'#t\' otherwise");
    AddOperation(kIsPair, "(pair? (1 . 2)) = #t", "1",
                 "Returns \'#t\' if argument is a pair, \'#f\' otherwise");
    AddOperation(kIsNull, "(null? ()) = #t", "1",
                 "Returns \'#t\' if argument is a null (empty list), \'#f\' otherwise");
    AddOperation(kIsList, "(list? (1 2 3 . 4)) = #t", "1",
                 "Returns \'#t\' if argument is a list, \'#f\' otherwise");
    AddOperation(kCar, "(car (1 2 3)) = 1", "1", "Returns first element of list");
    AddOperation(kCdr, "(cdr (1 2 3)) = (2 3)", "1", "Returns list without first element");
    AddOperation(kIsSymbol, "(symbol? abacaba) = #t", "1",
                 "Returns \'#t\' if argument is a symbol, \'#f\' otherwise");
    AddOperation(kEqual, "(= 2 2 (+ 1 1)) = #t", "0+",
                 "Returns \'#t\' if all arguments are equal, \'#f\' otherwise");
    AddOperation(kLess, "(< 1 2 3) = #t", "0+",
                 "Returns \'#t\' if sequence of arguments are increasing, \'#f\' otherwise");
    AddOperation(kGreater, "(> 3 2 1) = #t", "0+",
                 "Returns \'#t\' if sequence of arguments are decreasing, \'#f\' otherwise");
    AddOperation(kLEqual, "(<= 1 1 2) = #t", "0+",
                 "Returns \'#t\' if sequence of arguments are not decreasing, \'#f\' otherwise");
    AddOperation(kGEqual, "(>= 2 1 1) = #t", "0+",
                 "Returns \'#t\' if sequence of arguments are not increasing, \'#f\' otherwise");
    AddOperation(kPlus, "(+ 1 2 3) = 6", "0+",
                 "Returns the sum of the elements of a sequence. Note: (+) = 0");
    AddOperation(kMultiply, "(* 2 3 4) = 24", "0+",
                 "Returns the product of the elements of a sequence. Note: (*) = 1");
    AddOperation(kDivide, "(/ 14 2 2) = 3", "1+",
                 "Returns sequential division of the elements of a sequence. Note: (/ 5) = 5");
    AddOperation(kMinus, "(- 9 7 5) = -3", "2+",
                 "Returns sequential subtraction of the elements of a sequence. Note: (- 3) = -3");
    AddOperation(kMax, "(max 1 2 3) = 3", "1+",
                 "Returns the maximum of the elements of a sequence");
    AddOperation(kMin, "(min 1 2 3) = 1", "1+",
                 "Returns the minimum of the elements of a sequence");
    AddOperation(
        kAnd, "(and #t (= 1 2)) = #f", "0+",
        "Returns value of first false expression of the elements of a sequence. If an expression "
        "is computable, and result is not boolean, then it is implicitly true. Note: (and) = #t");
    AddOperation(
        kOr, "(or #f (= 1 2) (+ 1 1)) = 2", "0+",
        "Returns value of first true expression of the elements of a sequence. If an expression is "
        "computable, and result is not boolean, then it is implicitly true. Note: (or) = #f");
    AddOperation(kCons, "(cons 1 2) = (1 . 2)", "2", "Returns a pair of argument values");
    AddOperation(kList, "(list 1 2 3) = (1 2 3)", "0+", "Returns a list of argument values");
    AddOperation(kListRef, "(list-ref '(1 2 3) 1) = 2", "2", "Returns list value by index");
    AddOperation(kListTail, "(list-tail '(1 2 3) 1) = (3)", "2",
                 "Returns list without second argument value elements");
    AddOperation(kDefine, "(define name 3)", "2",
                 "Defines variable \'name\' equals to expression. In example: name = 3. You can "
                 "shortly define lambda: \'(define (fn-name <args>) <body>)\'");
    AddOperation(kSet, "(set! name 5)", "2",
                 "Sets \'name\' variable equals to expression. In example: name = 5");
    AddOperation(kIf, "(if (< 1 2) (+ 2 2) 10) = 4", "2-3",
                 "Simple if expression: (if (condition) <true-branch> <false-branch>), "
                 "<false-branch> is optional");
    AddOperation(kSetCar, "(set-car! (1 2 3) (2 2)) = ((2 2) 2 3)", "2",
                 "Sets the first element of the list to the value of the second argument");
    AddOperation(kSetCdr, "(set-cdr! (1 2 3) (3 2 1)) = (1 3 2 1)", "2",
                 "Replaces all elemets from the second to the value of the second argument");
    AddOperation(kLambda, "((lambda (x) (+ x 3) (* x 2)) 2) = 10", "2+",
                 "Returns lambda function with arguments and body that you want. Lambda syntax: "
                 "\'(lambda (<args>) <body>)\'");

    std::sort(operations_.begin() + 1, operations_.end());
}
