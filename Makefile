FOLDERS = $(notdir $(shell find challenges -type d -not -path "*.git*"))

CPPCHECK ?= 1
.PHONY: $(FOLDERS)
$(FOLDERS):
		g++ -std=c++20 -g -Wall -Werror -Icommon $(shell find challenges/$@ common -name *.cpp) -o solution && ./solution
