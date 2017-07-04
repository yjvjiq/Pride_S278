//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : Task.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2014.5.7
//* Description        : ¸ÃÎÄ¼þ½«Õû¸öÏµÍ³½øÐÐÈÎÎñ»®·Ö²¢ÕûºÏËùÓÐ¹ý³Ì
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h" 



//**************************************************************************************
//* Variable definition                            
//**************************************************************************************/
//ÈÎÎñÁÐ±í
static TASK_COMPONENTS TaskComps[] = 
{ 
    {0, 1,      1,      TaskCurrentDetect},         // µçÁ÷¼ì²âÈÎÎñ 1ms¼ì²âÒ»´ÎµçÁ÷Öµ //0 ±êÖ¾Î»£¬1£¬
    {0, 700,    700,    TaskSocProcess},            // SOC´¦Àí
    {0, 297,    300,    TaskBmuProcess},            // BMUÊý¾Ý´¦ÀíÈÎÎñ//ÒÔÇ°60ms
    {0, 17,     20,     TaskVoltage},               // ×ÜµçÑ¹¼ÆËã   .
    {0, 297,    300,    TaskInsulation},            // ¾øÔµµç×è¼ÆËã //¸Ã´¦Ê±¼ä×îÐ¡600MS£¬²»ÄÜÌ«Ð¡  80ms
    {0, 97,     100,    TaskReport2PC},             // ·¢ËÍ±¨ÎÄµ½ÉÏÎ»»ú
    {0, 5,      5,      TaskStatusMachine},         // ×´Ì¬»ú´¦Àí
    {0, 997,    1000,   TaskFaultProcess},          // 100ms¹ÊÕÏ´¦Àí
    //{0, 8,      8,    TaskRechargeDC},            // Ö±Á÷³äµç ²»ÄÜ·Å¿ª
    {0, 9,      10,     TaskRecordProcess},         // ÀúÊ·¼ÇÂ¼´¦Àí
      
};

