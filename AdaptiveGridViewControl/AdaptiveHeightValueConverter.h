#pragma once
#include "AdaptiveHeightValueConverter.g.h"

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Interop;

namespace winrt::AdaptiveGridViewControl::implementation
{
    struct AdaptiveHeightValueConverter : AdaptiveHeightValueConverterT<AdaptiveHeightValueConverter>
    {
        static Thickness GetItemMargin(GridView const& view, Thickness const& fallback);
        IInspectable Convert(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language);
        IInspectable ConvertBack(IInspectable const& value, TypeName const& targetType, IInspectable const& parameter, hstring const& language);
        Thickness GetThickness();
        void SetThickness(Thickness);
    private:
        Thickness m_thickness = Thickness{ 0, 0, 4, 4 };
    };
}
namespace winrt::AdaptiveGridViewControl::factory_implementation
{
    struct AdaptiveHeightValueConverter : AdaptiveHeightValueConverterT<AdaptiveHeightValueConverter, implementation::AdaptiveHeightValueConverter>
    {
    };
}
