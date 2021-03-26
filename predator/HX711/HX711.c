/************************************************************************************
						
*************************************************************************************/
#include "HX711.h"
//#include "delay.h"

int HX711_Buffer;
int Weight_Maopi;
int Weight_Shiwu;
int Flag_Error = 0;

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 106.5

int get_HX711data(void)
{		
	Init_HX711pin();
	
//	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//	uart_init(9600);	 //串口初始化为9600
	
	Get_Maopi();				//称毛皮重量
	HAL_Delay(1000);
	HAL_Delay(1000);
	Get_Maopi();				//重新获取毛皮重量
	
		Get_Weight();

//		print("净重量 = %d g\r\n",Weight_Shiwu); //打印 
//		HAL_Delay(1000);

return Weight_Shiwu;
	
}
void Init_HX711pin()
{
	HAL_GPIO_WritePin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin,0);
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);
}



//****************************************************
//读取HX711
//****************************************************
int HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
//  	HX711_DOUT=1; 
	HAL_GPIO_WritePin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin,1);
	HAL_Delay(1);
//  	HX711_SCK=0; 
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);
  	count=0; 
  	while(HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin)); 
  	for(i=0;i<24;i++)
	{ 
//	  	HX711_SCK=1; 
		HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,1);
	  	count=count<<1; 
		HAL_Delay(1);
//		HX711_SCK=0; 
		HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);

		HAL_GPIO_WritePin(DJ_GPIO_Port,DJ_Pin,1);
	  	if(HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin))
			count++; 
		HAL_Delay(1);
	} 
// 	HX711_SCK=1; 
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,1);
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	HAL_Delay(1);
//	HX711_SCK=0; 
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);	
	return(count);
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//称重
//****************************************************
void Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	
		Weight_Shiwu = (int)((float)Weight_Shiwu/GapValue); 	//计算实物的实际重量
																		//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
																		//当发现测试出来的重量偏大时，增加该数值。
																		//如果测试出来的重量偏小时，减小改数值。
	}

	
}
