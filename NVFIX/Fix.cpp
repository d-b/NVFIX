/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#include "NVFIX.hpp"

namespace nvfix
{
    bool fixer::_api_initialized = false;

    fixer::fixer() {
        if (!_api_initialized) {
            // Attempt to initialize the API
            NvAPI_Status status = NvAPI_Initialize();
            if (status != NVAPI_OK) return;
            status = NvAPI_Private_Init();
            if (status != NVAPI_OK) {
                NvAPI_Unload(); return;
            }
            
            // Initialized successfully
            _api_initialized = true;
        }
    }

    fixer::~fixer() {};

    fixer::poll_status fixer::poll(poll_results& results) {
        // Operation status
        NvAPI_Status status = NVAPI_OK;

        // Enumerate available GPUs
        NvU32 gpu_count = 0;
        NvPhysicalGpuHandle gpu_handles[NVAPI_MAX_PHYSICAL_GPUS];
        if ((status = NvAPI_EnumPhysicalGPUs(gpu_handles, &gpu_count)) != NVAPI_OK) {
            DEBUG_PRINT("[NVFIX] ERROR: NvAPI_EnumPhysicalGPUs(...) -> %d", status);
            return POLL_Error;
        }

        for (NvU32 gpu_index = 0; gpu_index < gpu_count; gpu_index++) {
            NvPhysicalGpuHandle gpu_selected = gpu_handles[gpu_index];

            // Repair the GPU
            NV_GPU_PERF_CLIENT_POWER_POLICIES_STATUS set_power = { 0 };
            set_power.version = NV_GPU_PERF_CLIENT_POWER_POLICIES_STATUS_VER;
            set_power.flags = 1;
            set_power.table[0].power = 100 * 1000;
            if ((status = NvAPI_GPU_ClientPowerPoliciesSetStatus(gpu_selected, &set_power)) != NVAPI_OK) {
                DEBUG_PRINT("[NVFIX] ERROR: NvAPI_GPU_ClientPowerPoliciesSetStatus(...) -> %d", status);
                return POLL_Error;
            }
        }

        return POLL_Corrected;
    }
}
