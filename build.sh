#! /bin/bash

project=fgm842d


if [ ! -d ./ql_tools/gcc-arm-none-eabi-5_4-2016q3 ];then 
   tar -xvf ./ql_tools/gcc-arm-none-eabi-5_4-2016q3.tar.bz2 -C ./ql_tools
fi

if [ ! -d ./ql_tools/cmake-3.25.1-linux-x86_64 ];then 
   tar -xf ./ql_tools/cmake-3.25.1-linux-x86_64.tar.gz -C ./ql_tools
fi



export PATH=$(pwd)/ql_tools/gcc-arm-none-eabi-5_4-2016q3/bin/:$(pwd)/ql_tools/cmake-3.25.1-linux-x86_64/bin/:$PATH

if [ ! -d "ql_out" ]; then
    mkdir ql_out
fi
#export KCONFIG_CONFIG=$(pwd)/ql_build/.config
#export KCONFIG_AUTOHEADER=$(pwd)/ql_build/config.h

#if [ ! -e ${KCONFIG_CONFIG} ];
#then 
#    cp $(pwd)/ql_build/defconfig ${KCONFIG_CONFIG}
#fi

#if [ "${if_ubuntu20_env}" != "" ]; # for Ubuntu20 and Ubuntu22
#then
#   $(pwd)/ql_tools/kconfig_tool/menuconfig_ubuntu20_22 ./Kconfig 
#else  # for Ubuntu16 and Ubuntu18
#   $(pwd)/ql_tools/kconfig_tool/menuconfig_ubuntu16_18 ./Kconfig
#fi

#$(pwd)/ql_tools/kconfig_tool/genconfig --config-out=${KCONFIG_CONFIG} --header-path=${KCONFIG_AUTOHEADER}
cd ql_build

if [ ! -d "gccout" ]; then
    mkdir gccout
else
    rm -rf gccout
    mkdir gccout
fi

echo "Enter gccout dir ......"
cd gccout

cmake ../..

make

cd ../..

cp $(pwd)/ql_build/gccout/${project}.map $(pwd)/ql_out/${project}.map
cp $(pwd)/ql_build/gccout/${project}.elf $(pwd)/ql_out/${project}.elf

#ota firmware package
current_datetime=$(date +"%Y-%m-%d %H:%M:%S")
if [ -f "$(pwd)/ql_build/gccout/${project}.bin" ];
then
   cp $(pwd)/ql_build/gccout/${project}.bin $(pwd)/ql_tools/rtt_ota/
   cd $(pwd)/ql_tools/rtt_ota/
   ./rt_ota_packaging_tool_cli -f ${project}.bin -v ${project}${current_datetime} -p app  -c gzip -s aes -i 0123456789ABCDEF -k 0123456789ABCDEF0123456789ABCDEF
   mv ${project}.rbl ../../ql_out/
   cd -
else
   echo "ota file generation failed!!!"  
   exit
fi
#cd ql_tools/beken_packager
#python ocpu_bk_packager.py


