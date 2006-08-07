#! /bin/sh
#
# $Id$
#
# RTEMS Build Executable Installers script.
#
# This script takes the RPM files built using the crossrpms/build-rpms.sh
# script.
#

terminate()
{
  echo "error: $*" >&2
  exit 1
}

check()
{
 if [ $? -ne 0 ]; then
  terminate
 fi
}

version=4.7
tool_build=1

target_list="i386 m68k powerpc sparc arm mips"

mingw32_cpu_list="i686"

rpm_topdir=$(rpm --eval "%{_topdir}")

common_label="common"
local_rpm_database=yes
targets=$target_list
run_prefix=
relocation=

source=$(dirname $0)

if [ "$source" = "." ]; then
 source=$(pwd)
fi

while [ $# -gt 0 ];
do
 case $1 in
  -d)
   set -x
   ;;
  -l)
   shift
   rpm_prefix=$1-
   rpm_prefix_arg="--enable-rpmprefix=$rpm_label"
   ;;
  -n)
   run_prefix="echo "
   ;;
  -p)
   shift
   prefix=$1
   ;;
  -R)
   local_rpm_database=no
   ;;
  -r)
   shift
   relocation=$1
   ;;
  -t)
   shift
   targets=$1
   ;;
  -v)
   shift
   version=$1
   ;;
  --help)
   echo "$0 -??"
   exit 2
   ;;
  *)
   terminate "error: invalid option (try --help): $1"
   ;;
 esac
 shift
done

cd=${run_prefix}cd
cp=${run_prefix}cp
make=${run_prefix}make
makensis=${run_prefix}makensis
mkdir=${run_prefix}mkdir
rm=${run_prefix}rm
rpmbuild=${run_prefix}rpmbuild
rpm=${run_prefix}rpm

if [ -z "$relocation" ]; then
 terminate "error: a relocation path needs to be specified"
fi

if [ $local_rpm_database = yes ]; then
  rpm_database="--dbpath $prefix/var/lib/rpm"
else
  rpm_database=
fi

get_rpm_list()
{
  if [ -d $rpm_topdir/mingw32/RPMS/$1 ]; then
   echo $(ls $rpm_topdir/mingw32/RPMS/$1/*.rpm | grep -v "debuginfo" | grep $2)
  fi
}

#
# Handle each type of host processor.
#
for p in $mingw32_cpu_list
do
 common_rpms=$(get_rpm_list $p $common_label)
 check "getting the common RPM list"

 rpm_options="--ignoreos --force --nodeps --noorder "

 for t in $targets
 do
  rpms=$(get_rpm_list $p $t)
  check "getting the RPM list"
  if [ -n "$rpms" ]; then
   echo "Clean the relocation directory"
   $rm -rf $relocation
   check "removing the relocation directory: $relocation"

   for r in $common_rpms $rpms
   do
    echo "rpm $rpm_database --relocate $prefix=$relocation $rpm_options -i $r"
    $rpm $rpm_database --relocate $prefix=$relocation $rpm_options -i $r
    check "installing rpm: $r"
   done

   files=$(find $relocation -type f | sed -e "s/^$(echo ${relocation} | sed 's/\//\\\//g')//" -e "s/^\///" | sort)
   check "find the file list"

   of=$relocation/rtems-files.nsi

   echo "!macro RTEMS_INSTALL_FILES" > $of
   echo " !ifndef EMPTY_INSTALLER" >> $of

   install_dir=

   for f in $files
   do
    d=$(dirname $f)
    if [ "$install_dir" != "$d" ]; then
     id=$(echo $d | sed -e 's/\//\\/g' -e 's/\/$//')
     echo "  SetOutPath \"\$INSTDIR\\$id\"" >> $of
     install_dir=$d
    fi
    echo "  File \"$relocation/$f\"" >> $of
   done

   echo " !endif" >> $of
   echo "!macroend" >> $of

   echo "!macro RTEMS_DELETE_FILES" >> $of
   echo " !ifndef EMPTY_INSTALLER" >> $of

   remove_dirs=
   remove_dir=

   for f in $files
   do
    d=$(dirname $f)
    if [ "$remove_dir" != "$d" ]; then
     remove_dirs="$remove_dirs $d"
     remove_dir=$d
    fi
    rf=$(echo $f | sed -e 's/\//\\/g' -e 's/\/$//')
    echo "  Delete \"\$INSTDIR\\$rf\"" >> $of
   done

   remove_dirs=$(for r in $remove_dirs; do echo $r; done | sort -r -u)

   for d in $remove_dirs
   do
     if [ "$d" = "." ]; then
      d=
     fi 
     rd=$(echo $d | sed -e 's/\//\\/g' -e 's/\/$//')
     echo "  RMDir \"\$INSTDIR\\$rd\"" >> $of
   done

   echo " !endif" >> $of
   echo "!macroend" >> $of

   rtems_binary=$rpm_topdir/mingw32/exe/$p
   echo "mkdir -p $rtems_binary"
   $mkdir -p $rtems_binary
   check "make the RTEMS binary install point: $rtems_binary"

   of=$relocation/rtems.nsi
   echo "!define RTEMS_TARGET \"$t\"" > $of
   echo "!define RTEMS_VERSION \"$version\"" >> $of
   echo "!define RTEMS_BUILD_VERSION \"$tool_build\"" >> $of
   echo "!define RTEMS_PREFIX \"rtems-tools\"" >> $of
   echo "!define RTEMS_SOURCE \"$source\"" >> $of
   echo "!define RTEMS_RELOCATION \"$relocation\"" >> $of
   echo "!define RTEMS_LOGO \"$source/rtems_logo.bmp\"" >> $of
   echo "!define RTEMS_BINARY \"$rtems_binary\"" >> $of
   echo "!define RTEMS_LICENSE_FILE \"$source/rtems-license.rtf\"" >> $of
   echo "!include \"$relocation/rtems-files.nsi\"" >> $of
   echo "!include \"$source/rtems-tools.nsi\"" >> $of

   echo "cp $source/rtems.ini $relocation/rtems.ini"
   $cp $source/rtems.ini $relocation/rtems.ini
   check "coping the dialog definition file: $relocation/rtems.ini"

   echo "makensis $of"
   $makensis $of
   check "making the installer: $of"

  fi
 done
done
