/*
 * Copyright (c) 2018 embedded brains GmbH
 *
 * Copyright (c) 2015 University of York.
 * Hesham ALmatary <hesham@alumni.york.ac.uk>
 *
 * COPYRIGHT (c) 1989-1999.
 * On-Line Applications Research Corporation (OAR).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <rtems/score/cpuimpl.h>
#include <rtems/score/isr.h>
#include <rtems/score/riscv-utility.h>

#define RISCV_ASSERT_CONTEXT_OFFSET( field, off ) \
  RTEMS_STATIC_ASSERT( \
    offsetof( Context_Control, field) == RISCV_CONTEXT_ ## off, \
    riscv_context_offset_ ## field \
  )

RISCV_ASSERT_CONTEXT_OFFSET( isr_dispatch_disable, ISR_DISPATCH_DISABLE );
#ifdef RTEMS_SMP
RISCV_ASSERT_CONTEXT_OFFSET( is_executing, IS_EXECUTING );
#endif
RISCV_ASSERT_CONTEXT_OFFSET( ra, RA );
RISCV_ASSERT_CONTEXT_OFFSET( sp, SP );
RISCV_ASSERT_CONTEXT_OFFSET( tp, TP );
RISCV_ASSERT_CONTEXT_OFFSET( s0, S0 );
RISCV_ASSERT_CONTEXT_OFFSET( s1, S1 );
RISCV_ASSERT_CONTEXT_OFFSET( s2, S2 );
RISCV_ASSERT_CONTEXT_OFFSET( s3, S3 );
RISCV_ASSERT_CONTEXT_OFFSET( s4, S4 );
RISCV_ASSERT_CONTEXT_OFFSET( s5, S5 );
RISCV_ASSERT_CONTEXT_OFFSET( s6, S6 );
RISCV_ASSERT_CONTEXT_OFFSET( s7, S7 );
RISCV_ASSERT_CONTEXT_OFFSET( s8, S8 );
RISCV_ASSERT_CONTEXT_OFFSET( s9, S9 );
RISCV_ASSERT_CONTEXT_OFFSET( s10, S10 );
RISCV_ASSERT_CONTEXT_OFFSET( s11, S11 );

#if __riscv_flen > 0

RISCV_ASSERT_CONTEXT_OFFSET( fcsr, FCSR );
RISCV_ASSERT_CONTEXT_OFFSET( fs0, FS0 );
RISCV_ASSERT_CONTEXT_OFFSET( fs1, FS1 );
RISCV_ASSERT_CONTEXT_OFFSET( fs2, FS2 );
RISCV_ASSERT_CONTEXT_OFFSET( fs3, FS3 );
RISCV_ASSERT_CONTEXT_OFFSET( fs4, FS4 );
RISCV_ASSERT_CONTEXT_OFFSET( fs5, FS5 );
RISCV_ASSERT_CONTEXT_OFFSET( fs6, FS6 );
RISCV_ASSERT_CONTEXT_OFFSET( fs7, FS7 );
RISCV_ASSERT_CONTEXT_OFFSET( fs8, FS8 );
RISCV_ASSERT_CONTEXT_OFFSET( fs9, FS9 );
RISCV_ASSERT_CONTEXT_OFFSET( fs10, FS10 );
RISCV_ASSERT_CONTEXT_OFFSET( fs11, FS11 );

#endif /* __riscv_flen */

#define RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( field, off ) \
  RTEMS_STATIC_ASSERT( \
    offsetof( CPU_Interrupt_frame, field) == RISCV_INTERRUPT_FRAME_ ## off, \
    riscv_interrupt_frame_offset_ ## field \
  )

RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( mstatus, MSTATUS );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( mepc, MEPC );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a2, A2 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( s0, S0 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( s1, S1 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ra, RA );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a3, A3 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a4, A4 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a5, A5 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a6, A6 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a7, A7 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( t0, T0 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( t1, T1 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( t2, T2 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( t3, T3 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( t4, T4 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( t5, T5 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( t6, T6 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a0, A0 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( a1, A1 );

#if __riscv_flen > 0

RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fcsr, FCSR );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft0, FT0 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft1, FT1 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft2, FT2 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft3, FT3 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft4, FT4 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft5, FT5 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft6, FT6 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft7, FT7 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft8, FT8 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft9, FT9 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft10, FT10 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( ft11, FT11 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa0, FA0 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa1, FA1 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa2, FA2 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa3, FA3 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa4, FA4 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa5, FA5 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa6, FA6 );
RISCV_ASSERT_INTERRUPT_FRAME_OFFSET( fa7, FA7 );

#endif /* __riscv_flen */

#define RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( field, off ) \
  RTEMS_STATIC_ASSERT( \
    offsetof( CPU_Exception_frame, field) == RISCV_EXCEPTION_FRAME_ ## off, \
    riscv_context_offset_ ## field \
  )

RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( mcause, MCAUSE );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( sp, SP );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( gp, GP );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( tp, TP );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s2, S2 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s3, S3 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s4, S4 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s5, S5 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s6, S6 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s7, S7 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s8, S8 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s9, S9 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s10, S10 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( s11, S11 );

#if __riscv_flen > 0

RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs0, FS0 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs1, FS1 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs2, FS2 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs3, FS3 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs4, FS4 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs5, FS5 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs6, FS6 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs7, FS7 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs8, FS8 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs9, FS9 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs10, FS10 );
RISCV_ASSERT_EXCEPTION_FRAME_OFFSET( fs11, FS11 );

#endif /* __riscv_flen */

RTEMS_STATIC_ASSERT(
  sizeof( CPU_Interrupt_frame ) % CPU_STACK_ALIGNMENT == 0,
  riscv_interrupt_frame_size
);

/* bsp_start_vector_table_begin is the start address of the vector table
 * containing addresses to ISR Handlers. It's defined at the BSP linkcmds
 * and may differ from one BSP to another.
 */
extern char bsp_start_vector_table_begin[];

void _init(void);

void _fini(void);

void _init(void)
{
}

void _fini(void)
{
}

/**
 * @brief Performs processor dependent initialization.
 */
void _CPU_Initialize(void)
{
  /* Do nothing */
}

uint32_t _CPU_ISR_Get_level( void )
{
  if ( _CPU_ISR_Is_enabled( read_csr( mstatus ) ) ) {
    return 0;
  }

  return 1;
}

void _CPU_ISR_install_raw_handler(
  uint32_t   vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
  /* Do nothing */
}

void _CPU_ISR_install_vector(
  unsigned long    vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
)
{
  proc_ptr *table =
    (proc_ptr *) bsp_start_vector_table_begin;
  proc_ptr current_handler;

  ISR_Level level;

  _ISR_Local_disable( level );

  current_handler = table [vector];

  /* The current handler is now the old one */
  if (old_handler != NULL) {
    *old_handler = (proc_ptr) current_handler;
  }

  /* Write only if necessary to avoid writes to a maybe read-only
   * memory */
  if (current_handler != new_handler) {
    table [vector] = new_handler;
  }

  _ISR_Local_enable( level );

}

void *_CPU_Thread_Idle_body( uintptr_t ignored )
{
  do {
  } while (1);

  return NULL;
}
