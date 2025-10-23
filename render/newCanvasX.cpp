#include "newCanvasX.h"
#include <iostream>

void HR(HRESULT const result)
{
	if (S_OK != result)
	{
		throw ComException(result);
	}
}

D2D1::ColorF NewCanvasX::nToD2D1Color(const Color& color)
{
	return D2D1::ColorF(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

D2D1_RECT_F NewCanvasX::nToD2D1Rect(const Shapes::Rectangle& rect)
{
	return D2D1::RectF(
		rect.x,
		rect.y,
		rect.x + rect.width,
		rect.y + rect.height
	);
}

D2D_POINT_2F NewCanvasX::nToD2D1Point(const Vector2D& point)
{
	return D2D1::Point2F(point.x, point.y);
}

NewCanvasX::NewCanvasX(HWND window):
	pDWriteFactory(nullptr), pTextFormat(nullptr)
{

	HR(D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&direct3dDevice,
		nullptr,
		nullptr));

	HR(direct3dDevice.As(&dxgiDevice));

	HR(CreateDXGIFactory2(
		DXGI_CREATE_FACTORY_DEBUG,
		__uuidof(dxFactory),
		reinterpret_cast<void**>(dxFactory.GetAddressOf())));

	DXGI_SWAP_CHAIN_DESC1 description = {};

	description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	description.BufferCount = 2;
	description.SampleDesc.Count = 1;
	description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

	RECT rect = {};
	GetClientRect(window, &rect);
	description.Width = rect.right - rect.left;
	description.Height = rect.bottom - rect.top;

	HR(dxFactory->CreateSwapChainForComposition(dxgiDevice.Get(),
		&description,
		nullptr,
		swapChain.GetAddressOf()));

	D2D1_FACTORY_OPTIONS const options = { D2D1_DEBUG_LEVEL_INFORMATION };

	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		options,
		d2Factory.GetAddressOf()));

	HR(d2Factory->CreateDevice(dxgiDevice.Get(),
		d2Device.GetAddressOf()));

	HR(d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		dc.GetAddressOf()));

	HR(swapChain->GetBuffer(
		0,
		__uuidof(surface),
		reinterpret_cast<void**>(surface.GetAddressOf())));

	D2D1_BITMAP_PROPERTIES1 properties = {};

	properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET |
		D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

	HR(dc->CreateBitmapFromDxgiSurface(surface.Get(),
		properties,
		bitmap.GetAddressOf()));

	dc->SetTarget(bitmap.Get());

	HR(DCompositionCreateDevice(dxgiDevice.Get(),
		__uuidof(dcompDevice),
		reinterpret_cast<void**>(dcompDevice.GetAddressOf())));

	HR(dcompDevice->CreateTargetForHwnd(window,
		true,
		target.GetAddressOf()));

	HR(dcompDevice->CreateVisual(visual.GetAddressOf()));
	HR(visual->SetContent(swapChain.Get()));
	HR(target->SetRoot(visual.Get()));
	HR(dcompDevice->Commit());

	HR(dc->CreateSolidColorBrush(nToD2D1Color(brushColor),
		brush.GetAddressOf()));



	//TEXT init

	HRESULT hrWrite = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(pDWriteFactory.GetAddressOf())
	);

	if (SUCCEEDED(hrWrite))
	{
		hrWrite = pDWriteFactory->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			24.0f,
			L"en-us",
			&pTextFormat
		);
	}

}


void NewCanvasX::BeginDraw()
{
	dc->BeginDraw();
}

void NewCanvasX::Clear(Color color)
{
	dc->Clear(nToD2D1Color(color));
}

void NewCanvasX::EndDraw()
{
	try
	{
		HR(dc->EndDraw());
	}
	catch (const ComException& e)
	{
		//std::cerr << "COM Exception caught: " << e.what() << std::endl;
	}

}

void NewCanvasX::Present()
{
	try
	{
		HR(swapChain->Present(1,
			0));
	}
	catch (const ComException& e)
	{
		//std::cerr << "COM Exception caught: " << e.what() << std::endl;
	}

}

void NewCanvasX::nSetBrushColor(Color color)
{
	brush->SetColor(nToD2D1Color(color));
}

ComPtr<ID2D1DeviceContext> NewCanvasX::GetDC()
{
	return this->dc;
}

ComPtr<ID2D1SolidColorBrush> NewCanvasX::GetBrush()
{
	return this->brush;
}

