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
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Newtonsoft.Json.Linq;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public class SignUpRequest
    {
        public string username { get; set; }
        public string password { get; set; }
        public string email { get; set; }
    }
    public class SignInRequest
    {
        public string username { get; set; }
        public string password { get; set; }
    }
    public class CreateRoomRequest
    {
        public string roomName { get; set; }
        public int maxUsers { get; set; }
        public int questionCount { get; set; }
        public int answerTimeout { get; set; }
    }
    public class GetPlayersInRoomRequest
    {
        public int roomId { get; set; }
    }
    public class JoinRoomRequest
    {
        public int roomId { get; set; }
    }
    public partial class MainWindow : Window
    {
        public const int LOGIN_REQUEST = 201;
        public const int SIGNUP_REQUEST = 202;
        public const int CREATE_ROOM_REQUEST = 91;
        public const int GET_ROOMS_REQUEST = 92;
        public const int GET_PLAYERS_IN_ROOM_REQUEST = 93;
        public const int JOIN_ROOM_REQUEST = 94;
        public const int GET_STATISTICS_REQUEST = 95;
        public const int LOGOUT_REQUEST = 96;
        public const int CLOSE_ROOM_REQUEST = 97;
        public const int START_GAME_REQUEST = 98;
        public const int GET_ROOM_STATE_REQUEST = 99;
        public const int LEAVE_ROOM_REQUEST = 100;
        public const int SIGNUP_RESPONSE = 102;
        public const int ERROR_RESPONSE = 103;
        public const int BITS_IN_BYTE = 8;
        public const int SUCCSESS_RESPONSE_CODE = 1;
        public const int MINIMAL_BUFFER_SIZE = 40;
        public const int MESSAGE_CODE_LENGTH = 8; 
        public const int MESSAGE_LENGTH_LENGTH = 32;

        public Thread updateParticipantsForAdminThread;
        public Thread updateParticipantsForMemberThread;

        public static NetworkStream clientStream;

        public MainWindow()
        {
            StartClient();
            InitializeComponent();
        }

        public static void StartClient()
        {
            TcpClient client = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 5150);
            client.Connect(serverEndPoint);
            clientStream = client.GetStream();
        }

        static string ConcatenateSerializedMessageParts(string messageCode, string messageLength, string message)
        {
            while(messageCode.Length < MESSAGE_CODE_LENGTH)
            { 
                messageCode = messageCode.Insert(0, "0");
            }
            while (messageLength.Length < MESSAGE_LENGTH_LENGTH)
            {
                
                messageLength = messageLength.Insert(0, "0");
            }
            return messageCode + messageLength + message;
        }

        static string ToBinaryString(Encoding encoding, string text)
        {
            return string.Join("", encoding.GetBytes(text).Select(n => Convert.ToString(n, 2).PadLeft(8, '0')));
        }

        static void serializeAndSendMessage(int messageCode, string jsonDump)
        {
            string serializedJsonMessage = ToBinaryString(Encoding.UTF8, jsonDump);
            string serializedMessageCode = Convert.ToString(messageCode, 2);
            string serializedMessageLength = Convert.ToString(serializedJsonMessage.Length, 2);
            string message = ConcatenateSerializedMessageParts(serializedMessageCode, serializedMessageLength, serializedJsonMessage);
            byte[] buffer = new ASCIIEncoding().GetBytes(message);

            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();
        }

        static Dictionary<string, object> receiveAndDeserializeMessage()
        {
            byte[] headers = new byte[MINIMAL_BUFFER_SIZE];

            clientStream.Read(headers, 0, headers.Length);

            int messageLength = Convert.ToInt32(System.Text.Encoding.Default.GetString(headers.Skip(8).ToArray()), 2) * BITS_IN_BYTE;

            byte[] serializedJsonResponse = new byte[messageLength];
            clientStream.Read(serializedJsonResponse, 0, serializedJsonResponse.Length);

            string jsonDump = BinaryToASCII(System.Text.Encoding.Default.GetString(serializedJsonResponse));

            Dictionary<string, object> result = JsonConvert.DeserializeObject<Dictionary<string, object>>(jsonDump);

            return result;
        }
        private static int BinaryToDecimal(string bin)
        {
            int binLength = bin.Length;
            double dec = 0;

            for (int i = 0; i < binLength; ++i)
            {
                dec += ((byte)bin[i] - 48) * Math.Pow(2, ((binLength - i) - 1));
            }

            return (int)dec;
        }
        public static string BinaryToASCII(string bin)
        {
            string ascii = string.Empty;

            for (int i = 0; i < bin.Length; i += 8)
            {
                ascii += (char)BinaryToDecimal(bin.Substring(i, 8));
            }

            return ascii;
        }

        private void signInWellcomeButtonClick(object sender, RoutedEventArgs e)
        {
            wellcomeBorder.Visibility = Visibility.Hidden;
            signInBorder.Visibility = Visibility.Visible;
        }
        private void signUpWellcomeButtonClick(object sender, RoutedEventArgs e)
        {
            wellcomeBorder.Visibility = Visibility.Hidden;
            signUpBorder.Visibility = Visibility.Visible;
        }
        private void backSignInButtonClick(object sender, RoutedEventArgs e)
        {
            signInBorder.Visibility = Visibility.Hidden;
            wellcomeBorder.Visibility = Visibility.Visible;
        }
        private void backSignUpButtonClick(object sender, RoutedEventArgs e)
        {
            signUpBorder.Visibility = Visibility.Hidden;
            wellcomeBorder.Visibility = Visibility.Visible;
        }
        private void signUpButtonClick(object sender, RoutedEventArgs e)
        {
            signUpErrorBox.Foreground = Brushes.Red;
            if (signUpUserNameBox.Text.Length < 3)
            {
                signUpErrorBox.Text = "User name must be at least 3 characters";
            }
            else if (signUpPasswordBox.Password.Length < 4)
            {
                signUpErrorBox.Text = "Password must be at least 4 characters";
            }
            else if (signUpPasswordBox.Password != signUpRepeatPasswordBox.Password)
            {
                signUpErrorBox.Text = "Passwords doesn't match";
            }
            else if (!signUpEmailBox.Text.Contains("@") || !signUpEmailBox.Text.Contains("."))
            {
                signUpErrorBox.Text = "Incorrect email format";
            }
            else
            {
                signUpErrorBox.Text = "";
                SignUpRequest signUpRequest = new SignUpRequest
                {
                    username = signUpUserNameBox.Text, password = signUpPasswordBox.Password, email = signUpEmailBox.Text
                };

                string jsonDump = JsonConvert.SerializeObject(signUpRequest, Formatting.Indented);

                serializeAndSendMessage(SIGNUP_REQUEST, jsonDump);
                Dictionary<string, object> response = receiveAndDeserializeMessage();

                if (response.ContainsKey("status") && (string)response["status"] == "1")
                {
                    signUpErrorBox.Foreground = Brushes.Green;
                    signUpErrorBox.Text = "Account successfully registered";
                }
                else if (response.ContainsKey("message"))
                {
                    signUpErrorBox.Text = (string)response["message"];
                }
            }
        }
        private void signInButtonClick(object sender, RoutedEventArgs e)
        {
            signInErrorBox.Text = "";

            SignInRequest signInRequest = new SignInRequest
            {
                username = signInUserNameBox.Text, password = signInPasswordBox.Password
            };

            string jsonDump = JsonConvert.SerializeObject(signInRequest, Formatting.Indented);

            serializeAndSendMessage(LOGIN_REQUEST, jsonDump);
            Dictionary<string, object> response = receiveAndDeserializeMessage();

            if (response.ContainsKey("status") && (string)response["status"] == "1")
            {
                signInBorder.Visibility = Visibility.Hidden;
                mainMenuBorder.Visibility = Visibility.Visible;
            }
            else if (response.ContainsKey("message"))
            {
                signInErrorBox.Text = (string)response["message"];
            }
        }
        private void mainMenuCreateRoomButtonClick(object sender, RoutedEventArgs e)
        {
            mainMenuBorder.Visibility = Visibility.Hidden;
            createRoomBorder.Visibility = Visibility.Visible;
        }
        private void mainMenuJoinRoomButtonClick(object sender, RoutedEventArgs e)
        {
            mainMenuBorder.Visibility = Visibility.Hidden;
            roomsListBorder.Visibility = Visibility.Visible;

            refreshRoomList();

        }

        public void refreshRoomList()
        {
            serializeAndSendMessage(GET_ROOMS_REQUEST, "");
            Dictionary<string, object> response = receiveAndDeserializeMessage();

            if (response.ContainsKey("status") && (string)response["status"] == "1")
            {
                roomsList.Items.Clear();
                Dictionary<int, string> rooms = new Dictionary<int, string>();

                JArray Jrooms = (JArray)response["rooms"];


                foreach (JArray room in Jrooms)
                {
                    rooms.Add((int)room[0], (string)room[1]);
                }

                foreach (KeyValuePair<int, string> roomInDict in rooms)
                {
                    roomsList.Items.Add(new Label().Content = $"{roomInDict.Key}: {roomInDict.Value}");
                }

            }
            else if (response.ContainsKey("message"))
            {
                roomsListErrorBox.Text = (string)response["message"];
            }
        }
        private void refreshRoomsListButtonClick(object sender, RoutedEventArgs e)
        {
            refreshRoomList();
        }
        private void roomListBackButtonClick(object sender, RoutedEventArgs e)
        {
            roomsListBorder.Visibility = Visibility.Hidden;
            mainMenuBorder.Visibility = Visibility.Visible;
        }
        private void mainMenuShowStatusButtonClick(object sender, RoutedEventArgs e)
        {
           
        }
        private void roomListJoinRoomButtonClick(object sender, RoutedEventArgs e)
        {
            signInErrorBox.Text = "";

            JoinRoomRequest joinRoomRequest = new JoinRoomRequest
            {
                roomId = Convert.ToInt32(roomsList.SelectedItem.ToString()[0].ToString())
            };

            string jsonDump = JsonConvert.SerializeObject(joinRoomRequest, Formatting.Indented);

            serializeAndSendMessage(JOIN_ROOM_REQUEST, jsonDump);
            Dictionary<string, object> response = receiveAndDeserializeMessage();

            if (response.ContainsKey("status") && (string)response["status"] == "1")
            {
                roomMemberPanelErrorBox.Text = "";
                roomsListBorder.Visibility = Visibility.Hidden;
                roomMemberBorder.Visibility = Visibility.Visible;
                updateParticipantsForMemberThread = new Thread(updateParticipantsForMember);
                updateParticipantsForMemberThread.Start();
            }
            else if (response.ContainsKey("message"))
            {
                roomsListErrorBox.Text = (string)response["message"];
            }
        }
        private void mainMenuShowScoresButtonClick(object sender, RoutedEventArgs e)
        {
           
        }
        private void mainMenuLogOutButtonClick(object sender, RoutedEventArgs e)
        {
           
        }
        private void mainMenuExitButtonClick(object sender, RoutedEventArgs e)
        {
           
        }
        private void creatingRoomCreateRoomButtonClick(object sender, RoutedEventArgs e)
        {
            createRoomErrorBox.Text = "";
            if (creatingRoomNameBox.Text.Length < 4)
            {
                createRoomErrorBox.Text = "Room name must be at least 4 characters.";
            }
            else if (Int16.Parse(creatingRoomPlayersAmountBox.Text) < 1)
            {
                createRoomErrorBox.Text = "Amount of players must be at least 1.";
            }
            else if (Int16.Parse(creatingRoomQuestionsAmountBox.Text) < 1)
            {
                createRoomErrorBox.Text = "Amount of questions must be at least 1.";
            }
            else if (Int16.Parse(creatingRoomQuestionTimeBox.Text) < 1)
            {
                createRoomErrorBox.Text = "Time for questions must be at least 1.";
            }
            else 
            {
                CreateRoomRequest createRoomRequest = new CreateRoomRequest
                {
                    roomName = creatingRoomNameBox.Text,
                    maxUsers = Int16.Parse(creatingRoomPlayersAmountBox.Text),
                    questionCount = Int16.Parse(creatingRoomQuestionsAmountBox.Text),
                    answerTimeout = Int16.Parse(creatingRoomQuestionTimeBox.Text)
                };

                string jsonDump = JsonConvert.SerializeObject(createRoomRequest, Formatting.Indented);

                serializeAndSendMessage(CREATE_ROOM_REQUEST, jsonDump);
                Dictionary<string, object> response = receiveAndDeserializeMessage();

                if (response.ContainsKey("status") && (string)response["status"] == "1")
                {
                    roomAdminPanelErrorBox.Text = "";
                    createRoomBorder.Visibility = Visibility.Hidden;
                    createRoomAdminBorder.Visibility = Visibility.Visible;
                    adminPanelRoomNameBox.Text = $"Room name: {creatingRoomNameBox.Text}";
                    adminPanelPlayersAmountBox.Text = $"Max amount of players: {creatingRoomPlayersAmountBox.Text}";
                    adminPanelQuestionsAmountBox.Text = $"Time per question: {creatingRoomQuestionTimeBox.Text}";

                    updateParticipantsForAdminThread = new Thread(updateParticipantsForAdmin);
                    updateParticipantsForAdminThread.Start();
                    
                }
                else if (response.ContainsKey("message"))
                {
                    createRoomErrorBox.Text = (string)response["message"];
                }
            }
        }
        public void updateParticipantsForAdmin()
        {
            while (true)
            { 

                serializeAndSendMessage(GET_ROOM_STATE_REQUEST, "");
                Dictionary<string, object> response = receiveAndDeserializeMessage();
                if (response.ContainsKey("status") && (string)response["status"] == "1")
                {
                    JArray Jplayers = (JArray)response["players"];
                    string[] players = Jplayers.Select(jv => (string)jv).ToArray();
                    this.Dispatcher.Invoke((Action)(() =>
                    {
                        roomParticipants.Items.Clear();
                    }));

                    for (int i = 0; i < players.Length; i++)
                    {
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            roomParticipants.Items.Add(new Label().Content = players[i]);
                        }));
                    }

                }
                else if (response.ContainsKey("message"))
                {
                    this.Dispatcher.Invoke((Action)(() =>
                    {
                        roomAdminPanelErrorBox.Text = (string)response["message"];
                    }));
                }

                Thread.Sleep(1000);
            }
        }public void updateParticipantsForMember()
        {
            while (true)
            { 

                serializeAndSendMessage(GET_ROOM_STATE_REQUEST, "");
                Dictionary<string, object> response = receiveAndDeserializeMessage();
                if (response.ContainsKey("status") && (string)response["status"] == "1")
                {
                    this.Dispatcher.Invoke((Action)(() =>
                    {
                        memberPanelQuestionsAmountBox.Text = $"Amount of questions: {(string)response["questionCount"]}";
                        memberPanelQuestionsTimeOutBox.Text = $"Questions timeout {(string)response["answerTimeout"]}";
                    }));

                    JArray Jplayers = (JArray)response["players"];
                    string[] players = Jplayers.Select(jv => (string)jv).ToArray();

                    this.Dispatcher.Invoke((Action)(() =>
                    {
                        roomMemberParticipants.Items.Clear();
                    }));

                    for (int i = 0; i < players.Length; i++)
                    {
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            roomMemberParticipants.Items.Add(new Label().Content = players[i]);
                        }));
                    }

                }
                else if (response.ContainsKey("message"))
                {
                    this.Dispatcher.Invoke((Action)(() =>
                    {
                        roomMemberPanelErrorBox.Text = (string)response["message"];
                    }));
                }

                Thread.Sleep(1000);
            }
        }

        private void createRoomBackButtonClick(object sender, RoutedEventArgs e)
        {
            createRoomBorder.Visibility = Visibility.Hidden;
            mainMenuBorder.Visibility = Visibility.Visible;
        }
        private void createRoomButtonClick(object sender, RoutedEventArgs e)
        {

        }
        private void memberPanelExitRoomButtonClick(object sender, RoutedEventArgs e)
        {
            serializeAndSendMessage(LEAVE_ROOM_REQUEST, "");
            Dictionary<string, object> response = receiveAndDeserializeMessage();

            if (response.ContainsKey("status") && (string)response["status"] == "1")
            {
                roomMemberBorder.Visibility = Visibility.Hidden;
                roomsListBorder.Visibility = Visibility.Visible;
            }
            else if (response.ContainsKey("message"))
            {
                roomMemberPanelErrorBox.Text = (string)response["message"];
            }
        }
        private void adminPanelCloseRoomButtonClick(object sender, RoutedEventArgs e)
        {
            serializeAndSendMessage(CLOSE_ROOM_REQUEST, "");
            Dictionary<string, object> response = receiveAndDeserializeMessage();

            if (response.ContainsKey("status") && (string)response["status"] == "1")
            {
                createRoomAdminBorder.Visibility = Visibility.Hidden;
                mainMenuBorder.Visibility = Visibility.Visible;
            }
            else if (response.ContainsKey("message"))
            {
                roomAdminPanelErrorBox.Text = (string)response["message"];
            }
        }
        
    }
}
