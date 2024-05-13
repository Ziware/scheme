#pragma once

#include "object.h"
#include <tokenizer.h>

#include <memory>

Object* Read(Tokenizer* tokenizer);

Object* ReadList(Tokenizer* tokenizer);
