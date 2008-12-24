/** @file
  Variable/Map manipulations routines
  
Copyright (c) 2006, Intel Corporation                                                         
All rights reserved. This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#include "IfrSupportLibInternal.h"

/**
  Extracts a variable form a Pack.

  @param  Pack              List of variables
  @param  Name              Name of the variable/map
  @param  Guid              GUID of the variable/map
  @param  Id                The index of the variable/map to retrieve
  @param  Var               Pointer to the variable/map
  @param  Size              Size of the variable/map in bytes
**/
VOID
EfiLibHiiVariablePackGetMap (
  IN      EFI_HII_VARIABLE_PACK     *Pack,  
     OUT  CHAR16                    **Name, OPTIONAL
     OUT  EFI_GUID                  **Guid, OPTIONAL
     OUT  UINT16                    *Id,    OPTIONAL
     OUT  VOID                      **Var,  OPTIONAL
     OUT  UINTN                     *Size   OPTIONAL
  )
{
  if (NULL != Name) {
    *Name = (VOID *) (Pack + 1);
  }
    
  if (NULL != Guid) { 
    *Guid = (EFI_GUID *)(UINTN)&Pack->VariableGuid;
  }
    
    
  if (NULL != Id) {
    *Id   = Pack->VariableId;
  }
    
  if (NULL != Var) {
    *Var  = (VOID *) ((CHAR8 *) (Pack + 1) + Pack->VariableNameLength);
  }
    
  if (NULL != Size) {
    *Size = Pack->Header.Length - sizeof (*Pack) - Pack->VariableNameLength;
  }
}

/**
  Finds a count of the variables/maps in the List.

  @param  List              List of variables

  @return                   The number of map count.
**/
UINTN
EfiLibHiiVariablePackListGetMapCnt (
  IN  EFI_HII_VARIABLE_PACK_LIST    *List
  )
{
  UINTN                             Cnt;
  
  Cnt = 0;
  while (NULL != List) {
    Cnt++;
    List = List->NextVariablePack;
  }
  return Cnt;
}

/**
  Will iterate all variable/maps as appearing 
  in List and for each, it will call the Callback.

  @param  List              List of variables
  @param  Callback          Routine to be called for each iterated variable.
**/
VOID
EfiLibHiiVariablePackListForEachVar (
  IN  EFI_HII_VARIABLE_PACK_LIST               *List,
  IN  EFI_LIB_HII_VARIABLE_PACK_LIST_CALLBACK  *Callback
  )
{
  CHAR16                            *MapName;
  EFI_GUID                          *MapGuid;
  UINT16                            MapId;
  VOID                              *Map;
  UINTN                             MapSize;

  while (NULL != List) {
    EfiLibHiiVariablePackGetMap (List->VariablePack, &MapName, &MapGuid, &MapId, &Map, &MapSize);
    //
    // call the callback
    //
    Callback (MapName, MapGuid, MapId, Map, MapSize); 
    List = List->NextVariablePack;
  }
}

/**
  Finds a variable form List given 
  the order number as appears in the List.

  @param  Idx            The index of the variable/map to retrieve
  @param  List           List of variables
  @param  Name           Name of the variable/map
  @param  Guid           GUID of the variable/map
  @param  Id             Id of the variable/map
  @param  Var            Pointer to the variable/map
  @param  Size           Size of the variable/map in bytes

  @return EFI_SUCCESS    Variable is found, OUT parameters are valid
  @return EFI_NOT_FOUND  Variable is not found, OUT parameters are not valid
**/
EFI_STATUS
EfiLibHiiVariablePackListGetMapByIdx (
  IN      UINTN                      Idx,  
  IN      EFI_HII_VARIABLE_PACK_LIST *List,  
     OUT  CHAR16                     **Name, OPTIONAL
     OUT  EFI_GUID                   **Guid, OPTIONAL
     OUT  UINT16                     *Id,    OPTIONAL
     OUT  VOID                       **Var,
     OUT  UINTN                      *Size
  )
{
  CHAR16                            *MapName;
  EFI_GUID                          *MapGuid;
  UINT16                            MapId;
  VOID                              *Map;
  UINTN                             MapSize;

  while (NULL != List) {
    EfiLibHiiVariablePackGetMap (List->VariablePack, &MapName, &MapGuid, &MapId, &Map, &MapSize);
    if (0 == Idx--) {
      *Var  = Map;
      *Size = MapSize;

      if (NULL != Name) {
        *Name = MapName;
      }

      if (NULL != Guid) {
        *Guid = MapGuid;
      }
        
      if (NULL != Id) {
        *Id   = MapId;
      }
        
      return EFI_SUCCESS; // Map found
    }
    List = List->NextVariablePack;
  }
  //
  // If here, the map is not found
  //
  return EFI_NOT_FOUND; 
}

