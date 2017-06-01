//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : ProcessBMU.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2014.5.7
//* Description        : 该文件专门用于处理接收到的BMU数据，从而得出单体极值和单体值
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h" 


#pragma DATA_SEG __GPAGE_SEG PAGED_RAM  
U8 g_group;  //BMU1 组号， 从0~18
U16 g_singleCellVoltage[BMU_NUMBER][SIX802_NUMBER][CELL_NUMBER];  //BMU1号, 6802（组）号, 单体电池号
U8 g_singleCellTemperature[BMU_NUMBER][SIX802_NUMBER][Tem_NUMBER];//BMU1号, 6802（组）号, 单体电池号
#pragma DATA_SEG DEFAULT 


float g_highestCellVoltage=0; //单体最高电压
float g_lowestCellVoltage=0;  //单体最低电压
U8 g_highestCellVoltage_Num=0; //单体最高电压编号
U8 g_lowestCellVoltage_Num=0;  //单体最低电压编号

float g_averageVoltage;       //平均单体电压
float g_systemVoltage;      //系统电压=单体电压累加总电压

U8 g_highestTemperature=0; //单体最高温度
U8 g_lowestTemperature=0;  //单体最低温度
U8 g_highestTemperature_Num=0; //单体最高温度编号
U8 g_lowestTemperature_Num=0;  //单体最低温度编号

U8 g_averageTemperature; //单体平均温度
U8 Tavg; //电芯平均温度

U8 LowVolNum=0;
U8 HighVolNum=0;
U8 HighTemNum=0;
U8 LowTemNum=0;

U8 highestCellVolNum=0;
U8 lowestCellVolNum=0;
U8 highestCellTempNum=0;
U8 lowestCellTempNum=0;

U16 g_singleCellVmax[BMU_NUMBER];
U16 g_singleCellVmin[BMU_NUMBER];
U8 g_singleCellVmax_Num[BMU_NUMBER];
U8 g_singleCellVmin_Num[BMU_NUMBER];           
U8 g_singleCellTmax[BMU_NUMBER];
U8 g_singleCellTmin[BMU_NUMBER];
U8 g_singleCellTmax_Num[BMU_NUMBER];
U8 g_singleCellTmin_Num[BMU_NUMBER]; 
 
U8 g_bmu2_number_v[BMU_NUMBER];
U8 g_cell_number_v[BMU_NUMBER][5];
U8 g_bmu2_number_t[BMU_NUMBER];
U8 g_cell_number_t[BMU_NUMBER][5];

//unsigned long recogBMUtoBMSmessage;//BMS BMU通信辨识信息
U8 recogBMStoBMUflag = 0;//BMS与BMU辨识成功标志
//U8 g_cellVol[CELL_VOL_GROUP][6];//
//U8 g_cellTemperature[CELL_TEMP_GROUP][6];
U8 g_cellVol[BMU_NUMBER][36]; // re-group all the cell voltage. 18 BMU and 36 cells in each BMU.
U8 g_cellTemperature[BMU_NUMBER][3]; // re-group all the cell temperature. 18 BMU and 3 temperatures in each BMU.
	
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
U32 g_circleFlag = 0; //一个循环完成的标志
U32 g_configFlag = 0;//收到配置信息标志，用于判断BMU个数，6805组数和单体个数 

