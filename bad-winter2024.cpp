#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

// Клас Piece, для зберігання числа, його "хвоста" та "голови"
class Piece
{
public:
    string head, tail, body;

    void splitNumber(const string& number)
    {
        // Отримання голови та хвоста, якщо число має довжину хоча б 4
        if (number.size() >= 4)
        {
            head = number.substr(0, 2);  
            tail = number.substr(number.size() - 2, 2);  
            body = number;  
        }
        else
        {
            head = ""; 
            tail = ""; 
            body = number;  
        }
    }
};

// Збереження вже обчислених ланцюгів для оптимізації
unordered_map<string, string> memo;

// Рекурсивна програма пошуку ланцюжків
string findLongestChain(const vector<Piece>& pieces, const string& currentChain, const string& currentTail)
{
    // Перевірка, чи вже обчислено для поточного хвоста
    if (memo.find(currentTail) != memo.end())
    {
        return memo[currentTail];
    }

    string longestChain = currentChain;

    for (size_t i = 0; i < pieces.size(); ++i)
    {
        // Перевірка, чи збігається хвіст поточного числа з головою іншого
        if (pieces[i].head == currentTail)
        {
            // Формування нового ланцюга
            string newChain = currentChain;

            newChain += pieces[i].body.substr(2);

            vector<Piece> remainingPieces = pieces;
            remainingPieces.erase(remainingPieces.begin() + i);

            // Рекурсивний пошук наступного ланцюга
            string candidateChain = findLongestChain(remainingPieces, newChain, pieces[i].tail);
            if (candidateChain.length() > longestChain.length())
            {
                longestChain = candidateChain;
            }
        }
    }

    // Збереження результату для цього хвоста
    memo[currentTail] = longestChain;

    return longestChain;
}

// Функція зчитування чисел з файлу
vector<string> loadNumbersFromFile(const string& filename)
{
    vector<string> numbers;
    ifstream file(filename);

    if (!file)
        return numbers;

    string number;
    while (file >> number)
    {
        numbers.push_back(number);
    }

    file.close();
    return numbers;
}

int main()
{
    // Зчитування чисел з файлу
    vector<string> numbers = loadNumbersFromFile("source.txt");
    if (numbers.empty())
    {
        cout << "Error. Could not open file." << endl;
        return 1;
    }

    // Створення вектора пазлів
    vector<Piece> pieces(numbers.size());
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        pieces[i].splitNumber(numbers[i]);
    }

    string longestChain;

    // Пошук ланцюгів для кожного пазлу
    for (const auto& piece : pieces)
    {
        memo.clear(); // Очищення 
        string chain = findLongestChain(pieces, piece.body, piece.tail);
        if (chain.length() > longestChain.length())
        {
            longestChain = chain;
        }
    }

    cout << "The longest sequence: " << longestChain << endl << endl;
    cout << "Lenght of sequence: " << longestChain.length() << endl;
    return 0;
}