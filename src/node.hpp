#pragma once
#include <nlohmann/json.hpp>
#include "sockpp/tcp_connector.h"
#include "sockpp/version.h"
#include <thread>
#include <cstring>
#include <string>
#include <chrono>
#include <unistd.h>

namespace fake_node{
    class nodehandle;
    class publisher{
        private:
            int topic_id;
        public:
            publisher(int);
            ~publisher(){};
            bool publish(std::string);
    };

    class subscriber{
        public:
            subscriber(nodehandle *node, int topic_id, void(*)(std::string));
            void(*cb_func)(std::string);
    };

    class nodehandle{
        private:
            std::thread *ptr_bgloop;
            std::map<int, void(*)(std::string)> callback_m;
        public:
            nodehandle(std::string, in_port_t);
            void init(std::string, in_port_t);
            publisher *advertise(int);
            subscriber *subscribe(int, void(*)(std::string));
            void bgloop(void);
    };
};