/**
 *****************************************************************************
  * Update
  * @Version : V1.0
  * @By      : dengzhiqin
  * @Date    : 2017 / 12 / 05
  * @Brief   : ITE6664 comm api
  *
  *****************************************************************************
**/
#include "includes.h"
#if ((IT666X_VER != IT666X_NONE)&&((_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)))

uint16_t g_update_rx_edid_cnt = 0;
edid_copy_from_t edid_copy_from_status = edid_copy_from_none;
const uint8_t hdmi_output_logic_index_table[] =
{
	_HDMI_OUTPUT_1_LOGIC_INDEX,
	_HDMI_OUTPUT_2_LOGIC_INDEX,
	_HDMI_OUTPUT_3_LOGIC_INDEX,
	_HDMI_OUTPUT_4_LOGIC_INDEX,
	_HDMI_OUTPUT_5_LOGIC_INDEX,
	_HDMI_OUTPUT_6_LOGIC_INDEX,
	_HDMI_OUTPUT_7_LOGIC_INDEX,
	_HDMI_OUTPUT_8_LOGIC_INDEX,
};

#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
uint16_t g_cpt_ds_manu_sw_cnt = 0;//detect signal after this time when manual switch
#endif

extern const st5VControl_t stProjectTx5VControlTable[];

extern extern_variables *gext_var;
extern extern_u8 *gext_u8;
extern iTE_u8 _CODE Default_Edid_table720p[];
extern stProjectGlobalAllVar_t g_stPrjGloVar;
extern iTE_u8 g_device;

void IT6664_MainLoop(void)
{
	it6664();
}

#if ((USING_ITE_DEMO_CODE==_MACRO_DISABLE)&&((_EN_GPIO_EDID_MODE_SWITCH==_MACRO_ENABLE)||(_EN_GPIO_EDID_SELECT_SWITCH==_MACRO_ENABLE)))
//check the edid is valid
bool IT6664_is_edid_valid(iTE_u8 port)
{
	iTE_u8 *edid_table = &g_edid_tabel_user_def[(port-1)*256];

	return is_edid_valid(edid_table);
}

//Copy edid from hdmi output port
bool it6664_my_Edid_Copy_Port(uint8_t iport)
{
	iTE_u8 tmp[128], ret=0,ret1=0, blk1, sum, device, port;
	bool ret_edid0=FALSE, ret_edid1=FALSE;
	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	iTE_u8 i;
	#endif

	#if _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8
	if((iport/4)>0) device = IT6664_B;
	else device = IT6664_A;
	#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4
	if(iport < 4) device = IT6663_C;
	#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2
	if(0<iport<3) device = IT6663_C;
	#endif

	port = hdmi_output_logic_index_table[iport];

	IT6664_DeviceSelect(device);
	//if(gext_u8->EDIDCopyDone) return TRUE;

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	iTE6664_Edid_Msg(("EDID Copy Port%d \r\n",(iTE_u8)port));
	h2rxwr(0xC6, 0x00); //VSDB Start Address
	h2rxwr(0xC7, EDID1data[port].vsdb1.phyaddr0); //AB
  	h2rxwr(0xC8, EDID1data[port].vsdb1.phyaddr1); //CD
  	h2rxwr(0x4B, (RXEDID_Addr+it6664_u8I2CAdr)|0x01); //EDID SlaveAdr
	#endif

	IT6664_DeviceSelect(device);

	if(it6664_read_one_block_edid((port%4),0,tmp) == FALSE)
	{
		ret_edid0 = FALSE;
		iTE6664_Edid_Msg(("Read P%d EDID fail ... \r\n",(iTE_u16)(port%4)));
	}
	else
	{
		ret_edid0 = TRUE;
		IT6664_DeviceSelect(IT6663_C);
		sum = CalEdidCheckSum(tmp,0);
		if(sum == tmp[0x7F])
		{
			ret = it6664_EDIDCompare(tmp,Compose_block0);
			if(ret == 1)
			{
				#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
				h2rxwr(0xC9,tmp[0x7F]);//bank0  checksum
				for(i=0;i<0x7F;i++)
				{
					h2rx_edidwr(i,tmp[i]);
					Compose_block0[i] = tmp[i];
				}
				Compose_block0[0x7F] = sum;

				#else
				memcpy(Compose_block0, tmp, 128);

				#endif
			}
			blk1 = tmp[0x7E];
		}
		else
		{
			if((tmp[0x00]==0x00)&&(tmp[0x01]==0xFF)\
				&&(tmp[0x02]==0xFF)&&(tmp[0x03]==0xFF)\
				&&(tmp[0x04]==0xFF)&&(tmp[0x05]==0xFF)\
				&&(tmp[0x06]==0xFF)&&(tmp[0x07]==0x00))
				{
					#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
					h2rxwr(0xC9,sum);//block0  checksum
					for(i=0;i<0x7F;i++)
					{
						h2rx_edidwr(i,tmp[i]);
						Compose_block0[i] = tmp[i];
					}

					#else
					memcpy(Compose_block0, tmp, 128);

					#endif
					Compose_block0[0x7F] = sum;
					blk1 = tmp[0x7E];
					iTE6664_Edid_Msg(("Port%d block0 checksum update \r\n",(iTE_u16)(port%4)));
				}
				else
				{
					ret_edid0 = FALSE;
					iTE6664_Edid_Msg(("Port%d block0 invalid ! use default EDID \r\n",(iTE_u16)(port%4)));
				}
		}
	}

	IT6664_DeviceSelect(device);

	if(it6664_read_one_block_edid((port%4),1,tmp)==FALSE)
	{
		ret_edid1 = FALSE;
		iTE6664_Edid_Msg(("Read P%d EDID fail ... \r\n",(iTE_u16)(port%4)));
	}
	else
	{
		ret_edid1 = TRUE;
		IT6664_DeviceSelect(IT6663_C);
		if(blk1)
		{
			sum = CalEdidCheckSum(tmp,0);
			if(sum == tmp[0x7F])
			{
				ret1 = it6664_EDIDCompare(tmp,Compose_block1);
				if(ret1 == 1)
				{
					#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
					h2rxwr(0xCA,tmp[0x7F]);//bank1  checksum
					for(i=0;i<0x7F;i++)
					{
						h2rx_edidwr(i+0x80,tmp[i]);
						Compose_block1[i] = tmp[i];
					}
					Compose_block0[0x7F] = sum;

					#else

					memcpy(Compose_block1, tmp, 128);
					#endif
				}
			}
			else
			{
				if((tmp[0x00] == 0x02) && (tmp[0x01] == 0x03))
				{
					#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
					for(i=0;i<0x7F;i++)
					{
						h2rx_edidwr(i+0x80,tmp[i]);
						Compose_block1[i] = tmp[i];
					}
					h2rxwr(0xCA,sum);//bank1  checksum

					#else
					memcpy(Compose_block1, tmp, 128);

					#endif

					Compose_block0[0x7F] = sum;
					iTE6664_Edid_Msg(("Port%d block1 checksum update \r\n",(iTE_u8)(port%4)));
				}
			}
		}
	}

	IT6664_DeviceSelect(device);
	gext_u8->EDIDCopyDone = 1;

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	if(gext_var->DVI_mode[port])
	{
		IT6664_DeviceSelect(IT6663_C);
		chgspbank(1);
		h2spset(0x10,0x40,0x00);
		chgspbank(0);
	}
	else
	{
		IT6664_DeviceSelect(IT6663_C);
		chgspbank(1);
		h2spset(0x10,0x40,0x40);
		chgspbank(0);
	}
	#endif

	if((!ret_edid0)||(blk1&&(!ret_edid1)))
	{
		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		//add to do

		#else
		memcpy(Compose_block0, &Default_Edid_table720p[0], 128);
		memcpy(Compose_block1, &Default_Edid_table720p[128], 128);
		#endif

		return FALSE;
	}

	return TRUE;
}

