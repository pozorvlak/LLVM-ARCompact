# Create the llvm folder.
rm -rf llvm_benchmarks
mkdir llvm_benchmarks
cd llvm_benchmarks

# Run configure.
../configure --with-dspstone --with-cc="/afs/inf.ed.ac.uk/user/s08/s0840449/Documents/Coursework/Project/LLVM-ARCompact/build/Release+Asserts/bin/clang -O3 -v -debug -ccc-host-triple arcompact-generic-generic -I/afs/inf.ed.ac.uk/group/project/arc_tools/arc_gcc_2.3/arc-elf32/arc-elf32/include -I/afs/inf.ed.ac.uk/group/project/arc_tools/arc_gcc_2.3/arc-elf32/lib/gcc/arc-elf32/4.2.1/include -I/afs/inf.ed.ac.uk/user/s08/s0840449/Documents/Coursework/Project/LLVM-ARCompact/arclibs/libgcc_extra.a" --with-collect="../collect.sh" --with-sim="/afs/inf.ed.ac.uk/group/project/arc_tools/arcsim/bin/arcsim-bleeding --arch /afs/inf.ed.ac.uk/group/project/arc_tools/arcsim/svn/etc/encore-bleeding.arc --isa /afs/inf.ed.ac.uk/group/project/arc_tools/arcsim/svn/etc/encore.isa -tcvX --emt"

# Build the benchmarks
echo "Building..."
make build-dspstone-fix &> build.log

# Remove the currently broken benchmarks
echo "Removing broken benchmarks..."
rm -rf "DSPstone/fft1024_13_fxp"
rm -rf "DSPstone/fft1024_7_fxp"
rm -rf "DSPstone/startup_fxp"

# Run them.
echo "Running..."
make run-dspstone-fix &> run.log

# Collect the stats
echo "Collecting..."
make collect-dspstone-fix &> collect.log
