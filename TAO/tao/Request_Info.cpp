// $Id$


//========================================================================
//
// = LIBRARY
//     TAO/tao/
//
// = FILENAME
//     Request_Info.h
//
// = DESCRIPTION
//     This is the implementation of the RequestInfo interface of the
//   Portable Interceptor specification.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=========================================================================

#include "Request_Info.h"
#include "tao/corba.h"

ACE_RCSID(tao, request_info, "$Id$")

TAO_ClientRequest_Info::TAO_ClientRequest_Info (const char * operation,
                                        IOP::ServiceContextList &service_context_list ,               
                                        CORBA::Object * target,
                                        CORBA::Environment &)
  : operation_ (operation), 
    service_context_list_ (service_context_list),
    target_ (CORBA_Object::_duplicate (target))
{
}

CORBA::Object_ptr 
TAO_ClientRequest_Info::target (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->target_.in ();
}

CORBA::Object_ptr  TAO_ClientRequest_Info::effective_target (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

IOP::TaggedProfile *  
TAO_ClientRequest_Info::effective_profile (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any *  
TAO_ClientRequest_Info::received_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The spec says that if it is a user exception which cant be inserted
  // then the UNKNOWN exception needs to be thrown with minor code TBD_U.
  this->any_exception_ <<= *this->caught_exception_;
  return &this->any_exception_;
}
  
char *  
TAO_ClientRequest_Info::received_exception_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // constness had to be casted since <_id> which is a TAO-specific
  // extension of CORBA::Exception returns a const char *.
  return ACE_const_cast (char *, 
                         this->caught_exception_->_id ());
}

IOP::TaggedComponent *  
TAO_ClientRequest_Info::get_effective_component (IOP::ComponentId,
                                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}
 
CORBA::Policy_ptr  
TAO_ClientRequest_Info::get_request_policy (CORBA::PolicyType,
                                            CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void  
TAO_ClientRequest_Info::add_request_service_context (const IOP::ServiceContext & service_context,
                                                     CORBA::Boolean,
                                                     CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  // Copy the service context into the list.
  CORBA::ULong length = this->service_context_list_.length ();
  this->service_context_list_.length (length + 1);

  service_context_list_[length] = service_context;
}

CORBA::ULong 
TAO_ClientRequest_Info::request_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_id_;
}

char * 
TAO_ClientRequest_Info::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ACE_const_cast (char *, 
                         this->operation_);

}

Dynamic::ParameterList * 
TAO_ClientRequest_Info::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->parameter_list_;
}

Dynamic::ExceptionList * 
TAO_ClientRequest_Info::exceptions (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &this->exception_list_;
}

Dynamic::ContextList * 
TAO_ClientRequest_Info::contexts (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->context_list_;
}

Dynamic::RequestContext * 
TAO_ClientRequest_Info::operation_context (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->request_context_;
}

CORBA::Any * 
TAO_ClientRequest_Info::result (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean 
TAO_ClientRequest_Info::response_expected (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

Messaging::SyncScope 
TAO_ClientRequest_Info::sync_scope (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

PortableInterceptor::ReplyStatus 
TAO_ClientRequest_Info::reply_status (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Object_ptr 
TAO_ClientRequest_Info::forward_reference (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any * 
TAO_ClientRequest_Info::get_slot (PortableInterceptor::SlotId,
                                  CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  return 0;
}

IOP::ServiceContext * 
TAO_ClientRequest_Info::get_request_service_context (IOP::ServiceId id,
                                                     CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
         return &(this->service_context_list_[size]);
        
      }
return 0;
}

IOP::ServiceContext * 
TAO_ClientRequest_Info::get_reply_service_context (IOP::ServiceId id,
                            CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
        //Will it will get safely destroyed at the
        // TAO_Client side when the service context gets back.
        return &this->service_context_list_[size];
        
      }
return 0;
}

void
TAO_ClientRequest_Info::exception (CORBA::Exception *exception)
{
  this->caught_exception_ = exception;
}

void
TAO_ClientRequest_Info::request_id (CORBA::ULong request_id)
{
  this->request_id_ = request_id;
}

//****************************************************************               

/* 
TAO_ServerRequest_Info::TAO_ServerRequest_Info(CORBA::ULong request_id,
                                       char * operation, 
                                       Dynamic::ParameterList * arguments,
                                       Dynamic::ExceptionList * exceptions,
                                       Dynamic::ContextList * contexts,
                                       Dynamic::RequestContext * operation_context,
                                       CORBA::Object_ptr forward_reference,
                                       PortableInterceptor::OctetSeq *object_id,
                                       PortableInterceptor::OctetSeq *adapter_id,
                                       CORBA::Environment &)
  : Request_Info (request_id, operation, arguments, exceptions, contexts, operation_context, forward_reference),
    object_id_ (object_id),
    adapter_id_ (adapter_id)
{
}
*/
TAO_ServerRequest_Info::TAO_ServerRequest_Info (const char * operation,
                                        IOP::ServiceContextList &service_context_list ,               
                                        CORBA::Environment &)
  : operation_ (operation), 
    service_context_list_ (service_context_list)
{
}


CORBA::ULong 
TAO_ServerRequest_Info::request_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_id_;
}

char * 
TAO_ServerRequest_Info::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ACE_const_cast (char *, 
                         this->operation_);
}

Dynamic::ParameterList * 
TAO_ServerRequest_Info::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->parameter_list_;
}

