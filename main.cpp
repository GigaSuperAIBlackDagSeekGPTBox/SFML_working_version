#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <variant>

int main()
{
    // Создаём окно 400x400 с заголовком
    sf::RenderWindow window(sf::VideoMode({960, 960}), "SFML works!");

    while(window.isOpen()){
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.display();

        window.clear();
    }

    return 0;
}