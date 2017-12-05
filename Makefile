FOLDERS = $(notdir $(shell find challenges -type d -not -path "*.git*"))

CPPCHECK ?= 1
.PHONY: $(FOLDERS)
$(FOLDERS):
		g++-7 -std=c++17 -Wall -Werror -Icommon $(shell find challenges/$@ common -name *.cpp) -o solution && ./solution
		cppcheck $(FOLDERS) --enable=style --enable=warning -Icommon --error-exitcode=1 -q
		
		