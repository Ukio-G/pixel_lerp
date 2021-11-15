#include <iostream>

#include <GL/glew.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window.hpp>
#include <src/MutablePixel.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <fstream>

std::vector<MutablePixel> generatePixels(sf::Clock & clock);

int main() {
    sf::ContextSettings settings;

    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(1024, 768), "OpenGL", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);
    ImGui::SFML::Init(window);
    window.setActive(true);
    sf::Clock deltaClock;
    sf::Clock globalClock;
    globalClock.restart();

    glewInit();

    // run the main loop
    bool running = true;
    std::vector<MutablePixel> pixels = generatePixels(globalClock);
    std::ofstream f("test");
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                running = false;
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Disassembly");

        ImGui::Text("Time elapsed %i", globalClock.getElapsedTime().asMilliseconds());

        ImGui::End();
        window.clear();

        // Render shapes

        int i = 0;
        for (auto &item: pixels) {
            item.calculateColor();
            if (i == 0)
            {
                f << item;
                i++;
            }
            window.draw(item);
        }

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}

std::vector<MutablePixel> generatePixels(sf::Clock & clock) {
    std::vector<MutablePixel> pixels;

    pixels.reserve(100);
    std::vector<MutablePixel::t_keyframe> a = {
            { 1500, sf::Color{0, 0, 0}},
            { 4000, sf::Color{255, 0, 0}},
            { 5000, sf::Color{255, 255, 0}},
            { 2000, sf::Color{0, 255, 0}},
            { 8000, sf::Color{0, 255, 255}},
            { 2100, sf::Color{255, 255, 255}},
    };


    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            auto pos = sf::Vector2f ((float)i * 20, (float)j * 30);
            auto color = sf::Color(i * 25, j * 25, (i + j) * 2);
            pixels.emplace_back(pos, 5.f, color, &clock);
            pixels.back().addKeyframes(a);
        }
    }

    return pixels;
}