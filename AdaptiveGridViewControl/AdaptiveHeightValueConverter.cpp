#include "pch.h"
#include "AdaptiveHeightValueConverter.h"
#include "AdaptiveHeightValueConverter.g.cpp"

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::UI::Xaml::Interop;

namespace winrt::AdaptiveGridViewControl::implementation
{
    IInspectable AdaptiveHeightValueConverter::Convert(IInspectable const& value, TypeName const& /*targetType*/, IInspectable const& parameter, hstring const& /*language*/)
    {
        if (value == nullptr)
        {
            return winrt::box_value(NAN);
        }

        auto gridView = parameter.as<AdaptiveGridView>();
        if (gridView == nullptr)
        {
            return value;
        }

        auto height = winrt::unbox_value<double>(value);
        Thickness padding = gridView.Padding();
        auto margin = GetItemMargin(gridView, GetThickness());
        height = height + margin.Top + margin.Bottom + padding.Top + padding.Bottom;

        return winrt::box_value(height);

    }

    Thickness AdaptiveHeightValueConverter::GetItemMargin(GridView const& view, Thickness const& fallback)
    {
        auto containerStyle = view.ItemContainerStyle();
        if (containerStyle == nullptr || containerStyle.Setters() == nullptr)
        {
            return fallback;
        }
        else
        {
            auto setters = containerStyle.Setters().GetView();
            auto setterPtr = std::find_if(begin(setters), end(setters), [](SetterBase const& item)
                {
                    return item.as<Setter>().Property() == FrameworkElement::MarginProperty();
                });

            if (*setterPtr != nullptr)
            {
                SetterBase first = *setterPtr;
                Setter setter = first.as<Setter>();
                IInspectable setterVal = setter.Value();
                Thickness setterThickness = winrt::unbox_value<Thickness>(setterVal);
                return setterThickness;
            }
            else if (view.Items().Size() > 0)
            {
                auto container = view.ContainerFromIndex(0).as<GridViewItem>();
                if (container != nullptr)
                {
                    return container.Margin();
                }
                else
                {
                    return fallback;
                }
            }
            else
            {
                return fallback;
            }
        }
    }

    IInspectable AdaptiveHeightValueConverter::ConvertBack(IInspectable const& /*value*/, TypeName const& /*targetType*/, IInspectable const& /*parameter*/, hstring const& /*language*/)
    {
        throw hresult_not_implemented();
    }

    Thickness AdaptiveHeightValueConverter::GetThickness()
    {
        return m_thickness;
    }

    void AdaptiveHeightValueConverter::SetThickness(Thickness value)
    {
        m_thickness = value;
    }
}