void NewCanvasX::DrawPoly(const Vector2D* points, size_t arraySize, Color color)
{
	if (arraySize < 2) return;

	std::vector<D2D_POINT_2F> d2dPoints(arraySize);
	for (size_t i = 0; i < arraySize; ++i)
	{
		d2dPoints[i] = nToD2D1Point(points[i]);
	}

	this->nSetBrushColor(color);

	ID2D1PathGeometry* pGeometry = nullptr;

	HRESULT hr = d2Factory->CreatePathGeometry(&pGeometry);
	if (FAILED(hr)) return;

	ID2D1GeometrySink* pSink = nullptr;

	hr = pGeometry->Open(&pSink);
	if (FAILED(hr))
	{
		SafeRelease(&pGeometry);
		return;
	}

	pSink->BeginFigure(d2dPoints[0], D2D1_FIGURE_BEGIN_FILLED);
	pSink->AddLines(d2dPoints.data() + 1, static_cast<UINT32>(arraySize - 1));
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pSink->Close();

	this->dc->FillGeometry(pGeometry, brush.Get());

	SafeRelease(&pSink);
	SafeRelease(&pGeometry);
}

void NewCanvasX::DrawRect(Shapes::Rectangle rect, Color color, float angle, float scaleX, float scaleY)
{
	D2D1_RECT_F rc = D2D1::RectF(0, 0, rect.width, rect.height);

	this->nSetBrushColor(color);

	D2D1_MATRIX_3X2_F transform =
		D2D1::Matrix3x2F::Translation(-rect.width * 0.5f, -rect.height * 0.5f) *
		D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(0, 0)) *
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
		D2D1::Matrix3x2F::Translation(rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f);

	dc->SetTransform(transform);

	this->dc->FillRectangle(rc, brush.Get());

	dc->SetTransform(D2D1::Matrix3x2F::Identity());
}

void NewCanvasX::DrawCir(Shapes::Circle circle, Color color, float angle, float scaleX, float scaleY)
{
	this->nSetBrushColor(color);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(0, 0),
		circle.radius,
		circle.radius
	);

	D2D1_MATRIX_3X2_F transform =
		D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(0, 0)) *
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
		D2D1::Matrix3x2F::Translation(circle.x, circle.y);

	dc->SetTransform(transform);

	this->dc->FillEllipse(ellipse, brush.Get());

	dc->SetTransform(D2D1::Matrix3x2F::Identity());
}

void NewCanvasX::DrawTri(Shapes::Triangle triangle, Color color, float angle, float scaleX, float scaleY)
{
	float halfW = triangle.base / 2.0f;
	float height = triangle.height;

	Vector2D localVertices[3] = {
		{-halfW, 0},
		{halfW, 0},
		{0, -height}
	};

	Vector2D transformed[3];

	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	for (int i = 0; i < 3; ++i)
	{
		float xRot = localVertices[i].x * cosA - localVertices[i].y * sinA;
		float yRot = localVertices[i].x * sinA + localVertices[i].y * cosA;

		transformed[i] = {
			triangle.x + xRot * scaleX,
			triangle.y + yRot * scaleY,
		};
	}

	this->DrawPoly(transformed, 3, color);
}

void NewCanvasX::DrawPolyO(const Vector2D* points, size_t arraySize, Color color)
{
	if (arraySize < 2) return;

	std::vector<D2D_POINT_2F> d2dPoints(arraySize);
	for (size_t i = 0; i < arraySize; ++i)
	{
		d2dPoints[i] = nToD2D1Point(points[i]);
	}

	this->nSetBrushColor(color);

	ID2D1PathGeometry* pGeometry = nullptr;

	HRESULT hr = d2Factory->CreatePathGeometry(&pGeometry);
	if (FAILED(hr)) return;

	ID2D1GeometrySink* pSink = nullptr;

	hr = pGeometry->Open(&pSink);
	if (FAILED(hr))
	{
		SafeRelease(&pGeometry);
		return;
	}

	pSink->BeginFigure(d2dPoints[0], D2D1_FIGURE_BEGIN_FILLED);
	pSink->AddLines(d2dPoints.data() + 1, static_cast<UINT32>(arraySize - 1));
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pSink->Close();

	this->dc->DrawGeometry(pGeometry, brush.Get());

	SafeRelease(&pSink);
	SafeRelease(&pGeometry);
}

void NewCanvasX::DrawRectO(Shapes::Rectangle rect, Color color, float angle, float scaleX, float scaleY)
{
	D2D1_RECT_F rc = D2D1::RectF(0, 0, rect.width, rect.height);

	this->nSetBrushColor(color);

	D2D1_MATRIX_3X2_F transform =
		D2D1::Matrix3x2F::Translation(-rect.width * 0.5f, -rect.height * 0.5f) *
		D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(0, 0)) *
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
		D2D1::Matrix3x2F::Translation(rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f);

	dc->SetTransform(transform);

	this->dc->DrawRectangle(rc, brush.Get());

	dc->SetTransform(D2D1::Matrix3x2F::Identity());
}

