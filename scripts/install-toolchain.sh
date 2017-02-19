#!/bin/sh

# immediately exit on any unhandled error
set -e

# current working directory
cwd=${0%/*}

. $cwd/lib/sh/utils.sh

. $cwd/config.sh

#
# Print the help message
#
cmd_help() {
  cat <<EOF
Usage: $0 [--help]

Download and install the official ARM toolchain \`arm-none-eabi\`.

OPTIONS
  --prefix=$cfg_arm_toolchain_path_default
  Toolchain installation path where the tarball is unpacked.
EOF
}

#
# script entry function
#
cmd() {
  while [ $# -gt 0 ]; do
    case $1 in
      -h|--help)
        cmd_help
        return 1 # note: doing this makes only one --help message possible
        ;;

      --prefix*)
        getopt_longopt_arg arg shift "$1" "$2"
        opt_toolchain_path="$arg"
        shift $shift
        ;;

      -*)
        log_error  "$0: illegal option −− $1"
        return 1
        ;;

      --)
        shift
        ;;

      *)
        [ -n "$dev" ] && log_info "warning: ignoring \`$dev\`"
        dev="$1"
        shift
        ;;
    esac
  done

  if [ -z "$opt_toolchain_path" ]; then
    opt_toolchain_path="$cfg_arm_toolchain_path_default"
    log_info "Using default installation path \`$opt_toolchain_path\`"
  fi

  log_info "Installing ARM cross-compiler into \`$opt_toolchain_path\`"
  mkdir -p $opt_toolchain_path
  curl -sSL $cfg_arm_toolchain_url | tar -C $opt_toolchain_path -xj --strip-components=1
  $opt_toolchain_path/bin/arm-none-eabi-gcc -v 1>&- 2>&-
  $opt_toolchain_path/bin/arm-none-eabi-gdb -v 1>&- 2>&-
  log_info "The toolchain has been successfully installed."

  return 0
}

cmd $@
