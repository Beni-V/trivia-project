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
using Newtonsoft.Json;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for SignupWindow.xaml
    /// </summary>
    public partial class SignupWindow : Window
    {
        public Communicator communicator { get; set; }

        public SignupWindow(Communicator communicator)
        {
            InitializeComponent();
            this.communicator = communicator;
        }

        private void SignUp_Click(object sender, RoutedEventArgs e)
        {
            var signUpRequest = new SignUpRequest
            {
                username = UsernameBox.Text,
                password = PasswordBox.Text,
                email = EmailBox.Text
            };

            communicator.SerializeAndSendMsg(JsonConvert.SerializeObject(signUpRequest), 202);
            communicator.RecvAndDeserializeMsg();

            new MenuWindow(this.communicator).Show();
            this.Close();
        }

        private void Return_Click(object sender, RoutedEventArgs e)
        {
            new MainWindow(this.communicator).Show();
            this.Close();
        }

        public class SignUpRequest
        {
            public string username { get; set; }
            public string password { get; set; }
            public string email { get; set; }
        }
    }
}
