#include "pch.h"
#include "SampleFpsTextRenderer.h"

#include "VixCommon\include\DirectXHelper.h"

using namespace Microsoft::WRL;

namespace VixenEditor {
// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
// Initializes D2D resources used for text rendering.
SampleFpsTextRenderer::SampleFpsTextRenderer(const std::shared_ptr<VixCommon::DeviceResources>& device_resources) : 
    m_text(L""),
    m_device_resources(device_resources) {
	ZeroMemory(&m_text_metrics, sizeof(DWRITE_TEXT_METRICS));

	// Create device independent resources
	ComPtr<IDWriteTextFormat> text_format;
	DX::ThrowIfFailed(
    m_device_resources->getDWriteFactory()->CreateTextFormat(
      L"Segoe UI",
      nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			32.0f,
			L"en-US",
			&text_format));

	DX::ThrowIfFailed(text_format.As(&(this->m_text_format)));

	DX::ThrowIfFailed(this->m_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

	DX::ThrowIfFailed(
    this->m_device_resources->getD2DFactory()->CreateDrawingStateBlock(&(this->m_state_block)));

	this->createDeviceDependentResources();
}


// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
// Updates the text to be displayed.
void SampleFpsTextRenderer::update(DX::StepTimer const& timer)
{
	// Update display text.
	uint32 fps = timer.GetFramesPerSecond();

	this->m_text = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";

	ComPtr<IDWriteTextLayout> text_layout;
	DX::ThrowIfFailed(this->m_device_resources->getDWriteFactory()->CreateTextLayout(
			m_text.c_str(),
			(uint32) m_text.length(),
			this->m_text_format.Get(),
			240.0f, // Max width of the input text.
			50.0f, // Max height of the input text.
			&text_layout));

  DX::ThrowIfFailed(text_layout.As(&(this->m_text_layout)));

  DX::ThrowIfFailed(this->m_text_layout->GetMetrics(&(this->m_text_metrics)));
}


// Renders a frame to the screen.
void SampleFpsTextRenderer::render() {
	ID2D1DeviceContext* context = this->m_device_resources->getD2DDeviceContext();
	Windows::Foundation::Size logical_size = m_device_resources->getLogicalSize();

	context->SaveDrawingState(this->m_state_block.Get());
	context->BeginDraw();

	// Position on the bottom right corner
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
		logical_size.Width  - this->m_text_metrics.layoutWidth,
		logical_size.Height - this->m_text_metrics.height);

	context->SetTransform(screenTranslation * 
                        this->m_device_resources->getOrientationTransform2D());

	DX::ThrowIfFailed(this->m_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING));

	context->DrawTextLayout(D2D1::Point2F(0.f, 0.f),
                          this->m_text_layout.Get(),
                          this->m_white_brush.Get());

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET) DX::ThrowIfFailed(hr);

	context->RestoreDrawingState(this->m_state_block.Get());
}


void SampleFpsTextRenderer::createDeviceDependentResources() {
	DX::ThrowIfFailed(this->m_device_resources->getD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), 
                                                                                    &(this->m_white_brush)));
}


void SampleFpsTextRenderer::releaseDeviceDependentResources() {
	this->m_white_brush.Reset();
}
} // VixenEditor