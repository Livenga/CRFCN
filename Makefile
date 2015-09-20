CC = gcc
#FLAGS = -lm -ljpeg -lpng -lOpenCL -Wall -O2 -g
FLAGS = -lm -lpng -lOpenCL

NOMAKEDIR = .git% data% doc% src/bin% src/kernel%
CSOURCES  = $(shell find src/* -name \*.c)
SRCS 		  = $(filter-out $(NOMAKEDIR), $(CSOURCES))
DIRS 			= $(dir $(SRCS))
BINDIRS 	= $(addprefix $(OBJDIR)/, $(DIRS))
OBJDIR 	  = objs

OBJS = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(SRCS)))
PRJC = crfcn

ifeq "$(strip $(OBJDIR))" ""
	OBJDIR = .
endif

ifeq "$(strip $(DIRS))" ""
	OBJDIR = .
endif

default:
	@[ -d  $(OBJDIR)   ] || mkdir -p $(OBJDIR)
	@[ -d "$(BINDIRS)" ] || mkdir -p $(BINDIRS)
	#@make all --no-print-directory
	@make $(PRJC)

$(PRJC): $(OBJS) $(LIBS)
	$(CC) -o $@ $^ $(FLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(FLAGS) -o $@ -c $<

all: clean $(PRJC)

clean:
	rm -v $(PRJC) $(OBJS)

run:
	./$(PRJC) -b librfcn.so
