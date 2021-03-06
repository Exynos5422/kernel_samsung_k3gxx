/*
<<<<<<< HEAD
 * u_uac1.c -- ALSA audio utilities for Gadget stack
 *
=======
 * u_audio.c -- ALSA audio utilities for Gadget stack
 *
 * Copyright (c) 2012-2014, The Linux Foundation. All rights reserved.
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
 * Copyright (C) 2008 Bryan Wu <cooloney@kernel.org>
 * Copyright (C) 2008 Analog Devices, Inc
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/ctype.h>
#include <linux/random.h>
#include <linux/syscalls.h>

#include "u_uac1.h"

<<<<<<< HEAD
=======
#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) "%s: " fmt, __func__

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
/*
 * This component encapsulates the ALSA devices for USB audio gadget
 */

<<<<<<< HEAD
#define FILE_PCM_PLAYBACK	"/dev/snd/pcmC0D0p"
#define FILE_PCM_CAPTURE	"/dev/snd/pcmC0D0c"
=======
#define FILE_PCM_PLAYBACK	"/dev/snd/pcmC0D5p"
#define FILE_PCM_CAPTURE	"/dev/snd/pcmC0D6c"
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
#define FILE_CONTROL		"/dev/snd/controlC0"

static char *fn_play = FILE_PCM_PLAYBACK;
module_param(fn_play, charp, S_IRUGO);
MODULE_PARM_DESC(fn_play, "Playback PCM device file name");

static char *fn_cap = FILE_PCM_CAPTURE;
module_param(fn_cap, charp, S_IRUGO);
MODULE_PARM_DESC(fn_cap, "Capture PCM device file name");

static char *fn_cntl = FILE_CONTROL;
module_param(fn_cntl, charp, S_IRUGO);
MODULE_PARM_DESC(fn_cntl, "Control device file name");

<<<<<<< HEAD
=======
static struct gaudio *the_card;

static bool audio_reinit;

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
/*-------------------------------------------------------------------------*/

/**
 * Some ALSA internal helper functions
 */
static int snd_interval_refine_set(struct snd_interval *i, unsigned int val)
{
	struct snd_interval t;
	t.empty = 0;
	t.min = t.max = val;
	t.openmin = t.openmax = 0;
	t.integer = 1;
	return snd_interval_refine(i, &t);
}

static int _snd_pcm_hw_param_set(struct snd_pcm_hw_params *params,
				 snd_pcm_hw_param_t var, unsigned int val,
				 int dir)
{
	int changed;
	if (hw_is_mask(var)) {
		struct snd_mask *m = hw_param_mask(params, var);
		if (val == 0 && dir < 0) {
			changed = -EINVAL;
			snd_mask_none(m);
		} else {
			if (dir > 0)
				val++;
			else if (dir < 0)
				val--;
			changed = snd_mask_refine_set(
					hw_param_mask(params, var), val);
		}
	} else if (hw_is_interval(var)) {
		struct snd_interval *i = hw_param_interval(params, var);
		if (val == 0 && dir < 0) {
			changed = -EINVAL;
			snd_interval_none(i);
		} else if (dir == 0)
			changed = snd_interval_refine_set(i, val);
		else {
			struct snd_interval t;
			t.openmin = 1;
			t.openmax = 1;
			t.empty = 0;
			t.integer = 0;
			if (dir < 0) {
				t.min = val - 1;
				t.max = val;
			} else {
				t.min = val;
				t.max = val+1;
			}
			changed = snd_interval_refine(i, &t);
		}
<<<<<<< HEAD
	} else
		return -EINVAL;
=======
	} else {
		return -EINVAL;
	}
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	if (changed) {
		params->cmask |= 1 << var;
		params->rmask |= 1 << var;
	}
	return changed;
}
/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
/**
 * Set default hardware params
 */
static int playback_default_hw_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_hw_params *params;
=======
static inline
struct snd_interval *param_to_interval(struct snd_pcm_hw_params *p, int n)
{
	return &(p->intervals[n - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL]);
}

int pcm_buffer_size(struct snd_pcm_hw_params *params)
{
	struct snd_interval *i =
		param_to_interval(params, SNDRV_PCM_HW_PARAM_BUFFER_BYTES);
	pr_debug("buffer_bytes = (%d,%d) omin=%d omax=%d int=%d empty=%d\n",
		i->min, i->max, i->openmin, i->openmax, i->integer, i->empty);
	return i->min;
}

