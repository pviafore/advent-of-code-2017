FOLDERS = $(notdir $(shell find -type d -not -path "*.git*"))
all:
		find -type d -not -path *.git*
		echo $(FOLDERS)

.PHONY: $(FOLDERS)
$(FOLDERS):
		g++-7 -std=c++17 -Icommon $(shell find $@ -name *.cpp) -o solution && ./solution