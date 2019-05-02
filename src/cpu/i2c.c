#include <cpu/i2c.h>

I2C_Definition I2CDefinitions[I2C_COUNT] =
{
	{
		.Id = 0,
		.Definition = I2C1,
		.I2CClockCmd = RCC_APB1PeriphClockCmd,
		.I2CClock = RCC_APB1Periph_I2C1,
		.GpioPort = GPIOB,
		.SDAPin = GPIO_Pin_9,
		.SCLPin = GPIO_Pin_8
	},
	{
		.Id = 1,
		.Definition = I2C2,
		.I2CClockCmd = RCC_APB1PeriphClockCmd,
		.I2CClock = RCC_APB1Periph_I2C2,
		.GpioPort = GPIOF,
		.SDAPin = GPIO_Pin_0,
		.SCLPin = GPIO_Pin_1
	}
};

bool i2c_enabled[I2C_COUNT];

bool I2C_Enable(I2C_TypeDef *i2cx)
{
	I2C_Definition *definition;
	GPIO_InitTypeDef gpio;
	I2C_InitTypeDef i2c;

	if(definition = I2C_GetDefinition(i2cx), definition == 0)
	{
		return false;
	}

	if(i2c_enabled[definition->Id]) return false;
	definition->I2CClockCmd(definition->I2CClock, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = definition->SDAPin | definition->SCLPin;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(definition->GpioPort, &gpio);

	I2C_StructInit(&i2c);
	i2c.I2C_Mode = I2C_Mode_I2C;
	I2C_Init(i2cx, &i2c);

	I2C_Cmd(i2cx, ENABLE);
	i2c_enabled[definition->Id] = true;

	return true;
}

bool I2C_Disable(I2C_TypeDef *i2cx)
{
	I2C_Definition *definition;
	if(definition = I2C_GetDefinition(i2cx), definition == 0)
	{
		return false;
	}

	if(!i2c_enabled[definition->Id]) return false;
	i2c_enabled[definition->Id] = false;

	definition->I2CClockCmd(definition->I2CClock, DISABLE);
	I2C_Cmd(i2cx, DISABLE);

	return true;
}

I2C_Definition *I2C_GetDefinition(I2C_TypeDef *i2cx)
{
	for(int i = 0; i < I2C_COUNT; i++)
	{
		if (I2CDefinitions[i].Definition == i2cx)
		{
			return &I2CDefinitions[i];
		}
	}

	return 0;
}
