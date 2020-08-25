/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： sensor.c
*   摘 要    ： 温度传感器实验主程序
*
*   当前版本 ： 
*   作 者    ： EFLAG
*   完成日期 ： 
*
*   取代版本 ：
*   原作者   ： 
*   修改内容 ：
*   完成日期 ：2012.08.01 
*
******************************************************************************/

/*************头文件**********************************************************/
#include "sensor.h"
#include "sensor_data.h"

/*************宏定义**********************************************************/
#define CaculateTIndexTabSize(TIndexTab)					    \
        (TIndexTab).nTableSize=									\
		(((TIndexTab).nIndexHigh-(TIndexTab).nIndexLow+1)/		\
		(TIndexTab).nIndexFactor)

/*************函数声明********************************************************/
void SensorDataInit(void);
int GetTableIndex(TIndexTable * indexTab,float tabData);
float NTC3950(float volt,float resi_div,float vcc);

/****************************************************************************
* 函数名: SensorDataInit()
* 功 能: 传感器计算相关数据初始化函数
* 输 入: 无
* 输 出: 无
****************************************************************************/
void SensorDataInit(void)
{
	TIndexTab_NTC3950.nIndexLow=-55;
	TIndexTab_NTC3950.nIndexHigh=300;
	TIndexTab_NTC3950.nIndexFactor=1;
	CaculateTIndexTabSize(TIndexTab_NTC3950);
	TIndexTab_NTC3950.pTableData=TableData_NTC3950;
}

/****************************************************************************
* 函数名: GetTableIndex()
* 功 能: 根据分度表数据查询对应索引函数
* 输 入: 分度表查询结构体，分度表数据
* 输 出: 数据在分度表中的索引位置
****************************************************************************/
int GetTableIndex(TIndexTable * indexTab,float tabData)
{
	int asc=1;
	int i;
	float a,b;

	if(indexTab->pTableData[0]>indexTab->pTableData[1])
	{
		asc=-1;	/* 分度表中的数据是单调递减的 */
	}

	a=indexTab->pTableData[0];
	b=indexTab->pTableData[indexTab->nTableSize-1];
	if(((asc*tabData)<(asc*a))||
		((asc*tabData)>(asc*b)))
	{
		printf("out of range\r\n");	/*  待查询数据超出分度表范围 */
	}

	for(i=0;i<indexTab->nTableSize-1;i++) /* 查询数据在分度表中的索引位置 */
	{
		a=indexTab->pTableData[i];
		b=indexTab->pTableData[i+1];
		if((asc*tabData)<(asc*(a+b)/2))
			break;
	}
	return i;
}

/****************************************************************************
* 函数名: NTC3950()
* 功 能: 热敏电阻传感器计算温度函数
* 输 入: 采样值电压，分压电阻阻值，5V供电电压值
* 输 出: 温度值
****************************************************************************/
float NTC3950(float volt,float resi_div,float vcc)
{
	int index;
	float resi;
	float temperature;

	/* 计算传感器电阻值*/
	resi=resi_div*volt/(vcc-volt);

	/* 查询电阻值在分度表中的索引 */
	index=GetTableIndex(&TIndexTab_NTC3950,resi);
	temperature = TIndexTab_NTC3950.nIndexLow+index*TIndexTab_NTC3950.nIndexFactor;

	return temperature;
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束**************/
