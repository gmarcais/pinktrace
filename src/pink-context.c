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

#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>

#include <pinktrace/internal.h>
#include <pinktrace/pink.h>

pink_context_t *
pink_context_new(void)
{
	pink_context_t *ctx;

	ctx = (pink_context_t *)malloc(sizeof(pink_context_t));
	if (!ctx)
		return NULL;

	ctx->options = PINK_TRACE_OPTION_SYSGOOD;
	ctx->eldest = -1;
	ctx->error = PINK_ERROR_SUCCESS;
	return ctx;
}

void
pink_context_free(pink_context_t *ctx)
{
	free(ctx);
}

void
pink_context_set_options(pink_context_t *ctx, int options)
{
	ctx->options = options;
	ctx->options |= PINK_TRACE_OPTION_SYSGOOD;
}

int
pink_context_get_options(const pink_context_t *ctx)
{
	return ctx->options;
}

void
pink_context_set_eldest(pink_context_t *ctx, pid_t pid)
{
	ctx->eldest = pid;
}

pid_t
pink_context_get_eldest(const pink_context_t *ctx)
{
	return ctx->eldest;
}

pink_error_t
pink_context_get_error(const pink_context_t *ctx)
{
	return ctx->error;
}

void
pink_context_clear_error(pink_context_t *ctx)
{
	ctx->error = PINK_ERROR_SUCCESS;
}
