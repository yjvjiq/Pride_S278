//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : 青年浦江6123
//* File Name          : DC_ChangerProcess.c
//* Author             : 孙丽雪
//* Version            : V1.0.0
//* Start Date         : 2016,11,30
//* Description        : 快充流程
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"

unsigned char plug_DC_Connect=0;             //CC2连接信号
static unsigned char sendi1=0;               //非全局变量
static unsigned char sendi2=0;               //非全局变量
static unsigned char sendi3=0;               //非全局变量

unsigned char	m_chmmode = CONSTANT_CURT_WORK;//全局变量
unsigned char fastendflag=0;                 //全局变量,快充结束标志位
float	m_askvoltage=0;                        //依据电池包充电曲线
float	m_askcurrent=0;                        //全局变量，直流充电电流
/////////快充用,BMS停止充电机原因////////////////
unsigned char  fastend1=0;                   //全局
unsigned char  fastend2=0;
unsigned char  fastend3=0;
unsigned char  fastend4=0xf0;

/////////快充用，发送BSM，电池当前状态////////////////
unsigned char  fasterror11 =0;
unsigned char  fasterror12 =0x10;            //允许充电，0 b 00 01 00 00

unsigned char BROStopState = 0;              //收到CRO后,不再发BRO
unsigned int chargeTimes=0;                  //充电次数

//////////超时时间标志///////////////////////////////
unsigned int CRMOverTimeBefore60s = 0;//收不到CRO报文60s，系统开机后
unsigned int CRMOverTimeBefore = 0;//收不到CRO报文
unsigned int CRMOverTime = 0;//CRM超时发送0xaa超时
unsigned int CROOverTime = 0;//5s未收到CRO报文
unsigned int CROOverTime60s = 0;//CRO超时发送0xaa超时
unsigned int CCSOverTime=0;                 //CCS超时时间
unsigned int CSTOverTime=0;                 //CST超时时间
unsigned int CSDOverTime=0;                 //CSD超时时间
unsigned char OverTimeState=0;               //超时标志

unsigned char CC2ResState = 0; //0位置状态 1故障状态 2正常状态
float DCTem1=0;//DC1温度采集
float DCTem2=0;//DC2温度采集
float CC2VOL = 0;
float PowerVOL = 0;
//unsigned char DC_CC2Count = 0;
unsigned char BROErrorAA = 1;//BRO不发送0xaa
unsigned char DC_Vesion = 1;//DC充电版本1：老国标;2：新国标
unsigned char SelfState3 = 0;     //自检计数器变成3标志
unsigned char BEMStop=0;

unsigned char DCStartState=0;//快充开始标志:1收到CRM,2收到CHM

static unsigned char SetBSD = 0;//BSD是否已经发送
static unsigned char SetBST = 0;//BST是否已经发送
unsigned char ChangerStopState = 0;//充电时如果没有充满，则SOC停留在99.2%
unsigned char PantNormalEndFlag=0;//受电弓正常下电标志位

//******************************************************************************
//* Function name:   GetRequestCurrentDC
//* Description:     快充充电请求电流计算 C=120A
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
float GetRequestCurrentDC(void) 
{
    //float DCAmp;
    static unsigned char ChanceHappen1 = 0;//电压高于3.78后不恢复
    static unsigned char ChanceHappen2 = 0;//电压高于3.92后不恢复
    
    m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;
}
//**********************************************************************
//* Function name:   DCFaultDone
//* Description:     快充过程中故障的处理
//* EntryParameter : None
//* ReturnValue    : 0:无故障;1:有故障
//**********************************************************************
unsigned char DCFaultDone(void)
{
    static unsigned char ReturnValue = 0;//一旦赋值,不能改变
    //////////////////////////////断高压/////////////////////////////        
    if((Error_Group4.Bit.F6_Bat_Over_V>=2)//总电压过压2级、3级
    ||(Error_Group2.Bit.F0_Cell_Over_V>=1))//单体过压1级、2级、3级
    
    {
        fastendflag=1;
        fastend4|=0x04;//电压异常
        OffState=1;//请求下电
        ReturnValue = 1;
    }
    else if((Error_Group1.Bit.F6_Ins_Err==3)&&(g_BmsModeFlag == FASTRECHARGING))//绝缘3级故障
    {
        fastendflag=1;
        fastend2|=0x01;//绝缘异常
        OffState=1;//请求下电
        ReturnValue = 1;    
    }
    else if(Error_Group1.Bit.F4_Ch_Over_I==3) //充电电流3级
    {
        fastendflag=1;
        fastend4|=0x01;//充电电流过大
        OffState=1;//请求下电
        ReturnValue = 1;     
    } 
    else if(Error_Group0.Bit.F2_Ele_Relay_Con==3)
    {
        fastendflag=1;
        fastend2|=0x40;//连接器温度过大
        OffState=1;//请求下电
        ReturnValue = 1; 
    } 
    else if(Error_Group2.Bit.F4_Temp_Over==3)//若是出现3级温度过高故障
    {
        fastendflag=1;
        fastend3|=0x01;//电池温度过高
        OffState=1;//请求下电
        ReturnValue = 1;
    } 
    else if(((g_caution_Flag_2 &0x80)!=0)//充电机通信故障
    ||(Error_Group3.Bit.F0_Sub_Com_Err)//内网通讯故障
    ||(ACCha_Flag_BST)//充电时电流方向异常
    ||(Error_Group0.Bit.F0_Fire_Warning==3))//火灾预警3级 
 //   ||(Error_Group6.Bit.F0_Power_Vol)  ///供电电源电压异常
    {
        fastendflag=1;
        fastend3|=0x40;//其他故障
        OffState=1;//请求下电
        ReturnValue = 1;
    }
    return ReturnValue;   
    //////////////////////////////*断高压*/////////////////////////////
}

