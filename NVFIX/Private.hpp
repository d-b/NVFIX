/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
**
** NVAPI PRIVATE/UNDOCUMENTED INTERFACE
*/

#pragma once

typedef struct
{
    NvU32 version;
    NvU32 flags;

    struct {
        NvU32 unknown1;
        NvU32 unknown2;
        NvU32 power;
        NvU32 unknown3;
    } table[4];
} NV_GPU_PERF_CLIENT_POWER_POLICIES_STATUS;

#define NV_GPU_PERF_CLIENT_POWER_POLICIES_STATUS_VER MAKE_NVAPI_VERSION(NV_GPU_PERF_CLIENT_POWER_POLICIES_STATUS,1)

NvAPI_Status NvAPI_Private_Init();
NvAPI_Status NvAPI_Private_Unload();
NvAPI_Status NvAPI_GPU_ClientPowerPoliciesGetStatus(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_PERF_CLIENT_POWER_POLICIES_STATUS* pPowerPoliciesStatus);
NvAPI_Status NvAPI_GPU_ClientPowerPoliciesSetStatus(NvPhysicalGpuHandle hPhysicalGpu, NV_GPU_PERF_CLIENT_POWER_POLICIES_STATUS* pPowerPoliciesStatus);
