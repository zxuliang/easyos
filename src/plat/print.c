#include <easyplat.h>


char uart_getc(const unsigned long regbase)
{
	char t = 0;

	do
	{
		t = readb(regbase + UFR);
	} while(t & RXFE);

	return readb(regbase + UDR);
}

void uart_putc(const unsigned long regbase, const char c)
{
	unsigned char t = 0;

	do {
		t = readb(regbase + UFR);
	} while (t & TXFF);

	writeb((regbase + UDR), c);
}

int uart_print(const unsigned long regbase, const char *s)
{
	while (*s) {
		if (*s == '\n') {
			uart_putc(regbase, '\r');
			uart_putc(regbase, '\n');
		} else {
			uart_putc(regbase, *s);
		}
		s++;
	}

	return 0;
}

void console_print(const char *str)
{
	uart_print(UART0_BASE, str);
}

void console_putc(const char c)
{
	uart_putc(UART0_BASE, c);
}

char console_getc(void)
{
	return uart_getc(UART0_BASE);
}

void putchar(const char c)
{
	console_putc(c);
}

/*------------------------------------------------------*/

#define get_num_va_args(_args, _lcount)				\
	(((_lcount) > 1)  ? va_arg(_args, long long int) :	\
	(((_lcount) == 1) ? va_arg(_args, long int) :		\
			    va_arg(_args, int)))

#define get_unum_va_args(_args, _lcount)				\
	(((_lcount) > 1)  ? va_arg(_args, unsigned long long int) :	\
	(((_lcount) == 1) ? va_arg(_args, unsigned long int) :		\
			    va_arg(_args, unsigned int)))

static int string_print(const char *str)
{
	int count = 0;

	if (!str) {
		__asm__ __volatile__ (" b .");
	}

	for ( ; *str != '\0'; str++) {
		(void)putchar(*str);
		count++;
	}

	return count;
}

static int unsigned_num_print(unsigned long long int unum, unsigned int radix,
			      char padc, int padn)
{
	/* Just need enough space to store 64 bit decimal integer */
	char num_buf[20];
	int i = 0, count = 0;
	unsigned int rem;

	do {
		rem = unum % radix;
		if (rem < 0xa)
			num_buf[i] = '0' + rem;
		else
			num_buf[i] = 'a' + (rem - 0xa);
		i++;
		unum /= radix;
	} while (unum > 0U);

	if (padn > 0) {
		while (i < padn) {
			(void)putchar(padc);
			count++;
			padn--;
		}
	}

	while (--i >= 0) {
		(void)putchar(num_buf[i]);
		count++;
	}

	return count;
}

/*******************************************************************
 * Reduced format print for Trusted firmware.
 * The following type specifiers are supported by this print
 * %x - hexadecimal format
 * %s - string format
 * %d or %i - signed decimal format
 * %u - unsigned decimal format
 * %p - pointer format
 *
 * The following length specifiers are supported by this print
 * %l - long int (64-bit on AArch64)
 * %ll - long long int (64-bit on AArch64)
 * %z - size_t sized integer formats (64 bit on AArch64)
 *
 * The following padding specifiers are supported by this print
 * %0NN - Left-pad the number with 0s (NN is a decimal number)
 *
 * The print exits on all other formats specifiers other than valid
 * combinations of the above specifiers.
 *******************************************************************/
int vprintfx(const char *fmt, va_list args)
{
	int l_count;
	long long int num;
	unsigned long long int unum;
	char *str;
	char padc = '\0'; /* Padding character */
	int padn; /* Number of characters to pad */
	int count = 0; /* Number of printed characters */

	while (*fmt != '\0') {
		l_count = 0;
		padn = 0;

		if (*fmt == '%') {
			fmt++;
			/* Check the format specifier */
loop:
			switch (*fmt) {
			case 'i': /* Fall through to next one */
			case 'd':
				num = get_num_va_args(args, l_count);
				if (num < 0) {
					(void)putchar('-');
					unum = (unsigned long long int)-num;
					padn--;
				} else
					unum = (unsigned long long int)num;

				count += unsigned_num_print(unum, 10,
							    padc, padn);
				break;
			case 's':
				str = va_arg(args, char *);
				count += string_print(str);
				break;
			case 'p':
				unum = (uintptr_t)va_arg(args, void *);
				if (unum > 0U) {
					count += string_print("0x");
					padn -= 2;
				}

				count += unsigned_num_print(unum, 16,
							    padc, padn);
				break;
			case 'x':
				unum = get_unum_va_args(args, l_count);
				count += unsigned_num_print(unum, 16,
							    padc, padn);
				break;
			case 'z':
				if (sizeof(size_t) == 8U)
					l_count = 2;

				fmt++;
				goto loop;
			case 'l':
				l_count++;
				fmt++;
				goto loop;
			case 'u':
				unum = get_unum_va_args(args, l_count);
				count += unsigned_num_print(unum, 10,
							    padc, padn);
				break;
			case '0':
				padc = '0';
				padn = 0;
				fmt++;

				for (;;) {
					char ch = *fmt;
					if ((ch < '0') || (ch > '9')) {
						goto loop;
					}
					padn = (padn * 10) + (ch - '0');
					fmt++;
				}
			default:
				/* Exit on any other format specifier */
				return -1;
			}
			fmt++;
			continue;
		}
		(void)putchar(*fmt);
		fmt++;
		count++;
	}

	return count;
}


void printk(const char *fmt, ...)
{
	va_list va;
	unsigned int flag;

	flag = irq_lock_save();

	va_start(va, fmt);
	vprintfx(fmt, va);
	va_end(va);

	irq_unlock_restore(flag);
}