void it6664_my_Default_Edid(void)
{
	iTE_u8 ret0, ret1, sum;
	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	iTE_u8 i;
	#endif

	ret0 = it6664_EDIDCompare((iTE_pu8)&Default_Edid_table720p[0],Compose_block0);
	ret1 = it6664_EDIDCompare((iTE_pu8)&Default_Edid_table720p[128],Compose_block1);

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0x34, 0x01, 0x00);	//emily 20161222 Enable ERCLK even no 5VDet

	h2rxwr(0xC6, 0x00); //VSDB Start Address
	h2rxwr(0xC7, 0x00); //AB
    h2rxwr(0xC8, 0xFF); //CD
    h2rxwr(0x4B, (RXEDID_Addr+it6664_u8I2CAdr)|0x01); //EDID SlaveAdr
	#endif

	if(ret0 == 1)
	{
		memcpy(Compose_block0, &Default_Edid_table720p[0], 128);
		sum = CalEdidCheckSum((iTE_pu8)Default_Edid_table720p,0);
		Compose_block0[0x7F] = sum;

		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		h2rxwr(0xC9,sum);//bank1  checksum
		for(i=0;i<0x7F;i++) h2rx_edidwr(i,Compose_block0[i]);
		#endif
	}

	if(ret1 == 1)
	{
		memcpy(Compose_block1, &Default_Edid_table720p[128], 128);
		sum = CalEdidCheckSum((iTE_pu8)Default_Edid_table720p,1);
		Compose_block1[0x7F] = sum;

		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		h2rxwr(0xCA,sum);//bank1  checksum
		for(i=0;i<0x7F;i++) h2rx_edidwr(i+0x80,Compose_block1[i]);
		#endif
	}

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	//HDMI mode
	IT6664_DeviceSelect(IT6663_C);
	chgspbank(1);
	h2spset(0x10,0x40,0x40);
	chgspbank(0);

	//toggle RX HPD
	h2rxset(0xC5, 0x01, 0x01);
	SetRxHpd(FALSE);
	it6664_h2rx_pwdon();
	mSleep(2000);
	SetRxHpd(TRUE);
	h2rxset(0xC5, 0x01, 0x00);

	h2rxset(0xC5, 0x01, 0x00);
	h2rxset(0x34, 0x01, 0x01);

	#else
    #if _ENABLE_EDID_SWITCH_UNREBOOT
     IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer,PortResetForEdid);
    #else
     IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer);
    #endif

	#endif
}

