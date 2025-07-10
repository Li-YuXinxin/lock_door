void set_volume(int volume);                // 设置音量
void set_lock(int isLocked);                // 1=上锁，0=开锁
void change_password(const char* new_pwd);  // 修改密码
void generate_temp_password(int minutes);   // 生成临时密码，有效分钟数