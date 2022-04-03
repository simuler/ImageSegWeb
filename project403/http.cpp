#include "http.h"

Http::Http(const int &fd)
    : req_method(METHOD_GET), file_path("/"), http_version(HTTP_10),checkstate(CHECK_STATE_HEADER),
      client_host("localhost"), accept_language("cn"), user_agent(""), req_len(0), now_pos(0), start_pos(0), client_fd(fd)
{
    req_len = readn(fd, buffer); 
}

LINE_STATUS Http::parse_line()
{
    char temp;
    for (; now_pos < req_len; now_pos++)
    {
        temp = buffer[now_pos];

        if (temp == '\r')
        {
            if (now_pos + 1 == req_len)
                return LINE_OPEN;
            else if (buffer[now_pos + 1] == '\n')
            {
                buffer[now_pos++] = '\0';
                buffer[now_pos++] = '\0';
                return LINE_OK;
            }
            else
                return LINE_BAD;
        }
        else if (temp == '\n')
        {
            if (now_pos > 1 && buffer[now_pos - 1] == '\r')
            {
                buffer[now_pos - 1] = '\0';
                return LINE_OK;
            }
            else
                return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

HTTP_CODE Http::parse_requestline()
{
    //解析方法
    int url_pos = buffer.find(' ', start_pos);
 
    if (url_pos == std::string::npos)
        return BAD_REQUEST;

    url_pos++;
    if (buffer.find("GET", start_pos) == 0)
    {
        req_method = METHOD_GET;
        std::cout << "req method is get" << std::endl;
    }
    else if (buffer.find("POST", start_pos) == 0)
    {
        req_method = METHOD_POST;
        std::cout << "req method is post" << std::endl;
    }
    else
        return BAD_REQUEST;
    //解析路径
    int path_pos = buffer.find(' ', url_pos);

    if (path_pos == std::string::npos)
        return BAD_REQUEST;

    path_pos++;
    if (buffer.find('/', url_pos) == url_pos)
    {
        file_path = buffer.substr(url_pos,path_pos-1-url_pos);
        std::cout << "req file path is " <<file_path<< std::endl;
    }else
        return BAD_REQUEST;
    //解析http版本
    int version_pos = buffer.find(' ', path_pos);
    if (version_pos == std::string::npos)
        return BAD_REQUEST;

    if (buffer.find("HTTP/1.0", path_pos) == path_pos)
    {
        http_version = HTTP_10;
        std::cout << "http version is http1.0" << std::endl;
    }
    else if (buffer.find("HTTP/1.1", path_pos) == path_pos)
    {
        http_version = HTTP_11;
        std::cout << "http version is http1.1" << std::endl;
    }else
        return BAD_REQUEST; 
    checkstate = CHECK_STATE_HEADER;
}

HTTP_CODE Http::parse_headers()
{
    if(buffer[start_pos] == '\0')
        return GET_REQUEST;
    else if(buffer.find("Host:", start_pos) == start_pos){
        int host_pos = buffer.find('\0',start_pos);
        client_host = buffer.substr(start_pos,host_pos-start_pos);
        std::cout << "client host is " <<client_host<< std::endl;
    }else if(buffer.find("User-Agent:", start_pos) == start_pos){
        int user_pos = buffer.find('\0',start_pos);
        user_agent = buffer.substr(start_pos,user_pos-start_pos);
        std::cout << "user-agent is " <<user_agent<< std::endl;
    }else if(buffer.find("Accept-Language:", start_pos) == start_pos){
        int language_pos = buffer.find('\0',start_pos);
        accept_language = buffer.substr(start_pos,language_pos-start_pos);
        std::cout << "accept_language is " <<accept_language<< std::endl;
    }else
        std::cout << "I can`t handle this header"<< std::endl;
}

HTTP_CODE Http::parse_content()
{
    LINE_STATUS linestatus = LINE_OK;
    HTTP_CODE recode = NO_REQUEST;

    while((linestatus = parse_line()) == LINE_OK){
      
        switch(checkstate){
            case CHECK_STATE_REQUESTLINE:{
                recode = parse_requestline();
                if(recode == BAD_REQUEST)
                    return BAD_REQUEST;
                break;
            }
            case CHECK_STATE_HEADER:{
                recode = parse_headers();
                if(recode == BAD_REQUEST)
                    return BAD_REQUEST;
                else if(recode == GET_REQUEST)
                    return GET_REQUEST;
                break;
            }
            default:
                return INTERNAL_ERROR;
        }
        start_pos = now_pos;
    }
    
    if(linestatus == LINE_OPEN)
        return NO_REQUEST;
    else
        return BAD_REQUEST;
}

void Http::http_loop(){
    checkstate = CHECK_STATE_REQUESTLINE;
    while(1){
        HTTP_CODE result = parse_content();

        if(result == NO_REQUEST)
            continue;
        else if(result == GET_REQUEST){
            std::string temp = "OK";
    
            writen(client_fd, temp);
            break;
        }else{
            std::string temp = "ERROR";
            ssize_t le = writen(client_fd, temp); 
            break;
        }
    }
    close(client_fd);
}