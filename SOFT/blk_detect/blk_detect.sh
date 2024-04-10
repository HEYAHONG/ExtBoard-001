#!/bin/bash

function CheckTool
{
        [  -n "$1"  ] ||
        {
                echo -e  "CheckTool 参数错误!!";
                return 255;
        };
        ToolPath=`which $1`;
        [ -e "$ToolPath" ] ||
        {
                 echo -e "$1 不存在，请先安装此工具!!!";
                 return 255;
        };
        return 0;
}

CheckTool find
[ $? -eq 0 ] || exit
CheckTool xargs
[ $? -eq 0 ] || exit
CheckTool dirname
[ $? -eq 0 ] || exit
CheckTool cat
[ $? -eq 0 ] || exit
CheckTool readlink
[ $? -eq 0 ] || exit

#判断sysfs
SYSFS_BLOCK="/sys/class/block/"
[ -d "${SYSFS_BLOCK}" ] || SYSFS_BLOCK="/sys/block"
[ -d "${SYSFS_BLOCK}" ] || exit


for i in `find /sys/block/ |xargs realpath`
do
       DIR=`dirname "$i"`
       DEVNAME=`realpath --relative-to="${DIR}" "${i}"`
       if [ -b "/dev/${DEVNAME}" ]
       then
                while [ "/" != "${DIR}" ]
                do
                        if [ -e "${DIR}/manufacturer" ] && [ -e "${DIR}/product" ]
                        then
                                if [ "HYH" == `cat "${DIR}/manufacturer"` ] && [ "ExtBoard-001" == `cat "${DIR}/product"` ]
                                then
                                        echo "Found ExtBoard-001 DAPLink Block /dev/${DEVNAME}"
                                fi
                                DIR="/"
                        fi
                        DIR=`dirname ${DIR}`
                done
       fi
done

