# 实战（乐鑫）
**以下三方的代码分别组合在一起即可运行**
## 发送视频方（树莓派）
1. 需要明确知道接受方的ip和端口
```
#服务器的ip+端口号  
HOST = '60.204.139.74'  # 主机号为空白表示可以使用任何可用的地址。60.204.139.74  
PORT = 8081  # 端口号
ADDR_send = (HOST, PORT)
```
2. 创建套接字
```
udpCliSock_send = socket(AF_INET, SOCK_DGRAM)
```
 - **AF_INET：** 这是套接字地址族（Address Family）的一个常量，表示IPv4地址族。在创建套接字时，你需要指定要使用的地址族，以便套接字能够与特定类型的网络通信
 - **SOCK_DGRAM：** 这是套接字类型（Socket Type）的一个常量，表示UDP套接字类型。在创建套接字时，你需要指定要使用的套接字类型，以决定套接字的通信特性，如面向连接还是无连接。
 3. 创建一个视频捕获对象，该对象将从默认的摄像头设备（索引号为0）获取视频帧。通过使用这个捕获对象，你可以连续地读取摄像头捕获的视频帧，进行后续的处理、显示或传输等操作
```
capture = cv2.VideoCapture(0)
```
4. 将摄像头数据发送给服务器
```
while True:  
    _, frame = capture.read()  
    #从摄像头中读取一帧视频。它会返回两个值：第一个是布尔值，表示是否成功读取到了一帧视频帧，第二个是实际的视频帧。
      
    frame = cv2.resize(frame, (300, 200))  
    #是帧调整为新的尺寸
    
    cv2.imshow("send", frame)  
    #显示图像窗口。函数接受两个参数：窗口的名称（这里是 "send"）和要显示的图像帧 `frame`。它将图像帧显示在一个命名为 "send" 的窗口中。
    
    cv2.waitKey(1)  
    #函数接受一个参数，表示等待的毫秒数。
    #运行该函数时，需要满足以下两个条件任一，后面的程序才会继续运行
    #1.按键  2. 等待1毫秒
    
    img_encode = cv2.imencode('.jpg', frame)[1]  
    #将视频帧 `frame` 压缩编码为 JPEG 格式，`cv2.imencode()` 函数返回一个元组，其中第一个元素是布尔值，表示是否成功地将图像编码为指定格式，第二个元素是实际的编码数据。通过 `[1]` 索引，获取到编码后的图像数据。
    #编码和压缩图像是优化数据传输和存储的常用手段，可以提高效率、降低成本，并更好地适应不同的应用需求和网络环境
    
    data = img_encode.tobytes()  
    #将压缩编码后的图像数据从 `numpy` 数组转换为字节形式的数据，以便后续通过网络传输或其他方式进行处理
    #网络通信和存储操作通常都是基于字节数据进行的，因此需要将图像数据转换为字节，以便能够在网络上传输或存储到文件中。
    
    print(len(data))  
    #输出图像的大小
    
    udpCliSock_send.sendto(data, ADDR_send)
    #将图像数据发送给目标主机，以进行实时的图像传输 
	
    time.sleep(0.1)
    #在每次循环迭代之间引入一个短暂的延迟0.1s
    #通过在每次循环迭代之间引入一个适当的延迟，比如 `time.sleep(0.1)`，可以有效控制数据发送的速率，降低系统资源消耗，避免数据丢失，并确保目标检测程序有足够的时间来处理接收到的图像数据。你可以根据实际情况调整延迟时间，以平衡数据传输速度和目标程序处理的需求。
```
5. 关闭套接字
```
udpCliSock_send.close()
```
## 服务端
1. 创建UDP套接字
```
udpClientSock = socket(AF_INET, SOCK_DGRAM)
```
2. 套接字绑定ip和端口（**第一个接受消息的对象必须绑定，而其他任何对象都可以不绑定**）
```
HOST = '0.0.0.0'   
#如果是服务器就设置为 0.0.0.0 ，如果是局域网内部的主机就设置为 #127.0.0.1  “why!!!”
PORT = 8081  # 端口号  
ADDR = (HOST, PORT)  

BUFSIZ = 300000  
# 接收数据缓冲大小，一定要大于单个数据报的最大大小  

udpSerSock = socket(AF_INET, SOCK_DGRAM)  
udpSerSock.bind((HOST, PORT))
#作为第一个接收消息的对象的套接字必须绑定ip和端口，不然无法获取消息
```
3. 本项目背景：树莓派上传视频流到服务器，服务器处理视频流并发送给app。先让服务器接收app的数据，从而获取app经内网穿透后的ip和端口，进而可以在视频流处理后发送给app
```
print("=====================Timestamp UDP server=====================")  
while True:  
    request, client_addr = udpSerSock.recvfrom(1024)  
    print(request)  
    if request == b"Request":  # 直接比较二进制数据  
        print("Received request from {}:{}".format(client_addr[0], client_addr[1]))  
        # 记录客户端信息  
        break
```
4. 称之为fps不准确，意为每接受到多少帧图片就进行一次AI处理，其他的图片显示原图
```
fps = 4
```
5. 对图片进行处理并发送给APP
```
num = 0  
skip = True  
#上面两个变量都是为了辅助fps

print('Waiting to receive data...') 
while True:  
    num += 1  
    if num % fps == 0:  
        skip = False  
    else:  
        skip = True  
    data, _ = udpSerSock.recvfrom(BUFSIZ)  
    # 接收数据，数据的最大字节大小由`BUFSIZ`定义。`recvfrom` 函数返回接收到的数据以及发送方的地址信息，但由于在这里你不需要发送方的地址信息，所以使用 `_` 代替来舍弃该信息
    
    #如果skip=False，即进行AI处理，并输出检测出的信息
    if not skip:  
		buf = BytesIO(data)  
	    #使用 `BytesIO` 类创建了一个字节流对象 `buf`，并将 `data_org` 的二进制数据传递给它
	    
	    frame = np.array(Image.open(buf))  
	    #从 `buf` 中读取图像数据，然后将其转换为NumPy数组 `frame`。这样，你就可以将接收到的图像数据转换为可用于OpenCV等图像处理库的NumPy数组格式。
		
	    frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)  
	    #将图像从RGB颜色空间转换为BGR颜色空间，这是OpenCV通常使用的颜色空间。这个转换确保图像的颜色通道正确，以便后续的处理
        
        info = predict_(frame)  
        #对图像进行YOLOv5处理
        #cv2.imshow('yolov5', frame)  
        #cv2.waitKey(100)  
        print(info)  
        
        encoded_image = cv2.imencode('.jpg', frame)[1]  
        #压缩图像
    
    
    #cv2.imshow('yolov5', frame)  
    #无论处理与否，都显示出图像
    #cv2.waitKey(10)
    
    #无论处理与否，都将图像发给app
	
	
	image_data = encoded_image.tobytes()  
	udpSerSock.sendto(image_data, client_addr)
```
6. 关闭套接字
```
udpSerSock.close()
```
## APP端
1. 创建UDP套接字
```
process_b_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
```
2. 指定服务器端(处理后的视频流)IP和端口
```
process_a_ip = '60.204.139.74'
process_a_port = 8081
```
3. 先向服务端发送消息，以使服务端知道APP端的外网IP和端口
```
message_to_send = b"Request"  
process_b_socket.sendto(message_to_send, (process_a_ip, process_a_port))
```
4. 接受服务端的视频流并显示
```while True:  
        data, process_a_addr = process_b_socket.recvfrom(800*800)  
        # 接收来自服务端的消息(视频流)  
        
        nparr = np.frombuffer(data, np.uint8)  
        #将图片形式转换为十六进制  
        
        frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)  
        # 解码数据（数据是JPEG格式）  
        
        frame = cv2.resize(frame, (640, 480))  
        #变换图像大小  
        
        cv2.imshow("Video Stream", frame)  
        cv2.waitKey(10)
```
5. 套接字关闭
```
process_b_socket.close()
```

## 应对方案
如果网络好
1. 树莓派中的frame = cv2.resize(frame, (400, 300))，可以调大些
2. 服务端中的BUFSIZ = 30000 * 4，接受区大小可以设置小一些
3. 服务端的pfs可以设置为2，甚至是1，即每张图片都进行AI处理
4. APP端的接受区可以设置小一点







