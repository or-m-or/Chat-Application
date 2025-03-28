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

                Console.WriteLine("������ ����Ǿ����ϴ�. ä���� �����ϼ���!");

                // ���� ������ ����
                Thread recvThread = new Thread(ReceiveLoop);
                recvThread.Start();

                // �۽� ���� (���� ������)
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
                Console.WriteLine("ä�� ����");
            }
            catch (Exception e)
            {
                Console.WriteLine($"[����] {e.Message}");
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
                        Console.WriteLine("������ ������ ����Ǿ����ϴ�.");
                        break;
                    }

                    string msg = Encoding.UTF8.GetString(buffer, 0, bytes);
                    Console.WriteLine($"\n[����] {msg}");
                    Console.Write("> "); // �Է� ǥ��
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"[���� ����] {e.Message}");
            }

            running = false;
        }
    }
}
