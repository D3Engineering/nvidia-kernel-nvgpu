/*
* Copyright (c) 2016-2017, NVIDIA CORPORATION.  All rights reserved.
*
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*/

#include "gk20a/gk20a.h"
#include "boardobj.h"
#include "boardobjgrp_e255.h"
#include "ctrl/ctrlboardobj.h"
#include "boardobjgrp.h"
#include "boardobjgrpmask.h"

u32 boardobjgrpconstruct_e255(struct gk20a *g,
			      struct boardobjgrp_e255 *pboardobjgrp_e255)
{
	u32 status = 0;
	u8  objslots;

	gk20a_dbg_info("");

	objslots = 255;
	status = boardobjgrpmask_e255_init(&pboardobjgrp_e255->mask, NULL);
	if (status)
		goto boardobjgrpconstruct_e255_exit;

	pboardobjgrp_e255->super.type      = CTRL_BOARDOBJGRP_TYPE_E255;
	pboardobjgrp_e255->super.ppobjects = pboardobjgrp_e255->objects;
	pboardobjgrp_e255->super.objslots  = objslots;
	pboardobjgrp_e255->super.mask     = &(pboardobjgrp_e255->mask.super);

	status = boardobjgrp_construct_super(g, &pboardobjgrp_e255->super);
	if (status)
		goto boardobjgrpconstruct_e255_exit;

	pboardobjgrp_e255->super.pmuhdrdatainit =
		boardobjgrp_pmuhdrdatainit_e255;

boardobjgrpconstruct_e255_exit:
	return status;
}

u32 boardobjgrp_pmuhdrdatainit_e255(struct gk20a *g,
		struct boardobjgrp *pboardobjgrp,
		struct nv_pmu_boardobjgrp_super *pboardobjgrppmu,
		struct boardobjgrpmask *mask)
{
	struct nv_pmu_boardobjgrp_e255 *pgrpe255 =
		(struct nv_pmu_boardobjgrp_e255 *)pboardobjgrppmu;
	u32 status;

	gk20a_dbg_info("");

	if (pboardobjgrp == NULL)
		return -EINVAL;

	if (pboardobjgrppmu == NULL)
		return -EINVAL;

	status = boardobjgrpmask_export(mask,
				mask->bitcount,
				&pgrpe255->obj_mask.super);
	if (status) {
		nvgpu_err(g, "e255 init:failed export grpmask");
		return status;
	}

	return boardobjgrp_pmuhdrdatainit_super(g,
			pboardobjgrp, pboardobjgrppmu, mask);
}
