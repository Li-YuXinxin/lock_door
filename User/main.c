/* 头文件包含区 */
#include "debug.h"      // 系统基础头文件
#include "lcd.h"        // LCD显示屏头文件
#include "pic.h"        // 图片资源头文件
#include "timer.h"      // 定时器头文件
#include "key.h"        // 按键头文件
#include "uart.h"       // 串口头文件
#include "audio.h"      // 音频模块头文件
#include "string.h"     // 字符串函数头文件
#include "as608.h"      // 指纹模块头文件
#include "esp8266.h"    // ESP8266模块头文件
#include "iic.h"        // EEPROM模块头文件
#include "telecontrol.h"

/* 全局变量定义区 */
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));    // 定时器3中断
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));  // 串口3中断
u8  string_chek(u8* string1,u8* string2,u8 len);                                // 字符串比较函数，相同返回1，不同返回0
void key_clear();                                                               // 清空按键数据缓存
void string_copy(u8* string1,u8* string2,u8 len);
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));  // 串口2中断
u8 rfid_chek();
void UART7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));   // 串口7中断
void UART6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));   // 串口6中断
void Ps_Wait();
u32 json(u8* string, u8* params, u8 len);
void SysTick_Handler(void);
uint32_t get_tick(void);

/* 全局变量 */
unsigned long int uwtick;               // 系统时基计数
u8 key_val,key_old,key_down,key_up;     // 按键相关变量
u8 password[6]={1,2,3,4,5,6};           // 用户密码
u8 password_cmd[6]={7,2,9,2,6,6};       // 管理员密码
u8 key_temp[7]={10,10,10,10,10,10};     // 按键输入缓存
u8 key_index;                           // 当前输入的按键数
u8 key_index_old;                       // 上一次输入的按键数
u16 time5000ms;                         // 5秒定时计数
u8 lock_flag=1;                         // 门锁状态标志
u8 password_error;                      // 密码错误次数
u16 time30s=30;                         // 密码错误锁定倒计时30秒
u16 time1000ms;                         // 1秒定时计数
u8 show_flag;                           // 显示标志位
u8 show_flag_old;                       // 上一次显示标志位
u8 mode;                                // 当前模式
                                        // 0首页 1修改用户密码（需管理员权限） 2修改管理员密码 3录入卡片（需管理员权限）
                                        // 4录入卡片（管理员） 5录入指纹（需管理员权限） 6录入指纹（管理员）
u8 rfid_index;                          // 刷卡模块索引
u8 rfid_temp[4]={0};                    // 刷卡数据缓存
u8 rfid[4][4]={0};                      // 卡片存储
u8 rfid_password_index;                 // 卡片存储索引
u8 uart7_rec_string[20]={0};            // 串口7接收缓存
u8 uart7_rec_index;                     // 串口7接收索引
u8 uart7_rec_tick;                      // 串口7接收定时
u8 ps_wait_flag;                        // 指纹等待标志
u8 as608_proc_falg;                     // 指纹处理标志
u8 as608_proc_falg_old;                 // 上一次指纹处理标志
u8 as608_store_index=1;                 // 指纹存储索引
u8 uart6_rec_string[256]={0};           // 串口6接收缓存
u8 uart6_rec_tick;                      // 串口6接收定时
u8 uart6_rec_index;                     // 串口6接收索引
char line[256] = "";
int line_len = 0;
volatile uint32_t sys_ms = 0;           // 毫秒计数


