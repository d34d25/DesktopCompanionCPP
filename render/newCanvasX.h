#pragma once
#include<Windows.h>
#include <d2d1.h>
#include <cmath>
#include <wincodec.h>
#include "myTypes.h"
#include <vector>
#include <algorithm>
#include <wrl.h>
#include <dwrite.h>
using namespace Microsoft::WRL;
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1_2helper.h>
#include <dcomp.h>
#include <string>
#include <sstream>
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dcomp")


struct ComException : public std::exception
{
	std::string message;

	HRESULT result;
	ComException(HRESULT const value):
		result(value)
	{
		std::ostringstream oss;
		oss << "COM Error with HRESULT: 0x" << std::hex << result;
		message = oss.str();
	}

	const char* what() const noexcept override
	{
		return message.c_str();
	}
};

void HR(HRESULT const result);


class NewCanvasX
{
private:

	D2D1::ColorF nToD2D1Color(const Color& color);
	D2D1_RECT_F nToD2D1Rect(const Shapes::Rectangle& rect);
	D2D_POINT_2F nToD2D1Point(const Vector2D& point);

	ComPtr<ID3D11Device> direct3dDevice;
	ComPtr<IDXGIDevice> dxgiDevice;
	ComPtr<IDXGIFactory2> dxFactory;
	ComPtr<IDXGISwapChain1> swapChain;

	ComPtr<ID2D1Factory2> d2Factory;
	ComPtr<ID2D1Device1> d2Device;
	ComPtr<ID2D1DeviceContext> dc;

	ComPtr<IDXGISurface2> surface;
	ComPtr<ID2D1Bitmap1> bitmap;

	ComPtr<IDCompositionDevice> dcompDevice;
	ComPtr<IDCompositionTarget> target;
	ComPtr<IDCompositionVisual> visual;


	ComPtr<ID2D1SolidColorBrush> brush;

	Color brushColor;

	IDWriteFactory* pDWriteFactory = nullptr;

	IDWriteTextFormat* pTextFormat = nullptr;

public:

	NewCanvasX(HWND hwnd);

	~NewCanvasX();

	void BeginDraw();

	void Clear(Color color);

	void EndDraw();

	void Present();

	void nSetBrushColor(Color color);

	ComPtr<ID2D1DeviceContext> GetDC();
	ComPtr<ID2D1SolidColorBrush> GetBrush();



	//shapes

	void DrawPoly(const Vector2D* points, size_t arraySize, Color color);

	void DrawRect(Shapes::Rectangle rect, Color color, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);

	void DrawCir(Shapes::Circle circle, Color color, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);

	void DrawTri(Shapes::Triangle triangle, Color color, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);
	
	//outline

	void DrawPolyO(const Vector2D* points, size_t arraySize, Color color);

	void DrawRectO(Shapes::Rectangle rect, Color color, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);

	void DrawCirO(Shapes::Circle circle, Color color, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);

	void DrawTriO(Shapes::Triangle triangle, Color color, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);
	
	//image

	Image LoadImg(const std::string& filepath);

	void DrawImg(const Image& image, float x, float y, const Shapes::Rectangle* sourceRect, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);

	//text
	void DrawTxt(const std::string& text, Shapes::Rectangle textArea, Color color, float angle = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);
};



std::vector<Shapes::Rectangle> CropImage(const Image& img, int frameWidth, int frameHeight);

Shapes::Rectangle* PlayAnimation(Image& spriteSheet, std::vector<Shapes::Rectangle>& frames, int startFrame, int endFrame, float animationSpeed, float elapsedTime, float x, float y);
