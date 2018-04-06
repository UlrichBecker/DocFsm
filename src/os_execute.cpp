/*****************************************************************************/
/*                                                                           */
/*!                @brief Module invokes a executable file                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    os_execute.cpp                                                  */
/*! @see     os_execute.hpp                                                  */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifdef __linux__
   #include "os_execute_linux.cpp"
#else
   #error Sorry, at the moment this module is compilable for Linux-targets only!
#endif
//!  @todo Class Execution for MS-Windows and MAC-OS
//================================== EOF ======================================