//**********************************************************************
//* Function name:   TaskRechargeDC
//* Description:     快充流程，大约每10ms调用一次该函数
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
    
    static unsigned char DCFinish=0; //直流充电时只修正Q1一次
    static unsigned char timer1S=0;//单体电压到达后,停止充电条件判断延时
    static unsigned char timer2S=0;//第一次降电流延时
    static unsigned char timer3S=0;//第二次降电流延时
    static unsigned char ChanceCurt=0;//充电末端,电流变化后不回跳,
    static unsigned char ChangeTimeState=0;//充电结束后自进行一次加法
    float curr=0;
    float curr1=0;

    unsigned char ErrorState = 0;
  
    if((g_BmsModeFlag != FASTRECHARGING)&&(g_BmsModeFlag != RECHARGING))
        return;
    
    if(g_highestTemperature>=(HIGHEST_ALLOWED_CHARGE_T+40))//40+57，40+0
    {
        fastend3|=0x01; //Temperature，中止充电
        fastendflag=1; //快充停止充电标志  
    }
    
    YoungMan_LT_FastChrgPowerAjust();
    curr =m_askcurrent_Model;
    ErrorState = DCFaultDone();
      
	if(g_BmsModeFlag == FASTRECHARGING) 
	{
		////////////请求电压//////////
		if(((Tavg-40)<=54)&&((Tavg-40)>0)&&(heatingStart==0))
		{
            m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;//请求充电电压3.6*165 =602
        }
        else if(((Tavg-40)<=0)||(((Tavg-40)<=5)&&(heatingStart)))//温度<0或者上电前温度小于0当前温度小于5
        {
            m_askvoltage = LOWEST_ALLOWED_CHARGE_V;//3.3*165 =544.5
        }
        else if((Tavg-40)>54)
        {
            m_askvoltage = 0;
        }
		
		////////////请求电流//////////
		if(((Tavg-40)<=5)&&(BeforeTempFlag1))//当前温度等于4度，上电前温度<0
            curr1=HeatCurt;
        else if(((Tavg-40)<=15)&&((BeforeTempFlag2)||(BeforeTempFlag1))) //当前温度等于15 上电前温度<10
            curr1=curr+HeatCurt;
        else
            curr1 = curr;
        
    }
    else if(g_BmsModeFlag == RECHARGING) 
    {
        m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;
        curr1 = curr; 
    }
     
    if((ErrorState==1)||(CutDCChaCurt0.word!=0)||(CutACChaCurt0.word!=0))//直流充电有下电故障处理
    {
        curr1 = 0;
        //curr1=0;     
    } 
    else 
    {
        if((CutDCChaCurt50.word!=0)||(CutACChaCurt50.word!=0))
            curr1=0.5*curr;
           
        if(curr1>=SetCap)
            curr1= SetCap;
    }
    
    if((g_highestCellVoltage>=CHARGE_CUTDOWN_CV1 )||(ChanceCurt==1))//电流为计数出来的一半
    {         
        timer2S++;
        if(timer2S>=80)//10*80=800ms
        {    
            timer2S=81;                      
            if(curr1>0.1*SetCap)
                curr1=0.1*SetCap;
            else if(curr1==0)
                curr1=0;
            
            ChanceCurt=1;
        }
    } 
    else 
    {
        timer2S=0;
    }
	  if(g_highestCellVoltage>=HIGHEST_ALLOWED_CHARGE_CV)  //3.65V停止充电
    {
        timer1S++;
        if(timer1S>=100)//10ms*100=1000ms
        {          
            if(DCFinish==0)
            {
                ChangerStopState = 1;
                SocRechargeAdjust(); //修SOC及Q1
        	      StoreParameter();

        	      DCFinish=1;
            }   
            fastendflag=1;
            fastend1|=0x10;//单体电压达到目标值，请求中止充电
            timer1S=103; 
            
            curr1=0; //请求电流降为0
        	  m_askvoltage=0;//请求电压降为0
        }
    } 
    else 
    {
        timer1S=0; 
    }
    m_askcurrent=curr1;
                  
    //////////////////////////////充电时发送功率///////////////////////////
   // BiggestFeedbackContinuous = m_askcurrent*m_askvoltage/1000;//快充时充电 
    
    ///*******///////快充发送消息 
	  /*if(CHMStep)
		{     
    	  if(CHMStep==0x01)      //1801f456 00,多帧报文，不是250ms，报文间隔10ms
    	  {

    	      counter_250ms1++;
    	      if(counter_250ms1>=25)
    	      {
        	      if(DCStartState==2)//新国标开始
        	      {
                    cpuToCHMBHM();
                    CRMOverTimeBefore++;
                    if(CRMOverTimeBefore>=120)//如果30s接收不到CRM则超时 
                    {
                        OverTimeState=1;//超时标志位置1                
                        CHMStep=0x07;
                        BEMError1|=0x01;//收不到CRM，30s后发送BEM
                        CRMOverTimeBefore = 0;
                    }
        	      }
        	      if(DCStartState==1)//检测0xaa是否超时
        	      {
        	          CRMOverTime++;
        			      if(CRMOverTime>=20)//收不到0xaa5s后上报
        			      {
            			      CHMStep=0x07;
        			          OverTimeState=1;
        			          BEMError1|=0x04;
        			          CRMOverTime = 0; 
        			      } 
        	      }
    	          counter_250ms1=0;
    	          CHMStep1 = 0;
    	      }    
    	      if(CHMStep1==0)
    	      { 
    			      if(DCStartState==1)//如果已经接收到旧国标开始
    			      {       			          
        			      if(BRMStep == 0)//如果可以发送BRM,则继续发送
        			      {
        			          BRMStep = 1;//收到CTS后,将其清零;发送完BRM后置1
        			          cpuToCHMBRM();
        			          counter_250ms1=0;
        			          sendi1++;
        			      }
        			      else if((BRMStep == 1)&&(FlagBRMSend==1))
        			      {
    			              sendi1++; 
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
            			              FlagBRMSend = 0;
            			              BRMStep = 0;//接收到CRM后开始发送BRM以及下次发送BRM的时机
            			              CHMStep1=1;
        			              }
            			      }
            			      else if(sendi1==8)
            			      {  
            			          cpuToCHMBRMDATA7();
            			          sendi1=0;
            			          FlagBRMSend = 0;
            			          BRMStep = 0;//接收到CRM后开始发送BRM以及下次发送BRM的时机        			          
            			          CHMStep1=1;
            			      }
        			      }
        			  }
    	      }
    	  }
    		if(CHMStep==0x02)   //1801f456 aa
			  {
			      if(BRMStep == 1)//已经发出来BRM都头文件
			      {
			          PGN[1] = 0x02;
			          J1939_TP_CM_Abort();
			          BRMStep = 0; 
			      }
			      counter_250ms2++;
			      if(counter_250ms2>=50)//500ms
			      {  
			          CHMStep2=0;
			          counter_250ms2=0;
			      }
			      if(CHMStep2==0)
			      { 
    			      if(BCPStep==0)
    			      {
    			          BCPStep = 1;
    			          sendi2++;
    			          cpuToCHMBCP();
    			          counter_250ms2=0;
    			      } 
    			      else if((FlagBCPSend)&&(BCPStep==1))
    			      {
        			      sendi2++;
        			      if(sendi2==2)
        			          cpuToCHMBCPDATA1();
        			      else if(sendi2==3)
        			      {
        			          sendi2=0;
        			          cpuToCHMBCPDATA2();
        			          CHMStep2=1;
        			          BCPStep = 0;
        			          FlagBCPSend = 0;
        			      }
    			      }
			      }
			  }
			  if(CHMStep==0x03)  //1808f456 aa 250ms
  		  {            
            
            if(BCPStep == 1)
            {
                PGN[1] = 0x06;
			          J1939_TP_CM_Abort();
                BCPStep = 0;
            }
            counter_250ms3++;
            if(CHMStep3==0)
            {
                CROOverTime++;
                if(SelfState3==0)
          			    cpuToCHMBRO();  //100956f4 aa
                if((CROOverTime>=21)||(CROOverTime60s>=240))//收到CRO,但没有收到0xaa,5s后超时 
                {
                    OverTimeState=1;//超时标志位置1                
                    CHMStep=0x07;
                    BEMError2|=0x04;
                    CROOverTime = 0;
                    CROOverTime60s = 0;
                }
                if(BROErrorAA==1)//充电桩不满足充电需求
                {
                    CHMStep=0x07;
                    BEMError4|=0x04;//其他故障                
                } 
                else if(0==BROErrorAA)
                {
                    CROOverTime60s++;
                    TurnOnChangerRelay = 1;
                }
                   
                CHMStep3=1;
            }
            if(counter_250ms3>=25)//确保第一帧数据发出，不必在延时小于250ms
            {
  			        counter_250ms3=0;
  			        CHMStep3=0;
            }
  		  }
    		if((CHMStep==0x04)||(CHMStep==0x05)) //50ms周期
        {
            CCSOverTime++;
            if(CCSOverTime>=100)//10ms*100=1S
            {
                OverTimeState=1;//超时标志位置1
                if(CHMStep<=0x07)                
                    CHMStep=0x07;
                BEMError3|=0x01;
                CCSOverTime = 0;    
            }
            
            if(SetBST)
                CCSOverTime = 0;      //防止由于故障，发送BST后，持续监测CCS超时
            
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
    			      
    			      if(BCSStep==0)
    			      { 
    			          sendi3++;
    			          cpuToCHMBCS();
    			          BCSStep = 1;
    			          counter_250ms4 = 0;
    			      } 
    			      else if((BCSStep == 1)&&(FlagBCSSend==1))
    			      {
    			          sendi3++;
        			      if(sendi3==2)
        			      {
        			          cpuToCHMBCSDATA1();
        			      }
        			      else if(sendi3==3)
        			      {
        			          sendi3=0;
        			          cpuToCHMBCSDATA2();
        			          CHMStep4_5=1;
        			          BCSStep = 0;     //严格讲,清零应该是收到停止报文后
        			          FlagBCSSend = 0; //严格讲,清零应该是收到停止报文后
        			      }
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
                if(counter_250ms5>=25)//确保第一帧数据发出，不必在延时小于250ms
                {
                  
      			        counter_250ms5=0;
      			        CHMStep5=0;
                }
            }
  		  }	  
			  if((CHMStep==0x06)||((CHMStep==0x05)&&(fastendflag==1)))//10ms发送一次,开始发的时候	
    		{
    			  //如果收到地面充电机充电截止报文或者单体电压、总电压超过保护值
			      if(BCSStep == 1)
    		    {
    		          PGN[1] = 0x11;
  	              J1939_TP_CM_Abort();
    		          BCSStep = 0;
    		    }
    			  if(OverTimeState==1)
    			  {
    			      if(counter_250ms6_1%25==0)
    			      {    			        
    			          counter_250ms6_1 = 0;
    			          cpuToCHMBEM();
    			      }
    			      counter_250ms6_1++;
    			  }
    			  if(SetBSD==0)//如果BSD已经发送,则BST不再发送
    			  {   			    
    			      cpuToCHMBST();
    			      SetBST=1;
    			      m_askcurrent=0;//请求电流为0
        			  CSTOverTime++;//10ms*200
    			      if(CSTOverTime>=500)//防止收不到CST报文5s延时,10ms*500 
    			      {
        			      CHMStep=0x07;
        			      OverTimeState = 1;
        			      BEMError3|=0x04;
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
    			      if(CSDOverTime>=1000)//接收不到充电机统计报文时,延时10s下电
    			      {
    			          CHMStep=0x07;
    			          OverTimeState = 1;
    			          BEMError4|=0x01;
    			          CSDOverTime = 0;     			          
    			      }    			      
    			  }
			  }        
			  if(CHMStep==0x07) //快充充电结束 
			  {     
	          if(counter_250ms7%25==0)
	          {
	              if((OverTimeState==1)||(BROErrorAA==1))//如果有超时故障才上报,没有不上报
	              {	                
	                  if(BEMStop==0)
	                      cpuToCHMBEM();
	              }
	              counter_250ms7 = 0;
	          }
	          counter_250ms7++;
	          OffState=1;//状态机由170跳转180的标志位 
	          Error_Group1.Bit.St_CHG_Allow=1; //充电允许状态位不允许
	          PantNormalEndFlag=1;//受电弓正常下电

			  }
												
		}/////////END快充发送消息
		
		
		*/
		if(CHMStep)
		{     
    	  
    	  if(CHMStep==0x01)      //1801f456 00,多帧报文，不是250ms，报文间隔10ms
    	  {
            
    	      counter_250ms1++;
    	      if(counter_250ms1>=25)
    	      {
        	      if(DCStartState==2)//新国标开始
        	      {
                    cpuToCHMBHM();
                    CRMOverTimeBefore++;
                    if(CRMOverTimeBefore>=120)//如果30s接收不到CRM则超时 
                    {
                        OverTimeState=1;//超时标志位置1                
                        CHMStep=0x07;
                        BEMError1|=0x01;//收不到CRM，30s后发送BEM
                        CRMOverTimeBefore = 0;
                    }
        	      }
        	      if(DCStartState==1)//检测0xaa是否超时
        	      {
        	          CRMOverTime++;
        			      if(CRMOverTime>=20)//收不到0xaa5s后上报
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
    			      if(DCStartState==1)//如果已经接收到旧国标开始
    			      {       			          
        			      sendi1++;
        			      if(sendi1==1)
        			          cpuToCHMBRM();
        			      else if(sendi1==2)
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
    			              }
        			      }
        			      else if(sendi1==8)
        			      {  
        			          cpuToCHMBRMDATA7();
        			          sendi1=0;        			          
        			          CHMStep1=1;
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
    			      if(sendi2%3==1)
    			          cpuToCHMBCP();
    			      if(sendi2%3==2)
    			          cpuToCHMBCPDATA1();
    			      else if(sendi2%3==0)
    			      {
    			          sendi2=0;
    			          cpuToCHMBCPDATA2();
    			          CHMStep2=1;
    			      }
    			      sendi1=0;//清除第一组计数
			      }
			  }
			  if(CHMStep==0x03)  //1808f456 aa 250ms
  		  {            
            counter_250ms3++;
            if(CHMStep3==0)
            {
                CROOverTime++;
                if(SelfState3==0)
          			    cpuToCHMBRO();  //100956f4 aa
                if((CROOverTime>=20)||(CROOverTime60s>=240))//收到CRO,但没有收到0xaa,5s后超时 
                {
                    OverTimeState=1;//超时标志位置1                
                    CHMStep=0x07;
                    BEMError2|=0x04;
                    CROOverTime = 0;
                    CROOverTime60s = 0;
                }
                if(BROErrorAA==1)//充电桩不满足充电需求
                {
                    CHMStep=0x07;
                    BEMError4|=0x04;//其他故障                
                } 
                else if(0==BROErrorAA)
                {
                    CROOverTime60s++;
                    TurnOnChangerRelay = 1;
                }
                   
                CHMStep3=1;
            }
            if(counter_250ms3>=25)//确保第一帧数据发出，不必在延时小于250ms
            {
              
  			        counter_250ms3=0;
  			        CHMStep3=0;
  			        
            }
  		  }
    		if((CHMStep==0x04)||(CHMStep==0x05)) //50ms周期
        {
           
            CCSOverTime++;
            if(CCSOverTime>=100)//10ms*100=1S
            {
                OverTimeState=1;//超时标志位置1
                if(CHMStep<=0x06)                
                    CHMStep=0x06;
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
    			      if(sendi1%3==1)
    			          cpuToCHMBCS();
    			      else if(sendi1%3==2)
    			      {
    			          cpuToCHMBCSDATA1();
    			      }
    			      else if(sendi1%3==0)
    			      {
    			          sendi1=0;
    			          cpuToCHMBCSDATA2();
    			          CHMStep4_5=1;
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
                if(counter_250ms5>=25)//确保第一帧数据发出，不必在延时小于250ms
                {
                  
      			        counter_250ms5=0;
      			        CHMStep5=0;
      			        
                }
            }
  		  }	  
			  if((CHMStep==0x06)||((CHMStep==0x05)&&(fastendflag==1)))//10ms发送一次,开始发的时候	
    		{
    			  //如果收到地面充电机充电截止报文或者单体电压、总电压超过保护值
    			 
    			  if(OverTimeState==1)
    			  {
    			      if(counter_250ms6_1%25==0)
    			      {    			        
    			          counter_250ms6_1 = 0;
    			          cpuToCHMBEM();
    			      }
    			      counter_250ms6_1++;
    			  }
    			  if(SetBSD==0)//如果BSD已经发送,则BST不再发送
    			  {   			    
    			      cpuToCHMBST();
    			      m_askcurrent=0;//请求电流为0
        			  CSTOverTime++;//10ms*200
    			      if(CSTOverTime>=500)//防止收不到CST报文5s延时,10ms*500 
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
    			      if(CSDOverTime>=1000)//接收不到充电机统计报文时,延时10s下电
    			      {
    			          CHMStep=0x07;
    			          OverTimeState = 1;
    			          BEMError4|=0x01;
    			          CSDOverTime = 0;     			          
    			      }    			      
    			      
    			  }
			  }        
			  if(CHMStep==0x07) //快充充电结束 
			  {     
	          
	          if(counter_250ms7%25==0)
	          {
	              if((OverTimeState==1)||(BROErrorAA==1))//如果有超时故障才上报,没有不上报
	              {	                
	                  if(BEMStop==0)
	                      cpuToCHMBEM();
	              }
	              counter_250ms7 = 0;
	          }
	          counter_250ms7++;
	          OffState=1;//状态机由170跳转40的标志位 
	          Error_Group1.Bit.St_CHG_Allow=1; //充电允许状态位不允许
	          PantNormalEndFlag=1;//受电弓正常下电              
			  }
												
		}/////////END快充发送消息
}
//******************************************************************************
//* Function name:   TaskDC
//* Description:     10ms中断中调用该函数
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void TaskDC(void)
{  
    if((FASTRECHARGING==g_BmsModeFlag)||(RECHARGING==g_BmsModeFlag)) ////快充或受电弓充电
    {
        if(DCStartState == 0)//当接收到之后，不再计时
            CRMOverTimeBefore60s++;        
        if(CRMOverTimeBefore60s>=6000)//10ms*6000
        {
            OverTimeState=1;//超时标志位置1                
            CHMStep=0x07;
            BEMError1|=0x01;//收不到CRM，30s后发送BEM
            CRMOverTimeBefore60s = 0;      
        }
        if((stateCode==110)||(stateCode==170)||(OverTimeState==1))        
            TaskRechargeDC();
    }
}

//******************************************************************************
//* Function name:   GetDCTem
//* Description:     检测直流充电的温度
//* EntryParameter : None
//* ReturnValue    : None
	
	//	TurnOn_CC2();//12V   电压切换在初始化BMS里
//******************************************************************************
void GetDCTem (void)
{
    float getAD1=0;//AD值1
    float getAD2=0;//AD值2
    unsigned int GetADVol1 = 0;
    unsigned int GetADVol2 = 0;
    float VolValue1=0;//转换完的电压值1 温度1
    float VolValue2=0;//转换完的电压值2 温度2
    float VolValue3=0;//转换完的电压值1 CC2电压
    float VolValue4=0;//转换完的电压值2 电源电压
    
    float R1=0;//温感头1阻值
    float R2=0;//温感头2阻值  

    getAD1 = readADC_SingleEnded(0); //T1
    getAD2 = readADC_SingleEnded(3); //T2
    
    GetADVol1 = readADC_SingleEnded(1);//Vol of CC2
    GetADVol2 = readADC_SingleEnded(2); //Vol of 电源
    //EnableInterrupts; //开中断，接收状态机编码 
    VolValue1=4.096*(float)getAD1/2048.0;     //(2048-0)/(4.096-0)=(AD1-0)/(ADX-0)
    VolValue2=4.096*(float)getAD2/2048.0;
    
    VolValue3=4.096*(float)GetADVol1/2048.0; //cc2
    VolValue4=4.096*(float)GetADVol2/2048.0; //电源电压
    
    CC2VOL = (75.0+12.0)*VolValue3/12.0; //CC2电压
    PowerVOL = (75.0+12.0)*VolValue4/12.0; //电源电压
 
    R1=VolValue1/(5-VolValue1)*10;
    R2=VolValue2/(5-VolValue2)*10;
    
    DCTem1=lookup_Table_Tem_NEW20170204_custom(VolValue1);
    
    DCTem2=lookup_Table_Tem_NEW20170204_custom(VolValue2);
    
    
}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************