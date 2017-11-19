#include <msp430.h> 

void setPWM(unsigned int duty);
void setUpPWM();
void pwmUARTInterrupt(long bufInput);
void setUpUART();
void setUpADC();
void ADC10Interrupt();
void timerA1Interrupt();
void setUpTimerA0();
void setDutyCycle(int temp);

int tempBuf[10];
int tempBuf_index = 0;

int k =5;
long goaltemp = 40;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    setUpPWM();
    setUpUART();
    setUpADC();
    setUpTimerA0();

    setDutyCycle(goaltemp);

    __bis_SR_register(GIE);       // Enter LPM0, interrupts enabled

    while(1){

    }
}


void setUpPWM(){
    P2DIR |= BIT1;                            // P2.1 output
    P2SEL |= BIT1;                            // P2.1 for TA0 CCR1 Output Capture
    P2SEL2 = 0;                               // Select default function for P1.6 (see table 19 in general datasheet)
    TA1CCR0 = 1000;                             // PWM Freq=1000Hz
    TA1CCTL1 = OUTMOD_7;                         // CCR1 reset/set: set on at CCR0, off at CCR1 capture (see table 12-2 in specific datasheet)
    TA1CCR1 = 500;                               // CCR1 50% PWM duty cycle
    TA1CTL = TASSEL_2 + MC_1 + ID_0;                  // SMCLK, up mode, 1MhZ
}

void setUpUART(){
    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
    {
      while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;
    P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

}

void setUpADC(){
    ADC10CTL1 = INCH_7 + SHS_1;               // P1.7, TA1 trigger sample start
    ADC10AE0 = 0x80;                          // P1.7 ADC10 option select

}

void setUpTimerA0(){
    TA0CCTL0 = CCIE;                           // Enable interrupt
    TA0CCR0 = 65534;                           // PWM Period
    TA0CCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TA0CCR1 = 781;                               // TACCR1 PWM Duty Cycle
    TA0CTL = TASSEL_2 + MC_1 + ID_3;                  // ACLK, up mode
}

void setDutyCycle(int temp){
    if(temp > 76){
        setPWM(0);
    }
    else if(temp > 31){
        long pwm = ((temp * -84) / 100) + 63;
        setPWM(pwm);
    }
    else if(temp > 20){
        long pwm = ((temp * -49) / 10) + 187;
        setPWM(pwm);
    }
    else{
        setPWM(100);
    }
}

void setPWM(unsigned int bitDuty){
    if(bitDuty>100){
        bitDuty = 100;
    }
    bitDuty = bitDuty * 10;
    TA1CCR1 = bitDuty;
}

void pwmUARTInterrupt(long bufInput){
    goaltemp = bufInput;;
}

void ADC10Interrupt(){
    ADC10CTL0 &= ~ENC;                        // ADC10 disabled
    ADC10CTL0 = 0;                            // ADC10, Vref disabled completely
//    long temp = ADC10MEM;
//    temp *= 330;
//    temp /= 1024;
//    UCA0TXBUF = temp;                     //send to TX
    if(tempBuf_index < 10){
        tempBuf[tempBuf_index] = ADC10MEM;
        tempBuf_index++;
    }
    else{
        long average = 0;
        int i = 0;
        for(i = 0; i < 10 ; i ++){
            average += tempBuf[i];
        }
        average /= 10;
        long temperature = (average * 330) >> 10;
        int newtemp = 0;
        long error = 0;
        error = (goaltemp - temperature);
        newtemp = (k*error) + goaltemp;
        setDutyCycle(newtemp);
        UCA0TXBUF = temperature;
        tempBuf_index = 0;
    }
}

void timerA0Interrupt(){
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE;
    ADC10CTL0 |= ENC;                         // ADC10 enable set
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void){
    pwmUARTInterrupt(UCA0RXBUF);
}
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)

{
    ADC10Interrupt();
}
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)

{
    timerA0Interrupt();
}
