#pragma once
#include "util.h"

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <algorithm>

enum CHECK_STATE    { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER };

enum LINE_STATUS    { LINE_OK = 0, LINE_BAD,LINE_OPEN };

enum HTTP_CODE  { NO_REQUEST, GET_REQUEST, BAD_REQUEST, FORBIDDEN_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };

enum HttpMethod { METHOD_POST = 1, METHOD_GET, METHOD_HEAD };

enum HttpVersion { HTTP_10 = 1, HTTP_11 };

class Http
{
public:
    Http(const int& );

    void http_loop();
    LINE_STATUS parse_line();
    HTTP_CODE parse_requestline();
    HTTP_CODE parse_headers();
    HTTP_CODE parse_content();
private:
    int req_method;
    std::string file_path;
    int http_version;
    std::string client_host;
    std::string accept_language;
    std::string user_agent;
    CHECK_STATE checkstate;//此时对请求的检查状态

    std::string buffer;//Http请求数据
    int req_len;//http请求的总长度
    int now_pos;//解析的当前位置
    int start_pos;//每一行的开始位置
    int client_fd;//客户端的描述符
};


