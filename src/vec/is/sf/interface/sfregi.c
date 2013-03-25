#include <petsc-private/sfimpl.h>     /*I  "petscsf.h"  I*/

#if defined(PETSC_HAVE_MPI_WIN_CREATE)
PETSC_EXTERN PetscErrorCode PetscSFCreate_Window(PetscSF);
#endif
PETSC_EXTERN PetscErrorCode PetscSFCreate_Basic(PetscSF);

PetscFunctionList PetscSFunctionList;

#undef __FUNCT__
#define __FUNCT__ "PetscSFRegisterAll"
/*@C
   PetscSFRegisterAll - Registers all the PetscSF communication implementations

   Not Collective

   Level: advanced

.keywords: PetscSF, register, all

.seealso:  PetscSFRegisterDestroy()
@*/
PetscErrorCode  PetscSFRegisterAll(const char path[])
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscSFRegisterAllCalled = PETSC_TRUE;
#if defined(PETSC_HAVE_MPI_WIN_CREATE) && defined(PETSC_HAVE_MPI_TYPE_DUP)
  ierr = PetscSFRegister(PETSCSFWINDOW,       path,"PetscSFCreate_Window",       PetscSFCreate_Window);CHKERRQ(ierr);
#endif
  ierr = PetscSFRegister(PETSCSFBASIC,        path,"PetscSFCreate_Basic",        PetscSFCreate_Basic);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscSFRegister"
/*@C
  PetscSFRegister  - Adds an implementation of the PetscSF communication protocol.

   Not collective

   Input Parameters:
+  name_impl - name of a new user-defined implementation
.  name_create - name of routine to create method context
-  routine_create - routine to create method context

   Notes:
   PetscSFRegister() may be called multiple times to add several user-defined implementations.

   Sample usage:
.vb
   PetscSFRegister("my_impl","MyImplCreate",MyImplCreate);
.ve

   Then, this implementation can be chosen with the procedural interface via
$     PetscSFSetType(sf,"my_impl")
   or at runtime via the option
$     -snes_type my_solver

   Level: advanced

.keywords: PetscSF, register

.seealso: PetscSFRegisterAll(), PetscSFRegisterDestroy()
@*/
PetscErrorCode  PetscSFRegister(const char sname[],const char path[],const char name[],PetscErrorCode (*function)(PetscSF))
{
  char           fullname[PETSC_MAX_PATH_LEN];
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFunctionListConcat(path,name,fullname);CHKERRQ(ierr);
  ierr = PetscFunctionListAdd(PETSC_COMM_WORLD,&PetscSFunctionList,sname,fullname,(void (*)(void))function);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "PetscSFRegisterDestroy"
/*@
   PetscSFRegisterDestroy - Frees the list of communication implementations registered by PetscSFRegister()

   Not Collective

   Level: advanced

.keywords: PetscSF, register, destroy

.seealso: PetscSFRegisterAll()
@*/
PetscErrorCode  PetscSFRegisterDestroy(void)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFunctionListDestroy(&PetscSFunctionList);CHKERRQ(ierr);

  PetscSFRegisterAllCalled = PETSC_FALSE;
  PetscFunctionReturn(0);
}
