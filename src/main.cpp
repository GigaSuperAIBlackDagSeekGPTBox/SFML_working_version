#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../Animation.h"
#include <variant>

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "GameTest");

    sf::Texture texture("assets/sprites/mainHeroTest.png", false, sf::IntRect({ 0, 0 }, { 32, 32 }));
    sf::Sprite sprite(texture);

    sprite.setPosition({500, 500});

    Animation idleHeroAnimation("D:/SFML_Test_project/src/sprites/mainHeroTest.png", true);

    Animator animator;

    window.setFramerateLimit(60);

    while(window.isOpen()){
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
                window.close();
            }
        }

        window.clear();
        //window.draw(sprite);
        animator.RunAnimation(idleHeroAnimation, window);
        window.display();

        
    }

    return 0;
}