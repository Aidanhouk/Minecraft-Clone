#ifndef POSTPROCESSSHADER_H_INCLUDED
#define POSTPROCESSSHADER_H_INCLUDED

#include "BasicShader.h"

class PostProcessShader : public BasicShader
{
public:
        PostProcessShader();
        
        void loadContrast(float c);
        void loadGamma(float g);
		void loadBrightness(float b);
		void loadSaturation(float b);
		void loadValue(float b);
        void loadResolution(glm::vec2 resolution);
		void loadUnderwater(bool underwater);
		void loadDarkScreen(bool darkScreen);
private:
        void getUniforms() override;

        GLuint m_contrast;
		GLuint m_gamma;
		GLuint m_brightness;
		GLuint m_saturation;
		GLuint m_value;
        GLuint m_resolution;
		GLfloat m_underwater;
		GLfloat m_darkScreen;
};

#endif // CHUNKSHADER_H_INCLUDED
