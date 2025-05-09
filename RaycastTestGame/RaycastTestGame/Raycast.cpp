#include "Raycast.h"

#include <algorithm>

#include "Camera.h"
#include "Color.h"
#include "Map.h"
#include "Player.h"
#include "Texture.h"
#include "Sprite.h"
#include "Viewport.h"

// RAYCASTING ENGINE HEAVILY BASED ON LODE'S TUTORIAL
// seen at https://lodev.org/cgtutor/raycasting.html
// Modified to include more object oriented programming

void FloorRaycast(int y, Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures)
{

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& camSizeX = camera->size.x;
	float& camSizeY = camera->size.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	Vector2i& mapSize = map->GetMapSize();

	int& width = viewport->size.x;
	int& height = viewport->size.y;

	uint16_t* floorData = map->GetDataTypeBuffer(MapDataType::FLOOR);
	uint16_t* roofData = map->GetDataTypeBuffer(MapDataType::ROOF);

	// Ray direction for leftmost ray (x = 0) and righmost ( x = width)
	Vector2 l_rayDir =
	{
		plDirX - camSizeX,
		plDirY - camSizeY
	};

	Vector2 r_rayDir =
	{
		plDirX + camSizeX,
		plDirY + camSizeY
	};

	// Current y position compared to the center of viewport (midpoint / horizon)
	int p = y - height / 2;

	// Z position of the camera (the centre of the screen)
	float z = 0.5f * static_cast<float>(height);

	// 0.0 - 1.0 ratio of the z position. 0.5 is the centre of the screen
	float rowDist = z / static_cast<float>(p);

	// calculate step vector we have to add for screen x pos (parallel to camera plane)
	Vector2 floorStep =
	{
		rowDist * (r_rayDir.x - l_rayDir.x) / static_cast<float>(width),
		rowDist * (r_rayDir.y - l_rayDir.y) / static_cast<float>(width),
	};

	// coordinates of the leftmost column (x = 0) on screen. Gets updated as we move right
	Vector2 floorPos =
	{
		plPosX + rowDist * l_rayDir.x,
		plPosY + rowDist * l_rayDir.y
	};

	for (int x = 0; x < width; ++x)
	{
		// the map cell pos
		Vector2i mapPos = { static_cast<int>(floorPos.x), static_cast<int>(floorPos.y) };
		
		// Skip if out of map boundary
		if (!(mapPos.x >= 0 && mapPos.y >= 0 && mapPos.x <= mapSize.x && mapPos.y <= mapSize.y))
		{
			// Update floor and ceiling positions
			floorPos.x += floorStep.x;
			floorPos.y += floorStep.y;

			continue;
		}

		// Get Floor texture from position on map
		Texture* floorTexture = map->GetTexture(floorData[mapPos.y * mapSize.x + mapPos.x] - 1, MapDataType::FLOOR, textures);

		Vector2i floorTextureSize = floorTexture->GetSize();

		// Texture pos from fractional part
		Vector2i floorTexturePos =
		{
			static_cast<int>((static_cast<float>(floorTextureSize.x) * (floorPos.x - static_cast<float>(mapPos.x)))) & (floorTextureSize.x - 1),
			static_cast<int>((static_cast<float>(floorTextureSize.y) * (floorPos.y - static_cast<float>(mapPos.y)))) & (floorTextureSize.y - 1)
		};

		// Get roof texture from position on map
		Texture* ceilTexture = map->GetTexture(roofData[mapPos.y * mapSize.x + mapPos.x] - 1, MapDataType::ROOF, textures);

		Vector2i ceilTextureSize = ceilTexture->GetSize();

		Vector2i ceilTexturePos = 
		{
			static_cast<int>((static_cast<float>(ceilTextureSize.x) * (floorPos.x - static_cast<float>(mapPos.x))))& (ceilTextureSize.x - 1),
			static_cast<int>((static_cast<float>(ceilTextureSize.y) * (floorPos.y - static_cast<float>(mapPos.y))))& (ceilTextureSize.y - 1)
		};

		// Update floor and ceiling positions
		floorPos.x += floorStep.x;
		floorPos.y += floorStep.y;

		// Drawing floor
		ColorA floorColor = floorTexture->GetColorFromLocation(floorTexturePos.x, floorTexturePos.y);
		floorColor /= 1.75f; // Dim the color
		viewport->AddColorAToBuffer(x, y, floorColor);
		
		// Drawing ceiling
		ColorA ceilingColor = ceilTexture->GetColorFromLocation(ceilTexturePos.x, ceilTexturePos.y);
		ceilingColor /= 1.33f; // Dim the color as before
		viewport->AddColorAToBuffer(x, height - y - 1, ceilingColor);
	}

	delete[] floorData;
	delete[] roofData;
}

