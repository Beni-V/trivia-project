using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using Newtonsoft.Json;

namespace ClientGUI
{
    public class Communicator
    {
        public NetworkStream clientStream { get; set; }

        public Communicator()
        {
            TcpClient client = new TcpClient();
            client = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 5150);
            client.Connect(serverEndPoint);
            this.clientStream = client.GetStream();
        }

        public void SerializeAndSendMsg(string msg, int codeMsg)
        {
            string binMsg = StringToBinary(msg);
            string binCode = Convert.ToString(codeMsg, 2).PadLeft(8, '0');
            string binLength = Convert.ToString(binMsg.Length, 2).PadLeft(32, '0');

            byte[] buffer = new ASCIIEncoding().GetBytes(binCode + binLength + binMsg);
            this.clientStream.Write(buffer, 0, buffer.Length);
            this.clientStream.Flush();
        }

        public Dictionary<string, string> RecvAndDeserializeMsg()
        {
            byte[] buffer = new byte[8];
            int bytesRead = clientStream.Read(buffer, 0, buffer.Length);
            string msg = Encoding.Default.GetString(buffer);
            int codeMsg = Convert.ToInt32(msg, 2);

            buffer = new byte[32];
            bytesRead = clientStream.Read(buffer, 0, buffer.Length);
            msg = Encoding.Default.GetString(buffer);
            int lengthMsg = Convert.ToInt32(msg, 2);

            buffer = new byte[lengthMsg * 8];
            bytesRead = clientStream.Read(buffer, 0, buffer.Length);
            msg = Encoding.Default.GetString(buffer);

            return JsonConvert.DeserializeObject<Dictionary<string, string>>(BinaryToString(msg));
        }

        public static string StringToBinary(string data)
        {
            StringBuilder sb = new StringBuilder();

            foreach (char c in data.ToCharArray())
            {
                sb.Append(Convert.ToString(c, 2).PadLeft(8, '0'));
            }
            return sb.ToString();
        }

        public static string BinaryToString(string data)
        {
            List<Byte> byteList = new List<Byte>();

            for (int i = 0; i < data.Length; i += 8)
            {
                byteList.Add(Convert.ToByte(data.Substring(i, 8), 2));
            }
            return Encoding.ASCII.GetString(byteList.ToArray());
        }
    }
}
