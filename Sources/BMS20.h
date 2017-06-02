//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : BMS.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ¸ÃÎÄ¼þ×÷ÎªÏµÍ³Í·ÎÄ¼þ£¬¶Ô¸ÃÏîÄ¿µÄÏµÍ³²ÎÊý½øÐÐ¶¨ÒåºÍ¶ÔËùÓÐÈ«¾Ö±äÁ¿½øÐÐÈ«¾ÖÉùÃ÷
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------Ã¿¸öÏîÄ¿±ØÐëÒªÐÞ¸ÄµÄ²ÎÊý---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

///////////////////////////////////ÏµÍ³²ÎÊý//////////////////////////////////

#define PROJECT_NAME                278     //ÏîÄ¿´úºÅ

#define StoreAHState                0       //¶¨ÒåÊÇ·ñ¿ÉÒÔ´æ´¢
#define BMU_NUMBER                  18      //BMU¸öÊý
#define G_BMU_CIRCLE_FLAG           0x3FFFF //BMU¸öÊý
#define CELL_NUM                    612     //µç³Ø½ÚÊý
#define CELL_TEMP_NUM               51      //15Ã—3 +ï¼“Ã—ï¼’ï¼ï¼•ï¼‘ï¼Œï¼‘ï¼˜ï¼Šï¼“ï¼ï¼•ï¼”
#define CELL_VOL_GROUP              304     //the number of cell voltage group, 612/3=304, 3 cells/group
#define CELL_TEMP_GROUP             17      //the number of cell temperature group, 51/3=17, 3 temps/group
//#define C                         150     //³äµç±¶ÂÊ
#define SetCap                      150     //³äµç±¶ÂÊ  ÎªÁËÇø·ÖC  C²éÕÒÆðÀ´Ì«¶àÁË  
#define DubugMode                   1       //·¢ËÍ×´Ì¬»ú±¨ÎÄ

//////////////////////////////////Ö±Á÷¿ì³ä//////////////////////////////////
#define HIGHEST_ALLOWED_CHARGE_CV   3.65    //×î´óÔÊÐí³äµçµ¥ÌåµçÑ¹ CV
#define HIGHEST_ALLOWED_CHARGE_V    744.6   //×î´óÔÊÐí³äµç×ÜµçÑ¹ V   3.65*204=744.6
#define LOWEST_ALLOWED_CHARGE_V     673.2   //×îµÍÔÊÐí³äµç×ÜµçÑ¹ V   3.3*204=673.2

#define HIGHEST_ALLOWED_CHARGE_T    57      //×î´óÔÊÐí³äµçºÍ¹¤×÷ÎÂ¶È   57
#define LOWEST_ALLOWED_CHARGE_T     0       //×îµÍÔÊÐí³äµçÎÂ¶È   0
#define LOWEST_ALLOWED_WORK_T       -20     //×îµÍÔÊÐí¹¤×÷ÎÂ¶È   -20

#define CHARGE_CUTDOWN_CV1          3.55    //µÚÒ»´Î½µÁ÷µçÑ¹
#define CHARGE_CUTDOWN_CV2          3.55    //µÚ¶þ´Î½µÁ÷µçÑ¹
#define HIGHEST_ALLOWED_CHARGE_A    150.0   //Ö±Á÷×î´óÔÊÐí³äµçµçÁ÷ A
#define SYS_NOMINAL_AH              150     //ÏµÍ³¶î¶¨ÈÝÁ¿ AH
#define SYS_NOMINAL_V               652.8   //ÏµÍ³¶î¶¨µçÑ¹ V//3.2*204= 652.8
#define SYS_KWH                     97.9    //ÏµÍ³ÄÜÁ¿  //150*652.8=97920WH
#define LOWEST_ALLOWED_DISCHARGE_V  510     //×îÐ¡ÔÊÐí³äµç×ÜµçÑ¹ V   2.5*204=510
#define CONSTANT_VOL_WORK           1       //ºãÑ¹¹¤×÷
#define CONSTANT_CURT_WORK          2       //ºãÁ÷¹¤×÷
#define CELL_TYPE                   3       //01Ç¦Ëáµç³Ø;02ÄøÇâµç³Ø;03:Á×ËáÌúï®;04:ÃÌËáÌúï®;
                                            //05:îÜËáï®;06:ÈýÔª;07:¾ÛºÏÎï;08:îÑËáï®;FFH:ÆäËû
                                          