//Copy the lowest EDID of output port
void it6664_my_Edid_Copy_Lowest(void)
{
	iTE_u8 i, j, port_lowest=0xFF, block54[2]={0}, retry=0, *DTD, port;
	static iTE_u8 pre_port_lowest = 0xFE;
	bool copy_flag = FALSE;
	iTE_u16 u16HotPlugState = 0, tmp = 0;
	static iTE_u16 pre_u16HotPlugState = 0;

	for(j=0; j<2; j++)
	{
		#if _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8
		if(j==0)
		{
			IT6664_DeviceSelect(IT6664_A);
			Device_off = 0;
			tmp = gext_u8->TXHPDsts;
		}
		else
		{
			IT6664_DeviceSelect(IT6664_B);
			Device_off = 1;
			u16HotPlugState = gext_u8->TXHPDsts;
			u16HotPlugState <<= 8;
			u16HotPlugState += tmp;
		}
		#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4
		if(j==1) break;
		IT6664_DeviceSelect(IT6663_C);
		Device_off = 0;
		u16HotPlugState = gext_u8->TXHPDsts;
		#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2
		if(j==1) break;
		IT6664_DeviceSelect(IT6663_C);
		Device_off = 0;
		u16HotPlugState = gext_u8->TXHPDsts;
		#endif

		if(gext_u8->TXHPDsts)
		{
			for(i=0; i<TxPortNum; i++)
			{
				port = hdmi_output_logic_index_table[j*4+i];
				port %= TxPortNum;
				if((gext_u8->TXHPDsts&(1<<port)) && gext_var->EDIDParseDone[port])
				{
					DTD = EDID0data[port+(4*Device_off)].DTD;
					if(!copy_flag)
					{
						memcpy(block54, DTD, 2);
						copy_flag = TRUE;
						port_lowest = port+(4*Device_off);
					}
					else
					{
						if((DTD[1]<block54[1])||((DTD[1]==block54[1])&&(DTD[0]<block54[0])))
						{
							memcpy(block54, DTD, 2);
							port_lowest = port+(4*Device_off);
						}
					}
				}
			}
		}
	}

	if(edid_copy_from_status==edid_copy_from_lowest)
	{
		tmp = u16HotPlugState ^ pre_u16HotPlugState;
		if((tmp==0)||((tmp&u16HotPlugState)==0))//plug out
		{
			pre_u16HotPlugState = u16HotPlugState;
			return;
		}

		//plug in
		tmp &= u16HotPlugState;//which one plug in
		pre_u16HotPlugState = u16HotPlugState;

		//the lowest one always plug in
		if((tmp&(1<<(port_lowest+(port_lowest/4)*4)))==0)
		{
			if(pre_port_lowest==port_lowest)
			{
				return;
			}
		}
	}

	//if the same TV from one port to another
	if(edid_copy_from_status==edid_copy_from_lowest)
	{
		iTE_u8 pu8EdidTmp[256];

		memcpy(&pu8EdidTmp[0], Compose_block0, 128);
		memcpy(&pu8EdidTmp[128], Compose_block1, 128);

		it6664_my_Edid_Copy_Port(port_lowest);

		if((memcmp(&pu8EdidTmp[0], Compose_block0, 128)==0)&&\
			(memcmp(&pu8EdidTmp[128], Compose_block1, 128)==0))
		{
			#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
			SetRxHpd(TRUE);
			#endif

			return;
		}
	}

	edid_copy_from_status = edid_copy_from_lowest;

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0x34, 0x01, 0x00);  //emily 20161222 Enable ERCLK even no 5VDet
	#endif

	if(port_lowest < _HDMI_O_PORT_NUM)
	{
		for(retry=0; retry<IT6664_EDID_RETRY_TIMES; /*retry++*/)
		{
			iTE6664_Edid_Msg(("EDID copy lowest write try %d \r\n",retry));
			if(!it6664_my_Edid_Copy_Port(port_lowest))
			{
				retry++;
				if(retry==IT6664_EDID_RETRY_TIMES)
				{
					it6664_my_Default_Edid();
				}
			}
			else
			{
				break;
			}
		}

		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		IT6664_DeviceSelect(IT6663_C);
		//toggle RX HPD
		h2rxset(0xC5, 0x01, 0x01);
		SetRxHpd(FALSE);
		it6664_h2rx_pwdon();
		mSleep(2000);
		SetRxHpd(TRUE);
		h2rxset(0xC5, 0x01, 0x00);

		#else
         #if _ENABLE_EDID_SWITCH_UNREBOOT
          IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer,PortResetForEdid);
         #else
		  IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer);
         #endif
		#endif
	}
	else
	{
		it6664_my_Default_Edid();
	}

	pre_port_lowest = port_lowest;

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0xC5, 0x01, 0x00);
	h2rxset(0x34, 0x01, 0x01);
	#endif
}

