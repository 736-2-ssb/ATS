#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "mstn_version.h"
#include "mstn_led.h"
#include "mstn_clk.h"
#include "mstn_usb.h"
#include "mstn_button.h"
#include "mstn_gpio.h"

#include "stdint.h"
#include "stdio.h"
#include "assert.h"
#include "main.h"


//ERROR CODES FOR FUNCTION update_diagramm()
#define PTR_TO_DIAGRAMM_IS_NULL         (uint8_t)1      //pointer to diagramm is 0
#define PTR_TO_STATE_MASK_IS_NULL       (uint8_t)2      //pointer to mask of letter is 0
#define PTR_TO_DICTIONARY_IS_NULL       (uint8_t)4      //pointer to dictionary is 0
#define PTR_TO_SIZES_ARR_IS_NULL        (uint8_t)8      //pointer to array of word's sizes is 0



uint32_t isFree = 4;    //Flag-variable for avaliable (not used) words.
                        //The i-th bit set to 1 means that the i-th word in dict is avaliable


//this struct describes one letter in word
typedef struct letter
{
    uint32_t code;          //sequance of 1 and 0 in letter
    uint8_t size;           //ammount of 1 and 0 in sequance
}letter;

//this struct describes the whole word
typedef struct diagramm
{
    letter* letters;        //pointer to array of letters
    uint8_t len;            //ammount of letters in array
    uint8_t letter_counter; //counter for current blinked letter
    uint8_t word_number;    //number of diagramm (0..32)
}diagramm;


void delay_msec(uint16_t msec)
{
    uint32_t delay = 1000*msec;
    while (delay)
    {
        --delay;
    }
}

//This function finds number of avaliable word using global flag isFree
//and returns number of this word
uint8_t find_free_word()
{
    uint8_t i = 0;
    while (!(isFree & (1 << i)))
    {
        ++i;
    }
    return i;
}

//This function update parametrs of diagramm
uint8_t update_diagramm(diagramm*   diag,                   //ptr to diagram that we want to change     (in/out)
                        uint32_t*   state_mask,             //ptr to state_mask for this diagramm       (in/out)
                        letter**    dictionary,             //ptr to dictionary with words              (in)
                        uint8_t*    sizes                   //array of sizes of working diagramms       (in/out)
                        )
{
    //DATA VALIDATION BLOCK:
    uint8_t status = 0;
    if (!diag)          status |= PTR_TO_DIAGRAMM_IS_NULL;
    if (!state_mask)    status |= PTR_TO_STATE_MASK_IS_NULL;
    if (!dictionary)    status |= PTR_TO_DICTIONARY_IS_NULL;
    if (!sizes)         status |= PTR_TO_SIZES_ARR_IS_NULL;
    if (status)         return status;

    //FUNCTION'S BODY:
    uint8_t free_number = find_free_word();                 //find free word


    isFree ^= (1 << free_number);                           //lock this free word
    isFree |= (1 << (diag->word_number));                   //and unlock current one

    //reinitialize current diagramm
    diag->letters = dictionary[free_number];                //put letters of new word to diagramm
    diag->len = sizes[free_number];                         //put amount of letters
    diag->letter_counter = 0;                               //drop letter counter
    diag->word_number = free_number;                        //write new number of word that will be shown

    *state_mask = 1 << (diag->letters[0].size - 1);         //write new mask of symbol

    return 0;
}


//Letters of English alphabet
letter let_A = {
    .code = 0b10111000,             //.-pause
    .size = 8};

letter let_B = {
    .code = 0b111010101000,         //-...pause
    .size = 12};

letter let_C = {
    .code = 0b11101011101000,       //-.-.pause
    .size = 14};
    
letter let_D = {
    .code = 0b1110101000,           //-..pause
    .size = 10};
    
letter let_E = {
    .code = 0b1000,                 //.pause
    .size = 4};
    
letter let_F = {
    .code = 0b101011101000,         //..-.pause
    .size = 12};
    
letter let_G = {
    .code = 0b111011101000,         //--.pause
    .size = 12};
    
letter let_H = {
    .code = 0b1010101000,           //....pause
    .size = 10};

letter let_I = {
    .code = 0b101000,               //..pause
    .size = 6};
    
letter let_J = {
    .code = 0b1011101110111000,     //.---pause
    .size = 16};
    
letter let_K = {
    .code = 0b111010111000,         //-.-pause
    .size = 12};
    
letter let_L = {
    .code = 0b101110101000,         //.-..pause
    .size = 12};
    
letter let_M = {
    .code = 0b1110111000,           //--pause
    .size = 10};
    
letter let_N = {
    .code = 0b11101000,             //-.pause
    .size = 8};
    
letter let_O = {
    .code = 0b11101110111000,       //---pause
    .size = 14};
    
letter let_P = {
    .code = 0b10111011101000,       //.--.pause
    .size = 14};
    
