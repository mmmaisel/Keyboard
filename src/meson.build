project('Keyboard', ['c', 'cpp'], default_options: ['buildtype=minsize'])
target = 'Keyboard'

srcs = [
  'main.cpp',
  'inttable.c',
  'led_matrix.cpp',
  'led_layout.cpp',
  'key_matrix.cpp',
  'key_layout.cpp',
  'hid_keyboard_endpoint.cpp',
  'modular_keyboard.cpp',
  'module.cpp',
  'startup.cpp',
  'system.cpp',
  'uart.cpp',
  'usb_control_endpoint.cpp',
  'usb_descriptor.cpp',
  'usb_endpoint.cpp',
  'usb_phy.cpp'
]

openocd_conf = '@0@/@1@'.format(meson.current_source_dir(), 'st_nucleo_f401re.cfg')
linker_conf = '@0@/@1@'.format(meson.current_source_dir(), 'memSections.ld')

link_args = ['-T@0@'.format(linker_conf), f'-Wl,-Map=@target@.map,--cref']

main = executable(
    target,
    srcs,
    name_suffix: 'elf',
    link_args: link_args
)

objdump = find_program('objdump')
nm = find_program('nm')
size = find_program('size')

mainlss = custom_target(
  'lss',
  capture: true,
  output: f'@target@.lss',
  build_by_default: true,
  command: [objdump, '-h', '-S', '-t', f'@target@.elf'],
  depends: [main]
)

mainsym = custom_target(
  'sym',
  capture: true,
  output: f'@target@.sym',
  build_by_default: true,
  command: [nm, '-n', f'@target@.elf'],
  depends: [main]
)

mainsize = custom_target(
  'size',
  build_by_default: true,
  output: f'@target@.size',
  command: [size, '--format=berkeley', f'@target@.elf'],
  depends: [main]
)

run_target(
  'debug',
  command : [
    'bash',
    '-ic',
    f'if ! pgrep -f "^openocd -f @openocd_conf@ -c init"; then\n' +
      f'openocd -f @openocd_conf@ -c "init; reset init"&\n' +
    'fi\n' +
    f'arm-none-eabi-gdb -ex "target remote localhost:3333" "@target@.elf"\n' +
    f'pkill -f "^openocd -f @openocd_conf@ -c init" ||:'
  ]
)

run_target(
  'flash',
  command : [
    'bash',
    '-ic',
    f'pkill -f "^openocd -f @openocd_conf@ -c init" ||:\n' +
    f'openocd -f @openocd_conf@ -c "program @target@.elf verify reset exit"'
  ]
)
