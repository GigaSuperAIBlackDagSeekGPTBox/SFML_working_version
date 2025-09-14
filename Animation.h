#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>
#include "MapGenerator.h"

class Animation;

// Проигрывает анимации из класса Animation
// Анимации которые буду проигрываться в аниматоре
class Animation {
public:
    int _imageCount;
    int _currentFrame;
    bool _isLooping;
    sf::Vector2u _imageSize;
    sf::Vector2f _spritePosition;
    sf::Image _animationImage;
    sf::Texture animationTexture;
    sf::Texture _animationTextures;
    std::vector<sf::Sprite> _animationSprites;

public:
    Animation(const std::string& filename, bool isLooping) {
        _isLooping = isLooping;
        _currentFrame = 0;

        if (!_animationImage.loadFromFile(filename)) {
            std::cout << "Файл не загружен" << std::endl;
            throw std::runtime_error("Файл загружен, но размер текстуры 0!");
        }

        _imageSize = { _animationImage.getSize().x, _animationImage.getSize().y };
        _spritePosition = { 960, 540};
        _imageCount = static_cast<int>(_imageSize.x / 32);

        animationTexture.loadFromFile(filename, false);

        for (int i = 0; i < _imageCount; ++i) {
            sf::Vector2i framePosition(i * 32, 0);
            sf::Vector2i frameSize(32, 32);

            sf::IntRect frameRect(framePosition, frameSize);

            sf::Sprite animationSprite(
                animationTexture,
                sf::IntRect({ i * 32, 0 }, { 32, 32 })
            );
            animationSprite.setPosition(_spritePosition);
            animationSprite.scale({ 2.0f, 2.0f });

            _animationSprites.push_back(animationSprite);
        }
    }
};

class Animator {
private:
    sf::Clock clock;
    std::vector<std::vector<sf::Sprite>> entitiesAnimationFrames;
    float _frameTime = 0.125f;

public:
    Animator() {}

    void RunAnimation(Animation& animation, sf::RenderWindow& window) {
        if (clock.getElapsedTime().asSeconds() >= _frameTime) {
            animation._currentFrame = (animation._currentFrame + 1) % animation._imageCount;
            clock.restart();
        }

        window.draw(animation._animationSprites[animation._currentFrame]);
    }

    void DrawMap(sf::RenderWindow &window, MapGenerator &generator, std::vector<std::vector<int>> map){
        for (int y = 0; y < map.size(); y++) {
            for (int x = 0; x < map[y].size(); x++) {
                int tile = map[y][x];

                if (tile == 0) continue; // пусто, не рисуем

                sf::Sprite sprite = generator.getTileSprite(tile);
                sprite.setPosition({x * 64, y * 64});
                window.draw(sprite);
            }
        }
    }
};