/* 按键处理函数 */
void key_proc() {

    key_val=key_read();
    key_down=key_val&(key_val^key_old);
    key_up=~key_val&(key_val^key_old);
    key_old=key_val;

    // 密码错误3次锁定
    if(password_error==3) return;

    // 按下按键有音效
    if(key_down) audio_play(1);

    switch(key_down) {
        case 1:
            key_temp[key_index]=1;  
            key_index++;      
        break;
        
        case 2:
            key_temp[key_index]=2;
            key_index++;
        break;
        
        case 3:
            key_temp[key_index]=3;
            key_index++;
        break;
        
        // 修改用户密码
        case 4:
            if(mode==0) {
                mode=1;
                audio_play(6);
                password_error=0;
                key_clear();
            }
        break;
        
        case 5:
            key_temp[key_index]=4;
            key_index++;
        break;
        
        case 6:
            key_temp[key_index]=5;
            key_index++;
        break;
        
        case 7:
            key_temp[key_index]=6;
            key_index++;
        break;
        
        // 录入指纹
        case 8:
            if(mode==0) {
                mode=5;
                audio_play(18);
                password_error=0;
                key_clear();
            }
        break;
        
        case 9:
            key_temp[key_index]=7;
            key_index++;
        break;
        
        case 10:
            key_temp[key_index]=8;
            key_index++;
        break;
        
        case 11:
            key_temp[key_index]=9;
            key_index++;
        break;
        
        // 录入卡片
        case 12:
            if(mode==0)
            {
                mode=3;
                audio_play(13);
                key_clear();
            }
        break;
        
        // 取消
        case 13:
            key_clear();
            if(mode==6)
            {
                PS_Empty();
                mode=0;
            }
        break;
        
        case 14:
            key_temp[key_index]=0;
            key_index++;
        break;
        
        // 删除
        case 15:
            if(key_index) {
                key_index--;
                key_temp[key_index]=10;
            }
        break;

        // 确认
        case 16:
            switch(mode)
            {
                // 首页
                case 0:
                    if( string_chek(key_temp,password,6) ) {
                        lock_flag=0;
                        show_flag=1;
                        audio_play(3);
                        key_clear();
                        password_error=0;
                    } else {
                        audio_play(4);
                        key_clear();
                        if(++password_error==3) {
                            audio_play(5);
                            show_flag=2;
                        }
                    }
                break;

                // 修改用户密码需管理员权限

                case 1:
                    if( string_chek(key_temp,password_cmd,6) )
                    {
                        mode=2;
                        audio_play(7);
                        key_clear();
                    }
                    else {
                        audio_play(9);
                        if(++password_error==3)
                        {
                            audio_play(10);
                            show_flag=2;
                        }
                    }
                break;

                //修改密码
                case 2:
                    string_copy(key_temp,password,6);
                    audio_play(8);
                    mode=0;
                    key_clear();
                break;

                case 3:
                    if( string_chek(key_temp,password_cmd,6) )
                    {
                        mode=4;
                        audio_play(14);
                        key_clear();
                    }
                    else {
                        audio_play(9);
                        if(++password_error==3)
                        {
                            audio_play(10);
                            show_flag=2;
                        }
                    }
                break;
                
                case 5:
                    if( string_chek(key_temp,password_cmd,6) )
                    {
                        mode=6;
                        audio_play(19);
                        key_clear();
                    }
                    else {
                        audio_play(9);
                        if(++password_error==3)
                        {
                            audio_play(10);
                            show_flag=2;
                        }
                    }

                break;
            }
        break;
    }
}

/* 门锁处理函数 */
void lock_proc() {
  lock(lock_flag);
}

