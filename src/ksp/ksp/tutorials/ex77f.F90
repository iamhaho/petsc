!
!   Description: Solves a linear system with a block of right-hand sides using KSPHPDDM.
!

      program main
#include <petsc/finclude/petscksp.h>
      use petscksp
      implicit none
      Mat             X,B
      Mat             A
      KSP             ksp
      PC              pc
      Mat             F
      PetscScalar     alpha
      PetscReal       norm
      PetscInt        m,K
      PetscViewer     viewer
      character*(128) dir,name
      PetscBool       flg
      PetscErrorCode  ierr

      call PetscInitialize(PETSC_NULL_CHARACTER,ierr)
      if (ierr .ne. 0) then
        print *,'Unable to initialize PETSc'
        stop
      endif
      dir = '.'
      call PetscOptionsGetString(PETSC_NULL_OPTIONS,PETSC_NULL_CHARACTER,'-load_dir',dir,flg,ierr);CHKERRA(ierr)
      K = 5
      call PetscOptionsGetInt(PETSC_NULL_OPTIONS,PETSC_NULL_CHARACTER,'-n',K,flg,ierr);CHKERRA(ierr)
      call MatCreate(PETSC_COMM_WORLD,A,ierr);CHKERRA(ierr)
      call KSPCreate(PETSC_COMM_WORLD,ksp,ierr);CHKERRA(ierr)
      call KSPSetOperators(ksp,A,A,ierr);CHKERRA(ierr)
      write (name,'(a)')trim(dir)//'/A_400.dat'
      call PetscViewerBinaryOpen(PETSC_COMM_WORLD,name,FILE_MODE_READ,viewer,ierr);CHKERRA(ierr)
      call MatLoad(A,viewer,ierr);CHKERRA(ierr)
      call PetscViewerDestroy(viewer,ierr);CHKERRA(ierr)
      call MatGetLocalSize(A,m,PETSC_NULL_INTEGER,ierr);CHKERRA(ierr)
      call MatCreateDense(PETSC_COMM_WORLD,m,PETSC_DECIDE,PETSC_DECIDE,K,PETSC_NULL_SCALAR,B,ierr);CHKERRA(ierr)
      call MatCreateDense(PETSC_COMM_WORLD,m,PETSC_DECIDE,PETSC_DECIDE,K,PETSC_NULL_SCALAR,X,ierr);CHKERRA(ierr)
      call MatSetRandom(B,PETSC_NULL_RANDOM,ierr);CHKERRA(ierr)
      call KSPSetFromOptions(ksp,ierr);CHKERRA(ierr)
      call KSPSetUp(ksp,ierr);CHKERRA(ierr)
      call KSPMatSolve(ksp,B,X,ierr);CHKERRA(ierr)
      call KSPGetMatSolveBlockSize(ksp,M,ierr);CHKERRA(ierr)
      if (M .ne. PETSC_DECIDE) then
        call KSPSetMatSolveBlockSize(ksp,PETSC_DECIDE,ierr);CHKERRA(ierr)
        call MatZeroEntries(X,ierr);CHKERRA(ierr)
        call KSPMatSolve(ksp,B,X,ierr);CHKERRA(ierr)
      endif
      call KSPGetPC(ksp,pc,ierr);CHKERRA(ierr)
      call PetscObjectTypeCompare(pc,PCLU,flg,ierr);CHKERRA(ierr)
      if (flg) then
        call PCFactorGetMatrix(pc,F,ierr);CHKERRA(ierr)
        call MatMatSolve(F,B,B,ierr);CHKERRA(ierr)
        alpha = -1.0
        call MatAYPX(B,alpha,X,SAME_NONZERO_PATTERN,ierr);CHKERRA(ierr)
        call MatNorm(B,NORM_INFINITY,norm,ierr);CHKERRA(ierr)
        if (norm > 100*PETSC_MACHINE_EPSILON) then
          SETERRA(PETSC_COMM_WORLD,PETSC_ERR_PLIB,'KSPMatSolve() and MatMatSolve() difference has nonzero norm')
        endif
      endif
      call MatDestroy(X,ierr);CHKERRA(ierr)
      call MatDestroy(B,ierr);CHKERRA(ierr)
      call MatDestroy(A,ierr);CHKERRA(ierr)
      call KSPDestroy(ksp,ierr);CHKERRA(ierr)
      call PetscFinalize(ierr)
      end

!/*TEST
!
!   testset:
!      nsize: 2
!      requires: datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
!      args: -ksp_converged_reason -ksp_max_it 1000 -load_dir ${DATAFILESPATH}/matrices/hpddm/GCRODR
!      test:
!         suffix: 1
!         output_file: output/ex77_1.out
!         args:
!      test:
!         suffix: 2a
!         requires: hpddm
!         output_file: output/ex77_2_ksp_hpddm_type-gmres.out
!         args: -ksp_type hpddm -pc_type asm -ksp_hpddm_type gmres
!      test:
!         suffix: 2b
!         requires: hpddm
!         output_file: output/ex77_2_ksp_hpddm_type-bgmres.out
!         args: -ksp_type hpddm -pc_type asm -ksp_hpddm_type bgmres
!      test:
!         suffix: 3a
!         requires: hpddm
!         output_file: output/ex77_3_ksp_hpddm_type-gcrodr.out
!         args: -ksp_type hpddm -ksp_hpddm_recycle 5 -ksp_hpddm_type gcrodr
!      test:
!         suffix: 3b
!         requires: hpddm
!         output_file: output/ex77_3_ksp_hpddm_type-bgcrodr.out
!         args: -ksp_type hpddm -ksp_hpddm_recycle 5 -ksp_hpddm_type bgcrodr
!      test:
!         suffix: 4
!         requires: hpddm
!         output_file: output/ex77_4.out
!         args: -ksp_type hpddm -ksp_hpddm_recycle 5 -ksp_hpddm_type bgcrodr -ksp_view_final_residual -N 12 -ksp_matsolve_block_size 5
!   test:
!      nsize: 1
!      suffix: preonly
!      requires: hpddm datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
!      output_file: output/ex77_preonly.out
!      args: -N 6 -load_dir ${DATAFILESPATH}/matrices/hpddm/GCRODR -pc_type lu -ksp_type hpddm -ksp_hpddm_type preonly
!
!TEST*/
