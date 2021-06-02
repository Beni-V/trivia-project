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

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for MyStatusWindow.xaml
    /// </summary>
    public partial class MyStatusWindow : Window
    {
        public Communicator communicator { get; set; }

        public MyStatusWindow(Communicator communicator)
        {
            InitializeComponent();
            this.communicator = communicator;
        }

        private void Return_Click(object sender, RoutedEventArgs e)
        {
            new MenuWindow(this.communicator).Show();
            this.Close();
        }
    }
}
