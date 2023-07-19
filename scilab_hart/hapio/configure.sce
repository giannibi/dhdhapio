mode(-1);
lines(0);

TOOLBOX_NAME = 'hapio';
TOOLBOX_TITLE = 'hapio module for Hart Toolbox';
// ====================================================================
buildable='yes'; //change to no, if the module should not be build


// Add the names of the c - funktions to names. E.g.: names=['rt_file1','rt_file2'];
names =   ['rt_hapio_device']; 

// Add the c file names which should be compiled to files. Replace the ending with .o. E.g.: files = ['file1.o';'file2.o'];
files     = ['hart_hapio.c';'hart_hapio_device.cpp'];

// Add external shared librarys to ldflags
// E.g.: ldflags = " -lspezial_lib"; 
ldflags = ' -ldhd -lrt -lusb-1.0 -lpci -lz -lpthread'
