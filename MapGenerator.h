#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "FastNoiseLite.h"

class MapGenerator {
public:
    MapGenerator(int width, int height, float frequency, const std::string& filename);

    // 0 = воздух, 1 = пол, 2 = стена
    std::vector<std::vector<int>> generate();

    const sf::Sprite& getTileSprite(int id) const;

private:
    int _width;
    int _height;
    float _frequency;
    FastNoiseLite _noise;
    sf::Vector2u _imageSize;
    sf::Image _tileImage;
    sf::Texture _tileTexture;
    std::vector<sf::Texture> _tileMapTextures;
    std::vector<sf::Sprite> _tileMap;
};