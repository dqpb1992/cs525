//
//  storage_mgr.c
//  testcode_2_0920
//
//  Created by Pingyu Xue on 9/21/16.
//  Copyright © 2016 Pingyu Xue. All rights reserved.
//

#include "storage_mgr.h"
#include "stdio.h"

#include "string.h"
#include "stdlib.h"


/**************************************************************************************
 *Function Name: initStorageManager
 *
 *Description: Initialization of Storage Manager
 *
 *Parameters:
 *            Void
 *Return:
 *            Void
 *Author:
 *            Uday Tak <utak@hawk.iit.edu>
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/14/2016             Uday Tak <utak@hawk.iit.edu>        first create
 **************************************************************************************/
extern void initStorageManager (void)
{
    printf ("Initialization of the Storage Manager...\n");
}

/**************************************************************************************
 *Function Name: createPageFile
 *
 *Description:
 *          Create a new page file fileName. with initial file size of one page. Fill this single page with '\0' bytes.
 *  Steps -
 *              Create New File
 *          Allocate block of memory of PAGE_SIZE elements. Calloc initializes it's bits to zero
 *          Write the page to file
 *          Free the memory
 *          Close the file
 *Parameters:
 *              char *fileName
 *Return:
 *              RC_OK
 *Author:
 *              Uday Tak <utak@hawk.iit.edu>
 *Hisory:
 *              Date                  Name                                Content
 *              ----------            ------------------------------      ---------------
 *              9/14/2016             Uday Tak <utak@hawk.iit.edu>        first create
 *             9/20/2016              Pingyu Xue<pxue2@hawk.iit.edu>      rewrite the code
 **************************************************************************************/

//
//extern RC createPageFile (char *fileName)
//{
//    File* fptr;
//
//    /* open the file */
//    fptr = fopen(fileName, "rw+");
//
//    if (fptr == NULL)
//    {
//        return RC_FILE_OPEN_FAILED;
//    }
//
//    /* Allocate block of memory of PAGESIZE elements. Calloc initializes it's bits to zero */
//    SM_PageHandle blank_page = (SM_PageHandle) calloc(PAGESIZE,sizeof(char));
//
//    /* Write the page to file */
//    int FileWriteFlag = fwrite (blank_page, sizeof(char), PAGESIZE, fptr);
//
//    if(FileWriteFlag == 0)
//        return RC_FILE_WRITE_FAILED;
//
//    /* Free the memory */
//    free(blank_page);
//
//    /* Close the file */
//    int fileCloseFlag = fclose (fptr);
//
//    if (fileCloseFlag != 0)
//    {
//        printf("Error: File is closed with the error..\n");
//        return RC_FILE_CLOSE_FAILED;
//    }
//
//    return RC_OK;
//}


RC createPageFile(char *fileName){
    
    // pointer of the file, Memory size of the page.
    
    if (fileName == NULL){
        return RC_FILE_NOT_FOUND;
    }
    //Question(by Pingyu Xue):the file is create by ourself.  need check or not?
    //Answer(by ):
    
    FILE *newPageFile;
    newPageFile= fopen(fileName, "w+");
    
    if(newPageFile == NULL){
        fclose(newPageFile);
        return RC_CREATE_PAGE_FAIL;
    }
    
//     If  create the page success ,then ,set the pointer of the page.
//     do not waste the memeory!
    
    char *newPage;
    newPage = (char *) calloc(PAGE_SIZE, sizeof(char));

//    write in and check the result.
    size_t writeCheck = 0;
    writeCheck = fwrite(newPage, sizeof(char), PAGE_SIZE, newPageFile);
    
    if (writeCheck == PAGE_SIZE){
        fclose(newPageFile);
        return RC_OK;
    }
    else{
        fclose(newPageFile);
        free(newPage);
        return RC_CREATE_PAGE_FAIL;}

    
}
           
