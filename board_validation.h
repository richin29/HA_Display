
/**
 * @file board_validation.h
 * @author Richin Abraham
 * @brief Contains pre-processor error validation for board configuration.
 */

#ifndef BOARD_VALIDATION_H
#define BOARD_VALIDATION_H

#if ARDUINO_USB_CDC_ON_BOOT != 1
#warning "If you need to monitor printed data, be sure to set USB CDC On boot to ENABLE, otherwise you will not see any data in the serial monitor"
#endif

#ifndef BOARD_HAS_PSRAM
#error "Detected that PSRAM is not turned on. Please set PSRAM to OPI PSRAM in ArduinoIDE"
#endif

#endif // BOARD_VALIDATION_H
