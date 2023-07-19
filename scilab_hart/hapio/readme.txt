README file for the toolbox hapio

Please update this file to provide information about your module:
 * what it is doing
 * the author
 * a few word about the license

Add interface functions (.sci files) in the directory hapio/macros
by using the command (you have to load the hart toolbox before)
   hart_edit_scicosblock()

Document the source code using the templates provided by help_from_scicosblock.
See help help_from_scicosblock for more information.
Create .xml help files using the command (run from directory hapio)
   help_from_scicosblock('macros','help/en_US')

Add computational c/c++ functions tohapio/src/c

Update configure.sce
Build the module using the command:
   chdir(../../) // go to the hart toolbox base directory
   exec prepare_toolbox.sce
   exec builder.sce
Finally load the toolbox using the command:
   exec loader.sce
