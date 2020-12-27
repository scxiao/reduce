HIP_PATH=/opt/rocm/hip
HIPCC=$(HIP_PATH)/bin/hipcc


CPPFLAGS=-c -O2 -std=c++14
NVCCFLAGS=-c -O2 -std=c++14
LDDFLAGS= -lpthread -lcudart -L/usr/local/cuda/lib64
CXX=g++
NVCC=nvcc

SRC=.
OBJ=obj
EXE=.
OBJFILES=$(OBJ)/reduce_cuda.o \
         $(OBJ)/reduce_thread.o \
         $(OBJ)/test_reduce.o

all: create \
     test_reduce

test_reduce : $(OBJFILES)
	$(CXX) -o $@ $^ $(LDDFLAGS)

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

$(OBJ)/%.o : $(SRC)/%.cu
	$(NVCC) $(NVCCFLAGS) -o $@ $<

create:
	if [ ! -d "./obj" ]; then \
		mkdir obj; \
	fi

clean:
	rm -rf 	$(OBJ) $(EXE)/test_reduce
