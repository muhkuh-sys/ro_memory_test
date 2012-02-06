# -*- coding: utf-8 -*-

#----------------------------------------------------------------------------
#
# Set up the Muhkuh Build System.
#

SConscript('mbs/SConscript')
Import('env_default')

#----------------------------------------------------------------------------
# This is the list of sources. The elements must be separated with whitespace
# (i.e. spaces, tabs, newlines). The amount of whitespace does not matter.
sources_common = """
	src/i2c_hifsta.c
	src/init_netx_test.S
	src/main.c
	src/systime.c
	src/uart.c
	src/uprintf.c
"""

sources_netx10 = """
	src/netx10/netx10_io_areas.c
"""


sources_nx56 = """
"""


sources_nx50 = """
	src/netx50/netx50_io_areas.c
"""


sources_nx500 = """
	src/netx500/netx500_io_areas.c
"""


#----------------------------------------------------------------------------
#
# Create the compiler environments.
#

env_default.Append(CPPPATH = ['src', 'platform'])

env_netx500_default = env_default.CreateCompilerEnv('500', ['cpu=arm926ej-s'])
env_netx500_default.Replace(BOOTBLOCK_CHIPTYPE = 500)
env_netx500_default.Append(CPPPATH = ['src/netx500'])

env_netx56_default = env_default.CreateCompilerEnv('56', ['cpu=arm966e-s'])
env_netx56_default.Replace(BOOTBLOCK_CHIPTYPE = 56)
env_netx56_default.Append(CPPPATH = ['src/netx56'])

env_netx50_default = env_default.CreateCompilerEnv('50', ['cpu=arm966e-s'])
env_netx50_default.Replace(BOOTBLOCK_CHIPTYPE = 50)
env_netx50_default.Append(CPPPATH = ['src/netx50'])

env_netx10_default = env_default.CreateCompilerEnv('10', ['cpu=arm966e-s'])
env_netx10_default.Replace(BOOTBLOCK_CHIPTYPE = 10)
env_netx10_default.Append(CPPPATH = ['src/netx10'])


#----------------------------------------------------------------------------
#
# This is a helper function which generates a sequence of pseudo random
# numbers.
#

def prn_obj(tEnv, sizSequence, strWorkingFolder):
	strPrnBinFilename = 'targets/%s/prn.bin' % strWorkingFolder

	strCmd = '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_%s_prn_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_%s_prn_bin_end"="_binary_test_bin_end" $SOURCE $TARGET' % (strWorkingFolder, strWorkingFolder)

	tPrnBin = tEnv.Prn(strPrnBinFilename, None, PRN_SIZE=sizSequence)
	return tEnv.Command('targets/nx10_sqixip/prn.o', tPrnBin, strCmd)


#----------------------------------------------------------------------------
#
# Build all files.
#

env_netx10_sqixip = env_netx10_default.Clone()
prn_netx10_sqixip = prn_obj(env_netx10_sqixip, 0x00080000, 'netx10_sqixip')
env_netx10_sqixip.Replace(LDFILE = 'src/netx10/netx10_sqixip2intram.ld')
src_netx10_sqixip = env_netx10_sqixip.SetBuildPath('targets/netx10_sqixip', 'src', sources_common + sources_netx10)
elf_netx10_sqixip = env_netx10_sqixip.Elf('targets/netx10_sqixip.elf', src_netx10_sqixip + prn_netx10_sqixip)
bb0_netx10_sqixip = env_netx10_sqixip.BootBlock('targets/nx10_sqixip.img', elf_netx10_sqixip, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))


#	nx10_intram_env = env_netx10.Clone()
#	nx10_intram_prnbin = nx10_intram_env.Prn('targets/nx10_intram/prn.bin', None, PRN_SIZE=0x00002000)
#	nx10_intram_prnobj = nx10_intram_env.Command('targets/nx10_intram/prn.o', nx10_intram_prnbin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_nx10_intram_prn_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_nx10_intram_prn_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
#	nx10_intram_env.VariantDir('targets/nx10_intram', 'src', duplicate=0)
#	nx10_intram_src  = [src.replace('src', 'targets/nx10_intram')  for src in Split(xiptest_sources+xiptest_sources_nx10)]
#	nx10_intram_env.Replace(LDFILE = 'src/netx10/netx10.ld')
#	nx10_intram_elf = nx10_intram_env.Elf('targets/nx10_intram', nx10_intram_src + nx10_intram_prnobj)
#	nx10_intram_img = nx10_intram_env.BootBlock('targets/nx10_intram.img', nx10_intram_elf, BOOTBLOCK_SRC=dict({}), BOOTBLOCK_DST=dict({}))
	
	
#	nx10_pflxip_env = env_netx10.Clone()
#	nx10_pflxip_prnbin = nx10_pflxip_env.Prn('targets/nx10_pflxip/prn.bin', None, PRN_SIZE=0x00002000)
#	nx10_pflxip_prnobj = nx10_intram_env.Command('targets/nx10_pflxip/prn.o', nx10_pflxip_prnbin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_nx10_pflxip_prn_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_nx10_pflxip_prn_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
#	nx10_pflxip_env.VariantDir('targets/nx10_pflxip', 'src', duplicate=0)
#	nx10_pflxip_src  = [src.replace('src', 'targets/nx10_pflxip')  for src in Split(xiptest_sources+xiptest_sources_nx10)]
#	nx10_pflxip_env.Replace(LDFILE = 'src/netx10/netx10_pflxip.ld')
#	nx10_pflxip_elf = nx10_pflxip_env.Elf('targets/nx10_pflxip', nx10_pflxip_src + nx10_pflxip_prnobj)
#	nx10_pflxip_img = nx10_pflxip_env.BootBlock('targets/nx10_pflxip.img', nx10_pflxip_elf, BOOTBLOCK_SRC=dict({0x00:0xf8beaf16, 0x01:0x0103030f, 0x0c:0x00000007}), BOOTBLOCK_DST=dict({0x0e:0x00000001}))
	
	
	
