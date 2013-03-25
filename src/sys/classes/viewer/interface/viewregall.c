
#include <petsc-private/viewerimpl.h>  /*I "petscsys.h" I*/

PETSC_EXTERN PetscErrorCode PetscViewerCreate_Socket(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_ASCII(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_Binary(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_String(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_Draw(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_VU(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_Mathematica(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_Netcdf(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_HDF5(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_Matlab(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_AMS(PetscViewer);
PETSC_EXTERN PetscErrorCode PetscViewerCreate_VTK(PetscViewer);

#undef __FUNCT__
#define __FUNCT__ "PetscViewerRegisterAll"
/*@C
  PetscViewerRegisterAll - Registers all of the graphics methods in the PetscViewer package.

  Not Collective

   Level: developer

.seealso:  PetscViewerRegisterDestroy()
@*/
PetscErrorCode  PetscViewerRegisterAll(const char *path)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscViewerRegister(PETSCVIEWERASCII,      path,"PetscViewerCreate_ASCII",      PetscViewerCreate_ASCII);CHKERRQ(ierr);
  ierr = PetscViewerRegister(PETSCVIEWERBINARY,     path,"PetscViewerCreate_Binary",     PetscViewerCreate_Binary);CHKERRQ(ierr);
  ierr = PetscViewerRegister(PETSCVIEWERSTRING,     path,"PetscViewerCreate_String",     PetscViewerCreate_String);CHKERRQ(ierr);
  ierr = PetscViewerRegister(PETSCVIEWERDRAW,       path,"PetscViewerCreate_Draw",       PetscViewerCreate_Draw);CHKERRQ(ierr);
#if defined(PETSC_USE_SOCKET_VIEWER)
  ierr = PetscViewerRegister(PETSCVIEWERSOCKET,     path,"PetscViewerCreate_Socket",     PetscViewerCreate_Socket);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_MATHEMATICA)
  ierr = PetscViewerRegister(PETSCVIEWERMATHEMATICA,path,"PetscViewerCreate_Mathematica",PetscViewerCreate_Mathematica);CHKERRQ(ierr);
#endif
  ierr = PetscViewerRegister(PETSCVIEWERVU,         path,"PetscViewerCreate_VU",         PetscViewerCreate_VU);CHKERRQ(ierr);
#if defined(PETSC_HAVE_HDF5)
  ierr = PetscViewerRegister(PETSCVIEWERHDF5,       path,"PetscViewerCreate_HDF5",       PetscViewerCreate_HDF5);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_MATLAB_ENGINE)
  ierr = PetscViewerRegister(PETSCVIEWERMATLAB,     path,"PetscViewerCreate_Matlab",     PetscViewerCreate_Matlab);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_AMS)
  ierr = PetscViewerRegister(PETSCVIEWERAMS,        path,"PetscViewerCreate_AMS",        PetscViewerCreate_AMS);CHKERRQ(ierr);
#endif
  ierr = PetscViewerRegister(PETSCVIEWERVTK,        path,"PetscViewerCreate_VTK",        PetscViewerCreate_VTK);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