//Copy the highest EDID of output port
void it6664_my_Edid_Copy_Highest(void)
{
	iTE_u8 i, j, port_highest=0xFF, block54[2]={0}, retry=0, *DTD, port;
	static iTE_u8 pre_port_highest = 0xFE;
	bool copy_flag = FALSE;
	iTE_u16 u16HotPlugState = 0, tmp = 0;
	static iTE_u16 pre_u16HotPlugState = 0;

	for(j=0; j<2; j++)
	{
		#if _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8
		if(j==0)
		{
			IT6664_DeviceSelect(IT6664_A);
			Device_off = 0;
			tmp = gext_u8->TXHPDsts;
		}
		else
		{
			IT6664_DeviceSelect(IT6664_B);
			Device_off = 1;
			u16HotPlugState = gext_u8->TXHPDsts;
			u16HotPlugState <<= 8;
			u16HotPlugState += tmp;
		}
		#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4
		if(j==1) break;
		IT6664_DeviceSelect(IT6663_C);
		Device_off = 0;
		u16HotPlugState = gext_u8->TXHPDsts;
		#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2
		if(j==1) break;
		IT6664_DeviceSelect(IT6663_C);
		Device_off = 0;
		u16HotPlugState = gext_u8->TXHPDsts;
		#endif

		if(gext_u8->TXHPDsts)
		{
			for(i=0; i<TxPortNum; i++)
			{
				port = hdmi_output_logic_index_table[j*4+i];
				port %= TxPortNum;
				if((gext_u8->TXHPDsts&(1<<port)) && gext_var->EDIDParseDone[port])
				{
					DTD = EDID0data[port+(4*Device_off)].DTD;
					if(!copy_flag)
					{
						memcpy(block54, DTD, 2);
						copy_flag = TRUE;
						port_highest = port+(4*Device_off);
					}
					else
					{
						if((DTD[1]>block54[1])||((DTD[1]==block54[1])&&(DTD[0]>block54[0])))
						{
							memcpy(block54, DTD, 2);
							port_highest = port+(4*Device_off);
						}
					}
				}
			}
		}
	}

	if(edid_copy_from_status==edid_copy_from_highest)
	{
		tmp = u16HotPlugState ^ pre_u16HotPlugState;
		//plug out
		if((tmp==0)||((tmp&u16HotPlugState)==0))
		{
			pre_u16HotPlugState = u16HotPlugState;
			return;
		}

		//plug in
		tmp &= u16HotPlugState;//which one plug in
		pre_u16HotPlugState = u16HotPlugState;

		//the highest one always plug in
		if((tmp&(1<<(port_highest+(port_highest/4)*4)))==0)
		{
			if(pre_port_highest == port_highest)
			{
				return;
			}
		}
	}

	//if the same TV from one port to another
	if(edid_copy_from_status==edid_copy_from_highest)
	{
		iTE_u8 pu8EdidTmp[256];

		memcpy(&pu8EdidTmp[0], Compose_block0, 128);
		memcpy(&pu8EdidTmp[128], Compose_block1, 128);

		it6664_my_Edid_Copy_Port(port_highest);

		if((memcmp(&pu8EdidTmp[0], Compose_block0, 128)==0)&&\
			(memcmp(&pu8EdidTmp[128], Compose_block1, 128)==0))
		{
			#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
			SetRxHpd(TRUE);
			#endif

			return;
		}
	}

	edid_copy_from_status = edid_copy_from_highest;

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0x34, 0x01, 0x00);	//emily 20161222 Enable ERCLK even no 5VDet
	#endif

	if(port_highest < _HDMI_O_PORT_NUM)
	{
		for(retry=0; retry<IT6664_EDID_RETRY_TIMES; /*retry++*/)
		{
			iTE6664_Edid_Msg(("EDID copy highest write try %d \r\n",retry));
			if(!it6664_my_Edid_Copy_Port(port_highest))
			{
				retry++;
				if(retry==IT6664_EDID_RETRY_TIMES)
				{
					it6664_my_Default_Edid();
				}
			}
			else
			{
				break;
			}
		}

		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		IT6664_DeviceSelect(IT6663_C);
		//toggle RX HPD
		h2rxset(0xC5, 0x01, 0x01);
		SetRxHpd(FALSE);
		it6664_h2rx_pwdon();
		mSleep(2000);
		SetRxHpd(TRUE);
		h2rxset(0xC5, 0x01, 0x00);

		#else
         #if _ENABLE_EDID_SWITCH_UNREBOOT
          IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer,PortResetForEdid);
         #else
          IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer);
         #endif
		#endif
	}
	else
	{
		it6664_my_Default_Edid();
	}

	pre_port_highest = port_highest;

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0xC5, 0x01, 0x00);
	h2rxset(0x34, 0x01, 0x01);
	#endif
}

