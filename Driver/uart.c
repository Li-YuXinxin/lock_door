#include "debug.h"
#include <string.h>
#include <stdbool.h>
#include "uart.h"

/*
 * Uart2 —— 刷卡
 * Uart3 —— 语音播放
 * Uart6 —— 联网
 * Uart7 —— 指纹
 */
volatile uint8_t uart6_rx_buf[UART6_RX_BUF_SIZE];
volatile uint16_t uart6_rx_head = 0, uart6_rx_tail = 0;
static uart6_line_callback_t uart6_line_callback = 0;
static char uart6_line_buf[UART6_LINE_BUF_SIZE];
static int uart6_line_len = 0;

// 注册回调
void uart6_register_line_callback(uart6_line_callback_t cb) {
    uart6_line_callback = cb;
}

// 在主循环中调用，处理一行
void uart6_check_line(void) {
    while (uart6_rx_tail != uart6_rx_head) {
        char ch = uart6_rx_buf[uart6_rx_tail++];
        if (uart6_rx_tail >= UART6_RX_BUF_SIZE) uart6_rx_tail = 0;
        if (ch == '\n' || ch == '\r') {
            if (uart6_line_len > 0) {
                uart6_line_buf[uart6_line_len] = 0;
                // 判断是否+MQTTSUBRECV开头
                if (strncmp(uart6_line_buf, "+MQTTSUBRECV:", 13) == 0) {
                    if (uart6_line_callback) uart6_line_callback(uart6_line_buf);
                }
                uart6_line_len = 0;
            }
        } else {
            if (uart6_line_len < UART6_LINE_BUF_SIZE - 1) {
                uart6_line_buf[uart6_line_len++] = ch;
            }
        }
    }
}

/* 联网模块 */
void Uart6_Init() {
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART6 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;

    USART_Init(UART6, &USART_InitStructure);

    USART_ITConfig(UART6, USART_IT_RXNE,ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = UART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(UART6, ENABLE);
    USART_ClearFlag(UART6,USART_FLAG_TC);//清空串口6的发送标志位
}

void uart6_send_string(u8* string,u8 len)
{
    u8 i;
    for(i=0;i<len;i++)
    {
        USART_SendData(UART6,string[i]);
        while(  USART_GetFlagStatus(UART6, USART_FLAG_TC)==0 );
    }
}


/* 指纹模块串口初始化 */
void Uart7_Init() {
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;//2TX 3RX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;

    USART_Init(UART7, &USART_InitStructure);

    USART_ITConfig(UART7, USART_IT_RXNE,ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(UART7, ENABLE);
    USART_ClearFlag(UART7,USART_FLAG_TC);//清空串口7的发送标志位
}



void usart1_send_string(u8* string,u8 len)
{
    u8 i;
    for(i=0;i<len;i++)
    {
        USART_SendData(USART1,string[i]);
        while(  USART_GetFlagStatus(USART1, USART_FLAG_TC)==0 );
    }
}

void uart7_send_string(u8* string,u8 len)
{
    u8 i;
    for(i=0;i<len;i++)
    {
        USART_SendData(UART7,string[i]);
        while(  USART_GetFlagStatus(UART7, USART_FLAG_TC)==0 );
    }
    usart1_send_string(string,len);
}

/* 语音播放模块串口初始化 */
void Usart3_Init() {
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);

     USART_Cmd(USART3, ENABLE);
     USART_ClearFlag(USART3,USART_FLAG_TC);//清空串口3的发送标志位，防止丢掉第一个数据
}

/*  */
void Usart2_Init() {
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);

     USART_Cmd(USART2, ENABLE);
     USART_ClearFlag(USART2,USART_FLAG_TC);//清空串口2的发送标志位
}

// 等待串口应答
int wait_for_response(const char* resp, uint32_t timeout_ms)
{
    uint32_t tickstart = get_tick();
    uint16_t pos = uart6_rx_tail;
    char temp_buf[UART6_RX_BUF_SIZE+1];
    uint16_t temp_len = 0;

    while ((get_tick() - tickstart) < timeout_ms)
    {
        // 拷贝新收到的数据到临时缓冲
        while (pos != uart6_rx_head)
        {
            temp_buf[temp_len++] = uart6_rx_buf[pos];
            pos++;
            if (pos >= UART6_RX_BUF_SIZE) pos = 0;
            if (temp_len >= UART6_RX_BUF_SIZE) temp_len = 0; // 防止溢出
        }
        temp_buf[temp_len] = 0; // 字符串结尾
        if (strstr(temp_buf, resp))
        {
            uart6_rx_tail = uart6_rx_head; // 清空已读
            return true;
        }
    }
    return false;
}