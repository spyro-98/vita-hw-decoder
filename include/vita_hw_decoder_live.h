#ifndef VITA_HW_DECODER_LIVE_H
#define VITA_HW_DECODER_LIVE_H
#include "vita_hw_decoder.h"

/* Additive, opt-in ABI. Existing config/stream/status layouts and open()
 * behavior are unchanged. V1 accepts forward-only multiplexed MPEG-TS,
 * H.264 with mono/stereo AAC. Transport owns buffering and reconnection.
 * Every cursor must start at the SAME retained segment/timeline.
 * read blocks cooperatively until bytes, final EOF or an error; temporary
 * starvation must not return zero. abort must wake a blocked read promptly.
 * Discontinuity/eviction requires closing BOTH pipelines and reopening.
 * Seeking and audio track changes return ENOSYS without mutating playback. */
typedef struct VitaHwDecoderLiveOptions {
    uint32_t size;
    uint32_t version; /* 1 */
    uint32_t reserved[2]; /* must be zero */
} VitaHwDecoderLiveOptions;

int vita_hw_decoder_open_live(VitaHwDecoderPlayer *player,
    const VitaHwDecoderPlayerConfig *config,
    const VitaHwDecoderLiveOptions *options);
#endif
