// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME LTEvent_Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "LTEvent.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_LTEvent(void *p = 0);
   static void *newArray_LTEvent(Long_t size, void *p);
   static void delete_LTEvent(void *p);
   static void deleteArray_LTEvent(void *p);
   static void destruct_LTEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LTEvent*)
   {
      ::LTEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LTEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("LTEvent", ::LTEvent::Class_Version(), "LTEvent.hxx", 17,
                  typeid(::LTEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LTEvent::Dictionary, isa_proxy, 4,
                  sizeof(::LTEvent) );
      instance.SetNew(&new_LTEvent);
      instance.SetNewArray(&newArray_LTEvent);
      instance.SetDelete(&delete_LTEvent);
      instance.SetDeleteArray(&deleteArray_LTEvent);
      instance.SetDestructor(&destruct_LTEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LTEvent*)
   {
      return GenerateInitInstanceLocal((::LTEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::LTEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr LTEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *LTEvent::Class_Name()
{
   return "LTEvent";
}

//______________________________________________________________________________
const char *LTEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LTEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int LTEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LTEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LTEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LTEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LTEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LTEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void LTEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class LTEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(LTEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(LTEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LTEvent(void *p) {
      return  p ? new(p) ::LTEvent : new ::LTEvent;
   }
   static void *newArray_LTEvent(Long_t nElements, void *p) {
      return p ? new(p) ::LTEvent[nElements] : new ::LTEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_LTEvent(void *p) {
      delete ((::LTEvent*)p);
   }
   static void deleteArray_LTEvent(void *p) {
      delete [] ((::LTEvent*)p);
   }
   static void destruct_LTEvent(void *p) {
      typedef ::LTEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LTEvent

namespace {
  void TriggerDictionaryInitialization_LTEvent_Dict_Impl() {
    static const char* headers[] = {
"LTEvent.hxx",
0
    };
    static const char* includePaths[] = {
"/usr/local/root/include",
"/.",
"/usr/local/root-6.08.00-build/include",
"/home/nmcfadde/Legend/LRoot/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "LTEvent_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$LTEvent.hxx")))  LTEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "LTEvent_Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "LTEvent.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"LTEvent", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("LTEvent_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_LTEvent_Dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_LTEvent_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_LTEvent_Dict() {
  TriggerDictionaryInitialization_LTEvent_Dict_Impl();
}
