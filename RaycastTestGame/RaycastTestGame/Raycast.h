#pragma once

#include <vector>

class Viewport;
class Player;
class Camera;
class Map;
class Texture;

using std::vector;


void Raycast(int x, Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures, bool useASCII);

unsigned char GetASCIIColorFromRaycast(int x, int y, Map*& map, bool isHorizontal);