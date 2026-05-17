#include <unistd.h>

#include "print.h"

void print_str(char *str)
{
        if (!str) {
                write(1, "(null)", 6);
                return;
        }

        size_t len = 0;
        while (str[len] != '\0')
                len++;

        write(1, str, len);
}

void print_szt(size_t size)
{
        char buff[BUFF_SIZE];
        int i = BUFF_SIZE - 1;
        buff[i] = '\0';

        if (size == 0) {
                buff[--i] = '0';
        } else {
                while (size > 0) {
                        buff[--i] = (size % 10) + '0';
                        size /= 10;
                }
        }

        print_str(&buff[i]);
}

void print_ptr(void *ptr)
{
        unsigned long num = (unsigned long)ptr;
        char buff[BUFF_SIZE];
        int i = BUFF_SIZE - 1;
        buff[i] = '\0';

        const char hex_chars[] = "0123456789abcdef";

        if (num == 0) {
                buff[--i] = '0';
        } else {
                while (num > 0) {
                        buff[--i] = hex_chars[num % 16];
                        num /= 16;
                }
        }

        buff[--i] = 'x';
        buff[--i] = '0';

        print_str(&buff[i]);
}
