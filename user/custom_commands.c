//Copyright 2015 <>< Charles Lohr, see LICENSE file.

#include <esp82xxutil.h>
#include <commonservices.h>
#include "ws2812_i2s.h"
#include "vars.h"

int ICACHE_FLASH_ATTR CustomCommand(char * buffer, int retsize, char *pusrdata, unsigned short len)
{
	char * buffend = buffer;

	switch( pusrdata[1] )	{
    	case 'C': case 'c': { //Custom command test
    		buffend += ets_sprintf( buffend, "CC" );
    		return buffend-buffer;
    	} break;

    	case 'l': case 'L': { //LEDs
    		int i, it = 0;
    		buffend += ets_sprintf( buffend, "CL:%d:", UsrCfg->nled );
    		uint16_t toledsvals = UsrCfg->nled*3;
    		if( toledsvals > 600 ) toledsvals = 600;
    		for( i = 0; i < toledsvals; i++ )
    		{
    			uint8_t samp = last_leds[it++];
    			*(buffend++) = tohex1( samp>>4 );
    			*(buffend++) = tohex1( samp&0x0f );
    		}
    		return buffend-buffer;
    	} break;

        case 'T': case 't': { // set led-ranges over Web-UI
            #ifdef DEBUG
                printf("Color Data: ");
                int it;
                for(it=3; it<len; ++it)
                    printf("%x ", pusrdata[it]);
                printf("\n");
            #endif
            ws2812_push(pusrdata+3,len-3);
            ets_memcpy( last_leds, pusrdata+3, len );
            UsrCfg->nled = (len-1) / 3;
            frame = 0;
        } break;


	}
	return -1;
}
