#pragma once
#include "stdafx.h"

#include "Resource.h"
#define printf printf2

int __cdecl printf2(const char *format, ...)
{
	char str[1024];

	va_list argptr;
	va_start(argptr, format);
	int ret = vsnprintf(str, sizeof(str), format, argptr);
	va_end(argptr);

	OutputDebugStringA(str);

	return ret;
}

void system_error(char *str) { printf2(str); }

char portName[32];
HANDLE file;

void comOpen(void)
{
	COMMTIMEOUTS timeouts;
	DWORD read;
	DCB port;

	wchar_t port_name[32];
	size_t len;
	mbstowcs_s(&len, port_name, 32, portName, 32);

	// open the comm port.
	file = CreateFile(port_name,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == file) {
		system_error("opening file");
	}

	memset(&port, 0, sizeof(port));
	port.DCBlength = sizeof(port);
	if (!GetCommState(file, &port))        			system_error("getting comm state");
	if (!BuildCommDCB(L"baud=19200 parity=n data=8 stop=1", &port))        system_error("building comm DCB");
	if (!SetCommState(file, &port))        			system_error("adjusting port settings");

	// set short timeouts on the comm port.
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;

	if (!SetCommTimeouts(file, &timeouts))        	system_error("setting port time-outs.");

	char ch;
	do
	{
		if (BOOL ret = ReadFile(file, &ch, 1, &read, NULL)) continue;
	} while (read>0);

}

void comClose(void)
{
	CloseHandle(file);
}

void T_load(char c, char *text, int nret)
{
	DWORD nn = 0;
	DWORD written, read;

	char *p = text;
	do { if (BOOL ret = ReadFile(file, p, 1, &read, NULL)) continue; } while (read > 0);
	//
	char txt[4];
	sprintf_s(txt, 4, "?%c", c);
	WriteFile(file, txt, 2, &written, NULL); printf("%s ", txt);
	//
	nn = 0;
	do
	{
		BOOL ret = ReadFile(file, p, 1, &read, NULL); if ((*p >= ' ') && (*p < 'z')) { nn += read; p += read; }
		if (ret) continue;
	} while (*p != '\n');
	*p = 0;

	printf("%s\n", text);
}

void T_store(char c, char *txt)
{
	char text[32];
	DWORD written;
	sprintf_s(text, 32, "!%c%s", c, txt);
	if (WriteFile(file, text, strlen(text), &written, NULL))
		printf("%d %d %s\n", strlen(text), written, text);
	else
		printf("write error %d\n", GetLastError());
}

#include <setupapi.h>

int multi_has(void *buf, const char *str)
{
	const char *p;
	int len;

	len = strlen(str);
	for (p = (const char *)buf; *p; p += strlen(p) + 1) { if (_strnicmp(p, str, len) == 0) return 1; }
	return 0;
}

int findTeensy(char *name)
{
	GUID guid;
	DWORD n, size, type;
	HDEVINFO info;
	LONG r;
	SP_DEVINFO_DATA data;
	HKEY key;
	BYTE buf[1024];
	int com;

	printf("findTeensy\n");

	if (!SetupDiClassGuidsFromName(L"Ports", &guid, 1, &n) ||
		(info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT)) == INVALID_HANDLE_VALUE)
	{
		printf("Unable to get GUID for Ports class\n");
		return 0;
	}

	for (n = 0; ; n++) {
		data.cbSize = sizeof(data);
		if (!SetupDiEnumDeviceInfo(info, n, &data)) break;

		//		printf("\nDevice Instance: %ld\n", data.DevInst);
		if (!SetupDiGetDeviceRegistryProperty(info, &data,
			SPDRP_HARDWAREID, &type, buf, sizeof(buf), NULL))
		{
			continue;
		}
		//		printf("  DeviceRegistryProperty: acquired\n");
		//		printf("  type %d %d\n", type,REG_MULTI_SZ);
		size_t len;
		char text[128];
		wcstombs_s(&len, text, 128, (wchar_t *)buf, sizeof(buf));
		//		printf("%s\n", text);

		if (type != REG_MULTI_SZ) continue;
		//
		if (!(multi_has(text, "USB\\Vid_16c0&Pid_0483")	//USB_SERIAL
			|| multi_has(text, "USB\\Vid_16c0&Pid_0487")	//USB_SERIAL_HID
			|| multi_has(text, "USB\\Vid_16c0&Pid_0485")
			|| multi_has(text, "USB\\Vid_16c0&Pid_0476")	//USB_EVERYTHING
			|| multi_has(text, "USB\\Vid_16c0&Pid_048A")	//USB_MIDI_AUDIO_SERIAL
			)) continue;

		printf("Found Teensy:\n");
		key = SetupDiOpenDevRegKey(info, &data, DICS_FLAG_GLOBAL,
			0, DIREG_DEV, KEY_QUERY_VALUE);
		if (key == INVALID_HANDLE_VALUE) continue;
		//		printf_verbose("  Registry Key: acquired\n");

		size = sizeof(buf);
		r = RegQueryValueEx(key, L"Name", NULL, &type, buf, &size);
		wcstombs_s(&len, text, 128, (wchar_t *)buf, sizeof(buf));
				printf("  Name: \"%s\"\n", text);

		r = RegQueryValueEx(key, L"PortName", NULL, &type, buf, &size);
		wcstombs_s(&len, text, 128, (wchar_t *)buf, sizeof(buf));
				printf("  PortName: \"%s\"\n", text);

		RegCloseKey(key);
		if (r != ERROR_SUCCESS || type != REG_SZ) continue;

		com = 0;
		if (swscanf_s((wchar_t*)buf, L"COM%d", &com) != 1) continue;
		if (com < 1 || com > 999) continue;
		sprintf_s(name, 32, "\\\\.\\COM%d", com);  // Microsoft KB115831
		break;

	}
	//	printf("%d\n", com);
	return com;
}


