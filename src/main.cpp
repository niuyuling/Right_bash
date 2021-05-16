#include "main.h"

// ��ӡ������Ϣ
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

// ���ݴ��������������Ϣ 
void showErrorText(DWORD error_num)
{
    char* msg = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ʹ��Ĭ������
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

    // ��ע���, HKEY_CLASSES_ROOT\Directory\Background\shell
    result = RegOpenKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell", 0, // �������������� 0
        KEY_WRITE,    // ��Ȩ�ޣ�д��
        &hKey         // ��֮��ľ��
    );

    if (result != ERROR_SUCCESS) {
        printf("ע�������ʧ��\n");
        showErrorText(result);
    }
    // ���ע�����, �� HKEY_CLASSES_ROOT\Directory\Background\shell\bash
    RegCreateKey(hKey, item1, &subKey);

    // �޸�ע����Ĭ�ϵ�����
    result = RegSetValueA(subKey, NULL, REG_SZ, value2, (unsigned long)strlen(value2));

    if (result != ERROR_SUCCESS) {
        printf("ע�������ʧ��\n");
        showErrorText(result);
    }
    // ��ע���, HKEY_CLASSES_ROOT\Directory\Background\shell\bash
    result = RegOpenKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell\\bash", 0, // �������������� 0
        KEY_WRITE,    // ��Ȩ�ޣ�д��
        &hKey         // ��֮��ľ��
    );

    if (result != ERROR_SUCCESS) {
        printf("ע�������ʧ��\n");
        showErrorText(result);
    }
    // ���ע�����, �� HKEY_CLASSES_ROOT\Directory\Background\shell\bash\command
    RegCreateKey(hKey, item2, &subKey);

    // �޸�Ĭ���������
    result = RegSetValueA(subKey, NULL, REG_SZ, value1, (unsigned long)strlen(value1));

    if (result != ERROR_SUCCESS) {
        printf("ע�������ʧ��\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);          // �ر�ע���
    RegCloseKey(subKey);        // �ر�ע���
}

void del_Right()
{
    HKEY hKey;
    DWORD result;

    // ��ע���, HKEY_CLASSES_ROOT\Directory\Background\shell
    result = RegOpenKeyEx(HKEY_CLASSES_ROOT, "Directory\\Background\\shell", 0, // �������������� 0
        KEY_WRITE,    // ��Ȩ�ޣ�д��
        &hKey         // ��֮��ľ��
    );

    if (result != ERROR_SUCCESS) {
        printf("ע����ʧ��\n");
        showErrorText(result);
    }
    // ɾ�� HKEY_CLASSES_ROOT\Directory\Background\shell\bash
    result = RegDeleteTreeA(hKey, "bash");

    if (result != ERROR_SUCCESS) {
        printf("ע�������ʧ��\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);      //�ر�ע���
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

    // �����\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
    result = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, // �������������� 0
        KEY_WRITE, // ��Ȩ��,д��
        &hKey // ��֮��ľ��
    );

    if (result != ERROR_SUCCESS) {
        printf("ע����ʧ��\n");
        showErrorText(result);
    }

    result = RegSetValueEx(hKey, item, // ����
        0,   // �������������� 0
        REG_SZ, // ��ֵ����Ϊ�ַ���
        (const unsigned char*)value, // �ַ����׵�ַ
        sizeof(value) // �ַ�������
    );

    if (result != ERROR_SUCCESS) {
        printf("ע�������ʧ��\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);          // �ر�ע���
}

void del_boot_ssh(char* optarg)
{
    HKEY hKey;
    DWORD result;

    char item[CACHE_SIZE];
    char value[] = "C:\\WINDOWS\\system32\\wsl.exe -d debian -u root /usr/sbin/service ssh status";
    
    memset(item, 0, CACHE_SIZE);
    strcpy_s(item, sizeof(item), optarg);

    // �����\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
    result = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, // �������������� 0
        KEY_WRITE, // ��Ȩ�ޣ�д��
        &hKey // ��֮��ľ��
    );

    if (result != ERROR_SUCCESS) {
        printf("ע����ʧ��\n");
        showErrorText(result);
    }
    // ɾ�� �����\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run �µ� item
    result = RegDeleteValue(hKey, item);

    if (result != ERROR_SUCCESS) {
        printf("ע�������ʧ��\n");
        showErrorText(result);
    }

    RegCloseKey(hKey);          // �ر�ע���
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
