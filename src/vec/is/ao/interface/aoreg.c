
#include <../src/vec/is/ao/aoimpl.h>    /*I "petscao.h"  I*/

PetscFunctionList AOList              = NULL;
PetscBool         AORegisterAllCalled = PETSC_FALSE;

#undef __FUNCT__
#define __FUNCT__ "AOSetType"
/*@C
  AOSetType - Builds an application ordering for a particular implementation.

  Collective on AO

  Input Parameters:
+ ao    - The AO object
- method - The name of the AO type

  Options Database Key:
. -ao_type <type> - Sets the AO type; use -help for a list of available types

  Notes:
  See "petsc/include/petscao.h" for available AO types (for instance, AOBASIC and AOMEMORYSCALABLE).

  Level: intermediate

.keywords: ao, set, type
.seealso: AOGetType(), AOCreate()
@*/
PetscErrorCode  AOSetType(AO ao, AOType method)
{
  PetscErrorCode (*r)(AO);
  PetscBool      match;
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ao, AO_CLASSID,1);
  ierr = PetscObjectTypeCompare((PetscObject)ao, method, &match);CHKERRQ(ierr);
  if (match) PetscFunctionReturn(0);

  if (!AORegisterAllCalled) {ierr = AORegisterAll(NULL);CHKERRQ(ierr);}
  ierr = PetscFunctionListFind(PetscObjectComm((PetscObject)ao), AOList, method,PETSC_TRUE,(void (**)(void)) &r);CHKERRQ(ierr);
  if (!r) SETERRQ1(PETSC_COMM_SELF,PETSC_ERR_ARG_UNKNOWN_TYPE, "Unknown AO type: %s", method);
  if (ao->ops->destroy) {
    ierr             = (*ao->ops->destroy)(ao);CHKERRQ(ierr);
    ao->ops->destroy = NULL;
  }

  ierr = (*r)(ao);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "AOGetType"
/*@C
  AOGetType - Gets the AO type name (as a string) from the AO.

  Not Collective

  Input Parameter:
. ao  - The vector

  Output Parameter:
. type - The AO type name

  Level: intermediate

.keywords: ao, get, type, name
.seealso: AOSetType(), AOCreate()
@*/
PetscErrorCode  AOGetType(AO ao, AOType *type)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PetscValidHeaderSpecific(ao, AO_CLASSID,1);
  PetscValidCharPointer(type,2);
  if (!AORegisterAllCalled) {
    ierr = AORegisterAll(NULL);CHKERRQ(ierr);
  }
  *type = ((PetscObject)ao)->type_name;
  PetscFunctionReturn(0);
}


/*--------------------------------------------------------------------------------------------------------------------*/

#undef __FUNCT__
#define __FUNCT__ "AORegister"
/*@C
  AORegister - See AORegister()

  Level: advanced
@*/
PetscErrorCode  AORegister(const char sname[], const char path[], const char name[], PetscErrorCode (*function)(AO))
{
  char           fullname[PETSC_MAX_PATH_LEN];
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr = PetscStrcpy(fullname, path);CHKERRQ(ierr);
  ierr = PetscStrcat(fullname, ":");CHKERRQ(ierr);
  ierr = PetscStrcat(fullname, name);CHKERRQ(ierr);
  ierr = PetscFunctionListAdd(PETSC_COMM_WORLD,&AOList, sname, fullname, (void (*)(void)) function);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}


/*--------------------------------------------------------------------------------------------------------------------*/
#undef __FUNCT__
#define __FUNCT__ "AORegisterDestroy"
/*@C
   AORegisterDestroy - Frees the list of AO methods that were registered by AORegister()/AORegister().

   Not Collective

   Level: advanced

.keywords: AO, register, destroy
.seealso: AORegister(), AORegisterAll(), AORegister()
@*/
PetscErrorCode  AORegisterDestroy(void)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  ierr                = PetscFunctionListDestroy(&AOList);CHKERRQ(ierr);
  AORegisterAllCalled = PETSC_FALSE;
  PetscFunctionReturn(0);
}

