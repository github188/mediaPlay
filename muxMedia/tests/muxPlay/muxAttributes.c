
#include "libRx.h"

HI_S32 muxSetAttr(MUXLAB_PLAY_T *mux, HI_CHAR *pszCmd)
{
	HI_S32 s32Ret = 0;
	HI_HANDLE hdl = 0;
	HI_U32 u32Tmp = 0;
	HI_SVR_PLAYER_SYNC_ATTR_S stSyncAttr;
	HI_UNF_WINDOW_ATTR_S stWinAttr;
	HI_SVR_PLAYER_STREAMID_S stStreamId;
	HI_UNF_DISP_ASPECT_RATIO_S stAspectRatio;
	HI_UNF_SND_GAIN_ATTR_S stGain;

	if (NULL == pszCmd)
		return HI_FAILURE;

	if (0 == strncmp("sync", pszCmd, 4))
	{
		if (3 != sscanf(pszCmd, "sync %d %d %d", 
			&stSyncAttr.s32VidFrameOffset, &stSyncAttr.s32AudFrameOffset, &stSyncAttr.s32SubTitleOffset))
		{
			PRINTF("ERR: input err, example: set sync vidptsadjust audptsadjust, subptsadjust! \n");
			return HI_FAILURE;
		}

		s32Ret = HI_SVR_PLAYER_SetParam(mux->playerHandler, HI_SVR_PLAYER_ATTR_SYNC, (HI_VOID*)&stSyncAttr);
	}
	else if (0 == strncmp("vol", pszCmd, 3))
	{
#if (USE_EXTERNAL_AVPLAY == 1)
		if ((HI_HANDLE)INVALID_TRACK_HDL == mux->trackHandle)
		{
			PRINTF("ERR: audio track handle is invalid! \n");
			return HI_FAILURE;
		}
		hdl = mux->trackHandle;
#else
		s32Ret = HI_SVR_PLAYER_GetParam( mux->playerHandler, HI_SVR_PLAYER_ATTR_AUDTRACK_HDL, &hdl);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("ERR: get audio track hdl fail! \n");
			return HI_FAILURE;
		}
#endif

		if (1 != sscanf(pszCmd, "vol %d", &u32Tmp))
		{
			PRINTF("ERR: not input volume! \n");
			return HI_FAILURE;
		}
		stGain.bLinearMode = HI_TRUE;
		stGain.s32Gain = u32Tmp;

		s32Ret = HI_UNF_SND_SetTrackWeight(hdl, &stGain);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("ERR: set volume failed! \n");
			return HI_FAILURE;
		}
	}
	else if (0 == strncmp("track", pszCmd, 5))
	{
		if (1 != sscanf(pszCmd, "track %d", &u32Tmp))
		{
			PRINTF("ERR: not input track mode! \n");
			return HI_FAILURE;
		}

		s32Ret = HI_UNF_SND_SetTrackMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL,u32Tmp);
	}
	else if (0 == strncmp("mute", pszCmd, 4))
	{
		if (1 != sscanf(pszCmd, "mute %d", &u32Tmp))
		{
			PRINTF("ERR: not input mute val! \n");
			return HI_FAILURE;
		}

		s32Ret = HI_UNF_SND_SetMute(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL,u32Tmp);
	}
	else if (0 == strncmp("pos", pszCmd, 3))
	{
		s32Ret = HI_SVR_PLAYER_GetParam( mux->playerHandler, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &hdl);
		s32Ret |= HI_UNF_VO_GetWindowAttr(hdl, &stWinAttr);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("ERR: get win attr fail! \n");
			return HI_FAILURE;
		}

		if (4 != sscanf(pszCmd, "pos %d %d %d %d",
			&stWinAttr.stOutputRect.s32X,
			&stWinAttr.stOutputRect.s32Y,
			&stWinAttr.stOutputRect.s32Width,
			&stWinAttr.stOutputRect.s32Height))
		{
			PRINTF("ERR: input err, example: set pos x y width height! \n");
			return HI_FAILURE;
		}

		s32Ret = HI_UNF_VO_SetWindowAttr(hdl, &stWinAttr);
	}
	else if (0 == strncmp("aspect", pszCmd, 6))
	{
		s32Ret = HI_SVR_PLAYER_GetParam(mux->playerHandler, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &hdl);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("ERR: get win attr fail! \n");
			return HI_FAILURE;
		}

		if (1 != sscanf(pszCmd, "aspect %d", &u32Tmp))
		{
			PRINTF("ERR: not input aspectration val! \n");
			return HI_FAILURE;
		}
		stAspectRatio.enDispAspectRatio = (HI_UNF_DISP_ASPECT_RATIO_E)u32Tmp;
		if (stAspectRatio.enDispAspectRatio== HI_UNF_DISP_ASPECT_RATIO_USER)
		{
			PRINTF("ERR: set aspect ratio fail, use user-aspect instead.\n");
			return HI_FAILURE;
		}
		s32Ret = HI_UNF_DISP_SetAspectRatio(hdl, &stAspectRatio);
		if (s32Ret != HI_SUCCESS)
		{
			PRINTF("ERR: set aspect ratio fail.\n");
			return HI_FAILURE;
		}
	}
	else if (0 == strncmp("user-aspect", pszCmd, 11))
	{
		s32Ret = HI_SVR_PLAYER_GetParam(mux->playerHandler, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &hdl);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("ERR: get win attr fail! \n");
			return HI_FAILURE;
		}

		if (3 != sscanf(pszCmd, "user-aspect %u %u %u", &u32Tmp,
			&stAspectRatio.u32UserAspectWidth, &stAspectRatio.u32UserAspectHeight))
		{
			PRINTF("ERR: not input user-aspectration vals! \n");
			return HI_FAILURE;
		}
		
		stAspectRatio.enDispAspectRatio = (HI_UNF_DISP_ASPECT_RATIO_E)u32Tmp;
		if (stAspectRatio.enDispAspectRatio != HI_UNF_DISP_ASPECT_RATIO_USER)
		{
			PRINTF("ERR: set user-aspect ratio fail, use aspect instead.\n");
			return HI_FAILURE;
		}
		
		s32Ret = HI_UNF_DISP_SetAspectRatio(hdl, &stAspectRatio);
		if (s32Ret != HI_SUCCESS)
		{
			PRINTF("ERR: set user-aspect ratio fail.\n");
			return HI_FAILURE;
		}
	}
	else if (0 == strncmp("zorder", pszCmd, 6))
	{
		s32Ret = HI_SVR_PLAYER_GetParam(mux->playerHandler, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &hdl);

		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("ERR: get win hdl fail! \n");
			return HI_FAILURE;
		}

		if (1 != sscanf(pszCmd, "zorder %d", &u32Tmp))
		{
			PRINTF("ERR: not input zorder val! \n");
			return HI_FAILURE;
		}

		s32Ret = HI_UNF_VO_SetWindowZorder(hdl, u32Tmp);
	}
	else if (0 == strncmp("id", pszCmd, 2))
	{
		HI_FORMAT_FILE_INFO_S *pstFileInfo = NULL;

		s32Ret = HI_SVR_PLAYER_GetFileInfo(mux->playerHandler, &pstFileInfo);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("ERR: get file info fail! \n");
			return HI_FAILURE;
		}

		if (4 != sscanf(pszCmd, "id %hu %hu %hu %hu",
			&stStreamId.u16ProgramId,
			&stStreamId.u16VidStreamId,
			&stStreamId.u16AudStreamId,
			&stStreamId.u16SubStreamId))
		{
			PRINTF("ERR: input err, example: set id 0 0 1 0, set id prgid videoid audioid subtitleid! \n");
			return HI_FAILURE;
		}

		if (stStreamId.u16ProgramId >= pstFileInfo->u32ProgramNum ||
			(stStreamId.u16VidStreamId != 0 &&
			stStreamId.u16VidStreamId >= pstFileInfo->pastProgramInfo[stStreamId.u16ProgramId].u32VidStreamNum) ||
			(stStreamId.u16AudStreamId != 0 &&
			stStreamId.u16AudStreamId >= pstFileInfo->pastProgramInfo[stStreamId.u16ProgramId].u32AudStreamNum) ||
			(stStreamId.u16SubStreamId != 0 &&
			stStreamId.u16SubStreamId >= pstFileInfo->pastProgramInfo[stStreamId.u16ProgramId].u32SubStreamNum))
		{
			PRINTF("invalid stream id\n");
			return HI_FAILURE;
		}

		s32Ret = HI_SVR_PLAYER_SetParam(mux->playerHandler, HI_SVR_PLAYER_ATTR_STREAMID, (HI_VOID*)&stStreamId);

		if (HI_SUCCESS == s32Ret &&
			pstFileInfo->u32ProgramNum > 0 &&
			pstFileInfo->pastProgramInfo[stStreamId.u16ProgramId].u32SubStreamNum > 0)
		{
			if (pstFileInfo->pastProgramInfo[stStreamId.u16ProgramId].pastSubStream[stStreamId.u16SubStreamId].u32Format ==
				HI_FORMAT_SUBTITLE_HDMV_PGS)
			{
				mux->s_bPgs = HI_TRUE;
			}
			else
			{
				mux->s_s64CurPgsClearTime = -1;
				mux->s_bPgs = HI_FALSE;
			}
		}
	}
	else if (0 == strncmp("vmode", pszCmd, 5))
	{
		if (1 != sscanf(pszCmd, "vmode %u", &u32Tmp))
		{
			PRINTF("no input val for set video mode!(%d-2d,%d-framepacking,%d-sbs,%d-tab...) \n",
				HI_UNF_DISP_3D_NONE, HI_UNF_DISP_3D_FRAME_PACKING, HI_UNF_DISP_3D_SIDE_BY_SIDE_HALF,
				HI_UNF_DISP_3D_TOP_AND_BOTTOM);
			PRINTF("for example:set vmode 1\n");
			return HI_FAILURE;
		}
		else if (u32Tmp >= HI_UNF_DISP_3D_BUTT)
		{
			PRINTF("invalid video mode (%u),should < %d\n", u32Tmp,  HI_UNF_DISP_3D_BUTT);
		}

		muxSetVideoMode(mux, u32Tmp);
	}
	else if (0 == strncmp("hdmi", pszCmd, 4))
	{
		static int hdmi_toggle =0;
		hdmi_toggle++;
		if(hdmi_toggle&1)
		{
			HI_UNF_SND_SetHdmiMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_RAW);
			printf("hmdi pass-through on!\n");
		}
		else
		{
			HI_UNF_SND_SetHdmiMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_LPCM);
			printf("hmdi pass-through off!\n");
		}

	}
	else if (0 == strncmp("spdif", pszCmd, 5))
	{
		static int spdif_toggle = 0;
		spdif_toggle++;
		if (spdif_toggle & 1)
		{
			HI_UNF_SND_SetSpdifMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_RAW);
			printf("spdif pass-through on!\n");
		}
		else
		{
			HI_UNF_SND_SetSpdifMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_LPCM);
			printf("spdif pass-through off!\n");
		}
	}
	else
	{
		PRINTF("ERR: not support commond! \n");
	}

	return HI_SUCCESS;
}