//Copy EDID from output port
void it6664_my_Edid_Copy_Output(iTE_u8 iport)
{
	iTE_u8 retry=0, port_read = 0xFF, port;
	static iTE_u8 pre_port_read = 0xFF;
	iTE_u16 u16HotPlugState = 0, tmp = 0;
	static iTE_u16 pre_u16HotPlugState = 0;

	#if _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_8
	if((iport/4)>0) IT6664_DeviceSelect(IT6664_B);
	else IT6664_DeviceSelect(IT6664_A);
	#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_4
	IT6664_DeviceSelect(IT6663_C);
	#elif _SPLITER_TYPE_IN_OUT == _SPLITER_TYPE_1_TO_2
	IT6664_DeviceSelect(IT6663_C);
	#endif

	u16HotPlugState = gext_u8->TXHPDsts;

	port = hdmi_output_logic_index_table[iport];

	if(gext_u8->TXHPDsts)
	{
		if((gext_u8->TXHPDsts&(1<<(port%4))) && gext_var->EDIDParseDone[port%4])
		{
			port_read = iport;
		}
	}

	if(edid_copy_from_status==edid_copy_from_output)
	{
		tmp = u16HotPlugState ^ pre_u16HotPlugState;
		//plug out
		if((tmp!=0)&&((tmp&u16HotPlugState)==0))
		{
			pre_u16HotPlugState = u16HotPlugState;
			return;
		}

		//read another port
		if(tmp==0)
		{
			pre_u16HotPlugState = u16HotPlugState;

			if((port_read == pre_port_read)||(port_read == 0xFF))
			{
				return;
			}
		}
		//plug in
		else if(tmp != 0)
		{
			tmp &= u16HotPlugState;//which one plug in
			pre_u16HotPlugState = u16HotPlugState;

			//the read one always plug in
			if(port_read != 0xFF)
			{
				if((tmp&(1<<(port_read+(port_read/4)*4)))==0)
				{
					return;
				}
			}
			//if the read port have no device, no action
			else
			{
				return;
			}
		}
	}

	if(edid_copy_from_status==edid_copy_from_output)
	{
		iTE_u8 pu8EdidTmp[256];

		memcpy(&pu8EdidTmp[0], Compose_block0, 128);
		memcpy(&pu8EdidTmp[128], Compose_block1, 128);

		it6664_my_Edid_Copy_Port(port_read);

		if((memcmp(&pu8EdidTmp[0], Compose_block0, 128)==0)&&\
			(memcmp(&pu8EdidTmp[128], Compose_block1, 128)==0))
		{
			#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
			SetRxHpd(TRUE);
			#endif

			return;
		}
	}

	edid_copy_from_status = edid_copy_from_output;
	pre_port_read = port_read;

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0x34, 0x01, 0x00);	//emily 20161222 Enable ERCLK even no 5VDet
	#endif

	if(port_read < _HDMI_O_PORT_NUM)
	{
		for(retry=0; retry<IT6664_EDID_RETRY_TIMES; retry++)
		{
			iTE6664_Edid_Msg(("EDID copy lowest write try %d \r\n",retry));
			if(!it6664_my_Edid_Copy_Port(port_read))
			{
				retry++;
				if(retry==IT6664_EDID_RETRY_TIMES)
				{
					it6664_my_Default_Edid();
				}
			}
			else
			{
				break;
			}
		}

		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		IT6664_DeviceSelect(IT6663_C);
		//toggle RX HPD
		h2rxset(0xC5, 0x01, 0x01);
		SetRxHpd(FALSE);
		it6664_h2rx_pwdon();
		mSleep(2000);
		SetRxHpd(TRUE);
		h2rxset(0xC5, 0x01, 0x00);

		#else
         #if _ENABLE_EDID_SWITCH_UNREBOOT
          IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer,PortResetForEdid);
         #else
 		  IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer);
         #endif
		#endif
	}
	else
	{
		it6664_my_Default_Edid();
	}

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0xC5, 0x01, 0x00);
	h2rxset(0x34, 0x01, 0x01);
	#endif
}

//Copy EDID from eeprom table
void it6664_my_Edid_Copy_Eeprom(iTE_u8 port)
{
	iTE_u8 ret0, ret1, sum;
	static iTE_u8 copy_port = 0xFF;
	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	iTE_u8 i;
	#endif

	if((edid_copy_from_status==edid_copy_from_eeprom)&&\
		(copy_port==port))
	{
		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		SetRxHpd(TRUE);
		#endif

		return;
	}

	edid_copy_from_status = edid_copy_from_eeprom;
	copy_port = port;

	if(IT6664_is_edid_valid(port))
	{
		ret0 = it6664_EDIDCompare(&g_edid_tabel_user_def[(port-1)*256],Compose_block0);
		ret1 = it6664_EDIDCompare(&g_edid_tabel_user_def[(port-1)*256+128],Compose_block1);
		if(ret0==1) memcpy(Compose_block0, &g_edid_tabel_user_def[(port-1)*256], 128);
		if(ret1==1) memcpy(Compose_block1, &g_edid_tabel_user_def[(port-1)*256+128], 128);
	}
	else
	{
		it6664_my_Default_Edid();
		return;
	}

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	IT6664_DeviceSelect(IT6663_C);
	h2rxset(0x34, 0x01, 0x00);	//emily 20161222 Enable ERCLK even no 5VDet

	h2rxwr(0xC6, 0x00); //VSDB Start Address
	h2rxwr(0xC7, 0x00); //AB
    h2rxwr(0xC8, 0xFF); //CD
    h2rxwr(0x4B, (RXEDID_Addr+it6664_u8I2CAdr)|0x01); //EDID SlaveAdr
	#endif

	if(ret0==1)
	{
		sum = CalEdidCheckSum(Compose_block0, 0);

		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		h2rxwr(0xC9, sum);
		for(i=0; i<0x7F; i++) h2rx_edidwr(i, Compose_block0[i]);

		#else
		Compose_block0[0x7F] = sum;


#endif
	}

	if(ret1==1)
	{
		sum = CalEdidCheckSum(Compose_block1, 0);

		#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
		h2rxwr(0xCA, sum);
		for(i=0; i<0x7F; i++) h2rx_edidwr((i+0x80), Compose_block1[i]);

		#else
		Compose_block1[0x7F] = sum;

		#endif
	}

	#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
	//HDMI mode
	IT6664_DeviceSelect(IT6663_C);
	chgspbank(1);
	h2spset(0x10,0x40,0x40);
	chgspbank(0);

	//toggle RX HPD
	h2rxset(0xC5, 0x01, 0x01);
	SetRxHpd(FALSE);
	it6664_h2rx_pwdon();
	mSleep(2000);
	SetRxHpd(TRUE);
	h2rxset(0xC5, 0x01, 0x00);

	h2rxset(0xC5, 0x01, 0x00);
	h2rxset(0x34, 0x01, 0x01);

	#else
     #if _ENABLE_EDID_SWITCH_UNREBOOT
      IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer,PortResetForEdid);
     #else
	  IT663XX_Reset_for_Edid(eIT663xxGetEdidFrom_Buffer);
     #endif
	#endif
}
#endif//#if (USING_ITE_DEMO_CODE == _MACRO_DISABLE)

