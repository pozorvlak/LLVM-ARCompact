# Create the llvm folder.
rm -rf gcc_benchmarks
mkdir gcc_benchmarks
cd gcc_benchmarks

# Run configure.
../configure --with-dspstone --with-cc="/afs/inf.ed.ac.uk/group/project/arc_tools/arc_gcc_2.3/arc-elf32/bin/arc-elf32-gcc -O3 -mARC700 -v" --with-collect="../collect.sh" --with-sim="/afs/inf.ed.ac.uk/group/project/arc_tools/arcsim/bin/arcsim -tcvX --emt"

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
