copy ..\baconloader\BACONLDR.BIN .
..\x64\release\msx_bacon.exe -O3 test.bas test.asm
..\zma.exe test.asm test.bin
if exist test2.bas (
	if exist zma_test.log del zma_test.log
	ren zma.log zma_test.log
	if exist zma_test.sym del zma_test.sym
	ren zma.sym zma_test.sym
	..\x64\release\msx_bacon.exe -O3 test2.bas test2.asm
	..\zma.exe test2.asm test2.bin
)
pause