/* 联网处理函数 */
void esp8266_proc() {
  if(uart6_rec_index==0) return;

  if(uart6_rec_tick>10)
  {
     usart1_send_string(uart6_rec_string,uart6_rec_index);

     char* add1 = strstr(uart6_rec_string,"appkey");
     int appkey=0;
     sscanf(add1,"appkey\":%d",&appkey);

     char* add2 = strstr(uart6_rec_string,"appstring");
     int appstring=0;
     sscanf(add2,"appstring\":\"%d",&appstring);

    // LCD_ShowIntNum(0, 80,appkey,3, BLUE, GREEN, 16);
    // LCD_ShowIntNum(0, 100,appstring,6, BLUE, GREEN, 16);
     u8 temp2_string[]="AT+MQTTPUB=0,\"$sys/56HeNPU4AK/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"appkey\\\":{\\\"value\\\":0}}}\",0,0\r\n";

    switch(appkey) {
        case 1:
            lock_flag=0;
        break;
        
        case 2:
            lock_flag=1;
        break;
        
        case 3:
            password[0]=appstring/100000%10;
            password[1]=appstring/10000%10;
            password[2]=appstring/1000%10;
            password[3]=appstring/100%10;
            password[4]=appstring/10%10;
            password[5]=appstring%10;
            EEPROM_Write(password, 8, 6);
            u8 temp_string[]="AT+MQTTPUB=0,\"$sys/56HeNPU4AK/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"appkey\\\":{\\\"value\\\":6}}}\",0,0\r\n";
            uart6_send_string(temp_string,sizeof(temp_string)-1);
        break;

        case 4:
                audio_yinliang(appstring);
                u8 temp3_string[]="AT+MQTTPUB=0,\"$sys/56HeNPU4AK/ch32/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"appkey\\\":{\\\"value\\\":7}}}\",0,0\r\n";
                uart6_send_string(temp3_string,sizeof(temp3_string)-1);
        break;

        case 7:
            uart6_send_string(temp2_string,sizeof(temp2_string)-1);
        break;

        case 8:
            uart6_send_string(temp2_string,sizeof(temp2_string)-1);
            break;

    }
    uart6_rec_index=0;
  }

}

/* 指纹处理函数 */
void as608_proc() {
    as608_proc_falg=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);
    if(as608_proc_falg==as608_proc_falg_old)return;
    as608_proc_falg_old=as608_proc_falg;
    if(as608_proc_falg==0)return;

    // 录入指纹
    if(mode==6)
    {
        PS_GetImage();
        Ps_Wait();
        PS_GenChar(1);
        Ps_Wait();
        PS_GetImage();
        Ps_Wait();
        PS_GenChar(2);
        Ps_Wait();
        PS_RegModel();
        Ps_Wait();
        PS_StoreChar(as608_store_index);
        as608_store_index++;
        Ps_Wait();
        audio_play(21);
        mode=0;
    }

    // 验证指纹
    if(mode==0)
    {
        PS_GetImage();
        Ps_Wait();
        PS_GenChar(2);
        Ps_Wait();
        PS_Search();
        Ps_Wait();

        if(uart7_rec_string[13]>50)
        {
            audio_play(16);
            lock_flag=0;
            show_flag=1;
        }
        else
        {
            audio_play(17);
        }
    }
}

/*显示屏处理函数*/
void lcd_proc() {
    if(key_index!=key_index_old)
    {
         u8 i=key_index;
         LCD_Fill(16,45,112,66,YELLOW);
         if(key_index==7)key_index=6; 
         while(i--)
         {
             if(i<6)
             LCD_ShowChar(20+16*i,45,'*',RED,YELLOW,16,0);
         }
         key_index_old=key_index;
    }

   if(show_flag!=show_flag_old)
   {
       LCD_Fill(0,0,128,32,WHITE);
      show_flag_old=show_flag;
   }

     switch(show_flag)
        {
        case 0:
        lcd_show_chinese(0,0,"门已上锁，请输入密码",RED,WHITE,16,0);
        break;
        case 1:
        lcd_show_chinese(0,0,"开锁成功，欢迎回家",RED,WHITE,16,0);
        break;
        case 2://锟斤拷锟斤拷锟斤拷锟斤拷时
        LCD_ShowIntNum(50, 16,time30s,2, RED,WHITE,16);
        break;
        }
}


