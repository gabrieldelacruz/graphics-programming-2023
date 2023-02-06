#pragma once

#include <glad/glad.h>
#include <span>

class Color
{
public:
    Color() : Color(0.0f, 0.0f, 0.0f, 1.0f) {}
    Color(float red, float green, float blue, float alpha);

    inline float GetRed() const { return m_red; };
    inline void SetRed(float red) { m_red = red; };

    inline float GetGreen() const { return m_green; };
    inline void SetGreen(float green) { m_green = green; };

    inline float GetBlue() const { return m_blue; };
    inline void SetBlue(float blue) { m_blue = blue; };

    inline float GetAlpha() const { return m_alpha; };
    inline void SetAlpha(float alpha) { m_alpha = alpha; };

private:
    // Red color component
    float m_red;

    // Green color component
    float m_green;

    // Blue color component
    float m_blue;

    // Alpha component
    float m_alpha;
};
