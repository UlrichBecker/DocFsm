/*****************************************************************************/
/*                                                                           */
/*!             @brief Handling and verifying keywords of DOT                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_dot_keywords.cpp                                             */
/*! @see     df_dot_keywords.hpp                                             */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include "df_docfsm.hpp"
#include "df_dot_keywords.hpp"

using namespace DocFsm;

/*!----------------------------------------------------------------------------
*/
const DotKeywords::DOT_ATTR_LIST_T DotKeywords::c_edgeAttributes =
{
   std::make_tuple("arrowhead",   "style of arrowhead at head end", NOTHING, DOT ),
   std::make_tuple("arrowsize",   "scaling factor for arrowheads", NOTHING, DOT ),
   std::make_tuple("arrowtail",   "style of arrowhead at tail end", NOTHING, DOT ),
   std::make_tuple("color",       "edge stroke color",COLOR, DOT ),
   std::make_tuple("colorscheme", "scheme for interpreting color names", NOTHING, DOT ),
   std::make_tuple("comment",     "any string (format-dependent)", NOTHING, DOT ),
   std::make_tuple("constraint",  "use edge to affect node ranking", NOTHING, DOT ),
   std::make_tuple("decorate",    "if set, draws a line connecting labels with their edges",NOTHING, DOT ),
   std::make_tuple("dir",         "forward, back, both, or none", NOTHING, DOT ),
   std::make_tuple("edgeURL",     "URL attached to non-label part of edge", NOTHING, DOT ),
   std::make_tuple("edgehref",    "synonym for edgeURL", NOTHING, DOT ),
   std::make_tuple("edgetarget",  "if URL is set, determines browser window for URL", NOTHING, DOT ),
   std::make_tuple("edgetooltip", "tooltip annotation for non-label part of edge", NOTHING, DOT ),
   std::make_tuple("fontcolor",   "type face color", FONTCOLOR, DOT ),  
   std::make_tuple("fontname",    "font family", NOTHING, DOT ),
   std::make_tuple("fontsize",    "point size of label", NOTHING, DOT ),
   std::make_tuple("headclip",    "if false, edge is not clipped to head node boundary", NOTHING, DOT ),
   std::make_tuple("headhref",    "synonym for headURL", NOTHING, DOT ),
   std::make_tuple("headlabel",   "label placed near head of edge", NOTHING, DOT ),
   std::make_tuple("headport",    "n,ne,e,se,s,sw,w,nw",NOTHING, DOT),
   std::make_tuple("headtarget",  "if headURL is set, determines browser window for URL", NOTHING, DOT ),
   std::make_tuple("headtooltip", "tooltip annotation near head of edge", NOTHING, DOT ),
   std::make_tuple("headURL",     "URL attached to head label", URL, DOT ),
   std::make_tuple("href",        "alias for URL", URL, DOT ),
   std::make_tuple("id",          "any string (user-defined output object tags)" ,NOTHING, DOT ),
   std::make_tuple("label",       "edge label", LABEL, DOT ),
   std::make_tuple("labelangle",  "angle in degrees which head or tail label is rotated off edge", NOTHING, DOT ),
   std::make_tuple("labeldistance","scaling factor for distance of head or tail label from node", NOTHING, DOT ),
   std::make_tuple("labelfloat",  "lessen constraints on edge label placement", NOTHING, DOT ),
   std::make_tuple("labelfontcolor","type face color for head and tail labels", NOTHING, DOT ),
   std::make_tuple("labelfontname","font family for head and tail labels", NOTHING, DOT ),
   std::make_tuple("labelfontsize","point size for head and tail labels", NOTHING, DOT ),
   std::make_tuple("labelhref",   "synonym for labelURL", NOTHING, DOT ),
   std::make_tuple("labelURL",    "URL for label, overrides edge URL", NOTHING, DOT ),
   std::make_tuple("labeltarget", "if URL or labelURL is set, determines browser window for URL", NOTHING, DOT ),
   std::make_tuple("labeltooltip","ooltip annotation near label", NOTHING, DOT ),
   std::make_tuple("layer",       "all, id or id:id, or a comma-separated list of the former", NOTHING, DOT ),
   std::make_tuple("lhead",       "name of cluster to use as head of edge", NOTHING, DOT ),
   std::make_tuple("ltail",       "name of cluster to use as tail of edge", NOTHING, DOT ),
   std::make_tuple("minlen",      "minimum rank distance between head and tail", NOTHING, DOT ),
   std::make_tuple("penwidth",    "width of pen for drawing edge stroke, in points", NOTHING, DOT ),
   std::make_tuple("samehead",    "tag for head node; edge heads with the same tag are merged onto the same port" ,NOTHING, DOT ),
   std::make_tuple("sametail",    "tag for tail node; edge tails with the same tag are merged onto the same port", NOTHING, DOT ),
   std::make_tuple("style",       "graphics options, e.g. bold, dotted, filled;", STYLE, DOT ),
   std::make_tuple("tailclip",    "if false, edge is not clipped to tail node boundary", NOTHING, DOT ),
   std::make_tuple("tailhref",    "synonym for tailURL", NOTHING, DOT ),
   std::make_tuple("taillabel",   "label placed near tail of edge", NOTHING, DOT ),
   std::make_tuple("tailport",    "n,ne,e,se,s,sw,w,nw", NOTHING, DOT ),
   std::make_tuple("tailtarget",  "if tailURL is set, determines browser window for URL", NOTHING, DOT ),
   std::make_tuple("tailtooltip", "tooltip annotation near tail of edge", NOTHING, DOT ),
   std::make_tuple("tailURL",     "URL attached to tail label", URL, DOT ),
   std::make_tuple("target",      "if URL is set, determines browser window for URL", NOTHING, DOT ),
   std::make_tuple("tooltip",     "tooltip annotation", NOTHING, DOT ),
   std::make_tuple("weight",      "integer cost of stretching an edge", NOTHING, DOT )
};

