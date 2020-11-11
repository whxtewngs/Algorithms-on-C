                                                                                     // FuzzySearch ПОЛСЕДНЯЯ ВЕРСИЯ
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono> 

const int MAX_NUMB_OF_EDITING = 1;

void levenshtein_distance(std::vector<std::string>* words, const std::string& search_word, int k);

int main() {

     std::fstream file;
     std::string file_name;
     std::cout << "Enter .txt file name:";
     std::cin >> file_name;
     file.open(file_name, std::ios::in);
     if (file.is_open()) {
         while (true) {
             std::cout << "Enter a search word:";
             std::string search_word;
             std::cin >> search_word;
             std::vector<std::string>* words = new std::vector<std::string>;
             auto begin = std::chrono::steady_clock::now();
             while (!file.eof()) {
                 std::string line_from_file;
                 getline(file, line_from_file);
                 std::string::size_type pos, prev_pos;
                 pos = 0;
                 prev_pos = 0;
                 while ((pos = line_from_file.find_first_of(',', pos)) != std::string::npos)
                 {
                     // сохраним слово 
                     words->push_back(line_from_file.substr(prev_pos, pos - prev_pos));
                     // перепрыгнем на начало следующего слова
                     prev_pos = pos + 2;
                     // увеличим счётчик
                     pos++;
                 }
                 // сохраним последнее слово
                 words->push_back(line_from_file.substr(prev_pos, pos - prev_pos));
                 line_from_file.clear();
                 levenshtein_distance(words, search_word, MAX_NUMB_OF_EDITING);
                 words->clear();
             }
             auto end = std::chrono::steady_clock::now();
             auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
             std::cout << "The time: " << elapsed_ms.count() << " ms\n";
             file.clear();
             file.seekg(0, std::ios::beg);
             delete words;
         }
     }
     else
     {
        std::cout << "File is not opened \n";
        return -1;
     }
     file.close();

     return 0;
}

void levenshtein_distance(std::vector<std::string>* words, const std::string& search_word, int max_editing) {

    int size2 = search_word.size();
    std::vector <std::vector <int>> d(2, std::vector <int>(size2 + 1, max_editing + 1));
    for (int i = 0; i < words->size(); i++) {
        std::string word_from_file = words->at(i);
        int size1 = word_from_file.size();
        int R = max_editing; // правая граница диагонали
        int L = 0; // левая граница диагонали
        for (int i = 0; i <= size1; i++) {
            for (int j = L; j <= R; j++) {
                if (i * j == 0) {
                    d[i & 1][j] = i + j;
                    continue;
                }
                int insert_d = d[i & 1][j - 1] + 1;
                int delete_d = d[(i - 1) & 1][j] + 1;
                int match_d = d[(i - 1) & 1][j - 1] + (word_from_file[i - 1] != search_word[j - 1]);
                d[i & 1][j] = std::min({ insert_d, delete_d, match_d });
            }
            if (std::min({ d[i & 1][L], d[i & 1][L + 1], d[i & 1][R] }) > max_editing) break; // все числа в строке больше max_editing
            if (R < size2) R++;
            if (i + 1 > max_editing && size2 > L + 2 * max_editing) L++;
        }
        if (d[size1 & 1][size2] <= max_editing) {
            std::cout << word_from_file << ", ";
        }
    }
}

