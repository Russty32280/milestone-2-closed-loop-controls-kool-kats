#include <msp430.h> 

//Declarations of all helper functions
void setPWM(unsigned int duty);
void setUpPWM();
void pwmUARTInterrupt(long bufInput);
void setUpUART();
void setUpADC();
void ADC10Interrupt();
void timerA1Interrupt();
void setUpTimerA0();
void setDutyCycle(int temp);

//Fields for average of 10 points
int tempBuf[10];
int tempBuf_index = 0;

//fields for error multiplication constant and the goal temperature
int k =5;
long goaltemp = 40;

/**
* Closed-loop control program receives a goal temperature over uart, sets a PWM output to control a fan, and updates that PWM based on the current temperature of the voltage
* regulator that the fan is cooling. 
* @author Russell Binaco, Jake Fraser, Tanner Smith, Gray Thurston, Tomas Uribe
* main sets up timers, UART communication and ADC channel, and initializes the duty cycle of the PWM. 
**/
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

/**
* setUpPWM uses P2.1 as the PWM output pin
* TA1CCR1/1000 is the duty cycle, initialized to 0.5 or 50%
*
**/
void setUpPWM(){
    P2DIR |= BIT1;                            // P2.1 output
    P2SEL |= BIT1;                            // P2.1 for TA0 CCR1 Output Capture
    P2SEL2 = 0;                               // Select default function for P1.6 (see table 19 in general datasheet)
    TA1CCR0 = 1000;                             // PWM Freq=1000Hz
    TA1CCTL1 = OUTMOD_7;                         // CCR1 reset/set: set on at CCR0, off at CCR1 capture (see table 12-2 in specific datasheet)
    TA1CCR1 = 500;                               // CCR1 50% PWM duty cycle
    TA1CTL = TASSEL_2 + MC_1 + ID_0;                  // SMCLK, up mode, 1MhZ
}

/**
* setUpUART uses 9600 BAUD rate, P1.1 for RX and P1.2 for TX. 
*
**/
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

/**
* setUpADC selects A7, which is P1.7, and uses Vcc as reference. 
*
**/
void setUpADC(){
    ADC10CTL1 = INCH_7 + SHS_1;               // P1.7, TA1 trigger sample start
    ADC10AE0 = 0x80;                          // P1.7 ADC10 option select

}

/**
* setUpTimerA0: Timer A0 is the timer that controls UART sampling
* in upmode, triggers at 2Hz. 
*
**/
void setUpTimerA0(){
    TA0CCTL0 = CCIE;                           // Enable interrupt
    TA0CCR0 = 65534;                           // PWM Period
    TA0CCTL1 = OUTMOD_3;                       // TACCR1 set/reset
    TA0CCR1 = 781;                               // TACCR1 PWM Duty Cycle
    TA0CTL = TASSEL_2 + MC_1 + ID_3;                  // ACLK, up mode
}

/**
* setDutyCycle takes a goal temperature and uses a piecewise linear function to set the PWM to a corresponding duty cycle
* @param temp the temperature used to set the corresponding duty cycle
* the piecewise linear function was found by characterizing the previously open-loop system at incremented duty cycle values
*
**/
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

/**
* setPWM sets the PWM duty cycle corresponding to a duty cycle percentage input
* @param bitDuty the duty cycle to set the PWM to. Multiplies by 10 since CCR0 is 1000. 
*
**/
void setPWM(unsigned int bitDuty){
    if(bitDuty>100){
        bitDuty = 100;
    }
    bitDuty = bitDuty * 10;
    TA1CCR1 = bitDuty;
}

/**
* pwmUARTInterrupt receives a value over UART and assigns it to the goal temperature. 
* @param bufInput the value from the RX buffer
*
**/
void pwmUARTInterrupt(long bufInput){
    goaltemp = bufInput;;
}

/**
* ADC10Interrupt disables the ADC then builds an array to compute the average of 10 values (achieved in 5 seconds) which is used to update the PWM value according to the error
* between the goal temperature and the current temperature. Also transmits the average of 10 via UART.
* 
**/
void ADC10Interrupt(){
    ADC10CTL0 &= ~ENC;                        			// ADC10 disabled
    ADC10CTL0 = 0;                            			// ADC10, Vref disabled completely
	
    if(tempBuf_index < 10){					  			//receive a new value and put it in the next index location in the array of data
        tempBuf[tempBuf_index] = ADC10MEM;
        tempBuf_index++;
    }
    else{									  			//compute the average of 10 from the array then update the PWM value and output the current temperature
        long average = 0;
        int i = 0;
        for(i = 0; i < 10 ; i ++){
            average += tempBuf[i];
        }
        average /= 10;						  			//average computed
        long temperature = (average * 330) >> 10;		//convert to degrees
        int newtemp = 0;
        long error = 0;
        error = (goaltemp - temperature);				//comparator error signal
        newtemp = (k*error) + goaltemp;					//COB to calculate updated "goal" temperature, accounting for error via feedback with a gain of k
        setDutyCycle(newtemp);							//update PWM
        UCA0TXBUF = temperature;						//transmit current temperature
        tempBuf_index = 0;								//reset data collection
    }
}

/**
* timerA0 interrupt turns the ADC back on to collect another data point
* this triggers at 2Hz.
*
**/
void timerA0Interrupt(){
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE;
    ADC10CTL0 |= ENC;                         // ADC10 enable set
}

/**
* UART RX ISR
* updates the goal temperature. 
*
**/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void){
    pwmUARTInterrupt(UCA0RXBUF);
}

/**
* ADC10 ISR
* computes average of 10, updates PWM, transmits current temperature, turns off ADC. 
*
**/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)

{
    ADC10Interrupt();
}

/**
* Timer A0 ISR
* turns the ADC on
*
**/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)

{
    timerA0Interrupt();
}
