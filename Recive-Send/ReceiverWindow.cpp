using System.Windows;

namespace StringGeneratorApp
{
    public partial class ReceiverWindow : Window
    {
        public ReceiverWindow()
        {
            InitializeComponent();
        }

        private void ReceiveButton_Click(object sender, RoutedEventArgs e)
        {
            // 这里接收字符串的逻辑可以通过某种方式与SenderWindow通信
            // 例如通过事件、消息传递或共享数据结构
            ReceivedStringTextBox.Text = "Sample received string"; // 示例代码
        }
    }
}