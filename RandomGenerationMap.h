#pragma once
#include <iostream>
#include <vector>
#include <random>

const int HEIGHT = 150;

enum Tile{FLOOR, WALL};

std::vector<std::vector<Tile>> generateMap(const int WIDTH, const int HEIGHT){
    std::vector<std::vector<Tile>> map(HEIGHT, std::vector<Tile>(WIDTH, WALL));
    std::mt19937 rng(std::random_device{}());

    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (rng() % 100 < 40) map[y][x] = FLOOR; 
        }
    }

    return map;
}