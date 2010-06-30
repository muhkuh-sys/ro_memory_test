# -*- coding: utf-8 -*-

# TODO:
#  * more than one system include dir (for arg parsing etc)
#  * set a default location for the compiler dir (but where could this be?)
#
# Done:
#  * print help even if the site dir for the compiler stuff is missing.
#  * generate relocated sources from list of real locations
#  * set compiler flags from build properties
#  * show command line summary (e.g. "Cc blah.o") for the actions, but show complete line if the command failed.
#  * accept 'clean' target like make
#  * auto dependency for LDFILE
#

import scons_common
import bootblock
import build_properties
import gen_random_seq
import uuencode

build_properties.Read()

#----------------------------------------------------------------------------
#
# set help text
#
Help("""
	Set the compiler dir with all the gcc_*.py files with --site-dir=path .
	Example: The compiler dir is usually in the folder '.ant' in the users home directory.
	This expands for the user 'brunhild' to '/home/brunhild/.ant'.

	Type: 'scons --site-dir=compiler_dir' to build the production program,
	      'scons --site-dir=compiler_dir clean' to clean everything.
""")

build_properties.GenerateHelp()


#----------------------------------------------------------------------------
# This is the list of sources. The elements must be separated with whitespace
# (i.e. spaces, tabs, newlines). The amount of whitespace does not matter.
xiptest_sources = """
	src/i2c_hifsta.c
	src/init_netx_test.S
	src/main.c
	src/systime.c
	src/uart.c
	src/uprintf.c
"""

xiptest_sources_nx10 = """
	src/netx10/netx10_io_areas.c
"""


xiptest_sources_nx50 = """
	src/netx50/netx50_io_areas.c
"""


xiptest_sources_nx500 = """
	src/netx500/netx500_io_areas.c
"""


default_ccflags = """
	-ffreestanding
	-mlong-calls
	-Wall
	-Wextra
	-Wconversion
	-Wshadow
	-Wcast-qual
	-Wwrite-strings
	-Wcast-align
	-Wpointer-arith
	-Wmissing-prototypes
	-Wstrict-prototypes
	-mapcs
	-g3
	-gdwarf-2
	-std=c99
	-pedantic
"""

