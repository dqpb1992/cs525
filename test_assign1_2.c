#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

// test name
char *newTest;

/* This is the testing file */
#define TESTPF "test_pagefile.bin"

/* prototypes for test functions */
static void testCreateOpenCloseDestroy(void);
static void testMultiplePageContents(void);

/* main function running all tests */
int
main (void)
{
  newTest = "";
  
  initStorageManager();

  testCreateOpenCloseDestroy();
  testMultiplePageContents();

  return 0;
}


/* check a return code. If it is not RC_OK then output a message, error description, and exit */
/* Try to create, open, and close a page file */
void
testCreateOpenCloseDestroy(void)
{
  SM_FileHandle fh;

  newTest = "This is a test for creating, openning, closing and destroying";

  TEST_CHECK(createPageFile (TESTPF));
  printf("Testing open file function\n");
  TEST_CHECK(openPageFile (TESTPF, &fh));
  ASSERT_TRUE(strcmp(fh.fileName, TESTPF) == 0, "filename passed");
  ASSERT_TRUE((fh.totalNumPages == 1), "page number passed");
  ASSERT_TRUE((fh.curPagePos == 0), "current page number passed");

  printf("Testing close and destroy file function\n");
  TEST_CHECK(closePageFile (&fh));
  TEST_CHECK(destroyPageFile (TESTPF));

  // after destruction trying to open the file should cause an error
  ASSERT_TRUE((openPageFile(TESTPF, &fh) != RC_OK), "closePageFile and destroyPageFile passed!");

  TEST_DONE();
}

/* Try to create, open, and close a page file */
void
testMultiplePageContents(void)
{
  SM_FileHandle fh;
  SM_PageHandle ph;
  int i;

  newTest = "test multiple page contents";

  ph = (SM_PageHandle) malloc(PAGE_SIZE);

  // create a new page file
  TEST_CHECK(createPageFile (TESTPF));
  TEST_CHECK(openPageFile (TESTPF, &fh));
  printf("created and opened file\n");
  
  // read first page into handle
  TEST_CHECK(readFirstBlock (&fh, ph));
  // the page should be empty (zero bytes)

  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in first page of freshly initialized page");
  printf("ReadFirstBlock and ReadBlock Passed!");

  ASSERT_TRUE((getBlockPos(fh) == 0), "getBlockPos function passed");

  TEST_CHECK(createPageFile (TESTPF));
  TEST_CHECK(openPageFile (TESTPF, &fh));

  // change ph to be a string and write that one to disk
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 100) + '0';

  TEST_CHECK(writeBlock (0, &fh, ph));
  printf("Write Block passed!\n");

  appendEmptyBlock(fh);
  TEST_CHECK(writeCurrentBlock (&fh, ph));
  printf("Write Current Block passed!\n");
  ASSERT_TRUE((readPreviousBlock(fh, ph) == RC_OK), "ReadPreviousBlock function passed");
  ASSERT_TRUE((readCurrentBlock(fh, ph) == RC_OK), "ReadCurrentBlock function passed");

  // read back the page containing the string and check that it is correct
  TEST_CHECK(readFirstBlock (&fh, ph));

  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("ReadFirstBlock Passed\n");

  ASSERT_TRUE((readNextBlock(fh, ph) == RC_OK), "ReadNextBlock function passed");
  ASSERT_TRUE((readLastBlock(fh, ph) == RC_OK), "ReadLastBlock function passed");

  appendEmptyBlock(fh);
  ASSERT_TRUE((fh.totalNumPages == 3), "AppendEmptyBlock function passed");
  ensureCapacity(10, fh);
  ASSERT_TRUE((fh.totalNumPages == 10), "EnsureCapacity function passed");

//   printf("test point line 115\n");
  // destroy new page file
  TEST_CHECK(destroyPageFile(TESTPF));  

  TEST_DONE();

  // find the test end of testcase
  printf("test out\n");

}
