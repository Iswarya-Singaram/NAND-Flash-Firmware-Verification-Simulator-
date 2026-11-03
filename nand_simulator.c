
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BLOCKS 4
#define PAGES 4
#define PAGE_SIZE 16
#define BLOCK_SIZE (PAGES * PAGE_SIZE)

uint8_t nand[BLOCKS][PAGES][PAGE_SIZE];
int programmed[BLOCKS][PAGES] = {0};

// Block 1 is simulated as defective
int bad_blocks[BLOCKS] = {0, 1, 0, 0};

void initialize_memory()
{
    memset(nand, 0xFF, sizeof(nand));
    memset(programmed, 0, sizeof(programmed));
    printf("INIT PASS\n");
}

void write_page(int block, int page, char *data)
{
    if (block < 0 || block >= BLOCKS ||
        page < 0 || page >= PAGES)
    {
        printf("WRITE FAIL: Invalid address\n");
        return;
    }

    if (bad_blocks[block])
    {
        printf("WRITE FAIL: Bad block\n");
        return;
    }

    if (strlen(data) > PAGE_SIZE)
    {
        printf("WRITE FAIL: Data too large\n");
        return;
    }

    if (programmed[block][page])
    {
        printf("WRITE FAIL: Page already programmed\n");
        return;
    }

    for (int i = 0; i < strlen(data); i++)
    {
        nand[block][page][i] = (uint8_t)data[i];
    }

    programmed[block][page] = 1;
    printf("WRITE PASS\n");
}

void read_page(int block, int page)
{
    if (block < 0 || block >= BLOCKS ||
        page < 0 || page >= PAGES)
    {
        printf("READ FAIL: Invalid address\n");
        return;
    }

    if (bad_blocks[block])
    {
        printf("READ FAIL: Bad block\n");
        return;
    }

    printf("DATA ");

    for (int i = 0; i < PAGE_SIZE; i++)
        printf("%02X", nand[block][page][i]);

    printf("\n");
}

void erase_block(int block)
{
    if (block < 0 || block >= BLOCKS)
    {
        printf("ERASE FAIL: Invalid block\n");
        return;
    }

    if (bad_blocks[block])
    {
        printf("ERASE FAIL: Bad block\n");
        return;
    }

    for (int p = 0; p < PAGES; p++)
    {
        memset(nand[block][p], 0xFF, PAGE_SIZE);
        programmed[block][p] = 0;
    }

    printf("ERASE PASS\n");
}

void logical_to_physical(int address)
{
    if (address < 0 || address >= BLOCKS * BLOCK_SIZE)
    {
        printf("MAP FAIL: Invalid address\n");
        return;
    }

    int block = address / BLOCK_SIZE;
    int page = (address % BLOCK_SIZE) / PAGE_SIZE;
    int offset = address % PAGE_SIZE;

    printf("MAP PASS: Block=%d Page=%d Offset=%d\n",
           block, page, offset);
}

int main()
{
    char command[100];
    int block, page, address;
    char data[PAGE_SIZE + 1];

    initialize_memory();

    while (fgets(command, sizeof(command), stdin))
    {
        if (sscanf(command, "WRITE %d %d %16s",
                   &block, &page, data) == 3)
        {
            write_page(block, page, data);
        }
        else if (sscanf(command, "READ %d %d",
                        &block, &page) == 2)
        {
            read_page(block, page);
        }
        else if (sscanf(command, "ERASE %d", &block) == 1)
        {
            erase_block(block);
        }
        else if (sscanf(command, "MAP %d", &address) == 1)
        {
            logical_to_physical(address);
        }
        else if (strncmp(command, "EXIT", 4) == 0)
        {
            break;
        }
        else
        {
            printf("INVALID COMMAND\n");
        }
    }

    return 0;
}
