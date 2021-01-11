// find_mem_addr_dynamic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "intrin.h"
#include <stdio.h>
#include "wchar.h"
#include "string.h"

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
	wchar_t kernel_string[] = L"KERNEL32.DLL";
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

	//execute function by function address.
	int *kernel32_dllbase;// = (int*)(temp_basedllName_p+0x6);
	int *kernel32_dllbase_data_dir;
	int *kernel32_func_addr;
	int *kernel32_func_name;
	int *kernel32_func_count;
	
	typedef int func1(int);//ExitProcess
	typedef int __stdcall func2(char*, int);//WinExec
	func1* f1;// = (func*)0xdeadbeef;
	func2* f2;// = (func*)0xdeadbeef;
	//int i = f();
	//WinExec("something", 0)
	//ExitProcess(0)

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
		if(wcscmp((wchar_t*)a_sulma_1, kernel_string)==0){
			//printf("kernel32.dlladfaisdfhlashdfuashdfllasudhfl\n");
			kernel32_dllbase = (int*)*(dllbase_1)+0x5a;
			wprintf(L"I Want kernel32's data Directory : 0x%08x, *(0x%08x)\n", kernel32_dllbase, *(kernel32_dllbase));
			kernel32_dllbase_data_dir = (int*)(*(dllbase_1)) + *(kernel32_dllbase)/4;
			wprintf(L"I Want dllbase_data_dir : 0x%08x, *(0x%08x)\n", kernel32_dllbase_data_dir, *(kernel32_dllbase_data_dir));
			kernel32_func_addr = (int*)(kernel32_dllbase_data_dir) + 0x7;
			kernel32_func_name = (int*)(kernel32_dllbase_data_dir) + 0x8;
			kernel32_func_count = (int*)(kernel32_dllbase_data_dir) + 0x9;
			wprintf(L"Find Trio addr 0x%08x, 0x%08x, 0x%08x\n", kernel32_func_addr, kernel32_func_name, kernel32_func_count);
			wprintf(L"Find Trio 0x%08x, 0x%08x, 0x%08x\n", *(kernel32_func_addr), *(kernel32_func_name), *(kernel32_func_count));
			int *space = (int*)(*(dllbase_1)) + *(kernel32_func_name)/4;
			int *space2 = (int*)(*(dllbase_1)) + *(kernel32_func_count)/4;
			int *space3 = (int*)(*(dllbase_1)) + *(kernel32_func_addr)/4;
			int name_counter = 0x00;
			wprintf(L"Find name start addr 0x%08x, (0x%08x)\n", space, *(space));
			getchar();

			for(int inde=0;inde<1603;inde++){//while(1){	//I don't Know why there is 1603 loop but if it gets error if it's over
				int *print_name_for_temp = (int*)(*(dllbase_1) + *(space+name_counter));
				if(*(print_name_for_temp) == 0){
					break;
				}
				if(strcmp((char*)print_name_for_temp, "ExitProcess") == 0){
					int temp = (short)*((short*)space2+name_counter);
					f1 = (func1*) (*(dllbase_1) + *(space3+temp));
					//printf("%d@@@@0x%08x, 0x%08x@@@@ realaddr : 0x%08x\n", temp, space3+temp, *(space3+temp), f1);
					//goto find;
				}
				if(strcmp((char*)print_name_for_temp, "WinExec") == 0){
					int temp = (short)*((short*)space2+name_counter);
					f2 = (func2*) (*(dllbase_1) + *(space3+temp));
					//printf("%d@@@@0x%08x, 0x%08x@@@@ realaddr : 0x%08x\n", temp, space3+temp, *(space3+temp), f2);
					goto find;
				}
				wprintf(L"In loop addr 0x%08x, (0x%08x) start name at : [0x%08x]\n", space+name_counter, *(space+name_counter), print_name_for_temp);
				printf("name Goccha : %s\n", print_name_for_temp);
				printf("number Goccha : %d\n", (short)*((short*)space2+name_counter));
				name_counter = name_counter + 0x01;
			}

		}
		wprintf(L"The next(%d) DLL is : %s\nDll Address is 0x%08x\n", count_of_loop, a_sulma_1, *(dllbase_1));
		next_link = *(next_link_addr+0x2)-8;
		next_link_addr = (int*)next_link;
		count_of_loop++;
	}
find:
	printf("----------------END----------------\n");
	getchar();
	f2("cmd", 5);// SW_SHOW == (int)5
	f1(0);

	return 0;
}

//https://en.wikipedia.org/wiki/Win32_Thread_Information_Block, Accessing the TIB
//change mov EAX, FS:[18h] ->fs[0x30] because 0x30 offset is peb's address
unsigned int Get_PEB(void){
	__asm{
		mov eax, fs:[0x30]
	};
}

