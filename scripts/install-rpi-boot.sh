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
Usage: $0 [--help] <sdcard-block-device>

Download and install into the block device the Raspberry Pi's firmware
& bootloader, and Farjump's Alpha. The block device must
contain a FAT file-system.

Note that the block device is mounted and finally unmounted during the
execution of this scripts, which requires root privileges.

EXAMPLE
  \$ mkfs.vfat -F 32 -n RPI /dev/sdX # /!\ erases everything in sdX
  \$ $0 /dev/sdX
EOF
}

#
# Mount the SD card block device `$1` into `$cfg_sdcard_mountpoint`.
#
cmd_mount() {
  dev="$1"
  log_info "Temporarily mounting \`$dev\` into \`$cfg_sdcard_mountpoint\`"
  mkdir -p $cfg_sdcard_mountpoint
  sudo mount -t vfat -o rw,umask=0000 "$dev" $cfg_sdcard_mountpoint
}

#
# Unmount `$cfg_sdcard_mountpoint`
#
cmd_unmount() {
  log_info "Un-mounting \`$cfg_sdcard_mountpoint\`"
  sync $cfg_sdcard_mountpoint/*
  sudo umount $cfg_sdcard_mountpoint
}

#
# Download official firmwares from the RPi repo
#
cmd_download_files() {
  dst="$1"
  if ! cmd_check_fw_sha256 $dst --quiet 1>&- 2>&-; then
      log_info "Downloading the Raspberry Pi's firmware version $cfg_rpi_fw_version"
      (
          cd $dst
          curl -fSL --remote-name-all --progress-bar $cfg_rpi_fw_baseurl/start.elf $cfg_rpi_fw_baseurl/bootcode.bin
      )
  fi
}

#
# Copy downloaded files into the mounted SD card and check the file integrity.
#
cmd_copy_files() {
  src=$1
  log_info "Installing the RPi firmware and the Alpha debugger"
  cp -fv $src/bootcode.bin $src/start.elf $src/Alpha.bin $src/config.txt $cfg_sdcard_mountpoint
  log_info "Checking the integrity"
  cmd_check_fw_sha256 $cfg_sdcard_mountpoint
  echo "$cfg_alpha_sha256  $cfg_sdcard_mountpoint/Alpha.bin" | sha256sum -c -
}

#
# Check the sha256sum of the firmware stored in $1.
# Optionally pass an extra sha256sum argument as $2.
#
cmd_check_fw_sha256() {
  ( echo "$cfg_bootcode_download_sha256  $1/bootcode.bin" | sha256sum -c $2 - ) || return 1
  echo "$cfg_start_download_sha256  $1/start.elf" | sha256sum -c $2 -
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

  if [ -z "$dev" ]; then
    log_error " missing block device argument"
    return 1
  fi

  src='boot'
  cmd_download_files $src
  cmd_mount "$dev"
  trap 'cmd_unmount "$dev"' INT QUIT KILL ABRT
  if ! cmd_copy_files $src; then
    cmd_unmount "$dev"
    return 1
  fi
  cmd_unmount "$dev"
  log_info 'Your SD card is ready!'
  log_info "You can now insert it into the RPi and use Alpha through the RPI's Mini-UART"
  return 0
}

cmd $@
