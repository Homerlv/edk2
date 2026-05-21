#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
HelloWorld(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  Print(L"Hello World!\n");
  return EFI_SUCCESS;
}