#define HIGHEST_VOL                 7344    //CML³äµç»ú¹æ¸ñ:×î´óÊä³öÄÜÁ¦  3.6*204*10
#define LOWEST_VOL                  6528    //CML³äµç»ú¹æ¸ñ:×îÐ¡Êä³öÄÜÁ¦   3.2*204*10

#define CC2VOLHIGH                  7.5     //CC2²âÁ¿·¶Î§¸ß
#define CC2VOLLOW                   4.2     //CC2²âÁ¿·¶Î§µÍ

////////////////////////¼ÓÈÈ²ÎÊýÅäÖÃ//////////////////////////////////////////////
#define HIGHEST_ALLOWED_HEAT_CHARGE_V       673.2   //×î´óÔÊÐí¼ÓÈÈ³äµç×ÜµçÑ¹ V   3.3*204=673.2
#define HIGHEST_ALLOWED_HEAT_CHARGE_C       2.13    //×î´óÔÊÐí¼ÓÈÈ³äµçµçÁ÷ A                       ??
#define START_ALLOWED_PreHEAT_CHARGE_TEM    0       //¿ªÆôÔ¤¼ÓÈÈ·§Öµ  ÉÏµçÈç¹ûÐ¡ÓÚ´ËÖµ Ö±½Ó½øÈë¼ÓÈÈ                               ??
#define STOP_ALLOWED_PreHEAT_CHARGE_TEM     5       //¹Ø±ÕÔ¤¼ÓÈÈ·§Öµ                                  ??
#define START_ALLOWED_HEAT_CHARGE_TEM       10      //¿ªÆô¼ÓÈÈ·§Öµ ÉÏµçÈç¹ûÐ¡ÓÚ´ËÖµ ½øÈë±ß³äµç±ß¼ÓÈÈ 
#define STOP_ALLOWED_HEAT_CHARGE_TEM        15      //¹Ø±Õ¼ÓÈÈ·§Öµ 
#define BACK_HEAT_TEM                       -1      //¹Ø±Õ¼ÓÈÈ·§Öµ 
#define COOL_TYPE                           1       //1 ×ÔÈ»ÀäÈ´ 2£º·çÉÈÀäÈ´ 3£ºË®Àä

///////////////////////BMS×îºó±£»¤µÄ·§Öµ///////////////////////
#define LOWEST_CELL_VOL     2.0     //·Åµç×îÐ¡µ¥ÌåµçÑ¹
#define HIGHEST_CELL_VOL    3.85    //³äµçµ¥Ìå×î´óµ¥ÌåµçÑ¹
#define HIGHEST_BATT_VOL    775.2   //×î´ó×ÜµçÑ¹3.8*204 = 775.2
#define HIGHEST_TEM         60      //×î¸ßÎÂ¶È60

//////////////////////BMS¹¦ÂÊ±ä»¯ËÙ¶È//////////////////////////
#define POWERSPEED  0.5       //¹¦ÂÊµÄ±ä»¯ËÙ¶È

//////////////////////¸ßÑ¹µÈ¼¶:BMS¾øÔµ¼ÆËã²ÉÑùµç×èÖµ//////////////////////////
#define RESVALUE  0.2       //²ÉÑùµç×èÎª0.2K

//////////////////////BMS±ê¶¨¸ßÑ¹Ê±µÄÅäÖÃµçÑ¹//////////////////////////
#define HIGHVOL_19    350       //·¢ËÍ0x19Ê±¶ÔÓ¦µÄµçÑ¹
#define HIGHVOL_23    550      //·¢ËÍ0x23Ê±¶ÔÓ¦µÄµçÑ¹