void WallRaycast(int x, Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*>& textures, float*& zBuffer)
{
	float const& plPosX = player->position.x;
	float const& plPosY = player->position.y;

	float const& plDirX = player->direction.x;
	float const& plDirY = player->direction.y;

	int const& width = viewport->size.x;
	int const& height = viewport->size.y;

	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);
	Vector2i mapSize = map->GetMapSize();

	//  Right of Screen = 1, Left of Screen = - 1
	float cameraX = 2.f * x / static_cast<float>(width) - 1.f; // Camera X Position

	// Set the direction of the ray to the players direction + the x axis of the camera
	Vector2 rayDir = Vector2
	(
		plDirX + camera->size.x * cameraX,
		plDirY + camera->size.y * cameraX
	);

	// Which map cell the Player is in
	Vector2i mapPos = Vector2i
	(
		static_cast<int>(player->position.x),
		static_cast<int>(player->position.y)
	);

	// Distance of ray from one x side or y side to next one
	Vector2 deltaDist = Vector2
	(
		// Prevents Dividing by Zero (i.e. very bad) and returns the closest to infinity instead
		// Abs prevents negative distance (impossible)
		(rayDir.x == 0) ? FLT_MAX : std::abs(1.f / rayDir.x),
		(rayDir.y == 0) ? FLT_MAX : std::abs(1.f / rayDir.y)
	);

	// Will be used to calculate total ray length (Perpendicular Wall Distance)
	float perpWallDist;

	bool wallHit = false; // If wall was hit
	bool isHorizontalWall; // If a NS or EW wall was hit

	// Distance to next x or y side
	Vector2 sideDist = Vector2
	{
		(rayDir.x < 0) ? (player->position.x - mapPos.x) * deltaDist.x : (mapPos.x + 1.f - player->position.x) * deltaDist.x,
		(rayDir.y < 0) ? (player->position.y - mapPos.y) * deltaDist.y : (mapPos.y + 1.f - player->position.y) * deltaDist.y
	
	};

	// What direction to make in x or y distance
	Vector2i step = Vector2i
	{
		(rayDir.x < 0) ? -1 : 1,
		(rayDir.y < 0) ? -1 : 1
	};

	// DDA algorithm (Digital Differential Analyser)
	while (!wallHit)
	{
		// Go to next map square, either in x or y direction
		if (sideDist.x < sideDist.y)
		{
			sideDist.x += deltaDist.x;
			mapPos.x += step.x;
			isHorizontalWall = false;
		}
		else
		{
			sideDist.y += deltaDist.y;
			mapPos.y += step.y;
			isHorizontalWall = true;
		}

		// Check if ray has hit a wall
		if (wallData[mapPos.y * mapSize.x + mapPos.x] > 0)
		{
			wallHit = true;
		}
	}

	// Calculate distance of wall projected on camera direction (Direct distance to player would cause fisheye effect)
	if (!isHorizontalWall)
	{
		perpWallDist = (sideDist.x - deltaDist.x);
	}
	else
	{
		perpWallDist = (sideDist.y - deltaDist.y);
	}

	// Calculate height of line to draw on screen
	int lineHeight = static_cast<int>(height / perpWallDist);

	// Calculate the lowest and highest pixel to fill in current line
	// (Center line in middle of viewport)
	int drawStart = -lineHeight / 2 + height / 2;

	// Prevents drawing off screen
	if (drawStart < 0)
	{
		drawStart = 0;
	}

	int drawEnd = lineHeight / 2 + height / 2;
	if (drawEnd > height)
	{
		// Accounts for Screen starting at 0 and not 1
		drawEnd = height;
	}

	
	// Allows texture 0 to be used
	Texture* texture = map->GetTexture(wallData[mapPos.y * mapSize.x + mapPos.x] - 1, MapDataType::WALL, textures);

	// Calculate exact part of the wall hit instead of just cell
	float wallX = isHorizontalWall ? plPosX + perpWallDist * rayDir.x : plPosY + perpWallDist * rayDir.y;
	// Technically its the y cord of the wall if its 
	// horizontal but its the x cord of the texture

	wallX -= floorf(wallX); // % of the x coordinate if start = 0 and end = 1

	Vector2i texSize = texture->GetSize();

	// Get X Coordinate on the texture

	// Flip Texture Accordingly Depending on if on NSEW Wall
	float texPosX = static_cast<int>(wallX * static_cast<float>(texSize.x));
	if (isHorizontalWall == 0 && rayDir.x < 0)
	{
		texPosX = texSize.x - texPosX - 1;
	}
	if (isHorizontalWall == 1 && rayDir.y > 0)
	{
		texPosX = texSize.x - texPosX - 1;
	}
	// X coordinate stays the same for each cast but y has to be calculated for each

	// How much to increase the texture coordinate per pixel
	float texStep = 1.f * texSize.y / lineHeight;

	// Starting texture coordinate
	float texPosY = (drawStart - (height * 0.5f) + (lineHeight * 0.5f)) * texStep;

	// Repeat for each character in the raycast
	ColorA color;
	for (int y = drawStart; y < drawEnd; y++)
	{

		color = texture->GetColorFromLocation(texPosX, texPosY);
		if (isHorizontalWall)
		{
			color /= 1.5f;
		}
		texPosY += texStep;

		viewport->AddColorAToBuffer(x, y, color);
	}
	

	// Set Zbuffer for sprite casting
	zBuffer[x] = abs(perpWallDist);

	delete[] wallData;
}

