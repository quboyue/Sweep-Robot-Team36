using UnityEngine;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

public class UDPServer7402 : MonoBehaviour
{
    public string ipAddress;
    public int ConnectPort;
    public string recvStr;//收信

    Socket socket;
    EndPoint clientEnd;
    IPEndPoint ipEnd;
    string sendStr;
    byte[] recvData = new byte[1024];
    byte[] sendData = new byte[1024];
    int recvLen;
    Thread connectThread;





    //初始化
    void InitSocket()
    {
        ipEnd = new IPEndPoint(IPAddress.Parse(ipAddress), ConnectPort);
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        socket.Bind(ipEnd);
        //定义客户端
        IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
        clientEnd = (EndPoint)sender;
        print("等待连接数据");


        //开启一个线程连接
        connectThread = new Thread(new ThreadStart(SocketReceive));
        connectThread.Start();
    }
    void SocketSend(string sendStr)
    {
        sendData = new byte[1024];
        sendData = Encoding.UTF8.GetBytes(sendStr);
        socket.SendTo(sendData, sendData.Length, SocketFlags.None, clientEnd);
    }
    //服务器接收
    void SocketReceive()
    {
  
        while (true)
        {

            print("等着收消息");
            recvData = new byte[1024];
            recvLen = socket.ReceiveFrom(recvData, ref clientEnd);
            print("收到消息");
            recvStr = Encoding.UTF8.GetString(recvData, 0, recvLen);
            Debug.Log("收到得信息 " + recvStr);
        }
    }

    //连接关闭
    void SocketQuit()
    {
        //关闭线程
        if (connectThread != null)
        {
            connectThread.Interrupt();
            connectThread.Abort();
        }
        //最后关闭socket
        if (socket != null)
            socket.Close();
        Debug.LogWarning("断开连接");
    }

    // Use this for initialization
    void Start()
    {
        InitSocket(); //在这里初始化server
    }

    void Update()
    {



    }


    void OnApplicationQuit()
    {
        SocketQuit();
    }




}