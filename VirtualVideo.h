#pragma once
#include <memory>
#include "D2DFramework.h"

const int BITMAP_WIDTH = 1024;
const int BITMAP_HEIGHT = 768;
const int BITMAP_BYTECOUNT = 4; // RGB + A (1byte)

class VirtualVideo : public D2DFramework
{
private:
	std::unique_ptr<UINT8[]> mspMemoryBuffer;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspVirtualGC;

public:
	virtual HRESULT Init(HINSTANCE hInstance,
		LPCWSTR title = L"D2DFramework",
		UINT width = 1024, UINT height = 768) override;
	void Render() override;
};

