#include "pushover.h"

#include <cstdint>
#include <thread>
#include <cmath>

Pushover::Pushover(std::string const& token, std::string const& user) :
    NotificationBase(),
    m_token(token),
    m_user(user)
{

}

Pushover::~Pushover() {
}

bool Pushover::SendMessage(std::string const& message)
{
    try {
       /*
       curl -s \
            --form-string "token=APP_TOKEN" \
            --form-string "user=USER_KEY" \
            --form-string "message=hello world" \
            https://api.pushover.net/1/messages.json       
       */
        std::string repURL = "https://api.pushover.net/1/messages.json";
        LogInfo(VB_PLUGIN, "URL %s \n", repURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(m_curl, CURLOPT_URL, repURL.c_str());


        //curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, message.c_str());
        curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(m_curl, CURLOPT_DEFAULT_PROTOCOL, "https");

       // struct curl_slist *headers = nullptr;
       // headers = curl_slist_append(headers, "Content-Type: text/plain");

       // curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers); 
       //struct curl_slist *post_fields = nullptr;
      // post_fields = curl_slist_append(post_fields, std::string("token=" + m_token).c_str());
       //post_fields = curl_slist_append(post_fields, std::string("user=" + m_user).c_str());
       //post_fields = curl_slist_append(post_fields, std::string("message=" + message).c_str());
       //curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, post_fields);
       // headers = curl_slist_append(headers, "Content-Type: text/plain");
       //LogInfo(VB_PLUGIN, "m_token %s \n", m_token.c_str());
       //LogInfo(VB_PLUGIN, "m_user %s \n", m_user.c_str());
       LogInfo(VB_PLUGIN, "message %s \n", message.c_str());
        std::string postmsg = "token=" + m_token + "&user=" + m_user + "&message=" + message;
        //LogInfo(VB_PLUGIN, "postmsg %s \n", postmsg.c_str());
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postmsg.c_str());
       // curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers); 

        CURLcode status = curl_easy_perform(m_curl);
        if (status != CURLE_OK) {
            m_unreachable = true;
            LogInfo(VB_PLUGIN, "failed to send curl command\n");
            return false;
        }
        return true;
    }
    catch(std::exception const& ex) {
        LogInfo(VB_PLUGIN, "Error %s \n", ex.what());
        return false;
    }

}
