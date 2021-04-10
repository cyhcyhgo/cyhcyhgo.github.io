# 基于MQTT和Lora无线技术的温度监测物联网系统 搭建日志

### 一. 焊接
由于esp32模块和LoRa模块的针脚老师已经帮我们焊接完成，因此只需要焊接OLED的针脚。

### 二. 接线
由于使用母对母杜邦线接线会导致模块连接不稳定，因此我们小组决定将所有模块插在面包板上，使用公对公杜邦线接线。

其中ds18b02温度传感器的接线示意图如下：

![temperature.jpg](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-3/resources/temperature.jpg)

最终成品如下：
![item.jpg](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-3/resources/item.jpg)

### 三. 烧录代码
接有温度传感器的模块为节点部分，负责温度的检测与数据的发送。

由于示例代码使用的是MCP9701A测温模块，需要修改部分代码才可以使用。我们小组进行“本土化”翻译后的代码如下：

" /*
  LoRa Node With and DS18B02
  MCU：ESP32-S
  Lora: ai-tinker RA-02 sx1278 470 
  Tempture Sensor:MCP9701A
  created 2020.8.25
  by Bloomlj
*/

#include <LoRaNow.h>

//vspi for lora radio module
#define MISO 19
#define MOSI 23
#define SCK 18
#define SS 5

#define DIO0 4

//for 18b20
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus= 25 ; 

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
double tmp;

void setup() {
  Serial.begin(115200);
  Serial.println("LoRaNow便携节点");
  pinMode(oneWireBus,INPUT);
  sensors.begin();
   LoRaNow.setFrequencyCN(); // Select the frequency 486.5 MHz - Used in China
  // LoRaNow.setFrequencyEU(); // Select the frequency 868.3 MHz - Used in Europe
  // LoRaNow.setFrequencyUS(); // Select the frequency 904.1 MHz - Used in USA, Canada and South America
  // LoRaNow.setFrequencyAU(); // Select the frequency 917.0 MHz - Used in Australia, Brazil and Chile

  // LoRaNow.setFrequency(frequency);
  // LoRaNow.setSpreadingFactor(sf);
  // LoRaNow.setPins(ss, dio0);

   LoRaNow.setPinsSPI(SCK, MISO, MOSI, SS, DIO0); // Only works with ESP32

  if (!LoRaNow.begin()) {
    Serial.println("LoRa加载失败。检查你的连接。");
    while (true);
  }

  LoRaNow.onMessage(onMessage);
  LoRaNow.onSleep(onSleep);
  LoRaNow.showStatus(Serial);
}

void loop() {
  sensors.requestTemperatures(); 
  tmp = sensors.getTempCByIndex(0);
  LoRaNow.loop();
}


void onMessage(uint8_t *buffer, size_t size)
{
  Serial.print("收到信息: ");
  Serial.write(buffer, size);
  Serial.println();
  Serial.println();
}

void onSleep()
{
  Serial.println("休息中...");
  Serial.print("现在的温度是：");
  Serial.print(tmp);
  Serial.println("℃");
  delay(5000); // "kind of a sleep"
  Serial.println("发送信息：");
  LoRaNow.print("信息编号");
  LoRaNow.print(millis());
  LoRaNow.print("：");
  LoRaNow.print("现在的温度是：");
  //Serial.println("LoRaNow Message sended");
  LoRaNow.print(tmp);
  LoRaNow.print("℃");
  LoRaNow.send();
} "

没有温度传感器的模块为网关部分，负责接收节点的数据以及为上位机传输数据。我们小组将老师发布的代码进行优化后的代码如下：

" /*
  LoRaNow Simple Gateway with ESP32 setPins
  This code creates a webServer show the Message and use mqtt publish the data.
  created 27 04 2019
  by Luiz H. Cassettari
  modified 2020.09.19
  by bloomlj
*/

#include <LoRaNow.h>
#include <WiFi.h>
#include <WebServer.h>
#include <StreamString.h>
#include <PubSubClient.h>

