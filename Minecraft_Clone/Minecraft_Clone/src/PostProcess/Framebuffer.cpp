#include "Framebuffer.h"

#include "../RenderSettings.h"

#include <iostream>

FrameBufferObject::FrameBufferObject()
{
    init(g_RenderSettings.resolutionX, g_RenderSettings.resolutionY);
}

FrameBufferObject::FrameBufferObject(int resolutionX, int resolutionY)
{
	if (resolutionX == 0 && resolutionY == 0) {
		init(g_RenderSettings.resolutionX, g_RenderSettings.resolutionY);
		width = g_RenderSettings.resolutionX;
		height = g_RenderSettings.resolutionY;
	}
	else {
		init(resolutionX, resolutionY);
		width = resolutionX;
		height = resolutionY;
	}
}

void FrameBufferObject::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, width, height);
}

GLuint FrameBufferObject::getColorTex()
{
    return m_colTex;
}

void FrameBufferObject::init(int resolutionX, int resolutionY)
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);


    //Render texture
    glGenTextures(1, &m_colTex);
    glBindTexture(GL_TEXTURE_2D, m_colTex);
    glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA32F, resolutionX, resolutionY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //Bind render texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colTex, 0);


    glGenTextures(1, &m_depTex);
    glBindTexture(GL_TEXTURE_2D, m_depTex);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, resolutionX, resolutionY, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//glTexImage2D(
	//	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, resolutionX, resolutionY, 0,
	//	GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depTex, 0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferObject::getDepthTex()
{
    return m_depTex;
}

void FrameBufferObject::clear()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}