/*!----------------------------------------------------------------------------
*/
const DotKeywords::DOT_ATTR_LIST_T DotKeywords::c_nodeAttributes =
{
   std::make_tuple("color",       "node shape color", COLOR, DOT),
   std::make_tuple("colorscheme", "scheme for interpreting color names", NOTHING, DOT),
   std::make_tuple("comment",     "any string (format-dependent)", NOTHING, DOT),
   std::make_tuple("distortion",  "node distortion for shape=polygon", NOTHING, DOT),
   std::make_tuple("fillcolor",   "node fill color", FILLCOLOR, DOT),
   std::make_tuple("fixedsize",   "label text has no affect on node size", NOTHING, DOT),
   std::make_tuple("fontcolor",   "type face color", FONTCOLOR, DOT),
   std::make_tuple("fontname",    "font family", NOTHING, DOT),
   std::make_tuple("fontsize",    "point size of label", NOTHING, DOT), 
   std::make_tuple("group",       "name of node’s horizontal alignment group", NOTHING, DOT),
   std::make_tuple("height",      "minimum height in inches", NOTHING, DOT),
   std::make_tuple("id",          "any string (user-defined output object tags)", NOTHING, DOT),
   std::make_tuple("image",       "image file name", NOTHING, DOT),
   std::make_tuple("imagescale",  "true, width, height, both", NOTHING, DOT),
   std::make_tuple("label",       "any string", LABEL, DOT),
   std::make_tuple("labelloc",    "node label vertical alignment", NOTHING, DOT),
   std::make_tuple("layer",       "all, id or id:id, or a comma-separated list of the former", NOTHING, DOT),
   std::make_tuple("margin",      "space around labe", NOTHING, DOT),
   std::make_tuple("nojustify",   "if true, justify to label, not node", NOTHING, DOT),
   std::make_tuple("orientation", "node rotation angle", NOTHING, DOT),
   std::make_tuple("penwidth",    "width of pen for drawing boundaries, in points", NOTHING, DOT),
   std::make_tuple("peripheries", "number of node boundaries", NOTHING, DOT),
   std::make_tuple("regular",     "force polygon to be regular", NOTHING, DOT),
   std::make_tuple("samplepoints","number vertices to convert circle or ellipse", NOTHING, DOT),
   std::make_tuple("shape",       "node shape", SHAPE, DOT),
   std::make_tuple("sides",       "number of sides for shape=polygon", NOTHING, DOT),
   std::make_tuple("skew",        "skewing of node for shape=polygon", NOTHING, DOT),
   std::make_tuple("style",       "graphics options, e.g. bold, dotted, filled;", STYLE, DOT),
   std::make_tuple("target",      "if URL is set, determines browser window for URL", NOTHING, DOT),
   std::make_tuple("tooltip",     "tooltip annotation", NOTHING, DOT),
   std::make_tuple("URL",         "URL associated with node (format-dependent)", URL, DOT),
   std::make_tuple("width",       "minimum width in inches", NOTHING, DOT),

   std::make_tuple("GROUP",       "name of belonging group for this state, if grouping desired", GROUP, DOCFSM )
};

