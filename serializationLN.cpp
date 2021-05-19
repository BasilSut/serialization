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
                {}

List::~List(){
    while(head){
        tail = head->next; 
        delete head;       
        head = tail;
    }
}
ListNode* List::set_Rand_ptr(){
    srand(std::time(nullptr));
    int rand_num = rand()%count;
    int tmp_count=0;
    //cout << rand_num << endl;
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
    file_ptr = fopen("binary_list.txt", "wb");
    if (fopen == NULL){
        printf("Write error\n");
    }
    else{
        char new_line = '\n';
        unordered_map<ListNode *, int> ptrToId;
        int idCount = 0;
        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next)
            ptrToId.insert({tmp_node, idCount++});

        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next)
        {
            fwrite(&ptrToId[tmp_node->rand], sizeof(ptrToId[tmp_node->rand]), 1, file_ptr);
            fwrite(&new_line, sizeof(new_line), 1, file_ptr);
            fwrite(&tmp_node->data, sizeof(tmp_node->data), 1, file_ptr);
            fwrite(&new_line, sizeof(new_line), 1, file_ptr);
        }
    }
}


void List::Deserialize(FILE *file_ptr){
    
    file_ptr = fopen("binary_list.txt", "rb");
    if (fopen == NULL){
        
        printf("Write error\n");
    }
    else
    {

        unordered_map<int, ListNode *> IdToPtr;

        int idCount = 0;
        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next)//мне нужно создать count количество пустых(data=0) узлов, чтобы узнать адрес head узла?
            IdToPtr.insert({ idCount++, tmp_node});
      
        int id;
        for (ListNode *tmp_node = head; tmp_node; tmp_node = tmp_node->next){
            fread(&id, sizeof(id), 1, file_ptr);
            cout<<id<<" " <<endl;
            fread(&tmp_node->data, sizeof(tmp_node->data), 1, file_ptr);
            tmp_node->rand = IdToPtr[id];
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
    test.Serialize(ptr_f);
    test2.Deserialize(ptr_f2);
    //test.print_List_data();

    test2.print_List_data();

    return 0;
}