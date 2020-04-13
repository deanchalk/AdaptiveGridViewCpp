using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace SampleApp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            Setup();          
        }

        private async void Setup()
        {
            AdaptiveGridViewCtrl.ItemsSource = await new PhotosDataSource().GetItemsAsync();
            AdaptiveGridViewCtrl.ItemClick += AdaptiveGridViewControl_ItemClick;
            AdaptiveGridViewCtrl.SelectionChanged += AdaptiveGridViewControl_SelectionChanged;
        }

        private void AdaptiveGridViewControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            SelectedItemCountTextBlock.Text = AdaptiveGridViewCtrl.SelectedItems.Any()
                ? $"You have selected {AdaptiveGridViewCtrl.SelectedItems.Count} items."
                : "You haven't selected any items";
        }

        private async void AdaptiveGridViewControl_ItemClick(object sender, ItemClickEventArgs e)
        {
            if (e.ClickedItem != null)
            {
                await new MessageDialog($"You clicked {(e.ClickedItem as PhotoDataItem).Title}", "Item Clicked").ShowAsync();
            }
        }
    }
}
