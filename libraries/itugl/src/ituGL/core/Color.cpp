#include <ituGL/core/Color.h>

Color::Color(float red, float green, float blue, float alpha)
    : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha)
{
}

Color::Color(const glm::vec3& color) : Color(color.r, color.g, color.b)
{
}

Color::Color(const glm::vec4& color) : Color(color.r, color.g, color.b, color.a)
{
}
