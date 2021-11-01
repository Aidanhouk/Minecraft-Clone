#pragma once

float bilinearInterpolation(float bottomLeft, float topLeft, float bottomRight,
                            float topRight, float xMin, float xMax, float zMin,
                            float zMax, float xToCalc, float yToCalc);

float smoothInterpolation(float bottomLeft, float topLeft, float bottomRight,
                          float topRight, float xMin, float xMax, float zMin,
                          float zMax, float x, float z);