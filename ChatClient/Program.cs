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
        /// �ش� ���ø����̼��� �� �������Դϴ�.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // ���� �ּҿ� ������ �޽���
            Connect("127.0.0.1", "Hello from WinForms project!");

            Console.WriteLine("�ƹ� Ű�� ������ ����˴ϴ�...");
            Console.ReadKey(); // �ܼ� ����
        }

        static void Connect(string server, string message)
        {
            try
            {
                int port = 27015; // ��Ʈ�� ������ �ݵ�� ��ġ�ؾ� ��

                using TcpClient client = new TcpClient(server, port);

                // �޽����� ASCII ����Ʈ �迭�� ��ȯ
                byte[] data = Encoding.ASCII.GetBytes(message);

                // NetworkStream���� �ۼ���
                NetworkStream stream = client.GetStream();

                // �޽��� ����
                stream.Write(data, 0, data.Length);
                Console.WriteLine("������ �޽����� ���½��ϴ�: {0}", message);

                // ���� ���� ����
                data = new byte[256];
                string responseData = string.Empty;

                int bytes = stream.Read(data, 0, data.Length);
                responseData = Encoding.ASCII.GetString(data, 0, bytes);
                Console.WriteLine("�����κ��� ������ �޾ҽ��ϴ�: {0}", responseData);
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("A[����] ���ڰ� �߸��Ǿ����ϴ�: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("[����] ���� ���� �߻�: {0}", e);
            }
        }
    }
}
