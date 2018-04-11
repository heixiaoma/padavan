/*
 * NVRAM variable manipulation
 *
 */

#ifndef _bcmnvram_h_
#define _bcmnvram_h_

#define NVRAM_MAGIC		0x48534C46	/* 'FLSH' */
/* This definition is for precommit staging, and will be removed */
#define NVRAM_SPACE		0x10000
/* For CFE builds this gets passed in thru the makefile */
#ifndef MAX_NVRAM_SPACE
#define MAX_NVRAM_SPACE	0x10000
#endif

#define NVRAM_MAX_PARAM_LEN	64
#define NVRAM_MAX_VALUE_LEN	4096

#define NVRAM_IOCTL_COMMIT	10
#define NVRAM_IOCTL_CLEAR	20
#define NVRAM_IOCTL_SET	30
#define NVRAM_IOCTL_GET	40

struct nvram_header {
	uint32_t magic;
	uint32_t len;
	uint32_t crc_ver_init;		/* 0:7 crc, 8:15 ver, 16:31 sdram_init */
	uint32_t config_refresh;	/* 0:15 sdram_config, 16:31 sdram_refresh */
	uint32_t config_ncdl;		/* ncdl values for memc */
};

typedef struct anvram_ioctl_s {
	int size;
	int is_temp;
	int len_param;
	int len_value;
	char *param;
	char *value;
} anvram_ioctl_t;

#endif /* _bcmnvram_h_ */
