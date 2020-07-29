#include "stdafx.h"
#include "Emu/Cell/PPUModule.h"
#include "Emu/IdManager.h"

#include "sceNpCommerce2.h"
#include "sceNp.h"
#include "cellSysutil.h"

#include "Emu/NP/np_handler.h"

LOG_CHANNEL(sceNpCommerce2);

s32 sceNpCommerce2ExecuteStoreBrowse()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetStoreBrowseUserdata()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2Init()
{
	sceNpCommerce2.warning("sceNpCommerce2Init()");

	return CELL_OK;
}

s32 sceNpCommerce2Term()
{
	sceNpCommerce2.warning("sceNpCommerce2Term()");

	return CELL_OK;
}

error_code sceNpCommerce2CreateCtx(u32 version, vm::cptr<SceNpId> npId, vm::ptr<SceNpCommerce2Handler> handler, vm::ptr<void> arg, vm::ptr<u32> ctx_id)
{
	sceNp2.warning("sceNpCommerce2CreateCtx(version=%d, npId=*0x%x, handler=*0x%x, arg=*0x%x, ctx_id=*0x%x)", version, npId, handler, arg, ctx_id);
	const auto nph = g_fxo->get<named_thread<np_handler>>();

	*ctx_id = nph->create_commerce2_context(version, npId, handler, arg);

	return CELL_OK;
}

s32 sceNpCommerce2DestroyCtx(u32 ctx_id)
{
	sceNp2.warning("sceNpCommerce2DestroyCtx(ctx_id=%d)", ctx_id);
	const auto nph = g_fxo->get<named_thread<np_handler>>();

	nph->destroy_commerce2_context(ctx_id);
	
	return CELL_OK;
}

s32 sceNpCommerce2EmptyStoreCheckStart(u32 ctx_id, u32 store_check_type, vm::cptr<char> target_id)
{
	sceNp2.warning("sceNpCommerce2EmptyStoreCheckStart(ctx_id=%d, store_check_type=%d, target_id=*0x%x(%s))", ctx_id, store_check_type, target_id, target_id);

	const auto nph = g_fxo->get<named_thread<np_handler>>();

	const auto ctx = nph->get_commerce2_context(ctx_id);

	if (ctx->context_callback)
	{
		sysutil_register_cb([=](ppu_thread& cb_ppu) -> s32 {
			ctx->context_callback(cb_ppu, ctx_id, 0, SCE_NP_COMMERCE2_EVENT_EMPTY_STORE_CHECK_DONE, 0, ctx->context_callback_param);
			return 0;
		});
	}

	return CELL_OK;
}

s32 sceNpCommerce2EmptyStoreCheckAbort()
{
	sceNpCommerce2.todo("sceNpCommerce2EmptyStoreCheckAbort()");
	return CELL_OK;
}

s32 sceNpCommerce2EmptyStoreCheckFinish(u32 ctx_id, vm::ptr<s32> is_empty)
{
	sceNp2.warning("sceNpCommerce2EmptyStoreCheckFinish(ctx_id=%d, is_empty=*0x%x)", ctx_id, is_empty);
	*is_empty = SCE_NP_COMMERCE2_STORE_IS_NOT_EMPTY;
	return CELL_OK;
}

s32 sceNpCommerce2CreateSessionStart(u32 ctx_id)
{
	sceNp2.warning("sceNpCommerce2CreateSessionStart(ctx_id=%d)", ctx_id);
	const auto nph = g_fxo->get<named_thread<np_handler>>();

	const auto ctx = nph->get_commerce2_context(ctx_id);

	if (ctx->context_callback)
	{
		sysutil_register_cb([=](ppu_thread& cb_ppu) -> s32 {
			ctx->context_callback(cb_ppu, ctx_id, 0, SCE_NP_COMMERCE2_EVENT_CREATE_SESSION_DONE, 0, ctx->context_callback_param);
			return 0;
		});
	}

	return CELL_OK;
}

