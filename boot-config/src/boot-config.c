
/*
* 
*/

#include <stdio.h>  
#include <string.h>
#include <strings.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/socket.h>
#include <sys/wait.h> 
#include <fcntl.h>  
#include <termios.h>  
#include <signal.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <ctype.h>

#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>
#include <linux/version.h>
#include <linux/types.h>

/*  ================================================================  */
#include "include/boot-config.h"
/*  ================================================================  */

/*  ================================================================  */
#if CONFIG_overlays	/* dt_overlays. */
#if CONFIG_overlay_1wire || CONFIG_overlay_pps || \
			CONFIG_overlay_gpio_keys_user || \
				CONFIG_overlay_rotary_encoder || CONFIG_overlay_i2c4
	#include "include/overlays_modules.h"

	#if CONFIG_overlay_1wire || CONFIG_overlay_pps
		#include "include/overlays_set_gpio.h"		/* edit overlays gpio pps and w1-gpio */
	#endif
#endif
#endif			/* End dt_overlays. */

#include "include/test_dts.h"

#include "include/okay_disabled.h"

#include "include/read_boardname.h"

#include "include/err_help.h"

#if CONFIG_overlays	/* dt_overlays. */
#include "include/dt_overlays.h"
#endif			/* End dt_overlays. */
/*  ================================================================  */

