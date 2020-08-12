/*
 * (C) Copyright 2006-2008
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
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

#include <common.h>
#include <asm/io.h>
#include <linux/string.h>

extern int nuc980_serial_init (void);
extern void nuc980_serial_putc (const char ch);
extern void nuc980_serial_puts (const char *s);
extern int nuc980_serial_getc (void);
extern int nuc980_serial_tstc (void);
extern void nuc980_serial_setbrg (void);

#define ENDSTRING '\0'
/* CWWeng : use lib/string.c
__kernel_size_t strlen(const char *string)
{
 int len=0;
 while(*string++ != ENDSTRING)
 len++;
 return len;
}
*/
void __div0 (void)
{
	//extern void hang (void);
	//hang();
}
typedef int               INT;
typedef char              INT8;
typedef char *            PINT8;
typedef unsigned char     UINT8;
typedef short             INT16;
typedef unsigned short    UINT16;
typedef int               INT32;
typedef unsigned int      UINT32;
#define FALSE             0
#define TRUE              1

void sysPutString(INT8 *string);
void sysprintf(PINT8 pcStr,...);

#define vaStart(list, param) list = (INT8*)((INT)&param + sizeof(param))
#define vaArg(list, type) ((type *)(list += sizeof(type)))[-1]
#define printf(fmt, arg...)		sysprintf(fmt, ##arg)
void _PutChar_f(UINT8 ucCh){
	//serial_putc(ucCh);
	nuc980_serial_putc(ucCh);
}
static void sysPutStringReverse(INT8 *s, INT index)
{
    while ((index--) > 0)
        _PutChar_f(s[index]);
}
void sysPutString(INT8 *string)
{
    while (*string != '\0')
    {
        _PutChar_f(*string);
        string++;
    }
}


static void sysPutRepChar(INT8 c, INT count)
{
    while (count--)
        _PutChar_f(c);
}

static void sysPutNumber(INT value, INT radix, INT width, INT8 fill)
{
    INT8    buffer[40];
    INT     bi = 0;
    UINT32  uvalue;
    UINT16  digit;
    UINT16  left = FALSE;
    UINT16  negative = FALSE;

    if (fill == 0)
        fill = ' ';

    if (width < 0)
    {
        width = -width;
        left = TRUE;
    }

    if (width < 0 || width > 80)
        width = 0;

    if (radix < 0)
    {
        radix = -radix;
        if (value < 0)
        {
            negative = TRUE;
            value = -value;
        }
    }

    uvalue = value;

    do
    {
        if (radix != 16)
        {
            digit = uvalue % radix;
            uvalue = uvalue / radix;
        }
        else
        {
            digit = uvalue & 0xf;
            uvalue = uvalue >> 4;
        }
        buffer[bi] = digit + ((digit <= 9) ? '0' : ('A' - 10));
        bi++;

        if (uvalue != 0)
        {
            if ((radix == 10)
                && ((bi == 3) || (bi == 7) || (bi == 11) | (bi == 15)))
            {
                buffer[bi++] = ',';
            }
        }
    }
    while (uvalue != 0);

    if (negative)
    {
        buffer[bi] = '-';
        bi += 1;
    }

    if (width <= bi)
        sysPutStringReverse(buffer, bi);
    else
    {
        width -= bi;
        if (!left)
            sysPutRepChar(fill, width);
        sysPutStringReverse(buffer, bi);
        if (left)
            sysPutRepChar(fill, width);
    }
}


static INT8 *FormatItem(INT8 *f, INT a)
{
    INT8   c;
    INT    fieldwidth = 0;
    INT    leftjust = FALSE;
    INT    radix = 0;
    INT8   fill = ' ';

    if (*f == '0')
        fill = '0';

    while ((c = *f++) != 0)
    {
        if (c >= '0' && c <= '9')
        {
            fieldwidth = (fieldwidth * 10) + (c - '0');
        }
        else
            switch (c)
            {
                case '\000':
                    return (--f);
                case '%':
                    _PutChar_f('%');
                    return (f);
                case '-':
                    leftjust = TRUE;
                    break;
                case 'c':
                    {
                        if (leftjust)
                            _PutChar_f(a & 0x7f);

                        if (fieldwidth > 0)
                            sysPutRepChar(fill, fieldwidth - 1);

                        if (!leftjust)
                            _PutChar_f(a & 0x7f);
                        return (f);
                    }
                case 's':
                    {
                        if (leftjust)
                            sysPutString((PINT8)a);

                        if (fieldwidth > strlen((PINT8)a))
                            sysPutRepChar(fill, fieldwidth - strlen((PINT8)a));

                        if (!leftjust)
                            sysPutString((PINT8)a);
                        return (f);
                    }
                case 'd':
                case 'i':
                    radix = -10;
                    break;
                case 'u':
                    radix = 10;
                    break;
                case 'x':
                    radix = 16;
                    break;
                case 'X':
                    radix = 16;
                    break;
                case 'o':
                    radix = 8;
                    break;
                default:
                    radix = 3;
                    break;      /* unknown switch! */
            }
        if (radix)
            break;
    }

    if (leftjust)
        fieldwidth = -fieldwidth;

    sysPutNumber(a, radix, fieldwidth, fill);

    return (f);
}


void sysprintf(PINT8 pcStr,...)
{
//    WB_UART_T uart;
    INT8  *argP;
    vaStart(argP, pcStr);       /* point at the end of the format string */
    while (*pcStr)
    {                       /* this works because args are all ints */
        if (*pcStr == '%')
            pcStr = FormatItem(pcStr + 1, vaArg(argP, INT));
        else
            _PutChar_f(*pcStr++);
    }
}
