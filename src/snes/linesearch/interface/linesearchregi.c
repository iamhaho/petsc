#include <petsc-private/linesearchimpl.h>     /*I  "petscsnes.h"  I*/

PETSC_EXTERN PetscErrorCode SNESLineSearchCreate_Basic(SNESLineSearch);
PETSC_EXTERN PetscErrorCode SNESLineSearchCreate_L2(SNESLineSearch);
PETSC_EXTERN PetscErrorCode SNESLineSearchCreate_CP(SNESLineSearch);
PETSC_EXTERN PetscErrorCode SNESLineSearchCreate_BT(SNESLineSearch);
PETSC_EXTERN PetscErrorCode SNESLineSearchCreate_Shell(SNESLineSearch);


#undef __FUNCT__
#define __FUNCT__ "SNESLineSearchRegisterAll"
/*@C
   SNESLineSearchRegisterAll - Registers all of the nonlinear solver methods in the SNESLineSearch package.

   Not Collective

   Level: advanced

.keywords: SNESLineSearch, register, all

.seealso:  SNESLineSearchRegisterDestroy()
@*/
PetscErrorCode SNESLineSearchRegisterAll(const char path[])
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  SNESLineSearchRegisterAllCalled = PETSC_TRUE;
  ierr = SNESLineSearchRegister(SNESLINESEARCHSHELL,             path,"SNESLineSearchCreate_Shell",             SNESLineSearchCreate_Shell);CHKERRQ(ierr);
  ierr = SNESLineSearchRegister(SNESLINESEARCHBASIC,             path,"SNESLineSearchCreate_Basic",             SNESLineSearchCreate_Basic);CHKERRQ(ierr);
  ierr = SNESLineSearchRegister(SNESLINESEARCHL2,                path,"SNESLineSearchCreate_L2",                SNESLineSearchCreate_L2);CHKERRQ(ierr);
  ierr = SNESLineSearchRegister(SNESLINESEARCHBT,                path,"SNESLineSearchCreate_BT",                SNESLineSearchCreate_BT);CHKERRQ(ierr);
  ierr = SNESLineSearchRegister(SNESLINESEARCHCP,                path,"SNESLineSearchCreate_CP",                SNESLineSearchCreate_CP);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "SNESLineSearchRegisterDestroy"
PetscErrorCode  SNESLineSearchRegisterDestroy(void)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscFunctionListDestroy(&SNESLineSearchList);CHKERRQ(ierr);

  SNESLineSearchRegisterAllCalled = PETSC_FALSE;
  PetscFunctionReturn(0);
}
