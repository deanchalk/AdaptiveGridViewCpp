#include "pch.h"
#include "AdaptiveGridView.h"
#include "AdaptiveHeightValueConverter.h"
#include "AdaptiveGridView.g.cpp"

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Data;

namespace winrt::AdaptiveGridViewControl::implementation
{
	DependencyProperty AdaptiveGridView::m_itemWidthProperty =
		DependencyProperty::Register(
			L"ItemWidth",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<winrt::AdaptiveGridViewControl::AdaptiveGridView>(),
			PropertyMetadata{ winrt::box_value(0.0), nullptr }
	);

	DependencyProperty AdaptiveGridView::m_itemHeightPropertyProperty =
		DependencyProperty::Register(
			L"ItemHeight",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<winrt::AdaptiveGridViewControl::AdaptiveGridView>(),
			PropertyMetadata{ winrt::box_value(0.0), nullptr }
	);

	DependencyProperty AdaptiveGridView::m_itemClickCommandProperty =
		DependencyProperty::Register(
			L"ItemClickCommand",
			winrt::xaml_typename<Input::ICommand>(),
			winrt::xaml_typename<winrt::AdaptiveGridViewControl::AdaptiveGridView>(),
			PropertyMetadata{ nullptr, nullptr }
	);

	DependencyProperty AdaptiveGridView::m_oneRowModeEnabledProperty =
		DependencyProperty::Register(
			L"OneRowModeEnabled",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<winrt::AdaptiveGridViewControl::AdaptiveGridView>(),
			PropertyMetadata{ winrt::box_value(false), PropertyChangedCallback{ &AdaptiveGridView::OnOneRowModeEnabledChanged } }
	);

	DependencyProperty AdaptiveGridView::m_desiredWidthProperty =
		DependencyProperty::Register(
			L"DesiredWidth",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<winrt::AdaptiveGridViewControl::AdaptiveGridView>(),
			PropertyMetadata{ winrt::box_value(0.0), PropertyChangedCallback{ &AdaptiveGridView::OnDesiredWidthChanged } }
	);

	DependencyProperty AdaptiveGridView::m_stretchContentForSingleRowProperty =
		DependencyProperty::Register(
			L"StretchContentForSingleRow",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<winrt::AdaptiveGridViewControl::AdaptiveGridView>(),
			PropertyMetadata{ winrt::box_value(false), PropertyChangedCallback{ &AdaptiveGridView::OnStretchContentForSingleRowPropertyChanged } }
	);

	AdaptiveGridView::AdaptiveGridView()
	{
		IsTabStop(false);
		SizeChanged({ this, &AdaptiveGridView::OnSizeChanged });
		ItemClick({ this, &AdaptiveGridView::OnItemClicked });
		Items().VectorChanged({ this, &AdaptiveGridView::OnVectorChanged });
		Loaded({ this, &AdaptiveGridView::OnGridLoaded });
		UseLayoutRounding(false);
	}

	void AdaptiveGridView::OnGridLoaded(IInspectable const& /*sender*/, RoutedEventArgs const& /*e */)
	{
		m_isLoaded = true;
		DetermineOneRowMode();
	}

	void AdaptiveGridView::OnGridUnloaded(IInspectable const& /*sender*/, RoutedEventArgs const& /*e */)
	{
		m_isLoaded = false;
	}

	void AdaptiveGridView::OnVectorChanged(IInspectable const& /*sender*/, IVectorChangedEventArgs const& /*e */)
	{
		if (!isnan(ActualWidth()))
		{
			RecalculateLayout(ActualWidth());
		}
	}

	void AdaptiveGridView::OnItemClicked(IInspectable const& sender, ItemClickEventArgs const& e)
	{
		auto gridView = sender.as<AdaptiveGridView>();
		auto cmd = gridView->ItemClickCommand();
		if (cmd != nullptr)
		{
			if (cmd.CanExecute(e.ClickedItem()))
			{
				cmd.Execute(e.ClickedItem());
			}
		}
	}

