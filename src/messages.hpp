/*****************************************************************************/
/*                                                                           */
/*!@brief Macros for colored error-, warning- and debug-messages for console */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    messages.hpp                                                    */
/*! @author  Ulrich Becker                                                   */
/*! @date    03.04.2018                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _MESSAGES_HPP
#define _MESSAGES_HPP

#include <iostream>

#define BOLD      "\e[1m"
#define RED       "\e[31m"
#define GREEN     "\e[32m"
#define YELLOW    "\e[33m"
#define BLUE      "\e[34m"
#define NORMAL    "\e[0m"

#define ERROR_MESSAGE( args... ) std::cerr << RED BOLD "ERROR: " \
                                 << args << NORMAL << std::endl

#define WARNING_MESSAGE( args... ) std::cerr << YELLOW BOLD "WARNING: " \
                                 << args << NORMAL << std::endl
#ifdef _DEBUG
  #define DEBUG_MESSAGE( args... ) std::cerr << YELLOW BOLD "DBG: " \
                                 << args << NORMAL << std::endl
#else
  #define DEBUG_MESSAGE( args... )
#endif

#endif // ifndef _MESSAGES_HPP
//================================== EOF ======================================
