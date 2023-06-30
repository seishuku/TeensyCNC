/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_DEVICE_DESCRIPTOR_H_
#define _USB_DEVICE_DESCRIPTOR_H_ 1

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USB_DEVICE_SPECIFIC_BCD_VERSION (0x0200U)
#define USB_DEVICE_DEMO_BCD_VERSION     (0x0101U)

#define USB_DEVICE_VID (0x1FC9U)
#define USB_DEVICE_PID (0x0094U)

/* Communication  Class Codes */
#define CDC_COMM_CLASS (0x02U)
/* Data  Class Codes */
#define CDC_DATA_CLASS (0x0AU)

/* Communication Class SubClass Codes */
#define USB_CDC_DIRECT_LINE_CONTROL_MODEL         (0x01U)
#define USB_CDC_ABSTRACT_CONTROL_MODEL            (0x02U)
#define USB_CDC_TELEPHONE_CONTROL_MODEL           (0x03U)
#define USB_CDC_MULTI_CHANNEL_CONTROL_MODEL       (0x04U)
#define USB_CDC_CAPI_CONTROL_MOPDEL               (0x05U)
#define USB_CDC_ETHERNET_NETWORKING_CONTROL_MODEL (0x06U)
#define USB_CDC_ATM_NETWORKING_CONTROL_MODEL      (0x07U)
#define USB_CDC_WIRELESS_HANDSET_CONTROL_MODEL    (0x08U)
#define USB_CDC_DEVICE_MANAGEMENT                 (0x09U)
#define USB_CDC_MOBILE_DIRECT_LINE_MODEL          (0x0AU)
#define USB_CDC_OBEX                              (0x0BU)
#define USB_CDC_ETHERNET_EMULATION_MODEL          (0x0CU)

/* Communication Class Protocol Codes */
#define USB_CDC_NO_CLASS_SPECIFIC_PROTOCOL  (0x00U) /*also for Data Class Protocol Code */
#define USB_CDC_AT_250_PROTOCOL             (0x01U)
#define USB_CDC_AT_PCCA_101_PROTOCOL        (0x02U)
#define USB_CDC_AT_PCCA_101_ANNEX_O         (0x03U)
#define USB_CDC_AT_GSM_7_07                 (0x04U)
#define USB_CDC_AT_3GPP_27_007              (0x05U)
#define USB_CDC_AT_TIA_CDMA                 (0x06U)
#define USB_CDC_ETHERNET_EMULATION_PROTOCOL (0x07U)
#define USB_CDC_EXTERNAL_PROTOCOL           (0xFEU)
#define USB_CDC_VENDOR_SPECIFIC             (0xFFU) /*also for Data Class Protocol Code */

/* Data Class Protocol Codes */
#define USB_CDC_PYHSICAL_INTERFACE_PROTOCOL (0x30U)
#define USB_CDC_HDLC_PROTOCOL               (0x31U)
#define USB_CDC_TRANSPARENT_PROTOCOL        (0x32U)
#define USB_CDC_MANAGEMENT_PROTOCOL         (0x50U)
#define USB_CDC_DATA_LINK_Q931_PROTOCOL     (0x51U)
#define USB_CDC_DATA_LINK_Q921_PROTOCOL     (0x52U)
#define USB_CDC_DATA_COMPRESSION_V42BIS     (0x90U)
#define USB_CDC_EURO_ISDN_PROTOCOL          (0x91U)
#define USB_CDC_RATE_ADAPTION_ISDN_V24      (0x92U)
#define USB_CDC_CAPI_COMMANDS               (0x93U)
#define USB_CDC_HOST_BASED_DRIVER           (0xFDU)
#define USB_CDC_UNIT_FUNCTIONAL             (0xFEU)

