// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be\be_codegen.cpp:302


#include "MIF_SchedulingC.h"
#include "tao/CDR.h"
#include "tao/Null_RefCount_Policy.h"
#include "tao/TypeCode_Constants.h"
#include "tao/Alias_TypeCode.h"
#include "tao/Objref_TypeCode.h"
#include "tao/String_TypeCode.h"
#include "tao/Any_Impl_T.h"
#include "tao/Basic_Arguments.h"
#include "ace/OS_NS_string.h"

#if defined (__BORLANDC__)
#pragma option -w-rvl -w-rch -w-ccc -w-aus -w-sig
#endif /* __BORLANDC__ */

// TAO_IDL - Generated from
// be\be_visitor_arg_traits.cpp:69

// Arg traits specializations.
namespace TAO
{
}


// TAO_IDL - Generated from
// be\be_visitor_interface/interface_cs.cpp:60

// Traits specializations for MIF_Scheduling::SegmentSchedulingParameterPolicy.

MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
TAO::Objref_Traits<MIF_Scheduling::SegmentSchedulingParameterPolicy>::duplicate (
    MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr p
  )
{
  return MIF_Scheduling::SegmentSchedulingParameterPolicy::_duplicate (p);
}

void
TAO::Objref_Traits<MIF_Scheduling::SegmentSchedulingParameterPolicy>::release (
    MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr p
  )
{
  CORBA::release (p);
}

MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
TAO::Objref_Traits<MIF_Scheduling::SegmentSchedulingParameterPolicy>::nil (void)
{
  return MIF_Scheduling::SegmentSchedulingParameterPolicy::_nil ();
}

CORBA::Boolean
TAO::Objref_Traits<MIF_Scheduling::SegmentSchedulingParameterPolicy>::marshal (
    MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr p,
    TAO_OutputCDR & cdr
  )
{
  return CORBA::Object::marshal (p, cdr);
}

MIF_Scheduling::SegmentSchedulingParameterPolicy::SegmentSchedulingParameterPolicy (void)
{}

MIF_Scheduling::SegmentSchedulingParameterPolicy::~SegmentSchedulingParameterPolicy (void)
{}

void
MIF_Scheduling::SegmentSchedulingParameterPolicy::_tao_any_destructor (void *_tao_void_pointer)
{
  SegmentSchedulingParameterPolicy *_tao_tmp_pointer =
    static_cast<SegmentSchedulingParameterPolicy *> (_tao_void_pointer);
  CORBA::release (_tao_tmp_pointer);
}

MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
MIF_Scheduling::SegmentSchedulingParameterPolicy::_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (_tao_objref))
    {
      return SegmentSchedulingParameterPolicy::_nil ();
    }

  SegmentSchedulingParameterPolicy_ptr proxy =
    dynamic_cast<SegmentSchedulingParameterPolicy_ptr> (_tao_objref);

  return SegmentSchedulingParameterPolicy::_duplicate (proxy);
}

MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
MIF_Scheduling::SegmentSchedulingParameterPolicy::_unchecked_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (_tao_objref))
    {
      return SegmentSchedulingParameterPolicy::_nil ();
    }

  SegmentSchedulingParameterPolicy_ptr proxy =
    dynamic_cast<SegmentSchedulingParameterPolicy_ptr> (_tao_objref);

  return SegmentSchedulingParameterPolicy::_duplicate (proxy);
}

MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
MIF_Scheduling::SegmentSchedulingParameterPolicy::_duplicate (SegmentSchedulingParameterPolicy_ptr obj)
{
  if (! CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }

  return obj;
}

void
MIF_Scheduling::SegmentSchedulingParameterPolicy::_tao_release (SegmentSchedulingParameterPolicy_ptr obj)
{
  CORBA::release (obj);
}

CORBA::Boolean
MIF_Scheduling::SegmentSchedulingParameterPolicy::_is_a (
    const char *value
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Policy:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:MIF_Scheduling/SegmentSchedulingParameterPolicy:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/LocalObject:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        )
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return false;
    }
}

const char* MIF_Scheduling::SegmentSchedulingParameterPolicy::_interface_repository_id (void) const
{
  return "IDL:MIF_Scheduling/SegmentSchedulingParameterPolicy:1.0";
}

CORBA::Boolean
MIF_Scheduling::SegmentSchedulingParameterPolicy::marshal (TAO_OutputCDR &)
{
  return false;
}

