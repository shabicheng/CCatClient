##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=CCatClient
ConfigurationName      :=Release
WorkspacePath          := "../../"
ProjectPath            := "../../CodeLite/CCatClient"
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=zhangcheng
Date                   :=12/02/17
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="CCatClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O2 -fPIC
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=   $(Preprocessors)
CFLAGS   :=  -fPIC $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/CCatClient_adlist.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_ae.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_anet.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatClient.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatClientCommon.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatClientConfig.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatContext.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatEncoder.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatEvent.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(ObjectSuffix) \
	$(IntermediateDirectory)/CCatClient_CatMessage.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatMessageManager.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatMessageSender.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatMetric.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatMonitor.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatRootMessage.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatStatistics.c$(ObjectSuffix) \
	$(IntermediateDirectory)/CCatClient_CatTrace.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatTransaction.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_CLog.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_sds.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_TimeUtility.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_ZRLog.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(ObjectSuffix) $(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(ObjectSuffix) 



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
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "./.build-release"
	@echo rebuilt > "./.build-release/CCatClient"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/CCatClient_adlist.c$(ObjectSuffix): ../../src/CCatClient/adlist.c $(IntermediateDirectory)/CCatClient_adlist.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/adlist.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_adlist.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_adlist.c$(DependSuffix): ../../src/CCatClient/adlist.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_adlist.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_adlist.c$(DependSuffix) -MM "../../src/CCatClient/adlist.c"

$(IntermediateDirectory)/CCatClient_adlist.c$(PreprocessSuffix): ../../src/CCatClient/adlist.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_adlist.c$(PreprocessSuffix) "../../src/CCatClient/adlist.c"

$(IntermediateDirectory)/CCatClient_ae.c$(ObjectSuffix): ../../src/CCatClient/ae.c $(IntermediateDirectory)/CCatClient_ae.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/ae.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_ae.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_ae.c$(DependSuffix): ../../src/CCatClient/ae.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_ae.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_ae.c$(DependSuffix) -MM "../../src/CCatClient/ae.c"

$(IntermediateDirectory)/CCatClient_ae.c$(PreprocessSuffix): ../../src/CCatClient/ae.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_ae.c$(PreprocessSuffix) "../../src/CCatClient/ae.c"

$(IntermediateDirectory)/CCatClient_anet.c$(ObjectSuffix): ../../src/CCatClient/anet.c $(IntermediateDirectory)/CCatClient_anet.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/anet.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_anet.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_anet.c$(DependSuffix): ../../src/CCatClient/anet.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_anet.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_anet.c$(DependSuffix) -MM "../../src/CCatClient/anet.c"

$(IntermediateDirectory)/CCatClient_anet.c$(PreprocessSuffix): ../../src/CCatClient/anet.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_anet.c$(PreprocessSuffix) "../../src/CCatClient/anet.c"

$(IntermediateDirectory)/CCatClient_CatClient.c$(ObjectSuffix): ../../src/CCatClient/CatClient.c $(IntermediateDirectory)/CCatClient_CatClient.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatClient.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatClient.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatClient.c$(DependSuffix): ../../src/CCatClient/CatClient.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatClient.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatClient.c$(DependSuffix) -MM "../../src/CCatClient/CatClient.c"

$(IntermediateDirectory)/CCatClient_CatClient.c$(PreprocessSuffix): ../../src/CCatClient/CatClient.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatClient.c$(PreprocessSuffix) "../../src/CCatClient/CatClient.c"

$(IntermediateDirectory)/CCatClient_CatClientCommon.c$(ObjectSuffix): ../../src/CCatClient/CatClientCommon.c $(IntermediateDirectory)/CCatClient_CatClientCommon.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatClientCommon.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatClientCommon.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatClientCommon.c$(DependSuffix): ../../src/CCatClient/CatClientCommon.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatClientCommon.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatClientCommon.c$(DependSuffix) -MM "../../src/CCatClient/CatClientCommon.c"

$(IntermediateDirectory)/CCatClient_CatClientCommon.c$(PreprocessSuffix): ../../src/CCatClient/CatClientCommon.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatClientCommon.c$(PreprocessSuffix) "../../src/CCatClient/CatClientCommon.c"

$(IntermediateDirectory)/CCatClient_CatClientConfig.c$(ObjectSuffix): ../../src/CCatClient/CatClientConfig.c $(IntermediateDirectory)/CCatClient_CatClientConfig.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatClientConfig.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatClientConfig.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatClientConfig.c$(DependSuffix): ../../src/CCatClient/CatClientConfig.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatClientConfig.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatClientConfig.c$(DependSuffix) -MM "../../src/CCatClient/CatClientConfig.c"

$(IntermediateDirectory)/CCatClient_CatClientConfig.c$(PreprocessSuffix): ../../src/CCatClient/CatClientConfig.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatClientConfig.c$(PreprocessSuffix) "../../src/CCatClient/CatClientConfig.c"

$(IntermediateDirectory)/CCatClient_CatContext.c$(ObjectSuffix): ../../src/CCatClient/CatContext.c $(IntermediateDirectory)/CCatClient_CatContext.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatContext.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatContext.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatContext.c$(DependSuffix): ../../src/CCatClient/CatContext.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatContext.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatContext.c$(DependSuffix) -MM "../../src/CCatClient/CatContext.c"

$(IntermediateDirectory)/CCatClient_CatContext.c$(PreprocessSuffix): ../../src/CCatClient/CatContext.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatContext.c$(PreprocessSuffix) "../../src/CCatClient/CatContext.c"

$(IntermediateDirectory)/CCatClient_CatEncoder.c$(ObjectSuffix): ../../src/CCatClient/CatEncoder.c $(IntermediateDirectory)/CCatClient_CatEncoder.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatEncoder.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatEncoder.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatEncoder.c$(DependSuffix): ../../src/CCatClient/CatEncoder.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatEncoder.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatEncoder.c$(DependSuffix) -MM "../../src/CCatClient/CatEncoder.c"

$(IntermediateDirectory)/CCatClient_CatEncoder.c$(PreprocessSuffix): ../../src/CCatClient/CatEncoder.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatEncoder.c$(PreprocessSuffix) "../../src/CCatClient/CatEncoder.c"

$(IntermediateDirectory)/CCatClient_CatEvent.c$(ObjectSuffix): ../../src/CCatClient/CatEvent.c $(IntermediateDirectory)/CCatClient_CatEvent.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatEvent.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatEvent.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatEvent.c$(DependSuffix): ../../src/CCatClient/CatEvent.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatEvent.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatEvent.c$(DependSuffix) -MM "../../src/CCatClient/CatEvent.c"

$(IntermediateDirectory)/CCatClient_CatEvent.c$(PreprocessSuffix): ../../src/CCatClient/CatEvent.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatEvent.c$(PreprocessSuffix) "../../src/CCatClient/CatEvent.c"

$(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(ObjectSuffix): ../../src/CCatClient/CatHeartBeat.c $(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatHeartBeat.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(DependSuffix): ../../src/CCatClient/CatHeartBeat.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(DependSuffix) -MM "../../src/CCatClient/CatHeartBeat.c"

$(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(PreprocessSuffix): ../../src/CCatClient/CatHeartBeat.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatHeartBeat.c$(PreprocessSuffix) "../../src/CCatClient/CatHeartBeat.c"

$(IntermediateDirectory)/CCatClient_CatMessage.c$(ObjectSuffix): ../../src/CCatClient/CatMessage.c $(IntermediateDirectory)/CCatClient_CatMessage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatMessage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatMessage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatMessage.c$(DependSuffix): ../../src/CCatClient/CatMessage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatMessage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatMessage.c$(DependSuffix) -MM "../../src/CCatClient/CatMessage.c"

$(IntermediateDirectory)/CCatClient_CatMessage.c$(PreprocessSuffix): ../../src/CCatClient/CatMessage.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatMessage.c$(PreprocessSuffix) "../../src/CCatClient/CatMessage.c"

$(IntermediateDirectory)/CCatClient_CatMessageManager.c$(ObjectSuffix): ../../src/CCatClient/CatMessageManager.c $(IntermediateDirectory)/CCatClient_CatMessageManager.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatMessageManager.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatMessageManager.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatMessageManager.c$(DependSuffix): ../../src/CCatClient/CatMessageManager.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatMessageManager.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatMessageManager.c$(DependSuffix) -MM "../../src/CCatClient/CatMessageManager.c"

$(IntermediateDirectory)/CCatClient_CatMessageManager.c$(PreprocessSuffix): ../../src/CCatClient/CatMessageManager.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatMessageManager.c$(PreprocessSuffix) "../../src/CCatClient/CatMessageManager.c"

$(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(ObjectSuffix): ../../src/CCatClient/CatMessageMergeAndEncode.c $(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatMessageMergeAndEncode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(DependSuffix): ../../src/CCatClient/CatMessageMergeAndEncode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(DependSuffix) -MM "../../src/CCatClient/CatMessageMergeAndEncode.c"

$(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(PreprocessSuffix): ../../src/CCatClient/CatMessageMergeAndEncode.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatMessageMergeAndEncode.c$(PreprocessSuffix) "../../src/CCatClient/CatMessageMergeAndEncode.c"

$(IntermediateDirectory)/CCatClient_CatMessageSender.c$(ObjectSuffix): ../../src/CCatClient/CatMessageSender.c $(IntermediateDirectory)/CCatClient_CatMessageSender.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatMessageSender.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatMessageSender.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatMessageSender.c$(DependSuffix): ../../src/CCatClient/CatMessageSender.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatMessageSender.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatMessageSender.c$(DependSuffix) -MM "../../src/CCatClient/CatMessageSender.c"

$(IntermediateDirectory)/CCatClient_CatMessageSender.c$(PreprocessSuffix): ../../src/CCatClient/CatMessageSender.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatMessageSender.c$(PreprocessSuffix) "../../src/CCatClient/CatMessageSender.c"

$(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(ObjectSuffix): ../../src/CCatClient/CatMessigeIdHelper.c $(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatMessigeIdHelper.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(DependSuffix): ../../src/CCatClient/CatMessigeIdHelper.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(DependSuffix) -MM "../../src/CCatClient/CatMessigeIdHelper.c"

$(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(PreprocessSuffix): ../../src/CCatClient/CatMessigeIdHelper.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatMessigeIdHelper.c$(PreprocessSuffix) "../../src/CCatClient/CatMessigeIdHelper.c"

$(IntermediateDirectory)/CCatClient_CatMetric.c$(ObjectSuffix): ../../src/CCatClient/CatMetric.c $(IntermediateDirectory)/CCatClient_CatMetric.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatMetric.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatMetric.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatMetric.c$(DependSuffix): ../../src/CCatClient/CatMetric.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatMetric.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatMetric.c$(DependSuffix) -MM "../../src/CCatClient/CatMetric.c"

$(IntermediateDirectory)/CCatClient_CatMetric.c$(PreprocessSuffix): ../../src/CCatClient/CatMetric.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatMetric.c$(PreprocessSuffix) "../../src/CCatClient/CatMetric.c"

$(IntermediateDirectory)/CCatClient_CatMonitor.c$(ObjectSuffix): ../../src/CCatClient/CatMonitor.c $(IntermediateDirectory)/CCatClient_CatMonitor.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatMonitor.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatMonitor.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatMonitor.c$(DependSuffix): ../../src/CCatClient/CatMonitor.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatMonitor.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatMonitor.c$(DependSuffix) -MM "../../src/CCatClient/CatMonitor.c"

$(IntermediateDirectory)/CCatClient_CatMonitor.c$(PreprocessSuffix): ../../src/CCatClient/CatMonitor.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatMonitor.c$(PreprocessSuffix) "../../src/CCatClient/CatMonitor.c"

$(IntermediateDirectory)/CCatClient_CatRootMessage.c$(ObjectSuffix): ../../src/CCatClient/CatRootMessage.c $(IntermediateDirectory)/CCatClient_CatRootMessage.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatRootMessage.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatRootMessage.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatRootMessage.c$(DependSuffix): ../../src/CCatClient/CatRootMessage.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatRootMessage.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatRootMessage.c$(DependSuffix) -MM "../../src/CCatClient/CatRootMessage.c"

$(IntermediateDirectory)/CCatClient_CatRootMessage.c$(PreprocessSuffix): ../../src/CCatClient/CatRootMessage.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatRootMessage.c$(PreprocessSuffix) "../../src/CCatClient/CatRootMessage.c"

$(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(ObjectSuffix): ../../src/CCatClient/CatServerConnManager.c $(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatServerConnManager.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(DependSuffix): ../../src/CCatClient/CatServerConnManager.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(DependSuffix) -MM "../../src/CCatClient/CatServerConnManager.c"

$(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(PreprocessSuffix): ../../src/CCatClient/CatServerConnManager.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatServerConnManager.c$(PreprocessSuffix) "../../src/CCatClient/CatServerConnManager.c"

$(IntermediateDirectory)/CCatClient_CatStatistics.c$(ObjectSuffix): ../../src/CCatClient/CatStatistics.c $(IntermediateDirectory)/CCatClient_CatStatistics.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatStatistics.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatStatistics.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatStatistics.c$(DependSuffix): ../../src/CCatClient/CatStatistics.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatStatistics.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatStatistics.c$(DependSuffix) -MM "../../src/CCatClient/CatStatistics.c"

$(IntermediateDirectory)/CCatClient_CatStatistics.c$(PreprocessSuffix): ../../src/CCatClient/CatStatistics.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatStatistics.c$(PreprocessSuffix) "../../src/CCatClient/CatStatistics.c"

$(IntermediateDirectory)/CCatClient_CatTrace.c$(ObjectSuffix): ../../src/CCatClient/CatTrace.c $(IntermediateDirectory)/CCatClient_CatTrace.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatTrace.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatTrace.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatTrace.c$(DependSuffix): ../../src/CCatClient/CatTrace.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatTrace.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatTrace.c$(DependSuffix) -MM "../../src/CCatClient/CatTrace.c"

$(IntermediateDirectory)/CCatClient_CatTrace.c$(PreprocessSuffix): ../../src/CCatClient/CatTrace.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatTrace.c$(PreprocessSuffix) "../../src/CCatClient/CatTrace.c"

$(IntermediateDirectory)/CCatClient_CatTransaction.c$(ObjectSuffix): ../../src/CCatClient/CatTransaction.c $(IntermediateDirectory)/CCatClient_CatTransaction.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatTransaction.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatTransaction.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatTransaction.c$(DependSuffix): ../../src/CCatClient/CatTransaction.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatTransaction.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatTransaction.c$(DependSuffix) -MM "../../src/CCatClient/CatTransaction.c"

$(IntermediateDirectory)/CCatClient_CatTransaction.c$(PreprocessSuffix): ../../src/CCatClient/CatTransaction.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatTransaction.c$(PreprocessSuffix) "../../src/CCatClient/CatTransaction.c"

$(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(ObjectSuffix): ../../src/CCatClient/CatTranscationHelper.c $(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CatTranscationHelper.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(DependSuffix): ../../src/CCatClient/CatTranscationHelper.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(DependSuffix) -MM "../../src/CCatClient/CatTranscationHelper.c"

$(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(PreprocessSuffix): ../../src/CCatClient/CatTranscationHelper.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CatTranscationHelper.c$(PreprocessSuffix) "../../src/CCatClient/CatTranscationHelper.c"

$(IntermediateDirectory)/CCatClient_CLog.c$(ObjectSuffix): ../../src/CCatClient/CLog.c $(IntermediateDirectory)/CCatClient_CLog.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/CLog.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_CLog.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_CLog.c$(DependSuffix): ../../src/CCatClient/CLog.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_CLog.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_CLog.c$(DependSuffix) -MM "../../src/CCatClient/CLog.c"

$(IntermediateDirectory)/CCatClient_CLog.c$(PreprocessSuffix): ../../src/CCatClient/CLog.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_CLog.c$(PreprocessSuffix) "../../src/CCatClient/CLog.c"

$(IntermediateDirectory)/CCatClient_sds.c$(ObjectSuffix): ../../src/CCatClient/sds.c $(IntermediateDirectory)/CCatClient_sds.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/sds.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_sds.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_sds.c$(DependSuffix): ../../src/CCatClient/sds.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_sds.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_sds.c$(DependSuffix) -MM "../../src/CCatClient/sds.c"

$(IntermediateDirectory)/CCatClient_sds.c$(PreprocessSuffix): ../../src/CCatClient/sds.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_sds.c$(PreprocessSuffix) "../../src/CCatClient/sds.c"

$(IntermediateDirectory)/CCatClient_TimeUtility.c$(ObjectSuffix): ../../src/CCatClient/TimeUtility.c $(IntermediateDirectory)/CCatClient_TimeUtility.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/TimeUtility.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_TimeUtility.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_TimeUtility.c$(DependSuffix): ../../src/CCatClient/TimeUtility.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_TimeUtility.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_TimeUtility.c$(DependSuffix) -MM "../../src/CCatClient/TimeUtility.c"

$(IntermediateDirectory)/CCatClient_TimeUtility.c$(PreprocessSuffix): ../../src/CCatClient/TimeUtility.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_TimeUtility.c$(PreprocessSuffix) "../../src/CCatClient/TimeUtility.c"

$(IntermediateDirectory)/CCatClient_ZRLog.c$(ObjectSuffix): ../../src/CCatClient/ZRLog.c $(IntermediateDirectory)/CCatClient_ZRLog.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/ZRLog.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_ZRLog.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_ZRLog.c$(DependSuffix): ../../src/CCatClient/ZRLog.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_ZRLog.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_ZRLog.c$(DependSuffix) -MM "../../src/CCatClient/ZRLog.c"

$(IntermediateDirectory)/CCatClient_ZRLog.c$(PreprocessSuffix): ../../src/CCatClient/ZRLog.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_ZRLog.c$(PreprocessSuffix) "../../src/CCatClient/ZRLog.c"

$(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(ObjectSuffix): ../../src/CCatClient/ZRSafeQueue.c $(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/ZRSafeQueue.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(DependSuffix): ../../src/CCatClient/ZRSafeQueue.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(DependSuffix) -MM "../../src/CCatClient/ZRSafeQueue.c"

$(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(PreprocessSuffix): ../../src/CCatClient/ZRSafeQueue.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_ZRSafeQueue.c$(PreprocessSuffix) "../../src/CCatClient/ZRSafeQueue.c"

$(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(ObjectSuffix): ../../src/CCatClient/ZRStaticQueue.c $(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../src/CCatClient/ZRStaticQueue.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(DependSuffix): ../../src/CCatClient/ZRStaticQueue.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(ObjectSuffix) -MF$(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(DependSuffix) -MM "../../src/CCatClient/ZRStaticQueue.c"

$(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(PreprocessSuffix): ../../src/CCatClient/ZRStaticQueue.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CCatClient_ZRStaticQueue.c$(PreprocessSuffix) "../../src/CCatClient/ZRStaticQueue.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


