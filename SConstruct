# -*- coding: utf-8 -*-


import os.path

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
	src/header.c
	src/init_netx_test.S
	src/main.c
"""

sources_netx56 = """
	src/usb_cdc.c
"""

#----------------------------------------------------------------------------
#
# Create the compiler environments.
#
env_default.Append(CPPPATH = ['src', 'platform'])

env_netx500_default = env_default.CreateCompilerEnv('500', ['arch=armv5te'])
env_netx500_default.Replace(BOOTBLOCK_CHIPTYPE = 500)

env_netx56_default = env_default.CreateCompilerEnv('56', ['arch=armv5te'])
env_netx56_default.Replace(BOOTBLOCK_CHIPTYPE = 56)

env_netx50_default = env_default.CreateCompilerEnv('50', ['arch=armv5te'])
env_netx50_default.Replace(BOOTBLOCK_CHIPTYPE = 50)

env_netx10_default = env_default.CreateCompilerEnv('10', ['arch=armv5te'])
env_netx10_default.Replace(BOOTBLOCK_CHIPTYPE = 10)

Export('env_netx500_default', 'env_netx56_default', 'env_netx50_default', 'env_netx10_default')


#----------------------------------------------------------------------------
#
# Build the platform library.
#
SConscript('platform/SConscript')
Import('platform_lib_netx500', 'platform_lib_netx56', 'platform_lib_netx50', 'platform_lib_netx10')


#----------------------------------------------------------------------------
#
# This is a helper function which generates a sequence of pseudo random
# numbers.
#
def prn_obj(tEnv, sizSequence, strPrnBinFilename):
	# Generate the sequence.
	tPrnBin = tEnv.Prn(strPrnBinFilename, None, PRN_SIZE=sizSequence)

	# Convert the binary file into an object.
	strLabelPath = strPrnBinFilename.replace('/', '_').replace('.', '_').replace('\\', '_')
	strOutput = os.path.splitext(strPrnBinFilename)[0] + tEnv['OBJSUFFIX']
	strCmd = '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_%s_start"="_binary_test_bin_start" --redefine-sym "_binary_%s_end"="_binary_test_bin_end" $SOURCE $TARGET' % (strLabelPath, strLabelPath)
	return tEnv.Command(strOutput, tPrnBin, strCmd)


#----------------------------------------------------------------------------
#
# Get the source code version from the VCS.
#
env_default.Version('targets/version/version.h', 'templates/version.h')


#----------------------------------------------------------------------------
#
# Build all files.
#

aCppPath = ['src', '#platform/src', '#platform/src/lib', '#targets/version']

#env_netx10_sqixip = env_netx10_default.Clone()
#prn_netx10_sqixip = prn_obj(env_netx10_sqixip, 0x00080000, 'netx10_sqixip')
#env_netx10_sqixip.Replace(LDFILE = 'src/netx10/netx10_sqixip2intram.ld')
#src_netx10_sqixip = env_netx10_sqixip.SetBuildPath('targets/netx10_sqixip', 'src', sources_common + sources_netx10)
#elf_netx10_sqixip = env_netx10_sqixip.Elf('targets/netx10_sqixip.elf', src_netx10_sqixip + prn_netx10_sqixip)
#bb0_netx10_sqixip = env_netx10_sqixip.BootBlock('targets/nx10_sqixip.img', elf_netx10_sqixip, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))


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


env_netx50_sdram = env_netx50_default.Clone()
env_netx50_sdram.Replace(LDFILE = 'src/netx50/netx50_sdram.ld')
env_netx50_sdram.Append(CPPPATH = aCppPath)
prn_netx50_sdram = prn_obj(env_netx50_sdram, 0x0003fd16, 'targets/netx50_sdram/prn.bin')
src_netx50_sdram = env_netx50_sdram.SetBuildPath('targets/netx50_sdram', 'src', sources_common)
elf_netx50_sdram = env_netx50_sdram.Elf('targets/netx50_sdram/rotest.elf', src_netx50_sdram + prn_netx50_sdram + platform_lib_netx50)
bb0_netx50_sdram = env_netx50_sdram.BootBlock('targets/netx50_sdram/netx.rom', elf_netx50_sdram, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')


#	nx500pfl_env = env_netx500.Clone()
#	nx500pfl_env.Replace(LDFILE = 'src/netx500/netx500_pflxip2intram.ld')
#	nx500pfl_env.Replace(PRN_SIZE = 0x00080000)
#	nx500pfl_prn_bin = nx500pfl_env.Prn('targets/netx500/nx500pfl_test.bin', None)
#	nx500pfl_prn_obj = nx500pfl_env.Command('targets/netx500/nx500pfl_test.o', nx500pfl_prn_bin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
#	nx500pfl_env.VariantDir('targets/netx500/pfl', 'src', duplicate=0)
#	nx500pfl_all_sources  = [src.replace('src', 'targets/netx500/pfl')  for src in Split(xiptest_sources+xiptest_sources_nx500)]
#	nx500pfl_elf = nx500pfl_env.Elf('targets/nx500_pfl', nx500pfl_all_sources + nx500pfl_prn_obj)
#	nx500pfl_js28f256j3_img = nx500pfl_env.BootBlock('targets/nx500_pfl_js28f256j3.img', nx500pfl_elf, BOOTBLOCK_SRC=dict({0x00:0xf8beaf16, 0x01:0x0100010c}), BOOTBLOCK_DST=dict({0x07:0x0100010c, 0x0e:0x00000001}))

env_netx500_sdram = env_netx500_default.Clone()
env_netx500_sdram.Replace(LDFILE = 'src/netx500/netx500_sdram.ld')
env_netx500_sdram.Append(CPPPATH = aCppPath)
prn_netx500_sdram = prn_obj(env_netx500_sdram, 0x0003fd16, 'targets/netx500/sdram/prn.bin')
src_netx500_sdram = env_netx500_sdram.SetBuildPath('targets/netx500/sdram', 'src', sources_common)
elf_netx500_sdram = env_netx500_sdram.Elf('targets/netx500/sdram/rotest.elf', src_netx500_sdram + prn_netx500_sdram + platform_lib_netx500)
bb0_netx500_sdram = env_netx500_sdram.BootBlock('targets/netx500/sdram/MT48LC2M32B2/netx.rom', elf_netx500_sdram, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx56_sdram = env_netx56_default.Clone()
env_netx56_sdram.Replace(LDFILE = 'src/netx56/netx56_sdram.ld')
env_netx56_sdram.Append(CPPPATH = aCppPath)
prn_netx56_sdram = prn_obj(env_netx56_sdram, 0x0003fd16, 'targets/netx56_sdram/prn.bin')
src_netx56_sdram = env_netx56_sdram.SetBuildPath('targets/netx56_sdram', 'src', sources_common + sources_netx56)
elf_netx56_sdram = env_netx56_sdram.Elf('targets/netx56_sdram/rotest.elf', src_netx56_sdram + prn_netx56_sdram + platform_lib_netx56)
bb0_netx56_sdram = env_netx56_sdram.BootBlock('targets/netx56_sdram/netx.rom', elf_netx56_sdram, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx56_sqixip = env_netx56_default.Clone()
env_netx56_sqixip.Replace(LDFILE = 'src/netx56/netx56_sqixip.ld')
env_netx56_sqixip.Append(CPPPATH = aCppPath)
prn_netx56_sqixip = prn_obj(env_netx56_sqixip, 0x000ff000, 'targets/netx56_sqixip/prn.bin')
src_netx56_sqixip = env_netx56_sqixip.SetBuildPath('targets/netx56_sqixip', 'src', sources_common + sources_netx56)
elf_netx56_sqixip = env_netx56_sqixip.Elf('targets/netx56_sqixip/rotest.elf', src_netx56_sqixip + prn_netx56_sqixip + platform_lib_netx56)
bb0_netx56_sqixip = env_netx56_sqixip.BootBlock('targets/netx56_sqixip/rotest.img', elf_netx56_sqixip, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

