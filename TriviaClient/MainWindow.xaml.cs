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

    public partial class MainWindow : Window
    {
        public const int LOGIN_REQUEST = 201;
        public const int SIGNUP_REQUEST = 202;
        public const int CREATE_ROOM_REQUEST = 501;
        public const int GET_ROOMS_REQUEST = 502;
        public const int GET_PLAYERS_IN_ROOM_REQUEST = 503;
        public const int JOIN_ROOM_REQUEST = 504;
        public const int GET_STATISTICS_REQUEST = 505;
        public const int LOGOUT_REQUEST = 506;
        public const int CLOSE_ROOM_REQUEST = 507;
        public const int START_GAME_REQUEST = 508;
        public const int GET_ROOM_STATE_REQUEST = 509;
        public const int LEAVE_ROOM_REQUEST = 510;
        public const int SIGNUP_RESPONSE = 102;
        public const int ERROR_RESPONSE = 103;
        public const int BITS_IN_BYTE = 8;
        public const int SUCCSESS_RESPONSE_CODE = 1;
        public const int MINIMAL_BUFFER_SIZE = 40;
        public const int MESSAGE_CODE_LENGTH = 8; 
        public const int MESSAGE_LENGTH_LENGTH = 32; 

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
    }
}
