// Keypad module connections
char keypadPort at PORTB;
// End of keypad module connections
// LCD module connections
sbit LCD_RS at RC4_bit;
sbit LCD_EN at RC5_bit;
sbit LCD_D4 at RC0_bit;
sbit LCD_D5 at RC1_bit;
sbit LCD_D6 at RC2_bit;
sbit LCD_D7 at RC3_bit;

sbit LCD_RS_Direction at TRISC4_bit;
sbit LCD_EN_Direction at TRISC5_bit;
sbit LCD_D4_Direction at TRISC0_bit;
sbit LCD_D5_Direction at TRISC1_bit;
sbit LCD_D6_Direction at TRISC2_bit;
sbit LCD_D7_Direction at TRISC3_bit;
// End LCD module connections
#define HEATER PORTD.RD0
#define FAN PORTD.RD1
#define ENTER 15
#define CLEAR 13
#define ON 1
#define OFF 0
void main() 
{
 unsigned short kp, Txt[14];
 unsigned short Temp_Ref;
 unsigned char inTemp;
 unsigned int temp;
 float mV,ActualTemp;
 ANSELC =0;
 ANSELB =0;
 ANSELD =0;
 TRISA0_bit=1;
 TRISC=0;
 TRISD0_bit=0;
 TRISD1_bit=0;
  keypad_init();
  Lcd_Init();                        // Initialize LCD
  Lcd_Cmd(_LCD_CLEAR);               // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
  Lcd_out(1,4,"Automatic");
  Lcd_out(2,2,"Temp Control");
  delay_ms(2000);
  HEATER = OFF;
  FAN = OFF;
  START:
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_out(1,1,"Enter Temp Ref");
  Temp_Ref=0;
  Lcd_out(2,1,"Temp Ref: ");
  while(1)
   {
    do
    kp = keypad_key_Click();
    while(!kp);
    if ( kp == ENTER)break;
    if ( kp > 3 && kp < 8) kp=kp-1;
    if ( kp > 8 && kp < 12) kp=kp-2;
    if (kp==14) kp= 0;
    if( kp == CLEAR) goto START;
    Lcd_chr_cp(kp+'0');
    Temp_Ref=(10*Temp_Ref)+kp;
   }
   Lcd_Cmd(_LCD_CLEAR);
   Lcd_out(1,1,"Temp Ref: ");
   intToStr(Temp_Ref,Txt);
   inTemp=Ltrim(Txt);
   lcd_out_cp(inTemp);
   lcd_out(2,1,"Press # to Cont.");
   kp=0;
   while(kp!=ENTER)
   {
    do
    kp = keypad_key_Click();
    while(!kp);
   }
   Lcd_Cmd(_LCD_CLEAR);
   Lcd_out(1,1,"Temp Ref: ");
   lcd_chr(1,15,223);
   lcd_chr(1,16,'C');
   while(1)
   {
    temp= ADC_Read(0);
    mV= Temp*5000.0/1024.0;
    ActualTemp= mV/10.0;
    intToStr(Temp_Ref,Txt);
    inTemp=Ltrim(Txt);
    Lcd_out(1,11,inTemp);
    Lcd_out(2,1,"Temp= ");
    FloatToStr(ActualTemp,Txt);
    Txt[4]=0;
    Lcd_out(2,7,Txt);
    lcd_out(2,12,"   ");

    if(Temp_Ref>ActualTemp)
     {
      HEATER=ON;
      FAN=OFF;
     }
     if(Temp_Ref<ActualTemp)
     {
      HEATER=OFF;
      FAN=ON;
     }
     if(Temp_Ref==ActualTemp)
     {
      HEATER=OFF;
      FAN=OFF;
     }
     delay_ms(10000);
   }
}