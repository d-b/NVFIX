/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#include "NVFIX.hpp"

int wmain(int argc, wchar_t* argv[]) {
    // See if we need to install the service
    if (argc > 1)
        for (int i = 1; i < argc; i++)
            if (_wcsicmp(argv[i], L"install") == 0)
                return nvfix::service_install();
            else if (_wcsicmp(argv[i], L"uninstall") == 0)
                return nvfix::service_uninstall();

    // Setup the service dispatch table
    SERVICE_TABLE_ENTRY dispatch_table[] = {
        { NVFIX_SERVICE, (LPSERVICE_MAIN_FUNCTION)nvfix::service_main },
        { NULL, NULL }
    };

    // Start the service
    if (!StartServiceCtrlDispatcher(dispatch_table))
        nvfix::service_report_error(L"StartServiceCtrlDispatcher");

    // Print usage and return    
    std::wcout << L"Usage: " << PathFindFileName(argv[0]) << L" <command: install/uninstall>" << std::endl;
    return 0;
}