void NewCanvasX::DrawCirO(Shapes::Circle circle, Color color, float angle, float scaleX, float scaleY)
{
	this->nSetBrushColor(color);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(0, 0),
		circle.radius,
		circle.radius
	);

	D2D1_MATRIX_3X2_F transform =
		D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(0, 0)) *
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
		D2D1::Matrix3x2F::Translation(circle.x, circle.y);

	dc->SetTransform(transform);

	this->dc->DrawEllipse(ellipse, brush.Get());

	dc->SetTransform(D2D1::Matrix3x2F::Identity());
}

void NewCanvasX::DrawTriO(Shapes::Triangle triangle, Color color, float angle, float scaleX, float scaleY)
{
	float halfW = triangle.base / 2.0f;
	float height = triangle.height;

	Vector2D localVertices[3] = {
		{-halfW, 0},
		{halfW, 0},
		{0, -height}
	};

	Vector2D transformed[3];

	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	for (int i = 0; i < 3; ++i)
	{
		float xRot = localVertices[i].x * cosA - localVertices[i].y * sinA;
		float yRot = localVertices[i].x * sinA + localVertices[i].y * cosA;

		transformed[i] = {
			triangle.x + xRot * scaleX,
			triangle.y + yRot * scaleY,
		};
	}

	this->DrawPolyO(transformed, 3, color);
}

Image NewCanvasX::LoadImg(const std::string& filepath)
{
	std::wstring wFilepath(filepath.begin(), filepath.end());

	IWICImagingFactory* wicFactory = nullptr;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frame = nullptr;
	IWICFormatConverter* converter = nullptr;

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)
	);

	if (FAILED(hr))
	{
		return {};
	}

	hr = wicFactory->CreateDecoderFromFilename(
		wFilepath.c_str(), nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder
	);

	if (FAILED(hr))
	{
		SafeRelease(&wicFactory);
		return {};
	}

	hr = decoder->GetFrame(0, &frame);

	if (FAILED(hr))
	{
		SafeRelease(&decoder);
		SafeRelease(&wicFactory);
		return {};
	}

	hr = wicFactory->CreateFormatConverter(&converter);

	if (FAILED(hr))
	{
		SafeRelease(&frame);
		SafeRelease(&decoder);
		SafeRelease(&wicFactory);
		return {};
	}

	hr = converter->Initialize(
		frame,
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr, 0.0,
		WICBitmapPaletteTypeCustom
	);

	if (FAILED(hr))
	{
		SafeRelease(&converter);
		SafeRelease(&frame);
		SafeRelease(&decoder);
		SafeRelease(&wicFactory);
		return {};
	}

	UINT width = 0, height = 0;
	converter->GetSize(&width, &height);

	Image img(width, height, 4);

	hr = converter->CopyPixels(
		nullptr,
		width * 4,
		static_cast<UINT>(img.data.size()),
		img.data.data()
	);

	SafeRelease(&converter);
	SafeRelease(&frame);
	SafeRelease(&decoder);
	SafeRelease(&wicFactory);

	if (FAILED(hr)) return {};

	if (!img.isValid() || !dc)
	{
		return {};
	}


	for (size_t i = 0; i < img.data.size(); i += 4)
	{
		float alpha = img.data[i + 3] / 255.0f;
		img.data[i + 0] = static_cast<uint8_t>(img.data[i + 0] * alpha); // R
		img.data[i + 1] = static_cast<uint8_t>(img.data[i + 1] * alpha); // G
		img.data[i + 2] = static_cast<uint8_t>(img.data[i + 2] * alpha); // B
	}

	D2D1_BITMAP_PROPERTIES bitmapProps = {
		{DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED},
		96.0f, 96.0f
	};


	ID2D1Bitmap* pBitmap = nullptr;

	hr = dc->CreateBitmap(
		D2D1::SizeU(img.width, img.height),
		img.data.data(),
		img.width * img.channels,
		&bitmapProps,
		&pBitmap
	);


	if (FAILED(hr))
	{
		std::cerr << "CreateBitmap failed with HRESULT: 0x" << std::hex << hr << std::endl;
		SafeRelease(&pBitmap);
		return {};
	}

	img.backendHandle = static_cast<void*>(pBitmap);

	img.backendCleanup = [](void*& handle)
		{
			if (handle)
			{
				ID2D1Bitmap*& bmp = reinterpret_cast<ID2D1Bitmap*&>(handle);

				std::cout << "releasing bitmap at " << bmp << std::endl;

				SafeRelease(&bmp);

			}
		};


	return img;
}

