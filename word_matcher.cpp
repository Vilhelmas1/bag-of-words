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


struct tasks {

  // the mutex, condition variable and deque form a single
  // thread-safe triggered queue of tasks:
  std::mutex m;
  std::condition_variable v;
  // note that a packaged_task<void> can store a packaged_task<R>:
  std::deque<std::packaged_task<void()>> work;

  // this holds futures representing the worker threads being done:
  std::vector<std::future<void>> finished;

  // queue( lambda ) will enqueue the lambda into the tasks for the threads
  // to use.  A future of the type the lambda returns is given to let you get
  // the result out.
  template<class F, class R=std::result_of_t<F&()>>
  std::future<R> queue(F&& f) {
    // wrap the function object into a packaged task, splitting
    // execution from the return value:
    std::packaged_task<R()> p(std::forward<F>(f));

    auto r=p.get_future(); // get the return value before we hand off the task
    {
      std::unique_lock<std::mutex> l(m);
      work.emplace_back(std::move(p)); // store the task<R()> as a task<void()>
    }
    v.notify_one(); // wake a thread to work on the task

    return r; // return the future result of the task
  }

  // start N threads in the thread pool.
  void start(std::size_t N=1){
    for (std::size_t i = 0; i < N; ++i)
    {
      // each thread is a std::async running this->thread_task():
      finished.push_back(
        std::async(
          std::launch::async,
          [this]{ thread_task(); }
        )
      );
    }
  }
  // abort() cancels all non-started tasks, and tells every working thread
  // stop running, and waits for them to finish up.
  void abort() {
    cancel_pending();
    finish();
  }
  // cancel_pending() merely cancels all non-started tasks:
  void cancel_pending() {
    std::unique_lock<std::mutex> l(m);
    work.clear();
  }
  // finish enques a "stop the thread" message for every thread, then waits for them:
  void finish() {
    {
      std::unique_lock<std::mutex> l(m);
      for(auto&&unused:finished){
        work.push_back({});
      }
    }
    v.notify_all();
    finished.clear();
  }
  ~tasks() {
    finish();
  }
private:
  // the work that a worker thread does:
  void thread_task() {
    while(true){
      // pop a task off the queue:
      std::packaged_task<void()> f;
      {
        // usual thread-safe queue code:
        std::unique_lock<std::mutex> l(m);
        if (work.empty()){
          v.wait(l,[&]{return !work.empty();});
        }
        f = std::move(work.front());
        work.pop_front();
      }
      // if the task is invalid, it means we are asked to abort:
      if (!f.valid()) return;
      // otherwise, run the task:
      f();

    }
  }
};
std::map<std::string, int> read_json_file(){
  const char *fileName="example.txt";
  ifstream paramFile;
  paramFile.open(fileName);
  string key;
  int value;
  std::map<std::string, int> mapOfWordCount;
  // cout << value ;
  while ( paramFile >> key >> value ) {
    mapOfWordCount.insert(pair<string, int>(key, value)); // input them into the map 
  }
return mapOfWordCount;
}

void find_all_mathces()
{
     
  std::map<std::string, int> mapOfWordCount = read_json_file();
  string sentence = "Need new floor tiles and roof work"; // Taskai neveikia

  for (std::pair<std::string, int> element : mapOfWordCount)
  {
    int count = element.second;
    if (sentence.find( element.first) != std::string::npos){
      cout << element.first <<endl;
    }
  }
}


int main()
{

  tasks test;
  test.start(15);

  for (int i = 0; i < 100; i++)
  {
    test.queue(find_all_mathces);

  }
  return 0;
}