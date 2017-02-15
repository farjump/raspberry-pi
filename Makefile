.PHONY: shell
shell:
	docker build --build-arg uid=$$(id -u) .
	docker run -it -v $$PWD:$$PWD -w $$PWD --privileged $$(docker build -q --build-arg uid=$$(id -u) .)
