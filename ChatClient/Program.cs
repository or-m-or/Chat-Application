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
using System.Threading;

namespace ChatClient
{
    class Program
    {
        static TcpClient client = null!;
        static NetworkStream stream = null!;
        static bool running = true;

        static void Main(string[] args)
        {
            try
            {
                client = new TcpClient("127.0.0.1", 27015);
                stream = client.GetStream();

                Console.WriteLine("서버에 연결되었습니다. 채팅을 시작하세요!");

                // 수신 스레드 시작
                Thread recvThread = new Thread(ReceiveLoop);
                recvThread.Start();

                // 송신 루프 (메인 스레드)
                while (running)
                {
                    string message = Console.ReadLine() ?? "";
                    if (message.ToLower() == "/exit")
                    {
                        running = false;
                        break;
                    }

                    byte[] data = Encoding.UTF8.GetBytes(message);
                    stream.Write(data, 0, data.Length);
                }

                stream.Close();
                client.Close();
                Console.WriteLine("채팅 종료");
            }
            catch (Exception e)
            {
                Console.WriteLine($"[예외] {e.Message}");
            }
        }

        static void ReceiveLoop()
        {
            byte[] buffer = new byte[1024];

            try
            {
                while (running)
                {
                    int bytes = stream.Read(buffer, 0, buffer.Length);
                    if (bytes == 0)
                    {
                        Console.WriteLine("서버와 연결이 종료되었습니다.");
                        break;
                    }

                    string msg = Encoding.UTF8.GetString(buffer, 0, bytes);
                    Console.WriteLine($"\n[서버] {msg}");
                    Console.Write("> "); // 입력 표시
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"[수신 오류] {e.Message}");
            }

            running = false;
        }
    }
}
