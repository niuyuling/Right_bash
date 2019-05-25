#include <windows.h>
#include <stdio.h>
#include <direct.h>


#define BUFFER_SIZE 1024

void showErrorText(DWORD error_num);

// ���ݴ��������������Ϣ 
void showErrorText(DWORD error_num)
{
	char *msg = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_num,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ʹ��Ĭ������
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

	// ���ע�����
	result = RegSetValueEx(
		subKey,
		item3,                              // Name�ֶ�
		0,                                  // �������������� 0
		REG_SZ,                             // ��ֵ����Ϊ�ַ���
		(const unsigned char *)value3,      // �ַ����׵�ַ
		sizeof(value3)                      // �ַ�������
	);

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


/**
 * ���ܣ������ļ�����
 * ������
 * 		sourceFileNameWithPath��Դ�ļ�������·����
 * 		targetFileNameWithPath��Ŀ���ļ�������·����
 * ����ֵ��
 * 		SUCCESS: �����ɹ�
 * 		FAILURE������ʧ��
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