s32 sceNpCommerce2CreateSessionAbort()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2CreateSessionFinish(u32 ctx_id, vm::ptr<SceNpCommerce2SessionInfo> sessionInfo)
{
	sceNp2.warning("sceNpCommerce2CreateSessionFinish(ctx_id=%d, sessionInfo=*0x%x)", ctx_id, sessionInfo);
	memset(sessionInfo.get_ptr(), 0, sizeof(sessionInfo));
	return CELL_OK;
}

s32 sceNpCommerce2GetCategoryContentsCreateReq()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetCategoryContentsStart()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetCategoryContentsGetResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2InitGetCategoryContentsResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetCategoryInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetContentInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetCategoryInfoFromContentInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetGameProductInfoFromContentInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DestroyGetCategoryContentsResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetProductInfoCreateReq()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetProductInfoStart()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetProductInfoGetResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2InitGetProductInfoResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetGameProductInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DestroyGetProductInfoResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetProductInfoListCreateReq()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetProductInfoListStart()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetProductInfoListGetResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2InitGetProductInfoListResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetGameProductInfoFromGetProductInfoListResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DestroyGetProductInfoListResult()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetContentRatingInfoFromGameProductInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetContentRatingInfoFromCategoryInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetContentRatingDescriptor()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetGameSkuInfoFromGameProductInfo()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetPrice()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoCheckoutStartAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoCheckoutFinishAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoProductBrowseStartAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoProductBrowseFinishAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoDlListStartAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoDlListFinishAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoProductCodeStartAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoProductCodeFinishAsync()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2GetBGDLAvailability()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2SetBGDLAvailability()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2AbortReq()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DestroyReq()
{
	UNIMPLEMENTED_FUNC(sceNpCommerce2);
	return CELL_OK;
}

s32 sceNpCommerce2DoServiceListStartAsync()
{
	sceNpCommerce2.todo("sceNpCommerce2DoServiceListStartAsync()");
	return CELL_OK;
}

s32 sceNpCommerce2DoServiceListFinishAsync()
{
	sceNpCommerce2.todo("sceNpCommerce2DoServiceListFinishAsync()");
	return CELL_OK;
}

DECLARE(ppu_module_manager::sceNpCommerce2)("sceNpCommerce2", []()
{
	REG_FUNC(sceNpCommerce2, sceNpCommerce2ExecuteStoreBrowse);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetStoreBrowseUserdata);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2Init);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2Term);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2CreateCtx);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DestroyCtx);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2EmptyStoreCheckStart);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2EmptyStoreCheckAbort);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2EmptyStoreCheckFinish);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2CreateSessionStart);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2CreateSessionAbort);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2CreateSessionFinish);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetCategoryContentsCreateReq);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetCategoryContentsStart);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetCategoryContentsGetResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2InitGetCategoryContentsResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetCategoryInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetContentInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetCategoryInfoFromContentInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetGameProductInfoFromContentInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DestroyGetCategoryContentsResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetProductInfoCreateReq);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetProductInfoStart);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetProductInfoGetResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2InitGetProductInfoResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetGameProductInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DestroyGetProductInfoResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetProductInfoListCreateReq);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetProductInfoListStart);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetProductInfoListGetResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2InitGetProductInfoListResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetGameProductInfoFromGetProductInfoListResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DestroyGetProductInfoListResult);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetContentRatingInfoFromGameProductInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetContentRatingInfoFromCategoryInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetContentRatingDescriptor);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetGameSkuInfoFromGameProductInfo);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetPrice);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoCheckoutStartAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoCheckoutFinishAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoProductBrowseStartAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoProductBrowseFinishAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoDlListStartAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoDlListFinishAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoProductCodeStartAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoProductCodeFinishAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2GetBGDLAvailability);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2SetBGDLAvailability);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2AbortReq);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DestroyReq);

	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoServiceListStartAsync);
	REG_FUNC(sceNpCommerce2, sceNpCommerce2DoServiceListFinishAsync);
});
