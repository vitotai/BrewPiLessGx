#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <ESPAsyncWebServer.h>

class FileManager: public AsyncWebHandler {
  private:
    fs::FS _fs;
    String _username;
    String _password; 
    bool _authenticated;
    String _path;

  public:
    FileManager(const fs::FS& fs,const String& path, const String& username=String(), const String& password=String());
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override final;
    virtual bool isRequestHandlerTrivial() override final {return false;}
    protected:
    void _handleFileList(AsyncWebServerRequest *request);
    void _handleFileDelete(AsyncWebServerRequest *request);
    void _handleFileUpload(AsyncWebServerRequest *request);
    void _handleFileCreate(AsyncWebServerRequest *request);
    void _handleDirCreate(AsyncWebServerRequest *request);
};

#endif
