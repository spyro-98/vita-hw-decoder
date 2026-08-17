#ifndef VITA_HW_DECODER_H
#define VITA_HW_DECODER_H

#include <stddef.h>
#include <stdint.h>

/* A factory must create a new independent cursor every time open() is called.
 * The player opens two cursors for a muxed file so audio and video can demux
 * concurrently without sharing seek state. */
typedef struct VitaHwDecoderStreamHandle {
	void *opaque;
	int (*read)(void *opaque, void *buffer, size_t size);
	int64_t (*seek)(void *opaque, int64_t offset, int whence);
	void (*close)(void *opaque);
	int64_t size;
} VitaHwDecoderStreamHandle;

typedef struct VitaHwDecoderStreamFactory {
	void *opaque;
	int (*open)(void *opaque, VitaHwDecoderStreamHandle *out);
} VitaHwDecoderStreamFactory;

typedef struct VitaHwDecoderPlayer VitaHwDecoderPlayer;

typedef struct VitaHwDecoderPlayerConfig {
	VitaHwDecoderStreamFactory stream;
	uint32_t expected_width;
	uint32_t expected_height;
	int expected_fps;
	uint64_t start_position_ms;
	int volume_percent;
	/* Optional cooperative cancellation flag used during remote opens and
	 * decode. The caller must keep it alive until close returns. */
	volatile int *cancel_flag;
} VitaHwDecoderPlayerConfig;

typedef struct VitaHwDecoderPlayerStatus {
	int opened;
	int paused;
	int eof;
	int error;
	int hardware_accelerated;
	int direct_rendering;
	int ready_frames;
	int frame_capacity;
	int fps;
	uint32_t width;
	uint32_t height;
	uint64_t position_ms;
	uint64_t duration_ms;
	unsigned int frames_decoded;
	unsigned int frames_shown;
	unsigned int frames_dropped;
} VitaHwDecoderPlayerStatus;

/* Convenience factory for a normal Vita path. The path must remain valid
 * until the player is closed. */
void vita_hw_decoder_file_stream_factory(const char *path,
	                             VitaHwDecoderStreamFactory *factory);

/* Loads the packaged decoder compatibility runtime once for the process.
 * Normal player open calls prepare it automatically; embedding applications
 * may call this before creating a separate SceAvPlayer instance. */
int vita_hw_decoder_prepare_runtime(void);
const char *vita_hw_decoder_backend_name(void);

VitaHwDecoderPlayer *vita_hw_decoder_create(void);
int vita_hw_decoder_open(VitaHwDecoderPlayer *player,
	                    const VitaHwDecoderPlayerConfig *config);
void vita_hw_decoder_close(VitaHwDecoderPlayer *player);
void vita_hw_decoder_destroy(VitaHwDecoderPlayer *player);

void vita_hw_decoder_set_paused(VitaHwDecoderPlayer *player, int paused);
void vita_hw_decoder_set_volume(VitaHwDecoderPlayer *player, int percent);
void vita_hw_decoder_request_stop(VitaHwDecoderPlayer *player);

/* Reopens both independent cursors at the requested media timestamp. */
int vita_hw_decoder_seek(VitaHwDecoderPlayer *player, uint64_t position_ms);

/* Call present() inside a vita2d drawing scene, then render_complete() after
 * vita2d_wait_rendering_done(). */
int vita_hw_decoder_present(VitaHwDecoderPlayer *player, int fill_screen);
int vita_hw_decoder_present_rect(VitaHwDecoderPlayer *player,
	                            float x, float y, float width, float height,
	                            int fill_rect);
void vita_hw_decoder_render_complete(VitaHwDecoderPlayer *player);
void vita_hw_decoder_get_status(VitaHwDecoderPlayer *player,
	                           VitaHwDecoderPlayerStatus *status);

#endif
