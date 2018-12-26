/**
 * @file leds.ino
 * @date 24.12.2018
 * @author Martin Smutny (xsmutn13)
 * @brief TODO
 *
 */

#include "leds.h"
#include "main.h"

/* Global declarations */
extern uint8_t cur_seq;		/* current sequence TODO in HEADER?? */
extern uint8_t LED_cnt;		/* Auxilliary LED counter */
extern uint16_t cur_speed;	/* Current speed */
extern uint8_t cur_pos;		/* Current position */
extern uint8_t cur_len;		/* Current Number of LEDs */
extern uint8_t cur_direct;	/* Current direction */

uint8_t seq_arr[] = {LED_1, LED_2, LED_3, 
					 LED_4, LED_5, LED_6, 
					 LED_7, LED_8, LED_9};

// TODO array of current lit leds - bool? or BITS
// TODO current param of seq
// TODO current speed

void ydelay(unsigned long ms)
{
    uint32_t start = micros();

    while (ms > 0) {
        yield();
        while ( ms > 0 && (micros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
    }
}

/**
 * @brief Sets all LED pins to digital LOW
 */
inline void led_reset()
{
	digitalWrite(LED_1, LOW);
	digitalWrite(LED_2, LOW);
	digitalWrite(LED_3, LOW);
	digitalWrite(LED_4, LOW);
	digitalWrite(LED_5, LOW);
	digitalWrite(LED_6, LOW);
	digitalWrite(LED_7, LOW);
	digitalWrite(LED_8, LOW);
	digitalWrite(LED_9, LOW);
}

/**
 * @brief Switches an LED on/off
 * @param led_pin Predefined LED pin
 * @param state ON or OFF
 */
inline void set_led(uint8_t led_pin, uint8_t state)
{
	digitalWrite(led_pin, state);
}

/**
 * @brief Performs LED reset and calls appropriate sequence function
 * TODO PARAMETERS
 * @param seq_id Uniquely identifies an LED sequence
 */
inline void change_seq(uint8_t seq_id)
{
	if (cur_seq != seq_id)
	{
		led_reset();
		cur_seq = seq_id;
	}
}

/**
 * @brief TODO
 *
 */
void cur_seq_continue()
{
	switch(cur_seq)
	{
	/* case SEQ_INDIV: Is manual */
	case SEQ_ONE:
		seq_one_by_one(cur_speed);
		break;
	case SEQ_ROW:
		seq_row(cur_speed, cur_pos);
		break;
	case SEQ_COL:
		seq_row(cur_speed, cur_pos);
		break;
	case SEQ_CIRC:
		seq_circle(cur_speed, cur_len, cur_direct);
		break;
	case SEQ_SWAP:
		seq_swap(cur_speed);
		break;
	case SEQ_ARROW:
		seq_arrow(cur_speed, cur_direct);
		break;
	}
}

/**
 * @brief Individual LED on/off
 */
void seq_individual(uint8_t led_pin, bool state)
{	
	change_seq(SEQ_INDIV);		/* change current sequence */
	set_led(led_pin, state);
}

/**
 * @brief One by one LED (sweep)
 * @param speed Switching speed 3000 - 100
 */
void seq_one_by_one(uint16_t speed)
{
	change_seq(SEQ_ONE);		/* change current sequence */
	uint8_t i = 0;
	while (42)					/* TODO remove LOOP */
	{
		set_led(seq_arr[i], ON);
		delay(speed);
    set_led(seq_arr[i], OFF);
		i = (i+1)%TOTAL_LED;
	}
}

/**
 * @brief Row by Row sequence
 * @param speed Switching speed
 * @param pos Starting position
 */
void seq_row(uint16_t speed, uint8_t pos)
{
	/* TODO local in loop() sufficient? */	
			
	change_seq(SEQ_ROW);		/* change current sequence */
	
	uint8_t start_pos = pos;
	while(42)
	{
		set_led(seq_arr[start_pos-1], ON);	// TODO POSITION CHANGING
		set_led(seq_arr[start_pos], ON);
		set_led(seq_arr[start_pos+1], ON);
		delay(speed);
		set_led(seq_arr[start_pos-1], OFF);
		set_led(seq_arr[start_pos], OFF);
		set_led(seq_arr[start_pos+1], OFF);
		switch(start_pos)     // TODO
		{
		case POS_TOP:
			pos = POS_BOTTOM;		/* setting next position */
			break;
		case POS_BOTTOM:
			pos = POS_TOP;
			break;
		}
		set_led(LED_4, ON);
		set_led(LED_5, ON);
		set_led(LED_6, ON);
		delay(speed);
		set_led(LED_4, OFF);
		set_led(LED_5, OFF);
		set_led(LED_6, OFF);
		set_led(seq_arr[pos-1], ON);
		set_led(seq_arr[pos], ON);
		set_led(seq_arr[pos+1], ON);	
		delay(speed);	
		set_led(seq_arr[pos-1], OFF);
		set_led(seq_arr[pos], OFF);
		set_led(seq_arr[pos+1], OFF);	
	}
}

/**
 * @brief Column by Column sequence
 * @param speed Switching speed
 * @param pos Starting position
 */
void seq_col(uint16_t speed, uint8_t pos)
{
	/* TODO local in loop() sufficient? 			
	uint8_t seq_arr[] = {LED_1, LED_2, LED_3, 
						 LED_4, LED_5, LED_6, 
						 LED_7, LED_8, LED_9}; */

  uint8_t wseq_arr[] = { LED_1,  LED_4, LED_7, // TODO
                        LED_2, LED_5, LED_8, 
                        LED_3, LED_6, LED_9};

	change_seq(SEQ_COL);		/* change current sequence */
	
	uint8_t start_pos = pos;
	while(42)
	{
		set_led(wseq_arr[start_pos-1], ON);
		set_led(wseq_arr[start_pos], ON);
		set_led(wseq_arr[start_pos+1], ON);
		delay(speed);
		set_led(wseq_arr[start_pos-1], OFF);
		set_led(wseq_arr[start_pos], OFF);
		set_led(wseq_arr[start_pos+1], OFF);
		switch(start_pos)
		{
    case POS_TOP:
      pos = POS_BOTTOM;   /* setting next position */
      break;
    case POS_BOTTOM:
      pos = POS_TOP;
      break;
		}
		set_led(LED_2, ON);
		set_led(LED_5, ON);
		set_led(LED_8, ON);	
		delay(speed);
		set_led(LED_2, OFF);
		set_led(LED_5, OFF);
		set_led(LED_8, OFF);	
		set_led(wseq_arr[pos-1], ON);
		set_led(wseq_arr[pos], ON);
		set_led(wseq_arr[pos+1], ON);	
		delay(speed);	
		set_led(wseq_arr[pos-1], OFF);
		set_led(wseq_arr[pos], OFF);
		set_led(wseq_arr[pos+1], OFF);	
	}
}

/**
 * @brief Performs rotation, one by one in a circle fashion
 * @param speed Speed of switching, 3000 - 100
 * @param led_len Total LEDs switch at once 9 - 1
 * @param direct Direction - clockwise / Anticlockwise
 */
void seq_circle(uint16_t speed, uint8_t led_len, uint8_t direct)
{
  (void)direct;
	change_seq(SEQ_CIRC);
	uint8_t cseq_arr[] = {LED_1, LED_2, LED_3, 
					 	            LED_6,		LED_9, 
						            LED_8, LED_7, LED_4};
	int8_t i = 0;		/* Current Led */
	int8_t y = 0;
	while(42)
	{
		for (uint8_t num = 0; num < led_len; num++)
		{	
			set_led(cseq_arr[i], ON);
			i = (i+1)%(TOTAL_LED-1);
		}
		y = i-1;
		if (y < 0) { y = 7; }
		delay(speed-10);
		for (uint8_t num = 0; num < led_len; num++)
		{	
			set_led(cseq_arr[y], OFF);
			if (!y) { y = 8; }
			y = (y-1)%(TOTAL_LED-1);
		}
	}
}

/**
 * @brief 
 * @param speed Speed of switching, 3000 - 100
 */
void seq_swap(uint16_t speed)
{
	change_seq(SEQ_SWAP);
	while(42)				// TODO switch
	{
		set_led(LED_5, ON);
		delay(speed);
		set_led(LED_5, OFF);

		set_led(LED_2, ON);
		set_led(LED_4, ON);
		set_led(LED_6, ON);
		set_led(LED_8, ON);

		delay(speed);
		set_led(LED_2, OFF);
		set_led(LED_4, OFF);
		set_led(LED_6, OFF);
		set_led(LED_8, OFF);

		set_led(LED_1, ON);
		set_led(LED_3, ON);
		set_led(LED_7, ON);
		set_led(LED_9, ON);
		delay(speed);
		set_led(LED_1, OFF);
		set_led(LED_3, OFF);
		set_led(LED_7, OFF);
		set_led(LED_9, OFF);
	}
}

/**
 * @brief 
 * @param speed Speed of switching, 3000 - 100
 * @param direct Direction from which it starts
 */
void seq_arrow(uint16_t speed, uint8_t direct)
{	
	change_seq(SEQ_ARROW);
	switch(direct)
	{
	case POS_TOP: case POS_LEFT: case POS_RIGHT: case POS_BOTTOM: /* TODO make more*/
		set_led(LED_2, ON);
		delay(speed);

		set_led(LED_1, ON);
		set_led(LED_3, ON);
		set_led(LED_5, ON);
		delay(speed);

		set_led(LED_1, OFF);
		set_led(LED_3, OFF);
		set_led(LED_4, ON);
		set_led(LED_6, ON);
		set_led(LED_8, ON);
		delay(speed);

		set_led(LED_4, OFF);
		set_led(LED_6, OFF);
		set_led(LED_7, ON);
		set_led(LED_9, ON);
		delay(speed);

		set_led(LED_2, OFF);
		set_led(LED_7, OFF);
		set_led(LED_9, OFF);
		delay(speed);
		
		set_led(LED_5, OFF);
		delay(speed);

		set_led(LED_8, OFF);
		break;	
	}	
}
