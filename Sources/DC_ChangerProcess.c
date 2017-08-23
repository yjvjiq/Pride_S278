//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : ÇàÄêÆÖ½­6123
//* File Name          : DC_ChangerProcess.c
//* Author             : ËïÀöÑ©
//* Version            : V1.0.0
//* Start Date         : 2016,11,30
//* Description        : ¿ì³äÁ÷³Ì
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"

unsigned char plug_DC_Connect=0;             //CC2Á¬½ÓÐÅºÅ
static unsigned char sendi1=0;               //·ÇÈ«¾Ö±äÁ¿
static unsigned char sendi2=0;               //·ÇÈ«¾Ö±äÁ¿
static unsigned char sendi3=0;               //·ÇÈ«¾Ö±äÁ¿

unsigned char	m_chmmode = CONSTANT_CURT_WORK;//È«¾Ö±äÁ¿
unsigned char fastendflag=0;                 //È«¾Ö±äÁ¿,¿ì³ä½áÊø±êÖ¾Î»
float	m_askvoltage=0;                        //ÒÀ¾Ýµç³Ø°ü³äµçÇúÏß
float	m_askcurrent=0;                        //È«¾Ö±äÁ¿£¬Ö±Á÷³äµçµçÁ÷
/////////¿ì³äÓÃ,BMSÍ£Ö¹³äµç»úÔ­Òò////////////////
unsigned char  fastend1=0;                   //È«¾Ö
unsigned char  fastend2=0;
unsigned char  fastend3=0;
unsigned char  fastend4=0xf0;

/////////¿ì³äÓÃ£¬·¢ËÍBSM£¬µç³Øµ±Ç°×´Ì¬////////////////
unsigned char  fasterror11 =0;
unsigned char  fasterror12 =0x10;            //ÔÊÐí³äµç£¬0 b 00 01 00 00

unsigned char BROStopState = 0;              //ÊÕµ½CROºó,²»ÔÙ·¢BRO
unsigned int chargeTimes=0;                  //³äµç´ÎÊý

//////////³¬Ê±Ê±¼ä±êÖ¾///////////////////////////////
unsigned int CRMOverTimeBefore60s = 0;//ÊÕ²»µ½CRO±¨ÎÄ60s£¬ÏµÍ³¿ª»úºó
unsigned int CRMOverTimeBefore = 0;//ÊÕ²»µ½CRO±¨ÎÄ
unsigned int CRMOverTime = 0;//CRM³¬Ê±·¢ËÍ0xaa³¬Ê±
unsigned int CROOverTime = 0;//5sÎ´ÊÕµ½CRO±¨ÎÄ
unsigned int CROOverTime60s = 0;//CRO³¬Ê±·¢ËÍ0xaa³¬Ê±
unsigned int CCSOverTime=0;                 //CCS³¬Ê±Ê±¼ä
unsigned int CSTOverTime=0;                 //CST³¬Ê±Ê±¼ä
unsigned int CSDOverTime=0;                 //CSD³¬Ê±Ê±¼ä
unsigned char OverTimeState=0;               //³¬Ê±±êÖ¾

unsigned char CC2ResState = 0; //0Î»ÖÃ×´Ì¬ 1¹ÊÕÏ×´Ì¬ 2Õý³£×´Ì¬
float DCTem1=0;//DC1ÎÂ¶È²É¼¯
float DCTem2=0;//DC2ÎÂ¶È²É¼¯
float CC2VOL = 0;
float PowerVOL = 0;
//unsigned char DC_CC2Count = 0;
unsigned char BROErrorAA = 1;//BRO²»·¢ËÍ0xaa
unsigned char DC_Vesion = 1;//DC³äµç°æ±¾1£ºÀÏ¹ú±ê;2£ºÐÂ¹ú±ê
unsigned char SelfState3 = 0;     //×Ô¼ì¼ÆÊýÆ÷±ä³É3±êÖ¾
unsigned char BEMStop=0;

unsigned char DCStartState=0;//¿ì³ä¿ªÊ¼±êÖ¾:1ÊÕµ½CRM,2ÊÕµ½CHM

