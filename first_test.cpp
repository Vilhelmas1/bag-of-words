#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <grpcpp\ext\proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

using namespace std::chrono;
using namespace std;

std::mutex mtx;
std::condition_variable condition;
std::queue<int> Queue;

bool terminate_pool = false;

const char *get_word(const char **string_ptr)
{
    const char *start;
    char *word = NULL;
    unsigned int word_len;
    // std::cout << string_ptr << "";

    if ((string_ptr == NULL) || (*string_ptr == NULL) || (**string_ptr == '\0'))
    {
        return NULL;
    }

    // Skip over any non-alpha characters
    while ((**string_ptr != '\0') && (!isalpha(**string_ptr)))
    {
        (*string_ptr)++;
    }

    if (**string_ptr == '\0')
    {
        return NULL;
    }

    // Make note of where this word starts
    start = *string_ptr;

    while (isalpha(**string_ptr))
    {
        (*string_ptr)++;
    }

    word_len = (*string_ptr) - start;
    if (word_len)
    {
        word = (char *)malloc(word_len + 1);
        strncpy(word, start, word_len);
        word[word_len] = '\0'; // Always NUL terminate strings when using strncpy()
    }
    return word;
}


const void map_to_dict(std::string sentence,  std::map<std::string, int> mapOfWordCount){
    const char *cStr = sentence.c_str();

        const char *word;
        std::vector<const char *> word_vector;
        // Reiktu užkrauti iš file ir padaryti į 3d masyva
        std::map<std::string, int>::iterator it = mapOfWordCount.begin();

        std::vector<bool> final_vector(64, 0);

        while (*cStr)
        {
            word = get_word(&cStr);
            std::string word_one(word);
            word_vector.push_back(word); // Kol kas biski useless taip daryti, bet pravers jei ne binary vektorius bus naudojamas
            for (std::pair<std::string, int> element : mapOfWordCount)
            {
                std::string word_two = element.first;
                int count = element.second;

                if (word_one.find(word_two) != std::string::npos)
                { // galima naudoti boost biblioteka
                    // std::cout << word_two << " position: " << count << std::endl;
                    final_vector[count] = 1;
                }
            }
        }
    std::terminate;

}

void print_num(int i)
{
    std::cout << i << "/";
}

namespace The_Pool
{
    void Infinite_loop_function(int thread_num);
    void Add_job();
};


void The_Pool:: Infinite_loop_function(int thread_num)
{
    bool check;
    while(true)
    {
        unique_lock<mutex> lock(mtx);
        condition.wait(lock, []{return !Queue.empty() || terminate_pool;}); // Panasu kad tik vienas thread veikia vienu metu. Kol kas nežinau kodėl taip yra
        int some = Queue.front();

        cout << "Some int " << some  << ", working thread: " << thread_num << endl; // Random veiksmas, kad lengviau būtų thread testuotis. Reiks pakeisti į map_to_dict

        Queue.pop(); 
        // map_to_dict(sentence, mapOfWordCount);
        // print_num(thread_num); // function<void()> type
    }
};

// // Logic and data behind the server's behavior.
// class GreeterServiceImpl final : public Greeter::Service {
//   Status SayHello(ServerContext* context, const HelloRequest* request,
//                   HelloReply* reply) override {
//     std::string prefix("Hello ");
//     reply->set_message(prefix + request->name());
//     return Status::OK;
//   }
// };

// void RunServer() {
//   std::string server_address("0.0.0.0:50051");
//   GreeterServiceImpl service;

//   grpc::EnableDefaultHealthCheckService(true);
//   grpc::reflection::InitProtoReflectionServerBuilderPlugin();
//   ServerBuilder builder;
//   // Listen on the given address without any authentication mechanism.
//   builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//   // Register "service" as the instance through which we'll communicate with
//   // clients. In this case it corresponds to an *synchronous* service.
//   builder.RegisterService(&service);
//   // Finally assemble the server.
//   std::unique_ptr<Server> server(builder.BuildAndStart());
//   std::cout << "Server listening on " << server_address << std::endl;

//   // Wait for the server to shutdown. Note that some other thread must be
//   // responsible for shutting down the server for this call to ever return.
//   server->Wait();
// }

int main()
{
    int Num_Threads = thread::hardware_concurrency();
    // int Num_Threads = 1;
    string sentence = "Need new floor tiles and roof work"; // Taskai neveikia
    
    map<string, int> mapOfWordCount;
    mapOfWordCount.insert(pair<string, int>("floor", 0));
    mapOfWordCount.insert(pair<string, int>("work", 1));
    mapOfWordCount.insert(pair<string, int>("gay", 2));

    vector<thread> Pool;

    for(int ii = 0; ii < Num_Threads; ii++)
    {  
        Pool.emplace_back(&The_Pool::Infinite_loop_function, ii);
    }

    

    for(int i = 0; i < 1000; i++) // make a listener function
    {    
        unique_lock<mutex> lock(mtx);
        Queue.push(i);
        condition.notify_one();

    }
    // unique_lock<mutex> lock(mtx);

    for(auto& thread:Pool){
        thread.join();
    }


}