void TIM3_IRQHandler(void) {
    if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
    {
      uwtick++;
      uart7_rec_tick++;
      uart6_rec_tick++;
      if(lock_flag==0)
      {
        if(++time5000ms==5000)
        {
            time5000ms=0;
            lock_flag=1;
            show_flag=0;
            audio_play(2);
        }
      }

      if(password_error==3)
      {
          if(++time1000ms==1000)
          {
              time1000ms=0;
              if(--time30s==0)
              {
               time30s=15;
               password_error=0;//
               show_flag=0;
              }
          }
      }


    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

typedef struct {
    void (*task_func)(void);
    unsigned long int rate_ms;  
    unsigned long int last_run; 
}task_t;
task_t scheduler_task[]={
        {lcd_proc,100,0},
        {key_proc,10,0},
        {lock_proc,30,0},
        {as608_proc,20,0},
        {esp8266_proc,2,0},
        {uart6_check_line,2,0}
};
unsigned char task_num; 
void scheduler_init() {
    task_num=sizeof(scheduler_task)/sizeof(task_t); 
}
void scheduler_run() {
    unsigned char i;
    for(i=0;i<task_num;i++)
    {
        unsigned long int now_time=uwtick;  
        if(now_time> (scheduler_task[i].last_run+scheduler_task[i].rate_ms) )
        {
            scheduler_task[i].last_run=now_time;
            scheduler_task[i].task_func();
        }
    }
}


/*锟斤拷锟斤拷锟斤拷*/
int main(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     // 中断优先级配置
	SystemCoreClockUpdate();                            // 系统时钟更新
	USART_Printf_Init(115200);                          // 串口初始化
	Delay_Init();                                       // 延时初始化
    key_init();                                         // 按键初始化
	TIM2_PWM_Init();                                    // 定时器2PWM初始化
    lock(1);                                            // 默认锁定
    LCD_Init();                                         // LCD初始化
    LCD_Fill(0,0,127,127,WHITE);                        // 清屏
    lcd_show_chinese(30,50,"智能门锁",RED,WHITE,16,0);   // 显示标题
    unsigned char i=0;                                  // 循环变量
    while(i<128) {                                      // 绘制背景
        LCD_DrawLine(i,100,i,128,RED);
        Delay_Ms(10);
        i++;
    }

   lcd_show_chinese(30,50,"正在开机",RED,WHITE,16,0);  // 显示提示
    Delay_Ms(500);

    LCD_ShowPicture(0,0,128,128,gImage_2);             
    lcd_show_chinese(0,0,"门已上锁，请输入密码",RED,WHITE,16,0);
    LCD_Fill(16,45,112,66,YELLOW);                      // 显示按键区域
    Tim3_Init(1000,96-1);                               // 系统时基初始化
    scheduler_init();                                   // 任务调度初始化
    audio_init();                                       // 音频模块初始化
    audio_yinliang(20);                                 // 音频播放测试
    Delay_Ms(10);                                  
    Usart2_Init();                                      // 串口2初始化
    as608_init();                                       // 指纹模块初始化
    esp8266_init();                                     // ESP8266初始化
    // AT24C02_Init();                                     // EEPROM初始化
    Delay_Ms(10);
    iotdm_init();                                       // 物联网初始化
    uart6_register_line_callback(my_mqtt_line_handler); // 注册MQTT回调
    audio_play(2);

	while(1)
    {
	    scheduler_run();                                //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
        
	}
}


void UART6_IRQHandler(void) { 
    u8 temp=0;
    if(USART_GetITStatus(UART6, USART_IT_RXNE) != RESET)
    {
        uart6_rec_tick=0;
        temp=USART_ReceiveData(UART6);
        uart6_rec_string[uart6_rec_index]=temp;
        uart6_rec_index++;
    }
    USART_ClearITPendingBit(UART6, USART_IT_RXNE);
}

void UART7_IRQHandler(void) {
    u8 temp=0;
    if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
       {
        ps_wait_flag=0;
        if(uart7_rec_tick>10)uart7_rec_index=0;
        temp=USART_ReceiveData(UART7);
        uart7_rec_string[uart7_rec_index]=temp;
        uart7_rec_index++;
        uart7_rec_tick=0;
       }
    USART_ClearITPendingBit(UART7, USART_IT_RXNE);
}

void USART2_IRQHandler(void) {
    u8 temp;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        temp=USART_ReceiveData(USART2);
        switch(rfid_index)
                {
                    case 0:
                        if(temp==0x04)rfid_index++;
                    break;
                    case 1:
                        if(temp==0x0c)rfid_index++;
                        else rfid_index=0;
                    break;
                    case 2:
                        if(temp==0x02)rfid_index++;
                        else rfid_index=0;
                    break;
                    case 3:
                        if(temp==0x30)rfid_index++;
                        else rfid_index=0;
                    break;
                    case 4:
                        if(temp==0x00)rfid_index++;
                        else rfid_index=0;
                    break;
                    case 5:
                        if(temp==0x04)rfid_index++;
                        else rfid_index=0;
                    break;
                    case 6:
                        if(temp==0x00)rfid_index++;
                        else rfid_index=0;
                    break;
                    case 7:
                        rfid_temp[0]=temp;
                        rfid_index++;
                    break;
                    case 8:
                        rfid_temp[1]=temp;
                        rfid_index++;
                    break;
                    case 9:
                        rfid_temp[2]=temp;
                        rfid_index++;
                    break;
                    case 10:
                        rfid_temp[3]=temp;
                        rfid_index=0;
                        if(mode==0)
                        {
                            if( rfid_chek() )
                            {
                                audio_play(11);
                                lock_flag=0;
                            }
                            else
                            {
                                audio_play(12);


                            }
                        }
                        if(mode==4)
                        {
                            string_copy(rfid_temp,rfid[rfid_password_index],4);
                            audio_play(15);
                            mode=0;
                            rfid_password_index++;

                        }

                    break;
                }
    }
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

void USART3_IRQHandler(void) {
    u8 temp;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        temp=USART_ReceiveData(USART3);

    }
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}



void key_clear() {
    memset(key_temp,10,6);
    key_index=0;
}



u8  string_chek(u8* string1,u8* string2,u8 len) {
    while(len--)
    {
        if(string1[len]==string2[len]);
        else return 0;
    }
    return 1;
}


void string_copy(u8* string1,u8* string2,u8 len) {
    u8 i;
    for (i = 0; i < len; ++i)
    {
      string2[i]=string1[i];
    }
}

u8 rfid_chek() {
    u8 i;
    for (i=0; i<rfid_password_index; ++i)
    {
       if( string_chek(rfid_temp,rfid[i],4) )return 1;
    }
    return 0;
}

void Ps_Wait() {
  ps_wait_flag=1;
  do
  {
   Delay_Ms(200);
  }
   while(ps_wait_flag);
}

u32 json(u8* string, u8* params, u8 len) {
    char* str = (char*)string;
    char* key = (char*)params;

    char* params_start = strstr(str, "\"params\":{");
    if (!params_start) return 0;
    params_start += 9;  


    char pattern[32];
    snprintf(pattern, sizeof(pattern), "\"%s\":", key);

    char* key_pos = strstr(params_start, pattern);
    if (!key_pos) return 0;

    char* val_start = key_pos + strlen(pattern);

    while (*val_start == ' ' || *val_start == '\t') val_start++;


    if (*val_start == 't') {
        if (strncmp(val_start, "true", 4) == 0) {
            char next_char = val_start[4];
            if (next_char == ',' || next_char == '}' ||
                next_char == ' ' || next_char == '\t') {
                return 1;
            }
        }
    }
    else if (*val_start == 'f') {
        if (strncmp(val_start, "false", 5) == 0) {
            char next_char = val_start[5];
            if (next_char == ',' || next_char == '}' ||
                next_char == ' ' || next_char == '\t') {
                return 0;
            }
        }
    }

    u32 result = 0;
    for (u8 i = 0; i < len; i++) {
        if (*val_start >= '0' && *val_start <= '9') {
            result = result * 10 + (*val_start - '0');
            val_start++;
        } else {
            break;
        }
    }

    return result;
}


void SysTick_Handler(void)
{
    sys_ms++;
}

uint32_t get_tick(void)
{
    return sys_ms;
}