/**
 * @file
 *
 * @brief Table of User Defined Configuration Parameters
 *
 * This include file contains the table of user defined configuration
 * parameters.
 */

/*
 *  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_CONFIG_H
#define _RTEMS_CONFIG_H

/*
 *  Unlimited object support. Changes the configuration table entry for POSIX
 *  or RTEMS APIs to bounded only by the memory of the work-space.
 *
 *  Use the macro to define the resource unlimited before placing in
 *  the configuration table.
 */

#include <rtems/score/object.h>
#include <rtems/score/isr.h>
#include <rtems/score/memory.h>
#include <rtems/score/stack.h>
#include <rtems/score/userextdata.h>
#include <rtems/score/watchdogticks.h>
#include <rtems/rtems/config.h>
#include <rtems/extension.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/rtems/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RTEMS_UNLIMITED_OBJECTS OBJECTS_UNLIMITED_OBJECTS

#define rtems_resource_unlimited(resource) \
  ( resource | RTEMS_UNLIMITED_OBJECTS )

#define rtems_resource_is_unlimited(resource) \
  _Objects_Is_unlimited(resource)

#define rtems_resource_maximum_per_allocation(resource) \
  _Objects_Maximum_per_allocation(resource)

/**
 * @copydoc Stack_Allocator_initialize
 */
typedef Stack_Allocator_initialize rtems_stack_allocate_init_hook;

/**
 * @copydoc Stack_Allocator_allocate
 */
typedef Stack_Allocator_allocate rtems_stack_allocate_hook;

/**
 * @copydoc Stack_Allocator_free
 */
typedef Stack_Allocator_free rtems_stack_free_hook;

/*
 *  The following records define the Configuration Table.  The
 *  information contained in this table is required in all
 *  RTEMS systems, whether single or multiprocessor.  This
 *  table primarily defines the following:
 *
 *     + location and size of the RTEMS Workspace
 *     + microseconds per clock tick
 *     + clock ticks per task timeslice
 *     + required number of each object type for each API configured
 */
typedef struct {
  /**
   * This field specifies the size in bytes of the RTEMS Workspace.
   */
  uintptr_t                      work_space_size;

  /** 
   * This element points to the BSP's optional idle task which may override
   * the default one provided with RTEMS.
   */
  void                        *(*idle_task)( uintptr_t );

  /** 
   * This field specifies the size of the IDLE task's stack.  If less than or
   * equal to the minimum stack size, then the IDLE task will have the minimum
   * stack size.
   */
  uint32_t                       idle_task_stack_size;

  /**
   * @brief Specifies if a unified work area is used or not.
   *
   * If this element is @a true, then the RTEMS Workspace and the C Program
   * Heap use the same heap, otherwise they use separate heaps.
   */
  bool                           unified_work_area;

  #ifdef RTEMS_SMP
    bool                         smp_enabled;
  #endif

  #ifdef RTEMS_SMP
    uint32_t                     maximum_processors;
  #endif
} rtems_configuration_table;

/**
 * This is the configuration table generated by confdefs.h.
 */
extern const rtems_configuration_table Configuration;

/*
 *  Some handy macros to avoid dependencies on either the BSP
 *  or the exact format of the configuration table.
 */

#define rtems_configuration_get_unified_work_area() \
        (Configuration.unified_work_area)

/**
 * @brief Return if the stack allocator avoids the work space.
 *
 * @retval true The stack allocator must not allocate the thread stacks from the
 * RTEMS Workspace
 *
 * @retval false The stack allocator should allocate the thread stacks from the
 * RTEMS Workspace.
 */
#define rtems_configuration_get_stack_allocator_avoids_work_space() \
  (_Stack_Allocator_avoids_workspace)

uintptr_t rtems_configuration_get_stack_space_size( void );

#define rtems_configuration_get_work_space_size() \
        (Configuration.work_space_size + \
          (rtems_configuration_get_stack_allocator_avoids_work_space() ? \
            0 : rtems_configuration_get_stack_space_size()))

uint32_t rtems_configuration_get_maximum_extensions( void );

#define rtems_configuration_get_microseconds_per_tick() \
        (_Watchdog_Microseconds_per_tick)
#define rtems_configuration_get_milliseconds_per_tick() \
        (_Watchdog_Microseconds_per_tick / 1000)
#define rtems_configuration_get_nanoseconds_per_tick() \
        (_Watchdog_Nanoseconds_per_tick)

#define rtems_configuration_get_ticks_per_timeslice() \
        (_Watchdog_Ticks_per_timeslice)

#define rtems_configuration_get_idle_task() \
        (Configuration.idle_task)

#define rtems_configuration_get_idle_task_stack_size() \
        (Configuration.idle_task_stack_size)

#define rtems_configuration_get_interrupt_stack_size() \
        ((size_t) _ISR_Stack_size)

#define rtems_configuration_get_stack_allocate_init_hook() \
  (_Stack_Allocator_initialize)

#define rtems_configuration_get_stack_allocate_hook() \
  (_Stack_Allocator_allocate)

#define rtems_configuration_get_stack_free_hook() \
  (_Stack_Allocator_free)

 /**
  * This macro assists in accessing the field which indicates whether
  * RTEMS is responsible for zeroing the Executive Workspace.
  */
#define rtems_configuration_get_do_zero_of_workspace() \
  _Memory_Zero_before_use

#define rtems_configuration_get_number_of_initial_extensions() \
  ((uint32_t) _User_extensions_Initial_count)

#define rtems_configuration_get_user_extension_table() \
  (&_User_extensions_Initial_extensions[ 0 ])

#if defined(RTEMS_MULTIPROCESSING)
  #define rtems_configuration_get_user_multiprocessing_table() \
    (&_MPCI_Configuration)
#else
  #define rtems_configuration_get_user_multiprocessing_table() \
    NULL
#endif

/**
 * @brief Returns true if the SMP mode of operation is enabled, and false
 * otherwise.
 *
 * In uni-processor configurations this is a compile-time constant which
 * evaluates to false.
 *
 * @retval true SMP mode of operation is enabled.
 * @retval false Otherwise.
 */
#ifdef RTEMS_SMP
  #define rtems_configuration_is_smp_enabled() \
        (Configuration.smp_enabled)
#else
  #define rtems_configuration_is_smp_enabled() \
        false
#endif

/**
 * @brief Returns the configured maximum count of processors.
 *
 * The actual number of processors available for the application will be less
 * than or equal to the configured maximum count of processors.
 *
 * On single-processor configurations this is a compile time constant which
 * evaluates to one.
 *
 * @return The configured maximum count of processors.
 */
#ifdef RTEMS_SMP
  #define rtems_configuration_get_maximum_processors() \
        (Configuration.maximum_processors)
#else
  #define rtems_configuration_get_maximum_processors() \
        1
#endif

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
