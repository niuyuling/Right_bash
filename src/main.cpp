#include "main.h"

// 打印帮助信息
void help(char** argv)
{
    static const char* help_s[] = {
        "",
        "Options:",
        "    -h,?    print help",
        "    -a      add Registry",
        "    -d      delete Registry",
        "    -b      Start SSH on boot",
        "    -p      Turn off and start SSH",
        "",
        0
    };
    static const char name[] = "Right_bash 1.2";
    static const char usage[] = "Usage: [-?hadbp]";
    static const char author[] = "Author: aixiao@aixiao.me";

    printf("%s\n", name);
    printf("%s\n", author);
    printf("%s\n", usage);

    for (int i = 0; help_s[i]; i++)
        fprintf(stderr, "%s\n", help_s[i]);
}

// 根据错误码输出错误信息 
void showErrorText(DWORD error_num)
{
    char* msg = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 使用默认语言
        (LPSTR)&msg, 0, NULL);

    printf("Error code %d: ", error_num);
    if (msg == NULL)
        printf("%sn", "Unknown error");
    else
        printf("%sn", msg);
}

void add_Right()
{
    HKEY hKey;
    HKEY subKey;
    DWORD result;

    char item1[] = "bash";
    char value1[] = "C:\\Windows\\System32\\bash.exe";
    char item2[] = "command";
    char value2[] = "Open Bash in this location";

    // 打开注册表, HKEY_CLASSES_ROOT\Directory\Background\shell
    result = RegOpenKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell", 0, // 保留参数必须填 0
        KEY_WRITE,    // 打开权限，写入
        &hKey         // 打开之后的句柄
    );

    if (result != ERROR_SUCCESS) {
        printf("注册表设置失败\n");
        showErrorText(result);
    }
    // 添加注册表项, 即 HKEY_CLASSES_ROOT\Directory\Background\shell\bash
    RegCreateKey(hKey, item1, &subKey);

    // 修改注册表项，默认的数据
    result = RegSetValueA(subKey, NULL, REG_SZ, value2, (unsigned long)strlen(value2));

    if (result != ERROR_SUCCESS) {
        printf("注册表设置失败\n");
        showErrorText(result);
    }
    // 打开注册表, HKEY_CLASSES_ROOT\Directory\Background\shell\bash
    result = RegOpenKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell\\bash", 0, // 保留参数必须填 0
        KEY_WRITE,    // 打开权限，写入
        &hKey         // 打开之后的句柄
    );

    if (result != ERROR_SUCCESS) {
        printf("注册表设置失败\n");
        showErrorText(result);
    }
    // 添加注册表项, 即 HKEY_CLASSES_ROOT\Directory\Background\shell\bash\command
    RegCreateKey(hKey, item2, &subKey);

    // 修改默认项的数据
    result = RegSetValueA(subKey, NULL, REG_SZ, value1, (unsigned long)strlen(value1));

    if (result != ERROR_SUCCESS) {
        printf("注册表设置失败\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);          // 关闭注册表
    RegCloseKey(subKey);        // 关闭注册表
}

void del_Right()
{
    HKEY hKey;
    DWORD result;

    // 打开注册表, HKEY_CLASSES_ROOT\Directory\Background\shell
    result = RegOpenKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell", 0, // 保留参数必须填 0
        KEY_WRITE,    // 打开权限，写入
        &hKey         // 打开之后的句柄
    );

    if (result != ERROR_SUCCESS) {
        printf("注册表打开失败\n");
        showErrorText(result);
    }
    // 删除 HKEY_CLASSES_ROOT\Directory\Background\shell\bash
    result = RegDeleteTreeA(hKey, "bash");

    if (result != ERROR_SUCCESS) {
        printf("注册表设置失败\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);      //关闭注册表
}

void add_boot_ssh(char* optarg)
{
    HKEY hKey;
    DWORD result;
    //char value[] = "C:\\WINDOWS\\system32\\wsl.exe -d debian -u root /usr/sbin/service ssh start";

    char item[CACHE_SIZE];
    char value[CACHE_SIZE];
    memset(item, 0, CACHE_SIZE);
    memset(value, 0, CACHE_SIZE);
    strcpy_s(item, sizeof(item), optarg);
    sprintf_s(value, "%s", "C:\\WINDOWS\\system32\\wsl.exe -d ");
    strcat_s(value, strlen(value) + strlen(optarg) + 1, optarg);
    strcat_s(value, strlen(value) + 37, " -u root /usr/sbin/service ssh start");

    // 计算机\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
    result = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, // 保留参数必须填 0
        KEY_WRITE, // 打开权限,写入
        &hKey // 打开之后的句柄
    );

    if (result != ERROR_SUCCESS) {
        printf("注册表打开失败\n");
        showErrorText(result);
    }

    result = RegSetValueEx(hKey, item, // 键名
        0,   // 保留参数必须填 0
        REG_SZ, // 键值类型为字符串
        (const unsigned char*)value, // 字符串首地址
        sizeof(value) // 字符串长度
    );

    if (result != ERROR_SUCCESS) {
        printf("注册表设置失败\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);          // 关闭注册表
}

void del_boot_ssh(char* optarg)
{
    HKEY hKey;
    DWORD result;

    char item[CACHE_SIZE];
    char value[] = "C:\\WINDOWS\\system32\\wsl.exe -d debian -u root /usr/sbin/service ssh status";
    
    memset(item, 0, CACHE_SIZE);
    strcpy_s(item, sizeof(item), optarg);

    // 计算机\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
    result = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, // 保留参数必须填 0
        KEY_WRITE, // 打开权限，写入
        &hKey // 打开之后的句柄
    );

    if (result != ERROR_SUCCESS) {
        printf("注册表打开失败\n");
        showErrorText(result);
    }
    // 删除 计算机\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run 下的 item
    result = RegDeleteValue(hKey, item);

    if (result != ERROR_SUCCESS) {
        printf("注册表设置失败\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);          // 关闭注册表
}

int main(int argc, char* argv[])
{
    int c;
    while ((c = getopt(argc, argv, "adb:p:h?")) != -1) {
        switch (c) {
        case 'a':
            add_Right();
            break;
        case 'd':
            del_Right();
            break;
        case 'b':
            add_boot_ssh(optarg);
            break;
        case 'p':
            del_boot_ssh(optarg);
            break;
        case 'h':
            help(argv);
            break;
        case '?':
            help(argv);
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
