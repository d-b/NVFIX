/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#pragma once

namespace nvfix
{
    // Service entry point & installation
    INT service_install();
    INT service_uninstall();
    VOID WINAPI service_main(DWORD argc, LPTSTR argv[]);
    VOID WINAPI service_control_handler(DWORD control);
    
    // Service routines
    void service_report_status(DWORD state, DWORD exitcode, DWORD waithint);
    void service_report_error(std::wstring function);
    void service_report_event(WORD type, WORD category, DWORD identifier, std::vector<std::wstring> message);
}