void SpriteCasting(Viewport*& viewport, Player*& player, Camera*& camera, Map* map, float*& zBuffer)
{
	float const& plPosX = player->position.x;
	float const& plPosY = player->position.y;

	float const& camSizeX = camera->size.x;
	float const& camSizeY = camera->size.y;

	float const& plDirX = player->direction.x;
	float const& plDirY = player->direction.y;

	int const& width = viewport->size.x;
	int const& height = viewport->size.y;

	int spriteAmt = map->GetSpriteAmt();

	vector<Sprite*>& sprites = map->GetSpriteData();

	// List of sprites from closest to furthest
	int* spriteOrder = new int[spriteAmt];

	// Distance of sprites from player
	float* spriteDistance = new float[spriteAmt];

	// Sort sprites from closest to furthest
	for (int i = 0; i < spriteAmt; i++)
	{
		spriteOrder[i] = i;
		spriteDistance[i] = // Distance saved as squared cause sqrt isnt needed
		(
			(plPosX - sprites[i]->GetPosition().x) * (plPosX - sprites[i]->GetPosition().x) +
			(plPosY - sprites[i]->GetPosition().y) * (plPosY - sprites[i]->GetPosition().y)
		);
	}
	SortSprites(spriteOrder, spriteDistance, spriteAmt);

	for (int i = 0; i < spriteAmt; i++)
	{
		Sprite* sprite = sprites[spriteOrder[i]];

		// Translate sprite position to relative to camera
		Vector2 spriteDrawPos = Vector2
		{
			sprite->GetPosition().x - plPosX,
			sprite->GetPosition().y - plPosY
		};

		// Transform sprite with invertse camera matrix
		float invertedDet = 1.0f / (camSizeX * plDirY - plDirX * camSizeY); // Rquired for correct matrix multiplication

		Vector2 transform = Vector2
		{
			invertedDet * (plDirY * spriteDrawPos.x - plDirX * spriteDrawPos.y),
			invertedDet * (-camSizeY * spriteDrawPos.x + camSizeX * spriteDrawPos.y)
		};

		int spriteScreenX = static_cast<int>( (width / 2) * (1 + transform.x / transform.y) );

		float yOffset = sprite->GetOffset();

		Vector2 scale = sprite->GetScale();

		int vMoveScreen = static_cast<int>(yOffset / transform.y);

		// Calculate height of the sprite on screen
		int spriteHeight = abs(static_cast<int>( height / transform.y)) / scale.y; // using 'transform.y' instead of real distance to prevent fisheye eye

		// Calculate lowest and highest pixel to fill stripe
		int drawStartY = -spriteHeight / 2 + height / 2 + vMoveScreen;

		if (drawStartY < 0)
		{
			drawStartY = 0;
		}

		int drawEndY = spriteHeight / 2 + height / 2 + vMoveScreen;

		if (drawEndY > height)
		{
			drawEndY = height;
		}

		// Calculate width of sprite
		int spriteWidth = abs(static_cast<int>( height / transform.y)) / scale.x;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		
		if (drawStartX < 0)
		{
			drawStartX = 0;
		}

		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX > width)
		{
			drawEndX = width;
		}

		Texture* spriteTexture = sprite->GetTexture();

		Vector2i textureSize = spriteTexture->GetSize();

		for (int stripe = drawStartX; stripe < drawEndX; stripe++)
		{
			Vector2i texturePos = Vector2i
			{
				static_cast<int>(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * textureSize.x / spriteWidth) / 256,
				0
			};

			/*
				1) it's in front of camera plane so you don't see things behind you
				2) it's on the screen (left)
				3) it's on the screen (right)
				4) ZBuffer, with perpendicular distance
			*/
			if (transform.y > 0 && stripe > 0 && stripe < width && transform.y < zBuffer[stripe])
			{
				for (int y = drawStartY; y < drawEndY; y++)
				{
					int d = (y - vMoveScreen) * 256 - height * 128 + spriteHeight * 128;
					texturePos.y = ((d * textureSize.y) / spriteHeight) / 256;

					ColorA color = spriteTexture->GetTexture()[texturePos.y * textureSize.x + texturePos.x];
					viewport->AddColorAToBuffer(stripe, y, color);
				}
			}
		}
	}

	// Delete sprite distance buffer
	delete[] spriteOrder;
	delete[] spriteDistance;
}

void SortSprites(int*& order, float*& distance, int amount)
{
	// Create new vector container pairs of floats and ints with the length of all sprites
	std::vector< std::pair<float, int> > sprites(amount);
	for (int i = 0; i < amount; i++)
	{
		// Add data to vector
		sprites[i].first = distance[i];
		sprites[i].second = order[i];
	}
	// Sort sprites in ascending order
	std::sort(sprites.begin(), sprites.end());
	// restore in recverse order to go from farthest to nearest
	for (int i = 0; i < amount; i++)
	{
		distance[i] = sprites[amount - i - 1].first;
		order[i] = sprites[amount - i - 1].second;
	}
}