#if( Project_SUH4E_H2_6664_DIP_SWITCH_DectTime == _MACRO_ENABLE)
volatile iTE_u32 		g_vu32DIPDectTimeCnt = 0;//
void SetDIPDetectTime(uint32_t u32SetTime)
{
	g_vu32DIPDectTimeCnt = u32SetTime;
}

void DIPDetectTime1MsLoop(void)
{
	if(g_vu32DIPDectTimeCnt > 0)
	{
		g_vu32DIPDectTimeCnt --;
	}
}

#endif
void it6664_edid_update(void)
{
	uint8_t i;
#if( Project_SUH4E_H2_6664_DIP_SWITCH_DectTime == _MACRO_DISABLE)
	if(g_update_rx_edid_cnt > 0)
	{
		g_update_rx_edid_cnt --;
		if(g_update_rx_edid_cnt == 0)
		{
			#if (USING_1to8==_MACRO_ENABLE)
			for(i=0; i<8; i++)
			{
				HPD[i] = 0;
			}
            #elif (_ENABLE_IT666X_MULTI_CHIP_USE == _MACRO_ENABLE)
            {
                u8 j;
                for(j=0;j<NUM_OF_IT666X;j++)
                {
                    for(i=0; i<4; i++)
        			{
        				HPD[j][i] = 0;
        			}
                }
            }
			#else
			for(i=0; i<4; i++)
			{
				HPD[i] = 0;
			}
			#endif
			it6664_EdidMode_Switch();
		}
	}
#else
static bool first_0_in = FALSE;
	if(g_vu32DIPDectTimeCnt>0)
	{
		first_0_in = TRUE;
		//RS232_printf("g_vu32DIPDectTimeCnt = %d\r\n",g_vu32DIPDectTimeCnt);
	}
	else
	{
		if(first_0_in)
		{
			first_0_in = FALSE;
			#if (USING_1to8==_MACRO_ENABLE)
			for(i=0; i<8; i++)
			{
				HPD[i] = 0;
			}
			#else
			for(i=0; i<4; i++)
			{
				HPD[i] = 0;
			}
			#endif
			//RS232_printf("it6664_EdidMode_Switch\r\n");
			it6664_EdidMode_Switch();
		}
	}
#endif

#if _ENABLE_IT666X_READ_TX_EDID
    it666X_ReadTxEdidToBuffer();
#endif

}

void it6664_led_work(void)
{
    #if (_SPLITER_TYPE_IN_OUT!=_SPLITER_TYPE_1_TO_24)
	iTE_u8 i;
    #endif
	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	bool bPowerOnTv = FALSE;
	static bool bPowerOnTvPre = FALSE;
	#endif

	#if (_SPLITER_TYPE_IN_OUT==_SPLITER_TYPE_1_TO_8)
	iTE_u8 j;

	for(j=0; j<2; j++)
	{
		if(j==0)IT6664_DeviceSelect(IT6664_A);
		else IT6664_DeviceSelect(IT6664_B);

		for(i=0; i<4; i++)
		{
            #if 0
			if(gext_u8->TXHPDsts & (1<<i))
			{
				led_on((led_index_name_t)(led_name_hdmi_output_1+i+j*4));
			}
			else
			{
				led_off((led_index_name_t)(led_name_hdmi_output_1+i+j*4));
			}
            #endif

			#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
			if(g_cpt_ds_manu_sw_cnt == 0)
			{
				if((gext_u8->TxTMDSStbFlag & (1<<i))&&(g_bCecOnTVDetectedNewSignal))
				{
					g_bCecOnTVDetectedNewSignal = FALSE;
					bPowerOnTv = TRUE;
				}
			}
			#endif
		}
	}

	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	if(g_cpt_ds_manu_sw_cnt == 0)
	{
		if((bPowerOnTv)&&(!bPowerOnTvPre))
		{
			//send cec cmd turn on tv
			#if (_EN_STM32_GPIO_CEC_OUTPUT != _MACRO_DISABLE)
			cec_cmd_tv_poweron(FALSE);
			#endif
		}
		bPowerOnTvPre = bPowerOnTv;
	}
	#endif

	#elif (_SPLITER_TYPE_IN_OUT==_SPLITER_TYPE_1_TO_4)

	IT6664_DeviceSelect(IT6663_C);
	for(i=0; i<4; i++)
	{


	#if Project_SUH4E_H2_LED_Crycle_Set
		if(gext_u8->TXHPDsts & (1<<i))
		{
			led_on((_EN_GPIO_HDMI_INPUT_NUM_LED+i));
		}
		else
		{
			led_off((_EN_GPIO_HDMI_INPUT_NUM_LED+i));
		}

	#else
        #if 0
		if(gext_u8->TXHPDsts & (1<<i))
		{
			led_on((led_index_name_t)(led_name_hdmi_output_1+i));
		}
		else
		{
			led_off((led_index_name_t)(led_name_hdmi_output_1+i));
		}
        #endif
	#endif
		#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
		if(g_cpt_ds_manu_sw_cnt == 0)
		{
			if((gext_u8->TxTMDSStbFlag & (1<<i))&&(g_bCecOnTVDetectedNewSignal))
			{
				g_bCecOnTVDetectedNewSignal = FALSE;
				bPowerOnTv = TRUE;
			}
		}
		#endif
	}

	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	if(g_cpt_ds_manu_sw_cnt == 0)
	{
		if((bPowerOnTv)&&(!bPowerOnTvPre))
		{
			//send cec cmd turn on tv
			#if (_EN_STM32_GPIO_CEC_OUTPUT != _MACRO_DISABLE)
			cec_cmd_tv_poweron(FALSE);
			#endif
		}
		bPowerOnTvPre = bPowerOnTv;
	}
	#endif

	#elif (_SPLITER_TYPE_IN_OUT==_SPLITER_TYPE_1_TO_2)

	IT6664_DeviceSelect(IT6663_C);
	#if Project_SUH2E_H2_LED_Crycle_Set
	for(i=1; i<3; i++)
	{
		if(gext_u8->TXHPDsts & (1<<i))
		{
			led_on((_EN_GPIO_HDMI_INPUT_NUM_LED+i-1));
		}
		else
		{
			led_off((_EN_GPIO_HDMI_INPUT_NUM_LED+i-1));
		}
	}
	#else
	for(i=0; i<2; i++)
	{
	#if(Project_INT_AS2H1U_TX_LED_Crycle_Set == _MACRO_ENABLE)
	#else
        #if 0
		if(gext_u8->TXHPDsts & (1<<i))
		{
			led_on((led_index_name_t)(led_name_hdmi_output_1+i));
		}
		else
		{
			led_off((led_index_name_t)(led_name_hdmi_output_1+i));
		}
        #endif
	#endif
		#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
		if(g_cpt_ds_manu_sw_cnt == 0)
		{
			if((gext_u8->TxTMDSStbFlag & (1<<i))&&(g_bCecOnTVDetectedNewSignal))
			{
				g_bCecOnTVDetectedNewSignal = FALSE;
				bPowerOnTv = TRUE;
			}
		}
		#endif
	}
	#endif

	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	if(g_cpt_ds_manu_sw_cnt == 0)
	{
		if((bPowerOnTv)&&(!bPowerOnTvPre))
		{
			//send cec cmd turn on tv
			#if (_EN_STM32_GPIO_CEC_OUTPUT != _MACRO_DISABLE)
			cec_cmd_tv_poweron(FALSE);
			#endif
		}
		bPowerOnTvPre = bPowerOnTv;
	}
	#endif

	#endif

	#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
	if(g_cpt_ds_manu_sw_cnt > 0)
	{
		g_cpt_ds_manu_sw_cnt --;
	}
	#endif
}