#	prn_bin_nx50 = env_netx50.Prn('targets/netx50/test.bin', None)
#	prn_obj_nx50 = env_netx50.Command('targets/netx50/test.o', prn_bin_nx50, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx50_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx50_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
#	all_sources_nx50  = [src.replace('src', 'targets/netx50')  for src in Split(xiptest_sources+xiptest_sources_nx50)]
#	env_netx50.Replace(LDFILE = 'src/netx50/netx50_pflxip2intram.ld')
#	xiptest_nx50_elf = env_netx50.Elf('targets/xiptest_netx50', all_sources_nx50 + prn_obj_nx50)
	
	
#	nx500pfl_env = env_netx500.Clone()
#	nx500pfl_env.Replace(LDFILE = 'src/netx500/netx500_pflxip2intram.ld')
#	nx500pfl_env.Replace(PRN_SIZE = 0x00080000)
#	nx500pfl_prn_bin = nx500pfl_env.Prn('targets/netx500/nx500pfl_test.bin', None)
#	nx500pfl_prn_obj = nx500pfl_env.Command('targets/netx500/nx500pfl_test.o', nx500pfl_prn_bin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
#	nx500pfl_env.VariantDir('targets/netx500/pfl', 'src', duplicate=0)
#	nx500pfl_all_sources  = [src.replace('src', 'targets/netx500/pfl')  for src in Split(xiptest_sources+xiptest_sources_nx500)]
#	nx500pfl_elf = nx500pfl_env.Elf('targets/nx500_pfl', nx500pfl_all_sources + nx500pfl_prn_obj)
#	nx500pfl_js28f256j3_img = nx500pfl_env.BootBlock('targets/nx500_pfl_js28f256j3.img', nx500pfl_elf, BOOTBLOCK_SRC=dict({0x00:0xf8beaf16, 0x01:0x0100010c}), BOOTBLOCK_DST=dict({0x07:0x0100010c, 0x0e:0x00000001}))
	
#	nx500intram_env = env_netx500.Clone()
#	nx500intram_env.Replace(LDFILE = 'src/netx500/netx500_intram.ld')
#	nx500intram_env.Replace(PRN_SIZE = 0x00001000)
#	nx500intram_prn_bin = nx500intram_env.Prn('targets/netx500/nx500intram_test.bin', None)
#	nx500intram_prn_obj = nx500intram_env.Command('targets/netx500/nx500intram_test.o', nx500intram_prn_bin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx500_nx500intram_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx500_nx500intram_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
#	nx500intram_env.VariantDir('targets/netx500/intram', 'src', duplicate=0)
#	nx500intram_all_sources  = [src.replace('src', 'targets/netx500/intram')  for src in Split(xiptest_sources+xiptest_sources_nx500)]
#	nx500intram_elf = nx500intram_env.Elf('targets/nx500_intram', nx500intram_all_sources + nx500intram_prn_obj)


env_netx56_sqixip = env_netx56_default.Clone()
prn_netx56_sqixip = prn_obj(env_netx56_sqixip, 0x00080000, 'netx56_sqixip')
env_netx56_sqixip.Replace(LDFILE = 'src/netx56/netx56_sqixip2intram.ld')
src_netx56_sqixip = env_netx56_sqixip.SetBuildPath('targets/netx56_sqixip', 'src', sources_common + sources_netx56)
elf_netx56_sqixip = env_netx56_sqixip.Elf('targets/netx56_sqixip.elf', src_netx56_sqixip + prn_netx56_sqixip)
bb0_netx56_sqixip = env_netx56_sqixip.BootBlock('targets/netx56_sqixip.img', elf_netx56_sqixip, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

