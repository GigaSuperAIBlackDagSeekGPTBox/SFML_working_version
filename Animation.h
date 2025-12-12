#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>
#include "MapGenerator.h"

// Константы для анимации
constexpr int FRAME_SIZE = 32;
constexpr float SPRITE_SCALE = 2.0f;
constexpr float DEFAULT_POSITION_X = 960.0f;
constexpr float DEFAULT_POSITION_Y = 540.0f;

class Animation;

// Проигрывает анимации из класса Animation
// Анимации которые буду проигрываться в аниматоре
class Animation {
private:
    int _imageCount;
    int _currentFrame;
    bool _isLooping;
    sf::Vector2u _imageSize;
    sf::Vector2f _spritePosition;
    sf::Texture _animationTexture;
    std::vector<sf::Sprite> _animationSprites;

public:
    Animation(const std::string& filename, bool isLooping, 
            sf::Vector2f position = {DEFAULT_POSITION_X, DEFAULT_POSITION_Y}) 
        : _isLooping(isLooping), _currentFrame(0), _spritePosition(position) {
        
        if (!_animationTexture.loadFromFile(filename)) {
            throw std::runtime_error("Не удалось загрузить файл: " + filename);
        }

        _imageSize = _animationTexture.getSize();
        if (_imageSize.x == 0 || _imageSize.y == 0) {
            throw std::runtime_error("Размер текстуры равен 0!");
        }

        _imageCount = static_cast<int>(_imageSize.x / FRAME_SIZE);

        for (int i = 0; i < _imageCount; ++i) {
            sf::Sprite animationSprite(
                _animationTexture,
                sf::IntRect({ i * FRAME_SIZE, 0 }, { FRAME_SIZE, FRAME_SIZE })
            );
            animationSprite.setPosition(_spritePosition);
            animationSprite.scale({ SPRITE_SCALE, SPRITE_SCALE });

            _animationSprites.push_back(animationSprite);
        }
    }

    // Геттеры
    int getCurrentFrame() const { return _currentFrame; }
    int getImageCount() const { return _imageCount; }
    bool isLooping() const { return _isLooping; }
    const sf::Sprite& getSprite(int frameIndex) const { return _animationSprites[frameIndex]; }
    const sf::Sprite& getCurrentSprite() const { return _animationSprites[_currentFrame]; }
    
    // Сеттеры
    void setCurrentFrame(int frame) { 
        if (frame >= 0 && frame < _imageCount) {
            _currentFrame = frame;
        }
    }
    void setPosition(const sf::Vector2f& position) {
        _spritePosition = position;
        for (auto& sprite : _animationSprites) {
            sprite.setPosition(_spritePosition);
        }
    }

    // Дружественный класс для доступа к приватным полям
    friend class Animator;
};

class Animator {
private:
    sf::Clock _clock;
    float _frameTime = 0.125f;

public:
    Animator(float frameTime = 0.125f) : _frameTime(frameTime) {}

    void setFrameTime(float frameTime) { _frameTime = frameTime; }

    void RunAnimation(Animation& animation, sf::RenderWindow& window) {
        if (_clock.getElapsedTime().asSeconds() >= _frameTime) {
            if (animation._isLooping) {
                animation._currentFrame = (animation._currentFrame + 1) % animation._imageCount;
            } else {
                if (animation._currentFrame < animation._imageCount - 1) {
                    animation._currentFrame++;
                }
            }
            _clock.restart();
        }

        window.draw(animation._animationSprites[animation._currentFrame]);
    }
};

// Отдельный класс для отрисовки карты
class MapRenderer {
private:
    static constexpr int TILE_SIZE = 64;

public:
    static void DrawMap(sf::RenderWindow& window, MapGenerator& generator, 
                       const std::vector<std::vector<int>>& map) {
        for (size_t y = 0; y < map.size(); y++) {
            for (size_t x = 0; x < map[y].size(); x++) {
                int tile = map[y][x];

                if (tile == 0) continue; // пусто, не рисуем

                sf::Sprite sprite = generator.getTileSprite(tile);
                sprite.setPosition({static_cast<float>(x * TILE_SIZE), 
                                   static_cast<float>(y * TILE_SIZE)});
                window.draw(sprite);
            }
        }
    }
};
