// -*- C++ -*-

//=============================================================================
/**
 *  @file    Container_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Components::Deployment::Container interface.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_CONTAINER_IMPL_H
#define CIAO_CONTAINER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Deployment_CoreS.h"
#include "Container_Base.h"
#include "Swapping_Container.h"
#include "ace/SString.h"
#include "Server_init.h"  // write_IOR function
#include "CIAO_common.h" // CIAO::debug_level

namespace CIAO
{
  /**
   * @class Container_Impl
   *
   * @brief Servant implementation for the interface Deployment::Container
   *
   * This class implements the Deployment::Container
   * interface which is not defined by the CCM DnC specification. 
   * As the interface implies, this is actually part of the deployment 
   * interface and is used to manage the lifecycle of the installed 
   * components and homes.
   */
  class CIAO_SERVER_Export Container_Impl
    : public virtual POA_Deployment::Container,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    Container_Impl (CORBA::ORB_ptr o,
                    PortableServer::POA_ptr p,
                    ::Deployment::NodeApplication_ptr server);

    /// Destructor
    virtual ~Container_Impl (void);

    /*-------------------------------------------------------------*/
    /*--------------------  IDL operations (idl) ------------------*/

    /// Initialize the container.
    virtual CORBA::Long init (const ::Deployment::Properties &properties
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Install all homes and components
    Deployment::ComponentInfos * 
      install (const ::Deployment::ContainerImplementationInfo & container_impl_info
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Deployment::UnknownImplId,
                         Deployment::ImplEntryPointNotFound,
                         Deployment::InstallationFailure,
                         Components::InvalidConfiguration));

    /// Remove all homes and components
    virtual void 
      remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::RemoveFailure));

    /// Deployment::Container interface defined attributes/operations.
    virtual ::Deployment::Properties * 
      properties (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Deployment::NodeApplication_ptr 
      get_node_application (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    /*-------------------------------------------------------------*/
    /*-------------------  C++ help methods (c++) -----------------*/

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);


    // Install the home of this particular component
    virtual ::Components::CCMHome_ptr 
      install_home (const ::Deployment::ComponentImplementationInfo & impl_info
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Deployment::UnknownImplId,
                         Deployment::ImplEntryPointNotFound,
                         Deployment::InstallationFailure,
                         Components::InvalidConfiguration));

    /**
     * @@Note: I don't know how to remove a home right now.
     *         I assume that user will only call remove instead.
     *         This is true at least for DnC run time.
     *
     * Right now, in this implementation I assumpe that there will be
     * same number of homes as the components even if the components
     * are of the same type. I don't think that we have the modeling
     * side support of this either. So bear me if you think I avoid
     * the real thinking for easiness.
     */
    // Remove the home of this particular component 
    virtual void 
      remove_home (const char * comp_ins_name
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Components::RemoveFailure));

    // Get all homes
    virtual ::Components::CCMHomes * 
      get_homes (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));


    // ------------------- CIAO Internal Operations ------------------------
    // These below two are helper methods to clean up components
    // should only be called when we are sure that there is no
    // active connection on this component.
    virtual void remove_components (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual void remove_component (const char * comp_ins_name
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));    
    
    /// Set the cached object reference.
    void set_objref (Deployment::Container_ptr o
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Get the cached object reference.  This operation will invoke
    /// _this if there's no cached reference available.  Notice that
    /// this method does *NOT* increase the reference count of the
    /// cached reference.
    Deployment::Container_ptr get_objref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

protected:
    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Internal container implementation.
    CIAO::Container *container_;

    /// Cached ConfigValues.
    Deployment::Properties properties_;

    /// Cached Container reference (of ourselves.)
    Deployment::Container_var objref_;

    /// Cached NodeApplication.
    Deployment::NodeApplication_var nodeapp_;

    /// To store all created CCMHome object
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMHome_ptr,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMHome_Map;
    typedef CCMHome_Map::iterator Home_Iterator;
    CCMHome_Map home_map_;

    /// To sotre all created Component object.
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMObject_ptr,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> CCMComponent_Map;
    typedef CCMComponent_Map::iterator Component_Iterator;
    CCMComponent_Map component_map_;
  };
}

#if defined (__ACE_INLINE__)
# include "Container_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_CONTAINER_IMPL_H */
