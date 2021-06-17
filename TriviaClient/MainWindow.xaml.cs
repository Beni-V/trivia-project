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
using System.ComponentModel;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    // will be used for sending signup request
    public class SignUpRequest
    {
        public string username { get; set; }
        public string password { get; set; }
        public string email { get; set; }
    }

    // will be used for sending signin request
    public class SignInRequest
    {
        public string username { get; set; }
        public string password { get; set; }
    }

    // will be used for creating room
    public class CreateRoomRequest
    {
        public string roomName { get; set; }
        public int maxUsers { get; set; }
        public int questionCount { get; set; }
        public int answerTimeout { get; set; }
    }

    // will be used for getting the players in room by id
    public class GetPlayersInRoomRequest
    {
        public int roomId { get; set; }
    }

    // will be used to join room
    public class JoinRoomRequest
    {
        public int roomId { get; set; }
    }
    public class SubmitAnswerRequest
    {
        public int answerId { get; set; }
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
        public const int GET_HIGH_SCORES_REQUEST = 101;
        public const int SIGNUP_RESPONSE = 102;
        public const int ERROR_RESPONSE = 103;
        public const int GET_GAME_RESULT_REQUEST = 110;
        public const int SUBMIT_ANSWER_REQUEST = 111;
        public const int GET_QUESTION_REQUEST = 112;
        public const int LEAVE_GAME_REQUEST = 113;
        public const int BITS_IN_BYTE = 8;
        public const int SUCCSESS_RESPONSE_CODE = 1;
        public const int MINIMAL_BUFFER_SIZE = 40; // minimal size is the message code + message length size in bits
        public const int MESSAGE_CODE_LENGTH = 8; 
        public const int MESSAGE_LENGTH_LENGTH = 32;

        // threads for autorefreshing info like rooms, or room participants
        public Thread updateParticipantsForAdminThread;
        public Thread updateParticipantsForMemberThread;
        public Thread refreshRoomsListThread;
        public Thread updateGameScreenThread;

        // bool vars for threads, threads will run only if his flag is true
        public bool runUpdateParticipantsForAdminThread = false;
        public bool runUpdateParticipantsForMemberThread = false;
        public bool runRefreshRoomsListThread = false;
        public bool runUpdateGameScreen = false;

        public int currentQuestionTimeOut;
        public int currentGameQuestionsTimeOut;
        public int currentQuestionsAmount;

        public static NetworkStream clientStream; // all the transfering will be completed through this variable

        public MainWindow()
        {
            Application.Current.MainWindow.Closing += new CancelEventHandler(MainWindow_Closing); // needed for handling close window

            // create and run the threads for updating info (all flags is false by default, will be switched to true after)
            updateParticipantsForAdminThread = new Thread(updateParticipantsForAdmin);
            updateParticipantsForAdminThread.Start();
            updateParticipantsForMemberThread = new Thread(updateParticipantsForMember);
            updateParticipantsForMemberThread.Start();
            refreshRoomsListThread = new Thread(refreshRoomList);
            refreshRoomsListThread.Start();
            updateGameScreenThread = new Thread(updateGameScreen);
            updateGameScreenThread.Start();

            StartClient(); // create connection with the server

            InitializeComponent(); // run the wpf
        }

        // will handle close window
        void MainWindow_Closing(object sender, CancelEventArgs e)
        {
            // needed to avoid exception when the app will be closed with 'X'
            System.Windows.Application.Current.Shutdown();
            System.Environment.Exit(0);
        }

        // will setup the connection with the server
        public static void StartClient()
        {
            TcpClient client = new TcpClient(); // create tcp client
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 5150); // will store server info
            try
            {
                client.Connect(serverEndPoint); // connect to the server
            }
            catch
            {
                // in case something went wrong, this will be executed
                MessageBox.Show("Error. You should run the server first.");
                System.Windows.Application.Current.Shutdown();
                System.Environment.Exit(0);
            }
            clientStream = client.GetStream(); // get the stream, and put into global var client stream
        }

        // this function will take all parts of a serialized message (messageCode, messageLength, message) and connect them
        static string ConcatenateSerializedMessageParts(string messageCode, string messageLength, string message)
        {
            // if the header have less bits then it should be by protocol, add zeroes in the beginning
            while(messageCode.Length < MESSAGE_CODE_LENGTH)
            { 
                messageCode = messageCode.Insert(0, "0");
            }

            // same with length header
            while (messageLength.Length < MESSAGE_LENGTH_LENGTH)
            {
                
                messageLength = messageLength.Insert(0, "0");
            }

            return messageCode + messageLength + message; // return concatenated parts
        }

        // will convert string to sequence of bits
        static string ToBinaryString(Encoding encoding, string text)
        {
            return string.Join("", encoding.GetBytes(text).Select(n => Convert.ToString(n, 2).PadLeft(8, '0')));
        }

        // will get message code and json dump, then will serialize the message and send it to server
        static void serializeAndSendMessage(int messageCode, string jsonDump)
        {
            string serializedJsonMessage = ToBinaryString(Encoding.UTF8, jsonDump); // will store jsonDump in bits
            string serializedMessageCode = Convert.ToString(messageCode, 2); // will store message code in bits
            string serializedMessageLength = Convert.ToString(serializedJsonMessage.Length, 2); // will store message length in bits
            string message = ConcatenateSerializedMessageParts(serializedMessageCode, serializedMessageLength, serializedJsonMessage); // conccatenate the parts
            byte[] buffer = new ASCIIEncoding().GetBytes(message); // put the whole bessage into buffer

            clientStream.Write(buffer, 0, buffer.Length); // send the buffer to server
            clientStream.Flush(); // clear the client stream buffer (not the variable buffer)
        }

        // will receive response message from server, deserialize it, and return as a map.
        static Dictionary<string, object> receiveAndDeserializeMessage()
        {
            byte[] headers = new byte[MINIMAL_BUFFER_SIZE]; // create byte array for headers (messageCode + messageLength)

            clientStream.Read(headers, 0, headers.Length); // read the headers

            int messageLength = Convert.ToInt32(System.Text.Encoding.Default.GetString(headers.Skip(8).ToArray()), 2) * BITS_IN_BYTE; // put here the message length

            // read the rest of the message with gotten message length into serializedJsonResponse
            byte[] serializedJsonResponse = new byte[messageLength];
            clientStream.Read(serializedJsonResponse, 0, serializedJsonResponse.Length);

            string jsonDump = BinaryToASCII(System.Text.Encoding.Default.GetString(serializedJsonResponse)); // convert gotten binary json dump into assci

            Dictionary<string, object> result = JsonConvert.DeserializeObject<Dictionary<string, object>>(jsonDump); // deserialize json dump into dictionary. (the value is object because the type is bassicly unknown on this stage)

            return result; // return the dictionary
        }

        // will binary to decimal
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

        // will convert binary sequene into assci string
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
            signUpErrorBox.Foreground = Brushes.Red; // change the color of error box to red in case that was green after succsess registration

            // check if the inputs are valid
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
            else // will be executed if inputs are valid
            {
                signUpErrorBox.Text = ""; // clean error box

                // create the struct of sign up request
                SignUpRequest signUpRequest = new SignUpRequest
                {
                    username = signUpUserNameBox.Text, password = signUpPasswordBox.Password, email = signUpEmailBox.Text
                };

                string jsonDump = JsonConvert.SerializeObject(signUpRequest, Formatting.Indented); // serialize struct into jsonDump

                serializeAndSendMessage(SIGNUP_REQUEST, jsonDump); // send message to server
                Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

                if (response.ContainsKey("status") && (string)response["status"] == "1") // if response has success key
                {
                    signUpErrorBox.Foreground = Brushes.Green; // change errorBox color to green
                    signUpErrorBox.Text = "Account successfully registered"; // tell user that he has succsessfully registered
                }
                else if (response.ContainsKey("message")) // if response has any error
                {
                    signUpErrorBox.Text = (string)response["message"]; // display the error in error box
                }
            }
        }
        private void signInButtonClick(object sender, RoutedEventArgs e)
        {
            signInErrorBox.Text = ""; // clean error box from possible previous error

            SignInRequest signInRequest = new SignInRequest // create the request struct
            {
                username = signInUserNameBox.Text, password = signInPasswordBox.Password
            };

            string jsonDump = JsonConvert.SerializeObject(signInRequest, Formatting.Indented); // get the jsonDump from struct

            serializeAndSendMessage(LOGIN_REQUEST, jsonDump); // send the json dump to server
            Dictionary<string, object> response = receiveAndDeserializeMessage(); // get response

            if (response.ContainsKey("status") && (string)response["status"] == "1") // if response has succsess code
            {
                // switch the borders and show the main menu
                signInBorder.Visibility = Visibility.Hidden;
                mainMenuBorder.Visibility = Visibility.Visible;
            }
            else if (response.ContainsKey("message")) // if no success code, display the error message
            {
                signInErrorBox.Text = (string)response["message"];
            }
        }

        private void mainMenuCreateRoomButtonClick(object sender, RoutedEventArgs e)
        {
            mainMenuBorder.Visibility = Visibility.Hidden;
            createRoomBorder.Visibility = Visibility.Visible;
        }

        // button for joining the romms list
        private void mainMenuJoinRoomButtonClick(object sender, RoutedEventArgs e)
        {
            roomsListErrorBox.Text = ""; // clean error box from possible previous error

            // seitch borders and display the rooms list
            mainMenuBorder.Visibility = Visibility.Hidden;
            roomsListBorder.Visibility = Visibility.Visible;

            runRefreshRoomsListThread = true; // set the thread flag for updating rooms list to true

        }

        // this function will be executed as thread for refreshing the rooms list. rooms list will be refreshed every 3 seconds
        public void refreshRoomList()
        {
            while (true) // general infinity loop
            {
                while (runRefreshRoomsListThread) // this will be executed only if the flag is true
                {
                    serializeAndSendMessage(GET_ROOMS_REQUEST, ""); // send get rooms request to server
                    Dictionary<string, object> response = receiveAndDeserializeMessage(); // get response

                    if (response.ContainsKey("status") && (string)response["status"] == "1") // if response has success status
                    {
                        // clear the rooms list from previous request
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            roomsList.Items.Clear();
                        }));

                        JArray Jrooms = (JArray)response["rooms"]; // put rooms dictionary into JArray

                        // fill the rooms list in the gui in format {id}: {name}
                        foreach (JArray room in Jrooms)
                        {
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomsList.Items.Add(new Label().Content = $"{room[0]}: {room[1]}");
                            }));
                        }
                    }
                    else if (response.ContainsKey("message")) // if response has error message
                    {
                        // display the error message in error box
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            roomsListErrorBox.Text = (string)response["message"];
                        }));
                    }
                    Thread.Sleep(3000); // sleep the thread for 3 seconds
                }
            }
        }

        // button to exit the rooms list back to main menu
        private void roomListBackButtonClick(object sender, RoutedEventArgs e)
        {
            runRefreshRoomsListThread = false; // set flag for updating rooms list to false

            // switch the borders
            roomsListBorder.Visibility = Visibility.Hidden;
            mainMenuBorder.Visibility = Visibility.Visible;
        }

        // button in main menu that will show user statistics
        private void mainMenuShowStatusButtonClick(object sender, RoutedEventArgs e)
        {
            // switch borders
            mainMenuBorder.Visibility = Visibility.Hidden;
            statusBorder.Visibility = Visibility.Visible;

            serializeAndSendMessage(GET_STATISTICS_REQUEST, ""); // send statistics request to server
            Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

            if (response.ContainsKey("status") && (string)response["status"] == "1") // if reponse have success status
            {

                // display the statistics
                JArray Jstatistics = (JArray)response["statistics"];
                statusGamesAmountBox.Text = $"Number of played games: {Jstatistics[3]}";
                statusRightAnswersAmountBox.Text = $"Amount of right answers: {Jstatistics[1]}";
                statusWrongAnswersAmountBox.Text = $"Amount of total answers: {Jstatistics[2]}";
                statusAverageAnswerTimeBox.Text = $"Average time for answer: {Jstatistics[0]}";
            }
            else if (response.ContainsKey("message")) // display the error message if there is an error message
            {
                statusErrorBox.Text = (string)response["message"];
            }
        }

        // join room button from romms list
        private void roomListJoinRoomButtonClick(object sender, RoutedEventArgs e)
        {
            runRefreshRoomsListThread = false; // set refresh room list thread flag to false

            JoinRoomRequest joinRoomRequest = new JoinRoomRequest // build struct with join room request
            {
                roomId = Convert.ToInt32(roomsList.SelectedItem.ToString()[0].ToString())
            };

            string jsonDump = JsonConvert.SerializeObject(joinRoomRequest, Formatting.Indented); // get the jsonump from request struct

            serializeAndSendMessage(JOIN_ROOM_REQUEST, jsonDump); // send the request
            Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

            if (response.ContainsKey("status") && (string)response["status"] == "1") // if response have success status
            {
                // switch status
                roomsListBorder.Visibility = Visibility.Hidden;
                roomMemberBorder.Visibility = Visibility.Visible;

                runUpdateParticipantsForMemberThread = true; // flag for refreshing participants in room for member set to true
            }
            else if (response.ContainsKey("message")) // if there is an error message in the response
            {
                roomsListErrorBox.Text = (string)response["message"]; // display the error

                if ((string)response["message"] == "This room is full") // if the error is "This room is full"
                {
                    runRefreshRoomsListThread = true; // set refresh rooms flag back to true
                }

            }
        }

        // button from main menu to show best 3 players
        private void mainMenuShowScoresButtonClick(object sender, RoutedEventArgs e)
        {
            bestPlayersList.Items.Clear(); // clear best players from previous best players


            // switch borders
            mainMenuBorder.Visibility = Visibility.Hidden;
            scoresBorder.Visibility = Visibility.Visible;

            serializeAndSendMessage(GET_HIGH_SCORES_REQUEST, ""); // send request to server
            Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

            if (response.ContainsKey("status") && (string)response["status"] == "1") // if success status in the response
            {
                // display the best players
                JArray bestPlayers = (JArray)response["statistics"];
                foreach (string player in bestPlayers)
                {
                    bestPlayersList.Items.Add(new Label().Content = player);
                }
            }
            else if (response.ContainsKey("message")) // if there is an error message in the response
            {
                scoresErrorBox.Text = (string)response["message"]; // display the error
            }

        }

        // logout button from main menu
        private void mainMenuLogOutButtonClick(object sender, RoutedEventArgs e)
        {
            serializeAndSendMessage(LOGOUT_REQUEST, "");
            Dictionary<string, object> response = receiveAndDeserializeMessage(); 

            if (response.ContainsKey("status") && (string)response["status"] == "1")
            {
                mainMenuBorder.Visibility = Visibility.Hidden;
                wellcomeBorder.Visibility = Visibility.Visible;
            }
        }
        private void mainMenuExitButtonClick(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }
        private void statusExitButtonClick(object sender, RoutedEventArgs e)
        {
            statusBorder.Visibility = Visibility.Hidden;
            mainMenuBorder.Visibility = Visibility.Visible;
        }

        // ceate room button from creating room menu
        private void creatingRoomCreateRoomButtonClick(object sender, RoutedEventArgs e)
        {
            createRoomErrorBox.Text = ""; // clean error box from possible previous error

            // check if the inputs are valid
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
            else // if all inputs are valid
            {
                CreateRoomRequest createRoomRequest = new CreateRoomRequest // build the struct with create room request
                {
                    roomName = creatingRoomNameBox.Text,
                    maxUsers = Int16.Parse(creatingRoomPlayersAmountBox.Text),
                    questionCount = Int16.Parse(creatingRoomQuestionsAmountBox.Text),
                    answerTimeout = Int16.Parse(creatingRoomQuestionTimeBox.Text)
                };

                string jsonDump = JsonConvert.SerializeObject(createRoomRequest, Formatting.Indented); // get the json dump

                serializeAndSendMessage(CREATE_ROOM_REQUEST, jsonDump); // send request
                Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

                if (response.ContainsKey("status") && (string)response["status"] == "1") // if response have success status
                {
                    // switch borders
                    createRoomBorder.Visibility = Visibility.Hidden;
                    createRoomAdminBorder.Visibility = Visibility.Visible;

                    // display room properties
                    adminPanelRoomNameBox.Text = $"Room name: {creatingRoomNameBox.Text}";
                    adminPanelPlayersAmountBox.Text = $"Max amount of players: {creatingRoomPlayersAmountBox.Text}";
                    adminPanelQuestionsAmountBox.Text = $"Time per question: {creatingRoomQuestionTimeBox.Text}";

                    // setup true flag for refresh participants of room for admin
                    runUpdateParticipantsForAdminThread = true;
                    
                }
                else if (response.ContainsKey("message")) // display the error if there is an error in the response
                {
                    createRoomErrorBox.Text = (string)response["message"];
                }
            }
        }

        // will be executed as a thread for updating participants in room for admin (refresh every 3 seconds)
        public void updateParticipantsForAdmin()
        {
            while (true) // main infinity loop
            {
                while (runUpdateParticipantsForAdminThread) // check if flag is true
                {
                    serializeAndSendMessage(GET_ROOM_STATE_REQUEST, ""); // send request for getting room state
                    Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

                    if (response.ContainsKey("status") && (string)response["status"] == "1") // if there is  a success status
                    {
                        JArray Jplayers = (JArray)response["players"]; // get players in Jarray format
                        string[] players = Jplayers.Select(jv => (string)jv).ToArray(); // convert Jarray to regular string array

                        // clean the players list from previous run
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            roomParticipants.Items.Clear();
                        }));

                        // display the players
                        for (int i = 0; i < players.Length; i++)
                        {
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomParticipants.Items.Add(new Label().Content = Jplayers[i]);
                            }));
                        }

                    }
                    else if (response.ContainsKey("message")) // display error message if there is an error message in the response
                    {
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            roomAdminPanelErrorBox.Text = (string)response["message"];
                        }));
                    }
                    Thread.Sleep(3000); // sleep the thread for 3 seconds
                }
            }
        }

        // will be executed as a thread for updating participants in room for member (refresh every 3 seconds)
        public void updateParticipantsForMember()
        {
            while (true) // main infinity loop
            {
                while (runUpdateParticipantsForMemberThread) // check if flag is true
                {
                    serializeAndSendMessage(GET_ROOM_STATE_REQUEST, ""); // send request for getting room state
                    Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

                    if (response.ContainsKey("status") && (string)response["status"] == "1") // if there is  a success status
                    {
                        // dispay room properties
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            memberPanelQuestionsAmountBox.Text = $"Amount of questions: {(string)response["questionCount"]}";
                            memberPanelQuestionsTimeOutBox.Text = $"Questions timeout {(string)response["answerTimeout"]}";
                        }));

                        currentGameQuestionsTimeOut = Int16.Parse((string)response["answerTimeout"]);
                        currentQuestionTimeOut = currentGameQuestionsTimeOut;

                        JArray Jplayers = (JArray)response["players"]; // get the players in Jarray format
                        string[] players = Jplayers.Select(jv => (string)jv).ToArray(); // convert it to regular string array

                        // clear players list from previous run
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            roomMemberParticipants.Items.Clear();
                        }));

                        // display the players
                        for (int i = 0; i < players.Length; i++)
                        {
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomMemberParticipants.Items.Add(new Label().Content = players[i]);
                            }));
                        }

                        if ((bool)response["hasGameBegun"])
                        {
                            runUpdateParticipantsForMemberThread = false;
                            runUpdateGameScreen = true;
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomMemberBorder.Visibility = Visibility.Hidden;
                                gameBorder.Visibility = Visibility.Visible;
                            }));
                        }
                    }
                    else if (response.ContainsKey("message")) // if there is an error message in the response
                    {
                        if ((string)response["message"] == "Room Closed") // if room were closed
                        {
                            // clean error box in rooms list
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomsListErrorBox.Text = "";
                            }));


                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomClosedDialog.IsOpen = true;
                            }));

                            // switch borders to rooms list
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomMemberBorder.Visibility = Visibility.Hidden;
                                roomsListBorder.Visibility = Visibility.Visible;
                            }));

                            // update the flags
                            runUpdateParticipantsForMemberThread = false;
                            runRefreshRoomsListThread = true;
                        }
                        else // if its any other error message, display it
                        {
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                roomMemberPanelErrorBox.Text = (string)response["message"];
                            }));
                        }
                    }
                    Thread.Sleep(3000); // sleep thread for 3 seconds
                }
            }
        }

        public void updateGameScreen()
        {
            while (true) // main infinity loop
            {
                while (runUpdateGameScreen) // check if flag is true
                {
                    if (currentQuestionTimeOut <= 0)
                    {
                        currentQuestionTimeOut = currentGameQuestionsTimeOut;
                        SubmitAnswerRequest submitAnswerRequest = new SubmitAnswerRequest // create the request struct
                        {
                            answerId = 1                            
                        };

                        string jsonDump = JsonConvert.SerializeObject(submitAnswerRequest, Formatting.Indented); // serialize struct into jsonDump
                        serializeAndSendMessage(SUBMIT_ANSWER_REQUEST, jsonDump); // send message to server

                        Dictionary<string, object> submitAnswerResponse = receiveAndDeserializeMessage(); // receive response

                        if (submitAnswerResponse.ContainsKey("status") && (string)submitAnswerResponse["status"] == "1") // if there is  a success status
                        {
                        }
                        else // if its any other error message, display it
                        {
                            this.Dispatcher.Invoke((Action)(() =>
                            {
                                gameErrorBox.Text = (string)submitAnswerResponse["message"];
                            }));
                        }
                    }

                    serializeAndSendMessage(GET_QUESTION_REQUEST, ""); 
                    Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

                    if (response.ContainsKey("status") && (string)response["status"] == "1") // if there is  a success status
                    {
                        JArray Janswers = (JArray)response["answers"];

                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            gameCurrentQuestion.Text = (string)response["question"];
                            firstAnswer.Content = Janswers[0][1];
                            secondAnswer.Content = Janswers[1][1];
                            thirdAnswer.Content = Janswers[2][1];
                            fourthAnswer.Content = Janswers[3][1];
                            gameQuestionTimer.Text = $"Time left: {currentQuestionTimeOut.ToString()}";
                        }));
                    }
                    else // if its any other error message, display it
                    {
                        this.Dispatcher.Invoke((Action)(() =>
                        {
                            gameErrorBox.Text = (string)response["message"];
                        }));
                    }
                    Thread.Sleep(1000); // sleep thread for 1 second
                    currentQuestionTimeOut--;
                }
            }
        }

        private void createRoomBackButtonClick(object sender, RoutedEventArgs e)
        {
            createRoomBorder.Visibility = Visibility.Hidden;
            mainMenuBorder.Visibility = Visibility.Visible;
        }
        private void leaveGameButtonClick(object sender, RoutedEventArgs e)
        {

        }
        private void createRoomButtonClick(object sender, RoutedEventArgs e)
        {
            serializeAndSendMessage(START_GAME_REQUEST, ""); // send request to start game
            Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

            if (response.ContainsKey("status") && (string)response["status"] == "1") // if response have success status
            {
                roomStartedDialog.IsOpen = true;
            }
            else if (response.ContainsKey("message")) // display the error message if there is an error message in the response
            {
                roomAdminPanelErrorBox.Text = (string)response["message"];
            }
        }
        private void scoresExitButtonClick(object sender, RoutedEventArgs e)
        {
            scoresBorder.Visibility = Visibility.Hidden;
            mainMenuBorder.Visibility = Visibility.Visible;
        }

        // exit room button from member panel
        private void memberPanelExitRoomButtonClick(object sender, RoutedEventArgs e)
        {
            runUpdateParticipantsForMemberThread = false; // update praticipants in member panel flag set to false

            roomsListErrorBox.Text = ""; // clean error box from previous possible error

            serializeAndSendMessage(LEAVE_ROOM_REQUEST, ""); // send request to leave room
            Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

            if (response.ContainsKey("status") && (string)response["status"] == "1") // if response have success status
            {
                runRefreshRoomsListThread = true; // flag for refreshing rooms list set to true

                // switch borders
                roomMemberBorder.Visibility = Visibility.Hidden;
                roomsListBorder.Visibility = Visibility.Visible;
            }
            else if (response.ContainsKey("message")) // display the error message if there is an error message in the response
            {
                roomMemberPanelErrorBox.Text = (string)response["message"];
            }
        }

        // button for closing the room from admin panel
        private void adminPanelCloseRoomButtonClick(object sender, RoutedEventArgs e)
        {
            runUpdateParticipantsForAdminThread = false; // stop refereshing participants for admin

            serializeAndSendMessage(CLOSE_ROOM_REQUEST, ""); // send request to close the room
            Dictionary<string, object> response = receiveAndDeserializeMessage(); // receive response

            if (response.ContainsKey("status") && (string)response["status"] == "1") // switch the border if response have success status
            {
                createRoomAdminBorder.Visibility = Visibility.Hidden;
                mainMenuBorder.Visibility = Visibility.Visible;
            }
            else if (response.ContainsKey("message")) // dispay error message if there is an error message in the response
            {
                roomAdminPanelErrorBox.Text = (string)response["message"];
            }
        }
    }
}
