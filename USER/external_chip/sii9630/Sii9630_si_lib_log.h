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

#ifndef __SI_LIB_LOG_API_H__
#define __SI_LIB_LOG_API_H__


enum dbg_msg_level {
	DBG_MSG_CRIT_ERR = 0,
	DBG_MSG_WARN,
	DBG_MSG_INFO,
	DBG_MSG_DEBUG,
};

#define SII_WRITE_DATA 0
#define SII_WRITE_STRING 1
#define SII_WRITE 0
#define SII_READ  1

#define sii_log_generic_dbg(level, ...)     sii_log_dbg_printf(__func__, __VA_ARGS__)

#define sii_log_generic_dbg_print(level, ...)   sii_log_printf(__VA_ARGS__)



/* public functions */
void sii_log_dbg_printf(const char *, const char *, ...);
void sii_log_printf(const char *, ...);
void sii_lib_write_to_log_file(unsigned char *, const uint16_t, bool);
void sii_lib_log_init(void);
void sii_lib_log_exit(void);

#endif /* __SI_LIB_LOG_API_H__ */
