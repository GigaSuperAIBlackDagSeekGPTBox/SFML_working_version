#include "MapGenerator.h"
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <string>

MapGenerator::MapGenerator(int width, int height, float frequency, const std::string& filename)
    : _width(width), _height(height), _frequency(frequency) 
{
    _noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    _noise.SetSeed(static_cast<int>(std::time(nullptr)));

    _noise.SetFrequency(_frequency);

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load tileset: " + filename);
    }
    _tileTexture = tileTexture;

    int imageCount = _tileTexture.getSize().x / 32;
    _tileMap.reserve(imageCount);

    for (int i = 0; i < imageCount; ++i) {
        sf::Sprite sprite(_tileTexture);
        sprite.setTextureRect(sf::IntRect({i * 32, 0}, {32, 32}));
        sprite.scale({2.0f, 2.0f});
        _tileMap.push_back(sprite);
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

const sf::Sprite& MapGenerator::getTileSprite(int id) const {
    if (id < 0 || static_cast<size_t>(id) >= _tileMap.size()) {
        throw std::out_of_range("Неверный ID тайла: " + std::to_string(id));
    }
    return _tileMap[id];
}