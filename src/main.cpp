#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>
#include <optional>
#include "../Animation.h"
#include "../Crosshair.h"
#include "../Bone.h"
#include "../MapGenerator.h"
#include "../SkeletalAnimation.h"


int main()
{
    Bone body(nullptr, "assets/sprites/heroBodyLegsTest.png", {400.f, 300.f});
    
    MapGenerator generator(60, 70, 0.20f, "assets/sprites/BlocksTest.png");
    auto map = generator.generate();

    Bone head(&body, "assets/sprites/heroHeadTest.png", {29.f, 16.f});
    Bone rightHand(&body, "assets/sprites/heroRightHandTest.png", {25.f, 22.f});
    Bone leftHand(&body, "assets/sprites/heroHandTest.png", {39.f, 20.f});

    head.setOriginVector({15.f, 5.f});
    rightHand.setOriginVector({11.f, 10.f});
    leftHand.setOriginVector({20.f, 10.f});

    // Создаем скелет для скелетной анимации
    Skeleton skeleton;
    skeleton.addBone("body", &body);
    skeleton.addBone("head", &head);
    skeleton.addBone("rightHand", &rightHand);
    skeleton.addBone("leftHand", &leftHand);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "GameTest", sf::Style::None);

    window.setMouseCursorVisible(false);
    window.setFramerateLimit(144);

    Crosshair crosshair("assets/sprites/crosshair.png", window);
    
    Animation idleHeroAnimation("assets/sprites/mainHeroTest.png", true);

    Animator animator;

    while(window.isOpen()){
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color(100, 150, 200));
        
        //MapRenderer::DrawMap(window, generator, map);
        
        // Получаем позицию мыши
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        
        // Получаем глобальную позицию тела (корневой кости)
        sf::Vector2f bodyPos = body.getGlobalPosition();
        
        // Вычисляем угол от головы к мышке
        sf::Vector2f headPos = head.getGlobalPosition();
        sf::Vector2f directionToMouse = mousePosF - headPos;
        float angleToMouse = std::atan2(directionToMouse.y, directionToMouse.x) * 180.0f / 3.14159f;
        // В SFML 0 градусов = вправо, но спрайты обычно смотрят вверх, поэтому вычитаем 90
        head.setRotationAngle(angleToMouse - 90.0f);
        
        // Вычисляем угол от правой руки к мышке
        sf::Vector2f rightHandPos = rightHand.getGlobalPosition();
        directionToMouse = mousePosF - rightHandPos;
        angleToMouse = std::atan2(directionToMouse.y, directionToMouse.x) * 180.0f / 3.14159f;
        rightHand.setRotationAngle(angleToMouse - 90.0f);
        
        // Вычисляем угол от левой руки к мышке
        sf::Vector2f leftHandPos = leftHand.getGlobalPosition();
        directionToMouse = mousePosF - leftHandPos;
        angleToMouse = std::atan2(directionToMouse.y, directionToMouse.x) * 180.0f / 3.14159f;
        leftHand.setRotationAngle(angleToMouse - 90.0f);
        
        // Рисуем скелет
        skeleton.draw(window);
        
        crosshair.Draw(window);

        window.display();
    }

    return 0;
}