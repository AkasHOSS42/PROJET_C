FLAGS = -Wall -pedantic -g

all : union select moyenne_ponderee tri fusion formatage

union : line.o csm.o decode.o encode.o union.o
	gcc $(FLAGS) -o union line.o csm.o decode.o encode.o union.o
select : select.o line.o csm.o decode.o encode.o
	gcc $(FLAGS) -o select line.o csm.o decode.o encode.o select.o
moyenne_ponderee : moyenne_ponderee.o line.o csm.o decode.o encode.o
	gcc $(FLAGS) -o moyenne_ponderee line.o csm.o decode.o encode.o moyenne_ponderee.o
tri : tri.o line.o csm.o decode.o encode.o sort.o
	gcc $(FLAGS) -o tri line.o csm.o decode.o encode.o sort.o tri.o
fusion : fusion.o line.o csm.o decode.o encode.o sort.o
	gcc $(FLAGS) -o fusion line.o csm.o decode.o encode.o sort.o fusion.o
formatage : formatage.o line.o csm.o decode.o encode.o sort.o
	gcc $(FLAGS) -o formatage line.o csm.o decode.o encode.o sort.o formatage.o

union.o : union.c csm.h decode.h encode.h
	gcc $(FLAGS) -c union.c
select.o : select.c csm.h decode.h encode.h
	gcc $(FLAGS) -c select.c
moyenne_ponderee.o : moyenne_ponderee.c csm.h decode.h encode.h
	gcc $(FLAGS) -c moyenne_ponderee.c
tri.o : tri.c csm.h decode.h encode.h sort.h
	gcc $(FLAGS) -c tri.c
fusion.o : fusion.c csm.h decode.h encode.h sort.h
	gcc $(FLAGS) -c fusion.c
formatage.o : formatage.c csm.h decode.h encode.h sort.h
	gcc $(FLAGS) -c formatage.c

sort.o : sort.c csm.h
	gcc $(FLAGS) -c sort.c	
decode.o : decode.c csm.h
	gcc $(FLAGS) -c decode.c
encode.o : encode.c csm.h
	gcc $(FLAGS) -c encode.c
csm.o : csm.c line.h
	gcc $(FLAGS) -c csm.c
line.o : line.c
	gcc $(FLAGS) -c line.c
clean :
	rm -f *.o main union select moyenne_ponderee tri fusion formatage
