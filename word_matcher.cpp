#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;


class TextOperation {
  public:
    std::map<std::string, int> read_dictionary_file();
    void find_all_mathces(std::map<std::string, int> mapOfWordCount, string sentence);
    void queue(string f);
    void start(std::size_t N=1);
    void abort();
    void cancel_pending();
    void finish(); // finish enques a "stop the thread" message for every thread, then waits for them:
    //~TextOperation();

    // the mutex, condition variable and deque form a single
    // thread-safe triggered queue of tasks:
    std::mutex m;
    std::condition_variable v;
    // note that a packaged_task<void> can store a packaged_task<R>:
    std::deque<std::string> work;

    // this holds futures representing the worker threads being done:
    std::vector<std::future<void>> finished;

  private:
    void thread_task(); // the work that a worker thread does:
};


/**
 *  @brief:
 *  @note:
 *  @return:
 */
void TextOperation::find_all_mathces(std::map<std::string, int> mapOfWordCount, string sentence)
{     
  for (std::pair<std::string, int> element : mapOfWordCount)
  {
    int count = element.second;
    if (sentence.find( element.first) != std::string::npos){
      cout << element.first <<endl;
    }
  }
}

/**
 *  @brief:
 *  @note:
 *  @return:
 */
  void TextOperation::queue(string f) {
    {
      std::unique_lock<std::mutex> l(m);
      work.emplace_back(std::move(f)); // store the task<R()> as a task<void()>
    }
    v.notify_one(); // wake a thread to work on the task
  }

/**
 *  @brief:
 *  @note: start N threads in the thread pool.
 *  @return:
 */
  void TextOperation::start(std::size_t N){
    for (std::size_t i = 0; i < N; ++i)
    {
      // each thread is a std::async running this->thread_task():
      finished.push_back(std::async( std::launch::async, [this]{ thread_task(); }));
    }
  }

/**
 *  @brief:
 *  @note:    abort() cancels all non-started tasks, and tells every working thread
              stop running, and waits for them to finish up.
 *  @return:
 */
  void TextOperation::abort() {
    cancel_pending();
    finish();
  }


/**
 *  @brief:
 *  @note: cancel_pending() merely cancels all non-started tasks:
 *  @return:
 */
  void TextOperation::cancel_pending() {
    std::unique_lock<std::mutex> l(m);
    work.clear();
  }


/**
 *  @brief:
 *  @note:
 *  @return:
 */
  // finish enques a "stop the thread" message for every thread, then waits for them:
  void TextOperation::finish() {
      std::unique_lock<std::mutex> l(m);
      for(auto&&unused:finished){
        work.push_back({});
      }
    }


   /* v.notify_all();
    finished.clear();
  }
  ~tasks() {
    finish();/**/
  
/**
 *  @brief:
 *  @note:
 *  @return:
 */
  void TextOperation::thread_task() {
    TextOperation op;
    std::map<std::string, int> mapOfWordCount = op.read_dictionary_file();
    while(true){
      // pop a task off the queue:
      std::string f;
      {
        // usual thread-safe queue code:
        std::unique_lock<std::mutex> l(m);
        if (work.empty()){
          v.wait(l,[&]{return !work.empty();});
        }
        f = std::move(work.front());
        find_all_mathces(mapOfWordCount, f);
        work.pop_front();
      }
    }
  }

/**
 *  @brief:
 *  @note:
 *  @return:
 */
std::map<std::string, int> TextOperation::read_dictionary_file(){
  const char *fileName="example.txt";
  ifstream paramFile;
  paramFile.open(fileName);
  string key;
  int value;
  std::map<std::string, int> mapOfWordCount;

  while ( paramFile >> key >> value ) {
    mapOfWordCount.insert(pair<string, int>(key, value)); // input them into the map 
  }

  return mapOfWordCount;
}



int main()
{
  TextOperation test;
  test.start(15);
  string sentence = "Need new floor tiles and roof work"; 

  for (int i = 0; i < 100; i++)
  {
    test.queue(sentence);

  }
  return 0;
}/**/