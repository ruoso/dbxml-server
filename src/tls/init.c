#include "init.h"
#include <gnutls/gnutls.h>
#include <gcrypt.h>
#include <errno.h>
#include <pthread.h>
GCRY_THREAD_OPTION_PTHREAD_IMPL;

#define X509 (options->Encryption.x509_cred)

void tls_init(DbXmlServerOptions *options) {
  gcry_control (GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
  gnutls_global_init();

  int ret;
  ret = gnutls_certificate_allocate_credentials(&(options->Encryption.x509_cred));
  if (ret != GNUTLS_E_SUCCESS) {
    LOG_ERROR("Failed to allocate credentials (%d)\n", ret);
    exit(-1);
  }

  if (options->Encryption.cafile[0]) {
    ret = gnutls_certificate_set_x509_trust_file(options->Encryption.x509_cred,
                                                 options->Encryption.cafile,
                                                 GNUTLS_X509_FMT_PEM);
    if (ret < 0) {
      LOG_ERROR("Failed to process trust file '%s'\n", options->Encryption.cafile);
      exit(-1);
    }
  }

  if (options->Encryption.crlfile[0]) {
    ret = gnutls_certificate_set_x509_crl_file(options->Encryption.x509_cred,
                                               options->Encryption.crlfile,
                                               GNUTLS_X509_FMT_PEM);
    if (ret < 0) {
      LOG_ERROR("Failed to process CRL file '%s'\n", options->Encryption.crlfile);
      exit(-1);
    }
  }

  ret = gnutls_certificate_set_x509_key_file(options->Encryption.x509_cred,
                                             options->Encryption.keyfile, 
                                             options->Encryption.certfile,
                                             GNUTLS_X509_FMT_PEM);
  if (ret != GNUTLS_E_SUCCESS) {
    LOG_ERROR("Failed to process server key '%s', '%s' (%d)\n", options->Encryption.certfile, options->Encryption.keyfile, ret);
    exit(-1);
  }

}