//******************************************************************************
//* Function name:   BMU_initial
//* Description:     上电接收BMU所有数据，确保单体温度和电压都没问题
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMU_initial(void) 
{
    U16 t=0;
    
    //***********BMU自检*******************************************       
    while((!bmuProcess2())&&(t<65000))    	        
    {        
        t++;      
        if(t>=60000) //超时故障报告
        {
            //t= 60000;
            //Can08f0Byte5.Bit.F3_innerComm=1;  //t0 vcu
            g_caution_Flag_3 |=0x01;    //to pc
                     
        }/////end of BMU通信判断  
        else 
        {            
            //Can08f0Byte5.Bit.F3_innerComm=0; //to vcu 
            g_caution_Flag_3 &=0xfe;  //不上报内部通讯故障   to pc 
        }  
        _FEED_COP();   //2s内不喂内狗，则系统复位 
    }

}
//******************************************************************************
//* Function name:   BMU_Processure
//* Description:     对接收到的BMU单体电压和温度进行解析
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void BMU_Processure(void)
{
    U32 framID;
    U16 i,boxNumber=0;
	   
    if((Int_Flag&0x08)==0x08) //若接收到1个数据，进行数据处理
    {
        Int_Flag &= 0xf7;//清中断标志		
          			  		
        g_group = 0;  		


        framID = g_mboxID;      
        g_group = framID&0x000000ff;
        g_group--;
        /*
        framID = framID>>4;
        framID = framID & 0x03ffffff;
        */
        framID = framID>>8;
        framID = framID & 0x003fffff;

		switch(framID)  //解析数据
		{
            case 0x018FF11://BMU标识信息；硬件版本、软件版本、通讯协议版本等；0x18FF110x
                //if(recogBMUtoBMSmessage != bufL)
                //	recogBMStoBMUflag = 0; //辨识失败
                //else
                recogBMStoBMUflag = 1; //辨识成功								
                break;	  
            case 0x018FF13://电池配置信息1，6802的个数、电池采压数量、采温数量；CC2
                switch(g_group) 
                {
                    case 0:
                      g_configFlag |= 0x00000001;
                      break;
                    case 1:
                      g_configFlag |= 0x00000002;
                      break;
                    case 2:
                      g_configFlag |= 0x00000004;
                      break;
                    case 3:
                      g_configFlag |= 0x00000008;
                      break;
                    case 4:
                      g_configFlag |= 0x00000010;
                      break;
                    case 5:
                      g_configFlag |= 0x00000020;
                      break;
                    case 6:
                      g_configFlag |= 0x00000040;
                      break;
                    case 7:
                      g_configFlag |= 0x00000080;
                      break;
                    case 8:
                      g_configFlag |= 0x00000100;
                      break;
                    case 9:
                      g_configFlag |= 0x00000200;
                      break;
                    case 10:
                      g_configFlag |= 0x00000400;
                      break;
                    case 11:
                      g_configFlag |= 0x00000800;
                      break;
                    case 12:
                      g_configFlag |= 0x00001000;
                      break;
                    case 13:
                      g_configFlag |= 0x00002000;
                      break;
                    case 14:
                      g_configFlag |= 0x00004000;
                      break;
                    case 15:
                      g_configFlag |= 0x00008000;
                      break;
                    case 16:
                      g_configFlag |= 0x00010000;
                      break;
                    case 17:
                      g_configFlag |= 0x00020000;
                      break;      				                				          
                }
			      
			    g_bmu2_number_v[g_group] = g_mboxData[boxNumber][0]&0x07; //&0x07;//BMU1带6802的个数
				g_bmu2_number_t[g_group] = (g_mboxData[boxNumber][0]&0x70)>>4; //&0x07;
        
				g_cell_number_v[g_group][0]=g_mboxData[boxNumber][1]&0x0f;
				g_cell_number_t[g_group][0]=(g_mboxData[boxNumber][1]&0xf0)>>4;

				g_cell_number_v[g_group][1]=g_mboxData[boxNumber][2]&0x0f;
				g_cell_number_t[g_group][1]=(g_mboxData[boxNumber][2]&0xf0)>>4;

				g_cell_number_v[g_group][2]=g_mboxData[boxNumber][3]&0x0f;
				g_cell_number_t[g_group][2]=(g_mboxData[boxNumber][3]&0xf0)>>4;

				g_cell_number_v[g_group][3]=g_mboxData[boxNumber][4]&0x0f;
				g_cell_number_t[g_group][3]=(g_mboxData[boxNumber][4]&0xf0)>>4;

				g_cell_number_v[g_group][4]=g_mboxData[boxNumber][5]&0x0f;	
				g_cell_number_t[g_group][4]=(g_mboxData[boxNumber][5]&0xf0)>>4;	
																	
			    break;
            case 0x018FF14://电池配置信息3，6802的个数、电池采压数量、采温数量；CC3
				break;
		    case 0x018FF16:   //判断生命信号 是否接收完一个周期
                switch(g_group) 
                {
                    case 0:
                      g_circleFlag |= 0x00000001;
                      break;
                    case 1:
                      g_circleFlag |= 0x00000002;
                      break;
                    case 2:
                      g_circleFlag |= 0x00000004;
                      break;
                    case 3:
                      g_circleFlag |= 0x00000008;
                      break;
                    case 4:
                      g_circleFlag |= 0x00000010;
                      break;
                    case 5:
                      g_circleFlag |= 0x00000020;
                      break;
                    case 6:
                      g_circleFlag |= 0x00000040;
                      break;
                    case 7:
                      g_circleFlag |= 0x00000080;
                      break;
                    case 8:
                      g_circleFlag |= 0x00000100;
                      break;
                    case 9:
                      g_circleFlag |= 0x00000200;
                      break;
                    case 10:
                      g_circleFlag |= 0x00000400;
                      break;
                    case 11:
                      g_circleFlag |= 0x00000800;
                      break;
                    case 12:
                      g_circleFlag |= 0x00001000;
                      break;
                    case 13:
                      g_circleFlag |= 0x00002000;
                      break;
                    case 14:
                      g_circleFlag |= 0x00004000;
                      break;
                    case 15:
                      g_circleFlag |= 0x00008000;
                      break;
                    case 16:
                      g_circleFlag |= 0x00010000;
                      break;
                    case 17:
                      g_circleFlag |= 0x00020000;
                      break;      				                				          
                }
                //State_Box_Online=g_circleFlag;
                break;
            //******group#1*******
            //********************
            case 0x0018FF21://组号＃1，电池的采压值＃1，可采集4路电压:
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][0][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF22://组号＃1，电池的采压值＃2，可采集4路电压:
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][0][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF23://组号＃1，电池的采压值＃3，可采集4路电压；  				  
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][0][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                } 
                break;  			  		
            case 0x0018FF37://组号＃1，电池的欠压过压标志
            	//g_groupUVflag[g_group][0] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][0] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;  				
            case 0x0018FF41://组号＃1，电池的采温值＃1，可杉?路温度；
            	for(i=0;i<2;i++)
            	    g_singleCellTemperature[g_group][0][i]= g_mboxData[boxNumber][i];
            	break; 	
            //******** group#2**********
            //**************************
            case 0x0018FF24://组号＃2，电池的采压值＃1，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][1][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF25://组号＃2，电池的采压值＃2，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][1][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF26://组号＃2，电池的采压值＃3，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][1][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;  					
            case 0x0018FF38://组号＃2，电池的欠压过压标志
            	//g_groupUVflag[g_group][1] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][1] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;     			  
            case 0x0018FF42://组号＃2，电池的采温值＃1，可采集8路温度；  				 
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][1][i]= g_mboxData[boxNumber][i];
                }
                break;
            //******group#3********
            //*********************
            case 0x0018FF27://组号＃3，电池的采压值＃1，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][2][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF28://组号＃3，电池的采压值＃2，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][2][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF29://组号＃3，电池的采压值＃3，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][2][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF39://组号＃3，电池的欠压过压标志
                //g_groupUVflag[g_group][2] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
                //g_groupOVflag[g_group][2] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
                break; 					
            case 0x0018FF43://组号＃3，电池的采温值＃1，可杉?路温度；  				
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][2][i]= g_mboxData[boxNumber][i];
                }
                break;  	
            //*******group#4*******
            //*********************
            case 0x0018FF2a://组号＃4，电池的采压值＃1，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][3][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2b://组号＃4，电池的采压值＃2，可采集4路电压；
                for(i=0;i<4;i++)
                {
					g_singleCellVoltage[g_group][3][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2c://组号＃4，电池的采压值＃3，可采集4路电压； 
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][3][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF3a://组号＃4，电池的欠压过压标志  				  
            	//g_groupUVflag[g_group][3] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][3] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;  				
            case 0x0018FF44://组号＃4，电池的采温值＃1，可采集8路温度  				 
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][3][i]= g_mboxData[boxNumber][i];
                }
                break;
            //******group#5********
            //*********************
            case 0x0018FF2d://组号＃5，电池的采压值＃1，可采集4路电压；	
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][4][i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2e://组号＃5，电池的采压值＃2，可采集4路电压；
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][4][4+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;
            case 0x0018FF2f://组号＃5，电池的采压值＃3，可采集4路电压；  
                for(i=0;i<4;i++)
                {
                    g_singleCellVoltage[g_group][4][8+i]=g_mboxData[boxNumber][2*i+1]|(unsigned int)g_mboxData[boxNumber][2*i]<<8;
                }
                break;  					
            case 0x0018FF3b://组号＃5，电池的欠压过压标志
            	//circleFlag |= 0x10000000;
            	//g_groupUVflag[g_group][4] = g_mboxData[boxNumber][1]|(unsigned int)g_mboxData[boxNumber][0]<<8;
            	//g_groupOVflag[g_group][4] = g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
            	break;  					
            case 0x0018FF45://组号＃5，电池的采温值＃1，可采集8路温度；  			
                for(i=0;i<2;i++){
                    g_singleCellTemperature[g_group][4][i]= g_mboxData[boxNumber][i];
                }
                break;
                
            /* new add, the max and min voltage/temperature in erery pack */
		    case 0x0018FF36://组号＃1，电压最值   3601 表示BMU1的所有口的电压最值
                g_singleCellVmax[g_group]= g_mboxData[boxNumber][3]|(unsigned int)g_mboxData[boxNumber][2]<<8;
                g_singleCellVmin[g_group]= g_mboxData[boxNumber][5]|(unsigned int)g_mboxData[boxNumber][4]<<8;
                g_singleCellVmax_Num[g_group]= g_mboxData[boxNumber][6];
                g_singleCellVmin_Num[g_group]= g_mboxData[boxNumber][7];
                break;       	
            case 0x0018FF4B://组号＃1，温度最值       
                g_singleCellTmax[g_group]= g_mboxData[boxNumber][0];
                g_singleCellTmin[g_group]= g_mboxData[boxNumber][1];
                g_singleCellTmax_Num[g_group]= g_mboxData[boxNumber][2];
                g_singleCellTmin_Num[g_group]= g_mboxData[boxNumber][3];
                break;
            default:
                break;
		} // end switch(framID)  //解析数据
    }
}
//******************************************************************************
//* Function name:   HighGroup
//* Description:     最值模组位置确定
//* EntryParameter : None
//* ReturnValue    : 1，收到所有信息；0：未收到所有信息
//******************************************************************************
unsigned char ModelLocation(unsigned char Addr) 
{
    if((Addr==0)||(Addr==1))
        return 1;
    else
        return 2;
}
//******************************************************************************
//* Function name:   bmuProcess2
//* Description:     收到所有单体信息后，算出单体极值
//* EntryParameter : None
//* ReturnValue    : 1，收到所有信息；0：未收到所有信息
//******************************************************************************
unsigned char bmuProcess2(void)//
{
    U16         buf=0,Cell_V_Max=0,Cell_V_Min=0;
    U8          Cell_V_Max_Num=0,Cell_V_Min_Num=0;
    U8          Cell_V_Max_Pack_Num=0,Cell_V_Min_Pack_Num=0;
    
    U8          Cell_T_Max=0,Cell_T_Min=0;
    U8          Cell_T_Max_Num=0,Cell_T_Min_Num=0;  
    U8          Cell_T_Max_Pack_Num=0,Cell_T_Min_Pack_Num=0;
     
    U16         i,j,k,h,t,boxNumber=0,count=0;
    U32         sum=0;
    U16         cell[650];
	U8			temperature[54];
    U8          ti=0,ci=0;
    U8          HighBMUAddr=0,HighBMUGroupNum=0,HighBMUNum=0;
    U8          LowBMUAddr=0,LowBMUGroupNum=0,LowBMUNum=0;
    static U8   ErrorBMUAddr=0;
    static U8   ErrorBMUGroup=0;
    static U8   ErrorBMULocation=0;
    static U8   TemLossState=0;
    static U8   TemLossTime=0;
    U8          cnt = 0;
    U8			pack_cnt = 0;
    U8			cell_cnt = 0;
    
    for(i=0;i<650;i++)
    {
        cell[i]=0;
    }	

    if((g_circleFlag==G_BMU_CIRCLE_FLAG)&&(g_configFlag==G_BMU_CIRCLE_FLAG))//如果收到所有的报文，则处理    
    {    
        g_circleFlag=0; //配置信息1分钟才发一次，所以不能在这里把它的标志位清掉。 		  

        //计算电池单体最高和最低电压,温度                                          
        //***单体电压极值处理***********************************************************************************		      			
        Cell_V_Max = g_singleCellVmax[0];
        Cell_V_Min = g_singleCellVmin[0];
        Cell_V_Max_Num = g_singleCellVmax_Num[0];
        Cell_V_Min_Num = g_singleCellVmin_Num[0];
        
        for(cnt=0;cnt<BMU_NUMBER;cnt++)
        {    						      						     						    
            if(g_singleCellVmax[cnt] >= Cell_V_Max)
            {
                Cell_V_Max = g_singleCellVmax[cnt];
                Cell_V_Max_Num = g_singleCellVmax_Num[cnt];
                Cell_V_Max_Pack_Num = cnt;
            }
            if(g_singleCellVmin[cnt] <= Cell_V_Min)
            {
                Cell_V_Min = g_singleCellVmin[cnt];
                Cell_V_Min_Num = g_singleCellVmin_Num[cnt];
                Cell_V_Min_Pack_Num = cnt;
            }
            
            /* the max and min cell voltage store array */
            g_storageSysVariableCell[cnt*2]       = g_singleCellVmax[cnt];
            g_storageSysVariableCell[cnt*2 + 1]   = g_singleCellVmin[cnt];
        }

        g_highestCellVoltage        = (float)Cell_V_Max / 10000;
        g_lowestCellVoltage         = (float)Cell_V_Min / 10000; 
        g_highestCellVoltage_Num    = Cell_V_Max_Num;
        g_lowestCellVoltage_Num     = Cell_V_Min_Num;     
               
        //***单体温度极值处理***********************************************************************************		      			
        Cell_T_Max = g_singleCellTmax[0];
        Cell_T_Min = g_singleCellTmin[0];
        Cell_T_Max_Num = g_singleCellTmax_Num[0];
        Cell_T_Min_Num = g_singleCellTmin_Num[0];

        for(cnt=0;cnt<BMU_NUMBER;cnt++)
        {    						
            if(g_singleCellTmax[cnt] >= Cell_T_Max)
            {
                Cell_T_Max = g_singleCellTmax[cnt];
                Cell_T_Max_Num = g_singleCellTmax_Num[cnt];
                Cell_T_Max_Pack_Num = cnt;
            }
            if(g_singleCellTmin[cnt] <= Cell_T_Min)
            {
                Cell_T_Min = g_singleCellTmin[cnt];
                Cell_T_Min_Num = g_singleCellTmin_Num[cnt];
                Cell_T_Min_Pack_Num = cnt;
            }
            
            sum += (U32)(g_singleCellTmax[cnt] + g_singleCellTmin[cnt]);
            
            /* the max and min cell temperature store array */
            g_storageSysVariableT[cnt*2]     = g_singleCellTmax[cnt] & 0x00ff;
            g_storageSysVariableT[cnt*2 + 1] = g_singleCellTmin[cnt] & 0x00ff;
        } 			

        g_lowestTemperature = Cell_T_Min;
        g_highestTemperature = Cell_T_Max;   //注意温度有48的偏移量
        g_lowestTemperature_Num = Cell_T_Min_Num;
        g_highestTemperature_Num = Cell_T_Max_Num;   //注意温度有48的偏移量
        
        sum = sum - g_highestTemperature - g_lowestTemperature;
        Tavg = (U8)(sum / ((U32)BMU_NUMBER*2 - 2));
        g_averageTemperature = Tavg;
        
        //将单体温度重新分组5组。且转换成char类型
		count = 0;
		for(i=0;i<BMU_NUMBER;i++){
			for(j=0;j<g_bmu2_number_t[i];j++){
				for(k=0;k<g_cell_number_t[i][j];k++){
					//g_cellTemperature[i][j] = g_singleCellTemperature[i][j][k]&0x00ff;
					temperature[count] = g_singleCellTemperature[i][j][k] & 0x00ff;
					count++;
				}
			}
		}
		
        ti = 0;
        for(i=0;i<CELL_TEMP_GROUP;i++)
        {
            for(j=0;j<6;j++)
            {
            	if(ti < count){
            	    g_cellTemperature[i][j] = (U8)(temperature[ti]);//分辨率为-50所以此处+10
            	}
            	else{
            	    g_cellTemperature[i][j] = 0;
            	}
            	ti++;
            }
        }
        	
        //将单体电压重新分组，45组。且转换成char类型
		h = 0;
		for(i=0;i<BMU_NUMBER;i++){
			for(j=0;j<g_bmu2_number_v[i];j++){
				for(k=0;k<g_cell_number_v[i][j];k++){
					cell[h] = g_singleCellVoltage[i][j][k];
					h++;
				}
			}
		}

        ci = 0;
        for(i=0;i<CELL_VOL_GROUP;i++)//
        {
        	for(j=0;j<3;j++)
        	{
        		if(ci<h)
        		{   
        		    g_cellVol[i][2*j] = (U8)(cell[ci]); 
        		    g_cellVol[i][2*j+1] = (cell[ci])>>8;
        		    
        		}
        		else
        		{    					  
        		    g_cellVol[i][2*j] = 0;
        		    g_cellVol[i][2*j+1] = 0;
        		}
        		ci++;
        	}
        }
        //************************************************************************************************
        for(i=0;i<1;i++)  //清空接收缓存
        {
            g_mboxID=0;
            for(j=0;j<8;j++){
                g_mboxData[0][j] = 0;
            }
        }
        
        if((g_lowestCellVoltage==0)||(g_lowestTemperature==0)||(TemLossState==1))              
            return 0;
        else 
            return 1;	 //全部数据处理完成
  	} // end of circleflag	
    return 0;
}

//**********************************************************************
//**********************************************************************
