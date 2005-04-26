/******************************************************************************
*******************************************************************************
**
**  Copyright (C) 2005 Red Hat, Inc.  All rights reserved.
**  
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
**
*******************************************************************************
******************************************************************************/

#include "dlm_internal.h"
#include "rcom.h"

/**
 * dlm_hash - hash an array of data
 * @data: the data to be hashed
 * @len: the length of data to be hashed
 *
 * Copied from GFS which copied from...
 *
 * Take some data and convert it to a 32-bit hash.
 * This is the 32-bit FNV-1a hash from:
 * http://www.isthe.com/chongo/tech/comp/fnv/
 */

static inline uint32_t hash_more_internal(const void *data, unsigned int len,
					  uint32_t hash)
{
	unsigned char *p = (unsigned char *)data;
	unsigned char *e = p + len;
	uint32_t h = hash;

	while (p < e) {
		h ^= (uint32_t)(*p++);
		h *= 0x01000193;
	}

	return h;
}

uint32_t dlm_hash(const void *data, int len)
{
	uint32_t h = 0x811C9DC5;
	h = hash_more_internal(data, len, h);
	return h;
}

static void header_out(struct dlm_header *hd)
{
	hd->h_version		= cpu_to_le32(hd->h_version);
	hd->h_lockspace		= cpu_to_le32(hd->h_lockspace);
	hd->h_nodeid		= cpu_to_le32(hd->h_nodeid);
	hd->h_length		= cpu_to_le16(hd->h_length);
}

static void header_in(struct dlm_header *hd)
{
	hd->h_version		= le32_to_cpu(hd->h_version);
	hd->h_lockspace		= le32_to_cpu(hd->h_lockspace);
	hd->h_nodeid		= le32_to_cpu(hd->h_nodeid);
	hd->h_length		= le16_to_cpu(hd->h_length);
}

void dlm_message_out(struct dlm_message *ms)
{
	struct dlm_header *hd = (struct dlm_header *) ms;

	header_out(hd);

	ms->m_type		= cpu_to_le32(ms->m_type);
	ms->m_nodeid		= cpu_to_le32(ms->m_nodeid);
	ms->m_pid		= cpu_to_le32(ms->m_pid);
	ms->m_lkid		= cpu_to_le32(ms->m_lkid);
	ms->m_remid		= cpu_to_le32(ms->m_remid);
	ms->m_parent_lkid	= cpu_to_le32(ms->m_parent_lkid);
	ms->m_parent_remid	= cpu_to_le32(ms->m_parent_remid);
	ms->m_exflags		= cpu_to_le32(ms->m_exflags);
	ms->m_sbflags		= cpu_to_le32(ms->m_sbflags);
	ms->m_flags		= cpu_to_le32(ms->m_flags);
	ms->m_lvbseq		= cpu_to_le32(ms->m_lvbseq);
	ms->m_status		= cpu_to_le32(ms->m_status);
	ms->m_grmode		= cpu_to_le32(ms->m_grmode);
	ms->m_rqmode		= cpu_to_le32(ms->m_rqmode);
	ms->m_bastmode		= cpu_to_le32(ms->m_bastmode);
	ms->m_asts		= cpu_to_le32(ms->m_asts);
	ms->m_result		= cpu_to_le32(ms->m_result);
	ms->m_range[0]		= cpu_to_le64(ms->m_range[0]);
	ms->m_range[1]		= cpu_to_le64(ms->m_range[1]);
}

void dlm_message_in(struct dlm_message *ms)
{
	struct dlm_header *hd = (struct dlm_header *) ms;

	header_in(hd);

	ms->m_type		= le32_to_cpu(ms->m_type);
	ms->m_nodeid		= le32_to_cpu(ms->m_nodeid);
	ms->m_pid		= le32_to_cpu(ms->m_pid);
	ms->m_lkid		= le32_to_cpu(ms->m_lkid);
	ms->m_remid		= le32_to_cpu(ms->m_remid);
	ms->m_parent_lkid	= le32_to_cpu(ms->m_parent_lkid);
	ms->m_parent_remid	= le32_to_cpu(ms->m_parent_remid);
	ms->m_exflags		= le32_to_cpu(ms->m_exflags);
	ms->m_sbflags		= le32_to_cpu(ms->m_sbflags);
	ms->m_flags		= le32_to_cpu(ms->m_flags);
	ms->m_lvbseq		= le32_to_cpu(ms->m_lvbseq);
	ms->m_status		= le32_to_cpu(ms->m_status);
	ms->m_grmode		= le32_to_cpu(ms->m_grmode);
	ms->m_rqmode		= le32_to_cpu(ms->m_rqmode);
	ms->m_bastmode		= le32_to_cpu(ms->m_bastmode);
	ms->m_asts		= le32_to_cpu(ms->m_asts);
	ms->m_result		= le32_to_cpu(ms->m_result);
	ms->m_range[0]		= le64_to_cpu(ms->m_range[0]);
	ms->m_range[1]		= le64_to_cpu(ms->m_range[1]);
}

