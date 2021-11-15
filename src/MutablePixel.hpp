#ifndef MUTABLEPIXEL_HPP
#define MUTABLEPIXEL_HPP
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <SFML/Graphics/CircleShape.hpp>


/*  Backend  */
class PixelData {
public:
    using t_keyframe = std::pair<uint32_t , sf::Color>;    // Time and color. Time for one color
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    void calculateColor();
    const Color & getColor() const;
    sf::Color m_color = {0, 0, 0};
    std::vector<t_keyframe> m_colorKeyframes;
    std::size_t m_currentColorKeyframeIdx;
    uint32_t m_timeReference;

};


/*  Frontend  */
class MutablePixel : public sf::Drawable {
public:
    using t_keyframe = std::pair<uint32_t , sf::Color>;    // Time and color. Time for one color

    MutablePixel() {};
    MutablePixel(const MutablePixel &other) = default;
    MutablePixel(sf::Vector2f position, float radius, sf::Color color, const sf::Clock * timer);
    MutablePixel &operator=(const MutablePixel &other) = default;
    ~MutablePixel();

    void addKeyframe(const t_keyframe & frame);
    void addKeyframes(std::vector<t_keyframe> & frame);
    void calculateColor();
    const sf::Color & getColor() const;
    uint32_t tmp_time;

private:
    sf::Vector2f m_position;
    float m_radius = 10.f;
    sf::Color m_color = {0, 0, 0};
    std::vector<t_keyframe> m_colorKeyframes;
    std::size_t m_currentColorKeyframeIdx;
    sf::CircleShape m_circle;
    uint32_t m_timeReference;


    const sf::Clock * m_clock;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

std::ostream & operator<< (std::ostream &lhs, const MutablePixel & pixel);

#endif
