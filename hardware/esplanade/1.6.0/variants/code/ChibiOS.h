#ifndef __CHIBI_OS_H__
#define __CHIBI_OS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CH_CFG_ST_FREQUENCY     100 /* Our SysTick timer is set to 100 Hz */

/**
 * @name    Time conversion utilities
 * @{
 */
/**
 * @brief   Seconds to system ticks.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define S2ST(sec)                                                           \
  ((systime_t)((uint32_t)(sec) * (uint32_t)CH_CFG_ST_FREQUENCY))

/**
 * @brief   Milliseconds to system ticks.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define MS2ST(msec)                                                         \
  ((systime_t)(((((uint32_t)(msec)) *                                       \
                 ((uint32_t)CH_CFG_ST_FREQUENCY)) + 999UL) / 1000UL))

/**
 * @brief   Microseconds to system ticks.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define US2ST(usec)                                                         \
  ((systime_t)(((((uint32_t)(usec)) *                                       \
                 ((uint32_t)CH_CFG_ST_FREQUENCY)) + 999999UL) / 1000000UL))

/**
 * @brief   System ticks to seconds.
 * @details Converts from system ticks number to seconds.
 * @note    The result is rounded up to the next second boundary.
 *
 * @param[in] n         number of system ticks
 * @return              The number of seconds.
 *
 * @api
 */
#define ST2S(n) (((n) + CH_CFG_ST_FREQUENCY - 1UL) / CH_CFG_ST_FREQUENCY)

/**
 * @brief   System ticks to milliseconds.
 * @details Converts from system ticks number to milliseconds.
 * @note    The result is rounded up to the next millisecond boundary.
 *
 * @param[in] n         number of system ticks
 * @return              The number of milliseconds.
 *
 * @api
 */
#define ST2MS(n) (((n) * 1000UL + CH_CFG_ST_FREQUENCY - 1UL) /              \
                  CH_CFG_ST_FREQUENCY)

/**
 * @brief   System ticks to microseconds.
 * @details Converts from system ticks number to microseconds.
 * @note    The result is rounded up to the next microsecond boundary.
 *
 * @param[in] n         number of system ticks
 * @return              The number of microseconds.
 *
 * @api
 */
#define ST2US(n) (((n) * 1000000UL + CH_CFG_ST_FREQUENCY - 1UL) /           \
                  CH_CFG_ST_FREQUENCY)
/** @} */

/* thread_t happens to be 64-bytes on our platform */
typedef struct {
    uint8_t data[64];
} thread_t;

typedef uint32_t systime_t;
typedef uint32_t tprio_t;
typedef uint32_t msg_t;
typedef thread_t * thread_reference_t;

/**
 * @brief   Type of a Virtual Timer callback function.
 */
typedef void (*vtfunc_t)(void *p);

/**
 * @brief   Thread function.
 */
typedef void (*tfunc_t)(void *p);

/**
 * @brief   Type of a Virtual Timer structure.
 */
typedef struct ch_virtual_timer virtual_timer_t;

/**
 * @extends virtual_timers_list_t
 *
 * @brief   Virtual Timer descriptor structure.
 */
struct ch_virtual_timer {
  virtual_timer_t       *vt_next;   /**< @brief Next timer in the list.     */
  virtual_timer_t       *vt_prev;   /**< @brief Previous timer in the list. */
  systime_t             vt_delta;   /**< @brief Time delta before timeout.  */
  vtfunc_t              vt_func;    /**< @brief Timer callback function
                                                pointer.                    */
  void                  *vt_par;    /**< @brief Timer callback function
                                                parameter.                  */
};

int setTimer(virtual_timer_t *vtp, systime_t delay,
             vtfunc_t vtfunc, void *par);
void resetTimer(virtual_timer_t *vtp);

thread_t *createThread(void *wsp, size_t size,
                       tprio_t prio, tfunc_t pf, void *arg);
msg_t suspendThread(thread_reference_t *trp);
msg_t suspendThreadTimeout(thread_reference_t *trp, systime_t timeout);
void resumeThread(thread_reference_t *trp, msg_t msg);
void yieldThread(void);
void threadSleep(systime_t time);
void threadSleepUntil(systime_t time);
msg_t waitThread(thread_t *tp);
void exitThread(msg_t msg);

void lockSystem(void);
void lockSystemFromISR(void);

void unlockSystem();
void unlockSystemFromISR(void);

void getSyscallABI(void);

#ifdef __cplusplus
};
#endif

#endif /* __CHIBI_OS_H__ */