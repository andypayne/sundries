#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include <Carbon/Carbon.h>

#include <iostream>
#include <string>


std::string URLEventToString (URLEvent TheEvent)
{
    switch(TheEvent)
    {
    case kURLDataAvailableEvent :
        return("kURLDataAvailableEvent");
        break;
    case kURLCompletedEvent :
        return("kURLCompletedEvent");
        break;
    case kURLErrorOccurredEvent :
        return("kURLErrorOccurredEvent");
        break;
    case kURLInitiatedEvent :
        return("kURLInitiatedEvent");
        break;
    case kURLAbortInitiatedEvent :
        return("kURLAbortInitiatedEvent");
        break;
    case kURLResourceFoundEvent :
        return("kURLResourceFoundEvent");
        break;
    case kURLDownloadingEvent :
        return("kURLDownloadingEvent");
        break;
    case kURLTransactionCompleteEvent :
        return("kURLTransactionCompletedEvent");
        break;
    case kURLUploadingEvent :
        return("kURLUploadingEvent");
    case kURLSystemEvent :
        return("kURLSystemEvent");
        break;
    case kURLPropertyChangedEvent :
        return("kURLPropertyChangedEvent");
        break;
    case kURLPeriodicEvent :
        return("kURLPeriodicEvent");
        break;
    case kURLPercentEvent :
        return("kURLPercentEvent");
        break;
    }
    return("Unknown Event");
}


std::string URLStateToString (URLState TheState)
{
    switch(TheState)
    {
    case kURLNullState :
        return("kURLNullState");
        break;
    case kURLInitiatingState :
        return("kURLInitiatingState");
        break;
    case kURLLookingUpHostState :
        return("kURLLookingUpHostState");
        break;
    case kURLConnectingState :
        return("kURLConnectingState");
        break;
    case kURLResourceFoundState :
        return("kURLResourceFoundState");
        break;
    case kURLDownloadingState :
        return("kURLDownloadingState");
        break;
    case kURLDataAvailableState :
        return("kURLDataAvailableState");
        break;
    case kURLTransactionCompleteState :
        return("kURLTransactionCompleteState");
        break;
    case kURLErrorOccurredState :
        return("kURLErrorOccurredState");
        break;
    case kURLAbortingState :
        return("kURLAbortingState");
        break;
    case kURLCompletedState :
        return("kURLCompletedState");
        break;
    case kURLUploadingState :
        return("kURLUploadingState");
        break;
    }
    return("Unknown State");
}




class HttpHelper
{
public:
    enum RequestMethod
    {
        RequestMethodGet = 1,
        RequestMethodPost = 2
    };

    HttpHelper () : m_UrlRef(NULL), m_ReqMethod(RequestMethodGet) { }

    HttpHelper (const std::string& UrlStr,
                const RequestMethod ReqMethod = RequestMethodGet)
        : m_UrlRef(NULL),
          m_UrlStr(UrlStr),
          m_ReqMethod(ReqMethod)
    {
    }

    virtual ~HttpHelper () { if (m_UrlRef != NULL) ::URLDisposeReference(m_UrlRef); }

    OSStatus SetBody (const std::string& ReqBody)
    {
        m_ReqBody = ReqBody;
        return noErr;
    }

    OSStatus Open (std::string& ReturnData, const std::string& UrlStr)
    {
        m_UrlStr = UrlStr;
        return Open (ReturnData);
    }

    OSStatus Open (std::string& ReturnData)
    {
        OSStatus    err = noErr;
        err = ::URLNewReference(m_UrlStr.c_str(), &m_UrlRef);
        if (err != noErr)
        {
            return err;
        }

        SetRequestMethod();
        SetRequestBody();
        err = ::URLOpen(m_UrlRef, NULL, 0, NULL, 0, NULL);

        URLState                    TransferState = kURLNullState;
        std::string                 strBuf;
        Size                        DataSize = 0;

        while (1)
        {
            ::URLIdle();
            ::URLGetCurrentState(m_UrlRef, &TransferState);
            if (TransferState == kURLDataAvailableState)
            {
                void*   UrlBuf;
                ::URLGetBuffer(m_UrlRef, &UrlBuf, &DataSize);
                for (unsigned int i = 0; i < DataSize; ++i)
                {
                    strBuf += *((unsigned char*)(UrlBuf) + i*sizeof(unsigned char));
                }
                ::URLReleaseBuffer(m_UrlRef, UrlBuf);
            }
            else if (TransferState == kURLCompletedState)
            {
                break;
            }
            else if (TransferState == kURLErrorOccurredState)
            {
                err = -1;
                break;
            }
        }
        ReturnData.assign(strBuf.begin(), strBuf.end());
        return err;
    }


protected:
    OSStatus SetRequestMethod ()
    {
        if (m_UrlRef == NULL)
        {
            return -1;
        }

        switch (m_ReqMethod)
        {
        case RequestMethodGet :
            return ::URLSetProperty(m_UrlRef,
                                    kURLHTTPRequestMethod,
                                    static_cast<char*>("GET"),
                                    3);
            break;
        case RequestMethodPost :
            return ::URLSetProperty(m_UrlRef,
                                    kURLHTTPRequestMethod,
                                    static_cast<char*>("POST"),
                                    4);
            break;
        }
        return -1;
    }

    OSStatus SetRequestBody ()
    {
        if (m_UrlRef == NULL)
        {
            return -1;
        }

        return ::URLSetProperty(m_UrlRef,
                                kURLHTTPRequestBody,
                                const_cast<char*>(m_ReqBody.c_str()),   // TODO: Bad?
                                m_ReqBody.size());      // TODO: Include NULL?
    }

protected:
    URLReference    m_UrlRef;
    std::string     m_UrlStr;
    RequestMethod   m_ReqMethod;
    std::string     m_ReqBody;
};




#endif  // #ifndef HTTPHELPER_H