Dynamic::ExceptionList * 
TAO_ServerRequest_Info::exceptions (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &this->exception_list_;
}

Dynamic::ContextList * 
TAO_ServerRequest_Info::contexts (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->context_list_;
}

Dynamic::RequestContext * 
TAO_ServerRequest_Info::operation_context (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->request_context_;
}

CORBA::Any * 
TAO_ServerRequest_Info::result (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean 
TAO_ServerRequest_Info::response_expected (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

Messaging::SyncScope 
TAO_ServerRequest_Info::sync_scope (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

PortableInterceptor::ReplyStatus 
TAO_ServerRequest_Info::reply_status (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Object_ptr 
TAO_ServerRequest_Info::forward_reference (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any * 
TAO_ServerRequest_Info::get_slot (PortableInterceptor::SlotId,
                                  CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  return 0;
}

IOP::ServiceContext * 
TAO_ServerRequest_Info::get_request_service_context (IOP::ServiceId id,
                                                     CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    {
      if (this->service_context_list_[size].context_id == id)
        return &(this->service_context_list_[size]);
    }
return 0;
}

IOP::ServiceContext * 
TAO_ServerRequest_Info::get_reply_service_context (IOP::ServiceId id,
                                                   CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    {
      if (this->service_context_list_[size].context_id == id)
        return &this->service_context_list_[size];
    }
  return 0;
}

CORBA::Any * 
TAO_ServerRequest_Info::sending_exception (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The spec says that if it is a user exception which cant be inserted
  // then the UNKNOWN exception needs to be thrown with minor code TBD_U.
  this->any_exception_ <<= *this->caught_exception_;
  return &this->any_exception_;
}
  
PortableInterceptor::OctetSeq * 
TAO_ServerRequest_Info::object_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0; 
}

PortableInterceptor::OctetSeq * 
TAO_ServerRequest_Info::adapter_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

char * 
TAO_ServerRequest_Info::target_most_derived_interface (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

CORBA::Policy_ptr 
TAO_ServerRequest_Info::get_server_policy (CORBA::PolicyType type,
                                      CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{ 
  return 0;
}

void 
TAO_ServerRequest_Info::set_slot (PortableInterceptor::SlotId,
                                  const CORBA::Any &,
                                  CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
}

CORBA::Boolean 
TAO_ServerRequest_Info::target_is_a (const char *,
                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void 
TAO_ServerRequest_Info::add_reply_service_context (const IOP::ServiceContext & service_context,
                                                   CORBA::Boolean,
                                                   CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong length = this->service_context_list_.length ();
  this->service_context_list_.length (length + 1);
  service_context_list_[length] = service_context;
}

void
TAO_ServerRequest_Info::exception (CORBA::Exception *exception)
{
  this->caught_exception_ = exception;
}

void
TAO_ServerRequest_Info::request_id (CORBA::ULong request_id)
{
  this->request_id_ = request_id;
}