int pcm_period_size(struct snd_pcm_hw_params *params)
{
	struct snd_interval *i =
		param_to_interval(params, SNDRV_PCM_HW_PARAM_PERIOD_BYTES);
	return i->min;
}

/**
 * Set default hardware params
 */
static int playback_prepare_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_hw_params *params;
	struct snd_pcm_sw_params *swparams;
	unsigned long period_size;
	unsigned long buffer_size;
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	snd_pcm_sframes_t result;

       /*
	* SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	* SNDRV_PCM_FORMAT_S16_LE
	* CHANNELS: 2
<<<<<<< HEAD
	* RATE: 48000
=======
	* RATE: 8000
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	*/
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 2;
<<<<<<< HEAD
	snd->rate = 48000;
=======
	snd->rate = 8000;
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

<<<<<<< HEAD
	snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DROP, NULL);
	snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_HW_PARAMS, params);

	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_PREPARE, NULL);
	if (result < 0) {
		ERROR(snd->card,
			"Preparing sound card failed: %d\n", (int)result);
=======
	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DROP, NULL);
	if (result < 0)
		pr_err("SNDRV_PCM_IOCTL_DROP failed: %d\n", (int)result);

	result = snd_pcm_kernel_ioctl(substream,
			SNDRV_PCM_IOCTL_HW_PARAMS, params);
	if (result < 0) {
		pr_err("SNDRV_PCM_IOCTL_HW_PARAMS failed: %d\n", (int)result);
		kfree(params);
		return result;
	}

	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_PREPARE, NULL);
	if (result < 0)
		pr_err("Preparing playback failed: %d\n", (int)result);


	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

	/* Set SW params */
	swparams = kzalloc(sizeof(*swparams), GFP_KERNEL);
	if (!swparams) {
		pr_err("Failed to allocate sw params");
		return -ENOMEM;
	}

	buffer_size = pcm_buffer_size(params);
	period_size = pcm_period_size(params);
	swparams->avail_min = period_size/2;
	swparams->xfer_align = period_size/2;

	swparams->tstamp_mode = SNDRV_PCM_TSTAMP_NONE;
	swparams->period_step = 1;
	swparams->start_threshold = 1;
	swparams->stop_threshold = INT_MAX;
	swparams->silence_size = 0;
	swparams->silence_threshold = 0;

	result = snd_pcm_kernel_ioctl(substream,
				      SNDRV_PCM_IOCTL_SW_PARAMS, swparams);
	if (result < 0)
		pr_err("SNDRV_PCM_IOCTL_SW_PARAMS failed: %d\n", (int)result);
	kfree(swparams);

	kfree(params);

	pr_debug("playback params: access %x, format %x, channels %d, rate %d\n",
		snd->access, snd->format, snd->channels, snd->rate);

	return 0;
}

static int capture_prepare_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_runtime   *runtime = substream->runtime;
	struct snd_pcm_hw_params *params;
	struct snd_pcm_sw_params *swparams;
	unsigned long period_size;
	unsigned long buffer_size;
	snd_pcm_sframes_t result = 0;

	/*
	 * SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	 * SNDRV_PCM_FORMAT_S16_LE
	 * CHANNELS: 1
	 * RATE: 8000
	 */
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 1;
	snd->rate = 8000;

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params) {
		pr_err("Failed to allocate hw params");
		return -ENOMEM;
	}

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DROP, NULL);
	if (result < 0)
		pr_err("SNDRV_PCM_IOCTL_DROP failed: %d\n", (int)result);

	result = snd_pcm_kernel_ioctl(substream,
			SNDRV_PCM_IOCTL_HW_PARAMS, params);
	if (result < 0) {
		pr_err("SNDRV_PCM_IOCTL_HW_PARAMS failed: %d\n", (int)result);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
		kfree(params);
		return result;
	}

<<<<<<< HEAD
=======
	result = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_PREPARE,
					NULL);
	if (result < 0)
		pr_err("Preparing capture failed: %d\n", (int)result);

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