// TAO_IDL - Generated from
// be\be_visitor_typecode/objref_typecode.cpp:73

static TAO::TypeCode::Objref<char const *,
                             CORBA::tk_local_interface,
                             TAO::Null_RefCount_Policy>
  _tao_tc_MIF_Scheduling_SegmentSchedulingParameterPolicy (
    "IDL:MIF_Scheduling/SegmentSchedulingParameterPolicy:1.0",
    "SegmentSchedulingParameterPolicy");
  
namespace MIF_Scheduling
{
  ::CORBA::TypeCode_ptr const _tc_SegmentSchedulingParameterPolicy =
    &_tao_tc_MIF_Scheduling_SegmentSchedulingParameterPolicy;
}



// TAO_IDL - Generated from
// be\be_visitor_interface/interface_cs.cpp:60

// Traits specializations for MIF_Scheduling::MIF_Scheduler.

MIF_Scheduling::MIF_Scheduler_ptr
TAO::Objref_Traits<MIF_Scheduling::MIF_Scheduler>::duplicate (
    MIF_Scheduling::MIF_Scheduler_ptr p
  )
{
  return MIF_Scheduling::MIF_Scheduler::_duplicate (p);
}

void
TAO::Objref_Traits<MIF_Scheduling::MIF_Scheduler>::release (
    MIF_Scheduling::MIF_Scheduler_ptr p
  )
{
  CORBA::release (p);
}

MIF_Scheduling::MIF_Scheduler_ptr
TAO::Objref_Traits<MIF_Scheduling::MIF_Scheduler>::nil (void)
{
  return MIF_Scheduling::MIF_Scheduler::_nil ();
}

CORBA::Boolean
TAO::Objref_Traits<MIF_Scheduling::MIF_Scheduler>::marshal (
    MIF_Scheduling::MIF_Scheduler_ptr p,
    TAO_OutputCDR & cdr
  )
{
  return CORBA::Object::marshal (p, cdr);
}

MIF_Scheduling::MIF_Scheduler::MIF_Scheduler (void)
{}

MIF_Scheduling::MIF_Scheduler::~MIF_Scheduler (void)
{}

void 
MIF_Scheduling::MIF_Scheduler::_tao_any_destructor (void *_tao_void_pointer)
{
  MIF_Scheduler *_tao_tmp_pointer =
    static_cast<MIF_Scheduler *> (_tao_void_pointer);
  CORBA::release (_tao_tmp_pointer);
}

MIF_Scheduling::MIF_Scheduler_ptr
MIF_Scheduling::MIF_Scheduler::_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (_tao_objref))
    {
      return MIF_Scheduler::_nil ();
    }
  
  MIF_Scheduler_ptr proxy =
    dynamic_cast<MIF_Scheduler_ptr> (_tao_objref);
  
  return MIF_Scheduler::_duplicate (proxy);
}

MIF_Scheduling::MIF_Scheduler_ptr
MIF_Scheduling::MIF_Scheduler::_unchecked_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (_tao_objref))
    {
      return MIF_Scheduler::_nil ();
    }
  
  MIF_Scheduler_ptr proxy =
    dynamic_cast<MIF_Scheduler_ptr> (_tao_objref);
  
  return MIF_Scheduler::_duplicate (proxy);
}

MIF_Scheduling::MIF_Scheduler_ptr
MIF_Scheduling::MIF_Scheduler::_duplicate (MIF_Scheduler_ptr obj)
{
  if (! CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }
  
  return obj;
}

void
MIF_Scheduling::MIF_Scheduler::_tao_release (MIF_Scheduler_ptr obj)
{
  CORBA::release (obj);
}

