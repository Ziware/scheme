#pragma once

#include "object.h"

#include <memory>
#include <vector>

bool CheckProperList(Object* obj);

std::vector<Object*> GetProperList(Object* obj, const std::string context = "");

std::vector<Object*> GetList(Object* obj);

bool CheckPair(Object* obj);

std::pair<Object*, Object*> GetPair(Object* obj);

bool CheckNull(Object* obj);
