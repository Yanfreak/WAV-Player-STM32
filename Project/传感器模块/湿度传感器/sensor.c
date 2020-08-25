/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� sensor.c
*   ժ Ҫ    �� ʪ�ȴ�����ʵ��������
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
******************************************************************************/

/*************ͷ�ļ�**********************************************************/
#include "sensor.h"
#include "sensor_data.h"

/*************�궨��**********************************************************/
#define CaculateTIndexTabSize(TIndexTab)					    \
        (TIndexTab).nTableSize=									\
		(((TIndexTab).nIndexHigh-(TIndexTab).nIndexLow+1)/		\
		(TIndexTab).nIndexFactor)

/*************��������********************************************************/
void SensorDataInit(void);
int GetTableIndex(TIndexTable * indexTab,float tabData);
float NTC3950(float volt,float resi_div,float vcc);
float HumiditySensor(float volt);

/****************************************************************************
* ������: SensorDataInit()
* �� ��: ����������������ݳ�ʼ������
* �� ��: ��
* �� ��: ��
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
* ������: SensorDataInit()
* �� ��: ���ݷֶȱ����ݲ�ѯ��Ӧ��������
* �� ��: �ֶȱ��ѯ�ṹ�壬�ֶȱ�����
* �� ��: �����ڷֶȱ��е�����λ��
****************************************************************************/
int GetTableIndex(TIndexTable * indexTab,float tabData)
{
	int asc=1;
	int i;
	float a,b;

	if(indexTab->pTableData[0]>indexTab->pTableData[1])
	{
		asc=-1;	/* �ֶȱ��е������ǵ����ݼ��� */
	}

	a=indexTab->pTableData[0];
	b=indexTab->pTableData[indexTab->nTableSize-1];
	if(((asc*tabData)<(asc*a))||
		((asc*tabData)>(asc*b)))
	{
		printf("out of range\r\n");	/*  ����ѯ���ݳ����ֶȱ�Χ */
	}

	for(i=0;i<indexTab->nTableSize-1;i++) /* ��ѯ�����ڷֶȱ��е�����λ�� */
	{
		a=indexTab->pTableData[i];
		b=indexTab->pTableData[i+1];
		if((asc*tabData)<(asc*(a+b)/2))
			break;
	}
	return i;
}

/****************************************************************************
* ������: HumiditySensor()
* �� ��: ʪ�ȴ����������¶Ⱥ���
* �� ��: ����ֵ��ѹ
* �� ��: ʪ��ֵ
****************************************************************************/
float HumiditySensor(float volt)
{
	float humidity;

	/* ����ʪ��ֵ */
	humidity=100/3*volt;

	return humidity;
}

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����**************/
