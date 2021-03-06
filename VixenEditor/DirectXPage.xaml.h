﻿//
// DirectXPage.xaml.h
// Declaration of the DirectXPage class.
//

#pragma once

#include "DirectXPage.g.h"

#include "VixCommon\include\DeviceResources.h"
#include "VixenEditorMain.h"

namespace VixenEditor
{
	/// <summary>
	/// A page that hosts a DirectX SwapChainPanel.
	/// </summary>
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

	private:
		// XAML low-level rendering event handler.
		void OnRendering(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		// Independent input handling functions.
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
    void OnPointerWheelChanged(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

    void OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e);
    void OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e);

		// Resources used to render the DirectX content in the XAML page background.
		std::shared_ptr<VixCommon::DeviceResources> m_deviceResources;
		std::unique_ptr<VixenEditorMain> m_main; 
		bool m_windowVisible;

    VixCommon::DisplayMetrics m_display_metrics;
    void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void Page_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    
    //TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^> ^ m_key_down_handler;
    //TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^> ^ m_key_up_handler;
    void SnapXPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void SnapXNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void SnapYPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void SnapYNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void SnapZPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void SnapZNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
  };
}