void NewCanvasX::DrawImg(const Image& image, float x, float y, const Shapes::Rectangle* sourceRect, float angle, float scaleX, float scaleY)
{
	if (!image.isValid() || !dc) return;

	ID2D1Bitmap* pBitmap = reinterpret_cast<ID2D1Bitmap*> (image.backendHandle);
	if (!pBitmap) return;

	D2D1_RECT_F d2dSourceRect;

	float drawWidth, drawHeight;

	if (sourceRect)
	{
		int sx = m_max(0.0f, sourceRect->x);
		int sy = m_max(0.0f, sourceRect->y);

		if (sx >= image.width || sy >= image.height) return;

		int sw = m_min(sourceRect->width, image.width - sx);
		int sh = m_min(sourceRect->height, image.height - sy);

		float scaledSw = sw * scaleX;
		float scaledSh = sh * scaleY;

		d2dSourceRect = D2D1::RectF(
			static_cast<FLOAT>(sx),
			static_cast<FLOAT>(sy),
			static_cast<FLOAT>(sx + scaledSw),
			static_cast<FLOAT>(sy + scaledSh)
		);

		drawWidth = sw;
		drawHeight = sh;
	}

	else
	{
		d2dSourceRect = D2D1::RectF(0, 0, image.width, image.height);
		drawWidth = static_cast<float> (image.width * scaleX);
		drawHeight = static_cast<float>(image.height * scaleY);
	}

	D2D1_MATRIX_3X2_F transform =
		D2D1::Matrix3x2F::Translation(-drawWidth * 0.5f, -drawHeight * 0.5f) *
		D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(0, 0)) *
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
		D2D1::Matrix3x2F::Translation(x + drawWidth * 0.5f, y + drawHeight * 0.5);

	dc->SetTransform(transform);

	D2D1_RECT_F destRect = D2D1::RectF(0, 0, drawWidth, drawHeight);
	dc->DrawBitmap(pBitmap, destRect, image.alpha, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &d2dSourceRect);

	dc->SetTransform(D2D1::Matrix3x2F::Identity());
}

void NewCanvasX::DrawTxt(const std::string& text, Shapes::Rectangle textArea, Color color, float angle, float scaleX, float scaleY)
{
	if (!dc || !pTextFormat)
		return;

	std::wstring wText(text.begin(), text.end());

	nSetBrushColor(color);

	D2D1_RECT_F layoutRect = D2D1::RectF(0.0f, 0.0f, textArea.width, textArea.height);

	D2D1_POINT_2F center = D2D1::Point2F(
		textArea.x + textArea.width / 2.0f,
		textArea.y + textArea.height / 2.0f
	);

	D2D1_MATRIX_3X2_F transform =
		D2D1::Matrix3x2F::Translation(-textArea.width * 0.5f, -textArea.height * 0.5f) *
		D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(0, 0)) *
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
		D2D1::Matrix3x2F::Translation(textArea.x + textArea.width * 0.5f, textArea.y + textArea.height * 0.5f);

	dc->SetTransform(transform);

	dc->DrawTextW(
		wText.c_str(),
		(UINT32)wText.length(),
		pTextFormat.Get(),
		layoutRect,
		brush.Get()
	);

	dc->SetTransform(D2D1::Matrix3x2F::Identity());
}



std::vector<Shapes::Rectangle> CropImage(const Image& img, int frameWidth, int frameHeight)
{
	std::vector<Shapes::Rectangle> frames;

	int cols = img.width / frameWidth;
	int rows = img.height / frameHeight;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			Shapes::Rectangle rect;
			rect.x = x * frameWidth;
			rect.y = y * frameHeight;
			rect.width = frameWidth;
			rect.height = frameHeight;

			frames.push_back(rect);
		}
	}
	return frames;
}

Shapes::Rectangle* PlayAnimation(Image& spriteSheet, std::vector<Shapes::Rectangle>& frames, int startFrame, int endFrame, float animationSpeed, float elapsedTime, float x, float y)
{
	if (frames.empty() || startFrame < 0 || endFrame >= static_cast<int>(frames.size())) return {};

	int frameCount = endFrame - startFrame + 1;

	int currentFrameIndex = startFrame + static_cast<int>(elapsedTime / animationSpeed) % frameCount;

	return &frames[currentFrameIndex];
}