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
    // vector find_all_mathces(std::map<std::string, int> mapOfWordCount, string sentence);
     vector<int> start(string sentence, map<std::string, int> mapOfWordCount);

    // Užkomentuoju, bet gal reikėtu async procesus deti i finished vektoriu, iš kurio veliau rezultatus issitraukti galima.
    // TextOperation::start funkcijoje bandžiau padaryti taip bet gaunu error neiaišku ir nepaėjo to padaryti. 
    // std::vector<vector<int>> finished;
};

/**
 *  @brief: Matcher function that checks if text has value from map
 *  @note: 
 *  @return: returns vectorized ids of words that exist in text
 */
vector<int> find_all_mathces(std::map<std::string, int> mapOfWordCount, string sentence)
{ 
  vector<int> item_ids;
  for (std::pair<std::string, int> element : mapOfWordCount)
  {
    int count = element.second;
    if (sentence.find( element.first) != std::string::npos){
      item_ids.push_back(count);
    }
  }
  return item_ids;
}

/**
 *  @brief: Main function that starts an async proccess and returns a vector with ids of words found in text.
 *  @note: start an async proccess that find map values in provided sentence.
 *  @return: vector<int> object with map ids.
 */
  vector<int> TextOperation::start(string sentence, map<std::string, int> mapOfWordCount){
    // each thread is a std::async running this->find_all_matches():
    future<vector<int>> fut = std::async(std::launch::async, find_all_mathces, mapOfWordCount, sentence);

    // finished.push_back(
    //     std::async(std::launch::async, find_all_mathces, mapOfWordCount, sentence));
    // finished.push_back(std::async(std::launch::async, find_all_mathces, mapOfWordCount, sentence));

    fut.wait();
    return fut.get();
  }

/**
 *  @brief: Reads from defaule example dictionary files. Store values in a map object.
 *  @note: map object containing string value in first place and in as second.
 *  @return: Map with word in dictionary and id.
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
  string sentence = "Need new floor tiles and roof work"; 
  map<std::string, int> mapOfWordCount = test.read_dictionary_file();
  vector<int> ids;
  for (int i = 0; i < 100; i++)
  {
    ids = test.start(sentence, mapOfWordCount);
    // Check if there are any return values
    for (auto i = ids.begin(); i != ids.end(); ++i)
    std::cout << *i << ' ';

  }
  return 0;
}/**/