void IT6664_SetTx5VOnOff(bool on_flag)
{
	pst5VControl_t p5vCtlPortN,p5vCtlPort = (pst5VControl_t)stProjectTx5VControlTable;//pstControlPort
	p5vCtlPortN = p5vCtlPort;
	while(p5vCtlPortN->pstControlPort != NULL)
    {
        if(p5vCtlPortN->pstControlPort->eInitLevel == _GPIO_PIN_LEVEL_LOW)
        {
            if(IS_MCU_GPIO(p5vCtlPortN->pstControlPort->u8PortType))
            {
                if(on_flag){
                    GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPortN->pstControlPort->u8PortType-PORT_GPIOA],
                    p5vCtlPortN->pstControlPort->u16GpioPin);
                }
                else{
                    GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPortN->pstControlPort->u8PortType-PORT_GPIOA],
                    p5vCtlPortN->pstControlPort->u16GpioPin);
                }
            }
        }
        else
        {
            if(IS_MCU_GPIO(p5vCtlPortN->pstControlPort->u8PortType))
            {
                if(on_flag){
                    GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPortN->pstControlPort->u8PortType-PORT_GPIOA],
                    p5vCtlPortN->pstControlPort->u16GpioPin);
                }
                else{
                    GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPortN->pstControlPort->u8PortType-PORT_GPIOA],
                    p5vCtlPortN->pstControlPort->u16GpioPin);
                }
            }
        }
        p5vCtlPortN++;
    }

}

#if _ENABLE_IT666X_READ_TX_EDID
static u8  g_pu8It666xTxEdidBuffer[4][256];
static bool sg_bEnableIt666xTxEdidFlag[4] = {FALSE,FALSE,FALSE,FALSE};
static bool sg_bTxIt666xHpdSta[4] = {FALSE,FALSE,FALSE,FALSE};

pu8It666xGetHdcpSta pIt666xGetHdcpStaFunc = NULL;

