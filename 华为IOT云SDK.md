# 连接
## 库下载
- 部分库在huaweicloud-iot-device-sdk的generatingLib里面有
- 更新系统
	- sudo apt-get update
	- sudo apt-get install gcc
	- sudo apt-get install make
**注：以下的所有库文件，如果硬件中已经存在，就不需要安装、编译，跳过即可，树莓派4B烧录系统后已经有openssl，故不需进行该步骤**
### openssl
wget https://github.com/openssl/openssl/archive/OpenSSL_1_1_1g.tar.gz
tar -zxvf OpenSSL_1_1_1g.tar.gz
cd openssl-OpenSSL_1_1_1g
./config no-asm shared --prefix=/home/fuxiaoiii/test/openssl --openssldir=/home/fuxiaoiii/test/openssl/ssl
make depend
make -j4
sudo make install
- libcrypto.so.1.1、libssl.so.1.1和软链接libcrypto.so、libssl.so，请将这些文件拷贝到SDK的lib文件夹下（同时将home/test/openssl/include底下的openssl文件夹拷贝到SDK的include目录下）。
'''
添加到环境变量
	which openssl
	vim ~/.bashrc
	export PATH="/usr/local/bin:$PATH"      /usr/bin/openssl
	source ~/.bashrc
'''
### **paho：**
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
vim Makefile
在"DOXYGEN_COMMAND"之后添加下面两行（openssl的头文件和库文件位置）
	CFLAGS += -I/home/**path**/include        
	LDFLAGS += -L/home/**path**/lib -lrt
make clean
make
cd build
ls
cd output
ls
- 拷贝paho库文件 ：当前SDK仅用到了libpaho-mqtt3as，请将文件libpaho-mqtt3as.so和libpaho-mqtt3as.so.1拷贝到SDK的lib文件夹下
- 同时将paho源码目录下src文件夹里的头文件（MQTTAsync.h/MQTTClient.h/MQTTClientPersistence.h/MQTTProperties.h/MQTTReasonCodes.h/MQTTSubscribeOpts.h）拷贝到SDK的include/base目录下，注意：有的paho版本会有 MQTTExportDeclarations.h 头文件，**建议可以将MQTT相关的头文件都添加进去**。

### zlib
wget https://github.com/madler/zlib/archive/v1.2.11.zip
cd zlib-1.2.11
./configure
make
- 复制 .so 库文件。 将源代码目录中生成的 **libz.so**、**libz.so.1** 和 **libz.so.1.2.11** 文件复制到 SDK 的 **lib** 文件夹中

### huawei_secure_c库
https://gitee.com/openeuler/libboundscheck.git
在这里下载压缩包，解压后上传
cd 进入
make
- 复制 .so 库文件。 将 **lib** 目录源代码中生成的 **libboundscheck.so** 文件复制到 SDK 的 **lib** 文件夹中

### libssh
wget https://www.libssh.org/files/0.10/libssh-0.10.4.tar.xz
tar xvf libssh-0.10.4.tar.xz
cd libssh-0.10.4
mkdir build
cd build
sudo apt-get install libssl-dev libgcrypt-dev libmbedtls-dev
cmake ..
make
sudo make install
-  复制 .so 库文件和头文件。 将源码目录中生成的 **libssh.so**、**libssh.so.4** 和 **libssh.so.4.9.4** 文件复制到 SDK 的 **lib** 文件夹中。 
- 将 **usr/local/include** 目录中的 **libssh** 文件夹复制到 SDK 的**include**文件夹中。
### libnopoll
wget http://www.aspl.es/nopoll/downloads/nopoll-0.4.8.b429.tar.gztar 
xzvf nopoll-0.4.8.b429.tar.gz
cd nopoll-0.4.8.b429
./configure
make -j4
make install
- 拷贝so库文件 通过上一步获取到的路径，将源码目录下生成的libnopoll.so libnopoll.so.0 libnopoll.so.0.0.0拷贝到sdk的lib文件夹下。
- 将/usr/local/include下的libnopoll的整个头文件目录拷贝到sdk的include文件夹下。
### huaweicloud-iot-device-sdk
git clone https://github.com/huaweicloud/huaweicloud-iot-device-sdk-c.git
删除 Makefile中的-m64
- 修改device_demo.c文件中
	- static void Test_PropertiesReport(void)![image.png](https://cdn.jsdelivr.net/gh/fuxiaoiii/Pictures/20230712221845.png)
	- char *service1 = "{\"Temperature\":\"27\"}";
	- services[0].service_id = "DeviceInfo";

## 创建设备
- [设备接入-控制台 (huaweicloud.com)](https://console.huaweicloud.com/iotdm/?region=cn-north-4#/dm-portal/home) 中创建产品、设备
	- 设备的服务名字为DeviceInfo
	- 该服务下新建属性为Temperature
- 将设备信息填入device_demo.c
## 不同端口的连接
### 1883
1883端口不需要证书鉴权，没有加密，仅修改![image.png](https://cdn.jsdelivr.net/gh/fuxiaoiii/Pictures/20230712222053.png)
为注册设备时生成的文件即可
### 8883端口
- 8883端口需要证书鉴权~~（可能需要改SDK目录下的src/base/mqtt_base.c中的static int g_verifyCert为1）~~SDK目录下的conf中的证书要换为[资源获取_设备接入 IoTDA_开发指南_华为云 (huaweicloud.com)](https://support.huaweicloud.com/devg-iothub/iot_02_1004.html) 中对应的![image.png](https://cdn.jsdelivr.net/gh/fuxiaoiii/Pictures/20230712170326.png)
###  X.509证书
- 配置过程参考[基于MQTT.fx的X.509证书接入指导_设备接入 IoTDA_最佳实践_设备接入_华为云 (huaweicloud.com)](https://support.huaweicloud.com/bestpractice-iothub/iot_bp_0077.html)，后面的连接鉴权可以选择是否做，如果做成功了，则代表该证书和该云设备配置成功
- 设备的密码是生成证书时的填写的密码，如果没有填写则为空
### 操作
```
 make
 export LD_LIBRARY_PATH=./lib/
 ./MQTT_Demo
```























## 注意事项
- 基础版的serverip不用改，标准版的是![image.png](https://cdn.jsdelivr.net/gh/fuxiaoiii/Pictures/20230712170512.png)
- 版本一定要对应！！！不然会有各种报错
- 如果报错TCP/TLS连接失败，应该是以下问题之一
	- 1、证书没配好
	- 2、libssl.so文件有问题
- 如果报错TCP连接失败
	- 可能是device_demo.c文件中的设备id和密码有问题


⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
⚠️upload failed, check dev console