//vspi
//#define MISO 19
//#define MOSI 23
//#define SCK 18
//#define SS 5

//hspi,only for ESP-32-Lora-Shield-Ra02
#define SCK 14
#define MISO 12
#define MOSI 13
#define SS 15

#define DIO0 4

const char *ssid = "test";
const char *password = "12345678";
const char *mqtt_server = "broker.hivemq.com";

WebServer server(80);
WiFiClient espClient;
PubSubClient mqttclient(espClient);

const char *script = "<script>function loop() {var resp = GET_NOW('loranow'); var area = document.getElementById('area').value; document.getElementById('area').value = area + resp; setTimeout('loop()', 1000);} function GET_NOW(get) { var xmlhttp; if (window.XMLHttpRequest) xmlhttp = new XMLHttpRequest(); else xmlhttp = new ActiveXObject('Microsoft.XMLHTTP'); xmlhttp.open('GET', get, false); xmlhttp.send(); return xmlhttp.responseText; }</script>";

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (60)
char msg[MSG_BUFFER_SIZE];

int value = 0;

void handleRoot()
{
  String str = "";
  str += "<html>";
  str += "<head>";
  str += "<title>Temperature monitor</title>";
  str += "<style type=\"text/css\">body{background: url(https://img.zcool.cn/community/015ea357afda5f0000018c1b2f966d.jpg@1280w_1l_2o_100sh.jpg) repeat-x center top fixed}</style>";
  str += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  str += script;
  str += "<link rel=\"SHORTCUT ICON\" href=\"https://cyhcyhgo.github.io/web_resourse/logo.jpg\">";
  str += "</head>";
  str += "<body onload='loop()'>";
  str += "<center>";
  str += "<textarea id='area' style='width:800px; height:400px;'></textarea>";
  str += "</center>";
  str += "</body>";
  str += "</html>";
  server.send(400, "text/html", str);
}

static StreamString string;