letter let_Q = {
    .code = 0b1110111010111000,     //--.-pause
    .size = 16};
    
letter let_R = {
    .code = 0b1011101000,           //.-.pause
    .size = 10};
    
letter let_S = {
    .code = 0b10101000,             //...pause
    .size = 8};
    
letter let_T = {
    .code = 0b111000,               //-pause
    .size = 6};
    
letter let_U = {
    .code = 0b1010111000,           //..-pause
    .size = 10};
    
letter let_V = {
    .code = 0b101010111000,         //...-pause
    .size = 12};
    
letter let_W = {
    .code = 0b101110111000,         //.--pause
    .size = 12};
    
letter let_X = {
    .code = 0b11101010111000,       //-..-pause
    .size = 14};
    
letter let_Y = {
    .code = 0b1110101110111000,     //-.--pause
    .size = 16};
    
letter let_Z = {
    .code = 0b11101110101000,       //--..pause
    .size = 14};
    
letter let_PAUSE = {
    .code = 0,                      //long pause
    .size = 10};

//*/

int main(void)
{
    uint8_t status = 0;     //exit-status for functions
    
   

    letter word_1[8] = {let_S, let_H, let_A, let_R, let_K, let_O, let_V, let_A};        //sharkova
    letter word_2[8] = {let_V, let_O, let_E, let_V, let_O, let_D, let_I, let_N};        //voevodin
    letter word_3[4] = {let_PAUSE, let_S, let_O, let_S};                                //sos

    letter* dictionary[3]   = {&word_1[0], &word_2[0], &word_3[0]};
    uint8_t sizes[3]        = { sizeof(word_1)/sizeof(typeof(word_1[0])),
                                sizeof(word_2)/sizeof(typeof(word_2[0])),
                                sizeof(word_3)/sizeof(typeof(word_3[0]))    };



    diagramm diag_1, diag_2;

    diag_1.letters = word_1;
    diag_1.len = sizes[0];
    diag_1.letter_counter = 0;
    diag_1.word_number = 0;

    diag_2.letters = word_2;
    diag_2.len = sizes[1];
    diag_2.letter_counter = 0;
    diag_2.word_number = 1;


    uint32_t state_mask_1 = 1 << (diag_1.letters[diag_1.letter_counter].size - 1);		//mask of states in a symbol
    uint32_t state_mask_2 = 1 << (diag_2.letters[diag_2.letter_counter].size - 1);
    
    //This pins will manage leds:
    GPIO_PinMode(D0, DIGITAL_OUTPUT);
    GPIO_PinMode(D1, DIGITAL_OUTPUT);
    
    //This pins are used as buttons
    GPIO_PinMode(D8, DIGITAL_INPUT_PULLDOWN);
    GPIO_PinMode(D9, DIGITAL_INPUT_PULLDOWN);

    

    BTN_UserButtonInit();
    while (1)
    {
        if(GPIO_DigitalRead(D8) == HIGH)
        {
            LED_SetGreenState(TURN_ON);
            status = update_diagramm(&diag_1, &state_mask_1, &dictionary[0], &sizes[0]);
            assert(status == 0);
            
        }
        else
        {
            LED_SetGreenState(TURN_OFF);
            //DELAY
        }
        
        if(GPIO_DigitalRead(D9) == HIGH)
        {
            LED_SetRedState(TURN_ON);
            status = update_diagramm(&diag_2, &state_mask_2, &dictionary[0], &sizes[0]);
            assert(status == 0);
        }
        else
        {
            LED_SetRedState(TURN_OFF);
            //DELAY
        }
                
                
        if (diag_1.letters[diag_1.letter_counter].code & state_mask_1)
        {
            GPIO_DigitalWrite(D0, 1);
        }
        else
        {
            GPIO_DigitalWrite(D0, 0);
        }
        
        if (diag_2.letters[diag_2.letter_counter].code & state_mask_2)
        {
            GPIO_DigitalWrite(D1, 1);
        }
        else
        {
            GPIO_DigitalWrite(D1, 0);
        }
        
        state_mask_1 >>= 1;
        state_mask_2 >>= 1;
        if (!state_mask_1)
        {
            diag_1.letter_counter++;
            if (diag_1.letter_counter > diag_1.len - 1)
            {
                diag_1.letter_counter = 0;
            }
            else
            {
                //DELAY
            }

            state_mask_1 = 1 << (diag_1.letters[diag_1.letter_counter].size - 1);
        }
        else
        {
            //DELAY
        }


        if (!state_mask_2)
        {
            diag_2.letter_counter++;
            if (diag_2.letter_counter > diag_2.len - 1)
            {
                diag_2.letter_counter = 0;
            }
            else
            {
                //DELAY
            }
            state_mask_2 = 1 << (diag_2.letters[diag_2.letter_counter].size - 1);
        }
        else
        {
            //DELAY
        }
        Delay(200);
    }
    return 0;
}