extern iTE_u1 it6664_GetTxHpd(u8 u8Port);
void it666X_ReadTxEdidToBuffer(void)
{
    static u8 sl_u8PreHpdSta[4] = {0xFF,0xFF,0xFF,0xFF};
    static u8 sl_u8DelayCnt = 50;
    u8 l_u8CurHpdSta;
    u8 l_u8Port;
    //u8 l_pu8EdidBuf[256];

    if(sl_u8DelayCnt>0) // unit 50ms
    {
        sl_u8DelayCnt--;
        return;
    }
    sl_u8DelayCnt = 10;

    for(l_u8Port=0;l_u8Port<4;l_u8Port++)
    {
        #ifdef IT6663
        if(l_u8Port==0||l_u8Port==3)
            continue;
        #endif
        l_u8CurHpdSta=it6664_GetTxHpd(l_u8Port);
        if(l_u8CurHpdSta!=sl_u8PreHpdSta[l_u8Port])
        {
            sg_bTxIt666xHpdSta[l_u8Port] = (bool)l_u8CurHpdSta;
            if(l_u8CurHpdSta)
            {
                //memset(l_pu8EdidBuf,0,256);
                /*if(it6664_read_one_block_edid(l_u8Port,0,l_pu8EdidBuf))
                {
                    if(l_pu8EdidBuf[0x7E] != 0)
                    {
                        it6664_read_one_block_edid(l_u8Port,1,l_pu8EdidBuf+128);
                    }
                }
                if((is_edid_valid(l_pu8EdidBuf))&&(!is_edid_compare_same(g_pu8It666xTxEdidBuffer[l_u8Port],l_pu8EdidBuf)))
                {
                    memcpy(g_pu8It666xTxEdidBuffer[l_u8Port],l_pu8EdidBuf,256);
                    it666X_SetReadTxEdidFlag(l_u8Port,TRUE);
                }*/
                if(pIt666xGetHdcpStaFunc!=NULL)
                {
                    #if _ENABLE_IT666X_FORCE_SET_OUTPUT_HDCP
                    it666X_SetHdcpMode(l_u8Port,pIt666xGetHdcpStaFunc(l_u8Port));
                    #endif
                }
            }
            else
            {
                if(pIt666xGetHdcpStaFunc!=NULL)
                {
                    #if _ENABLE_IT666X_FORCE_SET_OUTPUT_HDCP
                    it666X_SetHdcpMode(l_u8Port,tx_hdcp_force_off);
                    #endif
                }
            }
            sl_u8PreHpdSta[l_u8Port] = l_u8CurHpdSta;
        }
    }
}

void it666X_SetReadTxEdidFlag(u8 u8Port,bool bEn)
{
    sg_bEnableIt666xTxEdidFlag[u8Port] = bEn;
}

bool it666X_GetReadTxEdidFlag(u8 u8Port)
{
    return sg_bEnableIt666xTxEdidFlag[u8Port];
}

void it666X_GetTxPortEdid(u8 u8Port,u16 u16Len,u8 *pu8Edid)
{
    #if 1
    memcpy(pu8Edid,g_pu8It666xTxEdidBuffer[u8Port],u16Len);
    #else
    if(it6664_read_one_block_edid(u8Port,0,pu8Edid))
    {
        if((pu8Edid[0x7E] != 0)&&u16Len>128)
        {
            it6664_read_one_block_edid(u8Port,1,pu8Edid+128);
        }
    }
    #endif
}

bool it666X_GetTxHpdSta(u8 u8TxPort)
{
    return sg_bTxIt666xHpdSta[u8TxPort];
}

void it666x_TxEdidCopy(u8 u8TxPort,u8 *pu8EdidBlk0,u8 *pu8EdidBlk1)
{
    u8 l_pu8EdidBuf[256];

    memset(l_pu8EdidBuf,0,256);
    memcpy(l_pu8EdidBuf,pu8EdidBlk0,128);
    memcpy(l_pu8EdidBuf+128,pu8EdidBlk1,128);
    if((is_edid_valid(l_pu8EdidBuf))&&(!is_edid_compare_same(g_pu8It666xTxEdidBuffer[u8TxPort],l_pu8EdidBuf)))
    {
        memcpy(g_pu8It666xTxEdidBuffer[u8TxPort],l_pu8EdidBuf,256);
        it666X_SetReadTxEdidFlag(u8TxPort,TRUE);
    }
}
#endif

#if _ENABLE_IT666X_FORCE_SET_OUTPUT_HDCP
void it666X_SetHdcpMode(u8 u8TxPort,u8 u8HdcpMode)
{
    #if (HDCP_HANDLE_FOR_AUDIO_BEFORE_VIDEO == _MACRO_DISABLE)
    it6664_tx_hdcp_type = (tx_hdcp_force_t)u8HdcpMode;
    UNUSED(u8TxPort);
    #else
    gext_var->TXHDCPModeCur[u8TxPort] = u8HdcpMode;
	gext_u8->CP_Done &= ~(1<<u8TxPort);
    #endif
}
#endif

#if _ENABLE_IT666X_MANUAL_TO_TX_VIDEO_RESET
extern unsigned char g_current_system_state;
#if USING_1to8 == _MACRO_ENABLE
extern int Device[3];
#elif _ENABLE_IT666X_MULTI_CHIP_USE
extern int Device[NUM_OF_IT666X];
#else
extern int Device[1];
#endif

void it666X_SetVideoReset(u8 u8DevId)
{
    #if 1
    u8 l_u8Port;
    IT6664_DeviceSelect(Device[u8DevId]);
    for(l_u8Port=0;l_u8Port<4;l_u8Port++)
    {
        #ifdef IT6663
        if(l_u8Port==0||l_u8Port==3)
            continue;
        #endif
        gext_var->VideoState[l_u8Port] = Tx_Video_Reset;

        #if _ENABLE_IT666X_FORCE_SET_OUTPUT_HDCP
        it666X_SetHdcpMode(l_u8Port,tx_hdcp_force_off);
        #endif
    }
    #else
    g_current_system_state = STAT_CHECK_DEV_READY;
    #endif
}
#endif

#endif//#if (_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_AND_SPLITER)||(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SPLITER_ONLY)
















