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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public Communicator communicator{ get; set;}
        public MainWindow()
        { 
            InitializeComponent();
            this.communicator = new Communicator();
        }

        public MainWindow(Communicator communicator)
        {
            InitializeComponent();
            this.communicator = communicator;
        }

        private void Quit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            new SignupWindow(this.communicator).Show();
            this.Close();
        }

        private void Login_Click(object sender, RoutedEventArgs e)
        {
            var logInRequest = new LogInRequest
            {
                username = usernameBox.Text,
                password = passwordBox.Text
            };

            communicator.SerializeAndSendMsg(JsonConvert.SerializeObject(logInRequest), 201);
            Dictionary<string, string> answerMsg = communicator.RecvAndDeserializeMsg();

            if (answerMsg["code"] == Constants.ERROR_CODE)
            {
                Error.Text = answerMsg["message"];
            }
            else
            {
                this.communicator.username = logInRequest.username;
                new MenuWindow(this.communicator).Show();
                this.Close();
            }
        }

        public class LogInRequest
        {
            public string username { get; set; }
            public string password { get; set; }
        }
    }
}
