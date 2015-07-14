#Introduce:
        This framework is named `winner` what is my wife's name.
        This framework is used to build `network game server groups`.
        Winner is based on epoll, eventfd, c++ atomic and thread.
        Winner only supports Linux os. 
        
#Current Version: 0.0.1

#Winner will help you do these things:
    1. Create service easily and quickly.
        1) Create service by raw message mechanism (derive from `Service`, see `Service` and `LogService` for more details);
        2) Create service by traditional rpc callback(derive from `CallbackService`, see `CallbackService` and `S2CallbackService` for more details);
        3) Create service by modern rpc coroutine(derive from `CoroutineService`, see `CoroutineService` and `S3CoroutineService` for more details);
        4) Create service by modern lua coroutine(see s1);
        5) Create http service by coroutine, support HTML TEMPLATE ENGINE. 
    2. Depoly service more flexible(see examples).
    3. Service can split in one node or in many nodes(see examples).
    4. Monitor mechanism, every thread has a thread local monitor(instance of Monitor), can used to monitor every thing which is derived from Monitor;
    5. Dispatch message easily, see `DispatcherManager`;
    6. Listen and connect easily, see `Network`;
    7. Easily Manage service, see `ServiceManager`;
    8. Some tools:
        1) protocol generator, see `./tool/protocol`;
        2) cpp <=> lua, see `./tool/protocol`;
        2) class code generator, see `./tool/template/class`;
    9. Many class for develope.

#Install:
    run `./build.sh`
    
#Testing Case:
    See `examples`

#Attention:
    Before build this framework, you need install some thirdpart package:
        1) valgrind: ensure framework can found `<valgrind/valgrind.h>`
        2) openssl: ensure framework can found `<openssl/md5.h>` and `<openssl/sha.h>`
        3) libmysqlclient: ensure framework can found `<mysql.h>`

#Status:
    I have done some basic testing, and this framework need more testing.

#Road Map:
    1) will add documents and examples;
    2) will optimize performance.

#Best Hope:
    I hope you who are reading this file will help `winner` to be more better.
