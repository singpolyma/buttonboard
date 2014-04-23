avrdude -p m16u2 -P usb -c avrispmkii -U flash:w:$1 -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
