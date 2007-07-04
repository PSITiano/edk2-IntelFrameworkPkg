/** @file
  Guid used to define the Firmware File System.  See the Framework Firmware
  File System Specification for more details.

  Copyright (c) 2006, Intel Corporation
  All rights reserved. This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  Module Name:  FirmwareFileSystem.h

  @par Revision Reference:
  Guids defined in Firmware File System Spec 0.9

**/

#ifndef __FIRMWARE_FILE_SYSTEM_GUID_H__
#define __FIRMWARE_FILE_SYSTEM_GUID_H__

#include <PiPei.h>

//
// GUIDs defined by the FFS specification.
//
#define EFI_FIRMWARE_FILE_SYSTEM_GUID \
  { 0x7A9354D9, 0x0468, 0x444a, {0x81, 0xCE, 0x0B, 0xF6, 0x17, 0xD8, 0x90, 0xDF }}

extern EFI_GUID gEfiFirmwareFileSystemGuid;

#endif