/* Descriptor SubType in Communications Class Functional Descriptors */
#define USB_CDC_HEADER_FUNC_DESC              (0x00U)
#define USB_CDC_CALL_MANAGEMENT_FUNC_DESC     (0x01U)
#define USB_CDC_ABSTRACT_CONTROL_FUNC_DESC    (0x02U)
#define USB_CDC_DIRECT_LINE_FUNC_DESC         (0x03U)
#define USB_CDC_TELEPHONE_RINGER_FUNC_DESC    (0x04U)
#define USB_CDC_TELEPHONE_REPORT_FUNC_DESC    (0x05U)
#define USB_CDC_UNION_FUNC_DESC               (0x06U)
#define USB_CDC_COUNTRY_SELECT_FUNC_DESC      (0x07U)
#define USB_CDC_TELEPHONE_MODES_FUNC_DESC     (0x08U)
#define USB_CDC_TERMINAL_FUNC_DESC            (0x09U)
#define USB_CDC_NETWORK_CHANNEL_FUNC_DESC     (0x0AU)
#define USB_CDC_PROTOCOL_UNIT_FUNC_DESC       (0x0BU)
#define USB_CDC_EXTENSION_UNIT_FUNC_DESC      (0x0CU)
#define USB_CDC_MULTI_CHANNEL_FUNC_DESC       (0x0DU)
#define USB_CDC_CAPI_CONTROL_FUNC_DESC        (0x0EU)
#define USB_CDC_ETHERNET_NETWORKING_FUNC_DESC (0x0FU)
#define USB_CDC_ATM_NETWORKING_FUNC_DESC      (0x10U)
#define USB_CDC_WIRELESS_CONTROL_FUNC_DESC    (0x11U)
#define USB_CDC_MOBILE_DIRECT_LINE_FUNC_DESC  (0x12U)
#define USB_CDC_MDLM_DETAIL_FUNC_DESC         (0x13U)
#define USB_CDC_DEVICE_MANAGEMENT_FUNC_DESC   (0x14U)
#define USB_CDC_OBEX_FUNC_DESC                (0x15U)
#define USB_CDC_COMMAND_SET_FUNC_DESC         (0x16U)
#define USB_CDC_COMMAND_SET_DETAIL_FUNC_DESC  (0x17U)
#define USB_CDC_TELEPHONE_CONTROL_FUNC_DESC   (0x18U)
#define USB_CDC_OBEX_SERVICE_ID_FUNC_DESC     (0x19U)

/* usb descriptor length */
#define USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL (sizeof(g_UsbDeviceConfigurationDescriptor))
#define USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC   (5U)
#define USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG    (5U)
#define USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT      (4U)
#define USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC    (5U)

/* Configuration, interface and endpoint. */
#define USB_DEVICE_CONFIGURATION_COUNT (1U)
#define USB_DEVICE_STRING_COUNT        (3U)
#define USB_DEVICE_LANGUAGE_COUNT      (1U)

#define USB_CDC_VCOM_CONFIGURE_INDEX (1U)

#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#define USB_CDC_VCOM_ENDPOINT_CIC_COUNT (0U)
#else
#define USB_CDC_VCOM_ENDPOINT_CIC_COUNT (1U)
#endif
#define USB_CDC_VCOM_ENDPOINT_DIC_COUNT             (2U)
#define USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT          (1U)
#define USB_CDC_VCOM_BULK_IN_ENDPOINT               (2U)
#define USB_CDC_VCOM_BULK_OUT_ENDPOINT              (3U)
#define USB_CDC_VCOM_INTERFACE_COUNT                (2U)
#define USB_CDC_VCOM_COMM_INTERFACE_INDEX           (0U)
#define USB_CDC_VCOM_DATA_INTERFACE_INDEX           (1U)
#define USB_CDC_VCOM_COMM_INTERFACE_ALTERNATE_COUNT (1U)
#define USB_CDC_VCOM_DATA_INTERFACE_ALTERNATE_COUNT (1U)
#define USB_CDC_VCOM_COMM_INTERFACE_ALTERNATE_0     (0U)
#define USB_CDC_VCOM_DATA_INTERFACE_ALTERNATE_0     (0U)

