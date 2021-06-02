﻿using System;
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

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for MenuWindow.xaml
    /// </summary>
    public partial class MenuWindow : Window
    {

        public Communicator communicator { get; set; }

        public MenuWindow(Communicator communicator)
        {
            InitializeComponent();
            this.communicator = communicator;
        }

        private void SignOut_Click(object sender, RoutedEventArgs e)
        {
            new MainWindow(this.communicator).Show();
            this.Close();
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            new JoinRoomWindow(this.communicator).Show();
            this.Close();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            new CreateRoomWindow(this.communicator).Show();
            this.Close();
        }

        private void MyStatus_Click(object sender, RoutedEventArgs e)
        {
            new MyStatusWindow(this.communicator).Show();
            this.Close();
        }

        private void BestScores_Click(object sender, RoutedEventArgs e)
        {
            new BestScoresWindow(this.communicator).Show();
            this.Close();
        }

        private void Quit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
