using System.Windows;

namespace StringGeneratorApp
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void OpenSender_Click(object sender, RoutedEventArgs e)
        {
            SenderWindow senderWindow = new SenderWindow();
            senderWindow.Show();
        }

        private void OpenReceiver_Click(object sender, RoutedEventArgs e)
        {
            ReceiverWindow receiverWindow = new ReceiverWindow();
            receiverWindow.Show();
        }
    }
}