/** @file
  This file defines the data structures per HOB specification v0.9.

  Copyright (c) 2007 - 2009, Intel Corporation
  All rights reserved. This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  @par Revision Reference:
  These definitions are from HOB Spec 0.9 but not adopted by PI specs.

**/

#ifndef _HOB_H_
#define _HOB_H_

///
/// Capsule volume HOB -- identical to a firmware volume
/// This macro is defined  to comply with hob Framework Spec. And the marco has been
/// retired in PI1.0 spec.  
///
#define EFI_HOB_TYPE_CV           0x0008

typedef struct {
  EFI_HOB_GENERIC_HEADER            Header;
  EFI_PHYSICAL_ADDRESS              BaseAddress;
  UINT64                            Length;
} EFI_HOB_CAPSULE_VOLUME;

#endif
