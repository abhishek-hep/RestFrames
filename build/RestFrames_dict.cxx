// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME RestFrames_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "RestFrames/RestFrame.hh"
#include "RestFrames/RestFrame.hh"

// Header files passed via #pragma extra_include
#include "RestFrames/RestFrame.hh"

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace {
  void TriggerDictionaryInitialization_libRestFrames_Impl() {
    static const char* headers[] = {
"RestFrames/RestFrame.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/abhishek/RF_dev/RestFrames",
"/home/abhishek/RF_dev/RestFrames",
"/home/abhishek/root/include",
"/home/abhishek/root/include/",
"/home/abhishek/RF_dev/RestFrames/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libRestFrames dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRestFrames dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RestFrames/RestFrame.hh"
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

// Extra includes
#include "RestFrames/RestFrame.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRestFrames",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRestFrames_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRestFrames_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRestFrames() {
  TriggerDictionaryInitialization_libRestFrames_Impl();
}
