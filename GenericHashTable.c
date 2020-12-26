#include "GenericHashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*yisrael bar 23-30/10/20*/


/**
* The function gets the original size and the type of the data in the table elements.
* it initializes the Table struct members. 
* On success, the function returns a pointer to the new created Table, otherwise, it return NULL.
*/
Table* createTable(int size, int dType, int listLength){
    //check value's
    if (size <= 0 || listLength <= 0  || (dType != 0 && dType != 1 ))
    {
        printf("worng values entered to create table\n");
        return NULL;
    }
    //create a new table
    Table *t1 = (Table*)malloc(1*sizeof(Table));
    if (t1 == NULL)
    {
        perror("error in malloc table - in create table\n");
        return NULL;
    }
    //create a new array
    Object **arr = (Object**)malloc(size*sizeof(Object*));
    if (arr == NULL)
    {
        perror("error in malloc array - in create table\n");
        return NULL;    
    }
    //update all the table field's
    t1->arr = arr;
    t1->M = size;
    t1->N = size;
    t1->D = 1;
    t1->T = listLength;
    t1->dType = dType;
    //init all the array with linklist with null's
    for (int i = 0; i < size; i++)
    {
        Object *obj1 = (Object*)malloc(1*sizeof(Object));
        if (obj1 == NULL)
        {
            perror("error in malloc object - in create table\n");
            return NULL;
        }
        obj1->data = NULL; 
        obj1->next = NULL; 
        t1->arr[i] = obj1;
    }
    return t1;
}
/**
* The function release all the allocated members of struct Table.
*/
void freeTable(Table* table){
    //check value of table
    if (table ==  NULL )
    {
       printf("the table that was sent is null - in free table\n");
       return;
    }
    int dType = table->dType;
    int size = table->N;
    //clean each cell link_list - one object at time
    for (int i = 0; i < size; i++)
    {   
        Object *obj1 = table->arr[i];
        Object *p = NULL;
        //go throw the link list
        while (obj1 != NULL)  
        {  
            p = obj1->next;  
            freeObject(obj1, dType);
            obj1 = p;  
        }  
    }
    //clean the array
    free(table->arr);
    table->arr = NULL;
    //clean the table
    free(table);
    table = NULL;
}

/**
* The function adds data to the hashtable (as described in the exe definition)
* On success, the function returns the array index of the added data, otherwise, it return -1.
*/
int add(Table* table, void* data){
    // printf("i'm in add the *(int*)data is: %s\n",(char*)data);    
    //check value's
    if (table == NULL || data == NULL)
    {
        printf("null value's entered to add\n");
        return -1;
    }
    //take from the table all the value's
    int D = table->D;
    int dType = table->dType;
    int k = 0;
    int M = table->M;
    int T = table->T;
    int *intData; 
    char *charData;
    //calculate the k(key) and copy the data
    if (dType == INT_TYPE)
    {
        k = intHashFun(data, M);
        //----------------copy the int by value to a pointer
        intData = (int*)malloc(sizeof(int));
        if (intData ==  NULL)
        {
            printf("error in malloc int-data in- add\n");
            return -1;
        }
        
        *intData = *(int*)data;
    }
    else if (dType == STR_TYPE){
        k = strHashFun(data, M);
        //-----------------copy the string by value to a pointer
        charData = (char*)malloc(sizeof(char)*strlen((char*)data)+1);
         if (charData ==  NULL)
        {
            printf("error in malloc char-data in- add\n");
            return -1;
        }
        strcpy(charData, data);
    }
    //update the index
    int index = D*k;
    
    //add--------------------------------------- here we update the table with new value if it possiable    
    for (int i = index; i < index+D ; i++)
    {
        Object * obj2 = table->arr[i];
        for (int count = 1; count < T; count++)
        {
            //put the pointer with the copy data in object
            if (obj2->data == NULL)
            {
                if(dType == INT_TYPE)
                    obj2->data = intData;           
                if(dType == STR_TYPE)
                    obj2->data = charData;
                return i;
            }
            //put a new object in next
            else if (obj2->next == NULL)
            {
                if(dType == INT_TYPE)
                    obj2->next = createObject(intData);        
                if(dType == STR_TYPE)
                    obj2->next = createObject(charData);
                return i;
            }
            //go to the next in the link list
            else
            {
                obj2 = obj2->next;
            }   
        }
    }
        
    //--------------duplicate Table in case the link list in index was to long(T size)
    duplicateTable(table);
     
    //----------------------------------------------------------
    // atempet to add after the duplicate
    D = table->D;
    for (int i = index; i < index+D ; i++)
    {
        Object * obj2 = table->arr[i];
        for (int count  = 1; count < T; count++)
        {
            //put the pointer with the copy data in object
            if (obj2->data == NULL)
            {
                if(dType == INT_TYPE)
                    obj2->data = intData;           
                if(dType == STR_TYPE)
                    obj2->data = charData;                
                return i;
            }
            //put a new object in next
            else if (obj2->next == NULL)
            {
                if(dType == INT_TYPE)
                    obj2->next = createObject(intData);        
                if(dType == STR_TYPE)
                    obj2->next = createObject(charData);
                return i;
            }
            //go to the next in the link list
            else
            {
                obj2 =  obj2->next;
            }   
        }
    }
    // -------------------------------------------------------------------------------
    
    //failed
    return -1;
}

