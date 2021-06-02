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
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        public Communicator communicator { get; set; }

        public CreateRoomWindow(Communicator communicator)
        {
            InitializeComponent();
            this.communicator = communicator;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            var createRoomRequest = new CreateRoomRequest
            {
                roomName = RoomNameBox.Text,
                questionCount = QuestionsNumBox.Text,
                maxUsers = PlayersNumBox.Text,
                answerTimeout = TimeBox.Text
            };

            communicator.SerializeAndSendMsg(JsonConvert.SerializeObject(createRoomRequest), 504);
            communicator.RecvAndDeserializeMsg();

            new MenuWindow(this.communicator).Show();
            this.Close();
        }

        private void Return_Click(object sender, RoutedEventArgs e)
        {
            new MenuWindow(this.communicator).Show();
            this.Close();
        }

        public class CreateRoomRequest
        {
            public string roomName { get; set; }
            public string questionCount { get; set; }
            public string maxUsers { get; set; }
            public string answerTimeout { get; set; }

        }
    }
}
