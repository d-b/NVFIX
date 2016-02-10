/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#pragma once

namespace nvfix
{
    class fixer {
        static bool _api_initialized;

    public:
        enum utilization_type {
            UTIL_Gpu,
            UTIL_Memory,
            UTIL_Encoder,
            UTIL_Decoder,
            UTIL_MAX
        };

        enum clock_type {
            CLOCK_Graphics,
            CLOCK_SM,
            CLOCK_Memory,
            CLOCK_Video,
            CLOCK_MAX
        };

        enum poll_status {
            POLL_Okay,
            POLL_Error,
            POLL_Corrected
        };

        typedef struct {
            unsigned int gpu;
            unsigned int power;
            unsigned int utilization[UTIL_MAX];
            unsigned int clocks[CLOCK_MAX];
        } poll_results;

        fixer(); virtual ~fixer();
        poll_status poll(poll_results& results);
    };
}