// Question(by Pingyu Xue): "calloc()"  set a pointer is same with "memset( ,'\0',)"
//                           can we just switch the both?  ps.( see the test in main.c)
// Answer (by ) :



/**************************************************************************************
 *Function Name: openPageFile
 *
 *Description:
 *          Open the page file and Initialize the existing file handle
 *  Steps -
 *              Open the File
 *          Initialize the handle
 *          Initialize pointer to file's first position
 *Parameters:
 *              char *fileName,
 *      SM_FileHandle *fHandle
 *Return:
 *              RC_OK
 *Author:
 *              Uday Tak <utak@hawk.iit.edu>
 *Hisory:
 *              Date                  Name                                Content
 *              ----------            ------------------------------      ---------------
 *              9/15/2016             Uday Tak <utak@hawk.iit.edu>        first create
 *              9/20/2016             Pingyu Xue<pxue2@hawk.iit.edu>      rewrite code
 **************************************************************************************/
//extern RC openPageFile (char *fileName, SM_FileHandle *fHandle)
//{
//    File* fptr;
//    
//    /* open the file */
//    fptr = fopen(fileName, "rw+");
//    
//    if (fptr == NULL)
//    {
//        return RC_FILE_OPEN_FAILED;
//    }
//    
//    /*Get the file's end*/
//    int seekFlag = fseek( fptr, 0, SEEK_END );
//    
//    if (seekFlag != 0)
//    {
//        return RC_SEEK_POSITION_FAILED;
//    }
//    
//    int EndPosition = ftell(fptr);
//    if ( EndPosition == -1) {
//        return RC_TELL_POSITION_FAILED;
//    }
//    
//    /*Get the total no of pages */
//    int TotalPages = (EndPosition + 1) / ;
//    
//    /*Initialize pointer to file's first position*/
//    int seekFlag = fseek( fptr, 0, SEEK_SET);
//    if (seekFlag != 0)
//    {
//        return RC_SEEK_POSITION_FAILED;
//    }
//    
//    
//    //wrong code
//    /*Initialize the handle */
//    // here is the 1st version code:
//    fhandle -> fileName = filename;
//    fhandle -> totalNumPages = TotalPages;
//    fhandle -> curPagePos = 0;
//    fhandle -> mgmtInfo = fptr;
//    
//    return RC_OK;

// rewrite:
RC openPageFile (char *fileName, SM_FileHandle *fHandle){
    
    FILE *newFile;
    size_t pageSize;
    int seekResult;
    
    newFile = fopen(fileName, "r");
    
    // open file right or not
    if( newFile != 0){
        seekResult = fseek(newFile, 0, SEEK_END);
        
        // seek set right or not
        if(seekResult == 0){
            pageSize = ftell(newFile);
            
            // indicator got or not
            if( pageSize != 0){
                
                size_t modPageNum = pageSize/pageSize;
                
                // calculate the page number for the totalPageNum
                if (modPageNum == 0){
                    fHandle->totalNumPages =(int) modPageNum;    // test 3: size_t mod num 
                }
                else{
                    fHandle->totalNumPages = (int) (modPageNum+1);
                }
                
                // ininilize the info .
                fHandle->curPagePos = 0;
                fHandle->fileName = fileName;
                fHandle->mgmtInfo = newFile;
                
                // reset the pointer.
                fseek(newFile, 0, SEEK_SET);
                free(newFile);
                
                return RC_OK;
            }
            else
                free(newFile);
                return RC_FUNC_Ftell_ERROR;  // ftell error'
            
        }
        else
            free(newFile);
            return RC_FUNC_Fseek_ERROR;      // fseek error
    }
    else
        free(newFile);
        return RC_FILE_NOT_FOUND;
}