/**
  Finds a variable form List given the 
  order number as appears in the List.

  @param  Id             The ID of the variable/map to retrieve
  @param  List           List of variables
  @param  Name           Name of the variable/map
  @param  Guid           GUID of the variable/map
  @param  Var            Pointer to the variable/map
  @param  Size           Size of the variable/map in bytes

  @retval EFI_SUCCESS    Variable is found, OUT parameters are valid
  @retval EFI_NOT_FOUND  Variable is not found, OUT parameters are not valid
**/
EFI_STATUS
EfiLibHiiVariablePackListGetMapById (
  IN      UINT16                     Id,  
  IN      EFI_HII_VARIABLE_PACK_LIST *List,
     OUT  CHAR16                     **Name, OPTIONAL
     OUT  EFI_GUID                   **Guid, OPTIONAL
     OUT  VOID                       **Var,
     OUT  UINTN                      *Size
  )
{ 
  CHAR16                            *MapName;
  EFI_GUID                          *MapGuid;
  UINT16                            MapId;
  VOID                              *Map;
  UINTN                             MapSize;

  while (NULL != List) {
    EfiLibHiiVariablePackGetMap (List->VariablePack, &MapName, &MapGuid, &MapId, &Map, &MapSize);
    if (MapId == Id) {
      *Var  = Map;
      *Size = MapSize;
      if (NULL != Name) {
         *Name = MapName;
      }
      if (NULL != Guid) {
        *Guid = MapGuid;
      }
      //
      // Map found
      //
      return EFI_SUCCESS; 
    }
    List = List->NextVariablePack;
  }
  //
  // If here, the map is not found
  //
  return EFI_NOT_FOUND; 
}

/**
  Finds a variable form EFI_HII_VARIABLE_PACK_LIST given name and GUID.

  @param  List           List of variables
  @param  Name           Name of the variable/map to be found
  @param  Guid           GUID of the variable/map to be found
  @param  Id             Id of the variable/map to be found
  @param  Var            Pointer to the variable/map found
  @param  Size           Size of the variable/map in bytes found

  @retval EFI_SUCCESS    variable is found, OUT parameters are valid
  @retval EFI_NOT_FOUND  variable is not found, OUT parameters are not valid
**/
EFI_STATUS
EfiLibHiiVariablePackListGetMap (
  IN      EFI_HII_VARIABLE_PACK_LIST *List,
  IN      CHAR16                     *Name,
  IN      EFI_GUID                   *Guid,
     OUT  UINT16                     *Id,
     OUT  VOID                       **Var, 
     OUT  UINTN                      *Size
  )
{ 
  VOID                              *Map;
  UINTN                             MapSize;
  UINT16                            MapId;
  CHAR16                            *MapName;
  EFI_GUID                          *MapGuid;

  while (NULL != List) {
    EfiLibHiiVariablePackGetMap (List->VariablePack, &MapName, &MapGuid, &MapId, &Map, &MapSize);
    if ((0 == StrCmp (Name, MapName)) && CompareGuid (Guid, MapGuid)) {
      *Id   = MapId;
      *Var  = Map;
      *Size = MapSize;
      return EFI_SUCCESS;
    }
    List = List->NextVariablePack;
  }
  //
  // If here, the map is not found
  //
  return EFI_NOT_FOUND;
}

/**
  Finds out if a variable of specific Name/Guid/Size exists in NV. 
  If it does, it will retrieve it into the Var. 

  @param  Name            Parameters of the variable to retrieve. Must match exactly.
  @param  Guid            Parameters of the variable to retrieve. Must match exactly.
  @param  Size            Parameters of the variable to retrieve. Must match exactly.
  @param  Var             Variable will be retrieved into buffer pointed by this pointer.
                          If pointing to NULL, the buffer will be allocated. 
                          Caller is responsible for releasing the buffer.

  @retval EFI_SUCCESS     The variable of exact Name/Guid/Size parameters was retrieved and written to Var.
  @retval EFI_NOT_FOUND   The variable of this Name/Guid was not found in the NV.
  @retval EFI_LOAD_ERROR  The variable in the NV was of different size, or NV API returned error.
**/
EFI_STATUS
EfiLibHiiVariableRetrieveFromNv (
  IN      CHAR16                    *Name,
  IN      EFI_GUID                  *Guid,
  IN      UINTN                     Size,
     OUT  VOID                      **Var
  )
{
  EFI_STATUS                        Status;
  UINTN                             SizeNv;

  //
  // Test for existence of the variable.
  //
  SizeNv = 0;
  Status = gRT->GetVariable (Name, Guid, NULL, &SizeNv, NULL);
  if (EFI_BUFFER_TOO_SMALL != Status) {
    ASSERT (EFI_SUCCESS != Status);
    return EFI_NOT_FOUND;
  }
  if (SizeNv != Size) {
    //
    // The variable is considered corrupt, as it has different size from expected.
    //
    return EFI_LOAD_ERROR; 
  }

  if (NULL == *Var) {
    *Var = AllocatePool (Size);
    ASSERT (NULL != *Var);
  }
  SizeNv = Size;
  //
  // Final read into the Var
  //
  Status = gRT->GetVariable (Name, Guid, NULL, &SizeNv, *Var); 
  //
  // No tolerance for random failures. Such behavior is undetermined and not validated.
  //
  ASSERT_EFI_ERROR (Status); 
  ASSERT (SizeNv == Size);
  return EFI_SUCCESS;
}