#include <hidef.h>      /* common defines and macros */
#include <string.h>
#include "derivative.h"      /* derivative-specific definitions */
//µ×²ãÍ·ÎÄ¼þ
#include  "mc9s12gpio.h"
#include  "74hc595.h"
#include  "adc.h"
#include  "bmn.h"
#include  "mscan.h"
#include  "RTI.h"
#include  "sd2405.h"
#include  "dflash.h"
#include  "ProcessBMU.h"
#include  "current.h"
#include  "SocAHintegral.h"
#include  "Record.h"
#include  "Storage.h"
#include  "M95M02.h"
#include  "Connect.h"
//Ó¦ÓÃ²ãÍ·ÎÄ¼þ
#include  "Machine.h"
#include  "VehicleCAN.h"
#include  "InnerCAN.h"
#include  "FastChargeCAN.h"
#include  "SOF.h"
#include  "feedBack.h"
#include  "SocAdjust.h"
#include  "Heat.h"
#include  "MachineControl.h"
//#include "BatteryOverVoltage1_types.h"
#include "Fault.h"
#include "24LC64.h"
#include "InitBMS.h"
#include "mc9s12Isr.h"
#include "ParameterSetting.h"
#include "Q1Q2calculate.h"
#include "Task.h"
#include "DC_ChangerProcess.h"
#include "AC_ChangerProcess.h"
#include "ads1015.h"
#include "lookup_Table_Tem_NEW20170204.h"

#include "Q1.h"
#include "CellTempUnbalance.h"
#include "YoungMan_LT.h"

//#include "Isolation.h"
//*******************************************
//*******************************************
#define  uchar unsigned char
#define  uint  unsigned int
#define  ulong unsigned long
#define  HIGH 0
#define  LOW 1

#define U8  uchar
#define U16 uint
#define U32 ulong

// ÈÎÎñ½á¹¹
typedef struct _TASK_COMPONENTS
{
    uchar Run;                 // ³ÌÐòÔËÐÐ±ê¼Ç£º0-²»ÔËÐÐ£¬1ÔËÐÐ
    uint Timer;                // ¼ÆÊ±Æ÷
    uint ItvTime;              // ÈÎÎñÔËÐÐ¼ä¸ôÊ±¼ä
    void (*TaskHook)(void);    // ÒªÔËÐÐµÄÈÎÎñº¯Êý
} TASK_COMPONENTS;       // ÈÎÎñ¶¨Òå
/////////
//************************************************************************
//************************************************************************
// ÈÎÎñÇåµ¥
typedef enum _TASK_LIST
{
    TASK_CURRENT,
    TASK_SOC,
    TASK_BMU,            
    TASK_VOLTAGE,             
    TASK_INSULATION,             
    TASK_REPORT2PC,
    TASK_STAT_MACHINE,
    TASK_FAULT,
    TASK_RECORD,
    //TASK_DC_RECHARGE,
    TASKS_MAX      // ×ÜµÄ¿É¹©·ÖÅäµÄ¶¨Ê±ÈÎÎñÊýÄ¿                                     
} TASK_LIST;
////////
enum slaveNumber
{
S0,
S1,
S2,
S3,
S4,
S5,
S6,
S7
};
//*******************************************
//*******************************************
enum parameter_list
{
    PARA_SOC_VALUE,    //SOCÊµ¼ÊÖµ
    //PARA_DISCHARGE_RATIO,//¹ýµçÁ÷
    PARA_POSRELAY_PSTATE,//×ÜÕý¼ÌµçÆ÷Õ³Á¬¹ÊÕÏ  relayPstate
    //PARA_SINGLE_CAPACITY,  //µ¥Ìåµç³ØÈÝÁ¿AH
    PARA_PRECHARGE_RELAY_PSTATE,//Ô¤³ä¼ÌµçÆ÷Õ³Á¬¹ÊÕÏ
    PARA_WHOLE_CAPACITY,//µç³Ø×é×ÜÈÝÁ¿
    PARA_BMU_NUMBER,//  µç³ØÊýÁ¿

