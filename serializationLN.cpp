#include "serializationLN.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_map>

using std::cout;
using std::endl;
using std::srand;
using std::unordered_map;

List::List() :  head(NULL), 
                tail(NULL),
                count(0)
                {
                    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                    generator = std::default_random_engine(seed);
                }

List::~List(){
    while(head){
        tail = head->next; 
        delete head;       
        head = tail;
    }
}
ListNode* List::set_Rand_ptr(){
    srand(std::time(nullptr));
    std::uniform_int_distribution<int> distribution(0, count);

    int rand_num = distribution(generator);
    int tmp_count=1;
    if (rand_num == 0){
        return NULL;
    }else{
        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next, tmp_count++)
        {
            if (tmp_count == rand_num){
                return tmp_node;
            }
        }
    }
}

void List::add_to_head(string data_in){
    ListNode *temp = new ListNode; 
    temp->next = NULL;    
    temp->data = data_in;   

    if (head != NULL){
        temp->prev = tail; 
        tail->next = temp; 
        tail = temp;
        temp->rand = set_Rand_ptr();

                count++;
    }
    else {
        temp->prev = NULL;  
        head = tail = temp;
        count++;
        temp->rand = NULL;
    }
}

void List::print_List_data(){
    cout<<"kol-vo nodes: "<<count<<endl;
    for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next){
        cout<<tmp_node->data<<" ";
        //cout << tmp_node->rand << endl;
    }
    cout<<endl;
}




void List::Serialize(FILE *file_ptr){
    file_ptr = fopen("binary_list.dat", "wb");
    if (fopen == NULL){
        printf("Write error\n");
    }
    else{
        fwrite(&count, sizeof(count), 1, file_ptr);
        unordered_map<ListNode *, int> ptrToId;
        ptrToId[nullptr] = 0;
        int idCount = 1;
        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next)
            ptrToId[tmp_node] = idCount++;

        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next)
        {
            size_t len = tmp_node->data.length();
            fwrite(&len, sizeof(tmp_node->data.length()), 1, file_ptr);
            fwrite(tmp_node->data.c_str(), sizeof(tmp_node->data.length()), 1, file_ptr);
            fwrite(&ptrToId[tmp_node->rand], sizeof(ptrToId[tmp_node->rand]), 1, file_ptr);
        }
    }
}


void List::Deserialize(FILE *file_ptr){
    
    file_ptr = fopen("binary_list.dat", "rb");
    if (fopen == NULL){
        
        printf("Write error\n");
    }
    else{
        fread(&count, sizeof(count), 1, file_ptr);
        
        vector<ListNode *> IdToPtr(count+1);
        IdToPtr[0] = nullptr;

        vector<int> randIds(count);
        
        for (size_t i = 0; i < count; i++)
        {
            size_t len;
            fread(&len, sizeof(len), 1, file_ptr);
            char * tmp_str = new char[len];
            fread(tmp_str,sizeof(char)*len, 1, file_ptr);
            add_to_head(string(tmp_str, len));
            delete[] tmp_str;
            fread(&randIds[i], sizeof(int), 1, file_ptr);
            IdToPtr[i] = head;
        }
        size_t i = 0;
        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next)
        {
            tmp_node->rand = IdToPtr[randIds[i++]];
        }
    }
}

int main(int argc, const char **argv){
    FILE *ptr_f;
    FILE *ptr_f2; 
    List test,test2;
    //test2.add_to_head("d_");
    test.add_to_head("When");
    test.add_to_head("I");
    test.add_to_head("find");
    test.add_to_head("myself");
    test.add_to_head("in times");
    test.add_to_head("of");
    test.add_to_head("trouble");    
    test.print_List_data();
    
    test.Serialize(ptr_f);
    test2.Deserialize(ptr_f2);
    

    test2.print_List_data();
    
    return 0;
}