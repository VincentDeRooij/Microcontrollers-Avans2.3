#ifndef LCD_H_
#define LCD_H_

#define LCD_E 	3
#define LCD_RS	2

void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_data(unsigned char byte);
void lcd_write_command(unsigned char byte);

void lcd_set_cursor(int row, int index);
void lcd_overwrite_line(char *str, int row);
void lcd_clear_screen();

#endif /* INTERNET_CLOCK_H_ */