# 智能门锁
基于CH32V307单片机设计和开发的一款智能门锁，拥有密码、刷卡、指纹三种主流解锁方式，用户可以通过液晶显示屏和微信小程序实现人机交互。

# 硬件清单
- CH32V307
- 降压模块
- SG90舵机
- 12V电源适配器
- 指纹模块：AS608
- ESP8266联网模块
- 刷卡模块：XH3650-A1
- 1.44寸 TFT彩色显示屏
- EEPROM存储模块：AT24C02
- 串口语音模块：BY8301-16P
- 矩阵按键：Risym 4*4矩阵键盘

  # 接线
## 屏幕
- GND	接 降压GND
- VCC	接 降压3.3V
- SCL	接 主板PB3
- SDA	接 主板PB5
- RES	接 3.3V
- DC 接 主板PB3
- CS 接 主板PD4
- BLK	接 3.3V
## 舵机
- 棕 接 降压GND
- 红 接 降压5V
- 橙(PWM信号) 接 主板PA0
## 矩阵键盘
- C4 接 主板PC5
- C3 接 主板PE7
- C2 接 主板PE9
- C1 接 主板PE11
- R1 接 主板PE6
- R2 接 主板PE15
- R3 接 主板PD9
- R4 接 主板PD11
## 语音
- GND	接 降压GND
- VCC	接 降压5V
- TX 接	主板PB11
- RX 接 主板PB10
- SPK1 扬声器 1 输出
- SPK2 扬声器 2 输出
## 刷卡
- VIN 接 降压3.3V
- GND 接 降压GND 
- TXD 接 主板PB3 
## 指纹
- Vi 接 降压3.3V
- TX(黄) 接 主板PC3 
- RX(白) 接 主板PC2 
- GND 接 降压GND
# 联网
- VIN 接 降压5V
- GND 接 降压GND
- TX 接 主板PC1
- RX 接 主板PC0
## EEPROM
