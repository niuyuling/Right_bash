#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include "getopt.h"

// ���ݴ��������������Ϣ 
void showErrorText(DWORD error_num)
{
	char* msg = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_num,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ʹ��Ĭ������
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

	// ��ע���, HKEY_CLASSES_ROOT\Directory\Background\shell
	result = RegOpenKeyEx(
		HKEY_CLASSES_ROOT, "Directory\\Background\\shell",
		0,                                  // �������������� 0
		KEY_WRITE,                          // ��Ȩ�ޣ�д��
		&hKey                               // ��֮��ľ��
	);

	// ���ע�����, �� HKEY_CLASSES_ROOT\Directory\Background\shell\bash
	RegCreateKey(hKey, item1, &subKey);

	// �޸�ע����Ĭ�ϵ�����
	result = RegSetValueA(subKey, NULL, REG_SZ, value2, (unsigned long)strlen(value2));

	// ��ע���, HKEY_CLASSES_ROOT\Directory\Background\shell\bash
	result = RegOpenKeyEx(
		HKEY_CLASSES_ROOT, "Directory\\Background\\shell\\bash",
		0,                                  // �������������� 0
		KEY_WRITE,                          // ��Ȩ�ޣ�д��
		&hKey                               // ��֮��ľ��
	);

	// ���ע�����, �� HKEY_CLASSES_ROOT\Directory\Background\shell\bash\command
	RegCreateKey(hKey, item2, &subKey);

	// �޸�Ĭ���������
	result = RegSetValueA(subKey, NULL, REG_SZ, value1, (unsigned long)strlen(value1));

	RegCloseKey(hKey);                      //�ر�ע���
	RegCloseKey(subKey);                    //�ر�ע���
}

void del()
{
	HKEY hKey;
	DWORD result;

	// ��ע���, HKEY_CLASSES_ROOT\Directory\Background\shell
	result = RegOpenKeyEx(
		HKEY_CLASSES_ROOT, "Directory\\Background\\shell",
		0,                                  // �������������� 0
		KEY_WRITE,                          // ��Ȩ�ޣ�д��
		&hKey                               // ��֮��ľ��
	);

	// ɾ�� HKEY_CLASSES_ROOT\Directory\Background\shell\bash
	result = RegDeleteTreeA(hKey, "bash");

	RegCloseKey(hKey);                      //�ر�ע���
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

