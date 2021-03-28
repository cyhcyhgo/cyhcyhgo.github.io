# esp32开发环境搭建日志

### 一. 下载Arduino IDE
打开网站https://www.arduino.cc/en/software ，下载相应版本的压缩文件，解压缩后便得到应用程序。

### 二. 配置开发环境
1. 依次点击文件->首选项，在“附加开发板管理”一栏中输入https://codechina.csdn.net/mirrors/espressif/arduino-esp32/-/raw/gh-pages/package_esp32_index.json ，点击右边按钮；
2. 点击工具->开发板->开发板管理器，搜索“esp”，下载esp32管理器；
3. 在工具->开发板中选择“Node32s”；
4. 重启软件。

### 三. 安装串口驱动程序
1. 打开网站https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers ，下载相应版本的压缩文件；
2. 解压缩，运行安装程序，按照相关指示完成安装；
3. 使用数据线将开发板与电脑相连接，打开控制面板->设备管理器->端口，确认开发板是否与电脑连接，以及开发板所在的串口编号。

### 四. 上传并运行示例程序
1. 在开发软件中点击文件->示例->ESP32->ChipID->GetChipID，打开示例代码；
2. 在工具->端口处选择开发板所在的串行端口；
3. 点击工具->串口监视器打开串口监视器，将波特率设置为115200；
4. 点击左上角“上传”按钮（“->”按钮），等待代码编译及上传；
5. 若可在串口监视器中看见程序打印的芯片ID（如下图所示），说明代码可以正常编译并上传至开发板。开发环境搭建完成。
![preview.png](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-1/resource/preview.png)

# 遇到的问题

### 一. 下载速度过慢或无法打开相应网站。
解决方法：电脑端下载并启用vpn代理服务。

### 二. 上传代码后出现如下错误信息：
![error.png](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-1/resource/error.png)
解决方案：没有安装串口驱动程序。按照第三步完成驱动程序安装后可正常上传代码。

### 三. 代码上传速度过慢。
解决方案：暂未找到相应解决方案。（附：可以尝试使用小组其他成员的电脑重新上传，但因地理问题目前尚未有良好的方案）

### 四. 无法看见程序的输出结果。
解决方案：需要打开串口监视器查看程序的输出结果。

### 五. 程序没有输出结果/输出结果为乱码。
解决方案：检查波特率是否正确设置。将波特率改成与函数Serial.begin()里面相同的数字。