/**************************************************************************************
 *Function Name: closePageFile
 *
 *Description:
 *          Close the page file
 *  Steps -
 *              Close the File
 *          return if fails
 *Parameters:
 *      SM_FileHandle *fHandle
 *Return:
 *              RC_OK
 *Author:
 *              Uday Tak <utak@hawk.iit.edu>
 *Hisory:
 *              Date                  Name                                Content
 *              ----------            ------------------------------      ---------------
 *              9/17/2016             Uday Tak <utak@hawk.iit.edu>        first create
 *              9/20/2016             Pingyu Xue<pxue2k@hawk.iit.edu>     check fhandle and fix return request
 **************************************************************************************/


RC closePageFile (SM_FileHandle *fHandle)
{
    if (fHandle == NULL){
        return RC_FILE_HANDLE_NOT_INIT;
    }
    
    
    int fileCloseFlag = fclose(fHandle->mgmtInfo);
    
    if (fileCloseFlag != 0)
    {
        return RC_FILE_CLOSE_ERROR;
    }
    return RC_OK;
    
    
    // Question( by Pingyu Xue): should free memeory in this function ?
    // Answer  (by ):
}

/**************************************************************************************
 *Function Name: destroyPageFile
 *
 *Description:
 *          Destroy the page file
 *  Steps -
 *              Close the File
 *          return if fails
 *Parameters:
 *      SM_FileHandle *fHandle
 *Return:
 *              RC_OK
 *Author:
 *              Uday Tak <utak@hawk.iit.edu>
 *Hisory:
 *              Date                  Name                                Content
 *              ----------            ------------------------------      ---------------
 *              9/18/2016             Uday Tak <utak@hawk.iit.edu>        first create
 *              9/20/2016             Pingyu Xue<pxue2k@hawk.iit.edu>     rewrite
 **************************************************************************************/

//extern RC destroyPageFile (char *fileName)
//{
//    fileRemoveFlag = remove(fileName);
//   	if(fileRemoveFlag != 0)
//        
//   	{
//      		return RC_FILE_REMOVE_FAILED;
//   	}
//    return RC_OK;
//}


RC destroyPageFile (char *fileName)
{
    if (fileName == NULL){
        return RC_FILE_NOT_FOUND;
    }
    
    int removerFile = 1;
    
    removerFile = remove(fileName);
    
    if (removerFile == 0){
        return RC_OK;
    }
    else
        return RC_FILE_REMOVE_ERROR;
}

    //Question(by Pingyu Xue):  remove ()? free memory ,should check again or not?
    //Answer(by ):






/**************************************************************************************
 *Function Name: readBlock
 *
 *Description:
 *            read any page(block) which the user want to read
 *            in a txt file which already has an SM_FileHandle
 *Parameters:
 *            int pageNum: page number of the page user want to read
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK;
 *Author:
 *            Pingyu Xue
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/19/2016             Pingyu Xue <pxue2@hawk.iit.edu>      first create
 *            9/20/2016             Pingyu Xue <pxue2@hawk.iit.edu>      change border of readStatment
 **************************************************************************************/


RC readBlock(int pageNum, SM_FileHandle *fhandle, SM_PageHandle memPage)
{
    int seekpostion = 1 ;       // initialize the var
    size_t readStatement = 0;  // initialize the var
    
    if (fhandle== NULL){
        return RC_FILE_HANDLE_NOT_INIT;
    }
    
    if (pageNum>=0 && fhandle->totalNumPages>pageNum){
        seekpostion=fseek(fhandle->mgmtInfo, sizeof(char)*PAGE_SIZE*pageNum, SEEK_SET);
        // Q1:??? offset= sizeof(char)  || Do we need the sizeof(char)?
        // Q4:??? why fandle->mamtInfo can be used for FILE *point?
        
        if (seekpostion==0){
            readStatement=fread(memPage, sizeof(char), PAGE_SIZE, fhandle->mgmtInfo);
            // Q2:???  size_t sizeof(char)  ||
            
            if (readStatement != 0){   //change from page_size
                fhandle->curPagePos=pageNum;
                return RC_OK;
            }
            // Q3: how to check the fread() have the right result ?
            else
                return RC_READ_FAILED;
        }
        else
            return RC_SET_POINTER_FAILED;
        
    }
    else
        return RC_READ_NON_EXISTING_PAGE;
}


