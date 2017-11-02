#pragma once

#include "VixLib\include\camera\Camera.h"

namespace VixenEditor {

/// <summary> Mode describes the tracking states. </summary>
enum class Mode { None, Rotating, Translating };


/// <summary> 
/// UserInputInterpeter interprets the user input obtained from DirectXPage.xaml.cpp
/// and propagates it to the relevant subsystem.
/// </summary>
class UserInputInterpreter {
public:
  // --------------------------------------------------------------------------
  // Constructor
  // --------------------------------------------------------------------------
  /// <summary> 
  /// Construct a new UserInputInterpreter with a reference to the specified <paramref name="camera"/>.
  /// </summary>
  /// <param name="camera">The camera of this new UserInputInterpeter</param>
  UserInputInterpreter(VixLib::camera::Camera& camera);

  // --------------------------------------------------------------------------
  // Checkers
  // --------------------------------------------------------------------------
  /// <summary> Check whether this UserInputInterpreter is tracking mouse movement</summary>
  /// <returns> Whether this UserInputInterpreter is tracking mouse movement</summary>
  bool isTracking() { return (this->m_tracking_mode != Mode::None); }

  // --------------------------------------------------------------------------
  // Event Handlers
  // --------------------------------------------------------------------------
  void onPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
  void onPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
  void onPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
  void onPointerWheelChanged(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

  // --------------------------------------------------------------------------
  void onKeyDown(Platform::Object^ sender, Windows::UI::Core::KeyEventArgs^ e);
  void onKeyUp(Platform::Object^ sender, Windows::UI::Core::KeyEventArgs^ e);

  // --------------------------------------------------------------------------
  void SnapXPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
  void SnapXNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
  void SnapYPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
  void SnapYNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
  void SnapZPosButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
  void SnapZNegButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
private:
  /// <summary> The mode that this UserInputInterpreter is currently tracking</summary>
  Mode m_tracking_mode;

  /// <summary> Whether the shift button is currently pressed</summary>
  bool m_shift_down;
  /// <summary> The x location of the pointer at this moment, if m_shift_down</summary>
  float m_pointer_location_x;
  /// <summary> The y location of the pointer at this moment, if m_shift_down</summary>
  float m_pointer_location_y;

  /// <summary>The camera of this UserInputInterpreter</summary>
  VixLib::camera::Camera& m_camera;
};

}
