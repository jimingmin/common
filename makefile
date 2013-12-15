BASE_DIR= ${HOME}

DEBUG_LIB_DIR = $(BASE_DIR)/lib/Debug
RELEASE_LIB_DIR = $(BASE_DIR)/lib/Release

ifeq ($(mode),d)
	LIB_DIR = $(DEBUG_LIB_DIR)
	CPPFLAGS= -g -fPIC -Wall -DDEBUG $(INC) -Wno-invalid-offsetof
	LDFLAGS = -g -fPIC -L$(LIB_DIR)
else
	LIB_DIR = $(RELEASE_LIB_DIR)
	CPPFLAGS= -g -fPIC -Wall  $(INC) -Wno-invalid-offsetof
	LDFLAGS = -g -fPIC -L$(LIB_DIR)
endif

OBJ_DIR	= ./.objs

TARGET	= $(DEBUG_TARGET)

DEBUG_TARGET = $(LIB_DIR)/libcommon.a

SRC = $(wildcard *.cpp)
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o,$(SRC)))

INC = -I$(COMMON_INCLUDE_DIR)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $?

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)