/**
* The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
* On success, the function returns the array index of the removed data, otherwise, it returns -1.
* -1 is also returned in the case where there is no such object.
*/
int removeObj(Table* table, void* data){
    //check value's
    if (table == NULL || data == NULL)
    {
        printf("null value's entered to removeObj\n");
        return -1;
    }
    //take value's from table
    int D = table->D;
    int dType = table->dType;
    int k = 0;
    int M = table->M;
    //get key
    if (dType == INT_TYPE)
    {
        k = intHashFun(data, M);
    }
    else if (dType == STR_TYPE){
        k = strHashFun(data, M);
    }
    
    int index = D*k;
    //go throw the range of the possible location of data
    for (int i = index; i < index+D; i++)
    {   
        Object * obj1 = table->arr[i];
        Object * prev = NULL;
        //check if it's the first object(head of the list) that need to be removed 
        if (obj1 !=NULL && isEqual(dType, obj1->data, data) == 0)
        {
            table->arr[i] = obj1->next;
            freeObject(obj1, dType);
            return i;
        }
        do
        {
            //check if need to remove the object and update the pointers
            if (obj1 !=NULL && isEqual(dType, obj1->data, data) == 0)
            {
                prev->next = obj1->next;
                freeObject(obj1,dType);
                return i;
            }else
            {
                prev = obj1;
                if(obj1 == NULL)
                    continue;
                obj1 = obj1->next;
            }
        } while (obj1 != NULL);
        
    }
    //field / didn't found the object
    return -1;
}

/**
* The function search for an object that its data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
*/
Object* search(Table* table, void* data){
    //check value's
    if (table == NULL || data == NULL)
    {
        printf("null value's entered to search\n");
        return NULL;
    }
    //take value's from table
    int D = table->D;
    int dType = table->dType;
    int k = 0;
    int M = table->M;
    //get key
    if (dType == INT_TYPE)
    {
        k = intHashFun(data, M);
    }
    else if (dType == STR_TYPE){
        k = strHashFun(data, M);
    }
    int index = D*k;
    //go throw the range of the possible location of data
    for (int i = index; i < index+D; i++)
    {   
        Object * obj1 = table->arr[index];
        if (obj1 == NULL)
        {
            continue;
        }
        do
        {
            //go throw all the link list in atempet to find the object
            if (isEqual(dType, obj1->data, data) == 0)
            {
                return obj1;
            }else
            {
                if(obj1->next == NULL)
                    break;
                obj1 = obj1->next;
            }

        } while (obj1->data != NULL);
        
    }
    //if it didn't found the object we return null
    return NULL;
}

/**
* The function print the table (the format is in the exe definition)
*/
void printTable(Table* table){
    //check that there is no null value's
    if(table == NULL || table->arr == NULL || table->arr[0] ==  NULL){
        printf("null value entered to printTable \n");
        return;
    }
    int size = table->N;
    //go throw all the array and print each index with his linkk list
    for (int i = 0; i < size; i++)
    {
        printf("[%d]\t", i);
        Object *obj1 = table->arr[i];
        do
        {   if (obj1 != NULL && obj1->data != NULL)
            {
                if (table->dType ==  INT_TYPE)
                {
                    printf("%d\t-->\t", *(int*)obj1->data);
                }else if (table->dType ==  STR_TYPE)
                {
                    printf("%s\t-->\t", (char*)obj1->data);
                }
                if(obj1->next == NULL)
                    break;
                obj1 = obj1->next; 
            }
        } while (obj1 != NULL && obj1->data != NULL) ;
       
        
        printf("\n");
    }
    
}