CORBA::Boolean
MIF_Scheduling::MIF_Scheduler::_is_a (
    const char *value
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (
      !ACE_OS::strcmp (
          value,
          "IDL:RTScheduling/Scheduler:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:MIF_Scheduling/MIF_Scheduler:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/LocalObject:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        )
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return false;
    }
}

const char* MIF_Scheduling::MIF_Scheduler::_interface_repository_id (void) const
{
  return "IDL:MIF_Scheduling/MIF_Scheduler:1.0";
}

CORBA::Boolean
MIF_Scheduling::MIF_Scheduler::marshal (TAO_OutputCDR &)
{
  return false;
}

// TAO_IDL - Generated from
// be\be_visitor_typecode/objref_typecode.cpp:73

static TAO::TypeCode::Objref<char const *,
                             CORBA::tk_local_interface,
                             TAO::Null_RefCount_Policy>
  _tao_tc_MIF_Scheduling_MIF_Scheduler (
    "IDL:MIF_Scheduling/MIF_Scheduler:1.0",
    "MIF_Scheduler");
  
namespace MIF_Scheduling
{
  ::CORBA::TypeCode_ptr const _tc_MIF_Scheduler =
    &_tao_tc_MIF_Scheduling_MIF_Scheduler;
}



// TAO_IDL - Generated from
// be\be_visitor_interface/any_op_cs.cpp:50

template<>
CORBA::Boolean
TAO::Any_Impl_T<MIF_Scheduling::SegmentSchedulingParameterPolicy>::to_object (
    CORBA::Object_ptr &_tao_elem
  ) const
{
  _tao_elem = CORBA::Object::_duplicate (this->value_);
  return 1;
}

template<>
CORBA::Boolean
TAO::Any_Impl_T<MIF_Scheduling::SegmentSchedulingParameterPolicy>::marshal_value (TAO_OutputCDR &)
{
  return false;
}

template<>
CORBA::Boolean
TAO::Any_Impl_T<MIF_Scheduling::SegmentSchedulingParameterPolicy>::demarshal_value (TAO_InputCDR &)
{
  return false;
}

// Copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr _tao_elem
  )
{
  MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr _tao_objptr =
    MIF_Scheduling::SegmentSchedulingParameterPolicy::_duplicate (_tao_elem);
  _tao_any <<= &_tao_objptr;
}

// Non-copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr *_tao_elem
  )
{
  TAO::Any_Impl_T<MIF_Scheduling::SegmentSchedulingParameterPolicy>::insert (
      _tao_any,
      MIF_Scheduling::SegmentSchedulingParameterPolicy::_tao_any_destructor,
      MIF_Scheduling::_tc_SegmentSchedulingParameterPolicy,
      *_tao_elem
    );
}

CORBA::Boolean
operator>>= (
    const CORBA::Any &_tao_any,
    MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr &_tao_elem
  )
{
  return
    TAO::Any_Impl_T<MIF_Scheduling::SegmentSchedulingParameterPolicy>::extract (
        _tao_any,
        MIF_Scheduling::SegmentSchedulingParameterPolicy::_tao_any_destructor,
        MIF_Scheduling::_tc_SegmentSchedulingParameterPolicy,
        _tao_elem
      );
}

// TAO_IDL - Generated from
// be\be_visitor_interface/any_op_cs.cpp:50

template<>
CORBA::Boolean
TAO::Any_Impl_T<MIF_Scheduling::MIF_Scheduler>::to_object (
    CORBA::Object_ptr &_tao_elem
  ) const
{
  _tao_elem = CORBA::Object::_duplicate (this->value_);
  return 1;
}

template<>
CORBA::Boolean
TAO::Any_Impl_T<MIF_Scheduling::MIF_Scheduler>::marshal_value (TAO_OutputCDR &)
{
  return false;
}

template<>
CORBA::Boolean
TAO::Any_Impl_T<MIF_Scheduling::MIF_Scheduler>::demarshal_value (TAO_InputCDR &)
{
  return false;
}

// Copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    MIF_Scheduling::MIF_Scheduler_ptr _tao_elem
  )
{
  MIF_Scheduling::MIF_Scheduler_ptr _tao_objptr =
    MIF_Scheduling::MIF_Scheduler::_duplicate (_tao_elem);
  _tao_any <<= &_tao_objptr;
}

// Non-copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    MIF_Scheduling::MIF_Scheduler_ptr *_tao_elem
  )
{
  TAO::Any_Impl_T<MIF_Scheduling::MIF_Scheduler>::insert (
      _tao_any,
      MIF_Scheduling::MIF_Scheduler::_tao_any_destructor,
      MIF_Scheduling::_tc_MIF_Scheduler,
      *_tao_elem
    );
}

CORBA::Boolean
operator>>= (
    const CORBA::Any &_tao_any,
    MIF_Scheduling::MIF_Scheduler_ptr &_tao_elem
  )
{
  return
    TAO::Any_Impl_T<MIF_Scheduling::MIF_Scheduler>::extract (
        _tao_any,
        MIF_Scheduling::MIF_Scheduler::_tao_any_destructor,
        MIF_Scheduling::_tc_MIF_Scheduler,
        _tao_elem
      );
}
