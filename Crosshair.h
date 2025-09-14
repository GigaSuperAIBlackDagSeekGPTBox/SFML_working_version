#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>

class Crosshair{
private:
    sf::Texture _animationTexture;
    sf::Sprite _animationSprite;
public:
    Crosshair(const std::string &filename, sf::RenderWindow &window) : _animationSprite(_animationTexture){

        sf::Texture animationTexture;
        animationTexture.loadFromFile(filename);
        _animationTexture = animationTexture;

        sf::Sprite animationSprite(_animationTexture);
        _animationSprite = animationSprite;
        _animationSprite.scale({2.0f, 2.0f});
    }

    void Draw(sf::RenderWindow &window){
        _animationSprite.setPosition({static_cast<float>(sf::Mouse::getPosition().x), static_cast<float>(sf::Mouse::getPosition().y)});
        window.draw(_animationSprite);
    }
};