.PHONY: clean All

All:
	@echo "----------Building project:[ CCatClientTest - Release ]----------"
	@cd "CodeLite/CCatClientTest" && "$(MAKE)" -f  "CCatClientTest.mk"
	@cd "CodeLite/CCatClient" && "$(MAKE)" -f  "CCatClient.mk"
clean:
	@echo "----------Cleaning project:[ CCatClientTest - Release ]----------"
	@cd "CodeLite/CCatClientTest" && "$(MAKE)" -f  "CCatClientTest.mk" clean
	@cd "CodeLite/CCatClient" && "$(MAKE)" -f  "CCatClient.mk" clean
