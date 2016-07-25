
#ifndef __DRIVER_STATUSMACHINE_H
#define __DRIVER_STATUSMACHINE_H


#include "stm32f4xx.h"


#define VisiolModeChangeDataSendNum     10


#ifdef  __DRIVER_STATUSMACHINE_GLOBALS
#define __DRIVER_STATUSMACHINE_EXT
#else
#define __DRIVER_STATUSMACHINE_EXT extern
#endif

//摩擦轮状态
typedef enum
{
    FricStatus_Stop,
    FricStatus_Working,
    FricStatus_Crazy,
}FricStatus_Enum;


//控制模式
typedef enum
{
    ControlMode_Protect,            //保护模式，全部停止 
    ControlMode_RC,                 //遥控器控制（增量式）
    ControlMode_KM,                 //键鼠控制
}ControlMode_Enum;


//键鼠模式子模式
typedef enum
{
    KMSubschema_Normal,         //正常模式（全手动模式）
    KMSubschema_Supply,         //补给站模式
    KMSubschema_Swing,          //底盘摇动模式
    KMSubschema_Halfauto,       //半自动射击模式
    KMSubschema_Bigsample,      //大符模式
    KMSubschema_Fullauto,       //全自动模式
    KMSubschema_Circle,         //自主转圈
}KMSubschema_Enum;



//状态量
__DRIVER_STATUSMACHINE_EXT ControlMode_Enum         ControlMode;
__DRIVER_STATUSMACHINE_EXT FricStatus_Enum          FricStatus;
__DRIVER_STATUSMACHINE_EXT KMSubschema_Enum         KMSubschema;


void StatusMachine_InitConfig(void);
void StatusMachine_Update(void);


#endif
