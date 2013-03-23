BIN		:= bound

SDIR	:= src
ODIR	:= obj

CFLAGS	:= -std=c99 -Wall -O1
LIBS 	:= -lm -lbsd

SRC=utils.c main.c check.c
OBJ=$(patsubst %.c,$(ODIR)/%.o,$(SRC))

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJ): $(ODIR)

$(ODIR):
	mkdir $@

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -Rf $(BIN) $(ODIR)