static unsigned char SetBSD = 0;//BSDÊÇ·ñÒÑ¾­·¢ËÍ
static unsigned char SetBST = 0;//BSTÊÇ·ñÒÑ¾­·¢ËÍ
unsigned char ChargerStopState = 0;//³äµçÊ±Èç¹ûÃ»ÓÐ³äÂú£¬ÔòSOCÍ£ÁôÔÚ99.2%
unsigned char PantNormalEndFlag=0;//ÊÜµç¹­Õý³£ÏÂµç±êÖ¾Î»

//******************************************************************************
//* Function name:   GetRequestCurrentDC
//* Description:     ¿ì³ä³äµçÇëÇóµçÁ÷¼ÆËã C=120A
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
float GetRequestCurrentDC(void) 
{
    //float DCAmp;
    static unsigned char ChanceHappen1 = 0;//µçÑ¹¸ßÓÚ3.78ºó²»»Ö¸´
    static unsigned char ChanceHappen2 = 0;//µçÑ¹¸ßÓÚ3.92ºó²»»Ö¸´
    
    m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;
}
//**********************************************************************
//* Function name:   DCFaultDone
//* Description:     ¿ì³ä¹ý³ÌÖÐ¹ÊÕÏµÄ´¦Àí
//* EntryParameter : None
//* ReturnValue    : 0:ÎÞ¹ÊÕÏ;1:ÓÐ¹ÊÕÏ
//**********************************************************************
unsigned char DCFaultDone(void)
{
    static unsigned char ReturnValue = 0;//Ò»µ©¸³Öµ,²»ÄÜ¸Ä±ä
    //////////////////////////////¶Ï¸ßÑ¹/////////////////////////////        
    if((Error_Group4.Bit.F6_Bat_Over_V>=2)//×ÜµçÑ¹¹ýÑ¹2¼¶¡¢3¼¶
    ||(Error_Group2.Bit.F0_Cell_Over_V>=1))//µ¥Ìå¹ýÑ¹1¼¶¡¢2¼¶¡¢3¼¶
    
    {
        fastendflag = 1;
        fastend4 |= 0x04;//µçÑ¹Òì³£
        OffState = 1;//ÇëÇóÏÂµç
        ReturnValue = 1;
    }

	if(Error_Group1.Bit.F4_Ch_Over_I==3) //³äµçµçÁ÷3¼¶
    {
        fastendflag = 1;
        fastend4 |= 0x01;//³äµçµçÁ÷¹ý´ó
        OffState = 1;//ÇëÇóÏÂµç
        ReturnValue = 1;     
    } 

	if((Error_Group1.Bit.F6_Ins_Err==3)&&(g_BmsModeFlag == FASTRECHARGING))//¾øÔµ3¼¶¹ÊÕÏ
    {
        fastendflag = 1;
        fastend2 |= 0x01;//¾øÔµÒì³£
        OffState = 1;//ÇëÇóÏÂµç
        ReturnValue = 1;    
    }

	if(Error_Group0.Bit.F2_Ele_Relay_Con==3)
    {
        fastendflag = 1;
        fastend2 |= 0x40;//Á¬½ÓÆ÷ÎÂ¶È¹ý´ó
        OffState = 1;//ÇëÇóÏÂµç
        ReturnValue = 1; 
    } 

	if(Error_Group2.Bit.F4_Temp_Over==3)//ÈôÊÇ³öÏÖ3¼¶ÎÂ¶È¹ý¸ß¹ÊÕÏ
    {
        fastendflag = 1;
        fastend3 |= 0x01;//µç³ØÎÂ¶È¹ý¸ß
        OffState = 1;//ÇëÇóÏÂµç
        ReturnValue = 1;
    } 

	if(((g_caution_Flag_2 & 0x80) != 0)//³äµç»úÍ¨ÐÅ¹ÊÕÏ
	||(Error_Group3.Bit.F0_Sub_Com_Err)//ÄÚÍøÍ¨Ñ¶¹ÊÕÏ
	||(ACCha_Flag_BST)//³äµçÊ±µçÁ÷·½ÏòÒì³£
	||(Error_Group0.Bit.F0_Fire_Warning == 3))//»ðÔÖÔ¤¾¯3¼¶ 
//	||(Error_Group6.Bit.F0_Power_Vol)  ///¹©µçµçÔ´µçÑ¹Òì³£
    {
        fastendflag = 1;
        fastend3 |= 0x40;//ÆäËû¹ÊÕÏ
        OffState = 1;//ÇëÇóÏÂµç
        ReturnValue = 1;
    }

	if(Error_Group5.Bit.F6_Cell_Under_T == 3
	||g_bms_fault_msg.fault.Cell_T_Low_Flt == 3){
		fastendflag = 1;
	}
 
	return ReturnValue;   
	//////////////////////////////*¶Ï¸ßÑ¹*/////////////////////////////
}