/**
  Overrrides the variable with NV data if found.
  But it only does it if the Name ends with specified Suffix.
  For example, if Suffix="MyOverride" and the Name="XyzSetupMyOverride",
  the Suffix matches the end of Name, so the variable will be loaded from NV
  provided the variable exists and the GUID and Size matches.

  @param  Suffix            Suffix the Name should end with.
  @param  Name              Name of the variable to retrieve.
  @Param  Guid              Guid of the variable to retrieve.
  @Param  Size              Parameters of the variable to retrieve.
  @param  Var               Variable will be retrieved into this buffer.
                            Caller is responsible for providing storage of exactly Size size in bytes.

  @retval EFI_SUCCESS           The variable was overriden with NV variable of same Name/Guid/Size.
  @retval EFI_INVALID_PARAMETER The name of the variable does not end with <Suffix>.
  @retval EFI_NOT_FOUND         The variable of this Name/Guid was not found in the NV.
  @retval EFI_LOAD_ERROR        The variable in the NV was of different size, or NV API returned error.
**/
EFI_STATUS
EfiLibHiiVariableOverrideIfSuffix (
  IN      CHAR16                    *Suffix,
  IN      CHAR16                    *Name,
  IN      EFI_GUID                  *Guid,
  IN      UINTN                     Size,
     OUT  VOID                      *Var
  )  
{
  UINTN                             StrLength;
  UINTN                             StrLenSuffix;

  StrLength       = StrLen (Name);
  StrLenSuffix    = StrLen (Suffix);
  if ((StrLength <= StrLenSuffix) || (0 != StrCmp (Suffix, &Name[StrLength - StrLenSuffix]))) {
    //
    // Not ending with <Suffix>.
    //
    return EFI_INVALID_PARAMETER; 
  }
  return EfiLibHiiVariableRetrieveFromNv (Name, Guid, Size, &Var);
}

/**
  Overrrides the variable with NV data if found.
  But it only does it if the NV contains the same variable with Name is appended with Suffix.  
  For example, if Suffix="MyOverride" and the Name="XyzSetup",
  the Suffix will be appended to the end of Name, and the variable with Name="XyzSetupMyOverride"
  will be loaded from NV provided the variable exists and the GUID and Size matches.

  @param  Suffix          Suffix the variable will be appended with.
  @param  Name            Parameters of the Name variable to retrieve.
  @param  Guid            Parameters of the Guid variable to retrieve.
  @param  Size            Parameters of the Size variable to retrieve.
  @param  Var             Variable will be retrieved into this buffer.
                          Caller is responsible for providing storage of exactly Size size in bytes.

  @retval EFI_SUCCESS     The variable was overriden with NV variable of same Name/Guid/Size.
  @retval EFI_NOT_FOUND   The variable of this Name/Guid was not found in the NV.
  @retval EFI_LOAD_ERROR  The variable in the NV was of different size, or NV API returned error.
**/
EFI_STATUS
EfiLibHiiVariableOverrideBySuffix (
  IN      CHAR16                    *Suffix,
  IN      CHAR16                    *Name,
  IN      EFI_GUID                  *Guid,
  IN      UINTN                     Size,
     OUT  VOID                      *Var
  )
{
  EFI_STATUS                        Status;
  CHAR16                            *NameSuffixed;
  UINTN                             NameLength;
  UINTN                             SuffixLength;

  //
  // enough to concatenate both strings.
  //
  NameLength   = StrLen (Name);
  SuffixLength = StrLen (Suffix);
  NameSuffixed = AllocateZeroPool ((NameLength + SuffixLength + 1) * sizeof (CHAR16)); 
  
  StrCpy (NameSuffixed, Name);
  StrCat (NameSuffixed, Suffix);
  
  Status = EfiLibHiiVariableRetrieveFromNv (NameSuffixed, Guid, Size, &Var);
  gBS->FreePool (NameSuffixed);
  
  return Status;
}

