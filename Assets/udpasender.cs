//第一种
using UnityEngine;
using System.Collections;
//引入库
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine.UI;
using System.Globalization;
using System.Collections.Specialized;

public class udpasender : MonoBehaviour
{


    //以下默认都是私有的成员
   
    public Transform father;
    public Vector3 zero = new Vector3(0, 0, 0);
    public Text txt;
    Socket socket; //目标socket
    EndPoint serverEnd; //服务端
    IPEndPoint ipEnd; //服务端端口
    string recvStr; //接收的字符串
    string sendStr; //发送的字符串
    byte[] recvData = new byte[1024]; //接收的数据，必须为字节
    byte[] sendData = new byte[1024]; //发送的数据，必须为字节
    int recvLen; //接收的数据长度
    Thread connectThread; //连接线程
    bool gyinfo;
    Gyroscope go;
    //初始化



    void SocketSend(string sendStr)
    {

        sendData = new byte[1024];
        sendData = Encoding.ASCII.GetBytes(sendStr);
        socket.SendTo(sendData, sendData.Length, SocketFlags.None, ipEnd);

    }


    public void resetzero()
    {

        zero = this.transform.TransformPoint(Vector3.forward * -10)-new Vector3(5.0f,-7.071f,5.0f);


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
    }

    // Use this for initialization
    public void OnClick_105()
    {

        gyinfo = SystemInfo.supportsGyroscope;
        go = Input.gyro;
        go.enabled = true;
        print(this.transform.TransformPoint(Vector3.forward * -10));
        ipEnd = new IPEndPoint(IPAddress.Parse("192.168.0.105"), 7402);
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        //定义服务端
        IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
        serverEnd = (EndPoint)sender;
        print("waiting for sending UDP dgram");
        InvokeRepeating("delayOpen", 0, 0.05f);

    }

    public void OnClick_108()
    {

        gyinfo = SystemInfo.supportsGyroscope;
        go = Input.gyro;
        go.enabled = true;
        print(this.transform.TransformPoint(Vector3.forward * -10));
        ipEnd = new IPEndPoint(IPAddress.Parse("192.168.0.108"), 7402);
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        //定义服务端
        IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
        serverEnd = (EndPoint)sender;
        print("waiting for sending UDP dgram");
        InvokeRepeating("delayOpen", 0, 0.05f);

    }



    void delayOpen()

    {

        if (gyinfo)
        {


            Vector3 a = go.attitude.eulerAngles;
            a = new Vector3(-a.x, -a.y, a.z); //直接使用读取的欧拉角发现不对，于是自己调整一下符号
                                              // SocketSend(a.x.ToString() + " " + a.y.ToString() + " " + a.z.ToString());
            txt.text = (this.GetComponent<Transform>().localEulerAngles.y).ToString();
            SocketSend((this.GetComponent<Transform>().localEulerAngles.y).ToString());
            //txt.text = a.x.ToString();
        }
    }

    void OnApplicationQuit()
    {
        SocketQuit();
    }
}