<<<<<<< HEAD
	kfree(params);

	INFO(snd->card,
		"Hardware params: access %x, format %x, channels %d, rate %d\n",
=======
	runtime->frame_bits = snd_pcm_format_physical_width(runtime->format);

	kfree(params);

	swparams = kzalloc(sizeof(*swparams), GFP_KERNEL);
	if (!swparams) {
		pr_err("Failed to allocate sw params");
		return -ENOMEM;
	}

	buffer_size = pcm_buffer_size(params);
	period_size = pcm_period_size(params);
	swparams->avail_min = period_size/2;
	swparams->xfer_align = period_size/2;

	swparams->tstamp_mode = SNDRV_PCM_TSTAMP_NONE;
	swparams->period_step = 1;
	swparams->start_threshold = 1;
	swparams->stop_threshold = INT_MAX;
	swparams->silence_size = 0;
	swparams->silence_threshold = 0;

	result = snd_pcm_kernel_ioctl(substream,
			SNDRV_PCM_IOCTL_SW_PARAMS, swparams);
	if (result < 0)
		pr_err("SNDRV_PCM_IOCTL_SW_PARAMS failed: %d\n", (int)result);

	kfree(swparams);

	pr_debug("capture params: access %x, format %x, channels %d, rate %d\n",
		snd->access, snd->format, snd->channels, snd->rate);

	return result;
}

/**
 * Set default hardware params
 */
static int playback_default_hw_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_hw_params *params;

       /*
	* SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	* SNDRV_PCM_FORMAT_S16_LE
	* CHANNELS: 2
	* RATE: 8000
	*/
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 2;
	snd->rate = 8000;

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

	kfree(params);

	pr_debug("playback params: access %x, format %x, channels %d, rate %d\n",
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
		snd->access, snd->format, snd->channels, snd->rate);

	return 0;
}

<<<<<<< HEAD
=======
static int capture_default_hw_params(struct gaudio_snd_dev *snd)
{
	struct snd_pcm_hw_params *params;

	/*
	 * SNDRV_PCM_ACCESS_RW_INTERLEAVED,
	 * SNDRV_PCM_FORMAT_S16_LE
	 * CHANNELS: 1
	 * RATE: 8000
	 */
	snd->access = SNDRV_PCM_ACCESS_RW_INTERLEAVED;
	snd->format = SNDRV_PCM_FORMAT_S16_LE;
	snd->channels = 1;
	snd->rate = 8000;

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	_snd_pcm_hw_params_any(params);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_ACCESS,
			snd->access, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_FORMAT,
			snd->format, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_CHANNELS,
			snd->channels, 0);
	_snd_pcm_hw_param_set(params, SNDRV_PCM_HW_PARAM_RATE,
			snd->rate, 0);

	/* Store the hardware parameters */
	snd->access = params_access(params);
	snd->format = params_format(params);
	snd->channels = params_channels(params);
	snd->rate = params_rate(params);

	kfree(params);

	pr_debug("capture params: access %x, format %x, channels %d, rate %d\n",
		snd->access, snd->format, snd->channels, snd->rate);

	return 0;
}

static int gaudio_open_streams(void)
{
	struct gaudio_snd_dev *snd;
	int res = 0;

	if (!the_card) {
		pr_err("%s: Card is NULL", __func__);
		return -ENODEV;
	}

	pr_debug("Initialize hw params");

	/* Open PCM playback device and setup substream */
	snd = &the_card->playback;
	res = playback_prepare_params(snd);
	if (res) {
		pr_err("Setting playback params failed: err %d", res);
		return res;
	}

	pr_debug("Initialized playback params");

	/* Open PCM capture device and setup substream */
	snd = &the_card->capture;
	res = capture_prepare_params(snd);
	if (res) {
		pr_err("Setting capture params failed: err %d", res);
		return res;
	}

	pr_info("Initialized capture params");

	return 0;
}

void u_audio_clear(void)
{
	audio_reinit = false;
}

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
/**
 * Playback audio buffer data by ALSA PCM device
 */
