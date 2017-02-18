.PHONY: clean All

All:
	@echo "----------Building project:[ CCatClientTest - Debug ]----------"
	@cd "CodeLite/CCatClient" && "$(MAKE)" -f  "CCatClient.mk"
	@cd "CodeLite/CCatClientTest" && "$(MAKE)" -f  "CCatClientTest.mk"
clean:
	@echo "----------Cleaning project:[ CCatClientTest - Debug ]----------"
	@cd "CodeLite/CCatClient" && "$(MAKE)" -f  "CCatClient.mk" clean
	@cd "CodeLite/CCatClientTest" && "$(MAKE)" -f  "CCatClientTest.mk" clean
