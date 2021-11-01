#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"

class Singleton : public NonMovable, public NonCopyable {
};