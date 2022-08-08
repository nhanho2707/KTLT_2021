#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <iostream>
using namespace std;
struct SOPHUC {
	float thuc;
	float ao;
};
int headSize = sizeof(int) + sizeof(int);
int memSize(int nItem, int sizeItem) {
	if (sizeItem < 0) sizeItem = -sizeItem;
	if (sizeItem == 0) sizeItem = 1;
	if (nItem < 0) nItem = -nItem;
	return headSize + sizeItem * nItem;
}

ostream& operator<< (ostream& outDev, const SOPHUC& sp)
{
	outDev << sp.thuc << " + " << sp.ao << "i";
	return outDev;
}
istream& operator>>(istream& inDev, SOPHUC& sp) {
	inDev >> sp.thuc >> sp.ao;
	return inDev;
}

void* origin_addr(void* aData) {
	if (aData != NULL) {
		return (char*)aData - headSize;
	}
	return NULL;
}

void* data_addr(void* aOrigin) {
	if (aOrigin != NULL) {
		return (char*)aOrigin + headSize;
	}
	return NULL;
}
void* nItem_addr(void* aData) {
	return origin_addr(aData);
}
void* sizeItem_addr(void* aData) {
	if (aData != NULL) {
		return (char*)aData - sizeof(int);
	}
	return NULL;
}

void* arrInit(int nItem, int sizeItem) {
	int sz = memSize(nItem, sizeItem);
	void* aOrigin = malloc(sz);
	if (aOrigin != NULL) {
		memset(aOrigin, 0, sz);
		void* aData = data_addr(aOrigin);
		*(int*)origin_addr(aData) = nItem;
		*(int*)sizeItem_addr(aData) = sizeItem;
		return aData;;
	}
	return NULL;
	}

int arrSize(void* aData) {
	if (aData != NULL) {
		return *(int*)nItem_addr(aData);
	}
	return 0;
}

int arrItemSize(void* aData) {
	if (aData != NULL) {
		return *(int*)sizeItem_addr(aData);
	}
	return 0;
}
void* arrResize(void* aData, int nItem) {
	int sz, sizeItem;
	void* aNew = NULL, * aOrigin = NULL;
	if (aData == NULL || nItem < 0) {
		return NULL;
	}
	aOrigin = origin_addr(aData);
	sizeItem = *(int*)sizeItem_addr(aData);
	sz = memSize(nItem, sizeItem);
	aNew = realloc(aOrigin, sz);
	if (aNew != NULL) {
		aData = data_addr(aNew);
		*(int*)nItem_addr(aData) = nItem;
		return aData;
	}
	return NULL;
}

int arrPushBack(void** aData, void* x) {
	int nItem = arrSize(*aData), szItem = arrItemSize(*aData);
	void* anew = arrResize(*aData, 1 + nItem);
	if (anew != NULL) {
		memmove((char*)anew + nItem * szItem, x, szItem);
		*aData = anew;
		return 1;
	}
	return 0;
 }

void* arrPopBack(void** aData) {
	int nItem = arrSize(*aData), szItem = arrItemSize(*aData);
	void* x = malloc(szItem);
	if (*aData != NULL && nItem > 0) {
		nItem--;
		memmove(x, (char*)(*aData) + nItem * szItem, szItem);
		void* aNew = arrResize(*aData, nItem);
		if (aNew != NULL)*aData = aNew;
	}
	return x;
}
void arrFree(void* aData) {
	if (aData != NULL) {
		free(origin_addr(aData));
	}
}
void soPhucPrint(SOPHUC p)
{
	cout << p.thuc << " + " << p.ao << "i";
}
void soPhucArrOut(SOPHUC* a)
{
	int n = arrSize(a);
	for (int i = 0; i < n; i++)
	{
		soPhucPrint(a[i]);
	}
}
SOPHUC* soPhucArrIn()
{
	SOPHUC* a = (SOPHUC*)arrInit(0, sizeof(SOPHUC));
	SOPHUC p ;
	while (cin>>p)
	{
		arrPushBack((void**)&a, &p);
	}
	cin.clear();
	return a;
}

void main()
{
	cout << "Nhap cac phan tu so phuc n:\n";
	SOPHUC* B = soPhucArrIn();
	SOPHUC* x = (SOPHUC*)arrPopBack((void**)&B);
	cout << "After pop:\n";
	soPhucArrOut(B);
	cout << "\nPopped element:" << *x << endl;
	free(x);
	arrFree((SOPHUC*)B);
}
