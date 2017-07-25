/*
 * Copyright (c) 2005, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Clock implementation for Unix.
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "sys/clock.h"
#include <time.h>
#include <sys/time.h>
#if defined(__MACH__)
#include <mach/mach_time.h>
#endif

/*---------------------------------------------------------------------------*/
void
clock_init(void)
{
}
/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
#ifdef __linux
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#elif defined(__MACH__)
  static mach_timebase_info_data_t info = {0,0};
  if(info.denom == 0) {
    mach_timebase_info(&info);
  }
  uint64_t elapsednano = mach_absolute_time() * (info.numer / info.denom);
  return elapsednano / 1000000;
#else
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}
/*---------------------------------------------------------------------------*/
unsigned long
clock_seconds(void)
{
#ifdef __linux
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);

  return ts.tv_sec;
#elif defined(__MACH__)
  static mach_timebase_info_data_t info = {0,0};
  if(info.denom == 0) {
    mach_timebase_info(&info);
  }
  uint64_t elapsednano = mach_absolute_time() * (info.numer / info.denom);
  return elapsednano / 1000000000;
#else
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return tv.tv_sec;
#endif
}
/*---------------------------------------------------------------------------*/
void
clock_delay(unsigned int d)
{
  /* Does not do anything. */
}
/*---------------------------------------------------------------------------*/
