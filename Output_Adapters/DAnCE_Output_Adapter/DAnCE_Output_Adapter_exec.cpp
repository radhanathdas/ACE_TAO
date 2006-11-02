#include "DAnCE_Output_Adapter_exec.h"
#include "ciao/CIAO_common.h"
#include "common/RACE_Log_Msg.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_DAnCE_Output_Adapter_Impl
    {
      //==================================================================
      // Facet Executor Implementation Class:   PlanEgress_exec_i
      //================================================================
      PlanEgress_exec_i::PlanEgress_exec_i (CORBA::ORB_ptr orb,
                                            DAnCE_Output_Adapter_exec_i &comp)
        : orb_ (CORBA::ORB::_duplicate (orb)),
          comp_ (comp)
      {
      }

      PlanEgress_exec_i::~PlanEgress_exec_i (void)
      {
      }

      // Operations from ::CIAO::RACE::PlanEgress

      ::CORBA::Boolean
      PlanEgress_exec_i::output_plan
      (const ::CIAO::RACE::Plan_Actions & plan_seq
                                      ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        ACE_DEBUG ((LM_DEBUG, "RACE:DAnCE_Output_Adapter::output_plan\n"));

        for (size_t i = 0;i < plan_seq.length(); ++i)
          {
            if ((plan_seq[i].command == ::CIAO::RACE::DEPLOY) ||
                (plan_seq[i].command == ::CIAO::RACE::REPLAN_DEPLOY))
             {
                if ( !this->launch_helper(plan_seq[i]))
                  {
                    return false;
                  }

              }
            else
              {
                if ( !this->teardown_helper(plan_seq[i]) )
                  {
                    return false;
                  }
              }
          }

        /// Return true if all the actions are executed successfully (even
        /// if there are no actions to be executed).
        return true;
      }


      ::CORBA::Boolean
      PlanEgress_exec_i::launch_helper(
          const ::CIAO::RACE::Plan_Action & plan_action )

        {
          const ::Deployment::DeploymentPlan& plan=plan_action.plan;
          const ::Deployment::PackageConfiguration& package=plan_action.package;

          ACE_DEBUG(( LM_DEBUG, "RACE:DAnCE_OA: Trying to launch plan with uuid: %s\n",
               plan.UUID.in() ));

          CORBA::Long importance = Log::find_importance (package);

          RACE_DEPLOYMENT_STATUS(
		plan_action.command, Log::EventMark::BEGIN_EVENT, plan.UUID.in (), 0,
		Log::Status::SUCCESS, importance, "Success"  );

          try
            {
              ACE_CString uuid (this->comp_.launcher_.launch_plan (plan));

              if (uuid.c_str () == 0)
                {
                  ACE_ERROR ((LM_ERROR, "Unable to launch plan with UUID %s\n",
                              plan.UUID.in ()));

                  RACE_DEPLOYMENT_STATUS(plan_action.command, Log::EventMark::END_EVENT,
                                         plan.UUID.in (), 0,
                                         Log::Status::FAILURE, importance,
                                         "Failed on Plan Launcher\n"  );

                  return false;
                }

              // Write out DApp IOR
              try
                {
                  CORBA::String_var ior = this->orb_->object_to_string
                    (this->comp_.launcher_.get_dam (uuid.c_str ()));
                  uuid += "_DAM.ior";

                  FILE* ior_output_file = ACE_OS::fopen (uuid.c_str (), "w");

                  if (ior_output_file)
                    {
                      ACE_OS::fprintf (ior_output_file, "%s", ior.in ());
                      ACE_OS::fclose (ior_output_file);

                      RACE_DEPLOYMENT_STATUS(plan_action.command, Log::EventMark::END_EVENT,
                                             plan.UUID.in(), 0,
                                             Log::Status::SUCCESS, importance,
                                             "Success");

                    }
                  else
                    {
                      ACE_ERROR(( LM_ERROR, "Unable to write DAM ior for plan %s",
                                  uuid.c_str() ));

                      RACE_DEPLOYMENT_STATUS(plan_action.command, Log::EventMark::END_EVENT,
                                             plan.UUID.in(), 0,
                                             Log::Status::SUCCESS, importance,
                                             "Deployment succeeded, but failed to output DAM IOR");
                    }
                }
              catch (...)
                {
                  ACE_ERROR(( LM_ERROR, "Unable to write DAM ior for plan %s",
                              uuid.c_str() ));

                  RACE_DEPLOYMENT_STATUS(plan_action.command, Log::EventMark::END_EVENT,
                                         plan.UUID.in(), 0,
                                         Log::Status::SUCCESS, importance,
                                         "Deployment succeeded, but failed to output DAM IOR");
                }
            }
          catch (Plan_Launcher::Plan_Launcher_i::Deployment_Failure &ex_)
            {
              ACE_CString error ("Plan deployment failed: ");
              error += ex_.error_;

              RACE_DEPLOYMENT_STATUS(plan_action.command, Log::EventMark::END_EVENT,
                                     plan.UUID.in(), 0,
                                     Log::Status::FAILURE, importance,
                                     error.c_str()   );

              return false;
            }

          RACE_DEPLOYMENT_STATUS(
               plan_action.command, Log::EventMark::END_EVENT,
               plan.UUID.in(), 0, Log::Status::SUCCESS, importance,
               "Successful"  );

          // Pass along descriptors...
          this->comp_.context_->get_connection_control_output ()
               ->register_string (package, plan);

          return true;
        }

      ::CORBA::Boolean
      PlanEgress_exec_i::teardown_helper
      (const ::CIAO::RACE::Plan_Action & plan_action )
        {
          ACE_DEBUG ((LM_DEBUG, "RACE:DAnCE_OA: Trying to teardown plan with uuid: %s\n",
          plan_action.plan.UUID.in  ()));

          RACE_DEPLOYMENT_STATUS (
		plan_action.command, Log::EventMark::BEGIN_EVENT,
                plan_action.plan.UUID.in(), 0,
                Log::Status::SUCCESS, "Success");

          bool result=false;

          try
            {
              result = this->comp_.launcher_.teardown_plan( plan_action.plan.UUID.in() );

              if (result)
                RACE_DEPLOYMENT_STATUS (
		      plan_action.command, Log::EventMark::END_EVENT,
                      plan_action.plan.UUID.in(), 0,
                      Log::Status::SUCCESS, "Success");
              else
                RACE_DEPLOYMENT_STATUS (
		      plan_action.command, Log::EventMark::END_EVENT,
                      plan_action.plan.UUID.in(), 0,
                      Log::Status::FAILURE, "Teardown failed for unknown reason\n");
              }
          catch (Plan_Launcher::Plan_Launcher_i::Deployment_Failure &ex_)
            {
              ACE_CString error ("Plan teardown failed: ");
              error += ex_.error_;

              RACE_DEPLOYMENT_STATUS (
		      plan_action.command, Log::EventMark::END_EVENT,
                      plan_action.plan.UUID.in(), 0,
                      Log::Status::FAILURE,
                      error.c_str() );
            }

          this->comp_.context_->get_connection_control_output ()
            ->unregister_string (plan_action.plan.UUID.in ());

          /** @TODO Please check what to do after teardown.

          // Write out DApp IOR
          CORBA::String_var ior = this->orb_->object_to_string
            (this->comp_.launcher_.get_dam (uuid.c_str ()));

          uuid += "_DAM.ior";

          FILE* ior_output_file = ACE_OS::fopen (uuid.c_str (),
                                                "w");

          if (ior_output_file)
            {
              ACE_OS::fprintf (ior_output_file, "%s", ior.in ());
              ACE_OS::fclose (ior_output_file);
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Unable to write DAM ior for plan %s",
              uuid.c_str ()));
              return false;
            }

          // Pass along descriptors...
          this->comp_.context_->get_connection_control_output ()
            ->push_descriptors (package, plan);
          */
          return result;
        }

      //==================================================================
      // Facet Executor Implementation Class:   Plan_Status_exec_i
      //==================================================================

      Plan_Status_exec_i::Plan_Status_exec_i
      (DAnCE_Output_Adapter_exec_i & comp)
        : comp_ (comp)
      {
      }

      Plan_Status_exec_i::~Plan_Status_exec_i (void)
      {
      }

      // Operations from ::CIAO::RACE::Plan_Status

      ::CORBA::Boolean
      Plan_Status_exec_i::teardown (
        const char * uuid
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        bool status (false);

        try
          {
            status = this->comp_.launcher_.teardown_plan (uuid);
          }
        catch (Plan_Launcher::Plan_Launcher_i::Deployment_Failure &ex_)
          {
            ACE_CString error ("!!! SHOULD NEVER HAVE CALLED THIS ROUTINE...Plan teardown failed: ");
            error += ex_.error_;

            RACE_DEPLOYMENT_STATUS(
	          Log::EventType::MANUAL_PLAN,
		  Log::SubType::TEARDOWN_END,
                  uuid, 0,
                  Log::Status::FAILURE,
                  error.c_str()  );
          }

        return status;
      }

      //==================================================================
      // Component Executor Implementation Class:   DAnCE_Output_Adapter_exec_i
      //==================================================================

      DAnCE_Output_Adapter_exec_i::DAnCE_Output_Adapter_exec_i (void)
      {
      }

      DAnCE_Output_Adapter_exec_i::~DAnCE_Output_Adapter_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Attribute operations.

      // Port operations.

      ::CIAO::RACE::CCM_PlanEgress_ptr
      DAnCE_Output_Adapter_exec_i::get_plan_egress (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        return new PlanEgress_exec_i
          (this->context_->_ciao_the_Container ()->the_ORB (), *this);
      }

      ::CIAO::RACE::CCM_Plan_Status_ptr
      DAnCE_Output_Adapter_exec_i::get_plan_status ()
        ACE_THROW_SPEC ((CORBA::SystemException))
      {
        return new Plan_Status_exec_i (*this);
      }

      // Operations from Components::SessionComponent

      void
      DAnCE_Output_Adapter_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->context_ =
        DAnCE_Output_Adapter_Context::_narrow (
          ctx
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (this->context_ == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
      }

      void
      DAnCE_Output_Adapter_exec_i::ciao_preactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        this->launcher_.init
          (0, this->context_->_ciao_the_Container ()->the_ORB ());
      }

      void
      DAnCE_Output_Adapter_exec_i::ciao_postactivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      DAnCE_Output_Adapter_exec_i::ccm_activate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      DAnCE_Output_Adapter_exec_i::ccm_passivate (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      void
      DAnCE_Output_Adapter_exec_i::ccm_remove (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        // Your code here.
      }

      //==================================================================
      // Home Executor Implementation Class:   DAnCE_Output_Adapter_Home_exec_i
      //==================================================================

      DAnCE_Output_Adapter_Home_exec_i::DAnCE_Output_Adapter_Home_exec_i (void)
      {
      }

      DAnCE_Output_Adapter_Home_exec_i::~DAnCE_Output_Adapter_Home_exec_i (void)
      {
      }

      // Supported or inherited operations.

      // Home operations.

      // Factory and finder operations.

      // Attribute operations.

      // Implicit operations.

      ::Components::EnterpriseComponent_ptr
      DAnCE_Output_Adapter_Home_exec_i::create (
        ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
                        ::CORBA::SystemException,
                        ::Components::CCMException))
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (
          retval,
          DAnCE_Output_Adapter_exec_i,
          CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" DANCE_OUTPUT_ADAPTER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_DAnCE_Output_Adapter_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (
          retval,
          DAnCE_Output_Adapter_Home_exec_i,
          ::Components::HomeExecutorBase::_nil ());

        return retval;
      }
    }
  }
}
