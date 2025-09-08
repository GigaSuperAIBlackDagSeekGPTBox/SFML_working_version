#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>

class Animation;

//Проигрывает анимации из класса Animation


//Анимации которые буду проигрываться в аниматоре
class Animation{
public:
    int _imageCount;
    int _currentFrame;
    bool _isLooping;
    sf::Vector2u _imageSize;
    sf::Vector2f _spritePosition;
    sf::Image _animationImage;
    sf::Texture animationTexture;
    std::vector<sf::Texture> _animationTextures;
    std::vector<sf::Sprite> _animationSprites;

public:
    Animation(const std::string &filename, bool isLooping){
        _isLooping = isLooping;
        _currentFrame = 0;

        if (!_animationImage.loadFromFile(filename)){
            std::cout << "Файл не загружен" << std::endl;
            throw std::runtime_error("Файл загружен, но размер текстуры 0!");
        }

        _imageSize = {_animationImage.getSize().x, _animationImage.getSize().y};
        _spritePosition = {_imageSize.x / 2, _imageSize.y / 2};
        _imageCount = static_cast<int>(_imageSize.x/32);

        for (int i = 0; i < _imageCount; ++i){
            sf::Vector2i framePosition(i * 32, 0);
            sf::Vector2i frameSize(32, 32);

            sf::IntRect frameRect(framePosition, frameSize);
            animationTexture.loadFromFile(filename, false, sf::IntRect({i * 32, 0}, {32, 32}));
            _animationTextures.push_back(animationTexture);
            sf::Sprite animationSprite(animationTexture);
            animationSprite.setPosition(_spritePosition);
            _animationSprites.push_back(animationSprite);
        }

    }
};

class Animator{
private:
    sf::Clock clock; 
    std::vector<std::vector<sf::Sprite>> entitiesAnimationFrames;
    float _frameTime  = 0.5;
public:
    Animator(){

    }

    void RunAnimation(Animation &animation, sf::RenderWindow &window){
        if (clock.getElapsedTime().asSeconds() >= _frameTime){
            animation._currentFrame = (animation._currentFrame + 1) % animation._imageCount;
            clock.restart();
        }

        window.draw(animation._animationSprites[animation._currentFrame]);
    }

};