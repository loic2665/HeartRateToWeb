using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using HRServer;

namespace HeartRateToWeb
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        private HeartRateServer _client = null;

        public HeartRateServer Client
        {
            get { return _client; }
            set { _client = value; }
        }

        public MainWindow()
        {
            InitializeComponent();

            Client = new HeartRateServer(6547);
            Receiver.DataContext = Client;
            ListboxIPs.ItemsSource = Client.Server.Prefixes;

        }

        private void ButtonServer_Click(object sender, RoutedEventArgs e)
        {

            String status = "OFF";

            if (Client.IsServerStarted) {
                status = "OFF";
                ButtonServer.Content = "START SERVER";
                BoxServerStatus.Background = new SolidColorBrush(Color.FromRgb(231, 76, 60)); // #e74c3c rgb(231, 76, 60)
                
                Client.StopServer();
            }
            else
            {
                try
                {
                    Client.StartServer();
                    status = "ON";
                    ButtonServer.Content = "STOP SERVER";
                    BoxServerStatus.Background = new SolidColorBrush(Color.FromRgb(44, 204, 113)); // #2ecc71

                }
                catch (Exception exp)
                {
                    MessageBox.Show("Please lauch the program as admin, to create the Web Server.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }

            LabelServerStatus.Content = status;
        }

        private void CopyActiveIP_Click(object sender, RoutedEventArgs e)
        {
            if (ListboxIPs.SelectedItem != null)
            {
                String ip = ListboxIPs.SelectedItem.ToString();
                Clipboard.SetText(ip);
            }
            else
            {
                MessageBox.Show("Please select an address.", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
            } 
        }
    }
}
