import socket

def main():
    angle=0
    inital=0
    # 1创建套接字
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # 2.绑定一个本地信息
    localaddr = ("192.168.0.105",7402) # 必须绑定自己电脑IP和port
    udp_socket.bind(localaddr)
    # 3.接收数据
    while True:
       
        recv_data = udp_socket.recvfrom(1024)
        # recv_data存储元组（接收到的数据，（发送方的ip,port））
        recv_msg = recv_data[0] # 信息内容
        send_addr = recv_data[1] # 信息地址
        # 4.打印接收到的数据
        # print(recv_data)
        REC=float(recv_msg.decode("gbk").split(" ")[-1])
        
        if inital<10:
            inital+=1
            angle+=REC
        print(REC-(angle/inital))
        
        

    udp_socket.close()
if __name__ == "__main__":
    main()

