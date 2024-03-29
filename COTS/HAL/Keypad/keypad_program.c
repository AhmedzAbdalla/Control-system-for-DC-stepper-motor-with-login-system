/*
 * keypad_interface.h
 *
 *  Created on: Mar 29, 2023
 * Author: Ahmed Abdalla
 * Version: 1.1
 */
#include <util/delay.h>

#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/DIO/DIO_Interface.h"

#include "keypad_interface.h"
#include "keypad_cfg.h"

u8 au8_arr_col[KEYPAD_COLUMN] = {COLUMN0 ,COLUMN1 , COLUMN2 , COLUMN3};
u8 au8_arr_row[KEYPAD_ROW] = {ROW0 ,ROW1 , ROW2 , ROW3};

u8 keypad_au8_keys[KEYPAD_ROW][KEYPAD_COLUMN] = { {7 , 8 ,9 ,44},
																								{4 , 5 , 6 ,33},
																								{1 , 2 , 3 ,22},
																								{77 , 0 , 99 ,11},
};
u8 equal=0;
void keypad_void_init(void)
{
	//initializing row pins as input
	DIO_void_Pin_Initialize(kEYPAD_PORT , ROW0 , INPUT);
	DIO_void_Pin_Initialize(kEYPAD_PORT , ROW1 , INPUT);
	DIO_void_Pin_Initialize(kEYPAD_PORT , ROW2 , INPUT);
	DIO_void_Pin_Initialize(kEYPAD_PORT , ROW3 , INPUT);

	//initializing column pins as output
	DIO_void_Pin_Initialize(kEYPAD_PORT , COLUMN0 , OUTPUT);
	DIO_void_Pin_Initialize(kEYPAD_PORT , COLUMN1 , OUTPUT);
	DIO_void_Pin_Initialize(kEYPAD_PORT , COLUMN2 , OUTPUT);
	DIO_void_Pin_Initialize(kEYPAD_PORT , COLUMN3 , OUTPUT);

	//initializing row pins Pull up
	DIO_void_PORT_Write_Logic(kEYPAD_PORT , 0x0f);

}

u8 keypad_u8_read_digit(void)
{
	u8 local_u8_ret_val = 0xff;
	s8 local_u8_pressed_key = 0xff;
	u8 local_u8_row_counter = 0;
	u8 local_u8_column_counter = 0;
	u8 local_u8_pressed_key_flag = 0;

	for(local_u8_column_counter = 0 ; local_u8_column_counter < KEYPAD_COLUMN ; local_u8_column_counter++)
	{
		//activate column, remember it is pull resistor with rows
		DIO_void_Pin_Write_Logic(kEYPAD_PORT ,au8_arr_col[local_u8_column_counter]  , LOW);

		for(local_u8_row_counter = 0 ; local_u8_row_counter < KEYPAD_ROW ; local_u8_row_counter++)
			{
				// delay to handle bouncing with mechanical switches
				_delay_ms(10);
				// reading data from rows with specific column
				local_u8_ret_val = DIO_u8_Pin_Read_Logic(kEYPAD_PORT , au8_arr_row[local_u8_row_counter] );

				// as soon as button is pressed
				if( 0 == local_u8_ret_val)
				{
					// this loop will not end until the user releases the button
					while(0 == local_u8_ret_val)
					{

						local_u8_ret_val = DIO_u8_Pin_Read_Logic(kEYPAD_PORT , au8_arr_row[local_u8_row_counter] );
					}
					//returning the value represented by pressed key
					local_u8_pressed_key = keypad_au8_keys[local_u8_row_counter][local_u8_column_counter];

					//flag to exit the function as soon as the key pressed
					local_u8_pressed_key_flag = 1;
					break;
				}
			}
		//deactivate column, remember it is pull resistor with rows
		DIO_void_Pin_Write_Logic(kEYPAD_PORT ,au8_arr_col[local_u8_column_counter]  , HIGH);

		if(local_u8_pressed_key_flag == 1)
		{
			break;
		}
	}
	return local_u8_pressed_key;
}