/**************************************************************************************
 *Function Name: getBlockPos
 *
 *Description:
 *            find which page is reading
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *Return:
 *            (int) page number reading right now;
 *Author:
 *            Pingyu Xue
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/19/2016             Pingyu Xue <pxue2@hawk.iit.edu>      first create
 **************************************************************************************/

extern int getBlockPos(SM_FileHandle *fhandle){
    if (fhandle != NULL){
        return fhandle->curPagePos;
    }
    else
        return RC_FILE_HANDLE_NOT_INIT;
}

/**************************************************************************************
 *Function Name: readFirstBlock
 *
 *Description:
 *            show first page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            Pingyu Xue
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/19/2016             Pingyu Xue <pxue2@hawk.iit.edu>      first create
 **************************************************************************************/

RC readFirstBlock(SM_FileHandle *fhandle, SM_PageHandle mempage)
{
    
    if (fhandle!= NULL){
        return readBlock(0, fhandle, mempage);
        
    }
    else
        return RC_FILE_HANDLE_NOT_INIT;
}

/**************************************************************************************
 *Function Name: readPreviousBlock
 *
 *Description:
 *            show previous page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            Pingyu Xue
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/19/2016             Pingyu Xue <pxue2@hawk.iit.edu>      first create
 **************************************************************************************/

RC readPreviousBlock(SM_FileHandle *fhandle, SM_PageHandle mempage)
{
    // Does it have previous block?
    if (fhandle!=NULL){
        if (fhandle->curPagePos!=0){
            int currentPage= (fhandle->curPagePos)-1;
            return readBlock( currentPage, fhandle, mempage);
        }
        else
            return RC_READ_NON_EXISTING_PAGE;
    }
    else
        return RC_FILE_HANDLE_NOT_INIT;
}

/**************************************************************************************
 *Function Name: readCurrentBlock
 *
 *Description:
 *            show previous page content onto screen
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            Pingyu Xue
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/19/2016             Pingyu Xue <pxue2@hawk.iit.edu>      first create
 **************************************************************************************/

RC readCurrentBlock(SM_FileHandle *fhandle, SM_PageHandle mempage)
{
    if (fhandle != NULL)
    {
        int currentPage=fhandle->curPagePos;
        return readBlock(currentPage, fhandle, mempage);
    }
    else
        return RC_FILE_HANDLE_NOT_INIT;
}

/**************************************************************************************
 *Function Name: readNextBlock
 *
 *Description:
 *            show next page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            Pingyu Xue
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/19/2016             Pingyu Xue <pxue2@hawk.iit.edu>      first create
 **************************************************************************************/

RC readNextBlock(SM_FileHandle *fhandle, SM_PageHandle mempage)
{
    //    Does it have next block?
    if (fhandle != NULL){
        if(fhandle->curPagePos < fhandle->totalNumPages){
            int currentPage= (fhandle->curPagePos)+1;
            return readBlock(currentPage, fhandle, mempage);
        }
        else
            return RC_READ_NON_EXISTING_PAGE;
    }
    else
        return RC_FILE_HANDLE_NOT_INIT;
}

/**************************************************************************************
 *Function Name: readLastBlock
 *
 *Description:
 *            show last page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            Pingyu Xue
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            9/19/2016             Pingyu Xue <pxue2@hawk.iit.edu>      first create
 **************************************************************************************/


RC readLastBlock(SM_FileHandle *fhandle, SM_PageHandle mempage){
    if(fhandle != NULL){
        return readBlock(fhandle->totalNumPages, fhandle, mempage);
    }
    else
        return RC_FILE_HANDLE_NOT_INIT;
}



















