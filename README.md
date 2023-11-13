# HTTP/1.0 web framework written in C

### About:
Фреймворк, позволяющий создавать HTTP сервера, написанный на языке С.
Фреймворк абстрагирует от пользователя процесс создания сервера и приёма запросов, их парсинга, маршрутизации и отправки ответа

Функционал фреймворка включает: 
- Задание ip, порта сервера
- Добавление функций-хендлеров

Внутри функций-хендлеров: 
- Доступ к информации HHTP запроса: версии, хедеров и тела запроса
- Возможность записи статус-кода, хендлеров
- Возможность записи тела ответа как целиком, так и по частям

### Quickstart:

```shell
cd examples
make -s run [EXAMPLE]
```

### Usage:

Создание сервера  
```c
const unsigned short PORT = [PORT];
const char IP[] = [IP];
ChadtpServer *server = ChadtpServer_new(PORT, IP);
```

Добавление хендлера:  
Для добавления хендлеров - функция ChadtpServer_add_handler  
Срабатывает наиболее специфичный хендлер, например:  
Имея хендлер А для URL "/*" и хендлер B для URL "/api/*", при запросе с URL "/api/" всегда сработает хендлер B  

Параметры: 
ChadtpServer* - указатель на созданный в предыдущем пункте сервер
char* - путь, по которому этот хендлер срабатывает. 
Поддерживает шаблоны, например - "/*" - срабатывает для всех URL, начинающихся с "/".  
HandlerFunction* - указатель на функцию-хендлер  
```c
ChadtpServer_add_handler(server, "/", echo_handler);
```

Хендлер - функция с двумя параметрами:  
HTTPRequest* - указатель на получаемый запрос  
HTTPResponse* - указатель на ответ, который отправит сервер после выполнения хендлера  
```c
int example_handler(HTTPRequest *req, HTTPResponse *res) {
    HTTPResponse_write_header(res, "ServerName", "Chadttp");
    HTTPResponse_write_status_code(res, CHADTP_OK);
    HTTPResponse_write(res, "<h1>Hello</h1>");
    return 0;
}
```
