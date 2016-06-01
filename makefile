# Edit these variables if more directories are needed.
# Separate each entry by spaces.
CXX_DIRS=src

# This is likely specific to *nix.
PROJ=$(shell basename $(CURDIR))


CXX=g++
LD=g++
AR=ar

DEFINES=-DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED

DEBUG=yeah do debug

ifdef DEBUG
	DEBUG_FLAGS=-gdwarf-3 -g
	
	#BASE_FLAGS=-Wall -g -march=native -mtune=native $(DEFINES)
	BASE_FLAGS=-Wall -g $(DEFINES)
else
	#BASE_FLAGS=-Wall -O3 -march=native -mtune=native $(DEFINES)
	BASE_FLAGS=-Wall -O3 $(DEFINES)
endif


#CXX_FLAGS=-std=c++14 -pipe -I/usr/include $(BASE_FLAGS) `pkg-config \
#	--cflags Qt5Core Qt5Gui Qt5Widgets sfml-window sfml-graphics` -fPIC

CXX_FLAGS=-std=c++14 -pipe $(BASE_FLAGS) -fPIC
S_FLAGS=
#LD_FLAGS=-lm `pkg-config --libs Qt5Core Qt5Gui Qt5Widgets sfml-window \
#	sfml-graphics` -lpugixml $(DEBUG_FLAGS)
#LD_FLAGS=-lm -lpugixml $(DEBUG_FLAGS)
LD_FLAGS=-lm $(DEBUG_FLAGS)



ifeq ($(OS),Windows_NT)
	LD_FLAGS+=-Lpugixml_source -Wl,-Bstatic -lpugixml -Wl,-Bdynamic \
		-lQtWidgets -lQtGui -lQtCore -lsfml-graphics -lsfml-window \
		-lsfml-system
else
	## Uncomment these if you want the pkg-config results to be "cached" 
	## for the current run of make.  This may cause a problem if the 
	## directories of any libraries change, and a new build is done without 
	## doing make clean first.  Oh, and this appears to be specific to GNU 
	## Make as well.  GNU Make is definitely the best version of Make :^).
	#CXX_FLAGS+=$(shell pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets \
	#	sfml-window sfml-graphics)
	#LD_FLAGS+=$(shell pkg-config --libs Qt5Core Qt5Gui Qt5Widgets \
	#	sfml-window sfml-graphics) -lpugixml
	
	CXX_FLAGS+=`pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets \
		sfml-window sfml-graphics`
	LD_FLAGS+=`pkg-config --libs Qt5Core Qt5Gui Qt5Widgets \
		sfml-window sfml-graphics` -lpugixml
endif


OBJDIR=objs
DEPDIR=deps
OBJDIR_TEMP=objs_temp
MOC_SOURCE_DIR=moc_sources


# No, this stuff isn't portable to non-GNU versions of Make, at least as
# far as I know.
CXX_SOURCES=$(foreach DIR,$(CXX_DIRS),$(notdir $(wildcard $(DIR)/*.cpp)))
CXX_HEADERS=$(foreach DIR,$(CXX_DIRS),$(notdir $(wildcard $(DIR)/*.hpp)))
CXX_MOC_SOURCES=$(patsubst %.hpp,$(MOC_SOURCE_DIR)/%.moc.cpp, \
	$(CXX_HEADERS))

export VPATH	:=	$(foreach DIR,$(CXX_DIRS),$(CURDIR)/$(DIR))

CXX_OFILES=$(patsubst %.cpp,$(OBJDIR)/%.o,$(CXX_SOURCES))
CXX_MOC_OFILES=$(patsubst $(MOC_SOURCE_DIR)/%.moc.cpp,$(OBJDIR)/%.moc.o, \
	$(CXX_MOC_SOURCES))
OFILES=$(CXX_MOC_OFILES) $(CXX_OFILES)

CXX_PFILES=$(patsubst %.cpp,$(DEPDIR)/%.P,$(CXX_SOURCES))
PFILES=$(CXX_PFILES)

CXX_OFILES_TEMP=$(patsubst %.cpp,$(OBJDIR_TEMP)/%.o,$(CXX_SOURCES))
OFILES_TEMP=$(CXX_OFILES_TEMP)




all : all_pre $(OFILES)
	$(LD) $(OFILES) -o $(PROJ) $(LD_FLAGS)

all_objs : all_pre $(OFILES)
	@#

ifeq ($(OS),Windows_NT)
all_pre : pugixml_static_stuff
	mkdir -p $(OBJDIR) $(DEPDIR) $(MOC_SOURCE_DIR)

pugixml_static_stuff : 
	$(CXX) $(CXX_FLAGS) -c pugixml_source/pugixml.cpp \
		-o pugixml_source/pugixml.o
	$(AR) rcs pugixml_source/libpugixml.a pugixml_source/pugixml.o

else
all_pre :
	mkdir -p $(OBJDIR) $(DEPDIR) $(MOC_SOURCE_DIR)
endif




$(CXX_OFILES) : $(OBJDIR)/%.o : %.cpp
	@#echo "Generating dependency information for "$@"...."
	@echo $@" was updated or has no object file.  (Re)Compiling...."
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@
	@cp $(OBJDIR)/$*.d $(DEPDIR)/$*.P
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(OBJDIR)/$*.d >> $(DEPDIR)/$*.P
	@rm -f $(OBJDIR)/$*.d

-include $(PFILES)

ifeq ($(OS),Windows_NT)
$(CXX_MOC_SOURCES) : $(MOC_SOURCE_DIR)/%.moc.cpp : %.hpp
	@moc $(DEFINES) $< -o $@
else
$(CXX_MOC_SOURCES) : $(MOC_SOURCE_DIR)/%.moc.cpp : %.hpp
	@#moc-qt5 $(DEFINES) $< -o $@
	@#/usr/lib/qt5/bin/moc $(DEFINES) $< -o $@
	@moc -qt=5 $(DEFINES) $< -o $@
endif

$(CXX_MOC_OFILES) : $(OBJDIR)/%.moc.o : $(MOC_SOURCE_DIR)/%.moc.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@


#¯\(°_o)/¯

.PHONY : clean
clean :
	rm -rfv $(OBJDIR) $(DEPDIR) $(MOC_SOURCE_DIR) $(PROJ) tags *.taghl

.PHONY : clean_objs_with_no_source
clean_objs_with_no_source :
	@mkdir -p $(OBJDIR_TEMP)
	@#ls $(OBJDIR)
	@echo "Removing object files that don't have corresponding source files...."
	@for objfile in $(OFILES); \
	do \
		if [ -f $$objfile ]; \
		then \
			mv $$objfile $(OBJDIR_TEMP); \
		fi; \
	done;
	@#ls $(OBJDIR_TEMP)
	@rm -rf $(OBJDIR)
	@mkdir -p $(OBJDIR)
	@for objfile in $(OFILES_TEMP); \
	do \
		if [ -f $$objfile ]; \
		then \
			mv $$objfile $(OBJDIR); \
		fi; \
	done;
	@#ls $(OBJDIR)
	@rmdir $(OBJDIR_TEMP)
	
	
	@#rm -rfv $(OBJDIR_TEMP)


