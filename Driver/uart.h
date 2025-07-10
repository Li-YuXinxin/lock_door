#include "debug.h"

#define UART6_RX_BUF_SIZE 256
extern uint32_t get_tick(void);
#define UART6_LINE_BUF_SIZE 256
extern volatile uint8_t uart6_rx_buf[UART6_RX_BUF_SIZE];
extern volatile uint16_t uart6_rx_head, uart6_rx_tail;


// 回调函数类型
typedef void (*uart6_line_callback_t)(const char* line);
// static uart6_line_callback_t uart6_line_callback = 0;


void uart6_register_line_callback(uart6_line_callback_t cb);
void uart6_check_line(void);

void Usart3_Init();//发送语音模块
void Usart2_Init();//接收刷卡模块
void Uart7_Init();
void uart7_send_string(u8* string,u8 len);
void usart1_send_string(u8* string,u8 len);
void Uart6_Init();
void uart6_send_string(u8* string,u8 len);
int wait_for_response(const char* resp, uint32_t timeout_ms);
