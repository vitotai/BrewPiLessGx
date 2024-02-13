
#include "Config.h"
#include "FileManager.h"
#include "edit_html_gz.h"


#if 0
//holds the current upload
extern String getContentType(String filename);
String getResponseContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  return getContentType(filename);
}

static bool handleFileRead(String path){
  DBG_PRINTLN("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getResponseContentType(path);
  String pathWithGz = path + ".gz";
  if(FileSystem.exists(pathWithGz) || FileSystem.exists(path)){
    if(FileSystem.exists(pathWithGz))
      path += ".gz";
    File file = FileSystem.open(path, "r");
    /*size_t sent = */ server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
#endif

void FileManager::_handleFileUpload(AsyncWebServerRequest *request){
    if(request->hasParam("data", true, true) && _fs.exists(request->getParam("data", true, true)->value()))
      request->send(200, "", "UPLOADED: "+request->getParam("data", true, true)->value());
    else
      request->send(500);
}

void FileManager::_handleFileDelete(AsyncWebServerRequest *request){
    if(request->hasParam("path", true)){
        String path = request->getParam("path", true)->value();
        DBG_PRINTF("handleFileDelete:%s\n",path);
        if(path == "/"){
            request->send(500, "text/plain", "BAD PATH");
        }else{
            if(!_fs.exists(path)){
                return request->send(404, "text/plain", "FileNotFound");
            }else{
                _fs.remove(path);
                request->send(200, "text/plain", "");
            }
        }
    }else{
        request->send(500, "text/plain", "BAD ARGS");
    }
}


void FileManager::_handleFileCreate(AsyncWebServerRequest *request){
    if(request->hasParam("path", true)){
        String filename = request->getParam("path", true)->value();
        if(_fs.exists(filename)){
            request->send(200);
        } else {
            File f = _fs.open(filename, "w");
            if(f){
            f.write((uint8_t)0x00);
            f.close();
            request->send(200, "", "CREATE: "+filename);
        } else {
            request->send(500);
        }
      }
    }
}

void FileManager::_handleDirCreate(AsyncWebServerRequest *request){
    if(request->hasParam("path", true)){
        String filename = request->getParam("path", true)->value();
        if(_fs.exists(filename)){
            request->send(200);
        } else {
            if(_fs.mkdir(filename)){
                request->send(200, "{}");
            } else {
                request->send(500);
            }
      }
    }
}

void FileManager::_handleFileList(AsyncWebServerRequest *request) {
    if(!request->hasParam("dir")) {
        request->send(500, "text/plain", "BAD ARGS"); 
        return;
    }

    String path = request->getParam("dir")->value();
    DBG_PRINTF("handleFileList: %s\n ", path);
    // linked list(queue) is needed. 
    // avoid recursive call, which might open too many directories 
    File dir = _fs.open(path);
 

    String output = "[";

    File entry = dir.openNextFile();
    while(entry){

    if (output != "[") output += ',';
        #if UseLittleFS
        bool isDir = entry.isDirectory();
        #else
        bool isDir = false;
        #endif

        //DBG_PRINTF(" %s [%s]\n ", entry.name(),isDir? "DIR":"FILE");

        output += "{\"type\":\"";
        output += (isDir)? "dir":"file";
        output += "\",\"name\":\"";

        //DBG_PRINTF("%s, %s, cmp:%d, ", entry.name(),path.c_str(), strncmp(entry.name(),path.c_str(),path.length()));
        String name=(entry.name());
        if(name.startsWith(path)){
      
        int exlen =path.length();  
        
        if(path.charAt(exlen - 1) != '/') exlen ++;
            output += name.substring(exlen);
        }else{
            if(entry.name()[0] == '/'){
                output += name.substring(1);
            }else{
                output += name;
            }
        }

        output += "\"}";
        entry.close();  

        entry = dir.openNextFile();

    }

    output += "]";
    DBG_PRINTF(" ret: %s \n",output.c_str());
    request->send(200, "text/json", output);
}




void FileManager::handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index){
        if(!_username.length() || request->authenticate(_username.c_str(),_password.c_str())){
            _authenticated = true;
            request->_tempFile = _fs.open(filename, "w");
        }
    }
    if(_authenticated && request->_tempFile){
        if(len){
            request->_tempFile.write(data,len);
        }
        if(final){
            request->_tempFile.close();
        }
    }
}

void FileManager::handleRequest(AsyncWebServerRequest *request){
    if(_username.length() && _password.length() && !request->authenticate(_username.c_str(), _password.c_str()))
        return request->requestAuthentication();

    if(request->method() == HTTP_GET){
        if(request->url().equals("/list")) _handleFileList(request);
        else if(request->url().equals(_path)){
            AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", (const uint8_t*)edit_htm_gz, edit_htm_gz_len);
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        }
    }else if(request->url().equalsIgnoreCase("/edit")){
       if(request->method() == HTTP_PUT) _handleDirCreate(request);
       if(request->method() == HTTP_POST) _handleFileUpload(request);
       if(request->method() == HTTP_DELETE) _handleFileDelete(request);
    }
}

bool FileManager::canHandle(AsyncWebServerRequest *request){
    if(request->method() == HTTP_GET){
//     s        DBG_PRINTF("canHandle: %s comapre to %s\n",_path.c_str(),request->url().c_str());
        if(request->url().equals("/list")) return true;
        else if(request->url().equals(_path)) return true;
    }else if(request->url().equalsIgnoreCase("/edit")){
       if(request->method() == HTTP_PUT) return true;
       if(request->method() == HTTP_POST) return true;
       if(request->method() == HTTP_DELETE) return true;
    }
    return false;
}

FileManager::FileManager(const fs::FS& fs, const String& path,const String& username, const String& password)
:_fs(fs)
,_path(path)
,_username(username)
,_password(password)
,_authenticated(false)
{}