static size_t u_audio_playback(struct gaudio *card, void *buf, size_t count)
{
	struct gaudio_snd_dev	*snd = &card->playback;
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_runtime *runtime = substream->runtime;
	mm_segment_t old_fs;
	ssize_t result;
	snd_pcm_sframes_t frames;
<<<<<<< HEAD

try_again:
	if (runtime->status->state == SNDRV_PCM_STATE_XRUN ||
		runtime->status->state == SNDRV_PCM_STATE_SUSPENDED) {
		result = snd_pcm_kernel_ioctl(substream,
				SNDRV_PCM_IOCTL_PREPARE, NULL);
		if (result < 0) {
			ERROR(card, "Preparing sound card failed: %d\n",
=======
	int err = 0;

	if (!count) {
		pr_err("Buffer is empty, no data to play");
		return 0;
	}

	if (!audio_reinit) {
		err = gaudio_open_streams();
		if (err) {
			pr_err("Failed to init audio streams");
			return 0;
		}
		audio_reinit = 1;
	}

try_again:
	if (runtime->status->state == SNDRV_PCM_STATE_XRUN ||
		runtime->status->state == SNDRV_PCM_STATE_SUSPENDED ||
		runtime->status->state == SNDRV_PCM_STATE_SETUP) {
		result = snd_pcm_kernel_ioctl(substream,
				SNDRV_PCM_IOCTL_PREPARE, NULL);
		if (result < 0) {
			pr_err("Preparing playback failed: %d\n",
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
					(int)result);
			return result;
		}
	}

<<<<<<< HEAD
	frames = bytes_to_frames(runtime, count);
=======
	if (!runtime->frame_bits) {
		pr_err("SND failure - runtime->frame_bits == 0");
		return 0;
	}

	frames = bytes_to_frames(runtime, count);
	pr_debug("runtime->frame_bits = %d, count = %d, frames = %d",
		runtime->frame_bits, (int)count, (int)frames);

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	result = snd_pcm_lib_write(snd->substream, buf, frames);
	if (result != frames) {
<<<<<<< HEAD
		ERROR(card, "Playback error: %d\n", (int)result);
=======
		pr_err("snd_pcm_lib_write failed with err %d\n", (int)result);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
		set_fs(old_fs);
		goto try_again;
	}
	set_fs(old_fs);

<<<<<<< HEAD
=======
	pr_debug("Done. Sent %d frames", (int)frames);

	return 0;
}

static size_t u_audio_capture(struct gaudio *card, void *buf, size_t count)
{
	ssize_t result;
	mm_segment_t old_fs;
	snd_pcm_sframes_t frames;
	int err = 0;

	struct gaudio_snd_dev	 *snd = &card->capture;
	struct snd_pcm_substream *substream = snd->substream;
	struct snd_pcm_runtime   *runtime = substream->runtime;

	if (!audio_reinit) {
		err = gaudio_open_streams();
		if (err) {
			pr_err("Failed to init audio streams: err %d", err);
			return 0;
		}
		audio_reinit = 1;
	}

try_again:
	if (runtime->status->state == SNDRV_PCM_STATE_XRUN ||
		runtime->status->state == SNDRV_PCM_STATE_SUSPENDED ||
		runtime->status->state == SNDRV_PCM_STATE_SETUP) {
		result = snd_pcm_kernel_ioctl(substream,
				SNDRV_PCM_IOCTL_PREPARE, NULL);
		if (result < 0) {
			pr_err("Preparing capture failed: %d\n",
					(int)result);
			return result;
		}
	}

	frames = bytes_to_frames(runtime, count);

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	pr_debug("frames = %d, count = %zd", (int)frames, count);

	result = snd_pcm_lib_read(substream, buf, frames);
	if (result != frames) {
		pr_err("Capture error: %d\n", (int)result);
		set_fs(old_fs);
		goto try_again;
	}

	set_fs(old_fs);

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	return 0;
}

static int u_audio_get_playback_channels(struct gaudio *card)
{
<<<<<<< HEAD
=======
	pr_debug("Return %d", card->playback.channels);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	return card->playback.channels;
}

static int u_audio_get_playback_rate(struct gaudio *card)
{
<<<<<<< HEAD
	return card->playback.rate;
}

=======
	pr_debug("Return %d", card->playback.rate);
	return card->playback.rate;
}

static int u_audio_get_capture_channels(struct gaudio *card)
{
	pr_debug("Return %d", card->capture.channels);
	return card->capture.channels;
}

static int u_audio_get_capture_rate(struct gaudio *card)
{
	pr_debug("Return %d", card->capture.rate);
	return card->capture.rate;
}


>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
/**
 * Open ALSA PCM and control device files
 * Initial the PCM or control device
 */
static int gaudio_open_snd_dev(struct gaudio *card)
{
	struct snd_pcm_file *pcm_file;
	struct gaudio_snd_dev *snd;
<<<<<<< HEAD

	if (!card)
		return -ENODEV;

=======
	int res = 0;

	if (!card) {
		pr_err("%s: Card is NULL", __func__);
		return -ENODEV;
	}
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	/* Open control device */
	snd = &card->control;
	snd->filp = filp_open(fn_cntl, O_RDWR, 0);
	if (IS_ERR(snd->filp)) {
		int ret = PTR_ERR(snd->filp);
<<<<<<< HEAD
		ERROR(card, "unable to open sound control device file: %s\n",
=======
		pr_err("unable to open sound control device file: %s\n",
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
				fn_cntl);
		snd->filp = NULL;
		return ret;
	}
	snd->card = card;

	/* Open PCM playback device and setup substream */
	snd = &card->playback;
	snd->filp = filp_open(fn_play, O_WRONLY, 0);
	if (IS_ERR(snd->filp)) {
		int ret = PTR_ERR(snd->filp);

<<<<<<< HEAD
		ERROR(card, "No such PCM playback device: %s\n", fn_play);
		snd->filp = NULL;
		return ret;
	}
	pcm_file = snd->filp->private_data;
	snd->substream = pcm_file->substream;
	snd->card = card;
	playback_default_hw_params(snd);
=======
		pr_err("No such PCM playback device: %s\n", fn_play);
		snd->filp = NULL;
		return ret;
	}
	pr_debug("Initialized PCM playback device: %s\n", fn_play);

	pcm_file = snd->filp->private_data;
	snd->substream = pcm_file->substream;
	snd->card = card;
	res = playback_default_hw_params(snd);
	if (res) {
		pr_err("Setting playback HW params failed: err %d", res);
		return res;
	}
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83

	/* Open PCM capture device and setup substream */
	snd = &card->capture;
	snd->filp = filp_open(fn_cap, O_RDONLY, 0);
	if (IS_ERR(snd->filp)) {
<<<<<<< HEAD
		ERROR(card, "No such PCM capture device: %s\n", fn_cap);
		snd->substream = NULL;
		snd->card = NULL;
		snd->filp = NULL;
	} else {
		pcm_file = snd->filp->private_data;
		snd->substream = pcm_file->substream;
		snd->card = card;
	}

	return 0;
=======
		pr_err("No such PCM capture device: %s\n", fn_cap);
		snd->substream = NULL;
		snd->card = NULL;
		snd->filp = NULL;
		return -EINVAL;
	}

	pcm_file = snd->filp->private_data;
	snd->substream = pcm_file->substream;
	snd->card = card;
	res = capture_default_hw_params(snd);

	if (res)
		pr_err("Setting capture HW params failed: err %d", res);

	return res;
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
}

/**
 * Close ALSA PCM and control device files
 */
static int gaudio_close_snd_dev(struct gaudio *gau)
{
	struct gaudio_snd_dev	*snd;

<<<<<<< HEAD
=======
	pr_debug("Enter");
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	/* Close control device */
	snd = &gau->control;
	if (snd->filp)
		filp_close(snd->filp, NULL);

	/* Close PCM playback device and setup substream */
	snd = &gau->playback;
	if (snd->filp)
		filp_close(snd->filp, NULL);

	/* Close PCM capture device and setup substream */
	snd = &gau->capture;
	if (snd->filp)
		filp_close(snd->filp, NULL);

	return 0;
}

<<<<<<< HEAD
static struct gaudio *the_card;
=======

>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
/**
 * gaudio_setup - setup ALSA interface and preparing for USB transfer
 *
 * This sets up PCM, mixer or MIDI ALSA devices fore USB gadget using.
 *
 * Returns negative errno, or zero on success
 */
<<<<<<< HEAD
int __init gaudio_setup(struct gaudio *card)
=======
int gaudio_setup(struct gaudio *card)
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
{
	int	ret;

	ret = gaudio_open_snd_dev(card);
	if (ret)
<<<<<<< HEAD
		ERROR(card, "we need at least one control device\n");
=======
		pr_err("Failed to open snd devices\n");
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
	else if (!the_card)
		the_card = card;

	return ret;
<<<<<<< HEAD

=======
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
}

/**
 * gaudio_cleanup - remove ALSA device interface
 *
 * This is called to free all resources allocated by @gaudio_setup().
 */
void gaudio_cleanup(void)
{
	if (the_card) {
		gaudio_close_snd_dev(the_card);
		the_card = NULL;
	}
}