#----------------------------------------------------------------------------
# Only execute this part if the help text is not requested.
# This keeps the help message functional even if no include path for the
# compiler definitions was specified.
if not GetOption('help'):
	# Show summary of the build properties.
	build_properties.PrintSummary()
	
	
	#----------------------------------------------------------------------------
	#
	# import the compiler definitions
	#
	# NOTE: it would be possible to use execfile instead of import here. This
	gcc_arm_elf_4_3_3_3 = scons_common.get_compiler('gcc_arm_elf_4_3_3_3')
	
	
	#----------------------------------------------------------------------------
	#
	# create the default environment
	#
	env_default = Environment()
	gcc_arm_elf_4_3_3_3.ApplyToEnv(env_default)
	env_default.Decider('timestamp-newer')
	env_default.Append(CPPPATH = ['src'])
	env_default.Replace(CCFLAGS = Split(default_ccflags))
	env_default.Replace(LIBS = ['m', 'c', 'gcc'])
	env_default.Replace(LINKFLAGS = ['-nostdlib', '-static', '-Map=${TARGET}.map'])
	
	bootblock.ApplyToEnv(env_default)
	build_properties.ApplyToEnv(env_default)
	gen_random_seq.ApplyToEnv(env_default)
	uuencode.ApplyToEnv(env_default)

	
	env_netx10 = env_default.Clone()
	env_netx10.Append(CCFLAGS = ['-mcpu=arm966e-s'])
	env_netx10.Replace(LIBPATH = ['${GCC_DIR}/arm-elf/lib/arm966e-s', '${GCC_DIR}/lib/gcc/arm-elf/${GCC_VERSION}/arm966e-s'])
	env_netx10.Append(CPPDEFINES = [['ASIC_TYP', '10']])
	env_netx10.Append(CPPPATH = ['src/netx10'])
	
	env_netx50 = env_default.Clone()
	env_netx50.Append(CCFLAGS = ['-mcpu=arm966e-s'])
	env_netx50.Replace(LIBPATH = ['${GCC_DIR}/arm-elf/lib/arm966e-s', '${GCC_DIR}/lib/gcc/arm-elf/${GCC_VERSION}/arm966e-s'])
	env_netx50.Append(CPPDEFINES = [['ASIC_TYP', '50']])
	env_netx50.Append(CPPPATH = ['src/netx50'])
	env_netx50.VariantDir('targets/netx50', 'src', duplicate=0)
	
	env_netx500 = env_default.Clone()
	env_netx500.Append(CCFLAGS = ['-mcpu=arm926ej-s'])
	env_netx500.Replace(LIBPATH = ['${GCC_DIR}/arm-elf/lib/arm926ej-s', '${GCC_DIR}/lib/gcc/arm-elf/${GCC_VERSION}/arm926ej-s'])
	env_netx500.Append(CPPDEFINES = [['ASIC_TYP', '500']])
	env_netx500.Append(CPPPATH = ['src/netx500'])
	
	
	#----------------------------------------------------------------------------
	#
	# build the files
	#
	nx10_sqixip_env = env_netx10.Clone()
	nx10_sqixip_prnbin = nx10_sqixip_env.Prn('targets/nx10_sqixip/prn.bin', None, PRN_SIZE=0x00080000)
	nx10_sqixip_prnobj = nx10_sqixip_env.Command('targets/nx10_sqixip/prn.o', nx10_sqixip_prnbin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_nx10_sqixip_prn_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_nx10_sqixip_prn_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
	nx10_sqixip_env.VariantDir('targets/nx10_sqixip', 'src', duplicate=0)
	nx10_sqixip_src  = [src.replace('src', 'targets/nx10_sqixip')  for src in Split(xiptest_sources+xiptest_sources_nx10)]
	nx10_sqixip_env.Replace(LDFILE = 'src/netx10/netx10_sqixip2intram.ld')
	nx10_sqixip_elf = nx10_sqixip_env.Elf('targets/nx10_sqixip', nx10_sqixip_src + nx10_sqixip_prnobj)
	nx10_sqixip_img = nx10_sqixip_env.BootBlock('targets/nx10_sqixip.img', nx10_sqixip_elf, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))
	
	
	nx10_intram_env = env_netx10.Clone()
	nx10_intram_prnbin = nx10_intram_env.Prn('targets/nx10_intram/prn.bin', None, PRN_SIZE=0x00002000)
	nx10_intram_prnobj = nx10_intram_env.Command('targets/nx10_intram/prn.o', nx10_intram_prnbin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_nx10_intram_prn_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_nx10_intram_prn_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
	nx10_intram_env.VariantDir('targets/nx10_intram', 'src', duplicate=0)
	nx10_intram_src  = [src.replace('src', 'targets/nx10_intram')  for src in Split(xiptest_sources+xiptest_sources_nx10)]
	nx10_intram_env.Replace(LDFILE = 'src/netx10/netx10.ld')
	nx10_intram_elf = nx10_intram_env.Elf('targets/nx10_intram', nx10_intram_src + nx10_intram_prnobj)
	nx10_intram_img = nx10_intram_env.BootBlock('targets/nx10_intram.img', nx10_intram_elf, BOOTBLOCK_SRC=dict({}), BOOTBLOCK_DST=dict({}))
	
	
	
	
	prn_bin_nx50 = env_netx50.Prn('targets/netx50/test.bin', None)
	prn_obj_nx50 = env_netx50.Command('targets/netx50/test.o', prn_bin_nx50, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx50_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx50_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
	all_sources_nx50  = [src.replace('src', 'targets/netx50')  for src in Split(xiptest_sources+xiptest_sources_nx50)]
	env_netx50.Replace(LDFILE = 'src/netx50/netx50_pflxip2intram.ld')
	xiptest_nx50_elf = env_netx50.Elf('targets/xiptest_netx50', all_sources_nx50 + prn_obj_nx50)
	
	
	nx500pfl_env = env_netx500.Clone()
	nx500pfl_env.Replace(LDFILE = 'src/netx500/netx500_pflxip2intram.ld')
	nx500pfl_env.Replace(PRN_SIZE = 0x00080000)
	nx500pfl_prn_bin = nx500pfl_env.Prn('targets/netx500/nx500pfl_test.bin', None)
	nx500pfl_prn_obj = nx500pfl_env.Command('targets/netx500/nx500pfl_test.o', nx500pfl_prn_bin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
	nx500pfl_env.VariantDir('targets/netx500/pfl', 'src', duplicate=0)
	nx500pfl_all_sources  = [src.replace('src', 'targets/netx500/pfl')  for src in Split(xiptest_sources+xiptest_sources_nx500)]
	nx500pfl_elf = nx500pfl_env.Elf('targets/nx500_pfl', nx500pfl_all_sources + nx500pfl_prn_obj)
	nx500pfl_js28f256j3_img = nx500pfl_env.BootBlock('targets/nx500_pfl_js28f256j3.img', nx500pfl_elf, BOOTBLOCK_SRC=dict({0x00:0xf8beaf16, 0x01:0x0100010c}), BOOTBLOCK_DST=dict({0x07:0x0100010c, 0x0e:0x00000001}))
	
	nx500intram_env = env_netx500.Clone()
	nx500intram_env.Replace(LDFILE = 'src/netx500/netx500_intram.ld')
	nx500intram_env.Replace(PRN_SIZE = 0x00001000)
	nx500intram_prn_bin = nx500intram_env.Prn('targets/netx500/nx500intram_test.bin', None)
	nx500intram_prn_obj = nx500intram_env.Command('targets/netx500/nx500intram_test.o', nx500intram_prn_bin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx500_nx500intram_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx500_nx500intram_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
	nx500intram_env.VariantDir('targets/netx500/intram', 'src', duplicate=0)
	nx500intram_all_sources  = [src.replace('src', 'targets/netx500/intram')  for src in Split(xiptest_sources+xiptest_sources_nx500)]
	nx500intram_elf = nx500intram_env.Elf('targets/nx500_intram', nx500intram_all_sources + nx500intram_prn_obj)
	
