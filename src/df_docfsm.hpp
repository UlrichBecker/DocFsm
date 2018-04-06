/*****************************************************************************/
/*                                                                           */
/*!                    @brief Main-module of DOC-FSM                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_docfsm.hpp                                                   */
/*! @see     df_docfsm.cpp                                                   */
/*! @author  Ulrich Becker                                                   */
/*! @date    11.11.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _CSTATE_HPP
#define _CSTATE_HPP

#include "messages.hpp"

#ifdef TO_STRING_LITERAL
   #undef TO_STRING_LITERAL
#endif
#ifdef TO_STRING
   #undef TO_STRING
#endif
#define TO_STRING_LITERAL( s ) # s
#define TO_STRING( s ) TO_STRING_LITERAL( s )

#define DOCFSM_VERSION TO_STRING( __DOCFSM_VERSION__ )


#endif // ifndef _DF_CSTATE_HPP
//================================== EOF ======================================
