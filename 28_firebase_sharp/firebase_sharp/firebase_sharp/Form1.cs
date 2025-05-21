using FireSharp;
using FireSharp.Config;
using FireSharp.Interfaces;
using FireSharp.Response;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace firebase_sharp
{
    public partial class Form1 : Form
    {
        IFirebaseConfig config = new FirebaseConfig
        {
            AuthSecret = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
            BasePath = "https://xxxxxxxxxxxxxxxxxxxx.firebaseio.com/"
        };

        IFirebaseClient istemci_firebase;

        public Form1()
        {
            InitializeComponent();
        }

        // 1sn de bir timer tick olayı gerceklesiyor
        private async void timer1_Tick(object sender, EventArgs e)
        {
            FirebaseResponse response = await istemci_firebase.GetAsync("sicaklik");        // sicakligi oku
            label2.Text = response.ResultAs<String>();

            response = await istemci_firebase.GetAsync("durum");            // kombi durumunu oku

            label4.Text = response.ResultAs<String>();
            if(label4.Text=="KOMBI ON")
                label4.ForeColor = System.Drawing.Color.Green;
            else
                label4.ForeColor = System.Drawing.Color.Red;
            label4.Text = "Kombi durumu: " + label4.Text;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "")
                textBox1.Text = "24";

            istemci_firebase.Set("ayarlanan", Int32.Parse(textBox1.Text));      // ayarlanan kombi açma sıcaklıgını yaz 
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            istemci_firebase = new FirebaseClient(config);
        }
    }
}
