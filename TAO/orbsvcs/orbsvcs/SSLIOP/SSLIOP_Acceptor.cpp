// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "SSLIOP_Acceptor.h"
#include "SSLIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "SSLIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, SSLIOP_Acceptor, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_SSLIOP_Server_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_SSLIOP_Server_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_SSLIOP_Server_Connection_Handler>;
template class TAO_Creation_Strategy<TAO_SSLIOP_Server_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO_SSLIOP_Server_Connection_Handler>;
template class TAO_Accept_Strategy<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_SSLIOP_Server_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_SSLIOP_Server_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_SSLIOP_Server_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO_SSLIOP_Server_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO_SSLIOP_Server_Connection_Handler>
#pragma instantiate TAO_Accept_Strategy<TAO_SSLIOP_Server_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_SSLIOP_Acceptor::TAO_SSLIOP_Acceptor (void)
  : TAO_IIOP_Acceptor (),
    ssl_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0)
{
  this->ssl_component_.target_supports = 0;
  this->ssl_component_.target_requires = 0;

  // @@ Verify that these initial settings are correct.

  // @@ Security::NoDelegation is part of the Security Service 1.7
  //    spec.  Enable bit below once the Security.pidl file is
  //    updated.

  // SSLIOP requires these Security::AssociationOptions by default.
  ACE_SET_BITS (this->ssl_component_.target_requires,
                Security::Integrity
                | Security::Confidentiality
                | Security::DetectReplay
                | Security::DetectMisordering
                /* | Security::NoDelegation */);

  // SSLIOP supports these Security::AssociationOptions by default.
  ACE_SET_BITS (this->ssl_component_.target_supports,
                Security::NoProtection   // @@ Yeah?
                | Security::Integrity
                | Security::Confidentiality
                | Security::DetectReplay
                | Security::DetectMisordering
                /* | Security::NoDelegation */);

  // Initialize the default SSL port to zero.
  this->ssl_component_.port = 0;
}

TAO_SSLIOP_Acceptor::~TAO_SSLIOP_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

// TODO =
//    2) For V1.[1,2] there are tagged components

int
TAO_SSLIOP_Acceptor::create_mprofile (const TAO_ObjectKey &object_key,
                                      TAO_MProfile &mprofile)
{
  // Adding this->num_hosts_ to the TAO_MProfile.
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < this->num_hosts_
      && mprofile.grow (count + this->num_hosts_) == -1)
    return -1;

  for (size_t i = 0; i < this->num_hosts_; ++i)
    {
      TAO_SSLIOP_Profile *pfile = 0;
      ACE_NEW_RETURN (pfile,
                      TAO_SSLIOP_Profile (this->hosts_[i].c_str (),
                                          this->addrs_[i].get_port_number (),
                                          object_key,
                                          this->addrs_[i],
                                          this->version_,
                                          this->orb_core_,
                                          this->ssl_component_.port),
                      -1);

      if (mprofile.give_profile (pfile) == -1)
        {
          pfile->_decr_refcnt ();
          pfile = 0;
          return -1;
        }

      if (this->orb_core_->orb_params ()->std_profile_components () == 0)
        continue;

      pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

      CONV_FRAME::CodeSetComponentInfo code_set_info;
      code_set_info.ForCharData.native_code_set  =
        TAO_DEFAULT_CHAR_CODESET_ID;
      code_set_info.ForWcharData.native_code_set =
        TAO_DEFAULT_WCHAR_CODESET_ID;
      pfile->tagged_components ().set_code_sets (code_set_info);

      pfile->tagged_components ().set_tao_priority (this->priority ());

      IOP::TaggedComponent component;
      component.tag = IOP::TAG_SSL_SEC_TRANS;
      // @@???? Check this code, only intended as guideline...
      TAO_OutputCDR cdr;
      cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
      cdr << this->ssl_component_;
      // TAO extension, replace the contents of the octet sequence with
      // the CDR stream
      CORBA::ULong length = cdr.total_length ();
      component.component_data.length (length);
      CORBA::Octet *buf = component.component_data.get_buffer ();
      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
          buf += i->length ();
        }

      pfile->tagged_components ().set_component (component);
    }

  return 0;
}

int
TAO_SSLIOP_Acceptor::is_collocated (const TAO_Profile *pfile)
{
  const TAO_SSLIOP_Profile *profile =
    ACE_dynamic_cast (const TAO_SSLIOP_Profile *,
                      pfile);

  // Make sure the dynamically cast pointer is valid.
  if (profile == 0)
    return 0;

  for (size_t i = 0; i < this->num_hosts_; ++i)
    {
      // compare the port and sin_addr (numeric host address)
      if (profile->object_addr () == this->addrs_[i])
        return 1;  // Collocated
    }

  return 0;  // Not collocated
}

int
TAO_SSLIOP_Acceptor::close (void)
{
  int r = this->ssl_acceptor_.close ();
  if (this->TAO_IIOP_Acceptor::close () != 0)
    r = -1;

  return r;
}

int
TAO_SSLIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                           int major,
                           int minor,
                           const char *address,
                           const char *options)
{
  // Open the non-SSL enabled endpoints, then open the SSL enabled
  // endpoints.
  if (this->TAO_IIOP_Acceptor::open (orb_core,
				     major,
				     minor,
				     address,
				     options) != 0)
    return -1;

  // The SSL port is set in the parse_options() method. All we have
  // to do is call open_i()
  ACE_INET_Addr addr (this->ssl_component_.port,
                      this->addrs_[0].get_host_addr ());

  return this->open_i (orb_core, addr);
}

