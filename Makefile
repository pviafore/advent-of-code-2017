FOLDERS = $(notdir $(shell find challenges -type d -not -path "*.git*"))

.PHONY: $(FOLDERS)
$(FOLDERS): verify
		g++-7 -std=c++17 -Icommon $(shell find challenges/$@ common -name *.cpp) -o solution && ./solution

.PHONY: verify
verify:
		cppcheck $(FOLDERS) --enable=style --enable=warning -Icommon --error-exitcode=1 -q