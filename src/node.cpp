#include "node.hpp"

sockpp::tcp_connector *ptr_conn;

void fake_node::nodehandle::init(std::string host, in_port_t port){
    ptr_conn = new sockpp::tcp_connector({host, port});
    if(!(*ptr_conn)){
        std::cout << "connect fail" << std::endl;
        return;
    }
}

fake_node::nodehandle::nodehandle(std::string host, in_port_t port){
    this->init(host, port);
    ptr_bgloop = new std::thread(&fake_node::nodehandle::bgloop, this);
}


fake_node::publisher::publisher(int topic_id){
    this->topic_id = topic_id;
}

bool fake_node::publisher::publish(std::string data){
    std::string pub_data = std::string("{\"header\": 2, \"topic_id\": 5, \"body\": ");
    pub_data.append("\"");
    pub_data.append(data);
    pub_data.append("\"");
    pub_data.append("}");
    ptr_conn->write(pub_data);
    sleep(1);
    return true;
}

fake_node::publisher *fake_node::nodehandle::advertise(int topic_id){
    return new publisher(topic_id);
    // ptr_conn->write("{ \"header\": 2, \"topic_id\": 5, \"body\": " + "\"" + data + "\"" + "}");
}

fake_node::subscriber *fake_node::nodehandle::subscribe(int topic_id, void(*callback)(std::string)){
    std::string pub_data("{ \"header\": 1, \"topic_id\": ");
    pub_data.append(std::to_string(topic_id));
    pub_data.append(", \"body\": \"2.1\"}");
    ptr_conn->write(pub_data);
    callback_m[topic_id] = callback;
    sleep(1);
}

void fake_node::nodehandle::bgloop(void){
    while(true){
        ssize_t n;
        char buf[256] = {0};
        if ((n = ptr_conn->read(buf, sizeof(buf))) > 0){
            buf[n+1] = '\0';
            nlohmann::basic_json<> recv_json = nlohmann::json::parse(std::string(buf));
            int topic_id = recv_json["topic_id"];
            std::string body = recv_json["body"];
            callback_m[topic_id](body);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}