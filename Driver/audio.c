#include "uart.h"
#include "debug.h"

/* 引脚初始化 */
void audio_init()
{
    Usart3_Init();
}

/* 
 * 语音播放
 * 1. 按键音效
 * 2. 门已上锁，请刷卡或输入密码解锁
 * 3. 密码正确，门已打开，欢迎回家
 * 4. 密码错误，请重新输入
 * 5. 连续三次输入密码错误，已锁定，请等待30秒后重新输入
 * 6. 修改开锁密码，请输入管理员密码
 * 7. 管理员密码验证成功，请输入新密码
 * 8. 新密码设置成功
 * 9. 管理员密码错误，请重新输入
 * 10. 连续三次输入管理员密码错误，已锁定，请等待30秒后重新输入
 * 11. 刷卡成功，门已开启，欢迎回家
 * 12. 刷卡失败，请重试
 * 13. 录入卡片，请输入管理员密码
 * 14. 管理员密码验证成功，请将卡片平放在传感器上
 * 15. 卡片添加成功
 * 16. 指纹验证成功，门已开启，欢迎回家
 * 17. 指纹验证失败，请重试
 * 18. 录入指纹，请输入管理员密码
 * 19. 管理员密码验证成功，请将手指平放在传感器上
 * 20. 指纹正在录入
 * 21. 指纹录入成功
 */
void audio_play(u8 num)
{
//    u8 string[]={0x7E,0x05,0x41,0x00,num,0x05^0x41^0x00^num,0xEF};
    u8 string[]={0x7E,0x05,0x41,0x00,num,0x44^num,0xEF};

   u8 i;
   for(i=0;i<7;i++)
   {
       USART_SendData(USART3,string[i]);    // 发送数据
       while(  USART_GetFlagStatus(USART3, USART_FLAG_TC)==0 ); // 等待发送完成，保证语音播报完整
   }
}

/*  */
void audio_yinliang(u8 yinliang)
{
    u8 string[]={0x7e,0x04,0x31,yinliang,0x04^0x31^yinliang,0xef};
      u8 i;
      for(i=0;i<6;i++)
      {
          USART_SendData(USART3,string[i]);
          while(  USART_GetFlagStatus(USART3, USART_FLAG_TC)==0 );
      }
}
