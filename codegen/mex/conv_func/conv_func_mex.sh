MATLAB="/usr/local/MATLAB/R2016a"
Arch=glnxa64
ENTRYPOINT=mexFunction
MAPFILE=$ENTRYPOINT'.map'
PREFDIR="/home/pold/.matlab/R2016a"
OPTSFILE_NAME="./setEnv.sh"
. $OPTSFILE_NAME
COMPILER=$CC
. $OPTSFILE_NAME
echo "# Make settings for conv_func" > conv_func_mex.mki
echo "CC=$CC" >> conv_func_mex.mki
echo "CFLAGS=$CFLAGS" >> conv_func_mex.mki
echo "CLIBS=$CLIBS" >> conv_func_mex.mki
echo "COPTIMFLAGS=$COPTIMFLAGS" >> conv_func_mex.mki
echo "CDEBUGFLAGS=$CDEBUGFLAGS" >> conv_func_mex.mki
echo "CXX=$CXX" >> conv_func_mex.mki
echo "CXXFLAGS=$CXXFLAGS" >> conv_func_mex.mki
echo "CXXLIBS=$CXXLIBS" >> conv_func_mex.mki
echo "CXXOPTIMFLAGS=$CXXOPTIMFLAGS" >> conv_func_mex.mki
echo "CXXDEBUGFLAGS=$CXXDEBUGFLAGS" >> conv_func_mex.mki
echo "LD=$LD" >> conv_func_mex.mki
echo "LDFLAGS=$LDFLAGS" >> conv_func_mex.mki
echo "LDOPTIMFLAGS=$LDOPTIMFLAGS" >> conv_func_mex.mki
echo "LDDEBUGFLAGS=$LDDEBUGFLAGS" >> conv_func_mex.mki
echo "Arch=$Arch" >> conv_func_mex.mki
echo OMPFLAGS= >> conv_func_mex.mki
echo OMPLINKFLAGS= >> conv_func_mex.mki
echo "EMC_COMPILER=gcc" >> conv_func_mex.mki
echo "EMC_CONFIG=optim" >> conv_func_mex.mki
"/usr/local/MATLAB/R2016a/bin/glnxa64/gmake" -B -f conv_func_mex.mk
