## Word matcher main functions

To use this code you have to have a file with the name example.txt which contains map values (string, int) in every new line

To generate this file if you dont have one you can run the python script dataset_preparator.py. For the script to work you have to gave bbc-text.csv file. Its download link can be found in: https://www.kaggle.com/yufengdev/bbc-text-categorization.

To run the main cpp program first build the project using  g++ -o matcher.exe .\word_matcher.cpp, then run the resulting ./matcher.exe file.