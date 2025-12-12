#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>
#include <cmath>
#include <stdexcept>

class Bone{
private:
    sf::Texture _boneTexture;
    sf::Sprite _boneSprite;
    sf::Vector2f _localPosition;
    float _rotation = 0.f;
    Bone* _parent = nullptr;

public:
    Bone(Bone* parent, const std::string& filename, 
        sf::Vector2f initialPosition = {0.0f, 0.0f}) 
        : _boneSprite(_boneTexture), _parent(parent), _localPosition(initialPosition) {
        
        sf::Texture boneTexture;
        if (!boneTexture.loadFromFile(filename)) {
            throw std::runtime_error("Не удалось загрузить " + filename);
        }
        _boneTexture = boneTexture;
        
        sf::Sprite boneSprite(_boneTexture);
        _boneSprite = boneSprite;
        _boneSprite.scale({2.0f, 2.0f});
        _boneSprite.setPosition(initialPosition);
    }

    sf::Vector2f getGlobalPosition() const {
        if (_parent) {
            float rad = _parent->getGlobalRotation() * 3.14159f / 180.f;
            sf::Vector2f offset(
                _localPosition.x * static_cast<float>(cos(rad)) - _localPosition.y * static_cast<float>(sin(rad)),
                _localPosition.x * static_cast<float>(sin(rad)) + _localPosition.y * static_cast<float>(cos(rad))
            );
            return _parent->getGlobalPosition() + offset;
        }
        return _localPosition;
    }

    float getGlobalRotation() const {
        return _rotation + (_parent ? _parent->getGlobalRotation() : 0.0f);
    }

    // Геттеры для локальных значений (нужны для анимации)
    sf::Vector2f getLocalPosition() const { return _localPosition; }
    float getLocalRotation() const { return _rotation; }
    Bone* getParent() const { return _parent; }

    void setParent(Bone* parent) { _parent = parent; }
    void setLocalPositionVector(const sf::Vector2f& local) { _localPosition = local; }
    void setRotationAngle(float rotation) { _rotation = rotation; }
    void setOriginVector(const sf::Vector2f& origin) { _boneSprite.setOrigin(origin); }

    void updateSprite(){
        _boneSprite.setPosition(getGlobalPosition());
        _boneSprite.setRotation(sf::degrees(getGlobalRotation())); // конвертируем градусы в sf::Angle
    }

        void draw(sf::RenderTarget& target) {
        updateSprite();
        target.draw(_boneSprite);
    }

    sf::Sprite& spriteRef() { return _boneSprite;}
};