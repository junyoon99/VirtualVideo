#include "VirtualVideo.h"
#include <cmath>

HRESULT VirtualVideo::Init(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Init(hInstance, title, width, height);

	mspMemoryBuffer = std::make_unique<UINT8[]>(BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT);

	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
		),
		mspVirtualGC.GetAddressOf()
	);

	ThrowIfFailed(hr);

	return S_OK;
}

void VirtualVideo::Render()
{
	mspRenderTarget->BeginDraw();

	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	DrawPixelToBuffer(100, 100, D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
	DrawPixelToBuffer(101, 100, D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));
	DrawPixelToBuffer(102, 100, D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));

	FillRectToBuffer(100, 100, 100, 100, D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));

	LineToBuffer(50, 50, 50, 500, D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));

	CircleToBuffer(350, 350, 100, D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

	//ClearBuffer(D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));

	Present();
	mspRenderTarget->DrawBitmap(mspVirtualGC.Get());

	HRESULT hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

void VirtualVideo::Present()
{
	mspVirtualGC->CopyFromMemory(
		nullptr,
		//(void*)mspMemoryBuffer.get(),
		&mspMemoryBuffer[0],
		BITMAP_WIDTH * BITMAP_BYTECOUNT
	);
}

void VirtualVideo::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
	int index = BITMAP_BYTECOUNT * BITMAP_WIDTH * y + x * BITMAP_BYTECOUNT;

	//0.0f ~ 1.0f
	color.a;
	color.r;
	color.g;
	color.b;

	//0 ~ 255
	mspMemoryBuffer[index] = (UINT8)(color.r * 255);
	mspMemoryBuffer[index+1] = (UINT8)(color.g * 255);
	mspMemoryBuffer[index+2] = (UINT8)(color.b * 255);
	mspMemoryBuffer[index+3] = (UINT8)(color.a * 255);
}

void VirtualVideo::ClearBuffer(D2D1::ColorF color)
{
	for (int x{}; x < BITMAP_WIDTH; ++x) 
	{
		for (int y{}; y < BITMAP_HEIGHT; ++y) 
		{
			DrawPixelToBuffer(x, y, color);
		}
	}
}

void VirtualVideo::FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color)
{
	for (int x{}; x < width; ++x)
	{
		for (int y{}; y < height; ++y)
		{
			DrawPixelToBuffer(x + left, y + height, color);
		}
	}
}

void VirtualVideo::LineToBuffer(int x1, int y1, int x2, int y2, D2D1::ColorF color)
{
	//if (x1 - x2 != 0)
	//{
	//	int a = abs(y2 - y1) / abs(x2 - x1);
	//	int b = abs(x2 - x1) / abs(y2 - y1);
	//	for (int i{}; i < abs(x1 - x2); ++i)
	//	{
	//		DrawPixelToBuffer(b*(i + x1), a*(i + y1), color);
	//	}
	//}
	//else return;
	float dx = (float)(x2 - x1);
	float dy = (float)(y2 - y1);

	if (dx == 0 && dy == 0) 
	{
		DrawPixelToBuffer(x1, y1, color);
		return;
	}
	
	if (abs(dx) > abs(dy)) 
	{
		int sign = dx < 0 ? -1 : 1;
		int y{};
		for (int i{ x1 }; i != x2; i += sign) 
		{
			y = (int)((dy / dx) * (i - x1) + y1);
			DrawPixelToBuffer(i, y, color);
		}
	}
	else 
	{
		int sign = dy < 0 ? -1 : 1;
		int x{};
		for (int i{ y1 }; i != y2; i += sign)
		{
			x = (int)((dx / dy) * (i - y1) + x1);
			DrawPixelToBuffer(x, i, color);
		}
	}
}

void VirtualVideo::CircleToBuffer(int centerX, int centerY, int radius, D2D1::ColorF color)
{
	const double pi{ 3.141592 };
	double theta{};
	for (theta; theta < 2 * pi; theta += 0.1)
	{
		DrawPixelToBuffer((int)centerX + radius*cosf(theta), (int)centerY + radius*sinf(theta), color);
	}
}