/* Packet size. */
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
#define HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE (16U)
#define FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE (16U)
#define HS_CDC_VCOM_INTERRUPT_IN_INTERVAL    (0x07U) /* 2^(7-1) = 8ms */
#define FS_CDC_VCOM_INTERRUPT_IN_INTERVAL    (0x08U)
#endif
#define HS_CDC_VCOM_BULK_IN_PACKET_SIZE  (512U)
#define FS_CDC_VCOM_BULK_IN_PACKET_SIZE  (64U)
#define HS_CDC_VCOM_BULK_OUT_PACKET_SIZE (512U)
#define FS_CDC_VCOM_BULK_OUT_PACKET_SIZE (64U)

/* String descriptor length. */
#define USB_DESCRIPTOR_LENGTH_STRING0 (sizeof(g_UsbDeviceString0))
#define USB_DESCRIPTOR_LENGTH_STRING1 (sizeof(g_UsbDeviceString1))
#define USB_DESCRIPTOR_LENGTH_STRING2 (sizeof(g_UsbDeviceString2))

#define USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE (0x24U)
#define USB_DESCRIPTOR_TYPE_CDC_CS_ENDPOINT  (0x25U)

/* Class code. */
#define USB_DEVICE_CLASS    (0x02U)
#define USB_DEVICE_SUBCLASS (0x00U)
#define USB_DEVICE_PROTOCOL (0x00U)

#define USB_DEVICE_MAX_POWER (0x32U)

#define USB_CDC_VCOM_CIC_CLASS    (CDC_COMM_CLASS)
#define USB_CDC_VCOM_CIC_SUBCLASS (USB_CDC_ABSTRACT_CONTROL_MODEL)
#define USB_CDC_VCOM_CIC_PROTOCOL (USB_CDC_NO_CLASS_SPECIFIC_PROTOCOL)

#define USB_CDC_VCOM_DIC_CLASS    (CDC_DATA_CLASS)
#define USB_CDC_VCOM_DIC_SUBCLASS (0x00)
#define USB_CDC_VCOM_DIC_PROTOCOL (USB_CDC_NO_CLASS_SPECIFIC_PROTOCOL)

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief USB device set speed function.
 *
 * This function sets the speed of the USB device.
 *
 * Due to the difference of HS and FS descriptors, the device descriptors and configurations need to be updated to match
 * current speed.
 * As the default, the device descriptors and configurations are configured by using FS parameters for both EHCI and
 * KHCI.
 * When the EHCI is enabled, the application needs to call this function to update device by using current speed.
 * The updated information includes endpoint max packet size, endpoint interval, etc.
 *
 * @param handle The USB device handle.
 * @param speed Speed type. USB_SPEED_HIGH/USB_SPEED_FULL/USB_SPEED_LOW.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceSetSpeed(usb_device_handle handle, uint8_t speed);
/*!
 * @brief USB device get device descriptor function.
 *
 * This function gets the device descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param deviceDescriptor The pointer to the device descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceGetDeviceDescriptor(usb_device_handle handle,
                                                  usb_device_get_device_descriptor_struct_t *deviceDescriptor);
/*!
 * @brief USB device get string descriptor function.
 *
 * This function gets the string descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param stringDescriptor Pointer to the string descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetStringDescriptor(usb_device_handle handle,
                                           usb_device_get_string_descriptor_struct_t *stringDescriptor);
/*!
 * @brief USB device get configuration descriptor function.
 *
 * This function gets the configuration descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param configurationDescriptor The pointer to the configuration descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
extern usb_status_t USB_DeviceGetConfigurationDescriptor(
    usb_device_handle handle, usb_device_get_configuration_descriptor_struct_t *configurationDescriptor);
#endif /* _USB_DEVICE_DESCRIPTOR_H_ */