//**************************************************************************************
//* FunctionName   : TaskRemarks()
//* Description    : ÈÎÎñ±êÖ¾´¦Àí
//* EntryParameter : None
//* ReturnValue    : None
//**************************************************************************************/
void TaskRemarks(void)
{
    uchar i;

    for (i=0; i<TASKS_MAX; i++)          // Öð¸öÈÎÎñÊ±¼ä´¦Àí
    {
         if (TaskComps[i].Timer)          // Ê±¼ä²»Îª0
        {
            TaskComps[i].Timer--;         // ¼õÈ¥Ò»¸ö½ÚÅÄ
            if (TaskComps[i].Timer == 0)       // Ê±¼ä¼õÍêÁË
            {
                 TaskComps[i].Timer = TaskComps[i].ItvTime;       // »Ö¸´¼ÆÊ±Æ÷Öµ£¬´ÓÐÂÏÂÒ»´Î
                 TaskComps[i].Run = 1;           // ÈÎÎñ¿ÉÒÔÔËÐÐ
            }
        }
   }
}
//**************************************************************************************
//* FunctionName   : TaskProcess()
//* Description    : ÈÎÎñ´¦Àí
//* EntryParameter : None
//* ReturnValue    : None
//**************************************************************************************/
void TaskProcess(void)
{
    uchar i;

    for (i=0; i<TASKS_MAX; i++)           // Öð¸öÈÎÎñÊ±¼ä´¦Àí
    {
        if (TaskComps[i].Run)           // Ê±¼ä²»Îª0
        {
             TaskComps[i].TaskHook();         // ÔËÐÐÈÎÎñ
             TaskComps[i].Run = 0;          // ±êÖ¾Çå0
        }
    }   
}
//************************************************************************
//* Function name:TaskCurrentDetect
//* Description:Í¨¹ýÁ½¸öADÍ¨µÀ¶ÁÈ¡ADÖµ£¬ÔÙ¾­¹ý¹«Ê½×ª»»³ÉµçÁ÷£¬¸ù¾Ý·¶Î§È¡ÆäÖÐÒ»¸öµçÁ÷
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskCurrentDetect(void)//
{
    
    turnOnADC(); //´ò¿ªADCÖÐ¶Ï
	  delay(10);  
	  if((Int_Flag&0x01)==0x01) //²ÉÁË32´ÎADÖµºó²Å¼ÆËã
        g_systemCurrent = CurrentCalculation();

}  
//************************************************************************
//* Function name:TaskSocProcess
//* Description:Ã¿Ãë¼ÆËã²¢±£´æSOC£¬Ã¿·ÖÖÓ±£´æÒ»´ÎÀúÊ·¼ÇÂ¼
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskSocProcess(void)
{
    
    static float ahCharge1A=0;
    static float ahDischarge1A=0;
    static unsigned char DCfinish=0;

    unsigned char i=0;
    float ft=0;
	  	  	  
    I2CReadDate();  //¶ÁÈ¡ÏµÍ³Ê±¼ä

    if(CurrentTime[0]!=g_oldTime[0])   //Ã¿ÃëÖÓ¼ÆËãÒ»´ÎSOCµÄÖµ
    {
        g_oldTime[0] = CurrentTime[0];//Ãë
        SocIntegral();//¼ÆËã»ý·ÖµÃµ½µÄSOCÖµ	   
        g_energyOfUsed = 0;	    		
        StoreSocRealvalue();//Ã¿Ãë±£´æSOCÖµ
        //if(g_errorRecordRead_flag==0)
            //StoreSysVariable();//for test

    } //end of Ã¿ÃëÖÓ
				
    if(CurrentTime[1]!=g_oldTime[1])// Ã¿·ÖÖÓSOCÖµ¸³¸øÏµÍ³²ÎÊý£¬×÷Îª±£´æ»òÍ¨ÐÅ
    {
        g_oldTime[1] = CurrentTime[1];//·Ö

        g_sysPara[PARA_SOC_VALUE] = g_socValue;

        if((g_BmsModeFlag == DISCHARGING)&&(StoreAHState==1))
        {
            ft=(StoreAHSOC-g_socValue)*SetCap;//¼ÆËãÀÛ»ý³ä·ÅµçAHÈÝÁ¿
            if(ft>1)
            {			        
                dischargeAH += (unsigned int)ft;
                StoreAHSOC=First_g_socValue;
            }
        } 
        else if((g_BmsModeFlag == FASTRECHARGING)&&((StoreAHState==1)))
        {
            ft=(g_socValue-StoreAHSOC)*SetCap;//¼ÆËãÀÛ»ý³ä·ÅµçAHÈÝÁ¿
            if(ft>1)
            {			        
                chargeAH += (unsigned int)ft;
                StoreAHSOC=First_g_socValue;
            }
        }
			  
        //First_g_socValue=StoreAHSOC;      

        //±£´æ×´Ì¬Êý¾Ý
        if(g_errorRecordRead_flag==0)
            StoreSysVariable();//Ã¿·ÖÖÓ±£´æ¹ÊÕÏ¼ÇÂ¼ÐÅÏ¢
        if(StoreAHState==1)    	
            StoreChargeDischargeAH();//±£´æÀÛ»ý³ä·Åµç×ÜÈÝÁ¿	  
        sendBMSIDtoBMU(); //·¢ËÍBMS°æ±¾IDºÅ¸øBMU
        sendRealtimeToBMU(); //·¢ËÍBMSÏµÍ³Ê±¼ä¸øBMU
    }
}
//**********************************************************************
//* Function name:    TaskBmuProcess
//* Description:      BMU×Ô¼ì£¬Èç¹û³¤Ê±¼äÊÕ²»È«BMUÊý¾Ý£¬ÔòÏÔÊ¾ÄÚ²¿Í¨ÐÅ¹ÊÕÏ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void TaskBmuProcess(void) 
{
   static unsigned long t=0;
   
   unsigned char i=0,k=0x01,m=0x02;
   unsigned char ErrorState=0;
   float Value=0;
   unsigned long buff=0;
     
    if(!bmuProcess2()) //BMU×Ô¼ì   	        
    {        
        t++;
        _FEED_COP();   //2sÄÚ²»Î¹ÄÚ¹·£¬ÔòÏµÍ³¸´Î»
    } 
    else 
    {            
        t=0;
        GetTavgProcess();
        //State_Box_Online=0x3f;        
    }
	
    if(t>=100) //³¬Ê±¹ÊÕÏ±¨¸æ300ms*100=30s
    {
        if(t<=105)//5*300=1500ms
            return;
        else //t>105
        {          
            g_caution_Flag_3 |=0x01;    //to pc  
        }            
    }/////end of BMUÍ¨ÐÅÅÐ¶Ï
    
    GetDCTem(); 
}

