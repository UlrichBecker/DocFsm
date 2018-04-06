/*****************************************************************************/
/*                                                                           */
/*!                @brief Module invokes a executable file                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    os_execute.hpp                                                  */
/*! @see     os_execute.cpp                                                  */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _OS_EXECUTE_HPP
#define _OS_EXECUTE_HPP
#ifdef __linux__
   #include "os_execute_linux.hpp"
#else
   #error Sorry, at the moment this module is compilable for Linux-targets only!
#endif

//!  @todo Class Execution for MS-Windows and MAC-OS

#endif // ifndef _OS_EXECUTE_HPP
//================================== EOF ======================================