//**********************************************************************
//* Function name:   TaskRechargeDC
//* Description:     ¿ì³äÁ÷³Ì£¬´óÔ¼Ã¿10msµ÷ÓÃÒ»´Î¸Ãº¯Êý
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void TaskRechargeDC(void)
{
    static unsigned char counter_250ms1=0;
    static unsigned char CHMStep1=0;
    
    static unsigned char counter_250ms2=0;
    static unsigned char CHMStep2=0;
    
    static unsigned char counter_250ms3=0;
    static unsigned char CHMStep3=0;
    
    static unsigned char counter_250ms4=0;
    static unsigned char CHMStep4_5=0;
    static unsigned char counter_50ms=0;
    
    static unsigned char counter_250ms5=0;
    static unsigned char CHMStep5=0;
    
    static unsigned char counter_250ms6=0;
    static unsigned char counter_250ms6_1=0;
    static unsigned char CHMStep6=0;
    
    static unsigned char counter_250ms7=0;
    
    static unsigned char DCFinish=0; //Ö±Á÷³äµçÊ±Ö»ÐÞÕýQ1Ò»´Î
    static unsigned char timer1S=0;//µ¥ÌåµçÑ¹µ½´ïºó,Í£Ö¹³äµçÌõ¼þÅÐ¶ÏÑÓÊ±
    static unsigned char timer2S=0;//µÚÒ»´Î½µµçÁ÷ÑÓÊ±
    static unsigned char timer3S=0;//µÚ¶þ´Î½µµçÁ÷ÑÓÊ±
    static unsigned char ChanceCurt=0;//³äµçÄ©¶Ë,µçÁ÷±ä»¯ºó²»»ØÌø,
    static unsigned char ChangeTimeState=0;//³äµç½áÊøºó×Ô½øÐÐÒ»´Î¼Ó·¨
    float curr=0;
    float curr1=0;
	static U16 counter_30s = 0;
    unsigned char ErrorState = 0;
  
    if((g_BmsModeFlag != FASTRECHARGING)&&(g_BmsModeFlag != RECHARGING))
        return;
    
//    if(g_highestTemperature>=(HIGHEST_ALLOWED_CHARGE_T+40))//40+57£¬40+0
//    {
//        fastend3|=0x01; //Temperature£¬ÖÐÖ¹³äµç
//        fastendflag=1; //¿ì³äÍ£Ö¹³äµç±êÖ¾  
//    }
    
    YoungMan_LT_FastChrgPowerAjust();
    curr =m_askcurrent_Model;
    ErrorState = DCFaultDone();
	curr1 = curr;
      
	if(g_BmsModeFlag == FASTRECHARGING) 
	{
		////////////ÇëÇóµçÑ¹//////////
		if(((Tavg-40) <= 54) && ((Tavg-40) >= 0))
		{
		    m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;	//3.65*204 = 744.6
		}
        else	//if((Tavg-40) > 54)
        {
            m_askvoltage = 0;
        }
    }
    else if(g_BmsModeFlag == RECHARGING) 
    {
        m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;
    }
     
    if((ErrorState==1)||(CutDCChaCurt0.word!=0)||(CutACChaCurt0.word!=0))//if fault occured.
    {
        curr1 = 0;
    }
    else 
    {
		if(g_BmsModeFlag == RECHARGING){
	        if(curr >= EleBand_Max_Current){
	            curr = EleBand_Max_Current;
				curr1 = curr;
	        }
		}
		else if(g_BmsModeFlag == FASTRECHARGING){
			if(curr >= FastChg_Max_Current){
				curr = FastChg_Max_Current;
				curr1 = curr;
			}
		}
		
        if((CutDCChaCurt50.word != 0) || (CutACChaCurt50.word != 0)){
            curr1 = 0.5 * curr;
        }
    }
    
    if((g_highestCellVoltage >= CHARGE_CUTDOWN_CV1 ) || (ChanceCurt == 1))//µçÁ÷Îª¼ÆÊý³öÀ´µÄÒ»°ë
    {
        timer2S++;
        if(timer2S >= 80)//10*80 = 800ms
        {
            timer2S = 81;
            if(curr1 > 0.1 * SetCap){
                curr1 = 0.1 * SetCap;
            }
            else if(curr1 == 0){
                curr1 = 0;
            }
            
            ChanceCurt = 1;
        }
    }
    else
    {
        timer2S=0;
    }
	
	if(g_highestCellVoltage >= HIGHEST_ALLOWED_CHARGE_CV)  //3.65V = stop charge, charge finished.
	{
        timer1S++;
        if(timer1S >= 100)//10ms*100=1000ms
        {
            if(DCFinish == 0)
            {
				ChargerStopState = 1;
				SocRechargeAdjust(); //SOC adjust and Q1 adjust.
				StoreParameter();

				DCFinish = 1;
            }
			
			fastendflag = 1;
			fastend1 |= 0x10;//µ¥ÌåµçÑ¹´ïµ½Ä¿±êÖµ£¬ÇëÇóÖÐÖ¹³äµç
			timer1S = 103; 

			curr1 = 0; //ÇëÇóµçÁ÷½µÎª0
			m_askvoltage = 0;//ÇëÇóµçÑ¹½µÎª0
        }
    } 
    else 
    {
        timer1S = 0;
    }
	
    m_askcurrent = curr1;
	
    //////////////////////////////³äµçÊ±·¢ËÍ¹¦ÂÊ///////////////////////////
	if(CHMStep)
	{
		if(CHMStep == 0x01)   //1801f456 00,¶àÖ¡±¨ÎÄ£¬²»ÊÇ250ms£¬±¨ÎÄ¼ä¸ô10ms
		{
			counter_250ms1++;
			if(counter_250ms1 >= 25)
			{
				if(DCStartState == 2)//new GB-T start flag, reveiced CHM
				{
					cpuToCHMBHM();
					CRMOverTimeBefore++;
					if(CRMOverTimeBefore >= 120)//Èç¹û30s½ÓÊÕ²»µ½CRMÔò³¬Ê± 
					{
						OverTimeState=1;//³¬Ê±±êÖ¾Î»ÖÃ1                
						CHMStep=0x07;
						BEMError1|=0x01;//ÊÕ²»µ½CRM£¬30sºó·¢ËÍBEM
						CRMOverTimeBefore = 0;
					}
				}
				
				if(DCStartState==1)////old GB-T start flag, reveiced CRM, detect whether 0xaa overtime.
				{
					CRMOverTime++;
					if(CRMOverTime>=20)//ÊÕ²»µ½0xaa5sºóÉÏ±¨
					{
						CHMStep=0x07;
						OverTimeState=1;
						BEMError1|=0x04;
						CRMOverTime = 0; 
					} 
				}
				
				CHMStep1=0;
				counter_250ms1=0;
    	      } 
			
			if(CHMStep1==0)
			{ 
				if(DCStartState==1)//Èç¹ûÒÑ¾­½ÓÊÕµ½¾É¹ú±ê¿ªÊ¼
				{
					if(BRMStep == 0){ // if bms can send BRM, then continue send.
						BRMStep = 1; // if received CTS, clear it, set 1 when have sent BRM
						cpuToCHMBRM();
						counter_250ms1=0;
						sendi1++;
					}
					else if((BRMStep == 1)&&(FlagBRMSend==1)){
						sendi1++;
//						if(sendi1==1)
//						  cpuToCHMBRM();
//						else 
						if(sendi1==2)
						  cpuToCHMBRMDATA1();
						else if(sendi1==3)
						  cpuToCHMBRMDATA2();
						else if(sendi1==4)
						  cpuToCHMBRMDATA3();
						else if(sendi1==5)
						  cpuToCHMBRMDATA4();
						else if(sendi1==6)
						  cpuToCHMBRMDATA5();
						else if(sendi1==7)
						{
							cpuToCHMBRMDATA6();
							if(DC_Vesion==1)
							{
								sendi1=0;
								CHMStep1=1;
								FlagBRMSend = 0;
								BRMStep = 0;//æŽ¥æ”¶åˆ°CRMåŽå¼€å§‹å‘é€BRMä»¥åŠä¸‹æ¬¡å‘é€BRMçš„æ—¶æœº
							}
						}
						else if(sendi1==8)
						{  
							cpuToCHMBRMDATA7();
							sendi1=0;        			          
							CHMStep1=1;
							FlagBRMSend = 0;
							BRMStep = 0;//æŽ¥æ”¶åˆ°CRMåŽå¼€å§‹å‘é€BRMä»¥åŠä¸‹æ¬¡å‘é€BRMçš„æ—¶æœº  
						}
					}
				}
			}
		}
		if(CHMStep==0x02)   //1801f456 aa
		{
			counter_250ms2++;
			if(counter_250ms2>=50)//500ms
			{
				CHMStep2=0;
				counter_250ms2=0;
			}
			if(CHMStep2==0)
			{ 
				sendi2++;
				if(sendi2 == 1)
					cpuToCHMBCP();
				if(sendi2 == 2)
					cpuToCHMBCPDATA1();
				else if(sendi2 == 3)
				{
					sendi2=0;
					cpuToCHMBCPDATA2();
					CHMStep2=1;
				}
				sendi1=0;//Çå³ýµÚÒ»×é¼ÆÊý
			}
		}
		if(CHMStep==0x03)  //1808f456 aa 250ms
		{            
			counter_250ms3++;
			if(CHMStep3==0)
			{
				CROOverTime++;
				if(SelfState3==0){
					cpuToCHMBRO();  //100956f4 aa
//					CloseNegControl=1;
				}
				if((CROOverTime>=20)||(CROOverTime60s>=240))//ÊÕµ½CRO,µ«Ã»ÓÐÊÕµ½0xaa,5sºó³¬Ê± 
				{
					OverTimeState=1;//³¬Ê±±êÖ¾Î»ÖÃ1                
					CHMStep=0x07;
					BEMError2|=0x04;
					CROOverTime = 0;
					CROOverTime60s = 0;
                }
                if(BROErrorAA==1)//³äµç×®²»Âú×ã³äµçÐèÇó
                {
                    CHMStep=0x07;
                    BEMError4|=0x04;//ÆäËû¹ÊÕÏ                
                } 
                else if(0==BROErrorAA)
                {
                    CROOverTime60s++;
                    TurnOnChangerRelay = 1;
                }
                   
                CHMStep3=1;
			}
			
            if(counter_250ms3>=25)//È·±£µÚÒ»Ö¡Êý¾Ý·¢³ö£¬²»±ØÔÚÑÓÊ±Ð¡ÓÚ250ms
            {
              
  			        counter_250ms3=0;
  			        CHMStep3=0;
  			        
            }
  		  }
		if((CHMStep==0x04)||(CHMStep==0x05)) //50ms period
		{
			CCSOverTime++;
			if(CCSOverTime>=100)//10ms*100=1S
			{
				OverTimeState=1;//³¬Ê±±êÖ¾Î»ÖÃ1
				if(CHMStep<=0x06){                
					CHMStep=0x06;
				}
                BEMError3|=0x01;
                CCSOverTime = 0;    
            }
            if(counter_50ms>=5)//50ms 
            {			            
                cpuToCHMBCL();    // voltage request and current request
                counter_50ms=0;
            }
			counter_50ms++;
			counter_250ms4++;      //250ms
			if(counter_250ms4>=25)
			{
				CHMStep4_5=0;
				counter_250ms4=0;
			}
			if(CHMStep4_5==0)
			{			        
				sendi1++;
				if(sendi1 == 1){
					cpuToCHMBCS();
				}
				else if(sendi1 == 2){
					cpuToCHMBCSDATA1();
				}
				else if(sendi1 == 3){
					cpuToCHMBCSDATA2();
					sendi1 = 0;
					CHMStep4_5 = 1;
				}
			}
			if(CHMStep==0x05) 
			{
				counter_250ms5++;
				if(CHMStep5==0)
				{
					cpuToCHMBSM();
					CHMStep5=1;
                }
                if(counter_250ms5>=25)//È·±£µÚÒ»Ö¡Êý¾Ý·¢³ö£¬²»±ØÔÚÑÓÊ±Ð¡ÓÚ250ms
                {
					counter_250ms5=0;
					CHMStep5=0;
                }
            }
		}	  
		if(((CHMStep==0x06)||(CHMStep==0x05))&&((fastendflag==1) ||(PantographOff == 1) || (OffState == 1) || (g_BmsModeFlag == RECHARGING && acc_Connect == 0)))//10ms·¢ËÍÒ»´Î,¿ªÊ¼·¢µÄÊ±ºò	
		{
			//Èç¹ûÊÕµ½µØÃæ³äµç»ú³äµç½ØÖ¹±¨ÎÄ»òÕßµ¥ÌåµçÑ¹¡¢×ÜµçÑ¹³¬¹ý±£»¤Öµ
			if(OverTimeState==1)
			{
				if(counter_250ms6_1%25==0)
				{
					counter_250ms6_1 = 0;
					cpuToCHMBEM();
				}
				counter_250ms6_1++;
			}
			if(SetBSD==0)//Èç¹ûBSDÒÑ¾­·¢ËÍ,ÔòBST²»ÔÙ·¢ËÍ
			{
				cpuToCHMBST();
				m_askcurrent=0;//ÇëÇóµçÁ÷Îª0
				CSTOverTime++;//10ms*200
				if(CSTOverTime>=500)//·ÀÖ¹ÊÕ²»µ½CST±¨ÎÄ5sÑÓÊ±,10ms*500 
				{
					CHMStep=0x07;
					OverTimeState = 1;
					BEMError3|=0x02;
					CSTOverTime = 0;
				}
			}
			if(CHMStep==0x06)   
			{
				if(CHMStep6==0)
				{   
					cpuToCHMBSD();  // report BMS status: the highest and the lowest cell voltage, temperature and SOC.
					SetBSD = 1;
					CHMStep6=1;
				}
                counter_250ms6++;
				if(counter_250ms6>=25)
				{
					counter_250ms6=0;
					CHMStep6=0;			                       
				}
				
				CSDOverTime++;
				if(CSDOverTime>=1000)//½ÓÊÕ²»µ½³äµç»úÍ³¼Æ±¨ÎÄÊ±,ÑÓÊ±10sÏÂµç
				{
					CHMStep=0x07;
					OverTimeState = 1;
					BEMError4|=0x01;
					CSDOverTime = 0;     			          
				}    			      
			}
		}
		
		if(CHMStep==0x07) //¿ì³ä³äµç½áÊø 
		{
			SelfState3=0;//ä¹‹å‰æ”¶åˆ°CROä»¥åŽå†ä¹Ÿä¸å‘é€äº†ï¼Œæµç¨‹å¦‚æžœé‡æ–°æ¥ï¼Œéœ€è¦æ¸…é™¤
			if(counter_250ms7 % 25==0)
			{
				if((OverTimeState==1)||(BROErrorAA==1))//Èç¹ûÓÐ³¬Ê±¹ÊÕÏ²ÅÉÏ±¨,Ã»ÓÐ²»ÉÏ±¨
				{	                
					if(BEMStop==0)
					cpuToCHMBEM();
				}
				counter_250ms7 = 0;
			}
			counter_250ms7++;
			if(BEMStop == 1){
				counter_30s++;
				if(counter_30s>3000)//30Sä»¥å†… 3000*10ms=30S 
				{
					OffState = 1;//×´Ì¬»úÓÉ170Ìø×ª40µÄ±êÖ¾Î» 
					Error_Group1.Bit.St_CHG_Allow = 1; //³äµçÔÊÐí×´Ì¬Î»²»ÔÊÐí
					PantNormalEndFlag=1;//ÊÜµç¹­Õý³£ÏÂµç			  
					counter_30s=3002;
				}
			}
			else{
				OffState=1;//çŠ¶æ€æœºç”±170è·³è½¬180çš„æ ‡å¿—ä½ 
				Error_Group1.Bit.St_CHG_Allow=1; //å……ç”µå…è®¸çŠ¶æ€ä½ä¸å…è®¸
				PantNormalEndFlag=1;//å—ç”µå¼“æ­£å¸¸ä¸‹ç”µ
			}
		}
	}/////////END¿ì³ä·¢ËÍÏûÏ¢
}
//******************************************************************************
//* Function name:   TaskDC
//* Description:     10msÖÐ¶ÏÖÐµ÷ÓÃ¸Ãº¯Êý
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void TaskDC(void)
{  
    if((FASTRECHARGING==g_BmsModeFlag)||(RECHARGING==g_BmsModeFlag)) ////¿ì³ä»òÊÜµç¹­³äµç
    {
        if(DCStartState == 0)//µ±½ÓÊÕµ½Ö®ºó£¬²»ÔÙ¼ÆÊ±
        {
			CRMOverTimeBefore60s++;
			if(CRMOverTimeBefore60s>=6000)//10ms*6000
			{
				OverTimeState=1;//³¬Ê±±êÖ¾Î»ÖÃ1 			   
				CHMStep=0x07;
				BEMError1|=0x01;//ÊÕ²»µ½CRM£¬30sºó·¢ËÍBEM
				CRMOverTimeBefore60s = 0;	   
			}
        }
		
        if((stateCode==110)||(stateCode==170)|| (DCStartState != 0) ||(OverTimeState==1)){        
            TaskRechargeDC();
        }
    }
}

