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
#include <algorithm>

using namespace std;


class TextOperation {
  vector<int> unique_classes{0,1,2,3,4};

  public:
    std::map<std::string, int> read_dictionary_file();
    // vector<int> find_all_mathces(std::map<std::string, int> mapOfWordCount, string sentence);
    vector<pair<int,int>> start(string sentence, map<std::string, int> mapOfWordCount);
    std::vector<future<vector<int>>> finished;

    
};

bool sortByVal(const pair<int, int> &a, 
               const pair<int, int> &b) 
{ 
    return (a.second > b.second); 
} 
/**
 *  @brief: Matcher function that checks if text has value from map
 *  @note: 
 *  @return: returns vectorized ids of words that exist in text
 */
vector<pair<int,int>> find_all_mathces(std::map<std::string, int> mapOfWordCount, string sentence, vector<int> unique_classes)
    { 
      vector<int> item_ids;
      vector<pair<int,int>> occurence;

      for (std::pair<std::string, int> element : mapOfWordCount)
      {
        int count = element.second;
        if (sentence.find( element.first) != std::string::npos){
          item_ids.push_back(count / 100);
        }    
      }

      for (auto i = unique_classes.begin(); i != unique_classes.end(); ++i) {
          occurence.push_back(make_pair(*i, count(item_ids.begin(), item_ids.end(), *i)));
        }

      sort(occurence.begin(), occurence.end(), sortByVal); 
      return occurence;
    }


/**
 *  @brief: Main function that starts an async proccess and returns a vector with ids of words found in text.
 *  @note: start an async proccess that find map values in provided sentence.
 *  @return: vector<int> object with map ids.
 */
vector<pair<int,int>> TextOperation::start(string sentence, map<std::string, int> mapOfWordCount){
    future<vector<pair<int,int>>> fut = std::async(std::launch::async, find_all_mathces, mapOfWordCount, sentence, unique_classes);
    fut.wait();
    return fut.get();
  }

/**
 *  @brief: Reads from defaule example dictionary files. Store values in a map object.
 *  @note: map object containing string value in first place and in as second.
 *  @return: Map with word in dictionary and id.
 */
std::map<std::string, int> TextOperation::read_dictionary_file(){
  const char *fileName="word_list.txt";
  ifstream paramFile;
  paramFile.open(fileName);
  string key;
  int value;
  int count = 0;
  std::map<std::string, int> mapOfWordCount;

  while ( paramFile >> key >> value ) {
    mapOfWordCount.insert(pair<string, int>(key, count)); // input them into the map 
    count++;
  }

  return mapOfWordCount;
}




// Reiktu manau paskutini dalyka padaryti tai priderinti prie tavo atsiusto json, bet tam reiktu labiau pergeneruoti word_list file negu kazka kodiskai keisti.
// Kol kas sugeneravau random klases pagal bbc dataset. ['business' 'entertainment' 'politics' 'sport' 'tech']
int main()
{
  TextOperation test;
  string sentence = "Need new floor tiles and roof work, year, tv, want web"; 
  
  map<std::string, int> mapOfWordCount = test.read_dictionary_file();
  // ['business' 'entertainment' 'politics' 'sport' 'tech']
  vector<pair<int,int>> ids;

  for (int i = 0; i < 1; i++)
  {
    // callini funkcija ir gauni subgrubiu ids
    ids = test.start(sentence, mapOfWordCount);

    // Siaip print kad paziureti ka gavai
    cout << "The vector, sorted by value is: " << endl;
    for (int i = 0; i < ids.size(); i++)
      {
        cout << ids[i].first << ": " << ids[i].second << endl;
      }

      
  }

  return 0;
}