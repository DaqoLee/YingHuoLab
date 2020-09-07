# 粉丝数显示器


![粉丝数显示器](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/YingHuo2.22.png?raw=true)

有两大部分组成：主控部分和显示部分
![粉丝数显示器](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/MindMap.png?raw=true)


其中主控部分和墨水屏是一体的，可以单独使用。
![粉丝数显示器](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/YingHuo2.20.png?raw=true)
![粉丝数显示器](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/YingHuo2.21.png?raw=true)
墨水屏我是在
[微雪电子](https://www.waveshare.net/wiki/2.13inch_e-Paper_HAT)买的，这里有很多资料，也有配套的ESP32开发板。
![waveshare](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/waveshare.png?raw=true)


 
 一个墨水屏就可以显示基本的天气、时钟、粉丝数以及贪吃蛇小游戏。
![墨水屏](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/e-Paper.jpg?raw=true)

不过墨水屏有个通病就是很容易出现残影，时间久了影响显示，一般断电几分钟后才能恢复。  

拟辉光管部分就是LED灯珠、亚克力板、3D打印框架。 

![acrylic](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/acrylic.png?raw=true)

不想自己做的，网上也是可以买到的 
![粉丝数显示器](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/YingHuo2.23.png?raw=true)

这种看上去比较好看，并且是磁吸式的，后期更换比较方便。


## 代码
### ESP32
#### B站数据和天气的数据
B站数据和天气的数据(这里用的是[心知天气](https://www.seniverse.com/))，GET回来JSON数据然后解析。
![数据](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/BiliBiliJson.png?raw=true)

![数据](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/weatherJson.png?raw=true)

参考[粉丝显示器教程](https://blog.csdn.net/qq_39400113/article/details/105348187?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522159023413819724843309912%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=159023413819724843309912&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_v2~rank_v25-4-105348187.first_rank_v2_rank_v25&utm_term=esp8266+B%E7%AB%99%E7%B2%89%E4%B8%9D)
#### 时钟
时钟用的是NTP的库，开了一个定时器计时，一个小时联网校准一次。
#### WS2812灯珠
用的是ESP32内置的RMT库输出数字波形，并且提供了控制ws2812的范例程序(arduinoIDE-文件-示例-esp32-RMT-RMTWriteNeoPixel)。参考[WS2812使用教程](https://www.pianshen.com/article/8536257837/)


#### 贪吃蛇
参考教程[OLED贪吃蛇教程](https://blog.csdn.net/weixin_42653531/article/details/94662081)

### 小程序

![小程序](https://github.com/DaqoLee/YingHuoLab/blob/master/Image/WeChat.png?raw=true)
小程序是在[串口助手](https://blog.csdn.net/INTKILOW/article/details/106012736)的基础上修改的。更具自己的需求添加图标设置指令就行了。

## 致谢
参考的连接已经在文章中了，由衷感谢开源的的前辈们。


