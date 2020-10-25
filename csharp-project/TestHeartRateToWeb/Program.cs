using System;
using System.Net;
using System.IO;
using System.Text;
using HRServer;

namespace TestHeartRateToWeb
{
    class Program
    {
        static void Main(string[] args)
        {

            HeartRateServer Server = new HeartRateServer(6547);

            Server.StartServer();

            Console.ReadLine();

            Server.StopServer();




        }
    }
}
