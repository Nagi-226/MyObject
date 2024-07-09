using System;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace StringGeneratorApp
{
    public partial class SenderWindow : Window
    {
        private CancellationTokenSource _cancellationTokenSource;

        public SenderWindow()
        {
            InitializeComponent();
        }

        private void GenerateButton_Click(object sender, RoutedEventArgs e)
        {
            if (ManualMode.IsChecked == true)
            {
                StringTextBox.Text = GenerateRandomString(10);
            }
        }

        private void StartAutoButton_Click(object sender, RoutedEventArgs e)
        {
            _cancellationTokenSource = new CancellationTokenSource();
            Task.Run(() => GenerateStringsAutomatically(_cancellationTokenSource.Token));
            StartAutoButton.IsEnabled = false;
            StopAutoButton.IsEnabled = true;
        }

        private void StopAutoButton_Click(object sender, RoutedEventArgs e)
        {
            _cancellationTokenSource.Cancel();
            StartAutoButton.IsEnabled = true;
            StopAutoButton.IsEnabled = false;
        }

        private void GenerateStringsAutomatically(CancellationToken token)
        {
            int count = 0;
            while (!token.IsCancellationRequested)
            {
                string generatedString = GenerateRandomString(10);
                Dispatcher.Invoke(() =>
                {
                    StringTextBox.Text = generatedString;
                    StatusTextBlock.Text = $"Generated {++count} strings.";
                });
                Thread.Sleep(1000); // 每秒生成一个字符串
            }
        }

        private string GenerateRandomString(int length)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
            StringBuilder stringBuilder = new StringBuilder();
            Random random = new Random();
            for (int i = 0; i < length; i++)
            {
                stringBuilder.Append(chars[random.Next(chars.Length)]);
            }
            return stringBuilder.ToString();
        }
    }
}