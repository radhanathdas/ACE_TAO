// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v1.8.3
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/
// test use multiple and simplex porttypes side by side.
#include "UsesSM_Sender_exec.h"
#include "UsesSMA_conn_i.h"

namespace CIAO_UsesSM_Sender_Impl
{
  CORBA::Boolean asynch = false;

  //============================================================
  // Worker thread for asynchronous invocations for One (multiple)
  // and Two (simplex)
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::UsesSM::CCM_Sender_Context_ptr context,
     Atomic_UShort  &nr_of_received,
     Atomic_UShort  &nr_of_sent)
  : context_(::UsesSM::CCM_Sender_Context::_duplicate (context)),
    nr_of_received_(nr_of_received),
    nr_of_sent_(nr_of_sent)
  {
  }

  int asynch_foo_generator::svc ()
  {
      // multiple
    ::UsesSM::Sender::sendc_run_my_um_oneConnections_var my_one_ami_ =
      context_->get_connections_sendc_run_my_um_one();
    // simplex
    ::UsesSM::AMI4CCM_Two_var my_two_ami_ =
      context_->get_connection_sendc_run_my_two();

    if (my_one_ami_->length () == 0)
      {
        ACE_ERROR ((LM_ERROR,
                   "ERROR Sender (ASYNCH) :my_one_ami_ is NIL !\n"));
        return 1;
      }
    if (CORBA::is_nil (my_two_ami_))
      {
        ACE_ERROR ((LM_ERROR,
                   "ERROR Sender (ASYNCH) :my_two_ami_ is NIL !\n"));
        return 1;
      }
    // Invoke Asynchronous calls to test
    for (CORBA::ULong i = 0; i < my_one_ami_->length (); ++i)
      {
        ::UsesSM::AMI4CCM_OneReplyHandler_var cb_one =
          new CIAO_UsesSM_AMI4CCM_One_Connector_AMI4CCM_Connector_Impl::AMI4CCM_OneReplyHandler_i (
                                                   this->nr_of_received_,
                                                   this->nr_of_sent_);

        CORBA::String_var test;
        switch (i)
          {
            case 0:
              test = CORBA::string_dup ("Asynch. foo call one.");
              break;
            case 1:
              test = CORBA::string_dup ("Asynch. foo call two");
              break;
            case 2:
              test = CORBA::string_dup ("Asynch. foo call three");
              break;
            default:
              break;
          }
        ++this->nr_of_sent_;
        my_one_ami_[i].objref->sendc_foo (cb_one.in (),
                                          test.in (),
                                          i);
        ACE_DEBUG ((LM_DEBUG, "Sender (ASYNCH) : send <%C> !\n",
                    test.in ()));
        // There is more than 1 message sent, without receiving callbacks,
        // so it is asynchronous
        if (this->nr_of_sent_.value() > 1)
          {
            asynch = true;
          }
        ACE_DEBUG ((LM_DEBUG,
                    "Sender (ASYNCH) : send asynch call bar <%u>!\n", i));
        ::UsesSM::AMI4CCM_TwoReplyHandler_var cb_two =
           new CIAO_UsesSM_AMI4CCM_Two_Connector_AMI4CCM_Connector_Impl::AMI4CCM_TwoReplyHandler_i (
                                                    this->nr_of_received_);
        my_two_ami_->sendc_bar ( cb_two.in (), i);
    }
    return 0;
  }
  //============================================================
  // Worker thread for synchronous invocations for One
  //============================================================
  synch_foo_generator::synch_foo_generator (
     ::UsesSM::CCM_Sender_Context_ptr context,
      Atomic_UShort  &nr_of_received)
  : context_(::UsesSM::CCM_Sender_Context::_duplicate (context)),
    nr_of_received_ (nr_of_received)
  {
  }

  int synch_foo_generator::svc ()
  {
    ::UsesSM::Sender::run_my_um_oneConnections_var my_one_ami_ =
         context_->get_connections_run_my_um_one ();

    for(CORBA::ULong i = 0; i < my_one_ami_->length(); ++i)
      {
        CORBA::String_var test;
        switch (i)
          {
            case 0:
              test = CORBA::string_dup ("Synch. call 0.");
              break;
            case 1:
              test = CORBA::string_dup ("Synch. call 1");
              break;
            case 2:
              test = CORBA::string_dup ("Synch. call 2");
              break;
            default:
              break;
          }

        ACE_DEBUG ((LM_DEBUG,"Sender (SYNCH) : send <%C> !\n",
                    test.in ()));

        CORBA::String_var answer;
        CORBA::ULong result = my_one_ami_[i].objref->foo( test,
                              i,
                              answer.out ());
        if (result != i)
          {
            ACE_ERROR ((LM_ERROR,
                        "ERROR Sender (SYNCH) : CLASS One foo !\n"));
          }
        else
          {
            ++this->nr_of_received_;
            ACE_DEBUG ((LM_DEBUG,
                        "Sender (SYNCH) : received answer = <%C> !\n",
                        answer.in ()));
          }
      }
    return 0;
  }
  /**
   * Component Executor Implementation Class: Sender_exec_i
   */

  Sender_exec_i::Sender_exec_i (void){
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::UsesSM::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->asynch_foo_gen =
        new asynch_foo_generator (this->ciao_context_.in (),
            this->nr_of_received_,
            this->nr_of_sent_);
    this->asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    this->synch_foo_gen =
         new synch_foo_generator (this->ciao_context_.in (),
             this->nr_of_received_);
    this->synch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (asynch == false)
      {
        ACE_ERROR ((LM_ERROR,
                   "ERROR: Uses multiple test not asynchronous!\n"));
      }
    if (this->nr_of_received_.value() != 9)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Did not get all callbacks from "
                              "multiple ports."
                              " Received = %u of 9\n",
                              this->nr_of_received_.value()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    "OK: All messages received back by Sender\n"));
      }
    delete this->asynch_foo_gen;
    this->asynch_foo_gen = 0;
    delete this->synch_foo_gen;
    this->synch_foo_gen = 0;
  }

  extern "C" USESSM_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_UsesSM_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
