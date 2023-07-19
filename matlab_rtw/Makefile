DHDINCLUDES=-I/usr/local/include/  

DHDLIBPATHS= -L/usr/local/lib/

MACLIBS= -lpthread -ldhd

LINUXLIB = -lpthread -lrt -lpci -lz -ldhd -lusb-1.0

MACLDFLAGS = -arch x86_64 -Wl,-syslibroot,/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk -mmacosx-version-min=10.7 -bundle -Wl,-exported_symbols_list,/Applications/MATLAB_R2013a.app/extern/lib/maci64/mexFunction.map

MACFRAMEWORKS = -framework CoreFoundation -framework IOKit 

SOURCE=dhdhapio.cpp

MEX=dhdhapio

OS = $(shell uname)

$(MEX): $(SOURCE)
ifeq ($(OS),Darwin)
	mex -v $(DHDINCLUDES) $(DHDLIBPATHS) $(FLAGS) $(SOURCE) $(MACLIBS) LDFLAGS='$(MACLDFLAGS) $(MACFRAMEWORKS)'
else
	mex -v $(DHDINCLUDES) $(DHDLIBPATHS) $(FLAGS) $(SOURCE) $(LINUXLIB) 
endif

clean:
	rm -f *.o *.mex*

