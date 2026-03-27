PYTHON = python3

all: cliente portal servidor

cliente: cliente.py
	echo '#!/usr/bin/env $(PYTHON)' > cliente
	cat $< >> cliente
	chmod +x cliente

portal: portal.py
	echo '#!/usr/bin/env $(PYTHON)' > portal
	cat $< >> portal
	chmod +x portal

servidor: servidor.py
	echo '#!/usr/bin/env $(PYTHON)' > servidor
	cat $< >> servidor
	chmod +x servidor

clean:
	rm -f cliente portal servidor

.PHONY: all clean

