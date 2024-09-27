## Timer Interrupts PIC12F675  MD Harrington  London Kent DA6 8NP 

The project uses the 12F675 range of micro chips 8 bit cpu range using the XC8 compiler  to  create a countdown timer 
for latching a relay untill you push button attached to GP3 

## Theory of operation 

![FlowchartImage](https://github.com/user-attachments/assets/809f3830-8e47-4afe-821a-c7658cf33f72)#

 Initialization:
 
1 . GPIO and Peripherals Setup (InitCCT()):

    All ports are set as digital I/O.
    Analog-to-Digital, Comparator, and Voltage Reference are disabled.
    GP3 is configured as input; other pins are set as outputs.
    Timer0 is initialized with a 1-second interrupt using a 1:256 prescaler.
    GPIO is initialized to 0.

2. Main Loop:

    Global Interrupts are enabled, and Timer0 interrupt is set up.
    The program enters an infinite loop (while (1)), where it continuously checks the state of GP3 using the checkGP3() function.

3. Checking GP3 (checkGP3() function):

    If the GP3 pin is pulled low:
        GP5 (Relay) is set to low (relay off).
        The m_counter is reset to 9.
        The Timer0 interrupt is re-enabled.

4. Timer Interrupt Service Routine (myISR()):

    The 1-second Timer0 interrupt:
        TMR0 is reloaded to overflow every second.
        second_counter increments with each interrupt, counting 1 second intervals.
    Every 10 seconds:
        The current value from m_array[m_counter] is output to GPIO.
        If m_counter reaches 0:
            GP5 is set to high (Relay ON).
            Timer0 interrupt is disabled.
        Otherwise:
            The m_counter decrements.

5. Key Variables:

    m_counter: Starts at 9, decrements every 10 seconds until it reaches 0.
    second_counter: Tracks time in seconds, resets every 10 seconds.
    GP3: Input switch; when pulled low, it resets m_counter and disables GP5.
    GP5: Controls relay, set high when m_counter = 0, and low when GP3 is pressed.


## Circuit description

![schematic](https://github.com/user-attachments/assets/9c39b0a3-825a-415d-9acc-88632bc1f7b2)

Button with 10k resistor attached to supply then to GP3  Switch push button attaches to GP3 then ground 

GPO CONNECTS TO  DATA0 4511 BCD 7SEG DECODER
 
GP1 CONNECTS TO  DATA1 4511 BCD 7SEG DECODER 

GP2 CONNECTS TO  DATA2 4511 BCD 7SEG DECODER 

GP4 CONNECTS TO  DATA3 4511 BCD 7SEG DECODER 

GP3 CONNECTS TO SWITCH CONNECTS TO  GND AND TO 10K RESISTOR  ATTACHED TO SUPPLY RAIL 

BCD DECODER  CONNECTS TO 7 SEGMENT DISPLAY 

Ive akso included the simulation  file for use with Simul IDE 

Below is textual flow chart of operation

