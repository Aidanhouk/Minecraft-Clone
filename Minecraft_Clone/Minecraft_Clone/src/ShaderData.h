#pragma once

struct ShaderData{
    bool fxaa = false;
    bool bloom = false;
    bool motionblur = false;
    bool aniso = false;
};

extern ShaderData g_ShaderSettings;