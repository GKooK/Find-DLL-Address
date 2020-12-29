// find_mem_addr_dynamic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "intrin.h"
#include <stdio.h>
#include "wchar.h"

unsigned int Get_PEB(void);
int _tmain(int argc, _TCHAR* argv[])
{
	//a difference way to access to peb's address
	/* 
	unsigned __int32 fsReg = 0;
	fsReg = __readfsdword(0x30);
	printf("%I32x\n", fsReg);
	printf("%08x\n", fsReg);
	*/
	
	int *pebaddr = (int*)Get_PEB();//imagebase addr = (peb+0x08)
	//LDR = (peb+0x0c) = imaagebase addr + 0x04
	int ldrvalue = *(pebaddr+0x03);
	char *p = (char *)ldrvalue;
	int *ldr = (int*)ldrvalue;
	int firstinmemoryordermodulelistvlaue = *(ldr+0x5)-8;
	int *firstinmemoryordermodulelist = (int*)firstinmemoryordermodulelistvlaue;
	int firstinmemoryordermodulelist_basedllName = *(ldr+0x5)-8+44+4;
	int *firstinmemoryordermodulelist_basedllName_p = (int*)firstinmemoryordermodulelist_basedllName;
	int name_addr = *(firstinmemoryordermodulelist_basedllName_p);
	char*p2 = (char*)name_addr;
	wprintf(L"The file name is : %s\n", p2);
	char temp_sample[256] = {0,};
	int index_sample = 0;
	wchar_t kernel_string[] = L"kernel32.dll";
	if(wcscmp((wchar_t*)p2, kernel_string)==0){
		printf("success\n");
		goto find;
	}
	
	int next_link;
	int *next_link_addr;
	next_link = firstinmemoryordermodulelistvlaue+8;
	next_link_addr = (int*)next_link;

	//printf("next %08x %08x (-8 : %08x)\n", next_link_addr, *(next_link_addr), *(next_link_addr)-8);
	char *cmdp = "";
	int temp_basedllName = *(next_link_addr)-0x8;
	int *temp_basedllName_p = (int*)(temp_basedllName);
	int a_sulma = *(temp_basedllName_p+0xc);
	int *dllbase = (int*)(temp_basedllName_p+0x6);
	char a_sumla_c = a_sulma;
	int count_of_loop = 1;
	printf("-----------------%d-----------------\n", count_of_loop);
	wprintf(L"The next(%d) DLL is : %s\nDll Address is 0x%08x\n", count_of_loop, a_sulma, *(dllbase));
	next_link = temp_basedllName+8;
	next_link_addr = (int*)next_link;
	count_of_loop++;

	while(1){
		printf("-----------------%d-----------------\n", count_of_loop);
		int temp_basedllName_1 = *(next_link_addr)-8;
		int *temp_basedllName_p_1 = (int*)(temp_basedllName_1);
		int a_sulma_1 = *(temp_basedllName_p_1+0xc);
		int *dllbase_1 = (int*)(temp_basedllName_p_1+0x6);
		char a_sumla_c_1 = a_sulma_1;
		if(a_sulma_1 == 0){
			goto find;
		}
		wprintf(L"The next(%d) DLL is : %s\nDll Address is 0x%08x\n", count_of_loop, a_sulma_1, *(dllbase_1));
		next_link = *(next_link_addr+0x2)-8;
		next_link_addr = (int*)next_link;
		count_of_loop++;
	}
find:
	printf("----------------END----------------\n");

	return 0;
}

//https://en.wikipedia.org/wiki/Win32_Thread_Information_Block, Accessing the TIB
//change mov EAX, FS:[18h] ->fs[0x30] because 0x30 offset is peb's address
unsigned int Get_PEB(void){
	__asm{
		mov eax, fs:[0x30]
	};
}

