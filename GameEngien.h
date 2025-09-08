#pragma once
#include <SFML/Graphics.hpp>

class GameEngien{
private:

public:
    GameEngien(){
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        sf::RenderWindow window(desktop, "SFML works!");
    }

    void StartWindow(){
        
    }
};