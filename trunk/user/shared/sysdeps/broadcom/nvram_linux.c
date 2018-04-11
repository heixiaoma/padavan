/*
 * NVRAM variable manipulation (Linux user mode half)
 *
 * Copyright (C) 2012, Broadcom Corporation. All Rights Reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $Id: nvram_linux.c 349381 2012-08-08 03:34:49Z $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <bcmnvram.h>

#define PATH_DEV_NVRAM "/dev/nvram"

/* Globals */
static int nvram_fd = -1;
static char *nvram_buf = NULL;

int
nvram_init(void *unused)
{
	if (nvram_fd >= 0)
		return 0;

	if ((nvram_fd = open(PATH_DEV_NVRAM, O_RDWR)) < 0)
		goto err;

	/* Map kernel string buffer into user space */
	nvram_buf = mmap(NULL, MAX_NVRAM_SPACE, PROT_READ, MAP_SHARED, nvram_fd, 0);
	if (nvram_buf == MAP_FAILED) {
		close(nvram_fd);
		nvram_fd = -1;
		goto err;
	}

	fcntl(nvram_fd, F_SETFD, FD_CLOEXEC);

	return 0;

err:
	perror(PATH_DEV_NVRAM);
	return errno;
}

char *
nvram_get(const char *name)
{
	size_t count = strlen(name) + 1;
	char tmp[100], *value;
	unsigned long *off = (unsigned long *) tmp;

	if (nvram_init(NULL))
		return NULL;

	if (count > sizeof(tmp))
		if (!(off = malloc(count)))
			return NULL;

	/* Get offset into mmap() space */
	strcpy((char *) off, name);

	count = read(nvram_fd, off, count);
	if (count < 0)
		perror(PATH_DEV_NVRAM);

	value = (count == sizeof(unsigned long) ? (&nvram_buf[*off]) : NULL);

	if (off != (unsigned long *) tmp)
		free(off);

	return value;
}

char *
nvram_safe_get(const char *name)
{
	static const char nvram_empty[4] = {0};
	char *value = nvram_get(name);

	return (value ? value : (char *)nvram_empty);
}

int
nvram_get_int(const char *name)
{
	char *value = nvram_get(name);

	return (value ? atoi(value) : 0);
}

int
nvram_safe_get_int(const char* name, int val_def, int val_min, int val_max)
{
	int i_value = nvram_get_int(name);

	if (i_value < val_min)
		i_value = val_def;
	else if (i_value > val_max)
		i_value = val_def;

	return i_value;
}

int
nvram_getall(char *buf, int count, int include_temp)
{
	int ret;
	anvram_ioctl_t nvr;

	if (!buf || count < 1)
		return 0;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	/* Get all variables */
	*buf = '\0';

	nvr.size = sizeof(nvr);
	nvr.len_param = 0;
	nvr.len_value = count;
	nvr.param = NULL;
	nvr.value = buf;
	nvr.is_temp = include_temp;

	ret = ioctl(nvram_fd, NVRAM_IOCTL_GET, &nvr);
	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return ret;
}

static char *nvram_xfr_buf = NULL;

char *
nvram_xfr(const char *buf)
{
	size_t count = strlen(buf)*2+1; // ham 1120
	int ret;
	char tmpbuf[1024];

	if (count > sizeof(tmpbuf))
		return NULL;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return NULL;

	strcpy(tmpbuf, buf);

	if (!nvram_xfr_buf)
		if(!(nvram_xfr_buf = (char *)malloc(1024+1)))
			return NULL;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, tmpbuf);
	if (ret < 0) {
		perror(PATH_DEV_NVRAM);
		return NULL;
	} else {
		strcpy(nvram_xfr_buf, tmpbuf);
		return nvram_xfr_buf;
	}
}

static int
_nvram_set(const char *name, const char *value, int is_temp)
{
	int ret;
	anvram_ioctl_t nvr;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return -1;

	nvr.size = sizeof(nvr);
	nvr.len_param = strlen(name);
	nvr.len_value = 0;
	nvr.param = (char*)name;
	nvr.value = (char*)value;
	nvr.is_temp = is_temp;
	if (value)
		nvr.len_value = strlen(value);

	ret = ioctl(nvram_fd, NVRAM_IOCTL_SET, &nvr);
	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return ret;
}

int
nvram_set(const char *name, const char *value)
{
	return _nvram_set(name, value, 0);
}

int
nvram_set_int(const char *name, int value)
{
	char int_str[16];
	sprintf(int_str, "%d", value);
	return _nvram_set(name, int_str, 0);
}

int
nvram_unset(const char *name)
{
	return _nvram_set(name, NULL, 0);
}

int
nvram_set_temp(const char *name, const char *value)
{
	return _nvram_set(name, value, 1);
}

int
nvram_set_int_temp(const char *name, int value)
{
	char int_str[16];
	sprintf(int_str, "%d", value);
	return _nvram_set(name, int_str, 1);
}

int
nvram_match(const char *name, char *match)
{
	const char *value = nvram_get(name);
	return (value && !strcmp(value, match));
}

int
nvram_invmatch(const char *name, char *invmatch)
{
	const char *value = nvram_get(name);
	return (value && strcmp(value, invmatch));
}

int
nvram_commit(void)
{
	int ret;

	if ((ret = nvram_init(NULL)))
		return -1;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, NULL);
	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return ret;
}

int
nvram_clear(void)
{
	int ret;

	if ((ret = nvram_init(NULL)))
		return -1;

	ret = ioctl(nvram_fd, NVRAM_IOCTL_CLEAR, 0);
	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return ret;
}
