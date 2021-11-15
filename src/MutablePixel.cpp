#include <SFML/Graphics/RenderWindow.hpp>
#include "MutablePixel.hpp"
#include <algorithm>
#include <iostream>

sf::Color operator*(const sf::Color &l, float r) {
    return sf::Color(l.r * r,l.g * r,l.b * r);
}

sf::Color operator*(float r, const sf::Color &l) {

    uint8_t color_r = static_cast<uint8_t>(l.r * r);
    uint8_t color_g = static_cast<uint8_t>(l.g * r);
    uint8_t color_b = static_cast<uint8_t>(l.b * r);

    return sf::Color(color_r,color_g,color_b);
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

sf::Color color_lerp (const sf::Color & a, const sf::Color & b, float f) {
    uint8_t cr = a.r + (b.r - a.r) * f;
    uint8_t cg = a.g + (b.g - a.g) * f;
    uint8_t cb = a.b + (b.b - a.b) * f;
    return sf::Color(cr, cg, cb);
}

sf::Color color_lerp_time(const sf::Color & a, const sf::Color & b, uint32_t dt, uint32_t animation_time) {
    float f = static_cast<float>(dt) / static_cast<float>(animation_time);
    return color_lerp(a, b, f);
}

MutablePixel::MutablePixel(sf::Vector2f position, float radius, sf::Color color, const sf::Clock * timer) : m_position(position), m_radius(radius), m_color(color), m_clock(timer){
    m_circle = sf::CircleShape(m_radius);
    m_circle.setPosition(m_position);
    m_circle.setFillColor(m_color);
    m_timeReference = m_clock->getElapsedTime().asMilliseconds();
}

MutablePixel::~MutablePixel() { }

void MutablePixel::calculateColor() {
    if (m_colorKeyframes.size() < 2)
        return;

    auto now = m_clock->getElapsedTime().asMilliseconds();
    auto dt = now - m_timeReference;

    if (dt > m_colorKeyframes[m_currentColorKeyframeIdx].first) {
        m_timeReference = now;
        dt = 1;
        m_currentColorKeyframeIdx = (m_colorKeyframes.size() - 1 == m_currentColorKeyframeIdx) ? 0 : m_currentColorKeyframeIdx + 1;
    }

    tmp_time = now;
    size_t next_idx = (m_colorKeyframes.size() - 1 == m_currentColorKeyframeIdx) ? 0 : m_currentColorKeyframeIdx + 1;
    auto &next_color = m_colorKeyframes[next_idx].second;
    auto &[key_time, key_color] = m_colorKeyframes[m_currentColorKeyframeIdx];
    m_color = color_lerp_time(key_color, next_color, dt, key_time);
    m_circle.setFillColor(m_color);
}

void MutablePixel::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(m_circle, states);
}

void MutablePixel::addKeyframe(const MutablePixel::t_keyframe &frame) {
    m_colorKeyframes.push_back(frame);
}

void MutablePixel::addKeyframes(std::vector<t_keyframe> &frame) {
    std::copy (frame.begin(), frame.end(), std::back_inserter(m_colorKeyframes));
}

const sf::Color &MutablePixel::getColor() const {
    return m_color;
}

std::ostream & operator<< (std::ostream &lhs, const MutablePixel & pixel)
{
    sf::Color c = pixel.getColor();
    lhs << (int)c.r << '\t' << (int)c.g << '\t' << (int)c.b << '\t' << pixel.tmp_time << '\n';
    return lhs;
}

