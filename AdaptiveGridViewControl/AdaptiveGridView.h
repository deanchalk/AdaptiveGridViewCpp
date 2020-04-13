#pragma once
#include "AdaptiveGridView.g.h"

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::AdaptiveGridViewControl::implementation
{
    struct AdaptiveGridView : AdaptiveGridViewT<AdaptiveGridView>
    {
        static DependencyProperty ItemClickCommandProperty() { return m_itemClickCommandProperty; }
        static DependencyProperty ItemHeightProperty() { return m_itemHeightPropertyProperty; }
        static DependencyProperty OneRowModeEnabledProperty() { return m_oneRowModeEnabledProperty; }
        static DependencyProperty ItemWidthProperty() { return m_itemWidthProperty; }
        static DependencyProperty DesiredWidthProperty() { return m_desiredWidthProperty; }
        static DependencyProperty StretchContentForSingleRowProperty() { return m_stretchContentForSingleRowProperty; }

        AdaptiveGridView();

        void OnSizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);
        void OnItemClicked(IInspectable const& sender, ItemClickEventArgs const& e);
        void OnVectorChanged(IInspectable const& sender, IVectorChangedEventArgs const& e);
        void OnGridLoaded(IInspectable const& sender, RoutedEventArgs const& e);
        void OnGridUnloaded(IInspectable const& sender, RoutedEventArgs const& e);

        Input::ICommand ItemClickCommand()
        {
            return (GetValue(m_itemClickCommandProperty)).as<Input::ICommand>();
        }
        void ItemClickCommand(Input::ICommand const& value)
        {
            SetValue(m_itemClickCommandProperty, value);
        }

        bool OneRowModeEnabled()
        {
            return winrt::unbox_value<bool>(GetValue(m_oneRowModeEnabledProperty));
        }
        void OneRowModeEnabled(bool value)
        {
            SetValue(m_oneRowModeEnabledProperty, winrt::box_value(value));
        }

        double ItemHeight()
        {
            return winrt::unbox_value<double>(GetValue(m_itemHeightPropertyProperty));
        }
        void ItemHeight(double value)
        {
            SetValue(m_itemHeightPropertyProperty, winrt::box_value(value));
        }

        double ItemWidth()
        {
            return winrt::unbox_value<double>(GetValue(m_itemWidthProperty));
        }
        void ItemWidth(double value)
        {
            SetValue(m_itemWidthProperty, winrt::box_value(value));
        }

        double DesiredWidth()
        {
            auto value = GetValue(m_desiredWidthProperty);
            return winrt::unbox_value<double>(value);
        }
        void DesiredWidth(double value)
        {
            SetValue(m_desiredWidthProperty, winrt::box_value(value));
        }

        bool StretchContentForSingleRow()
        {
            return winrt::unbox_value<bool>(GetValue(m_stretchContentForSingleRowProperty));
        }
        void StretchContentForSingleRow(bool value)
        {
            SetValue(m_stretchContentForSingleRowProperty, winrt::box_value(value));
        }

        static void OnDesiredWidthChanged(DependencyObject const&, DependencyPropertyChangedEventArgs const&);
        static void OnOneRowModeEnabledChanged(DependencyObject const&, DependencyPropertyChangedEventArgs const&);
        static void OnStretchContentForSingleRowPropertyChanged(DependencyObject const&, DependencyPropertyChangedEventArgs const&);

        void PrepareContainerForItemOverride(Windows::UI::Xaml::DependencyObject const& obj, Windows::Foundation::IInspectable const& item);
        void OnApplyTemplate();

    private:
        static DependencyProperty m_itemClickCommandProperty;
        static DependencyProperty m_itemHeightPropertyProperty;
        static DependencyProperty m_oneRowModeEnabledProperty;
        static DependencyProperty m_itemWidthProperty;
        static DependencyProperty m_desiredWidthProperty;
        static DependencyProperty m_stretchContentForSingleRowProperty;
        static int CalculateColumns(double containerWidth, double itemWidth);
        void RecalculateLayout(double containerWidth);
        double CalculateItemWidth(double containerWidth);
        bool m_needContainerMarginForLayout{ false };
        void DetermineOneRowMode();
        bool m_isLoaded{ false };
        Orientation m_savedOrientation{ Orientation::Horizontal };
        ScrollMode m_savedVerticalScrollMode{ ScrollMode::Auto };
        ScrollMode m_savedHorizontalScrollMode{ ScrollMode::Auto };
        ScrollBarVisibility m_savedVerticalScrollBarVisibility{ ScrollBarVisibility::Auto };
        ScrollBarVisibility m_savedHorizontalScrollBarVisibility{ ScrollBarVisibility::Auto };
        bool m_needToRestoreScrollStates{ false };
    };
}
namespace winrt::AdaptiveGridViewControl::factory_implementation
{
    struct AdaptiveGridView : AdaptiveGridViewT<AdaptiveGridView, implementation::AdaptiveGridView>
    {
    };
}
