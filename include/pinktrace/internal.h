/* vim: set cino= fo=croql sw=8 ts=8 sts=0 noet cin fdm=syntax : */

/*
 * Copyright (c) 2010 Ali Polatel <alip@exherbo.org>
 *
 * This file is part of Pink's Tracing Library. pinktrace is free software; you
 * can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License version 2.1, as published by the Free Software
 * Foundation.
 *
 * pinktrace is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef PINKTRACE_GUARD_INTERNAL_H
#define PINKTRACE_GUARD_INTERNAL_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <sys/un.h>

#ifdef HAVE_SYS_REG_H
#include <sys/reg.h>
#endif /*  HAVE_SYS_REG_H */

/* We need additional hackery on IA64 to include linux/ptrace.h. */
#if defined(IA64)
#ifdef HAVE_STRUCT_IA64_FPREG
#define ia64_fpreg XXX_ia64_fpreg
#endif /* HAVE_STRUCT_IA64_FPREG */
#ifdef HAVE_STRUCT_PT_ALL_USER_REGS
#define pt_all_user_regs XXX_pt_all_user_regs
#endif /* HAVE_STRUCT_PT_ALL_USER_REGS */
#endif /* defined(IA64) */
#include <linux/ptrace.h>
#if defined(IA64)
#undef ia64_fpreg
#undef pt_all_user_regs
#endif /* defined(IA64) */

#define MAX_ARGS	6
#define ADDR_MUL	((64 == __WORDSIZE) ? 8 : 4)

#include <pinktrace/context.h>
#include <pinktrace/error.h>
#include <pinktrace/socket.h>

struct pink_context
{
	bool attach;
	int options;
	pid_t eldest;
	pink_error_t error;
};

struct pink_sockaddr {
	int family;

	union {
		char pad[128];
		struct sockaddr sa;
		struct sockaddr_un sa_un;
		struct sockaddr_in sa_in;
#if PINKTRACE_HAVE_IPV6
		struct sockaddr_in6 sa6;
#endif /* PINKTRACE_HAVE_IPV6 */
	} u;
};

const char *
pink_name_syscall_nobitness(long scno);

const char *
pink_name_syscall32(long scno);

const char *
pink_name_syscall64(long scno);

pink_sockaddr_t *
pink_internal_decode_socket_address(pid_t pid, long addr, long addrlen);

#endif /* !PINKTRACE_GUARD_INTERNAL_H */