/*  ================================================================  */
int main (int argc, char *argv [])
{
	if ((argc == 2) && (strcasecmp (argv [1+ofset], "dtb-dts-mmc") == 0)) 
			test_dts (DTB_TO_DTS_MMC,CONV);
	else if ((argc == 2) && (strcasecmp (argv [1+ofset], "dts-dtb-mmc") == 0)) 
			test_dts (DTS_TO_DTB_MMC,CONV);
/*
	else if ((argc == 2) && (strcasecmp (argv [1+ofset], "dtb-dts-spi") == 0))
			test_dts (DTB_TO_DTS_SPI,CONV);
	else if ((argc == 2) && (strcasecmp (argv [1+ofset], "dts-dtb-spi") == 0))
			test_dts (DTS_TO_DTB_SPI,CONV);
*/
	else test_dts (TEST_DTS_MMC,CONV);

	if ((argc == 3) && (strcasecmp (argv [1+ofset], "overlays") == 0))
			dt_overlays (argc,argv);

	read_boardname ();

	char *on_off;
	if (argc != 3)				/* Not '3' input arguments. */
	{
		if (argc == 1) err_help (ERROR_ON_OFF,argv);
		else if ((strcasecmp (argv [1+ofset], "-h") == 0) || (argc == 2)) mini_help (HELP,argv);
		else if ((strcasecmp (argv [1+ofset], "-g") == 0) && (argc == 4)) ofset=1;
		else err_help (ERROR_ON_OFF,argv);
	} else if (strcasecmp (argv [1+ofset], "-g") == 0) err_help (NUM_GPIO,argv);	/* '3' input arguments. */

#if CONFIG_overlays	/* dt_overlays. */
#if CONFIG_overlay_1wire			/* edit overlays w1-gpio gpio */
	if ((strcasecmp (argv [1+ofset], "w1pin") == 0) || (strcasecmp (argv [1+ofset], "w1") == 0)) set_gpio (w1,argc,argv);
#endif
#if CONFIG_overlay_pps				/* edit overlays pps gpio */
	if ((strcasecmp (argv [1+ofset], "ppspin") == 0) || (strcasecmp (argv [1+ofset], "pps") == 0)) set_gpio (pps,argc,argv);
#endif
#endif			/* End dt_overlays. */

	if ((argc >= 4) || (strcasecmp (argv [1+ofset], "-g") == 0))	/* '4' or more input arguments. */
			err_help (MODULE,argv);

		/* Test argv [2+ofset]: okay or disabled */
	if ((strcasecmp (argv [2+ofset], "disabled") == 0) || (strcasecmp (argv [2+ofset], "off") == 0) || \
					(strcasecmp (argv [2+ofset], "OFF") == 0))
			on_off="okay/disabled";
	else if ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
					(strcasecmp (argv [2+ofset], "ON") == 0))
			on_off="disabled/okay";
	else err_help (ERROR_ON_OFF,argv);
		/* End test argv [2+ofset]: okay or disabled */

		/* USB */
	if (strcasecmp (argv [1+ofset], "usb0") == 0) { okay_disabled (ehci0, on_off); okay_disabled (ohci0, on_off); }
	else if (strcasecmp (argv [1+ofset], "usb1") == 0) { okay_disabled (ehci1, on_off); okay_disabled (ohci1, on_off); }
	else if (strcasecmp (argv [1+ofset], "usb2") == 0) { okay_disabled (ehci2, on_off); okay_disabled (ohci2, on_off); }
	else if (strcasecmp (argv [1+ofset], "usb3") == 0) { okay_disabled (ehci3, on_off); okay_disabled (ohci3, on_off); }
	else if (strcasecmp (argv [1+ofset], "ehci0") == 0) okay_disabled (ehci0, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci0") == 0) okay_disabled (ohci0, on_off);
	else if (strcasecmp (argv [1+ofset], "ehci1") == 0) okay_disabled (ehci1, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci1") == 0) okay_disabled (ohci1, on_off);
	else if (strcasecmp (argv [1+ofset], "ehci2") == 0) okay_disabled (ehci2, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci2") == 0) okay_disabled (ohci2, on_off);
	else if (strcasecmp (argv [1+ofset], "ehci3") == 0) okay_disabled (ehci3, on_off);
	else if (strcasecmp (argv [1+ofset], "ohci3") == 0) okay_disabled (ohci3, on_off);
		/* End USB */

		/* peripheral, host, otg */
	else if ((strcasecmp (argv [1+ofset], "usb_otg") == 0) || (strcasecmp (argv [1+ofset], "otg") == 0) || \
		(strcasecmp (argv [1+ofset], "usb_host") == 0) || (strcasecmp (argv [1+ofset], "host") == 0) || \
		(strcasecmp (argv [1+ofset], "usb_peripheral") == 0) || (strcasecmp (argv [1+ofset], "peripheral") == 0))
	{
			okay_disabled (usb_otg, on_off);		/* on - off */
		if ((strcasecmp (argv [1+ofset], "usb_otg") == 0) || (strcasecmp (argv [1+ofset], "otg") == 0))
			okay_disabled (usb_otg, otg_tipe);		/* otg */
		else if ((strcasecmp (argv [1+ofset], "usb_host") == 0) || (strcasecmp (argv [1+ofset], "host") == 0))
			okay_disabled (usb_otg, host_tipe);		/* host */
		else if ((strcasecmp (argv [1+ofset], "usb_peripheral") == 0) || (strcasecmp (argv [1+ofset], "peripheral") == 0))
			okay_disabled (usb_otg, peripheral_tipe);	/* peripheral */
	}	/* End peripheral, host, otg */

	else if (strcasecmp (argv [1+ofset], "ir") == 0) okay_disabled (ir, on_off);
	else if (strcasecmp (argv [1+ofset], "i2c0") == 0) okay_disabled (i2c0, on_off);
	else if (strcasecmp (argv [1+ofset], "i2c1") == 0) okay_disabled (i2c1, on_off);
	else if (strcasecmp (argv [1+ofset], "i2c2") == 0) okay_disabled (i2c2, on_off);
	else if ((strcasecmp (argv [1+ofset], "audcodec") == 0) || (strcasecmp (argv [1+ofset], "codec") == 0) || \
							(strcasecmp (argv [1+ofset], "audio") == 0))
			okay_disabled (codec, on_off);
		/* Uart0 */
	else if (strcasecmp (argv [1+ofset], "uart0") == 0) {
		okay_disabled (uart0, on_off);
		if ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
							(strcasecmp (argv [2+ofset], "ON") == 0))
		{	/* Disable "pwm" only if "uart0" is enabled. */
			on_off="okay/disabled";
			okay_disabled (pwm, on_off);
		}
	}	/* End uart0 */

		/* pwm */
	else if (strcasecmp (argv [1+ofset], "pwm") == 0) {
		okay_disabled (pwm, on_off);
		if ((strcasecmp (argv [2+ofset], "okay") == 0) || (strcasecmp (argv [2+ofset], "on") == 0) || \
							(strcasecmp (argv [2+ofset], "ON") == 0))
		{	/* Disable "uart0" only if "pwm" is enabled. */
			on_off="okay/disabled";
			okay_disabled (uart0, on_off);
		}
	}	/* End pwm */

	else if (strcasecmp (argv [1+ofset], "uart1") == 0) okay_disabled (uart1, on_off);
	else if (strcasecmp (argv [1+ofset], "uart2") == 0) okay_disabled (uart2, on_off);
	else if (strcasecmp (argv [1+ofset], "spi0") == 0) okay_disabled (spi0, on_off);
	else if (strcasecmp (argv [1+ofset], "spi1") == 0) okay_disabled (spi1, on_off);
	else if (strcasecmp (argv [1+ofset], "ethernet0") == 0) okay_disabled (ethernet0, on_off);
	else if (strcasecmp (argv [1+ofset], "i2s0") == 0) okay_disabled (i2s0, on_off);
	else if (strcasecmp (argv [1+ofset], "i2s1") == 0) okay_disabled (i2s1, on_off);
	else if (strcasecmp (argv [1+ofset], "camera") == 0) okay_disabled (camera, on_off);

	else if (strcasecmp (argv [1+ofset], "spdif") == 0) okay_disabled (spdif, on_off);

		/* MMC0 - MMC1 - MMC2 */
	else if (strcasecmp (argv [1+ofset], "mmc0") == 0) okay_disabled (mmc0, on_off);	/* End mmc0 - cd-flash */
	else if ((strcasecmp (argv [1+ofset], "wifi") == 0) || (strcasecmp (argv [1+ofset], "mmc1") == 0))
			okay_disabled (mmc1, on_off);						/* End mmc1 */
	else if (strcasecmp (argv [1+ofset], "mmc2") == 0) okay_disabled (mmc2, on_off);	/* End mmc2 */

#if CONFIG_overlays	/* dt_overlays. */

		/* LED ON/OFF */
	else if ((strcasecmp (argv [1+ofset], "pwr_led") == 0) || \
			(strcasecmp (argv [1+ofset], "green-led") == 0)) okay_disabled (pwr_led, on_off);
	else if ((strcasecmp (argv [1+ofset], "status_led") == 0) || \
			(strcasecmp (argv [1+ofset], "red-led") == 0)) okay_disabled (status_led, on_off);
		/* END LED ON/OFF */

#if CONFIG_overlay_1wire
	else if ((strcasecmp (argv [1+ofset], "w1") == 0) || (strcasecmp (argv [1+ofset], "w1pin") == 0)) okay_disabled (w1, on_off);
#endif
#if CONFIG_overlay_pps
	else if ((strcasecmp (argv [1+ofset], "pps") == 0) || (strcasecmp (argv [1+ofset], "ppspin") == 0)) okay_disabled (pps, on_off);
#endif
#if CONFIG_overlay_gpio_keys_user
	else if ((strcasecmp (argv [1+ofset], "gpio-keys-user") == 0) || (strcasecmp (argv [1+ofset], "gpio-keys") == 0) || \
							(strcasecmp (argv [1+ofset], "keys-user") == 0))
			okay_disabled (gpio_keys_user, on_off);
#endif
#if CONFIG_overlay_rotary_encoder
	else if ((strcasecmp (argv [1+ofset], "rotary") == 0) || (strcasecmp (argv [1+ofset], "rotary_button") == 0) || \
							(strcasecmp (argv [1+ofset], "rotary_axis") == 0))
	{
		if ((strcasecmp (argv [1+ofset], "rotary") == 0) || (strcasecmp (argv [1+ofset], "rotary_button") == 0))
			okay_disabled (rotary_button, on_off);
		if ((strcasecmp (argv [1+ofset], "rotary") == 0) || (strcasecmp (argv [1+ofset], "rotary_axis") == 0))
			okay_disabled (rotary_axis, on_off);
	}
#endif
#if CONFIG_overlay_i2c4
	else if (strcasecmp (argv [1+ofset], "i2c4") == 0) okay_disabled (i2c4, on_off);
#endif
#endif			/* End dt_overlays. */

	else err_help (MODULE,argv);
};
