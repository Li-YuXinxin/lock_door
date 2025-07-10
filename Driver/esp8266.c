#include "debug.h"
#include "uart.h"
#include "esp8266.h"
#include <stdbool.h>
#include "string.h"
#include "telecontrol.h"

extern int wait_for_response(const char* resp, uint32_t timeout_ms);

u8 string1[]="AT+RST\r\n";
u8 string2[]="AT+CWMODE=1\r\n";
u8 string3[]="AT+CWDHCP=1,1\r\n";
u8 string4[]="AT+CWJAP=\"lyx\",\"lyxlyxlyx\"\r\n";
u8 string5[]="AT+MQTTUSERCFG=0,1,\"lock_250627_0_0_2025062712\",\"lock_250627\",\"7cb55e8dd2bb613524b6245c7752cfd15bffca8701e81967937738796a9b8c56\",0,0,\"\"\r\n";
u8 string6[]="AT+MQTTCONN=0,\"c6fee624a6.st1.iotda-device.cn-north-4.myhuaweicloud.com\",1883,0\r\n";
u8 string7[]="AT+MQTTSUB=0,\"$oc/devices/lock_250627/sys/commands/#\",1\r\n";   // 订阅
u8 string8[]="AT+MQTTSUB=0,\"$oc/devices/lock_250627/sys/properties/report\",1\r\n";   // 订阅
u8 string9[] = "AT+MQTTPUB=0,\"$oc/devices/lock_250627/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"Lock\\\"\\,\\\"properties\\\":{\\\"isLocked\\\":true\\,\\\"volume\\\":18\\,\\\"password\\\":\\\"123456\\\"}}]}\",1,0\r\n";    // 发布

void esp8266_init()
{
    Uart6_Init();
}


void iotdm_init()
{
    uart6_send_string(string1,sizeof(string1)-1);
    Delay_Ms(1000);
    uart6_send_string(string2,sizeof(string2)-1);
    Delay_Ms(1000);
    uart6_send_string(string3,sizeof(string3)-1);
    Delay_Ms(1000);
    uart6_send_string(string4,sizeof(string4)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string5,sizeof(string5)-1);
    // Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string5,sizeof(string5)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string6,sizeof(string6)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string7,sizeof(string7)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string8,sizeof(string8)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
    uart6_send_string(string9,sizeof(string9)-1);
    Delay_Ms(1000);
    Delay_Ms(1000);
}


// 提取 request_id
int extract_request_id(const char* line, char* out, int outlen) {
    const char* p1 = strchr(line, '"');
    if (!p1) return 0;
    p1++;
    const char* p2 = strchr(p1, '"');
    if (!p2) return 0;
    const char* key = "request_id=";
    const char* reqid = strstr(p1, key);
    if (!reqid || reqid > p2) return 0;
    reqid += strlen(key);
    int i = 0;
    while (reqid < p2 && *reqid && *reqid != ',' && i < outlen-1) {
        out[i++] = *reqid++;
    }
    out[i] = 0;
    return i > 0 ? 1 : 0;
}

// 提取 command_name
int extract_command_name(const char* line, char* out, int outlen) {
    const char* key = "\"command_name\":";
    const char* pos = strstr(line, key);
    if (!pos) return 0;
    pos += strlen(key);
    while (*pos == ' ' || *pos == '"') pos++;
    int i = 0;
    while (*pos && *pos != '"' && *pos != ',' && *pos != '}' && i < outlen-1) {
        out[i++] = *pos++;
    }
    out[i] = 0;
    return i > 0 ? 1 : 0;
}

// 提取布尔型参数
int extract_param_bool(const char* line, const char* param, int* value) {
    char pattern[32];
    snprintf(pattern, sizeof(pattern), "\"%s\":", param);
    const char* pos = strstr(line, pattern);
    if (!pos) return 0;
    pos += strlen(pattern);
    while (*pos == ' ' || *pos == '"') pos++;
    if (strncmp(pos, "true", 4) == 0) { *value = 1; return 1; }
    if (strncmp(pos, "false", 5) == 0) { *value = 0; return 1; }
    return 0;
}

// 提取整型参数
int extract_param_int(const char* line, const char* param, int* value) {
    char pattern[32];
    snprintf(pattern, sizeof(pattern), "\"%s\":", param);
    const char* pos = strstr(line, pattern);
    if (!pos) return 0;
    pos += strlen(pattern);
    while (*pos == ' ' || *pos == '"') pos++;
    int v = 0;
    int n = sscanf(pos, "%d", &v);
    if (n == 1) { *value = v; return 1; }
    return 0;
}

// 提取字符串参数
int extract_param_string(const char* line, const char* param, char* out, int outlen) {
    char pattern[32];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", param);
    const char* pos = strstr(line, pattern);
    if (!pos) return 0;
    pos += strlen(pattern);
    int i = 0;
    while (*pos && *pos != '"' && i < outlen-1) {
        out[i++] = *pos++;
    }
    out[i] = 0;
    return i > 0 ? 1 : 0;
}

// 修改回调函数调用逻辑
void my_mqtt_line_handler(const char* line) {
    char request_id[64] = {0};
    char command_name[32] = {0};
    int ret_code = 1; // 1为失败

    // 1. 提取 request_id
    if (!extract_request_id(line, request_id, sizeof(request_id))) {
        printf("request_id not found!\r\n");
        return;
    }

    // 2. 提取 command_name
    if (!extract_command_name(line, command_name, sizeof(command_name))) {
        printf("command_name not found!\r\n");
        return;
    }

    // 3. 解析参数并执行动
    if (strcmp(command_name, "SetVolume") == 0) {
        int volume = 0;
        if (extract_param_int(line, "volume", &volume)) {
            set_volume(volume);
            ret_code = 0;
        }
    } else if (strcmp(command_name, "SetLock") == 0) {
        int isLocked = 0;
        if (extract_param_bool(line, "isLocked", &isLocked)) {
            set_lock(isLocked);
            ret_code = 0;
        }
    } else if (strcmp(command_name, "ChangePassword") == 0) {
        char new_pwd[32] = {0};
        if (extract_param_string(line, "password", new_pwd, sizeof(new_pwd))) {
            change_password(new_pwd);
            ret_code = 0;
        }
    // } else if (strcmp(command_name, "GenerateTempPassword") == 0) {
    //     int minutes = 0;
    //     if (extract_param_int(line, "minutes", &minutes)) {
    //         generate_temp_password(minutes);
    //         ret_code = 0;
    //     }
    }

    // 4. 回复云端
    char at_cmd[256];
    snprintf(at_cmd, sizeof(at_cmd),
        "AT+MQTTPUB=0,\"$oc/devices/lock_250627/sys/commands/response/request_id=%s\",\"{\\\"result_code\\\":%d,\\\"response_name\\\":\\\"%s\\\",\\\"paras\\\":{\\\"result\\\":\\\"%s\\\"}}\",1,0\r\n",
        request_id, ret_code, command_name, ret_code == 0 ? "success" : "fail");
    uart6_send_string((u8*)at_cmd, strlen(at_cmd));
    wait_for_response("OK", 2000);
}
