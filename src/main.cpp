#include "node.hpp"
void callback(std::string data){
    std::cout << data << std::endl;
}
void callback2(std::string data){
    std::cout << data << std::endl;
}
int main(int argc, char** argv){
    fake_node::nodehandle nh("127.0.0.1", 1843);
    fake_node::publisher *pub = nh.advertise(5);
    fake_node::publisher *pub_1 = nh.advertise(6);
    nh.subscribe(6, callback2);
    nh.subscribe(5, callback);
    while(true){
        pub->publish("sqw");
        pub_1->publish("sqw2");
        sleep(1);
    }
    return 0;
}