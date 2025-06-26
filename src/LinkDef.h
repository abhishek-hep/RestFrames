/**
 * @file LinkDef.h
 * @brief File needed for ROOT dictionaries creation
 *
 */

#include "RestFrames/RestFrame.hh"

#ifdef __CINT__

#pragma extra_include "RestFrames/RestFrame.hh";
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ class RestFrame+;
#pragma link C++ class ROOT6_RestFramesHelperAutoloadHook+;

#endif
