#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>
#include "../Animation.h"
#include "../Crosshair.h"


int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "GameTest", sf::Style::None);

    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    sf::Texture texture("assets/sprites/mainHeroTest.png", false, sf::IntRect({ 0, 0 }, { 32, 32 }));
    sf::Sprite sprite(texture);

    sprite.setPosition({500, 500});

    Crosshair crosshair("assets/sprites/crosshair.png", window);

    Animation idleHeroAnimation("D:/SFML_Test_project/src/sprites/mainHeroTest.png", true);

    Animator animator;

    while(window.isOpen()){
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
                window.close();
            }
        }

        window.clear(sf::Color(100,150,200));// свой цвет RGB
        //window.draw(sprite);
        animator.RunAnimation(idleHeroAnimation, window);
        crosshair.Draw(window);
        window.display();

        
    }

    return 0;
}