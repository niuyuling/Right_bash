#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include "getopt.h"

// 根据错误码输出错误信息 
void showErrorText(DWORD error_num)
{
	char* msg = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_num,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 使用默认语言
		(LPSTR)& msg,
		0,
		NULL
	);

	printf("Error code %d: ", error_num);
	if (msg == NULL)
		printf("%sn", "Unknown error");
	else
		printf("%sn", msg);
}

void add()
{
	HKEY hKey;
	HKEY subKey;
	DWORD result;

	char item1[] = "bash";
	char item2[] = "command";

	char value1[] = "C:\\Windows\\System32\\bash.exe";
	char value2[] = "Open Bash in this location";

	// 打开注册表, HKEY_CLASSES_ROOT\Directory\Background\shell
	result = RegOpenKeyEx(
		HKEY_CLASSES_ROOT, "Directory\\Background\\shell",
		0,                                  // 保留参数必须填 0
		KEY_WRITE,                          // 打开权限，写入
		&hKey                               // 打开之后的句柄
	);

	// 添加注册表项, 即 HKEY_CLASSES_ROOT\Directory\Background\shell\bash
	RegCreateKey(hKey, item1, &subKey);

	// 修改注册表项，默认的数据
	result = RegSetValueA(subKey, NULL, REG_SZ, value2, (unsigned long)strlen(value2));

	// 打开注册表, HKEY_CLASSES_ROOT\Directory\Background\shell\bash
	result = RegOpenKeyEx(
		HKEY_CLASSES_ROOT, "Directory\\Background\\shell\\bash",
		0,                                  // 保留参数必须填 0
		KEY_WRITE,                          // 打开权限，写入
		&hKey                               // 打开之后的句柄
	);

	// 添加注册表项, 即 HKEY_CLASSES_ROOT\Directory\Background\shell\bash\command
	RegCreateKey(hKey, item2, &subKey);

	// 修改默认项的数据
	result = RegSetValueA(subKey, NULL, REG_SZ, value1, (unsigned long)strlen(value1));

	RegCloseKey(hKey);                      //关闭注册表
	RegCloseKey(subKey);                    //关闭注册表
}

void del()
{
	HKEY hKey;
	DWORD result;

	// 打开注册表, HKEY_CLASSES_ROOT\Directory\Background\shell
	result = RegOpenKeyEx(
		HKEY_CLASSES_ROOT, "Directory\\Background\\shell",
		0,                                  // 保留参数必须填 0
		KEY_WRITE,                          // 打开权限，写入
		&hKey                               // 打开之后的句柄
	);

	// 删除 HKEY_CLASSES_ROOT\Directory\Background\shell\bash
	result = RegDeleteTreeA(hKey, "bash");

	RegCloseKey(hKey);                      //关闭注册表
}

void Help(char** argv) {
	static const char* help_s[] = {
		"",
		"Options:",
		"    -h,?    print help",
		"    -a      add Registry",
		"    -d      delete Registry",
		"",
		0
	};
	static const char name[] = "Right_bash";
	static const char usage[] =
		"Usage: [-?had]";
	static const char author[] = "Author: aixiao@aixiao.me";

	printf("%s\n", name);
	printf("%s\n", author);
	printf("%s\n", usage);

	for (int i = 0; help_s[i]; i++)
		fprintf(stderr, "%s\n", help_s[i]);


}

int main(int argc, char* argv[])
{
	int c;
	while ((c = getopt(argc, argv, "adh?")) != -1) {
		switch (c) {
		case 'a':
			add();
			break;
		case 'd':
			del();
			break;
		case 'h':
			Help(argv);
			break;
		case '?':
			Help(argv);
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

