using System.Net.Sockets;
using System.Text;

namespace ChatClient
{
    public partial class Form1 : Form
    {
        private TcpClient? client;
        private NetworkStream? stream;
        private Thread? recvThread;
        private bool running = false;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            client = new TcpClient("127.0.0.1", 27015);
            stream = client.GetStream();

            running = true;
            recvThread = new Thread(ReceiveLoop);
            recvThread.Start();

            AppendChat("서버에 연결되었습니다.");
            button1.Enabled = false;
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Enter)
            {
                string message = textBox1.Text.Trim();
                if (!string.IsNullOrEmpty(message))
                {
                    byte[] data = Encoding.UTF8.GetBytes(message);
                    if (stream != null)
                    {
                        stream.Write(data, 0, data.Length);
                    }
                    textBox1.Clear();
                }
                e.Handled = true;
            }
        }

        private void ReceiveLoop()
        {
            byte[] buffer = new byte[1024];
            try
            {
                while (running)
                {

                    int bytes = 0;
                    if (stream != null)
                    {
                        bytes = stream.Read(buffer, 0, buffer.Length);
                    }
                    if (bytes == 0) break;

                    string msg = Encoding.UTF8.GetString(buffer, 0, bytes);
                    AppendChat($"서버: {msg}");
                }
            }
            catch (Exception e)
            {
                AppendChat($"[수신 오류] {e.Message}");
            }

            running = false;
        }

        // UI 스레드에서 안전하게 텍스트 추가
        private void AppendChat(string text)
        {
            if (textBox2.InvokeRequired)
                Invoke(() => textBox2.AppendText(text + "\r\n"));
            else
                textBox2.AppendText(text + "\r\n");
        }
    }
}
