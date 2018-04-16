/*****************************************************************************/
/*                                                                           */
/*!                    @brief Main-module of DOCFSM                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_docfsm.cpp                                                   */
/*! @see     df_docfsm.hpp                                                   */
/*! @author  Ulrich Becker                                                   */
/*! @date    11.11.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include <fstream>

#include "df_commandline.hpp"
#include "df_preparser.hpp"
#include "df_dotgenerator.hpp"
#include "df_transition_finder.hpp"
#include "df_cpp_caller.hpp"
#include "df_dot_keywords.hpp"
#include "df_docfsm.hpp"

using namespace DocFsm;

//=============================================================================

int main(int argc, char** ppArgv )
{
   std::string    out;
   std::stringbuf str;
   std::istream   inStream(&str);
   std::ostream   outStream(&str);

   try
   {
      CommandlineParser  oCommandlineParser( argc, ppArgv );
      KeywordPool        oKeywordPool( oCommandlineParser );
      CppCaller          cppCaller( outStream, oCommandlineParser );
      SourceBrowser      sourceBrowser( out );
      StateCollector     collector( sourceBrowser, oKeywordPool, oCommandlineParser );
      TransitionFinder   transitionFinder( collector, oCommandlineParser );
      DotKeywords        dotKeyWords( oCommandlineParser );

      if( oCommandlineParser() < 0 )
         return EXIT_FAILURE;

      if( oCommandlineParser.getFileNameList().empty() )
      {
         ERROR_MESSAGE( "No input source file(s) given!" );
         return EXIT_FAILURE;
      }

      if( oKeywordPool.setDefaultsIfEmpty() )
          return EXIT_FAILURE;

      for( auto& fileName : oCommandlineParser.getFileNameList() )
      {
         if( cppCaller.run( fileName ) )
            return EXIT_FAILURE;

         Preparser preparser( inStream, out );
         if( preparser() )
            return EXIT_FAILURE;

         if( collector( fileName ) )
            return EXIT_FAILURE;

         if( collector.isEmpty() )
            continue;

         if( transitionFinder() )
            return EXIT_FAILURE;
      }
      collector.splitInClusters();
      collector.splitInGroups();
      collector.generateTooltipFromLabel();
      collector.print( std::cout );
   }
   catch( ... )
   {
      ERROR_MESSAGE( "Exception: perhaps not enough memory!" );
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

//================================== EOF ======================================
