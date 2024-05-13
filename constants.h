#pragma once

#include <string>

const char kOpenBracket = '(';
const char kCloseBracket = ')';
const char kQuoteChar = '\'';
const char kDot = '.';
const std::string kTrue = "#t";
const std::string kFalse = "#f";

const std::string kConstantRegex = "^[+-]?[0-9]+$";
const std::string kSymbolRegex = "(^[+-]$)|(^[a-zA-Z<=>*/#][a-zA-Z<=>*/#0-9?!-]*$)";

// error text
const std::string kDotPositionHelp = "Dot must locate before last symbol in list";
const std::string kBracketHelp = "Bracket sequence must be correct";

const std::string kMustOneArg = " must have only 1 argument";
const std::string kMustTwoMoreArg = " must have 2 or more argumets";
const std::string kMustOneMoreArg = " must have 1 or more argumets";
const std::string kMustTwoArg = " must have 2 argumets";
const std::string kMustTwoThreeArg = " must have 2 or 3 argumets";

const std::string kMustBeNum = " arguments must be numbers";
const std::string kSMustBeNum = " second argument must be a number";
const std::string kMustBeBool = " argumets must be boolean";
const std::string kMustBeList = " arguments must be lists";
const std::string kMustBeEval = " arguments must be evaluatable";
const std::string kMustNotNull = " arguments must not equals ()";

const std::string kOutOfRange = " out of range";
const std::string kZeroDivision = " caught zero division";

// ---- functions ----

//  --- basic ---

//   -- unary --
// - special
const std::string kQuote = "quote";
// - number
const std::string kAbs = "abs";
const std::string kIsNumber = "number?";
// - boolean
const std::string kIsBool = "boolean?";
const std::string kNot = "not";
// - list
const std::string kIsPair = "pair?";
const std::string kIsNull = "null?";
const std::string kIsList = "list?";
const std::string kCar = "car";
const std::string kCdr = "cdr";
// - other
const std::string kIsSymbol = "symbol?";

//   -- binary --
// - number
const std::string kEqual = "=";
const std::string kLess = "<";
const std::string kGreater = ">";
const std::string kLEqual = "<=";
const std::string kGEqual = ">=";
const std::string kPlus = "+";
const std::string kMultiply = "*";
const std::string kMinus = "-";
const std::string kDivide = "/";
const std::string kMax = "max";
const std::string kMin = "min";
// - boolean
const std::string kAnd = "and";
const std::string kOr = "or";
// - list
const std::string kCons = "cons";
const std::string kList = "list";
const std::string kListRef = "list-ref";
const std::string kListTail = "list-tail";

//  --- advanced ---

const std::string kDefine = "define";
const std::string kSet = "set!";
const std::string kIf = "if";
const std::string kSetCar = "set-car!";
const std::string kSetCdr = "set-cdr!";
const std::string kLambda = "lambda";
