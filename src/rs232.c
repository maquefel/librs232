/*
 * Copyright (c) 2011 Petr Stetiar <ynezz@true.cz>, Gaben Ltd.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "librs232/rs232.h"

static const char *
rs232_baud[] = {
	"300",
	"2400",
	"4800",
	"9600",
	"19200",
	"38400",
	"57600",
	"115200",
	"460800",
};

static const char *
rs232_data[] = {
	"5",
	"6",
	"7",
	"8",
};

static const char *
rs232_parity[] = {
	"none",
	"odd",
	"even",
};

static const char *
rs232_stop[] = {
	"1",
	"2",
};

static const char *
rs232_flow[] = {
	"off",
	"hardware",
	"xon/xoff",
};

static const char *
rs232_dtr[] = {
	"off",
	"on",
};

static const char *
rs232_rts[] = {
	"off",
	"on",
};

static const char *
rs232_error[] = {
	"no error",
	"unknown error",
	"open error",
	"close error",
	"flush error",
	"get/set settings error",
	"read error",
	"write error",
	"select error",
	"timeout error",
	"ioctl error",
	"port closed error",
};

#ifdef RS232_DEBUG
const char *
rs232_hex_dump(const void *data, unsigned int len)
{
	static char string[1024] = {0};
	unsigned char *d = (unsigned char *) data;
	unsigned int i;

	for (i = 0; len--; i += 3) {
		if (i >= sizeof(string) - 4)
			break;
		snprintf(string+i, 4, "%02x ", *d++);
	}

	string[i] = '\0';
	return string;
}

const char *
rs232_ascii_dump(const void *data, unsigned int len)
{
	static char string[1024] = {0};
	unsigned char *d = (unsigned char *) data;
	unsigned int i;
	unsigned char c;

	for (i = 0; len--; i++) {
		if (i >= sizeof(string) - 2)
			break;
		c = *d++;
		snprintf(string+i, 2, "%c", isprint(c) ? c : '.');
	}

	string[i] = '\0';
	return string;
}
#endif

RS232_LIB const char *
rs232_strerror(unsigned int error)
{
	DBG("error=%d\n", error);

	if (error >= RS232_ERR_MAX)
		return NULL;

	return rs232_error[error];
}

RS232_LIB const char *
rs232_strbaud(unsigned int baud)
{
	DBG("baud=%d\n", baud);

	if (baud >= RS232_BAUD_MAX)
		return NULL;

	return rs232_baud[baud];
}

RS232_LIB enum rs232_baud_e
rs232_baudstr(const char* baud)
{
	int i = 0;
	
	DBG("baud=%s\n", baud);
	
	for(;i <= RS232_BAUD_MAX; i++)
		if(strncmp(rs232_baud[i], baud, strlen(rs232_baud[i])) == 0)
			break;
	
	return (enum rs232_baud_e)i;
}

RS232_LIB const char *
rs232_strdata(unsigned int data)
{
	DBG("data=%d\n", data);

	if (data >= RS232_DATA_MAX)
		return NULL;

	return rs232_data[data];
}

RS232_LIB enum rs232_data_e
rs232_datastr(const char* data)
{
	int i = 0;
	
	DBG("data=%s\n", data);
	
	for(;i <= RS232_DATA_MAX; i++)
		if(strncmp(rs232_data[i], data, strlen(rs232_data[i])) == 0)
			break;
	
	return (enum rs232_data_e)i;
}

RS232_LIB const char *
rs232_strparity(unsigned int parity)
{
	DBG("parity=%d\n", parity);

	if (parity >= RS232_PARITY_MAX)
		return NULL;

	return rs232_parity[parity];
}

RS232_LIB enum rs232_parity_e
rs232_paritystr(const char* parity)
{
	int i = 0;
	
	DBG("parity=%s\n", parity);
	
	for(;i <= RS232_PARITY_MAX; i++)
		if(strncmp(rs232_parity[i], parity, strlen(rs232_parity[i])) == 0)
			break;
		
	return (enum rs232_parity_e)i;
}

RS232_LIB const char *
rs232_strstop(unsigned int stop)
{
	DBG("stop=%d\n", stop);

	if (stop >= RS232_STOP_MAX)
		return NULL;

	return rs232_stop[stop];
}

RS232_LIB enum rs232_stop_e
rs232_stopstr(const char* stop)
{
	int i = 0;
	
	DBG("stop=%s\n", stop);
	
	for(;i <= RS232_STOP_MAX; i++)
		if(strncmp(rs232_stop[i], stop, strlen(rs232_stop[i])) == 0)
			break;
	
	return (enum rs232_stop_e)i;
}

RS232_LIB const char *
rs232_strflow(unsigned int flow)
{
	DBG("flow=%d\n", flow);

	if (flow >= RS232_FLOW_MAX)
		return NULL;

	return rs232_flow[flow];
}

RS232_LIB enum rs232_flow_e
rs232_flowstr(const char* flow)
{
	int i = 0;

	DBG("flow=%s\n", flow);

	for(;i <= RS232_FLOW_MAX; i++)
		if(strncmp(rs232_flow[i], flow, strlen(rs232_flow[i])) == 0)
			break;
	
	return (enum rs232_flow_e)i;
}

RS232_LIB const char *
rs232_strdtr(unsigned int dtr)
{
	DBG("dtr=%d\n", dtr);

	if (dtr >= RS232_DTR_MAX)
		return NULL;

	return rs232_dtr[dtr];
}

RS232_LIB enum rs232_dtr_e
rs232_dtrstr(const char* dtr)
{
	int i = 0;
	
	DBG("dtr=%s\n", dtr);
	
	for(;i <= RS232_DTR_MAX; i++)
		if(strncmp(rs232_dtr[i], dtr, strlen(rs232_dtr[i])) == 0)
			break;
	
	return (enum rs232_dtr_e)i;
}

RS232_LIB const char *
rs232_strrts(unsigned int rts)
{
	DBG("rts=%d\n", rts);

	if (rts >= RS232_RTS_MAX)
		return NULL;

	return rs232_rts[rts];
}

RS232_LIB enum rs232_rts_e
rs232_rtsstr(const char* rts)
{
	int i = 0;
	
	DBG("rts=%s\n", rts);
	
	for(;i <= RS232_RTS_MAX; i++)
		if(strncmp(rs232_rts[i], rts, strlen(rs232_rts[i])) == 0)
			break;
		
	return (enum rs232_rts_e)i;
}

RS232_LIB const char *
rs232_to_string(struct rs232_port_t *p)
{
	static char str[RS232_STRLEN+1];

	DBG("p=%p\n", (void *)p);

	if (p == NULL)
		return NULL;

	snprintf(str, RS232_STRLEN, "device: %s, baud: %s, data bits: %s,"
					" parity: %s, stop bits: %s,"
					" flow control: %s",
					p->dev,
					rs232_strbaud(p->baud),
					rs232_strdata(p->data),
					rs232_strparity(p->parity),
					rs232_strstop(p->stop),
					rs232_strflow(p->flow));

	return str;
}

RS232_LIB const char *
rs232_get_device(struct rs232_port_t *p)
{
	DBG("p=%p device: %s\n", (void *)p, p->dev);
	return p->dev;
}

RS232_LIB unsigned int
rs232_get_baud(struct rs232_port_t *p)
{
	DBG("p=%p baud: %d\n", (void *)p, p->baud);
	return p->baud;
}

RS232_LIB unsigned int
rs232_get_stop(struct rs232_port_t *p)
{
	DBG("p=%p baud: %d\n", (void *)p, p->stop);
	return p->stop;
}

RS232_LIB unsigned int
rs232_get_data(struct rs232_port_t *p)
{
	DBG("p=%p data: %d\n", (void *)p, p->data);
	return p->data;

}

RS232_LIB unsigned int
rs232_get_parity(struct rs232_port_t *p)
{
	DBG("p=%p parity: %d\n", (void *)p, p->parity);
	return p->parity;
}

RS232_LIB unsigned int
rs232_get_flow(struct rs232_port_t *p)
{
	DBG("p=%p flow: %d\n", (void *)p, p->flow);
	return p->flow;
}

RS232_LIB unsigned int
rs232_port_open(struct rs232_port_t *p)
{
	DBG("p=%p p->status=%d\n", (void *)p, p->status);
	return p->status;
}

RS232_LIB unsigned int
rs232_get_dtr(struct rs232_port_t *p)
{
	DBG("p=%p dtr: %d\n", (void *)p, p->dtr);
	return p->dtr;
}

RS232_LIB unsigned int
rs232_get_rts(struct rs232_port_t *p)
{
	DBG("p=%p rts: %d\n", (void *)p, p->rts);
	return p->rts;
}
