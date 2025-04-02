#ifndef VERTICES_H_
#define VERTICES_H_

#include "common.h"

// clang-format off
vertex_t vertices[] = {
    // Front face (Z+) - Red
    { { { 0.5f, -0.5f,  0.5f} }, {{ 1.0f, 0.0f, 0.0f, 1.0f }}}, // top right
    { { { 0.5f,  0.5f,  0.5f} }, {{ 1.0f, 0.0f, 0.0f, 1.0f }}}, // bottom right
    { { {-0.5f,  0.5f,  0.5f} }, {{ 1.0f, 0.0f, 0.0f, 1.0f }}}, // bottom left

    { { { 0.5f, -0.5f,  0.5f} }, {{ 1.0f, 0.0f, 0.0f, 1.0f }}}, // top right
    { { {-0.5f,  0.5f,  0.5f} }, {{ 1.0f, 0.0f, 0.0f, 1.0f }}}, // bottom left
    { { {-0.5f, -0.5f,  0.5f} }, {{ 1.0f, 0.0f, 0.0f, 1.0f }}}, // top left

    // Back face (Z-) - Cyan
    { { {-0.5f, -0.5f, -0.5f} }, {{ 0.0f, 1.0f, 1.0f, 1.0f }}}, // top left
    { { {-0.5f,  0.5f, -0.5f} }, {{ 0.0f, 1.0f, 1.0f, 1.0f }}}, // bottom left
    { { { 0.5f,  0.5f, -0.5f} }, {{ 0.0f, 1.0f, 1.0f, 1.0f }}}, // bottom right

    { { {-0.5f, -0.5f, -0.5f} }, {{ 0.0f, 1.0f, 1.0f, 1.0f }}}, // top left
    { { { 0.5f,  0.5f, -0.5f} }, {{ 0.0f, 1.0f, 1.0f, 1.0f }}}, // bottom right
    { { { 0.5f, -0.5f, -0.5f} }, {{ 0.0f, 1.0f, 1.0f, 1.0f }}}, // top right

    // Top face (Y-) - Green (in Y-down, top is Y-)
    { { {-0.5f, -0.5f, -0.5f} }, {{ 0.0f, 1.0f, 0.0f, 1.0f }}}, // back left
    { { { 0.5f, -0.5f, -0.5f} }, {{ 0.0f, 1.0f, 0.0f, 1.0f }}}, // back right
    { { { 0.5f, -0.5f,  0.5f} }, {{ 0.0f, 1.0f, 0.0f, 1.0f }}}, // front right

    { { {-0.5f, -0.5f, -0.5f} }, {{ 0.0f, 1.0f, 0.0f, 1.0f }}}, // back left
    { { { 0.5f, -0.5f,  0.5f} }, {{ 0.0f, 1.0f, 0.0f, 1.0f }}}, // front right
    { { {-0.5f, -0.5f,  0.5f} }, {{ 0.0f, 1.0f, 0.0f, 1.0f }}}, // front left

    // Bottom face (Y+) - Magenta (in Y-down, bottom is Y+)
    { { { 0.5f,  0.5f, -0.5f} }, {{ 1.0f, 0.0f, 1.0f, 1.0f }}}, // back right
    { { {-0.5f,  0.5f, -0.5f} }, {{ 1.0f, 0.0f, 1.0f, 1.0f }}}, // back left
    { { {-0.5f,  0.5f,  0.5f} }, {{ 1.0f, 0.0f, 1.0f, 1.0f }}}, // front left

    { { { 0.5f,  0.5f, -0.5f} }, {{ 1.0f, 0.0f, 1.0f, 1.0f }}}, // back right
    { { {-0.5f,  0.5f,  0.5f} }, {{ 1.0f, 0.0f, 1.0f, 1.0f }}}, // front left
    { { { 0.5f,  0.5f,  0.5f} }, {{ 1.0f, 0.0f, 1.0f, 1.0f }}}, // front right

    // Right face (X+) - Yellow
    { { { 0.5f, -0.5f, -0.5f} }, {{ 1.0f, 1.0f, 0.0f, 1.0f }}}, // top back
    { { { 0.5f,  0.5f, -0.5f} }, {{ 1.0f, 1.0f, 0.0f, 1.0f }}}, // bottom back
    { { { 0.5f,  0.5f,  0.5f} }, {{ 1.0f, 1.0f, 0.0f, 1.0f }}}, // bottom front

    { { { 0.5f, -0.5f, -0.5f} }, {{ 1.0f, 1.0f, 0.0f, 1.0f }}}, // top back
    { { { 0.5f,  0.5f,  0.5f} }, {{ 1.0f, 1.0f, 0.0f, 1.0f }}}, // bottom front
    { { { 0.5f, -0.5f,  0.5f} }, {{ 1.0f, 1.0f, 0.0f, 1.0f }}}, // top front

    // Left face (X-) - Blue
    { { {-0.5f,  0.5f, -0.5f} }, {{ 0.0f, 0.0f, 1.0f, 1.0f }}}, // bottom back
    { { {-0.5f, -0.5f, -0.5f} }, {{ 0.0f, 0.0f, 1.0f, 1.0f }}}, // top back
    { { {-0.5f, -0.5f,  0.5f} }, {{ 0.0f, 0.0f, 1.0f, 1.0f }}}, // top front

    { { {-0.5f,  0.5f, -0.5f} }, {{ 0.0f, 0.0f, 1.0f, 1.0f }}}, // bottom back
    { { {-0.5f, -0.5f,  0.5f} }, {{ 0.0f, 0.0f, 1.0f, 1.0f }}}, // top front
    { { {-0.5f,  0.5f,  0.5f} }, {{ 0.0f, 0.0f, 1.0f, 1.0f }}}, // bottom front
};
// clang-format on
#endif // VERTICES_H_
