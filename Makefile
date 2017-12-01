FOLDERS = $(notdir $(shell find -type d -not -path "*.git*"))
all:
		find -type d -not -path *.git*
		echo $(FOLDERS)

.PHONY: $(FOLDERS)
$(FOLDERS):
		g++-7 -I$@ $(shell find $@ -name *.cpp) -o solution && ./solution