void esp8266_init();
void iotdm_init();

// 参数提取函数声明
int extract_request_id(const char* line, char* out, int outlen);
int extract_command_name(const char* line, char* out, int outlen);
int extract_param_bool(const char* line, const char* param, int* value);
int extract_param_int(const char* line, const char* param, int* value);
int extract_param_string(const char* line, const char* param, char* out, int outlen);

// MQTT命令回调
void my_mqtt_line_handler(const char* line);