using System;
using System.Net;
using System.IO;
using System.Text;
using System.Threading;
using System.Web;
using System.ComponentModel;

namespace HRServer
{
    public class HeartRateServer : INotifyPropertyChanged
    {

        

        private HttpListener _server;
        private int _port = 6547;

        private String _webRoot = Directory.GetCurrentDirectory() + @"/www/";

        private String _lastUpdate = "hh:mm:ss";
        private bool _isServerStarted = false;

        private String _bpm = "000";

        public event PropertyChangedEventHandler PropertyChanged;


        public bool IsServerStarted
        {
            get { return _isServerStarted; }
            set { _isServerStarted = value; }
        }

        public String LastUpdate
        {
            get { return _lastUpdate; }
            set 
            {
                _lastUpdate = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("LastUpdate"));
            }
        }

        public String BPM
        {
            get { return _bpm; }
            set 
            { 
                _bpm = value;
                LastUpdate = DateTime.Now.ToString("HH:mm:ss");
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("BPM"));
            }
        }

        public HttpListener Server
        {
            get { return _server; }
            private set { _server = value; }
        }

        public String WebRoot
        {
            get { return _webRoot; }
        }

        public int Port
        {
            get { return _port; }
            set { 
                if (value > 65535 || value <= 0) {
                    _port = 6547;
                } else{
                    _port = value;
                }
            }
        }

        public HeartRateServer(int port)
        {
            Port = port;
            Server = new HttpListener();  // this is the http server
            string hostName = Dns.GetHostName();

            Server.Prefixes.Add("http://127.0.0.1:" + Port + "/");  //we set a listening address here (localhost)
            Server.Prefixes.Add("http://localhost:" + Port + "/");  //we set a listening address here (localhost)

            foreach (IPAddress ip in Dns.GetHostEntry(hostName).AddressList)
            {
                if (ip.AddressFamily.ToString() == "InterNetwork")
                {
                    Server.Prefixes.Add("http://" + ip + ":"+ Port +"/");  //we set a listening address here (localhost)
                }
            }


        }

        public void StartServer()
        {
            this.StartListener();
            this.WaitNextRequest();
            IsServerStarted = true;

        }

        private void WaitNextRequest()
        {
            
               IAsyncResult context = Server.BeginGetContext(new AsyncCallback(ListenerCallBack), Server);
            
            
        }

        public void StopServer()
        {
            this.StopListener();
            Server.Stop();
            IsServerStarted = false;


            //

            Server = new HttpListener();  // this is the http server
            string hostName = Dns.GetHostName();

            Server.Prefixes.Add("http://127.0.0.1:" + Port + "/");  //we set a listening address here (localhost)
            Server.Prefixes.Add("http://localhost:" + Port + "/");  //we set a listening address here (localhost)

            foreach (IPAddress ip in Dns.GetHostEntry(hostName).AddressList)
            {
                if (ip.AddressFamily.ToString() == "InterNetwork")
                {
                    Server.Prefixes.Add("http://" + ip + ":" + Port + "/");  //we set a listening address here (localhost)
                }
            }

        }

        private void ListenerCallBack(IAsyncResult result)
        {
            HttpListener listener = (HttpListener)result.AsyncState;

            HttpListenerContext context;
            try
            {
                context = listener.EndGetContext(result);
            }
            catch (Exception exp)
            {
                return; 
            } 

            HttpListenerResponse response = context.Response;

            string file = context.Request.Url.LocalPath;
            if (file == "/")
            {
                file = @"index.html";
            }

            string page = WebRoot + file;
            //this will get the page requested by the browser 

            String msg = "";
            try
            {
                if (context.Request.HttpMethod.Contains("GET"))
                {
                    msg = HandleGETRequest(page);
                }
                else if (context.Request.HttpMethod.Contains("POST"))
                {
                    msg = HandlePOSTRequest(context);
                }
                else
                {
                    throw new HttpListenerException(405, "Request method non supported.");
                }
            }
            catch (FileNotFoundException fnfe)
            {
                response.StatusCode = 404;
                Console.Out.WriteLine("/!/ 404 ! => " + fnfe.Message);

            }
            catch (Exception e)
            {
                Console.Out.WriteLine("Erreur ! => " + e.Message);
            }


            byte[] buffer = Encoding.UTF8.GetBytes(msg);
            //then we transform it into a byte array
            response.ContentLength64 = buffer.Length;  // set up the messasge's length
            Stream st = response.OutputStream;  // here we create a stream to send the message
            st.Write(buffer, 0, buffer.Length); // and this will send all the content to the browser
            context.Response.Close();  // here we close the connection

            this.WaitNextRequest();

        }

        private void StartListener()
        {
            Server.Start();
        }

        private void StopListener()
        {
            Server.Stop();
        }

        public String HandleGETRequest(String page) {
            TextReader tr = new StreamReader(page);
            String content = tr.ReadToEnd();  //getting the page's content
            tr.Close();
            return content;
        }

        public String HandlePOSTRequest(HttpListenerContext context)
        {
            var data_text = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding).ReadToEnd();
            var clean = HttpUtility.ParseQueryString(data_text);

            string hr = clean.Get("rate");
            BPM = hr;
            if (!Directory.Exists(@"./www/"))
            {
                Directory.CreateDirectory(@"./www/");
            }
            using (StreamWriter file = new StreamWriter(@"./www/hr.txt", false))
            {
                file.WriteLine(hr);
                file.Close();
            }

            return "OK"; // answer provided to the smart watch for an "ack"

        }

        

        


    }
}
