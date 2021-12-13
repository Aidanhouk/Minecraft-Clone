#include "Seed.h"

#include "Util/Random.h"

// Seed affects only terrain now
int g_Seed = RandomSingleton::get().intInRange(424, 325322);
//const int g_Seed = 20000;