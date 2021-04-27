#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iterator>

const char *get_word(const char **string_ptr)
{
    const char *start;
    char *word = NULL;
    unsigned int word_len;

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

int main()
{
    std::string sentence = "Need new floor tiles and roof work"; // Taskai neveikia
    const char *cStr = sentence.c_str();

    const char *word;
    std::vector<const char *> word_vector;

    // Reiktu užkrauti iš file ir padaryti į 3d masyva
    std::map<std::string, int> mapOfWordCount;
    mapOfWordCount.insert(std::pair<std::string, int>("floor", 0));
    mapOfWordCount.insert(std::pair<std::string, int>("work", 1));
    mapOfWordCount.insert(std::pair<std::string, int>("gay", 2));

    std::map<std::string, int>::iterator it = mapOfWordCount.begin();

    std::vector<bool> final_vector(64, 0);

    while (*cStr)
    {
        word = get_word(&cStr);
        word_vector.push_back(word); // Kol kas biski useless taip daryti, bet pravers jei ne binary vektorius bus naudojamas
    }

    for (auto i = word_vector.begin(); i != word_vector.end(); ++i)
    {
        std::string word_one(*i);

        for (std::pair<std::string, int> element : mapOfWordCount)
        {
            std::string word_two = element.first;
            int count = element.second;

            if (word_one.find(word_two) != std::string::npos)
            { // galima naudoti boost biblioteka
                std::cout << word_two << " position: " << count << std::endl;
                final_vector[count] = 1;
            }
        }
    }
    for (std::vector<bool>::const_iterator h = final_vector.begin(); h != final_vector.end(); ++h)
        std::cout << *h << ""; // pasitikrinti ar suveike

    return 0;
}