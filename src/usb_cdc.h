#ifndef __USB_CDC_H__
#define __USB_CDC_H__

int usbcdc_init(void);
void usbcdc_put(unsigned int uiData);
void usbcdc_flush(void);
unsigned char usbcdc_get(void);
unsigned int usbcdc_peek(void);
void usbcdc_close(void);


#endif  /* __USB_CDC_H__ */
