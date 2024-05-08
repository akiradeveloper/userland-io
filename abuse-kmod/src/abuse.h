/*
 * include/linux/abuse.h
 *
 * Copyright (c) 2009 Zachary Amsden
 * Copyright (c) 2015 Naohiro Aota
 * Copyright (c) 2021 Akira Hayakawa <ruby.wktk@gmail.com>
 * 
 * Redistribution of this file is permitted under the GNU General Public License.
 */

#ifndef _LINUX_ABUSE_H
#define _LINUX_ABUSE_H

#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/types.h> /* for __u64 */

enum {
	ABUSE_FLAGS_READ_ONLY	= 1,
};

struct abuse_info {
	__u32 ab_number; /* r/o */
	__u64 ab_size; /* r/w */
	__u32 ab_blocksize; /* r/w */
};

#define ABUSE_GET_STATUS 0x4120
#define ABUSE_SET_STATUS 0x4121
#define ABUSE_RESET	0x4122
#define ABUSE_GET_REQ 0x4123
#define ABUSE_PUT_REQ 0x4124

#define ABUSE_CTL_ADD 0x4186
#define ABUSE_CTL_REMOVE 0x4187
#define ABUSE_CONNECT 0x4188

struct abuse_vec {
	__u64 ab_address;
	__u32 ab_offset;
	__u32 ab_len;
};

#define CMD_OP_UNKNOWN 0
#define CMD_OP_READ 1
#define CMD_OP_WRITE 2
#define CMD_OP_FLUSH 3
#define CMD_OP_DISCARD 4
#define CMD_OP_SECURE_ERASE 5
#define CMD_OP_WRITE_ZEROES 6

#define CMD_FUA 1<<8
#define CMD_PREFLUSH 1<<9
#define CMD_NOUNMAP 1<<10
#define CMD_NOWAIT 1<<11
#define CMD_RAHEAD 1<<12

struct abuse_xfr_hdr {
	__u64 ab_id;
	__u64 ab_offset;
	__u64 ab_len;
	__u32 ab_command;
	__u32 ab_vec_count;
	__u64 ab_transfer_address;
};

struct abuse_completion {
	__u64 ab_id;
	__u32 ab_errno;
};

#define ABUSE_MAJOR 60
#define ABUSECTL_MAJOR 61

struct abuse_device {
	int ab_number;
	loff_t ab_size;
	unsigned ab_blocksize;

	spinlock_t ab_lock;
	struct list_head ab_reqlist;
	wait_queue_head_t ab_event;

	struct request_queue *ab_queue;
	struct blk_mq_tag_set tag_set;
	struct gendisk *ab_disk;

	/* user xfer area */
	struct abuse_vec ab_xfer[BIO_MAX_VECS];
};

struct ab_req {
	struct request *rq;
	struct list_head list;
};

#endif