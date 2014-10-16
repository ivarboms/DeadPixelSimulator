#include <random>
#include <Windows.h>

//Longer delays between new pixels looks more real.
const unsigned int msBetweenNewPixels = 1000;


struct Pixel
{
	int x;
	int y;
	COLORREF color;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	const HDC desktop = GetDC(nullptr);
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const COLORREF red = RGB(255, 0, 0);

	std::mt19937 rng;
	const std::uniform_int_distribution<unsigned int> pixelWidthDistribution(0, screenWidth);
	const std::uniform_int_distribution<unsigned int> pixelHeightDistribution(0, screenHeight);
	const std::uniform_int_distribution<unsigned int> colorDistribution(0, 255);

	std::vector<Pixel> pixels;

	unsigned int msSincePixelWasAdded = 0;

	while (true)
	{
		if (msSincePixelWasAdded >= msBetweenNewPixels)
		{
			Pixel pixel;
			pixel.x = pixelWidthDistribution(rng);
			pixel.y = pixelHeightDistribution(rng);

			//Random colored pixels.
			pixel.color = RGB(colorDistribution(rng), colorDistribution(rng), colorDistribution(rng));
			//Red pixels (looks more like real dead pixels).
			//pixel.color = red;

			pixels.push_back(pixel);

			msSincePixelWasAdded = 0;
		}

		for (const Pixel& pixel : pixels)
		{
			SetPixel(desktop, pixel.x, pixel.y, pixel.color);
		}

		//If sleep duration is greater than the monitor's refresh rate, the pixels can start flickering
		//when moving windows behind them.
		//Sleeping for 6 ms should be fine for most monitors up to 144 hz (144 hz = 6.94 ms between frames).
		const unsigned int sleepDurationMs = 6;
		Sleep(sleepDurationMs);
		msSincePixelWasAdded += sleepDurationMs;
	}
}