	void AdaptiveGridView::OnSizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e)
	{
		auto gridView = sender.as<AdaptiveGridView>();
		auto oldWidth = e.PreviousSize().Width;
		auto newWidth = e.NewSize().Width;
		if (gridView->HorizontalAlignment() == HorizontalAlignment::Stretch)
		{
			auto prevColumns = gridView->CalculateColumns(oldWidth, gridView->DesiredWidth());
			auto newColumns = gridView->CalculateColumns(newWidth, gridView->DesiredWidth());
			if (prevColumns != newColumns)
			{
				RecalculateLayout(newWidth);
			}
		}
		else if (oldWidth != newWidth)
		{
			RecalculateLayout(newWidth);
		}
	}

	void AdaptiveGridView::PrepareContainerForItemOverride(DependencyObject const& obj, IInspectable const& item)
	{
		this->AdaptiveGridViewT<AdaptiveGridView>::PrepareContainerForItemOverride(obj, item);
		auto element = obj.try_as<FrameworkElement>();
		if (element)
		{
			auto heightBinding = Binding();
			heightBinding.Source(*this);
			heightBinding.Path(PropertyPath(L"ItemHeight"));
			heightBinding.Mode(BindingMode::TwoWay);

			auto widthBinding = Binding();
			widthBinding.Source(*this);
			widthBinding.Path(PropertyPath(L"ItemWidth"));
			widthBinding.Mode(BindingMode::TwoWay);

			element.SetBinding(FrameworkElement::HeightProperty(), heightBinding);
			element.SetBinding(FrameworkElement::WidthProperty(), widthBinding);
		}

		auto contentControl = obj.try_as<ContentControl>();
		if (contentControl)
		{
			contentControl.HorizontalContentAlignment(HorizontalAlignment::Stretch);
			contentControl.VerticalContentAlignment(VerticalAlignment::Stretch);
		}

		if (m_needContainerMarginForLayout)
		{
			m_needContainerMarginForLayout = false;
			RecalculateLayout(ActualWidth());
		}
	}

	int AdaptiveGridView::CalculateColumns(double containerWidth, double itemWidth)
	{
		auto columns = (int)round(containerWidth / itemWidth);
		if (columns == 0)
		{
			columns = 1;
		}

		return columns;
	};

	void AdaptiveGridView::OnDesiredWidthChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& /*e */)
	{
		auto gridView = d.try_as<AdaptiveGridView>();
		if (gridView)
		{
			gridView->RecalculateLayout(gridView->ActualWidth());
		}
	}

	void AdaptiveGridView::OnOneRowModeEnabledChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& /*e */)
	{
		auto gridView = d.try_as<AdaptiveGridView>();
		if (gridView)
		{
			gridView->DetermineOneRowMode();
		}
	}

	void AdaptiveGridView::OnStretchContentForSingleRowPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& /*e */)
	{
		auto gridView = d.try_as<AdaptiveGridView>();
		if (gridView)
		{
			gridView->RecalculateLayout(gridView->ActualWidth());
		}
	}

	void AdaptiveGridView::RecalculateLayout(double containerWidth)
	{
		auto itemsPanel = ItemsPanelRoot().as<Panel>();
		auto panelMargin = itemsPanel != nullptr ? itemsPanel.Margin().Left + itemsPanel.Margin().Right : 0;
		auto padding = Padding().Left + Padding().Right;
		auto border = BorderThickness().Left + BorderThickness().Right;
		containerWidth = containerWidth - padding - panelMargin - border;
		if (containerWidth > 0)
		{
			auto newWidth = CalculateItemWidth(containerWidth);
			ItemWidth(floor(newWidth));
		}
	}

	void AdaptiveGridView::DetermineOneRowMode()
	{
		if (!m_isLoaded)
		{
			return;
		}

		auto itemsWrapGridPanel = ItemsPanelRoot().try_as<ItemsWrapGrid>();
		if (!itemsWrapGridPanel)
		{
			return;
		}

		if (OneRowModeEnabled())
		{
			auto b = Binding();
			b.Source(*this);
			b.Path(PropertyPath(L"ItemHeight"));
			b.Converter(winrt::AdaptiveGridViewControl::AdaptiveHeightValueConverter());
			b.ConverterParameter(*this);

			if (itemsWrapGridPanel)
			{
				m_savedOrientation = itemsWrapGridPanel.Orientation();
				itemsWrapGridPanel.Orientation(Orientation::Vertical);
			}

			SetBinding(FrameworkElement::MaxHeightProperty(), b);

			m_savedHorizontalScrollMode = ScrollViewer::GetHorizontalScrollMode(*this);
			m_savedVerticalScrollMode = ScrollViewer::GetVerticalScrollMode(*this);
			m_savedHorizontalScrollBarVisibility = ScrollViewer::GetHorizontalScrollBarVisibility(*this);
			m_savedVerticalScrollBarVisibility = ScrollViewer::GetVerticalScrollBarVisibility(*this);
			m_needToRestoreScrollStates = true;

			winrt::Windows::UI::Xaml::Controls::ScrollViewer::SetVerticalScrollMode(*this, ScrollMode::Disabled);
			winrt::Windows::UI::Xaml::Controls::ScrollViewer::SetVerticalScrollBarVisibility(*this, ScrollBarVisibility::Hidden);
			winrt::Windows::UI::Xaml::Controls::ScrollViewer::SetHorizontalScrollBarVisibility(*this, ScrollBarVisibility::Visible);
			winrt::Windows::UI::Xaml::Controls::ScrollViewer::SetHorizontalScrollMode(*this, ScrollMode::Enabled);
		}
		else
		{
			ClearValue(FrameworkElement::MaxHeightProperty());

			if (!m_needToRestoreScrollStates)
			{
				return;
			}

			m_needToRestoreScrollStates = false;

			if (itemsWrapGridPanel)
			{
				itemsWrapGridPanel.Orientation(m_savedOrientation);
			}

			ScrollViewer::SetVerticalScrollMode(*this, m_savedVerticalScrollMode);
			ScrollViewer::SetVerticalScrollBarVisibility(*this, m_savedVerticalScrollBarVisibility);
			ScrollViewer::SetHorizontalScrollBarVisibility(*this, m_savedHorizontalScrollBarVisibility);
			ScrollViewer::SetHorizontalScrollMode(*this, m_savedHorizontalScrollMode);
		}
	}

	void AdaptiveGridView::OnApplyTemplate()
	{
		this->AdaptiveGridViewT<AdaptiveGridView>::OnApplyTemplate();
		OnOneRowModeEnabledChanged(*this, nullptr);
	}

	double AdaptiveGridView::CalculateItemWidth(double containerWidth)
	{
		if (isnan(DesiredWidth()))
		{
			return DesiredWidth();
		}
		int columns = CalculateColumns(containerWidth, DesiredWidth());
		auto items = Items();
		int count = items.Size();
		if (items != nullptr && count > 0 && count < columns && StretchContentForSingleRow())
		{
			columns = count;
		}
		Thickness fallbackThickness = Thickness();
		auto itemMargin = AdaptiveHeightValueConverter::GetItemMargin(*this, fallbackThickness);
		if (itemMargin == fallbackThickness)
		{
			m_needContainerMarginForLayout = true;
		}

		return (containerWidth / columns) - itemMargin.Left - itemMargin.Right;
	}
}
