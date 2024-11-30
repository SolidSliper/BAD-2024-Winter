#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

// ���� Piece, ��� ��������� �����, ���� "������" �� "������"
class Piece
{
public:
    string head, tail, body;

    void splitNumber(const string& number)
    {
        // ��������� ������ �� ������, ���� ����� �� ������� ���� � 4
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

// ���������� ��� ���������� ������� ��� ����������
unordered_map<string, string> memo;

// ���������� �������� ������ ��������
string findLongestChain(const vector<Piece>& pieces, const string& currentChain, const string& currentTail)
{
    // ��������, �� ��� ��������� ��� ��������� ������
    if (memo.find(currentTail) != memo.end())
    {
        return memo[currentTail];
    }

    string longestChain = currentChain;

    for (size_t i = 0; i < pieces.size(); ++i)
    {
        // ��������, �� �������� ���� ��������� ����� � ������� ������
        if (pieces[i].head == currentTail)
        {
            // ���������� ������ �������
            string newChain = currentChain;

            newChain += pieces[i].body.substr(2);

            vector<Piece> remainingPieces = pieces;
            remainingPieces.erase(remainingPieces.begin() + i);

            // ����������� ����� ���������� �������
            string candidateChain = findLongestChain(remainingPieces, newChain, pieces[i].tail);
            if (candidateChain.length() > longestChain.length())
            {
                longestChain = candidateChain;
            }
        }
    }

    // ���������� ���������� ��� ����� ������
    memo[currentTail] = longestChain;

    return longestChain;
}

// ������� ���������� ����� � �����
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
    // ���������� ����� � �����
    vector<string> numbers = loadNumbersFromFile("source.txt");
    if (numbers.empty())
    {
        cout << "Error. Could not open file." << endl;
        return 1;
    }

    // ��������� ������� �����
    vector<Piece> pieces(numbers.size());
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        pieces[i].splitNumber(numbers[i]);
    }

    string longestChain;

    // ����� ������� ��� ������� �����
    for (const auto& piece : pieces)
    {
        memo.clear(); // �������� 
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