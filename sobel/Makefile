obj-m += sobel_edge.o
KDIR = 
RESULT = sobel
SRC = $(RESULT).cpp
CCC = g++

all:
	make -C $(KDIR) M=$(PWD) modules
	$(CCC) -o $(RESULT) $(SRC)

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f $(RESULT)

