#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>
#include <variant>
#include "../Animation.h"
#include "../Crosshair.h"
#include "../Bone.h"
#include "../MapGenerator.h"


int main()
{
    Bone body(nullptr, "assets/sprites/heroBodyLegsTest.png");

    body.setLocalPositionVector({400.f, 300.f});
    //body.setOriginVector({body.spriteRef().getLocalBounds().width/2.f,
    //body.spriteRef().getLocalBounds().height/2.f});

    
    MapGenerator generator(60, 70, 0.20f, "assets/sprites/BlocksTest.png");
    auto map = generator.generate();
    sf::RectangleShape tile(sf::Vector2f((float)64, (float)64));


    Bone head(&body, "assets/sprites/heroHeadTest.png");
    Bone rightHand(&body, "assets/sprites/heroRightHandTest.png");
    Bone leftHand(&body, "assets/sprites/heroHandTest.png");

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "GameTest", sf::Style::None);

    window.setMouseCursorVisible(false);
    window.setFramerateLimit(144);

    sf::Texture texture("assets/sprites/mainHeroTest.png", false, sf::IntRect({ 0, 0 }, { 32, 32 }));
    sf::Sprite sprite(texture);

    int a = sprite.getLocalBounds().size.x;

    sprite.setPosition({500, 500});

    Crosshair crosshair("assets/sprites/crosshair.png", window);
    

    Animation idleHeroAnimation("assets/sprites/mainHeroTest.png", true);

    Animator animator;

    std::cout << a << std::endl;

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
        animator.DrawMap(window, generator, map);

        animator.RunAnimation(idleHeroAnimation, window);
        crosshair.Draw(window);

        //body.draw(window);
        //head.draw(window);
        //rightHand.draw(window);
        //leftHand.draw(window);

        window.display();

        
    }

    return 0;
}