#include <windows.h>
#include <stdio.h>
#include <direct.h>


#define BUFFER_SIZE 1024

void showErrorText(DWORD error_num);

// 根据错误码输出错误信息 
void showErrorText(DWORD error_num)
{
	char *msg = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_num,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 使用默认语言
		(LPSTR)&msg,
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
	char item3[] = "icon";

	char value1[] = "C:\\Windows\\System32\\bash.exe";
	char value2[] = "Open Bash in this location";
	char value3[] = "C:\\Users\\niuyuling\\AppData\\Local\\Microsoft\\WindowsApps\\debian.ico";

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

	// 添加注册表项
	result = RegSetValueEx(
		subKey,
		item3,                              // Name字段
		0,                                  // 保留参数必须填 0
		REG_SZ,                             // 键值类型为字符串
		(const unsigned char *)value3,      // 字符串首地址
		sizeof(value3)                      // 字符串长度
	);

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


/**
 * 功能：拷贝文件函数
 * 参数：
 * 		sourceFileNameWithPath：源文件名（带路径）
 * 		targetFileNameWithPath：目标文件名（带路径）
 * 返回值：
 * 		SUCCESS: 拷贝成功
 * 		FAILURE：拷贝失败
 * author:wangchangshuai jlu
 */
int copyFile(const char* sourceFileNameWithPath, const char* targetFileNameWithPath)
{
	FILE *fin, *fout;
	size_t c;
	char buf[1024];

	fopen_s(&fin, sourceFileNameWithPath, "rb");
	fopen_s(&fout, targetFileNameWithPath, "wb");

	while ((c = fread(buf, sizeof(unsigned char), 1024, fin)) != 0)
	{
		fwrite(buf, sizeof(unsigned char), c, fout);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}

int main(int argc, char *argv[])
{;
	if (argc == 2) {
		if (strcmp(argv[1], "add") == 0) {
			add();
			//copyFile("debian.ico", "%userprofile%\\AppData\\Local\\Microsoft\\WindowsApps\\debian.ico");

			system("copy debian.ico %userprofile%\\AppData\\Local\\Microsoft\\WindowsApps\\debian.ico > null");
			//system("taskkill /f /im explorer.exe > null");
			//system("DEL  %userprofile%\\AppData\\Local\\IconCache.db /a > null");
			//system("start explorer.exe > null");
		}
		else if (strcmp(argv[1], "del") == 0) {
			del();
			system("DEL %userprofile%\\AppData\\Local\\Microsoft\\WindowsApps\\debian.ico 2> null");
		}
	}
	else {
		printf("%s add | del\n", argv[0]);
	}

	return 0;
}

