#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Color to RGB888 conversion function.
Uint32 rgb888(Uint8 r, Uint8 g, Uint8 b) {
	return 0xFF000000 | r << 16 | g << 8 | b;
}

// A sprite.
class Sprite {
public:
	Uint32* data;
	int x_res;
	int y_res;
	int channels;

	// Null constructor.
	Sprite() {
		data = nullptr;
	}

	// Default constructor (load from file).
	Sprite(std::string path) {
		// Load the sprite from a file.
		unsigned char* raw_data = stbi_load(path.c_str(), &x_res, &y_res, &channels, 3);
		if (!raw_data) {
			fprintf(stderr, "Could not load sprite \"%s\".\n", path.c_str());
			exit(EXIT_FAILURE);
		}

		// Convert the sprite to an optimized format.
		data = new Uint32[x_res * y_res];
		for (int y = 0; y < y_res; y++) {
			for (int x = 0; x < x_res; x++) {
				unsigned char* raw_pixel = raw_data + (y * x_res + x) * 3;
				Uint8 r = raw_pixel[0];
				Uint8 g = raw_pixel[1];
				Uint8 b = raw_pixel[2];
				data[y * x_res + x] = rgb888(r, g, b);
			}
		}

		// Free the original sprite.
		stbi_image_free(raw_data);
	}

	// Section constructor (load from another sprite).
	Sprite(Sprite sheet,
		   int top_left_x,
		   int top_left_y,
		   int x_res,
		   int y_res)
	{
		this->x_res = x_res;
		this->y_res = y_res;
		data = new Uint32[x_res * y_res];

		// Copy a section of the sprite sheet to this sprite.
		for (int y = 0; y < y_res; y++) {
			for (int x = 0; x < x_res; x++) {
				int u = x + top_left_x;
				int v = y + top_left_y;
				data[y * x_res + x] = sheet.data[v * sheet.x_res + u];
			}
		}
	}
};