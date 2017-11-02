﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class SwapChainPanel;
                ref class Grid;
                ref class Button;
                ref class AppBar;
            }
        }
    }
}

namespace VixenEditor
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class DirectXPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::SwapChainPanel^ swapChainPanel;
        private: ::Windows::UI::Xaml::Controls::Grid^ ButtonsLayout;
        private: ::Windows::UI::Xaml::Controls::Button^ SnapXPosButton;
        private: ::Windows::UI::Xaml::Controls::Button^ SnapXNegButton;
        private: ::Windows::UI::Xaml::Controls::Button^ SnapYPosButton;
        private: ::Windows::UI::Xaml::Controls::Button^ SnapYNegButton;
        private: ::Windows::UI::Xaml::Controls::Button^ SnapZPosButton;
        private: ::Windows::UI::Xaml::Controls::Button^ SnapZNegButton;
        private: ::Windows::UI::Xaml::Controls::AppBar^ bottomAppBar;
    };
}
