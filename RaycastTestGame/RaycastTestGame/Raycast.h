#pragma once

#include <vector>

#include "render/buffer/ConsolePixelBuffer.h"

class Player;
class Camera;
class Map;
class Texture;

using std::vector;

void FloorRaycast(int y, ConsoleViewport* viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures);

void WallRaycast(int x, ConsoleViewport* viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures, float*& zBuffer);

void SpriteCasting(ConsoleViewport* viewport, Player*& player, Camera*& camera, Map* map, float*& zBuffer);

void SortSprites(int*& order, float*& distance, int amount);