/*!----------------------------------------------------------------------------
*/
const DotKeywords::DOT_ATTR_LIST_T DotKeywords::c_graphAttributes =
{
   std::make_tuple("aspect",      "controls aspect ratio adjustment", NOTHING, DOT ),
   std::make_tuple("bgcolor",     "background color for drawing, plus initial fill color", NOTHING, DOT ),
   std::make_tuple("center",      "center drawing on page", NOTHING, DOT ),
   std::make_tuple("clusterrank", "may be global or none", NOTHING, DOT ),
   std::make_tuple("color",       "for clusters, outline color, and fill color if fillcolor not defined", COLOR, DOT ),
   std::make_tuple("colorscheme", "scheme for interpreting color names", NOTHING, DOT ),
   std::make_tuple("comment",     "any string (format-dependent)", NOTHING, DOT ),
   std::make_tuple("compound",    "allow edges between clusters", NOTHING, DOT ),
   std::make_tuple("concentrate", "enables edge concentrators", NOTHING, DOT ),
   std::make_tuple("dpi",         "dots per inch for image output", NOTHING, DOT ),
   std::make_tuple("fillcolor",   "cluster fill color", FILLCOLOR, DOT ),
   std::make_tuple("fontcolor",   "type face color", FONTCOLOR, DOT ),
   std::make_tuple("fontname",    "font family", NOTHING, DOT ),
   std::make_tuple("fontnames",   "svg, ps, gd (SVG only)", NOTHING, DOT ),
   std::make_tuple("fontpath",    "list of directories to search for fonts", NOTHING, DOT ),
   std::make_tuple("fontsize",    "point size of label", NOTHING, DOT ),
   std::make_tuple("id",          "any string (user-defined output object tags)", NOTHING, DOT ),
   std::make_tuple("label",       "any string", LABEL, DOT ),
   std::make_tuple("labeljust",   "\"l\" and \"r\" for left- and right-justified cluster labels, respectively", NOTHING, DOT ),
   std::make_tuple("labelloc",    "\"t\" and \"b\" for top- and bottom-justified cluster labels, respectively", NOTHING, DOT ),
   std::make_tuple("landscape",   "if true, means orientation=landscape", NOTHING, DOT ),
   std::make_tuple("layers",      "id:id:id...", NOTHING, DOT ),
   std::make_tuple("layersep",    "specifies separator character to split layers", NOTHING, DOT ),
   std::make_tuple("margin",      "margin included in page, inches", NOTHING, DOT ),
   std::make_tuple("mindist",     "minimum separation between all nodes (not dot)", NOTHING, DOT ),
   std::make_tuple("nodesep",     "separation between nodes, in inches.", NOTHING, DOT ),
   std::make_tuple("nojustify",   "if true, justify to label, not graph", NOTHING, DOT ),
   std::make_tuple("ordering",    "if out, out edge order is preserved", NOTHING, DOT ),
   std::make_tuple("orientation", "if rotate is not used and the value is landscape, use landscape orientation", NOTHING, DOT ),
   std::make_tuple("outputorder", "breadthfirst or nodesfirst, edgesfirst", NOTHING, DOT ),
   std::make_tuple("page",        "unit of pagination, e.g. \"8.5,11\"", NOTHING, DOT ),
   std::make_tuple("pagedir",     "traversal order of pages", NOTHING, DOT ),
   std::make_tuple("pencolor",    "color for drawing cluster boundaries" ,NOTHING, DOT ),
   std::make_tuple("penwidth",    "width of pen for drawing boundaries, in points", NOTHING, DOT ),
   std::make_tuple("peripheries", "number of cluster boundaries", NOTHING, DOT),
   std::make_tuple("rank",        "same, min, max, source or sink", NOTHING, DOT),
   std::make_tuple("rankdir",     "LR (left to right) or TB (top to bottom)", NOTHING, DOT ),
   std::make_tuple("ranksep",     "separation between ranks, in inches.", NOTHING, DOT),
   std::make_tuple("ratio",       "approximate aspect ratio desired, fill or auto minimization", NOTHING, DOT),
   std::make_tuple("rotate",      "If 90, set orientation to landscape", NOTHING, DOT),
   std::make_tuple("samplepoints","number of points used to represent ellipses and circles on output", NOTHING, DOT),
   std::make_tuple("searchsize",  "maximum edges with negative cut values to check when looking for a minimum one during network simplex", NOTHING, DOT),
   std::make_tuple("size",        "maximum drawing size, in inches", NOTHING, DOT),
   std::make_tuple("splines",     "draw edges as splines, polylines, lines", NOTHING, DOT ),
   std::make_tuple("style",       "graphics options, e.g. filled for clusters", STYLE, DOT ),
   std::make_tuple("stylesheet",  "pathname or URL to XML style sheet for SVG", NOTHING, DOT ),
   std::make_tuple("target",      "if URL is set, determines browser window for URL", NOTHING, DOT ),
   std::make_tuple("tooltip",     "tooltip annotation for cluster", NOTHING, DOT ),
   std::make_tuple("truecolor",   "if set, force 24 bit or indexed color in image output", NOTHING, DOT ),
   std::make_tuple("viewport",    "clipping window on output", NOTHING, DOT ),
   std::make_tuple("URL",         "URL associated with graph (format-dependent)", URL, DOT )
};

