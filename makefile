

dberror.o: dberror.c
	
	gcc -c dberror.c -o dberror.o

       
storge_mgr.o:storge_mgr.c
	
	gcc -c storage_mgr.c -o storage.o

test_assign1_1.o:test_assign1_1.c
	
	gcc -c test_assign1_1.c -o test_assign1_1.o

525test: dberror.o storage_mgr.o test_assign1_1.o
	
	gcc  dberror.o storage_mgr.o test_assign1_1.o -o 525test




clean:
	rm  525test
