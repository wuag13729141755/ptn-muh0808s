/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed “AS IS” WITHOUT ANY WARRANTY of any kind,
* whether express or implied, including without limitation the implied warranty
* of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE or NON-INFRINGEMENT.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* You may contact Lattice Semiconductor Corporation at 111 SW 5th Ave,
* Suite 700, Portland, OR 97204
*/

#include "Sii9630_si_datatypes.h"
#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_platform.h"
#include "usart.h"

#define MAX_DBG_MSG_LEN 160

//struct semaphore lock_to_log;
long long bytes_to_write;
long long bytes_written;
struct file *fp;

#if 0//def CONFIG_CAPTURE_REGISTER_DUMP
static void my_itoa_buf(char *buf, size_t len, uint16_t num);
static int sii_lib_log_write(struct file *fp, uint8_t *, const int len);
static void sii_lib_write_to_log(char *, uint16_t, bool);
#endif

#ifdef SII_DEBUG

void sii_log_dbg_printf(char const *func_name, const char *p_fmt, ...)
{
	va_list arg;
	uint8_t *p_msg;
	uint8_t pl_u8msg_start[MAX_DBG_MSG_LEN];
	int len = 0;
	int max_msg_len = MAX_DBG_MSG_LEN;
    int l_s32LastLen = 0;
#if (_DEBUG_PORT_SEL==_UART_PORT_1)
    USART_TypeDef *pstUart = USART1;
#elif (_DEBUG_PORT_SEL==_UART_PORT_2)
    USART_TypeDef *pstUart = USART2;
#elif (_DEBUG_PORT_SEL==_UART_PORT_3)
    USART_TypeDef *pstUart = USART3;
#else
    USART_TypeDef *pstUart = USART2;
#endif

	if (p_fmt == NULL) {
		pr_err("invalid arguments");
		return;
	}
	memset(pl_u8msg_start,0,MAX_DBG_MSG_LEN);
	p_msg = pl_u8msg_start;
	if (func_name) {
		len = Uart_snprintf((char *)p_msg, max_msg_len, "%s", func_name);
		p_msg += len;
		max_msg_len -= len;
        l_s32LastLen += len;
	}
	len = Uart_snprintf((char *)p_msg, max_msg_len, "%s", "::");
	p_msg += len;
	max_msg_len -= len;
    l_s32LastLen += len;
	va_start(arg, p_fmt);
	len = Uart_vsnprintf((char *)p_msg, max_msg_len, p_fmt, arg);
	va_end(arg);
    l_s32LastLen += len;

	USARTx_DirectSend_bytes(pstUart, pl_u8msg_start, l_s32LastLen);
}
#endif

#ifdef SII_DEBUG
void sii_log_printf(const char *p_fmt, ...)
{
	va_list arg;
	uint8_t p_msg[MAX_DBG_MSG_LEN];
	int len = 0;
	int max_msg_len = MAX_DBG_MSG_LEN;
#if (_DEBUG_PORT_SEL==_UART_PORT_1)
        USART_TypeDef *pstUart = USART1;
#elif (_DEBUG_PORT_SEL==_UART_PORT_2)
        USART_TypeDef *pstUart = USART2;
#elif (_DEBUG_PORT_SEL==_UART_PORT_3)
        USART_TypeDef *pstUart = USART3;
#else
        USART_TypeDef *pstUart = USART2;
#endif

	va_start(arg, p_fmt);
	len = Uart_vsnprintf((char *)p_msg, max_msg_len, p_fmt, arg);
	va_end(arg);

	USARTx_DirectSend_bytes(pstUart, p_msg, len);
}

//#define sii_log_printf   dbg_printf
#endif

#if 0//def CONFIG_CAPTURE_REGISTER_DUMP
void sii_lib_log_init(void)
{
	mm_segment_t user_fs;
	user_fs = get_fs();
	set_fs(KERNEL_DS);
	fp = filp_open("/dev/sii_logdump.txt", O_CREAT, 777);
	set_fs(user_fs);
	sema_init(&lock_to_log, 1);
	if (IS_ERR(fp)) {
		pr_err("%s: open file error\n", __func__);
		goto exit;
	} else {
	}
	goto done;
exit: sii_lib_log_exit();
done:
	pr_info("\nLog file opened successfully\n");
}

void sii_lib_log_exit(void)
{
	if (fp) {
		bytes_to_write = 0;
		filp_close(fp, NULL);
		pr_info("\n Log File closed");
	}
}

void sii_lib_write_to_log_file(unsigned char *buf, const uint16_t in_length,
				bool mode)
{
	if (fp) {
		if (!down_interruptible(&lock_to_log)) {
			if (mode == SII_WRITE) {
				sii_lib_write_to_log("WRITE:: ", 1,
					SII_WRITE_STRING);
			} else {
				sii_lib_write_to_log("READ::  ", 1,
					SII_WRITE_STRING);
			}
			sii_lib_write_to_log(buf, in_length, SII_WRITE_DATA);
			sii_lib_write_to_log("\n", 1, SII_WRITE_STRING);
			up(&lock_to_log);
		}
	} else {
		pr_info("Capturing log Functionality disabled");
	}
}
static void sii_lib_write_to_log(char *buf, const uint16_t in_length, bool mode)
{
	int ret;
	int i;
	char l_s8str[2];
	char *str = l_s8str;
	char *str_start;
	bool address_enable = 0;
	int address_byte;

    memset(l_s8str,0,2);

	str_start = str;
	address_byte = buf[0] << 8 | buf[1];
	if (str == NULL) {
		pr_err("\n memory allocation failed for log mesage\n");
		return;
	}
	for (i = 0; i < in_length; i++) {
		if (mode == SII_WRITE_DATA) {
			if (address_enable == 0) {
				my_itoa_buf(str, 2, address_byte);
				i++;
				address_enable = 1;

			} else {
				my_itoa_buf(str, 1, buf[i]);
			}
		} else {
			str = buf;
		}
		ret = sii_lib_log_write(fp, str, strlen(str));

		if (ret)
			pr_info("\nwriting into log buffer failed!!!\n");

		str = "\0";
		str = str_start;
	}
}
static int sii_lib_log_write(struct file *fp, uint8_t *buf, const int len)
{
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	bytes_to_write = fp->f_op->write(fp, (char *)buf, len, &fp->f_pos);
	set_fs(old_fs);
	if (bytes_to_write == len) {
		fp->f_pos += bytes_to_write;
		bytes_to_write = 0;
		goto exit;
	}
	if (bytes_to_write >= 0)
		bytes_to_write = -EIO;
exit:

	return bytes_to_write;
}
static void my_itoa_buf(char *buf, size_t len, uint16_t num)
{
	snprintf(buf, len, "%02x", num);
}
#endif

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