void handleLoRaNow()
{
  server.send(200, "text/plain", string);
  while (string.available()) // clear
  {
    string.read();
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("有新消息[");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void setup(void)
{

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  //if (ssid != "")
    WiFi.begin(ssid, password);
  WiFi.begin();
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("已经成功连接到：");
  Serial.println(ssid);
  Serial.print("IP地址: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/loranow", handleLoRaNow);
  server.begin();
  Serial.println("已启用HTTP服务器");

  LoRaNow.setFrequencyCN(); // Select the frequency 486.5 MHz - Used in China
  // LoRaNow.setFrequencyEU(); // Select the frequency 868.3 MHz - Used in Europe
  // LoRaNow.setFrequencyUS(); // Select the frequency 904.1 MHz - Used in USA, Canada and South America
  // LoRaNow.setFrequencyAU(); // Select the frequency 917.0 MHz - Used in Australia, Brazil and Chile

  // LoRaNow.setFrequency(frequency);
  // LoRaNow.setSpreadingFactor(sf);
  // LoRaNow.setPins(ss, dio0);

  LoRaNow.setPinsSPI(SCK, MISO, MOSI, SS, DIO0); // Only works with ESP32

  if (!LoRaNow.begin())
  {
    Serial.println("LoRa加载失败。检查你的连接。");
    while (true)
      ;
  }

  LoRaNow.onMessage(onMessage);
  LoRaNow.gateway();
  //mqtt
  mqttclient.setServer(mqtt_server, 1883);
  mqttclient.setCallback(callback);

}

void loop(void)
{
  LoRaNow.loop();
  server.handleClient();
  mqttloop();
}

void onMessage(uint8_t *buffer, size_t size)
{
  unsigned long id = LoRaNow.id();
  byte count = LoRaNow.count();

  Serial.print("节点ID: ");
  Serial.println(id, HEX);
  Serial.print("接收次数: ");
  Serial.println(count);
  Serial.print("信息内容: ");
  Serial.write(buffer, size);
  Serial.println();
  Serial.println();
  
   //此处通过mqtt发送信息。
   snprintf (msg, MSG_BUFFER_SIZE, "#%d#%s", count,buffer);
   Serial.print("发布信息: ");
   Serial.println(msg);
   mqttclient.publish("C2TLOutTopic", msg);

  if (string.available() > 512)
  {
    while (string.available())
    {
      string.read();
    }
  }

  string.print("节点ID: ");
  string.println(id, HEX);
  string.print("接收次数: ");
  string.println(count);
  string.print("信息内容: ");
  string.write(buffer, size);
  string.println();
  string.println();

  // Send data to the node
  LoRaNow.clear();
  LoRaNow.print("LoRaNow 网关信息");
  LoRaNow.print(millis());
  LoRaNow.send();
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttclient.connected()) {
    Serial.print("尝试与MQTT服务器进行连接");
    // Create a random mqttclient ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttclient.connect(clientId.c_str())) {
      Serial.println("连接成功");
      // Once connected, publish an announcement...
      mqttclient.publish("C2TLOutTopic", "hello world");
      // ... and resubscribe
      mqttclient.subscribe("C2TLInTopic");
    } else {
      Serial.print("失败, 错误代码：");
      Serial.print(mqttclient.state());
      Serial.println("。5秒后再次尝试。");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqttloop() {

  if (!mqttclient.connected()) {
    reconnect();
  }
  mqttclient.loop();

//  unsigned long now = millis();
//  if (now - lastMsg > 2000) {
//    lastMsg = now;
//    ++value;
//    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    mqttclient.publish("C2TLOutTopic", msg);
//  }
} "

其中WiFi的SSID与密码应该修改为自己使用的WiFi。

烧录完代码后，开启手机热点，待网关连接至热点后，即可正常工作。

### 四. 通过串口监视器接收信息
将节点和网关分别用数据线连接至电脑USB接口。打开串口监视器，波特率设置为115200。

节点上传的数据为：
![node.png](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-3/resources/node.png)

网关上传的数据为：
![gateway.png](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-3/resources/gateway.png)

### 五. 在手机上接收信息
以手机为上位机。当网关连接至手机热点后，打开MQTT客户端，订阅如下两个话题：C2TLInTopic, C2TLOutTopic。

前者用于向下位机发送信息，后者用于接收下位机的信息。

可通过修改代码更改话题。

接收的信息如下：
![MQTT.jpg](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-3/resources/MQTT.jpg)

### 六. 在电脑网页接收信息
网关也可以向电脑网页发送信息。使网关、电脑连接至同一热点，电脑端打开broker.hivemq.com，一段时间后即可在网页上接收下位机发来的数据。

可通过修改代码优化网页的显示。

接收的信息如下：
![web.png](https://github.com/cyhcyhgo/cyhcyhgo.github.io/blob/main/assignment-3/resources/web.png)

# 遇到的问题及解决方法

### 一. esp32模块无法识别LoRa
1. 对照老师的教程再次检查连线；
2. 网关和节点的连线不完全一样。网关需参照“hspi”部分接线，而节点需参照“vspi”部分接线；
3. 导线损坏。建议购买全新的杜邦线接线。

### 二. 编译失败
缺少相关库文件。需要安装“DallasTemperature”和“OneWire”支持库。

### 三. 温度显示异常
1. 温度传感器接线不正确。参考网上的教程进行接线，其中中间的DQ接线柱接esp32模块的35号引脚（以代码为准）。注意确认温度传感器的正反，以免接错导线；
2. 代码不正确。示例代码不适用于DS18B02温度传感器。需在网上查阅DS18B02温度传感器测量温度的代码并替换源代码当中的温度测量函数。

# 成功之处
1. 成功完成接线并修正了代码，可以测量出正确的温度；
2. 可以通过手机客户端和电脑网页获取数据；

# 还需要改进的地方
1. 由于网关和节点在一个面包板上，无法进行远距离数据传输；
2. 未使用OLED屏幕，无法通过下位机直接显示数据；
3. 没有对模块进行封装，导致稳定性较差。

