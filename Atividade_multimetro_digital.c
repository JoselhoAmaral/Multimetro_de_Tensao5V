#include <18F4550.h>
#device adc=10
#fuses HS, CPUDIV1, PLL5, USBDIV
#fuses NOWDT, PUT, BROWNOUT, NOLVP
#use delay(clock=20MHz)
#int_timer1

void multimetro(){
   set_timer1(53036 + get_timer1());
   
   static int display[4] = {0,0,0,0};
   static int flag = 0;
   static float entradaAnalogica = 0;
   static float tensao = 0;

   int digito[10] =
               {0b00111111,    //Digito 0
                0b00000110,    //Digito 1
                0b01011011,    //Digito 2
                0b01001111,    //Digito 3
                0b01100110,    //Digito 4
                0b01101101,    //Digito 5
                0b01111101,    //Digito 6
                0b00000111,    //Digito 7
                0b01111111,    //Digito 8
                0b01101111     //Digito 9
               };
               
//---------------------------------------------------------------------------//
//----- Estrutura de leitura do A0 ------------------------------------------//
   //Faz a leitura da entrada analagica AN 0.
      if (flag == 0){       
         entradaAnalogica = read_adc();
         delay_us(100);
      }
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//----- Estrutura de conversao para valor de tensao -------------------------//
      tensao = ((entradaAnalogica/1023)*5);
//---------------------------------------------------------------------------//

//---- Estrutura de atribuicao da tensao para o vetor display ---------------//
      display[0] = tensao; 
      tensao = (tensao - display[0])*10;
      display[1] = tensao;
      tensao = (tensao - display[1])*10;
      display[2] = tensao;
      tensao = (tensao - display[2])*10;
      display[3] = tensao;
//---------------------------------------------------------------------------//
      
//---------------------------------------------------------------------------//
//----- Estrutura de apresentacao do LED ------------------------------------//
      switch (flag){
         case 0:
            OUTPUT_D(digito[display[0]]);
            /* Para o SimulIDE
            output_bit(pin_a5,1);
            output_bit(pin_a4,1);
            output_bit(pin_a3,1);
            output_bit(pin_a2,0); */
            
            // Para o PICSimLab
            output_bit(pin_a5,0);
            output_bit(pin_a4,0);
            output_bit(pin_a3,0);
            output_bit(pin_a2,1);
         break;
   
         case 1:
            OUTPUT_D(digito[display[1]]);
            //OUTPUT_A(0b11110111);                // Para o SimulIDE
            // Para o PICSimLab
            output_bit(pin_a5,0);
            output_bit(pin_a4,0);
            output_bit(pin_a3,1);
            output_bit(pin_a2,0);
         break;
   
         case 2:
            OUTPUT_D(digito[display[2]]);
            //OUTPUT_A(0b11101111);                // Para o SimulIDE
            // Para o PICSimLab
            output_bit(pin_a5,0);
            output_bit(pin_a4,1);
            output_bit(pin_a3,0);
            output_bit(pin_a2,0);
         break;
   
         case 3:
            OUTPUT_D(digito[display[3]]);
            //OUTPUT_A(0b11011111);                // Para o SimulIDE
            // Para o PICSimLab
            output_bit(pin_a5,1);
            output_bit(pin_a4,0);
            output_bit(pin_a3,0);
            output_bit(pin_a2,0);
         break;
      }
//---------------------------------------------------------------------------//
   
//---------------------------------------------------------------------------//
//---- Estrutura do chaveamento do switch -----------------------------------//
      flag++;
      if (flag == 4){
         flag = 0;
      }
//---------------------------------------------------------------------------//
}

void main(){
   
   output_d(0);         // Desliga os pinos do grupo D
   SET_TRIS_D(0x00);    // Seta o grupo D como saida
   
   setup_timer_1 (T1_INTERNAL|T1_DIV_BY_8);
   set_timer1(53036); // Quando 34286, a interrupcao ocorrera a cada 0,05s
                      // Quando 53036, a interrupcao ocorrera a cada 0,02s 
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER1);

   setup_adc_ports(AN0);
   SETUP_ADC(ADC_CLOCK_INTERNAL);
   SET_ADC_CHANNEL(0);
  
   while(True)
   { }
}
