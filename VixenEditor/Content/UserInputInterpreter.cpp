#include "pch.h"
#include "Content\UserInputInterpreter.h"

namespace VixenEditor {
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
UserInputInterpreter::UserInputInterpreter(VixLib::camera::Camera& camera) :
    m_shift_down(false),
    m_camera(camera) {
}


// ---------------------------------------------------------------------------
// Event Handlers
// ---------------------------------------------------------------------------
void UserInputInterpreter::onPointerPressed(Platform::Object^ sender,
                                        Windows::UI::Core::PointerEventArgs^ e) {
  if (e->CurrentPoint->Properties->IsMiddleButtonPressed) {
    if (this->m_shift_down) this->m_tracking_mode = Mode::Translating;
    else                    this->m_tracking_mode = Mode::Rotating;

    this->m_pointer_location_x = e->CurrentPoint->Position.X;
    this->m_pointer_location_y = e->CurrentPoint->Position.Y;
  }
}


void UserInputInterpreter::onPointerMoved(Platform::Object^ sender,
                                      Windows::UI::Core::PointerEventArgs^ e) {
  if (this->isTracking()) {
    float dx = e->CurrentPoint->Position.X - this->m_pointer_location_x;
    float dy = e->CurrentPoint->Position.Y - this->m_pointer_location_y;

    if (this->m_tracking_mode == Mode::Rotating)    this->m_camera.rotate(dx, dy);
    if (this->m_tracking_mode == Mode::Translating) this->m_camera.translate(dx, dy);

    this->m_pointer_location_x = e->CurrentPoint->Position.X;
    this->m_pointer_location_y = e->CurrentPoint->Position.Y;
  }
}


void UserInputInterpreter::onPointerReleased(Platform::Object^ sender,
                                         Windows::UI::Core::PointerEventArgs^ e) {
  this->m_tracking_mode = Mode::None;
}


void UserInputInterpreter::onPointerWheelChanged(Platform::Object^ sender, 
                                                 Windows::UI::Core::PointerEventArgs^ e) {
  this->m_camera.zoom(e->CurrentPoint->Properties->MouseWheelDelta);
}

// ---------------------------------------------------------------------------
void UserInputInterpreter::onKeyDown(Platform::Object^ sender,
                                     Windows::UI::Core::KeyEventArgs^ e) {
  if (e->VirtualKey == Windows::System::VirtualKey::Shift) {
    this->m_shift_down = true;
  }
}

void UserInputInterpreter::onKeyUp(Platform::Object^ sender,
                                   Windows::UI::Core::KeyEventArgs^ e) {
  if (e->VirtualKey == Windows::System::VirtualKey::Shift) {
    this->m_shift_down = false;
  }
}


void UserInputInterpreter::SnapXPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_camera.snapToAxis(VixLib::camera::Axis::X, VixLib::camera::Direction::POS);
}


void UserInputInterpreter::SnapXNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_camera.snapToAxis(VixLib::camera::Axis::X, VixLib::camera::Direction::NEG);
}

void UserInputInterpreter::SnapYPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_camera.snapToAxis(VixLib::camera::Axis::Y, VixLib::camera::Direction::POS);
}

void UserInputInterpreter::SnapYNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_camera.snapToAxis(VixLib::camera::Axis::Y, VixLib::camera::Direction::NEG);
}

void UserInputInterpreter::SnapZPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_camera.snapToAxis(VixLib::camera::Axis::Z, VixLib::camera::Direction::POS);
}

void UserInputInterpreter::SnapZNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
  this->m_camera.snapToAxis(VixLib::camera::Axis::Z, VixLib::camera::Direction::NEG);
}

} // VixenEditor