HI_S32 muxGetAttr(MUXLAB_PLAY_T *mux, HI_CHAR *pszCmd)
{
	HI_S32 s32Ret = 0;
	HI_S64 s64Tmp = 0;
	HI_CHAR cTmp[512];
	HI_FORMAT_BUFFER_CONFIG_S stBufConfig;
	HI_FORMAT_BUFFER_STATUS_S stBufStatus;

	if (NULL == pszCmd)
		return HI_FAILURE;

	if (0 == strncmp("bandwidth", pszCmd, 9))
	{
		s32Ret = HI_SVR_PLAYER_Invoke(mux->playerHandler, HI_FORMAT_INVOKE_GET_BANDWIDTH, (HI_VOID*)&s64Tmp);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("get bandwidth fail! \n");
			return HI_FAILURE;
		}

		PRINTF("get bandwidth is %lld bps\n", s64Tmp);
		PRINTF("get bandwidth is %lld K bps\n", s64Tmp/1024);
		PRINTF("get bandwidth is %lld KBps\n", s64Tmp/8192);
	}
	else if (0 == strncmp("bufferconfig", pszCmd, 12))
	{
		memset(&stBufConfig, 0, sizeof(HI_FORMAT_BUFFER_CONFIG_S));

		if (1 != sscanf(pszCmd, "bufferconfig %s", cTmp))
		{
			PRINTF("ERR: not input bufferconfig type! \n");
			return HI_FAILURE;
		}

		if (0 == strcmp(cTmp,"size"))
		{
			stBufConfig.eType = HI_FORMAT_BUFFER_CONFIG_SIZE;
		}
		else if (0 == strcmp(cTmp, "time"))
		{
		stBufConfig.eType = HI_FORMAT_BUFFER_CONFIG_TIME;
		}
		else
		{
			PRINTF("ERR: do not know bufferconfig type(%s)! \n",cTmp);
			return HI_FAILURE;
		}

		s32Ret = HI_SVR_PLAYER_Invoke(mux->playerHandler, HI_FORMAT_INVOKE_GET_BUFFER_CONFIG, (HI_VOID*)&stBufConfig);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("get bufferconfig fail! \n");
			return HI_FAILURE;
		}
		PRINTF("BufferConfig:type(%d):%s\n",stBufConfig.eType, cTmp);
		PRINTF("s64EventStart:%lld\n", stBufConfig.s64EventStart);
		PRINTF("s64EventEnough:%lld\n", stBufConfig.s64EventEnough);
		PRINTF("s64Total:%lld\n", stBufConfig.s64Total);
		PRINTF("s64TimeOut:%lld\n", stBufConfig.s64TimeOut);
	}
	else if (0 == strncmp("bufferstatus", pszCmd, 12))
	{
		s32Ret = HI_SVR_PLAYER_Invoke(mux->playerHandler, HI_FORMAT_INVOKE_GET_BUFFER_STATUS, (HI_VOID*)&stBufStatus);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("get Buffer status fail! \n");
			return HI_FAILURE;
		}
		PRINTF("BufferStatus:\n");
		PRINTF("s64Duration:%lld ms\n", stBufStatus.s64Duration);
		PRINTF("s64BufferSize:%lld bytes\n", stBufStatus.s64BufferSize);
	}
	else if (0 == strncmp("hlsinfo", pszCmd, 7))
	{
		HI_FORMAT_HLS_STREAM_INFO_S stHlsInfo;

		if (1 != sscanf(pszCmd, "hlsinfo %d", &stHlsInfo.stream_nb) ||stHlsInfo.stream_nb < 0)
		{
			PRINTF("ERR: input err, example: get hlsinfo 0\n");
			return HI_FAILURE;
		}

		s32Ret = HI_SVR_PLAYER_Invoke(mux->playerHandler, HI_FORMAT_INVOKE_GET_HLS_STREAM_INFO, (HI_VOID*)&stHlsInfo);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("get HLS streaminfo fail! \n");
			return HI_FAILURE;
		}

		PRINTF("hls streamindex:%d\n", stHlsInfo.stream_nb);
		PRINTF("hls segments number in current stream:%d\n", stHlsInfo.hls_segment_nb);
		PRINTF("hls stream url:%s\n", stHlsInfo.url);
	}
	else if (0 == strncmp("hlsseg", pszCmd, 6))
	{
		HI_FORMAT_HLS_SEGMENT_INFO_S stHlsSegInfo;

		s32Ret = HI_SVR_PLAYER_Invoke(mux->playerHandler, HI_FORMAT_INVOKE_GET_HLS_SEGMENT_INFO, (HI_VOID*)&stHlsSegInfo);
		if (HI_SUCCESS != s32Ret)
		{
			PRINTF("get HLS segment info fail! \n");
			return HI_FAILURE;
		}

		PRINTF("hls stream of current segment:%d\n", stHlsSegInfo.stream_num);
		PRINTF("hls current segment duration :%d\n", stHlsSegInfo.total_time);
		PRINTF("hls index of current segment :%d\n", stHlsSegInfo.seq_num);
		PRINTF("hls bandwidth required       :%lld\n", stHlsSegInfo.bandwidth);
		PRINTF("hls current segment url      :%s\n", stHlsSegInfo.url);

	}
	else
	{
		PRINTF("ERR: not support command! \n");
	}

	return HI_SUCCESS;
}