static void rcom_lock_out(struct rcom_lock *rl)
{
	rl->rl_ownpid		= cpu_to_le32(rl->rl_ownpid);
	rl->rl_lkid		= cpu_to_le32(rl->rl_lkid);
	rl->rl_remid		= cpu_to_le32(rl->rl_remid);
	rl->rl_parent_lkid	= cpu_to_le32(rl->rl_parent_lkid);
	rl->rl_parent_remid	= cpu_to_le32(rl->rl_parent_remid);
	rl->rl_exflags		= cpu_to_le32(rl->rl_exflags);
	rl->rl_flags		= cpu_to_le32(rl->rl_flags);
	rl->rl_lvbseq		= cpu_to_le32(rl->rl_lvbseq);
	rl->rl_result		= cpu_to_le32(rl->rl_result);
	rl->rl_wait_type	= cpu_to_le16(rl->rl_wait_type);
	rl->rl_namelen		= cpu_to_le16(rl->rl_namelen);
	rl->rl_range[0]		= cpu_to_le64(rl->rl_range[0]);
	rl->rl_range[1]		= cpu_to_le64(rl->rl_range[1]);
	rl->rl_range[2]		= cpu_to_le64(rl->rl_range[2]);
	rl->rl_range[3]		= cpu_to_le64(rl->rl_range[3]);
}

static void rcom_lock_in(struct rcom_lock *rl)
{
	rl->rl_ownpid		= le32_to_cpu(rl->rl_ownpid);
	rl->rl_lkid		= le32_to_cpu(rl->rl_lkid);
	rl->rl_remid		= le32_to_cpu(rl->rl_remid);
	rl->rl_parent_lkid	= le32_to_cpu(rl->rl_parent_lkid);
	rl->rl_parent_remid	= le32_to_cpu(rl->rl_parent_remid);
	rl->rl_exflags		= le32_to_cpu(rl->rl_exflags);
	rl->rl_flags		= le32_to_cpu(rl->rl_flags);
	rl->rl_lvbseq		= le32_to_cpu(rl->rl_lvbseq);
	rl->rl_result		= le32_to_cpu(rl->rl_result);
	rl->rl_wait_type	= le16_to_cpu(rl->rl_wait_type);
	rl->rl_namelen		= le16_to_cpu(rl->rl_namelen);
	rl->rl_range[0]		= le64_to_cpu(rl->rl_range[0]);
	rl->rl_range[1]		= le64_to_cpu(rl->rl_range[1]);
	rl->rl_range[2]		= le64_to_cpu(rl->rl_range[2]);
	rl->rl_range[3]		= le64_to_cpu(rl->rl_range[3]);
}

void dlm_rcom_out(struct dlm_rcom *rc)
{
	struct dlm_header *hd = (struct dlm_header *) rc;
	int type = rc->rc_type;

	header_out(hd);

	rc->rc_type		= cpu_to_le16(rc->rc_type);
	rc->rc_result		= cpu_to_le16(rc->rc_result);
	rc->rc_id		= cpu_to_le64(rc->rc_id);

	if (type == DLM_RCOM_LOCK)
		rcom_lock_out((struct rcom_lock *) rc->rc_buf);
}

void dlm_rcom_in(struct dlm_rcom *rc)
{
	struct dlm_header *hd = (struct dlm_header *) rc;

	header_in(hd);

	rc->rc_type		= le16_to_cpu(rc->rc_type);
	rc->rc_result		= le16_to_cpu(rc->rc_result);
	rc->rc_id		= le64_to_cpu(rc->rc_id);

	if (rc->rc_type == DLM_RCOM_LOCK)
		rcom_lock_in((struct rcom_lock *) rc->rc_buf);
}

