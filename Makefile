CC=gcc
CFLAGS=-shared -fPIC

PLUGIN_DIR=target/lv2

midijoint:
	mkdir -p ${PLUGIN_DIR}/$@.lv2
	${CC} ${CFLAGS} -o ${PLUGIN_DIR}/$@.lv2/$@.so src/$@.c
	cp rdf/* ${PLUGIN_DIR}/$@.lv2

install:
	mkdir -p ${HOME}/.lv2
	ln -s ${PWD}/${PLUGIN_DIR}/midijoint.lv2 ${HOME}/.lv2

uninstall:
	rm ${HOME}/.lv2/midijoint.lv2

clean:
	rm -rf target