    PARA_DIFFERENCE_SINGLE_V, // µ¥ÌåµçÑ¹¾ùºâÖµ
    PARA_DIFFERENCE_SINGLE_T, // µç³ØÎÂ¶È¾ùºâÖµ
    PARA_BALANCE_ON_VOLTAGE, //¾ùºâ¿ªÆôµçÑ¹Öµ
    PARA_BALANCE_OFF_VOLTAGE,//¾ùºâ¹Ø±ÕµçÑ¹Öµ
    PARA_O_SOC_VALUE,// soc¸ß

    PARA_OO_SOC_VALUE,// soc¹ý¸ß
    PARA_L_SOC_VALUE,//socµÍ
    PARA_LL_SOC_VALUE,//soc¹ýµÍ
    PARA_O_WHOLE_VOLTAGE, // ×ÜµçÑ¹Öµ¸ß
    PARA_OO_WHOLE_VOLTAGE, // ×ÜµçÑ¹Öµ¼«¸ß

    PARA_L_WHOLE_VOLTAGE, // ×ÜµçÑ¹ÖµµÍ
    PARA_LL_WHOLE_VOLTAGE, // ×ÜµçÑ¹Öµ¼«µÍ
    PARA_O_CELL_VOLTAGE, // µ¥ÌåµçÑ¹¸ß£¨¹ýÑ¹Öµ£©
    PARA_OO_CELL_VOLTAGE, // µ¥ÌåµçÑ¹¼«¸ß
    PARA_L_CELL_VOLTAGE,//  µ¥ÌåµçÑ¹µÍ(Ç·Ñ¹Öµ)

    PARA_LL_CELL_VOLTAGE,//  µ¥ÌåµçÑ¹¼«µÍ
    PARA_O_TEMPERATURE,//¹ýÎÂ
    PARA_OO_TEMPERATURE,//ÎÂ¶È¼«¸ß
    PARA_L_TEMPERATURE, // ÎÂ¶ÈµÍ
    PARA_LL_TEMPERATURE, // ÎÂ¶È¼«µÍ

    PARA_O_CONSISTENCY_V, //µçÑ¹Ò»ÖÂÐÔ²î
    PARA_OO_CONSISTENCY_V,//µçÑ¹Ò»ÖÂÐÔ¼«²î
    PARA_O_CONSISTENCY_T, //ÎÂ¶ÈÒ»ÖÂÐÔ²î
    PARA_OO_CONSISTENCY_T,//ÎÂ¶ÈÒ»ÖÂÐÔ¼«²î
    PARA_CURRENT,//   ¹ýµçÁ÷Öµ

    PARA_HIGHEST_RECHARGE_VOLTAGE,//ÔÊÐí×î¸ß³äµç¶ËµçÑ¹
    PARA_HIGHEST_RECHARGE_CURRENT,//×î´óÔÊÐí³äµçµçÁ÷ 
    PARA_HIGHEST_RECHARGE_CURRENT_TIME,//ÔÊÐí×î´ó³äµçµçÁ÷Ê±¼ä
    PARA_HIGHEST_DISCHARGE_CURRENT,//×î´óÔÊÐí·ÅµçµçÁ÷ 
    PARA_HIGHEST_CHARGE_CURRENT_TIME,//ÔÊÐí·ÅµçµçÁ÷×î´óµçÁ÷Ê±¼ä

    PARA_L_ISOLATION_RESISTANCE, //  ¾øÔµµç×èµÍÖµ
    PARA_LL_ISOLATION_RESISTANCE, //  ¾øÔµµç×è¹ýµÍÖµ

    PARA_ENDFLAG
};
enum storage_list 
{
    INDEX,
    TOTAL_VOLTAGE,
    TOTAL_CURRENT,
    PARA_SOC_DISPLAY, // SOCÖµ
    SYS_CONTACTORS_STATE,  //×ÜÕý×Ü¸º¼ÌµçÆ÷×´Ì¬

