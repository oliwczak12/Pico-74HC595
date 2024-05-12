/*
author: Mücahit KURTLAR
license: GNU GENERAL PUBLIC LICENSE Version 3
*/

#ifndef SHIFT_REGISTER_74HC595_H
#define SHIFT_REGISTER_74HC595_H

#define CLOCK_DELAY_US 5
#define LATCH_DELAY_US 5

#include "pico/stdlib.h"
#include <math.h>
#include <stdlib.h>

// Define shift register structure
struct shift_register_74hc595_t_chain {
    int clk_pin;
    int data_pin;
    int latch_pin;
    unsigned long long pin;
    unsigned int number_of_shreg;
    unsigned int numbers_of_pins;
};

/*! \brief Create a new shift register pointer with given pin numbers
 *
 * \param c_pin Clock pin
 * \param d_pin Data pin
 * \param l_pin Latch pin
 */
static struct shift_register_74hc595_t_chain *new_shreg_74hc595(int c_pin, int d_pin, int l_pin, unsigned int number_of_sr) {
    // Allocate memory for new shift register object
    struct shift_register_74hc595_t_chain *shreg_74hc595 = (struct shift_register_74hc595_t_chain *)malloc(sizeof(struct shift_register_74hc595_t_chain));

    // Set pins
    shreg_74hc595->clk_pin = c_pin;
    shreg_74hc595->data_pin = d_pin;
    shreg_74hc595->latch_pin = l_pin;
    shreg_74hc595->number_of_shreg = number_of_sr;
    shreg_74hc595->numbers_of_pins = number_of_sr * 8;
    // Set default pins state
    shreg_74hc595->pin = 0;

    // Return the created shift register
    return shreg_74hc595;
}

/*! \brief Free the memory allocated for the given shift register
 *
 * \param shreg_74hc595 Shift register pointer
 */
static void free_shreg_74hc595(struct shift_register_74hc595_t_chain *shreg_74hc595) {
    free(shreg_74hc595);
}

// Creates clock signal
static void clock_signal(struct shift_register_74hc595_t_chain *shreg_74hc595) {
    gpio_put(shreg_74hc595->clk_pin, 1);
    sleep_us(CLOCK_DELAY_US);
    gpio_put(shreg_74hc595->clk_pin, 0);
    sleep_us(CLOCK_DELAY_US);
}

// Enables latch pin
static void latch_enable(struct shift_register_74hc595_t_chain *shreg_74hc595) {
    gpio_put(shreg_74hc595->latch_pin, 1);
    sleep_us(LATCH_DELAY_US);
    gpio_put(shreg_74hc595->latch_pin, 0);
}

// Sends serial data to 74HC595
static void send_data(struct shift_register_74hc595_t_chain *shreg_74hc595, unsigned int data_out) {
    unsigned hold;
    // Send data as serail...
    // do zmainy na 8xilość sr
    for (int i = 0; i < shreg_74hc595->numbers_of_pins; i++) {
        gpio_put(shreg_74hc595->data_pin, (data_out >> i) & (0x01));
        clock_signal(shreg_74hc595);
    }
    // ... and enable latch
    latch_enable(shreg_74hc595);
}

/*! \brief Set selected pin digital high or digital low
 *
 * \param shreg_74hc595 Shift register pointer
 * \param pin Pin number (0-7) or (QA-QH)
 * \param value 1 for high, 0 for low
 */
static void shreg_74hc595_put(struct shift_register_74hc595_t_chain *shreg_74hc595, unsigned long long pin, bool value) {
    if (value) {
        // Use OR operator to set pin high
        shreg_74hc595->pin |= (unsigned long long)pow(2, (shreg_74hc595->numbers_of_pins - 1 - pin));
    } else {
        // Take complement of pin_bin and use AND operator to set pin low
        shreg_74hc595->pin &= ~(unsigned long long)pow(2, (shreg_74hc595->numbers_of_pins - 1 - pin));
    }
    // Send pin data to 74HC595
    send_data(shreg_74hc595, shreg_74hc595->pin);
}

#endif