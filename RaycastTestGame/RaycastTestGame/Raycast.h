#pragma once

#include <vector>

class Viewport;
class Player;
class Camera;
class Map;
class Texture;

using std::vector;

void FloorRaycast(int y, Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures);

void WallRaycast(int x, Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures, bool useASCII, float*& zBuffer);

void SpriteCasting(Viewport*& viewport, Player*& player, Camera*& camera, vector<Texture*>& textures, Map* map, float*& zBuffer);

void SortSprites(int*& order, float*& distance, int amount);

unsigned char GetASCIIColorFromRaycast(int x, int y, Map*& map, bool isHorizontal);