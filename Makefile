# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Main executable
EXE = solutionchecker

# Find all exercises automatically
EXERCISES := $(shell find . -maxdepth 1 -type d -name 'ex??' -printf '%f\n')

# Compile the main program and run it
all: $(EXE)
	./$(EXE)

$(EXE): main.c  functions.c printheader.c
	$(CC) $(CFLAGS) main.c functions.c printheader.c -o $(EXE)

# Reset all exercise source files (empty exXX.c files)
again:
	@echo "[↻] Resetting all exercises...\n"
	@for dir in $(EXERCISES); do \
		if [ -f "$$dir/$$dir.c" ]; then \
			> "$$dir/$$dir.c"; \
			echo "// $$dir - Good luck and enjoy coding!" > "$$dir/$$dir.c"; \
		fi \
	done
	@echo "0" > progress.txt
	@echo "[✔] All exercises reset. You can start over!"

# Clean compiled binaries and output files
clean:
	@echo "[::] Cleaning up compiled files..."
	rm -f $(EXE) ex*/ex*.out tmp/exercise.out tmp/user_output.txt
	find . -type f -name '*.out' -delete
	@echo "[✔] Cleanup complete."
