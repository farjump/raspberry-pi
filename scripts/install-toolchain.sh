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

Download and install the official ARM toolchain `arm-none-eabi` into
`/usr/local`.

Note that it will overwrite an existing `arm-none-eabi` already
installed into `/usr/local`.
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

  log_info "Installing ARM cross-compiler"
  curl -sSL $cfg_arm_toolchain_url | tar -v -C /usr/local/ -xj --strip-components=1
  arm-none-eabi-gcc -v 1>&- 2>&-
  arm-none-eabi-gdb -v 1>&- 2>&-
  log_info "The toolchain has been successfully installed."

  return 0
}

cmd $@
