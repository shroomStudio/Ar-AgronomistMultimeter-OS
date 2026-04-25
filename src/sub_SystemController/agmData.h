// ShroomCorp AgM OS-2.0 — agmData.h
// Shared pipeline data struct flowing through the FSM stages.

#ifndef AGM_DATA_H
#define AGM_DATA_H

#include <stdint.h>
#include <sub_SensingManagement/ShroomCorp_AS7265x.h>

struct AgMData {
    uint32_t accumulator[AS7265X_NUM_CHANNELS]; // running sum during multi-read  (72 B)
    uint16_t calibRef[AS7265X_NUM_CHANNELS];    // white-reference average        (36 B)
    uint16_t sampleAvg[AS7265X_NUM_CHANNELS];   // sample average                 (36 B)

    // Feature indices — computed in STATE_INFER, not stored per-channel
    // R(λ) = sampleAvg[i] / calibRef[i] * 100  (computed on-the-fly to save RAM)
    float nIndex;   // R(460) / R(645)
    float pIndex;   // R(460) / R(535)
    float kIndex;   // R(810) / R(645)
    float darkness; // R(410)

    uint8_t readCount; // reads accumulated in current block
};

#endif // AGM_DATA_H
