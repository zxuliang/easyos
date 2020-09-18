#include <easyos.h>

#define TXFE 0x80
#define RXFF 0x40
#define TXFF 0x20
#define RXFE 0x10
#define BUSY 0x08

#define UDR 0x00
#define UFR 0x18

#define UART0_BASE 		(0x101F1000)
#define UART1_BASE 		(0x101F2000)
#define UART2_BASE 		(0x101F3000)
#define UART3_BASE 		(0x10009000)

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