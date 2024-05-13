#pragma once

#include <memory>
#include <string>
#include <vector>

#include "constants.h"

const std::string kHelp = "!help";
const std::string kBenchOn = "!benchon";
const std::string kBenchOff = "!benchoff";
const std::string kExit = "!exit";

const std::string kDelim = "─────────────────────\n";
const std::string kHorisotal = "─";
const std::string kVertical = "│";
const std::string kCross = "┼";
const std::string kLeftT = "├";
const std::string kRightT = "┤";
const std::string kUpT = "┬";
const std::string kDownT = "┴";
const std::string kSpace = " ";
const std::string kTab = "    ";


struct Operation {
    Operation();
    Operation(const std::string& name, const std::string& example, const std::string& arg_cnt,
              const std::string& description);

    static const size_t kOpArgs = 4;
    std::vector<std::string> args_;

    bool operator<(const Operation& other);
};

class BeautyHelp {
public:
    BeautyHelp();
    void AddOperation();
    void AddOperation(const std::string& name, const std::string& example, const std::string& arg_cnt,
              const std::string& description);

    std::string GetHelp();
    std::vector<size_t> GetSizes();

private:
    std::vector<Operation> operations_;
    std::vector<size_t> sizes_;
};
