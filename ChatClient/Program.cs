//namespace ChatClient
//{
//    internal static class Program
//    {
//        /// <summary>
//        ///  The main entry point for the application.
//        /// </summary>
//        [STAThread]
//        static void Main()
//        {
//            // To customize application configuration such as set high DPI settings or default font,
//            // see https://aka.ms/applicationconfiguration.
//            ApplicationConfiguration.Initialize();
//            Application.Run(new Form1());
//        }
//    }
//}

using System;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;

namespace ChatClient
{
    internal static class Program
    {
        /// <summary>
        /// 해당 애플리케이션의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // 서버 주소와 전송할 메시지
            Connect("127.0.0.1", "Hello from WinForms project!");

            Console.WriteLine("아무 키나 누르면 종료됩니다...");
            Console.ReadKey(); // 콘솔 유지
        }

        static void Connect(string server, string message)
        {
            try
            {
                int port = 27015; // 포트는 서버와 반드시 일치해야 함

                using TcpClient client = new TcpClient(server, port);

                // 메시지를 ASCII 바이트 배열로 변환
                byte[] data = Encoding.ASCII.GetBytes(message);

                // NetworkStream으로 송수신
                NetworkStream stream = client.GetStream();

                // 메시지 전송
                stream.Write(data, 0, data.Length);
                Console.WriteLine("서버에 메시지를 보냈습니다: {0}", message);

                // 서버 응답 수신
                data = new byte[256];
                string responseData = string.Empty;

                int bytes = stream.Read(data, 0, data.Length);
                responseData = Encoding.ASCII.GetString(data, 0, bytes);
                Console.WriteLine("서버로부터 응답을 받았습니다: {0}", responseData);
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("A[예외] 인자가 잘못되었습니다: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("[예외] 소켓 오류 발생: {0}", e);
            }
        }
    }
}
