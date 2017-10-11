#include <iostream>
#include <cstring>

using namespace std;

std::string serialize(void* ptr, int size);
void* deserialize(std::string encoded);
bool compare(string ptr1, string ptr2);

struct node
{
    void* value;
};

int main()
{
    node mynode;

    char* num = "helloworld";
    void* pNum = &num;
    void* pNum2 = &num;

    string hola = serialize(pNum, sizeof(char*));
    string hola2 = serialize(pNum2, sizeof(char*));

    mynode.value = deserialize(hola2);

    int n = memcmp(pNum, deserialize(hola), sizeof(char*));

    cout << n << endl;

    cout << compare(hola, serialize(mynode.value, sizeof(char*))) << std::endl;

    void* ptr = deserialize(hola);

    cout << *((char**) ptr) << endl;

    return 0;
}

std::string serialize(void* ptr, int size)
{
    std::string final;

    char* data = (char*) ptr;

    for (int i = 0; i < size; ++i)
    {
        final += data[i];
    }

    return final;
}

void* deserialize(std::string encoded)
{
    char* data = (char*) encoded.c_str();

    return (void*) data;
}

bool compare(string ptr1, string ptr2)
{
    for (int i = 0; i < ptr1.size(); ++i)
    {
        if (ptr1[i] != ptr2[i])
        {
            return false;
        }

        cout << ptr1[i] << " - " << ptr2[i] << endl;
    }

    return true;
}