int
TAO_SSLIOP_Acceptor::open_default (TAO_ORB_Core *orb_core,
                                   const char *options)
{
  // Open the non-SSL enabled endpoints, then open the SSL enabled
  // endpoints.
  if (this->TAO_IIOP_Acceptor::open_default (orb_core, options) == -1)
    return -1;

  // Now that each network interface's hostname has been cached, open
  // an endpoint on each network interface using the INADDR_ANY
  // address.
  ACE_INET_Addr addr;

  // this->ssl_component_.port is initialized to zero or it is set in
  // this->parse_options().
  if (addr.set (this->ssl_component_.port,
                ACE_static_cast(ACE_UINT32, INADDR_ANY),
                1) != 0)
    return -1;

  return this->open_i (orb_core, addr);
}

int
TAO_SSLIOP_Acceptor::open_i (TAO_ORB_Core* orb_core,
                             const ACE_INET_Addr& addr)
{
  this->orb_core_ = orb_core;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_SSLIOP_CREATION_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_SSLIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_SSLIOP_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  if (this->ssl_acceptor_.open (addr,
                                orb_core->reactor (this),
                                this->creation_strategy_,
                                this->accept_strategy_,
                                this->concurrency_strategy_) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("SSLIOP_Acceptor::open_i - %p\n\n"),
                    ACE_TEXT ("cannot open acceptor")));
      return -1;
    }

  ACE_INET_Addr ssl_address;

  // We do this make sure the port number the endpoint is listening on
  // gets set in the addr.
  if (this->ssl_acceptor_.acceptor ().get_local_addr (ssl_address) != 0)
    {
      // @@ Should this be a catastrophic error???
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) ")
                    ACE_TEXT ("SSLIOP_Acceptor::open_i - %p\n\n"),
                    ACE_TEXT ("cannot get local addr")));
      return -1;
    }

  // Reset the SSL endpoint port to the one chosen by the OS (or by
  // the user if provided.
  this->ssl_component_.port = ssl_address.get_port_number ();

  if (TAO_debug_level > 5)
    {
      for (size_t i = 0; i < this->num_hosts_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) ")
                      ACE_TEXT ("SSLIOP_Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%s:%u>\n"),
                      this->hosts_[i].c_str (),
                      this->ssl_component_.port));
        }
    }

  return 0;
}

int
TAO_SSLIOP_Acceptor::parse_options (const char *str)
{
  // Set the SSL port to the default value...
  // @@ Do we really need to do this...
  this->ssl_component_.port = 0;

  if (str == 0)
    return 0;  // No options to parse.  Not a problem.

  // Use an option format similar to the one used for CGI scripts in
  // HTTP URLs.
  // e.g.:  option1=foo&option2=bar

  ACE_CString options (str);

  size_t len = options.length ();

  const char option_delimiter = '&';

  // Count the number of options.

  CORBA::ULong option_count = 1;
  // Number of endpoints in the string  (initialized to 1).

  // Only check for endpoints after the protocol specification and
  // before the object key.
  for (size_t i = 0; i < len; ++i)
    if (options[i] == option_delimiter)
      option_count++;

  // The idea behind the following loop is to split the options into
  // (option, name) pairs.
  // For example,
  //    `option1=foo&option2=bar'
  // will be parsed into:
  //    `option1=foo'
  //    `option2=bar'

  int begin = 0;
  int end = -1;

  // @@ We should add options to set the security association options,
  // or are those controlled by Policies?
  for (CORBA::ULong j = 0; j < option_count; ++j)
    {
      begin += end + 1;

      if (j < option_count - 1)
        end = options.find (option_delimiter, begin);
      else
        end = len - begin;  // Handle last endpoint differently

      if (end == begin)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) Zero length")
                           ACE_TEXT ("IIOP/SSL option.\n")),
                          -1);
      else if (end != ACE_CString::npos)
        {
          ACE_CString opt = options.substring (begin, end);

          int slot = opt.find ("=");

          if (slot == ACE_static_cast (int, len - 1)
              || slot == ACE_CString::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) IIOP/SSL")
                               ACE_TEXT ("option <%s> is ")
                               ACE_TEXT ("missing a value.\n"),
                               opt.c_str ()),
                              -1);

          ACE_CString name = opt.substring (0, slot);
          ACE_CString value = opt.substring (slot + 1);

          if (name.length () == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "TAO (%P|%t) Zero length IIOP/SSL "
                               "option name.\n"),
                              -1);

          if (name == "priority")
            {
              CORBA::Short corba_priority =
                ACE_static_cast (CORBA::Short,
                                 ACE_OS::atoi (value.c_str ()));

              if (corba_priority >= 0
                  /* && corba_priority < 32768 */)
                // priority_ and corba_priority will always be less
                // than 32768 since CORBA::Short is a signed 16 bit
                // integer.
                this->priority_ = corba_priority;
              else
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("TAO (%P|%t) Invalid ")
                                   ACE_TEXT ("IIOP/SSL endpoint ")
                                   ACE_TEXT ("priority: <%s>\n"),
                                   value.c_str ()),
                                  -1);
            }
          else if (ACE_OS::strcmp (name.c_str (), "ssl_port") == 0)
            {
              int ssl_port = ACE_OS::atoi (value.c_str ());

              if (ssl_port >= 0 && ssl_port < 65536)
                this->ssl_component_.port = ssl_port;
              else
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("TAO (%P|%t) Invalid ")
                                   ACE_TEXT ("IIOP/SSL endpoint ")
                                   ACE_TEXT ("port: <%s>\n"),
                                   value.c_str ()),
                                  -1);
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("TAO (%P|%t) Invalid ")
                               ACE_TEXT ("IIOP/SSL ")
                               ACE_TEXT ("option: <%s>\n"),
                               name.c_str ()),
                              -1);
        }
    }
  return 0;
}
