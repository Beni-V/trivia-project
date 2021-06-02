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
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        public Communicator communicator { get; set; }

        public JoinRoomWindow(Communicator communicator)
        {
            InitializeComponent();
            this.communicator = communicator;
        }

        private void Join_Click(object sender, RoutedEventArgs e)
        {
            var joinRoomRequest = new JoinRoomRequest
            {
                roomId = Convert.ToString(1) //Rooms.Items....
            };

            communicator.SerializeAndSendMsg(JsonConvert.SerializeObject(joinRoomRequest), 504);
            communicator.RecvAndDeserializeMsg();

            new MenuWindow(this.communicator).Show(); //just for now
            this.Close();
        }

        private void Return_Click(object sender, RoutedEventArgs e)
        {
            new MenuWindow(this.communicator).Show();
            this.Close();
        }

        public class JoinRoomRequest
        {
            public string roomId { get; set; }
        }
    }
}
