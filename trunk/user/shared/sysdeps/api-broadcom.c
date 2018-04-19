/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <stdint.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
// GPIO
////////////////////////////////////////////////////////////////////////////////
extern uint32_t gpio_read(void);
extern void gpio_write(uint32_t bitvalue, uint32_t en);

int cpu_gpio_mode_set_bit(int bit, unsigned int value)
{
	/* Not implement! */
	return 0;
}

int cpu_gpio_mode_get_bit(int bit, unsigned int *p_value)
{
	/* Not implement! */
	return 0;
}

int cpu_gpio_set_pin_direction(int pin, unsigned int use_output_direction)
{
	/* Not implement! */
	return 0;
}

int cpu_gpio_set_pin(int pin, unsigned int value)
{
	gpio_write(pin, value);
	return 0;
}

int cpu_gpio_get_pin(int pin, unsigned int *p_value)
{
	uint32_t bit_value;
	uint32_t bit_mask;

	bit_mask = 1 << pin;
	bit_value = gpio_read() & bit_mask;

	*p_value = (bit_value == 0 ? 0 : 1);
	return 0;
}

int cpu_gpio_led_set(unsigned int led_pin, int blink_inverted)
{
	/* Not implement! */
	return 0;
}

int cpu_gpio_led_enabled(unsigned int led_pin, int enabled)
{
	/* Not implement! */
	return 0;
}

int cpu_gpio_irq_set(unsigned int irq_pin, int rising_edge, int falling_edge, pid_t pid)
{
	/* Not implement! */
	return 0;
}
