#include "email.h"

#include <cstdint>
#include <thread>
#include <cmath>

Email::Email(std::string const& server, uint16_t port, std::string const& username, std::string const& password, bool tls) :
    NotificationBase(),
    m_server(server),
    m_port(port),
    m_username(username),
    m_password(password)
{
}

Email::~Email() {

}

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;
  size_t room = size * nmemb;
 
  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }
 
  data = &payload_text[upload_ctx->bytes_read];
 
  if(data) {
    size_t len = strlen(data);
    if(room < len)
      len = room;
    memcpy(ptr, data, len);
    upload_ctx->bytes_read += len;
 
    return len;
  }
 
  return 0;
}

bool Email::SendMessage(std::string const& message)
{

  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx = { 0 };

    /* Set username and password */
    curl_easy_setopt(m_curl, CURLOPT_USERNAME, m_username.c_str());
    curl_easy_setopt(m_curl, CURLOPT_PASSWORD, m_password.c_str());
 
    /* This is the URL for your mailserver. Note the use of port 587 here,
     * instead of the normal SMTP port (25). Port 587 is commonly used for
     * secure mail submission (see RFC 4403), but you should use whatever
     * matches your server configuration. */
    curl_easy_setopt(m_curl, CURLOPT_URL, "smtp://mainserver.example.net:587");
 
    /* In this example, we start with a plain text connection, and upgrade to
     * Transport Layer Security (TLS) using the STARTTLS command. Be careful
     * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the
     * transfer continues anyway - see the security discussion in the libcurl
     * tutorial for more details. */
    curl_easy_setopt(m_curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
 
    /* If your server does not have a valid certificate, then you can disable
     * part of the Transport Layer Security protection by setting the
     * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
     *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
     *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
     * That is, in general, a bad idea. It is still better than sending your
     * authentication details in plain text though.  Instead, you should get
     * the issuer certificate (or the host certificate if the certificate is
     * self-signed) and add it to the set of certificates that are known to
     * libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See docs/SSLCERTS
     * for more information. */
    //curl_easy_setopt(m_curl, CURLOPT_CAINFO, "/path/to/certificate.pem");
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
 
    /* Note that this option is not strictly required, omitting it results in
     * libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(m_curl, CURLOPT_MAIL_FROM, FROM_MAIL);
 
    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
    recipients = curl_slist_append(recipients, TO_MAIL);
    recipients = curl_slist_append(recipients, CC_MAIL);
    curl_easy_setopt(m_curl, CURLOPT_MAIL_RCPT, recipients);
 
    /* We are using a callback function to specify the payload (the headers and
     * body of the message). You could just use the CURLOPT_READDATA option to
     * specify a FILE pointer to read from. */
    curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(m_curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(m_curl, CURLOPT_UPLOAD, 1L);
 
    /* Since the traffic is encrypted, it is useful to turn on debug
     * information within libcurl to see what is happening during the
     * transfer.
     */
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
 
    /* Send the message */
    res = curl_easy_perform(m_curl);
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* Free the list of recipients */
    curl_slist_free_all(recipients);
 

 
  return (int)res;

}



