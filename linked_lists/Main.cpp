#include "stdio.h"
#include "stdlib.h"
#include "DateTime.h"
#include "Objects.h"
#include "Headers.h"
#include "Structs.h"

#pragma warning(disable:4996)
#define OBJECTS_NUM 21

void PrintObjects(HeaderD* pStruct7);
int insertNewObject(HeaderD** pStruct7, char pNewID, int NewCode); 




int main(void) {

	HeaderD* pStruct = GetStruct7(7, OBJECTS_NUM);


	char test_id[] = "Bdfasdfas";

	HeaderD** ppStruct = &pStruct; 
	
	PrintObjects(pStruct); 
	// insertNewObject(ppStruct, test_id, 13);
	return 0;
}


 void PrintObjects(HeaderD* pStruct7) {
	Object7* t; 
	int i = 1;
	int identifier_counter = 0; 

	while (pStruct7) {
		t = (Object7*)pStruct7->pObject; 
		while (t) {
			printf("%d) %s %lu %02d %s %04d\n", i, t->pID, t->Code, t->pDate2->Day, t->pDate2->Month, t->pDate2->Year);
			t = t->pNext; 
			++i; 
		}

		pStruct7 = pStruct7->pNext;
		
	}
} 



int insertNewObject(HeaderD** pStruct7, char *pNewID, int NewCode) {


 if (!((*pNewID) >= 'A' && (*pNewID) <= 'Z')) {
	 return 0; 
 }

 
 HeaderD** first = (HeaderD**)(*pStruct7); 
 HeaderD** last = (HeaderD**)(*pStruct7); 
 Object7* temp; 

 first = (HeaderD**)(*pStruct7);
 last = (HeaderD**)(*pStruct7); 
 int nodes_found = 0; 


 while ((*pStruct7)) {
	 if ((*pStruct7)->cBegin != *pNewID) {
		 if ((*pStruct7)->cBegin > *pNewID) {
			 (*last) = (*pStruct7);
			 while (*first) {
				 if (((*last)->cBegin - (*first)->cBegin) == 1) {
					 break;
				 }
			 }
			 break;
		 }
		 (*pStruct7) = (*pStruct7)->pNext;
	 }

	 (*pStruct7) = (HeaderD*)malloc(sizeof(pStruct7));
	 (*pStruct7)->cBegin = *pNewID; 
	 (*pStruct7)->pNext = (HeaderD*)last;
	 (*pStruct7)->pPrior = (HeaderD*)first;
	 temp = (Object7*)(*pStruct7)->pObject;
	 temp->pID = pNewID;
	 temp->Code = NewCode; 

	

 }




 return 0; 
} 
