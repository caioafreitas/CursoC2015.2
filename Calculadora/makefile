include cfg.mak

all: $(DIST)/$(EXE)
	@echo Compilado!
	
$(DIST)/$(EXE): $(OBJS)
	@echo Gerando $(EXE) ...
	@gcc $(OBJS) -o $(DIST)/$(EXE)
	
$(OBJ)/%.o: $(SRC)/%.c
	@echo Compilando $< ...
	@gcc -c $< -I ./$(INC) -o $@
	
clean:
	@rm -rf $(OBJ)/*.o $(DIST)/$(EXE)
	@echo Limpo!
	
remake: clean all
	@echo Done!
	