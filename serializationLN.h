#include <string>
#include <cstdio>

using std::string;


struct ListNode{
    ListNode *prev;
    ListNode *next;
    ListNode *rand;

    string data;
};

class List{
public:
    List();
    ~List();


    void Serialize(FILE *file);
    void Deserialize(FILE *file);

    ListNode *set_Rand_ptr();

    void add_to_head(string data_in);
    void print_List_data();

private:
    ListNode *head;
    ListNode *tail;
    int count;
};