//******************************************************************************
//* Function name:   GetDCTem
//* Description:     ¼ì²âÖ±Á÷³äµçµÄÎÂ¶È
//* EntryParameter : None
//* ReturnValue    : None
	
	//	TurnOn_CC2();//12V   µçÑ¹ÇÐ»»ÔÚ³õÊ¼»¯BMSÀï
//******************************************************************************
void GetDCTem (void)
{
    float getAD1=0;//ADÖµ1
    float getAD2=0;//ADÖµ2
    unsigned int GetADVol1 = 0;
    unsigned int GetADVol2 = 0;
    float VolValue1=0;//×ª»»ÍêµÄµçÑ¹Öµ1 ÎÂ¶È1
    float VolValue2=0;//×ª»»ÍêµÄµçÑ¹Öµ2 ÎÂ¶È2
    float VolValue3=0;//×ª»»ÍêµÄµçÑ¹Öµ1 CC2µçÑ¹
    float VolValue4=0;//×ª»»ÍêµÄµçÑ¹Öµ2 µçÔ´µçÑ¹
    
    float R1=0;//ÎÂ¸ÐÍ·1×èÖµ
    float R2=0;//ÎÂ¸ÐÍ·2×èÖµ  

    getAD1 = readADC_SingleEnded(0); //T1
    getAD2 = readADC_SingleEnded(3); //T2
    
    GetADVol1 = readADC_SingleEnded(1);//Vol of CC2
    GetADVol2 = readADC_SingleEnded(2); //Vol of µçÔ´
    //EnableInterrupts; //¿ªÖÐ¶Ï£¬½ÓÊÕ×´Ì¬»ú±àÂë 
    VolValue1=4.096*(float)getAD1/2048.0;     //(2048-0)/(4.096-0)=(AD1-0)/(ADX-0)
    VolValue2=4.096*(float)getAD2/2048.0;
    
    VolValue3=4.096*(float)GetADVol1/2048.0; //cc2
    VolValue4=4.096*(float)GetADVol2/2048.0; //µçÔ´µçÑ¹
    
    CC2VOL = (75.0+12.0)*VolValue3/12.0; //CC2µçÑ¹
    PowerVOL = (75.0+12.0)*VolValue4/12.0; //µçÔ´µçÑ¹
 
    R1=VolValue1/(5-VolValue1)*10;
    R2=VolValue2/(5-VolValue2)*10;
    
    DCTem1=lookup_Table_Tem_NEW20170204_custom(VolValue1);
    
    DCTem2=lookup_Table_Tem_NEW20170204_custom(VolValue2);
    
    
}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************