//***********************************************************************
//* Function name:   TaskReport2PC
//* Description:     ½«BMSµÄÏà¹ØÐÅÏ¢Í¨¹ýÄÚ²¿±¨ÎÄ0x0c0450,0x0c0451,0x0c0452·¢¸øÉÏÎ»»úÈí¼þ
//*                  Îª·ÀÖ¹¶ªÖ¡£¬ËùÒÔ±¨ÎÄ¼ä¸ô·¢ËÍ
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskReport2PC(void) 
{
    static unsigned char ct=0;
	
    ct++;
	switch(ct){
		case 1:
			bmsToPcInfo450();
			break;
		case 2:
			bmsToPcInfo451();
			break;
		case 3:
			bmsToPcInfo552();
			break;
		case 4:
			bmsToPcInfo553();
			break;
		case 5:
			bmsToPcInfo554();
			break;
		case 6:
			if(g_BmsModeFlag == FASTRECHARGING){
				bmsToSBMSMessage1(); //0x000c0125
			}
			ct = 0;
			break;
		default:
			break;
	}
}
//***********************************************************************
//* Function name:   TaskGpioTest
//* Description:     Ó¦ÓÃ³ÌÐò½øÈëµ½µ÷ÊÔ½×¶ÎÊ±,¶ÔÖ÷°åA¿ÚµÄ¼ì²â
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskGpioTest(void) 
{
    static unsigned int TimerCP=0; 
    if((input2_state()==0)&&(input3_state()==1))   //16A 680R
        C0552_0 |= 0x01; 
    else
        C0552_0 &= 0xfe;
    
    if((input2_state()==0)&&(input3_state()==0))   //32A 220R
        C0552_0 |= 0x02; 
    else
        C0552_0 &= 0xfd;

    if(input6_state()==0)       //CP
    {
        C0552_0|=0x04;
        if(TimerCP>=500)      //10ms ¶¨Ê±Æ÷ 2S
        {
            //TurnOn_CP();
            TimerCP=501;
        }
    }
    else 
    {
        C0552_0&=0xfb;
        //TurnOff_CP();
        TimerCP=0;
    }
    
    if(input4_state()==0)      //ACC
        C0552_0|=0x08; 
    else
        C0552_0&=0xf7;
    

    if(input6_state()==0)    //³äµç·´À¡     (±ÕºÏ³äµç·´À¡Ê±,ACCÒ²ÁÁ)
        C0552_0|=0x10; 
    else
        C0552_0&=0xef;

    if(input5_state()==0)       //CC2
        C0552_0|=0x20; 
    else
        C0552_0&=0xdf;
    /*
    if(inputP_state()==0)       // Õý¼«·´À¡
        gpio_state|=0x20;  
    else
        gpio_state&=0xdf;

    if(inputN_state()==0)       // ¸º¼«·´À¡
        gpio_state|=0x40;
    else
        gpio_state&=0xbf;

    if(inputH_state()==0)       ///INPUT1 ¸ßµÍÑ¹»¥Ëø
        gpio_state|=0x80; 
    else
        gpio_state&=0x7f;  
    */
}
//***********************************************************************
//* Function name:   TaskStatusMachine
//* Description:     BMS¸ù¾ÝÕû³µ·¢¹ýÀ´µÄ×´Ì¬»ú±àÂë½øÐÐÏàÓ¦¶¯×÷
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskStatusMachine(void)//5msµ÷ÓÃÒ»´Î 
{
    static unsigned char counter1_10ms=0;
    static unsigned char counter1_500ms=0;
    static unsigned char counter2_500ms=0;
    static unsigned char counter3_500ms=0;
    static unsigned char state30=0;
    static unsigned char state46=0;
    static unsigned char Delay14=0;
    static unsigned char TestDelay=0;//½øÈëµ½µ÷ÊÔÄ£Ê½ÏÂµÄÊ±¼äÑÓÊ±
    static unsigned char dcMode,pcMode =0;//³äµç×´Ì¬¸³Öµ±êÖ¾Î»

    
    SignalOnOffJudge(); 
//    HeatManage();  
	CarFaultDone();
	
    switch(stateCode)
    {
        //////////////////****************ÉÏµç¹ý³Ì********************///////////////// 
        case 11:
        case 81:
        case 141:
			tmr_p1=0;
			tmr_p2=0;
			tmr_p3=0;
            status_group1.Bit.St_BMS =3;
            state46 = 0;
            if((stateCode==141)&&((DCTem1>=85)||(DCTem2>=85))) //ÔÚ´ËÅÐ¶Ï·ÀÖ¹¹ÊÕÏÉÏ±¨µÄÂý
            {
                fastendflag=1;
                fastend2|=0x40;//Á¬½ÓÆ÷ÎÂ¶È¹ý´ó
                OffState=1;//ÇëÇóÏÂµç
                Error_Group0.Bit.F2_Ele_Relay_Con=2;//Õû³µCAN¸³Öµ     
            }
            break;
        case 12:  //********************µÍÑ¹×Ô¼ì*************//////////////
        case 82:
        case 142:
            Error10S=0;
            if(stateCode == 12){
				turnOnSW_Power();
			}
			
            HighVoltDetectPart1();//ÐÐ³µ£ºMSDÓë¸º¼«Õ³Á¬ ÊÜµç¹­£ºMSDºÍ³äµç¸ºÕ³Á¬
            break;
        case 14:  //***********************±ÕºÏÖ÷¸º************/////////////
        case 84:
        case 144:
            Delay14++;
            if(stateCode == 14)
				Kn_Switch(ON);
            else
            {
                KChg_N_Switch(ON);	//±ÕºÏ³äµç¸º
                delay(25000);       //19ms
                delay(25000);       //19ms
            }
            if(Delay14 >= 4) 
            {
                if(stateCode == 14)
                    status_group3.Bit.St_N_Relay=1;
                else
                    BmsCtlStat0 |=0x08;//Ô¤³ä¼ÌµçÆ÷×´Ì¬±ÕºÏ
                Delay14=0;
            }
            break;
        case 17:   //***********************µç³Ø¸ßÑ¹¼ì²â*****//////////////
        case 87:
        case 147:
            HighVoltDetectPart2();//¸º¼«¶ÏÂ·ÓëÕý¼«Õ³Á¬
            break;
        case 20:   //*********************Õý¼«¼ÌµçÆ÷±ÕºÏ*******////////////
        case 90:
        case 150:
            if(stateCode == 20)
                closePosRelay();
            else if(stateCode == 90) 
            {
                TurnOn_INA1K();
                delay(25000); //20ms
            }
            else if(stateCode == 150) 
            {
                TurnOn_INA2K();
                delay(25000); //20ms
            }
            HighVoltDetectPart3();
            break;
        //////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        //////////////////****************¹¤×÷¹ý³Ì********************///////////////// 
        //////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////      
        case 30:    //*********************ÐÐ³µ×´Ì¬***********/////////////
            turnOnSW_Power();//´ò¿ªÈí¼þ¿ª¹Ø
            if(state30==0) 
            {
//                tmr_p1=0;
//                tmr_p2=0;
//                tmr_p3=0;
                state30=1;
                pcMode=0; //·ÀÖ¹½øÈë×´Ì¬»úÖØ¸´¸³Öµ³äµç×´Ì¬£¬ÔÚ30Çå0
                dcMode=0; //·ÀÖ¹½øÈë×´Ì¬»úÖØ¸´¸³Öµ³äµç×´Ì¬£¬ÔÚ30Çå0
                
            }
    
            //YoungMan_LT_step();
            if(plug_AC_CP_Connect == 0) 
            {
                BiggestDischargeCurt = BigDischargePowerAdjust();//SOF//30s
                BiggestDisCurtContinuous = BigDischargePowerContinuous();//SOF//5min
                BiggestFeedbackCurt = PulseRechargePowerAdjust1(); //ÖÆ¶¯ÄÜÁ¿»ØÊÕ30s 
                BiggestFeedbackCurtContinuous = ContinueRechargeCurt(); //ÖÆ¶¯ÄÜÁ¿»ØÊÕ5min         
            }
            else if((plug_AC_CP_Connect == 1)||(HighVolPowerOff == 1))//¼ì²âµ½ÊÜµç¹­
            {
                BiggestDischargeCurt =0;//SOF//30s
                BiggestDisCurtContinuous = 0;//SOF//5min
                BiggestFeedbackCurt = 0; //ÖÆ¶¯ÄÜÁ¿»ØÊÕ30s 
                BiggestFeedbackCurtContinuous = 0; //ÖÆ¶¯ÄÜÁ¿»ØÊÕ5min         
            }
            //if((plug_DC_Connect==1)||(OffState==1)||(HighVolPowerOff==1))//×´Ì¬»úÇÐ»»»òÕßÓÐÏÂµç¹ÊÕÏ»òÕßÓÐ¹ÊÕÏ
            {
                //state_group4.Bit.Request_Power_Off = 1;//BMS¸ßÑ¹ÏÂµçÇëÇó
            } 
			SocEndDischargeAdjust(); //·ÅµçÄ©¶ËSOCÐÞÕý
//			CarFaultDone();//¹ý³Ì¹ÊÕÏ´¦Àí,¹¦ÂÊÎªÑ­»·ÉÏ±¨,·ÀÖ¹ÓÉÓÚÊ±ÐòÎó±¨
			break;
			
        case 110:  //*********************Âý³ä³äµç***********//////////////
            turnOnSW_Power();//´ò¿ªÈí¼þ¿ª¹Ø ·ÀÖ¹×´Ì¬»úÔÚÖ®Ç°¾ÍËÀµô¶Ï²»ÁËµç
            BiggestDischargeCurt =0;//SOF//30s
            BiggestDisCurtContinuous = 0;//SOF//5min
            BiggestFeedbackCurt = 0; //ÖÆ¶¯ÄÜÁ¿»ØÊÕ30s 
            BiggestFeedbackCurtContinuous = 0; //ÖÆ¶¯ÄÜÁ¿»ØÊÕ5min         
            counter1_500ms++;
            if(pcMode==0) 
            {
                status_group3.Bit.St_Charge = 1; //³äµçÖÐ
                pcMode=1;
            }
            
            //state_group2.Bit.St_Charge_Achieve = 1;   //³äµçÖÐ        
            if(counter1_500ms>=70) //7*70=490ms
            {
                counter1_500ms=0;
                Error_Group1.Bit.St_DisCHG_Allow=1;//·ÅµçÔÊÐí×´Ì¬Î»²»ÔÊÐí
   
            }
            
//			CarFaultDone();//¹ý³Ì¹ÊÕÏ´¦Àí,¹¦ÂÊÎªÑ­»·ÉÏ±¨,·ÀÖ¹ÓÉÓÚÊ±ÐòÎó±¨
    
            break;
            
        case 170:   //********************¿ì³ä³äµç *************///////////
            turnOnSW_Power();//´ò¿ªÈí¼þ¿ª¹Ø ·ÀÖ¹×´Ì¬»úÔÚÖ®Ç°¾ÍËÀµô¶Ï²»ÁËµç
            BiggestDischargeCurt =0;//SOF//30s
            BiggestDisCurtContinuous = 0;//SOF//5min
            BiggestFeedbackCurt = 0; //ÖÆ¶¯ÄÜÁ¿»ØÊÕ30s 
            BiggestFeedbackCurtContinuous = 0; //ÖÆ¶¯ÄÜÁ¿»ØÊÕ5min 
            
            if( dcMode==0) 
            {
                status_group3.Bit.St_Charge = 1; //³äµçÖÐ
                dcMode=1;
            }
 
            counter1_500ms++;
            if(counter1_500ms>=70)
            {
				counter1_500ms=0;
//				HeatAndChargeControl();
				Error_Group1.Bit.St_DisCHG_Allow=1;//·ÅµçÔÊÐí×´Ì¬Î»²»ÔÊÐí
            }
            
//			CarFaultDone();//¹ý³Ì¹ÊÕÏ´¦Àí,¹¦ÂÊÎªÑ­»·ÉÏ±¨,·ÀÖ¹ÓÉÓÚÊ±ÐòÎó±¨

            break;
 
        //////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        //////////////////****************ÏÂµç¹ý³Ì********************///////////////// 
        //////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////                         
        case 40:   //*****************¶Ï¿ªÖ÷Õý¼ÌµçÆ÷***********////////
        case 120:
            status_group1.Bit.St_BMS =2;//¸ßÑ¹¶Ï¿ª
			
            if(stateCode == 40 && Error_Group6.Bit.F3_BMS_Protect != 1) 
            {
                openPosRelay(); //¶Ï¿ªÕý¼«¼ÌµçÆ÷
                //PRelayConnectTest();//Õý¼«Õ³Á¬ÔÚ´Ë´¦¼ì²â Õ³Á¬²»ÔÙ¼ì²âÁË 
            }
            else if(stateCode == 120 && Error_Group6.Bit.F3_BMS_Protect != 1)
            {
                TurnOff_INA1K();//¶ÏÊÜµç¹­¼ÌµçÆ÷
                //ChgRelayConnectTest();//ÊÜµç¹­¼ÌµçÆ÷Õ³Á¬
                status_group3.Bit.St_Charge = 2; //³äµç½áÊø
            }
            delay(25000); //19ms
            delay(25000); //19ms
            bmsSelfcheckCounter=2;//Ã»ÓÐ¹ÊÕÏ£¬×Ô¼ì¼ÆÊýÆ÷
			
            break;
        case 180:
            InsRelayControl=0;//¹Ø±Õ¾øÔµ¼ì²â¿ª¹Ø
            TurnOffInsulation();//¹Ø±Õ¾øÔµ¼ì²âµÄ¿ª¹Ø
            status_group3.Bit.St_Charge = 2; //³äµç½áÊø
            status_group1.Bit.St_BMS =2;//¸ßÑ¹¶Ï¿ª
            TurnOff_INA2K();//¶Ï¿ª¿ì³ä¼ÌµçÆ÷
            delay(25000); //19ms
            delay(25000); //19ms
            //DCChgRelayConnectTest();//¿ì³ä¼ÌµçÆ÷Õ³Á¬
            bmsSelfcheckCounter=2;//Ã»ÓÐ¹ÊÕÏ£¬×Ô¼ì¼ÆÊýÆ÷
            break;
             
        case 44:   //********************¶Ï¿ªÖ÷¸º***************//////////      
        case 124:
        case 184:
            if((stateCode==44)||(stateCode==184)) //ÊÜµç¹­³äµçÏÂµçÊ±²»¶Ï¿ªÕý¼«£¬·ÀÖ¹ÔÙ´ÎÉÏµçÉÕ¹­
            {
                openPosRelay();//ÔÙ¶Ï¿ªÕý¼«¼ÌµçÆ÷£¬È·±£Õý¼«¼ÌµçÆ÷¶Ï¿ª¡
			}
			
			TurnOff_INA1K();//ÔÙ¶Ï¿ªÊÜµç¹­¼ÌµçÆ÷£¬È·±£¼ÌµçÆ÷¶Ï¿ª¡

            if(stateCode == 44){
                openNegRelay();//¶Ï¿ªÖ÷¸º¼ÌµçÆ÷
            }
            else{
                TurnOff_INBK();//¶Ï¿ª³äµç¸º¼ÌµçÆ÷
            }
            
            delay(25000); //19ms
            delay(25000); //19ms
            TurnOffNRelay=1;//¶Ï¸º¼«¼ÌµçÆ÷Íê³É±êÖ¾
            break;  
            
        case 46:   //*****************¸ßÑ¹µôµç¼ì²â************//////////////
        case 126:
        case 186:
            /*
            if(stateCode == 46)
                NRelayConnectTest();//Ö÷¸ºÕ³Á¬    //ÏÂµç²»¼ì²âÕ³Á¬ÁË ËùÒÔ×¢ÊÍµô
            else 
                ChgNRelayConnectTest();//³äµç¸ºÕ³Á¬
			*/
            if(state46==0)
            {
                delay(25000); //20ms
                delay(25000); //20ms
               
                TurnOff_INHK();//¹Ø±Õ¼ÓÈÈ¼ÌµçÆ÷ 
                preChargeON=0;
                /*
                tmr_p1=0;
                tmr_p2=0;
                tmr_p3=0;
                tmr_p4=0;
                tmr_p6=0;
                */

                TurnOffNRelay=0;
                //TurnOff_INBK();//¶Ï¿ªÔ¤³ä,·ÀÖ¹×´Ì¬»úÓÉ12Ìø×ªµ½46Ê±Ô¤³ä¼ÌµçÆ÷Ã»ÓÐ¶Ï¿ª
                StoreSysVariable();
                StoreSocRealvalue();
                state46=1;
                SelfCheck = 0;//×Ô¼ìÇåÁã,·ÀÖ¹²»¶ÏµçÔÚ×Ô¼ìÊ±³ÌÐò²»Æð×÷ÓÃ
            }
            
            delay(25000); //19ms
            delay(25000); //19ms
            bmsSelfcheckCounter=1;
			
			if(stateCode == 86 || stateCode == 126){
				Error_Group1.Bit.St_CHG_Allow = 1;	// not allowed.
				status_group3.Bit.St_Charge = 3;	// charge invalid.
			}
			if(stateCode == 46){
				Error_Group1.Bit.St_DisCHG_Allow = 1; // discharge not allowed.
			}
            break;
            
        case 47:  //******************BMS¶Ïµç***************////////////////
        case 127:
        case 187:
            openPosRelay();//ÔÙ¶Ï¿ªÕý¼«¼ÌµçÆ÷£¬È·±£Õý¼«¼ÌµçÆ÷¶Ï¿ª¡£ 
            openNegRelay();//ÔÙ¶Ï¿ª¸º¼«¼ÌµçÆ÷£¬È·±£Õý¼«¼ÌµçÆ÷¶Ï¿ª¡£
            bmsSelfcheckCounter=0;
            delay(25000); //20ms
            delay(25000); //20ms 
            delay(25000); //20ms
            delay(25000); //20ms
            turnOffSW_Power();//close×ÜµçÔ´¿ª¹Ø  
            break;
        case 177://µ÷ÊÔ½×¶Î
            openNegRelay();//¶Ï¿ª¸º¼«¼ÌµçÆ÷
            delay(25000); //20ms
            
            _FEED_COP();   //2sÄÚ²»Î¹ÄÚ¹·£¬ÔòÏµÍ³¸´Î»
            
            TurnOff_INHK();//¶Ï¿ª¼ÓÈÈ¼ÌµçÆ÷
            delay(25000); //19ms
            delay(25000); //19ms
            TurnOff_INA2K();//¶Ï¿ª¿ì³ä¼ÌµçÆ÷
            turnOffSW_Power();//close×ÜµçÔ´¿ª¹Ø 
            State177End=1;//Ìø×ªµ½179µÈ´ý
            break;
        case 179://µ÷ÊÔ½×¶Î
            TaskGpioTest();
            TestDelay++;
            if(TestDelay>80)//5*80=400
            {
                TestDelay=0;
            }  
            break;
        default:
            break;                        
    }
    
    stateCodeTransfer(); //ÔÚ´Ëµ÷ÓÃ·ÀÖ¹×´Ì¬Ã»¸ø£¬×´Ì¬»úÒÑ¾­Ìø×ß

}


//**************************************************************************************
//**************************************************************************************
//**************************************************************************************
//**************************************************************************************
