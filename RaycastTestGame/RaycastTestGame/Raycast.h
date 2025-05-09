#pragma once

#include <vector>

class Viewport;
class Player;
class Camera;
class Map;
class Texture;

using std::vector;

void FloorRaycast(int y, Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures);

void WallRaycast(int x, Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures, float*& zBuffer);

void SpriteCasting(Viewport*& viewport, Player*& player, Camera*& camera, Map* map, float*& zBuffer);

void SortSprites(int*& order, float*& distance, int amount);