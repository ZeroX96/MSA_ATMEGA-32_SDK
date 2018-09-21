/*
 * LCD_P216_CFG.h
 *
 * Created: 20/09/2018 00:29:34
 *  Author: Mahmoud
 */ 


#ifndef LCD_P216_CFG_H_
#define LCD_P216_CFG_H_

//the data pins must be following each others and come in row so if from pin 3>> 3.4.5.6 
//the beginning must be zero when 8 data lines mode and must not exceed the 4th pin when 4 data lines mode 
#define NO_OF_DATA_LINES 4  // 8 or 4

//if the lcd is the first func will need to wait for some time to avoid the noises
//if not please set hte next macro to zero
#define LCD_INIT_IS_FIRST_FUNC 1 //set to zero if it's not the first func in the project



#endif /* LCD_P216_CFG_H_ */