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
Object7* RemoveExistingObject(HeaderD** pStruct7, char* pExistingD); 



int main(void) {

	HeaderD* pStruct = GetStruct7(7, OBJECTS_NUM);



	HeaderD** ppStruct = &pStruct; 
	
	PrintObjects(pStruct); 
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
	

	 // sizeof(pNewID) < 3 ---- 3 because strings contain a "\0" operator
	 if (!("A" < pNewID && pNewID > "Z") || !NewCode || sizeof(pNewID) < 3) {
		 return 0; 
	 }
	

	 // Maybe need to redo this 
	 while ((*pStruct7)) {
		 if ((*pStruct7)->cBegin == *pNewID) {
			Object7* t_obj = (Object7*)(*pStruct7)->pObject;
			Object7* t_holder = (Object7*)malloc(sizeof(Object7)); 
			while (t_obj) {
				for (int i = 1; i < sizeof(pNewID) || i < sizeof(t_obj->pID); i++) {
					if (pNewID[i] < t_obj->pID[i]) {
						t_holder->pID = (char*)malloc(sizeof(pNewID)); 
						t_holder->Code = NewCode; 
						strcpy(t_holder->pID, pNewID); 
						GetDate2(time(&raw_time), t_holder->pDate2);
						t_holder->pNext = t_obj->pNext;
						t_obj->pNext = t_holder; 
						return 1; 
					}
				}
				t_obj = t_obj->pNext; 
			}
		}
		(*pStruct7) = (*pStruct7)->pNext; 
	 }
	/* 
		If the above condition fails, i.e. if no such header Node exists, allocate room 
		for another header Node, allocate room for another objct within that Node and feed them
		into the list
	*/

	 // We need two helper Nodes for this 

	 HeaderD** t_1; 
	 HeaderD** t_2; 
	 t_2 = pStruct7; 

	 while ((*pStruct7)) {
		 if ((*pStruct7)->cBegin == *pNewID) {
			 t_1 = pStruct7;
			 while (t_2) {
				 if ((*t_1)->cBegin - (*t_2)->cBegin == 1) {
					 pStruct7 = (HeaderD**)malloc(sizeof(pStruct7));
					 (*pStruct7)->cBegin = *pNewID; 
					 (*pStruct7)->pNext = (*t_1); 
					 (*pStruct7)->pPrior = (*t_2); 
					 (*t_1)->pNext = (*pStruct7); 

					 // Object
					 Object7* t_obj_1 = (Object7*)(*pStruct7)->pObject; 
					 t_obj_1->Code = NewCode; 
					 t_obj_1->pID = (char*)malloc(sizeof(pNewID)); 
					 strcpy(t_obj_1->pID, pNewID); 
					 t_obj_1->pDate2 = (Date2*)malloc(sizeof(Date2));
					 GetDate2(time(&raw_time), t_obj_1->pDate2); 
					 t_obj_1->pNext = NULL; 
					 (*pStruct7)->pObject = (Object7*)malloc(sizeof(t_obj_1)); 
					 memcpy((*pStruct7)->pObject, t_obj_1, sizeof(t_obj_1)); 
					 free(t_obj_1->pID); 
					 free(t_obj_1->pDate2); 
					 return 1; 
				 }

				 (*t_2) = (*t_2)->pNext; 
			 }
		 }
		 (*pStruct7) = (*pStruct7)->pNext; 
	 }



	 return 0; 
 }

 Object7* RemoveExistingObject(HeaderD** pStruct7, char* pExistingD) {
	 uint32_t node_count, object_count; 
	 Object7* helper_obj;
	 HeaderD** helper_struct; 
	 object_count = 0, node_count = 0; 
	 
	 
	 while ((*pStruct7)) {
		 
		 helper_struct = pStruct7; 
		 helper_obj = (Object7*)(*pStruct7)->pObject; 
		 while (helper_obj) {
			 if (helper_obj->pID = pExistingD) {
				 if (node_count == 0) {
					 (*pStruct7) = (*pStruct7)->pNext; 
					 return (Object7*)(*helper_struct)->pObject; 
				 }
				 (*pStruct7) = (*pStruct7)->pNext; 
				 return helper_obj; 
			 }
			 helper_obj = helper_obj->pNext; 
		 }


		 (*pStruct7) = (*pStruct7)->pNext; 
		 ++node_count; 
	 }

	 return 0; 
 }

 
