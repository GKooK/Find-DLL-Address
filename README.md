# Find-DLL-Address

> There is many way to Find **KERNEL32.DLL**'s location like using Debugger. I used Process Environment Block to access to loaded dll

## Process Environment Block

> Process Environment Block, PEB's location is at +0x30 offset from Thread Information Block's  start address.

At PEB's structure, There is **Ininitialization order module list**'s address at +0x0c location, and at LDR's +0x14 address, there is LIST_ENTRY(LDR_DATA_TABLE_ENTRY) structure named InMemoryOrderLinks. and InMemoryOrderLinks includes **DllBase, DllName(full, base)** at +0x08 offset, +0x24, +0x28 offset.  Also There's the next LDR_DATA_TABLE_ENTRY's address stored at +0x08 offset.

But when you are trying to calculate the address of LDR_DATA_TABLE_ENTRY with the value stored in InMemoryOrderLinks, You need to access the memory address that is -0x08's location. 

For example

> Assume InMemoryOrderLinks's start value is 0x77777778, you need to access at 0x77777770 to find the next LDR_DATA_TABLE_ENTRY

