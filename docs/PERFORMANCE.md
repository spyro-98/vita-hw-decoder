# vita-hw-decoder: performance and evidence

Source baseline is ABI 2.0.0; app integration 1.4.1. No new runtime algorithm,
codec, clock default or battery measurement is introduced by this doc refresh.

## Implemented in the decoder baseline

- Kernel event notification for frame/surface availability, with bounded waits
  and stop handling; readiness is rechecked around notification.
- Copy only needed YUV rows and deterministic padding, rather than clear every
  complete surface on every frame. Preserve crop/pitch and GXM plane layout.
- MP4 duration fallback from finite A/V stream timing and indexed seek/preroll.
- Diagnostics separate decode, copy, queue/surface wait, startup and seek work;
  dropped counters include preroll and are not alone a playback loss metric.

## Hardware path

Public h264_vita/SceVideodec, direct NV12 sampled as YVU420P2; no silent CPU
fallback. The twenty-surface direct pool preserves decoder/GXM ownership and the
copied ring is allocated only when required. Texture dimensions are aligned to
16 and respect hardware minimum 64x64; shrinking below that can overlap UV/luma.
Fences, AVFrame references and event signaling are part of correctness, not
optional performance switches. The old frame-threaded CPU zero-copy path is off.

`VITA_HW_DECODER_CPU_THREADS` accepts 1..3 (default 3) for internal CPU code. It
does not select three hardware decoder engines; h264_vita stays single-threaded
at the FFmpeg codec layer. The package does not change system clocks.

## Physical result already recorded

VitaMediaDeck PlayerFix captured HW 1280x720 at nominal 30 fps, 29.8 displayed
fps and 8 ms video lag in the reported sample. API readback: CPU 333, BUS/GPU
222, XBAR 111 MHz. The user confirmed that configuration works well for the
material tested. It does not validate 720p60, every bitrate/profile, a complete
333/166/166 clock profile, battery savings or a fresh 1.4.1 physical run.

VitaMediaDeck now requests CPU 333 MHz for HW and 444 MHz for SW, with
BUS/GPU/XBAR 222/222/111 MHz. The two CPU targets are independently configurable
in the app; neither decoder package sets system clocks. API readback must be
measured: a request is not proof of the actual clock. PSVshell manual hooks take
priority, and explicit PSVshell mode writes no frequencies. A physical comparison
requires the same file, intervals, backend, clocks, temperature, audio/subtitles
and UI load, plus throughput, dropped pictures, lag, memory and battery data.

The detailed evidence is in VitaMediaDeck's
[PlayerFix report](https://github.com/spyro-98/VitaMediaDeck/blob/feature/iptv/docs/console-validation-playerfix-2026-10-23.md).
The paper/algorithm research is historical candidate work, not implemented speed
or energy gains. See [validation](VALIDATION.md) for reproducible checks.

## 27 October: integrated IPTV HW confirmation

The user confirmed excellent IPTV hardware playback after app commit `5419ffe`
fixed HLS timestamps and the app-built FFmpeg codec lifecycle, including several
sources described as 1080p. This is a successful physical playback observation,
not a measured 1080p/FPS, battery or clock result. It does not change the live API
or rendition-selection limits. The patch belongs to VitaMediaDeck's FFmpeg build;
the standalone package builder is unchanged. See the
[app report](https://github.com/spyro-98/VitaMediaDeck/blob/feature/iptv/docs/iptv-hw-startup-2026-10-27.md).
