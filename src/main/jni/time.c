#include "gif.h"

int calculateInvalidationDelay(GifInfo *info, time_t renderStartTime) {
    if (info->gifFilePtr->ImageCount > 1 && (info->loopCount == 0 || info->currentLoop < info->loopCount)) {
        uint_fast16_t scaledDuration = info->infos[info->currentIndex].DelayTime;
        if (info->speedFactor != 1.0) {
            scaledDuration /= info->speedFactor;
            if (scaledDuration <= 0)
                scaledDuration = 1;
            else if (scaledDuration > UINT16_MAX)
                scaledDuration = UINT16_MAX;
        }
        int invalidationDelay = (int) (scaledDuration - (getRealTime() - renderStartTime));
        if (invalidationDelay < 0)
            invalidationDelay = 0;
        info->nextStartTime = renderStartTime + invalidationDelay;
        return invalidationDelay;
    }
    info->lastFrameRemainder = 0;
    return -1;
}

inline time_t getRealTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts); //result not checked since CLOCK_MONOTONIC_RAW availability is checked in JNI_ONLoad
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}