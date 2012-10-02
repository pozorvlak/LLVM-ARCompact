rm -rf obj
mkdir obj

rm libgcc_extra.a

/afs/inf.ed.ac.uk/group/project/arc_tools/arc_gcc_2.3/arc-elf32/bin/arc-elf32-gcc -c src/ledf2.c -o obj/ledf2.o
/afs/inf.ed.ac.uk/group/project/arc_tools/arc_gcc_2.3/arc-elf32/bin/arc-elf32-gcc -c src/unorddf2.c -o obj/unorddf2.o
/afs/inf.ed.ac.uk/group/project/arc_tools/arc_gcc_2.3/arc-elf32/bin/arc-elf32-gcc -c src/eqdf2.c -o obj/eqdf2.o

/afs/inf.ed.ac.uk/group/project/arc_tools/arc_gcc_2.3/arc-elf32/bin/arc-elf32-ar rcs libgcc_extra.a obj/ledf2.o obj/unorddf2.o obj/eqdf2.o
