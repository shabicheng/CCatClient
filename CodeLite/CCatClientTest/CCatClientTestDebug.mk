##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=CCatClientTest
ConfigurationName      :=Debug
WorkspacePath          := "../../"
ProjectPath            := "../../CodeLite/CCatClientTest"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=zhangcheng
Date                   :=13/02/17
CodeLitePath           :="/home/zc/.codelite"
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="CCatClientTest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../src/CCatClient 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)CCatClient $(LibrarySwitch)pthread 
ArLibs                 :=  "CCatClient" "pthread" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../CCatClient/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(ObjectSuffix): ../../src/CCatClientTest/CatTestMain.cpp $(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../src/CCatClientTest/CatTestMain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(DependSuffix): ../../src/CCatClientTest/CatTestMain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(DependSuffix) -MM "../../src/CCatClientTest/CatTestMain.cpp"

$(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(PreprocessSuffix): ../../src/CCatClientTest/CatTestMain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClientTest_CatTestMain.cpp$(PreprocessSuffix) "../../src/CCatClientTest/CatTestMain.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