/*!----------------------------------------------------------------------------
*/
DotKeywords::OptionListNodeWords::OptionListNodeWords( DotKeywords* pParent )
   :DotKeywordsOption( pParent )
{
   m_longOpt  = "lNodes";
   m_helpText = "List all supported keywords of node-attributes and exit.\n"
                "Note if the option -v input at first so the short description"
                " will also shown.";
}

/*!----------------------------------------------------------------------------
*/
int DotKeywords::OptionListNodeWords::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->list( c_nodeAttributes, 
                    static_cast<CommandlineParser*>(poParser)->isVerbose() );
   ::exit( EXIT_SUCCESS );
   return 0;
}

/*!----------------------------------------------------------------------------
*/
DotKeywords::OptionListEdgeWords::OptionListEdgeWords( DotKeywords* pParent )
   :DotKeywordsOption( pParent )
{
   m_longOpt  = "lEdges";
   m_helpText = "List all supported keywords of edge-attributes and exit\n"
                "Note if the option -v input at first so the short description"
                " will also shown.";
}

/*!----------------------------------------------------------------------------
*/
int DotKeywords::OptionListEdgeWords::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->list( c_edgeAttributes, 
                    static_cast<CommandlineParser*>(poParser)->isVerbose() );
   ::exit( EXIT_SUCCESS );
   return 0;
}

/*!----------------------------------------------------------------------------
*/
DotKeywords::OptionListGraphWords::OptionListGraphWords( DotKeywords* pParent )
   :DotKeywordsOption( pParent )
{
   m_longOpt  = "lGraph";
   m_helpText = "List all supported keywords of principal graph attributes and exit.\n"
                "Note if the option -v input at first so the short description"
                " will also shown.";
}

/*!----------------------------------------------------------------------------
*/
int DotKeywords::OptionListGraphWords::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->list( c_graphAttributes, 
                    static_cast<CommandlineParser*>(poParser)->isVerbose() );
   ::exit( EXIT_SUCCESS );
   return 0;
}

////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
DotKeywords::DotKeywords( CommandlineParser& rCommandlineParser )
   :m_oOptionListNodeWords( this )
   ,m_oOptionListEdgeWords( this )
   ,m_oOptionListGraphWords( this )
{
   rCommandlineParser( m_oOptionListNodeWords )
                     ( m_oOptionListEdgeWords )
                     ( m_oOptionListGraphWords);
}

/*!----------------------------------------------------------------------------
*/
const DotKeywords::DOT_ATTR_ITEM_T* DotKeywords::find( const DOT_ATTR_LIST_T& rList,
                                                       const std::string& word )
{
   for( auto& item : rList )
   {
      if( getKeyWord( item ) == word )
         return &item;
   }
   return nullptr;
}

/*!----------------------------------------------------------------------------
*/
void DotKeywords::list( const DOT_ATTR_LIST_T& rList, bool verbose )
{
   for( auto& item : rList )
   {
      if( verbose )
          std::cout << BOLD;
      std::cout << getKeyWord( item );
      if( verbose )
      {
         for( int i = getKeyWord( item ).size(); i < 20; i++ )
             std::cout << ' ';
         std::cout << NORMAL << getHelpText( item );
      }
      std::cout << std::endl;
   }
}

//================================== EOF ======================================
