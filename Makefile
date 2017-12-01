FOLDERS = $(notdir $(shell find -type d -not -path "*.git*"))
all:
		find -type d -not -path *.git*
		echo $(FOLDERS)

.PHONY: $(FOLDERS)
$(FOLDERS): verify
		g++-7 -std=c++17 -Icommon $(shell find $@ common -name *.cpp) -o solution && ./solution

.PHONY: verify
verify:
		cppcheck $(FOLDERS) --enable=style --enable=warning -Icommon --error-exitcode=1 -q