/**
* This function creates an object and return the pointer to it or NULL if there is some error.
*/
Object* createObject(void* data){
    //check value
    if(data == NULL){
        printf("null value's entered to createObject\n");
        return NULL;
    }
    //create a new object
    Object *obj1 = (Object*)malloc(1*sizeof(Object));
    if (obj1 ==  NULL)
    {
        perror("error in malloc - in create object\n");
        return NULL;
    }
    //update new object with data
    obj1->data = data;
    obj1->next = NULL;
    return obj1;
}

/**
* This function frees an object, the data of the object should also be freed.
*/
void freeObject(Object* obj, int type){
    //check value's
    if (obj == NULL || (type != 0 && type != 1))
    {
        printf("object is null - in free object\n");
        return;
    }
    free(obj->data);   
    // obj->next = NULL;
    // obj->data = NULL;
    free(obj);
}

/**
* check the equality of the data of two objects. The implementation is different depending the type of the data.
* the function returns 0 if they are equal or some other value if they are not equal.
*/

int isEqual(int type, void* data1, void* data2){
    //check value's
    if (data1 == NULL || data2 ==NULL )
    {
        if (data1 == NULL && data2 ==NULL)
        {
            return 0;
        }else return -1;  
    }
    if (type != 0 && type != 1)
    {
        return -1;
    }
    
    //in case it's int
    if (type ==  INT_TYPE)
    {
         if (*(int*)data1 == *(int*)data2)
         {
             return 0;
         }else return -1;
    }
    if (type == STR_TYPE)
    {
        int len1 = strlen(data1);
        int len2 = strlen(data2);
        if(len1 != len2)
            return -1;
        return strcmp(data1,data2);
        //in case str compare isn't allowd
        // for (int i = 0; i < len1; i++)
        // {
        //     printf("the char is: %c \n", (char)data1+i);
        //     if (data1+i != data2+i)
        //     {
        //         return -1;
        //     }
        // }
        // return 0;
    }
 
    return -1;
}

/**
* returns the hash value of an integer, which is key mod origSize 
*/
int intHashFun(int* key, int origSize){
    return (*key % origSize);
}

/**
* returns the hash value of an string, which is m mod origSize, where m is the sum of the ascii value of all the 
* character in key. 
*/
int strHashFun(char* key, int origSize){
    int m = 0;
    int length = strlen(key);
    for (int i = 0; i < length; i++)
    {
        // printf("asciii : %s\n", (int)key[i]);
        m += (int)key[i];
    }
    return (m % origSize);
}

/* take the table->arr and make the size double, and update all the pointers from i to i*2*/
void duplicateTable (Table* table){
    if (table == NULL)
    {
        printf("null value's entered to duplicateTable\n");
        return;
    }
    
    int size = table->N;
    Object ** arr = table->arr;
    int len = 2*size*sizeof(Object*);
    //make the array double size
    arr = (Object**)realloc(arr, len);
    if (arr ==  NULL)
    {
        perror("error in realloc - in duplicateTable\n");
        return ;
    }
    table->arr = arr;
    //initlize all the other cell's with an object with null values
    for (int i = size; i < size*2; i++)
    {
        Object *obj1 = (Object*)malloc(1*sizeof(Object));
        if (obj1 == NULL)
        {
            perror("error in malloc object - in duplicateTable\n");
            return;
        }
        obj1->data = NULL; 
        obj1->next = NULL; 
        table->arr[i] = obj1;
    }
    //copy all the pointers to the right place
    for (int i = size-1; 0 < i; i--)
    {

        table->arr[i*2]->data = table->arr[i]->data;
        table->arr[i*2]->next = table->arr[i]->next;
        table->arr[i]->data = NULL;
        table->arr[i]->next = NULL;
    }
    //update the table
    table->N = size * 2;
    table->D = (size*2) / table->M;
    
}