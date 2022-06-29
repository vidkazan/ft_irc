#include "../main.hpp"
#include <vector>


// int length(char *array) {
//     char *start=array;
//     while (*array++);

//     return array-start-1;
// }

// char **split(char *text, char delim) {
//     int words = 1;
//     int len = length(text);

//     for(int i=0; i<len; ++i)
//         if(text[i] == delim) ++words;

//     char **result = new char*[words];

//     int j = 0; 
//     for(int i=0; i<words; ++i){
//         result[i] = new char[len];
//         int t = 0;
//         while(text[j] != delim && text[j] != '\0') result[i][t++] = text[j++];
//         j++;
//     }

//     return result;
// }

/**
 * @brief   Split the string using substr() function
            and adding the splitted word into the vector
 * 
 * @param   data data to split
 * @param   separator separator
 * @return  std::vector<std::string> with splitted words
 */
std::vector<std::string> split(std::string data, std::string separator) {
    std::vector<std::string>    res;
    int                         position;
    std::string                 outstr, token;

    while ((position = data.find(separator)) != std::string::npos) {
        token = data.substr(0, position);
        res.push_back(token);
        data.erase(0, position + separator.length());
    }
    // add last word
    res.push_back(data);
    return res;
}


// std::string *splitNicknames(std::string receiverNames) {

//         // считаем кол-во запятых
//         int n = 0;
//         for (int i = 0; i < receiverNames.length(); i++) {
//             if (receiverNames[i] == ',') {
//                 n++;
//             }
//         }
//         // std::cout << "**** " << n << std::endl;
//         // получаетелей на один больше чем запятых
//         // n - кол-во получателей
//         n++;


// // заменить массив на вектор
// // использовать split

// /*
// PRIVMSG a,c,b :bbbb


// *** a
// *** c,b
// *** b :bbbb
// 0000000000000000 3
// */

//         std::string *names = new std::string[n];

//         // разделяем receiverNames по запятым и кидаем в массив names;
//         if (n == 1) {
//             // если одно имя и нет запятых
//             names[0] = receiverNames;
//         } else {
//             int l = 0;
//             size_t m = 0;
//             for (int i = 0; i < n; i++) {
//                 if (i == 0) {
//                     m = receiverNames.find(',', 0);
//                 } else {
//                     m = receiverNames.find(',', m);
//                 }
//                 names[i] = receiverNames.substr(l, m); // ????? line
//                 l = m + 1;
//             }
//         }

//         for (int i = 0; i < n ; i++) {
//             std::cout << "*** " << names[i] << "\n";
//         }

//         std::cout << "0000000000000000 " << n << std::endl;
// }