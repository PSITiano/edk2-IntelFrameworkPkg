/** @file
  Provides the parent dispatch service for a given SMI source generator.

  Copyright (c) 2007 - 2009, Intel Corporation
  All rights reserved. This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  Module Name:  SmmSwDispatch.h

  @par Revision Reference:
  This Protocol is defined in Framework for EFI SMM Core Interface Spec
  Version 0.9.

**/

#ifndef _EFI_SMM_SW_DISPATCH_H_
#define _EFI_SMM_SW_DISPATCH_H_

#include <PiDxe.h>

//
// Global ID for the SW SMI Protocol
//
#define EFI_SMM_SW_DISPATCH_PROTOCOL_GUID \
  { \
    0xe541b773, 0xdd11, 0x420c, {0xb0, 0x26, 0xdf, 0x99, 0x36, 0x53, 0xf8, 0xbf } \
  }

typedef struct _EFI_SMM_SW_DISPATCH_PROTOCOL  EFI_SMM_SW_DISPATCH_PROTOCOL;

//
// Related Definitions
//
//
// A particular chipset may not support all possible software SMI input values.
// For example, the ICH supports only values 00h to 0FFh.  The parent only allows a single
// child registration for each SwSmiInputValue.
//
typedef struct {
  UINTN SwSmiInputValue;
} EFI_SMM_SW_DISPATCH_CONTEXT;

//
// Member functions
//
/**
  Dispatch function for a Software SMI handler.

  @param  DispatchHandle        Handle of this dispatch function.
  @param  DispatchContext       Pointer to the dispatch function's context.
                                The SwSmiInputValue field is filled in
                                by the software dispatch driver prior to
                                invoking this dispatch function.
                                The dispatch function will only be called
                                for input values for which it is registered.

  @return None

**/
typedef
VOID
(EFIAPI *EFI_SMM_SW_DISPATCH)(
  IN  EFI_HANDLE                    DispatchHandle,
  IN  EFI_SMM_SW_DISPATCH_CONTEXT   *DispatchContext
  );

/**
  Register a child SMI source dispatch function with a parent SMM driver

  @param  This                  Pointer to the EFI_SMM_SW_DISPATCH_PROTOCOL instance.
  @param  DispatchFunction      Function to install.
  @param  DispatchContext       Pointer to the dispatch function's context.
                                The caller fills this context in before calling
                                the register function to indicate to the register
                                function which Software SMI input value the
                                dispatch function should be invoked for.
  @param  DispatchHandle        Handle generated by the dispatcher to track the function instance.

  @retval EFI_SUCCESS           The dispatch function has been successfully
                                registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR      The SW driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES  Not enough memory (system or SMM) to manage this
                                child.
  @retval EFI_INVALID_PARAMETER DispatchContext is invalid. The SW SMI input value
                                is not within valid range.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SMM_SW_REGISTER)(
  IN EFI_SMM_SW_DISPATCH_PROTOCOL          *This,
  IN EFI_SMM_SW_DISPATCH                   DispatchFunction,
  IN EFI_SMM_SW_DISPATCH_CONTEXT           *DispatchContext,
  OUT EFI_HANDLE                           *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param  This                  Pointer to the EFI_SMM_SW_DISPATCH_PROTOCOL instance.
  @param  DispatchHandle        Handle of the service to remove.

  @retval EFI_SUCCESS           The dispatch function has been successfully
                                unregistered and the SMI source has been disabled
                                if there are no other registered child dispatch
                                functions for this SMI source.
  @retval EFI_INVALID_PARAMETER Handle is invalid.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SMM_SW_UNREGISTER)(
  IN EFI_SMM_SW_DISPATCH_PROTOCOL          *This,
  IN EFI_HANDLE                            DispatchHandle
  );


//
// Interface structure for the SMM Software SMI Dispatch Protocol
//
/**
  Provides the parent dispatch service for a given SMI source generator.
**/
///
/// Inconsistent with specification here:
/// In framework spec SmmCis, this definition is named as _EFI_SMM_ICHN_DISPATCH_PROTOCOL by mistake.
///
struct _EFI_SMM_SW_DISPATCH_PROTOCOL {
  ///
  /// Installs a child service to be dispatched by this protocol.
  ///
  EFI_SMM_SW_REGISTER   Register;

  ///
  /// Removes a child service dispatched by this protocol.
  ///
  EFI_SMM_SW_UNREGISTER UnRegister;

  ///
  /// A read-only field that describes the maximum value that can be used
  /// in the EFI_SMM_SW_DISPATCH_PROTOCOL.Register() service.
  ///
  UINTN                 MaximumSwiValue;
};

extern EFI_GUID gEfiSmmSwDispatchProtocolGuid;

#endif
