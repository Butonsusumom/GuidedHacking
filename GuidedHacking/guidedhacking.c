
#pragma warning (disable : 4100)

#include "guidedhacking.h"

void appStatus(HANDLE hProcess, WCHAR* appName, WCHAR* status) {
	PEPROCESS pProcess;
	PUNICODE_STRING procName;

	PsLookupProcessByProcessId(hProcess, &pProcess);
	SeLocateProcessImageName(pProcess, &procName);

	WCHAR* convertName = ((WCHAR*)procName) + 4;

	if (wcsstr(convertName, appName)) {
		DbgPrintEx(0, 0, "process %ls is %ls", appName, status);
	}
}

void onProcessEvent(HANDLE parentPid, HANDLE processPid, BOOLEAN isCreation) {
	WCHAR* appName = (WCHAR*)L"Calculator.exe";

	if (isCreation) {
		appStatus(processPid, appName, L"started");
	}
	else appStatus(processPid, appName, L"ended");
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	pDriverObject->DriverUnload = UnloadDriver;
	DbgPrintEx(0, 0, "DRIVER_ENTRY");
	PsSetCreateProcessNotifyRoutine(onProcessEvent, FALSE);
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) {
	DbgPrintEx(0, 0, "UNLOAD_DRIVER");
	PsSetCreateProcessNotifyRoutine(onProcessEvent, TRUE);
	return STATUS_SUCCESS;
}