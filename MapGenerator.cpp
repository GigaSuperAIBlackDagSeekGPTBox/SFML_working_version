#include "MapGenerator.h"
#include <stdexcept>
#include <iostream>
#include <ctime>

MapGenerator::MapGenerator(int width, int height, float frequency, const std::string& filename)
    : _width(width), _height(height), _frequency(frequency), _sprite(_tileTexture) 
{
    _noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    _noise.SetSeed(static_cast<int>(std::time(nullptr)));

    _noise.SetFrequency(_frequency);

    if (!_tileTexture.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load tileset: " + filename);
    }

    int imageCount = _tileTexture.getSize().x / 32;
    _tileMap.reserve(imageCount);

    for (int i = 0; i < imageCount; ++i) {
        _sprite.setTexture(_tileTexture);
        _sprite.setTextureRect(sf::IntRect({i * 32, 0}, {32, 32}));
        _sprite.setScale({2.0f, 2.0f});
        _tileMap.push_back(_sprite);
    }

    std::cout << "Tiles loaded: " << _tileMap.size() << std::endl;
}

std::vector<std::vector<int>> MapGenerator::generate() {
    std::vector<std::vector<int>> map(_height, std::vector<int>(_width, 0));

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            float noiseValue = _noise.GetNoise((float)x, (float)y);
            if (noiseValue < -0.5f) map[y][x] = 0; // пусто
            else if (noiseValue < -0.1f) map[y][x] = 1; // пол
            else map[y][x] = 2; // стена
        }
    }

    return map;
}

sf::Sprite MapGenerator::getTileSprite(int id) {
    return _tileMap[id]; // id=0 это пустой тайл из файла
}