//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"

using namespace VixenEditor;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;

DirectXPage::DirectXPage():
	m_windowVisible(true),
	m_coreInput(nullptr),
  m_display_metrics(VixCommon::DisplayMetrics(false,
                                              192.0f,
                                              1920.0f,
                                              1080.0f)) { //,
  //m_key_down_handler(),
  //m_key_up_handler() {
	InitializeComponent();

  //m_key_down_handler = ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyDown);
  //m_key_up_handler   = ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyUp);

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged += 
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);

	swapChainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_shared<VixCommon::DeviceResources>(this->m_display_metrics);
	m_deviceResources->setSwapChainPanel(swapChainPanel);

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this] (IAsyncAction ^) {
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
			);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);
    m_coreInput->PointerWheelChanged += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerWheelChanged);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<VixenEditorMain>(new VixenEditorMain(m_deviceResources));
	m_main->startRenderLoop();
}

DirectXPage::~DirectXPage()
{
	// Stop rendering and processing events on destruction.
	m_main->stopRenderLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
}

// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->getCriticalSection());
	m_deviceResources->trim();

	// Stop rendering when the app is suspended.
	m_main->stopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	m_main->startRenderLoop();
}

void VixenEditor::DirectXPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	CoreWindow^ window = Window::Current->CoreWindow;
  window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyDown); //this->m_key_down_handler;
  window->KeyUp   += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyUp); //this->m_key_up_handler; 
}


// TODO FIX THIS MEMORY LEAK
void VixenEditor::DirectXPage::Page_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	CoreWindow^ window = Window::Current->CoreWindow;
  //window->KeyDown -= this->m_key_down_handler; //ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyDown);
  //window->KeyUp   -= this->m_key_up_handler; //ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyUp);

}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
	{
		m_main->startRenderLoop();
	}
	else
	{
		m_main->stopRenderLoop();
	}
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->getCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->setDPI(sender->LogicalDpi);
	m_main->createWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->getCriticalSection());
	m_deviceResources->setCurrentOrientation(sender->CurrentOrientation);
	m_main->createWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->getCriticalSection());
	m_deviceResources->validateDevice();
}

// Called when the app bar button is clicked.
void DirectXPage::AppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	// Use the app bar if it is appropriate for your app. Design the app bar, 
	// then fill in event handlers (like this one).
}


void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e) {
	// When the pointer is pressed begin tracking the pointer movement.
  m_main->system_warden.getInputInterpreter().onPointerPressed(sender, e);
}


void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e) {
  m_main->system_warden.getInputInterpreter().onPointerMoved(sender, e);
}


void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e) {
  m_main->system_warden.getInputInterpreter().onPointerReleased(sender, e);
}


void DirectXPage::OnPointerWheelChanged(Object^ sender, PointerEventArgs^ e) {
  m_main->system_warden.getInputInterpreter().onPointerWheelChanged(sender, e);
}


void DirectXPage::OnKeyDown(CoreWindow^ sender, KeyEventArgs^ e) {
  m_main->system_warden.getInputInterpreter().onKeyDown(sender, e);
}


void DirectXPage::OnKeyUp(CoreWindow^ sender, KeyEventArgs^ e) {
  m_main->system_warden.getInputInterpreter().onKeyUp(sender, e);
}


void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->getCriticalSection());
	m_deviceResources->setCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->createWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->getCriticalSection());
	m_deviceResources->setLogicalSize(e->NewSize);
	m_main->createWindowSizeDependentResources();
}

void VixenEditor::DirectXPage::SnapXPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_main->system_warden.getInputInterpreter().SnapXPosButton_Click(sender, e);
}


void VixenEditor::DirectXPage::SnapXNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_main->system_warden.getInputInterpreter().SnapXNegButton_Click(sender, e);
}

void VixenEditor::DirectXPage::SnapYPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_main->system_warden.getInputInterpreter().SnapYPosButton_Click(sender, e);

}

void VixenEditor::DirectXPage::SnapYNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_main->system_warden.getInputInterpreter().SnapYNegButton_Click(sender, e);

}

void VixenEditor::DirectXPage::SnapZPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_main->system_warden.getInputInterpreter().SnapZPosButton_Click(sender, e);

}

void VixenEditor::DirectXPage::SnapZNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_main->system_warden.getInputInterpreter().SnapZNegButton_Click(sender, e);
}
