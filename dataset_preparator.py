import os 
import pandas as pd
import numpy as np
from collections import Counter

data = pd.read_csv('bbc-text.csv')
# print(data.category)

data_filtered = data.loc[data['category'] == 'tech']
tech_word_list = []
for index, row in data_filtered.iterrows():
    unique_words = row.text.split(' ')
    tech_word_list += unique_words

occurances = sorted(Counter(tech_word_list).items(),  key=lambda item: item[1], reverse=True)

ignore_list = ['', 'the', 'to', 'of', 'and', 'a', 'in', 'that', 'is', 'it', 'for', 'on', 'be', 'are', 'as', 'with', 's', 'will', 'said', 'have', 'by', 'has', 'they', 'was', 'more', 'people', 'at', 'not', 'but',
               'which', 'can', 'this', 'from', 'he', 'or', 'their', 'an', 'its', 'you', 'about', 'also', 'than', 'new', '-', 'up', 'mr', 'been', 'we', 'were', 'one', 'would', 'could', 'technology', 'there', 'i',
                'other', 'users', 'out', 'such', 'all', 'some', 'many', 'use', 'if', 'what', 'who', 'us', 'so', 'had', 'like', 'net', 'said.', 'make', 'get', 'when', 'into', 
                'over', 'them', 'now', 'do', 'being', 'just', 'used', 'how', 'most', 'uk', 'way', 'world', 'first', 'those', '.', 'very', 'my', 't']

with open('example.txt', 'w') as example:
    for x, word in enumerate(occurances):
        # ignore_list.append(word[0])
        if not word[0] in ignore_list:
            example.write(f'{word[0]} {word[1]} \n')
        if x == 200:
            break

print(ignore_list)