    PARA_ERROR_LEVER,//¹ÊÕÏµÈ¼¶Öµ  1:1¼¶¹ÊÕÏ  2:2¼¶¹ÊÕÏ
    CAUTION_FLAG_1,
    CAUTION_FLAG_2,
    CAUTION_FLAG_3,
    CAUTION_FLAG_4,

    SYS_REALTIME_SECOND,
    SYS_REALTIME_MINUTE,
    SYS_REALTIME_HOUR,
    SYS_REALTIME_WEEK,
    SYS_REALTIME_DAY,

    SYS_REALTIME_MONTH,
    SYS_REALTIME_YEAR,
    CELL_MAX_TEM,
    CELL_MIN_TEM,
    CELL_AVERAGE_TEM,

    CELL_MAX_VOLTAGE,
    CELL_MIN_VOLTAGE,
    CELL_AVERAGE_VOLTAGE,
    SYS_INSULATION_P,
    SYS_INSULATION_N,
    
    VOLT_K1,
    VOLT_B1,
    VOLT_K2,
    VOLT_B2,
    VOLT_K3,
    
    VOLT_B3,
    VERIFICATION  //Ð£Ñé
};

enum storage_cell_vol_list 
{  
    CELL_VOLTAGE_0,
    CELL_VOLTAGE_1,
    CELL_VOLTAGE_2,
    CELL_VOLTAGE_3,
    CELL_VOLTAGE_4,
    CELL_VOLTAGE_5,
    CELL_VOLTAGE_6,
    CELL_VOLTAGE_7,
    CELL_VOLTAGE_8,
    CELL_VOLTAGE_9,

    CELL_VOLTAGE_10,
    CELL_VOLTAGE_11,
    CELL_VOLTAGE_12,
    CELL_VOLTAGE_13,
    CELL_VOLTAGE_14,
    CELL_VOLTAGE_15,
    CELL_VOLTAGE_16,
    CELL_VOLTAGE_17,
    CELL_VOLTAGE_18,
    CELL_VOLTAGE_19,

    CELL_VOLTAGE_20,
    CELL_VOLTAGE_21,
    CELL_VOLTAGE_22,
    CELL_VOLTAGE_23,
    CELL_VOLTAGE_24,
    CELL_VOLTAGE_25,
    CELL_VOLTAGE_26,
    CELL_VOLTAGE_27,
    CELL_VOLTAGE_28,
    CELL_VOLTAGE_29,

    CELL_VOLTAGE_30,
    CELL_VOLTAGE_31,
    CELL_VOLTAGE_32,
    CELL_VOLTAGE_33,
    CELL_VOLTAGE_34,
    CELL_VOLTAGE_35,
    CELL_VOLTAGE_36
    
};
//                                           
#define DEVICE_SERIAL_NUMBER 0x11    //	²¿¼þÐòÁÐºÅ 1byte
#define HARDWARE_VERSION 0x01    //	Ó²¼þ°æ±¾ µÍ4Î»ÓÐÐ§   
#define SOFTWARE_VERSION 0x01   // 	Èí¼þ°æ±¾ µÍ4Î»ÓÐÐ§
#define COMMUNICATION_VERSION 0x01  // Í¨Ñ¶Ð­Òé°æ±¾ µÍ4Î»ÓÐÐ§
//
//
//#define ParamNUMBER 38
#define ENDFLAG 96

//
//
#define DISCHARGING 0 //·Åµç
#define RECHARGING 2  //³äµç
#define FASTRECHARGING 1  //¿ì³ä
#define TESTSTATE 4  //µ÷ÊÔÄ£Ê½


//************ main.c ****************************************
extern unsigned char BMS_SW_Version[8];
extern unsigned char BMUOK;
extern unsigned char HeatFlag; //ÉÏµçÇ°ÎÂ¶È±êÖ¾Î»

 //***********************************************************************************
//*******************************the end**********************************************
//************************************************************************************
///////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////


