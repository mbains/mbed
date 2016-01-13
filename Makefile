all:
	##python workspace_tools/build.py -m NUCLEO_F042K6 -t GCC_ARM  -v -o "debug-info"
	python workspace_tools/make.py -m NUCLEO_F042K6 -t GCC_ARM -p 27 -v -o "debug-info"
	python workspace_tools/make.py -m NUCLEO_F042K6 -t GCC_ARM -p 28 -v -o "debug-info"
clean:
	echo


