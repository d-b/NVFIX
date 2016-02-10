/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#include "NVFIX.hpp"

// Static global variables
static SERVICE_STATUS        _service_status;
static SERVICE_STATUS_HANDLE _service_status_handle;
static HANDLE                _service_stop_event = NULL;

int nvfix::service_install() {
    SC_HANDLE handle_scmanager;
    SC_HANDLE handle_service;
    TCHAR path_service[MAX_PATH];

    if (!GetModuleFileName(NULL, path_service, MAX_PATH)) {
        printf("Cannot install service (%d)\n", GetLastError()); return -1;
    }

    // Open service control manager
    handle_scmanager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); if (!handle_scmanager) {
        printf("OpenSCManager failed (%d)\n", GetLastError()); return -1;
    }

    // Create the service
    handle_service = CreateService(
        handle_scmanager,          // SCM database
        NVFIX_SERVICE,             // name of service
        NVFIX_TITLE,               // service name to display
        SERVICE_ALL_ACCESS,        // desired access
        SERVICE_WIN32_OWN_PROCESS, // service type
        SERVICE_AUTO_START,        // start type
        SERVICE_ERROR_NORMAL,      // error control type
        path_service,              // path to service's binary
        NULL,                      // no load ordering group
        NULL,                      // no tag identifier
        NULL,                      // no dependencies
        NULL,                      // LocalSystem account
        NULL);                     // no password


    if (!handle_service) {
        printf("CreateService failed (%d)\n", GetLastError());
        CloseServiceHandle(handle_scmanager);
        return -1;
    }
    else {
        SERVICE_DESCRIPTION service_desc = { NVFIX_DESC };
        ChangeServiceConfig2(handle_service, SERVICE_CONFIG_DESCRIPTION, &service_desc);
        printf("Service installed successfully\n");
    }

    CloseServiceHandle(handle_service);
    CloseServiceHandle(handle_scmanager);
    return 0;
}

INT nvfix::service_uninstall() {
    SC_HANDLE handle_scmanager;
    SC_HANDLE handle_service;

    // Open service control manager
    handle_scmanager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); if (!handle_scmanager) {
        printf("OpenSCManager failed (%d)\n", GetLastError()); return -1;
    }

    // Open the service
    handle_service = OpenService(handle_scmanager, NVFIX_SERVICE, SERVICE_ALL_ACCESS); if (!handle_service) {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(handle_scmanager);
        return -1;
    }

    // Delete the service
    DeleteService(handle_service);
    printf("Service uninstalled successfully\n");

    CloseServiceHandle(handle_service);
    CloseServiceHandle(handle_scmanager);
    return 0;
}

VOID WINAPI nvfix::service_main(DWORD argc, LPTSTR argv[]) {
    // Register the handler function for the service
    _service_status_handle = RegisterServiceCtrlHandler(NVFIX_SERVICE, service_control_handler); if (!_service_status_handle) {
        service_report_error(L"RegisterServiceCtrlHandler"); return;
    }

    // Prepare and report service status
    _service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    _service_status.dwServiceSpecificExitCode = 0;
    service_report_status(SERVICE_START_PENDING, NO_ERROR, 3000);

    // Create service stop event
    _service_stop_event = CreateEvent(NULL, TRUE, FALSE, NULL); if (!_service_stop_event) {
        service_report_status(SERVICE_STOPPED, NO_ERROR, 0); return;
    }

    // Perform initialization
    fixer fix;
    service_report_status(SERVICE_RUNNING, NO_ERROR, 0);

    // Main loop
    for (;;) {
        // Perform a poll
        fixer::poll_results results;
        fixer::poll_status status = fix.poll(results);

        // Perform a wait on the service stop event
        if (WaitForSingleObject(_service_stop_event, NVFIX_INTERVAL) == WAIT_OBJECT_0) {
            service_report_status(SERVICE_STOPPED, NO_ERROR, 0);
            break;
        }
    }
}

VOID WINAPI nvfix::service_control_handler(DWORD control) {
    switch (control)
    {
    case SERVICE_CONTROL_STOP:
        service_report_status(SERVICE_STOP_PENDING, NO_ERROR, 0);
        SetEvent(_service_stop_event);
        service_report_status(_service_status.dwCurrentState, NO_ERROR, 0);
        return;

    case SERVICE_CONTROL_INTERROGATE:
        break;

    default:
        break;
    }
}

void nvfix::service_report_status(DWORD state, DWORD exitcode, DWORD waithint) {
    static DWORD dwCheckPoint = 1;

    _service_status.dwCurrentState = state;
    _service_status.dwWin32ExitCode = exitcode;
    _service_status.dwWaitHint = waithint;

    if (state == SERVICE_START_PENDING)
        _service_status.dwControlsAccepted = 0;
    else _service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ((state == SERVICE_RUNNING) ||
        (state == SERVICE_STOPPED))
        _service_status.dwCheckPoint = 0;
    else _service_status.dwCheckPoint = dwCheckPoint++;

    SetServiceStatus(_service_status_handle, &_service_status);
}

void nvfix::service_report_error(std::wstring function) {
    // Get the last error
    DWORD last_error = GetLastError();
    
    // Prepare the message
    std::wstringstream ss;
    ss << function << L" failed with " << last_error;
    
    // Report the event
    service_report_event(EVENTLOG_ERROR_TYPE, 0, NVFIX_ERROR, { NVFIX_SERVICE,  ss.str() });
}

void nvfix::service_report_event(WORD type, WORD category, DWORD identifier, std::vector<std::wstring> message) {
    // Register event source
    HANDLE handle_event_source = RegisterEventSource(NULL, NVFIX_SERVICE);
    if (!handle_event_source) return;

    // Prepare strings
    std::vector<LPCWSTR> strings;
    for (std::wstring& s : message)
        strings.push_back(s.c_str());

    // Report the event
    ReportEvent(handle_event_source,
        type,            // event type
        category,        // event category
        identifier,      // event identifier
        NULL,            // no security identifier
        (WORD)strings.size(),  // size of strings array
        0,               // no binary data
        strings.data(),  // array of strings
        NULL);           // no binary data
    
    // Deregister the event source
    DeregisterEventSource(handle_event_source);
}
