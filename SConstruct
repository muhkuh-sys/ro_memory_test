# -*- coding: utf-8 -*-


import os.path

#----------------------------------------------------------------------------
#
# Set up the Muhkuh Build System.
#
SConscript('mbs/SConscript')
Import('atEnv')

# Create a build environment for the ARM9 based netX chips.
env_arm9 = atEnv.DEFAULT.CreateEnvironment(['gcc-arm-none-eabi-4.7', 'asciidoc'])
env_arm9.CreateCompilerEnv('NETX500', ['arch=armv5te'])
env_arm9.CreateCompilerEnv('NETX56', ['arch=armv5te'])
env_arm9.CreateCompilerEnv('NETX50', ['arch=armv5te'])
env_arm9.CreateCompilerEnv('NETX10', ['arch=armv5te'])

# Create a build environment for the Cortex-R7 and Cortex-A9 based netX chips.
env_cortexR7 = atEnv.DEFAULT.CreateEnvironment(['gcc-arm-none-eabi-4.9', 'asciidoc'])
env_cortexR7.CreateCompilerEnv('NETX4000_RELAXED', ['arch=armv7', 'thumb'], ['arch=armv7-r', 'thumb'])

# Create a build environment for the Cortex-M4 based netX chips.
env_cortexM4 = atEnv.DEFAULT.CreateEnvironment(['gcc-arm-none-eabi-4.9', 'asciidoc'])
env_cortexM4.CreateCompilerEnv('NETX90_MPW', ['arch=armv7', 'thumb'], ['arch=armv7e-m', 'thumb'])
env_cortexM4.CreateCompilerEnv('NETX90_MPW_APP', ['arch=armv7', 'thumb'], ['arch=armv7e-m', 'thumb'])

# Build the platform libraries.
SConscript('platform/SConscript')


#----------------------------------------------------------------------------
#
# This is a helper function which generates a sequence of pseudo random
# numbers.
#
def prn_obj(tEnv, sizSequence, strPrnBinFilename):
	# Convert the size into a "long" to prevent floats.
	ulSequenceSize = long(sizSequence)
	
	# Generate the sequence.
	tPrnBin = tEnv.Prn(strPrnBinFilename, None, PRN_SIZE=ulSequenceSize)
	
	# Convert the binary file into an object.
	strLabelPath = strPrnBinFilename.replace('/', '_').replace('.', '_').replace('\\', '_')
	strOutput = os.path.splitext(strPrnBinFilename)[0] + tEnv['OBJSUFFIX']
	strCmd = '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.prn_rodata --redefine-sym "_binary_%s_start"="_binary_test_bin_start" --redefine-sym "_binary_%s_end"="_binary_test_bin_end" $SOURCE $TARGET' % (strLabelPath, strLabelPath)
	return tEnv.Command(strOutput, tPrnBin, strCmd)


#----------------------------------------------------------------------------
#
# Get the source code version from the VCS.
#
atEnv.DEFAULT.Version('targets/version/version.h', 'templates/version.h')


#----------------------------------------------------------------------------
#
# Build all files.
#

aCppPath = ['src', '#platform/src', '#platform/src/lib', '#targets/version']

sources_common = """
	src/header.c
	src/init.S
	src/main.c
"""


sources_netx10 = """
"""


sources_netx56 = """
	src/usb_cdc.c
"""


sources_netx90_com = """
	src/netx90/comled.c
"""

env_netx10 = atEnv.NETX10.Clone()
env_netx10.Append(CPPPATH = aCppPath)

env_netx10_sqixip = env_netx10.Clone()
env_netx10_sqixip.Replace(LDFILE = 'src/netx10/netx10_sqixip.ld')

env_netx10_sqixip_128k = env_netx10_sqixip.Clone()
prn_netx10_sqixip_128k = prn_obj(env_netx10_sqixip_128k, 0x00008000, 'targets/netx10_sqixip_128k/prn_128k.bin')
src_netx10_sqixip_128k = env_netx10_sqixip_128k.SetBuildPath('targets/netx10_sqixip_128k', 'src', sources_common + sources_netx10)
elf_netx10_sqixip_128k = env_netx10_sqixip_128k.Elf('targets/netx10_sqixip_128k/netx10_sqixip.elf', src_netx10_sqixip_128k + env_netx10_sqixip_128k['PLATFORM_LIBRARY'] + prn_netx10_sqixip_128k)
bb0_netx10_sqixip_128k = env_netx10_sqixip_128k.BootBlock('targets/rotest_netx10_sqixip_128k.img', elf_netx10_sqixip_128k, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

env_netx10_sqixip_256k = env_netx10_sqixip.Clone()
prn_netx10_sqixip_256k = prn_obj(env_netx10_sqixip_256k, 0x00010000, 'targets/netx10_sqixip_256k/prn_256k.bin')
src_netx10_sqixip_256k = env_netx10_sqixip_256k.SetBuildPath('targets/netx10_sqixip_256k', 'src', sources_common + sources_netx10)
elf_netx10_sqixip_256k = env_netx10_sqixip_256k.Elf('targets/netx10_sqixip_256k/netx10_sqixip.elf', src_netx10_sqixip_256k + env_netx10_sqixip_256k['PLATFORM_LIBRARY'] + prn_netx10_sqixip_256k)
bb0_netx10_sqixip_256k = env_netx10_sqixip_256k.BootBlock('targets/rotest_netx10_sqixip_256k.img', elf_netx10_sqixip_256k, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

env_netx10_sqixip_512k = env_netx10_sqixip.Clone()
prn_netx10_sqixip_512k = prn_obj(env_netx10_sqixip_512k, 0x00020000, 'targets/netx10_sqixip_512k/prn_512k.bin')
src_netx10_sqixip_512k = env_netx10_sqixip_512k.SetBuildPath('targets/netx10_sqixip_512k', 'src', sources_common + sources_netx10)
elf_netx10_sqixip_512k = env_netx10_sqixip_512k.Elf('targets/netx10_sqixip_512k/netx10_sqixip.elf', src_netx10_sqixip_512k + env_netx10_sqixip_512k['PLATFORM_LIBRARY'] + prn_netx10_sqixip_512k)
bb0_netx10_sqixip_512k = env_netx10_sqixip_512k.BootBlock('targets/rotest_netx10_sqixip_512k.img', elf_netx10_sqixip_512k, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))



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


env_netx50_sdram = atEnv.NETX50.Clone()
env_netx50_sdram.Replace(LDFILE = 'src/netx50/netx50_sdram.ld')
env_netx50_sdram.Append(CPPPATH = aCppPath)

env_netx50_sdram_128k = env_netx50_sdram.Clone()
prn_netx50_sdram_128k = prn_obj(env_netx50_sdram_128k, 0x00008000, 'targets/netx50_sdram_128k/prn_128k.bin')
src_netx50_sdram_128k = env_netx50_sdram_128k.SetBuildPath('targets/netx50_sdram_128k', 'src', sources_common)
elf_netx50_sdram_128k = env_netx50_sdram_128k.Elf('targets/netx50_sdram_128k/rotest_128k.elf', src_netx50_sdram_128k + prn_netx50_sdram_128k + env_netx50_sdram_128k['PLATFORM_LIBRARY'])
bb0_netx50_sdram_128k = env_netx50_sdram_128k.BootBlock('targets/netx50_sdram_128k/MT48LC2M32B2/netx.rom', elf_netx50_sdram_128k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx50_sdram_256k = env_netx50_sdram.Clone()
prn_netx50_sdram_256k = prn_obj(env_netx50_sdram_256k, 0x00010000, 'targets/netx50_sdram_256k/prn_256k.bin')
src_netx50_sdram_256k = env_netx50_sdram_256k.SetBuildPath('targets/netx50_sdram_256k', 'src', sources_common)
elf_netx50_sdram_256k = env_netx50_sdram_256k.Elf('targets/netx50_sdram_256k/rotest_256k.elf', src_netx50_sdram_256k + prn_netx50_sdram_256k + env_netx50_sdram_256k['PLATFORM_LIBRARY'])
bb0_netx50_sdram_256k = env_netx50_sdram_256k.BootBlock('targets/netx50_sdram_256k/MT48LC2M32B2/netx.rom', elf_netx50_sdram_256k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx50_sdram_512k = env_netx50_sdram.Clone()
prn_netx50_sdram_512k = prn_obj(env_netx50_sdram_512k, 0x00020000, 'targets/netx50_sdram_512k/prn_512k.bin')
src_netx50_sdram_512k = env_netx50_sdram_512k.SetBuildPath('targets/netx50_sdram_512k', 'src', sources_common)
elf_netx50_sdram_512k = env_netx50_sdram_512k.Elf('targets/netx50_sdram_512k/rotest_512k.elf', src_netx50_sdram_512k + prn_netx50_sdram_512k + env_netx50_sdram_512k['PLATFORM_LIBRARY'])
bb0_netx50_sdram_512k = env_netx50_sdram_512k.BootBlock('targets/netx50_sdram_512k/MT48LC2M32B2/netx.rom', elf_netx50_sdram_512k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')


# NOTE: Use 4000k and not 4096k (or 4mb) to keep some space for the test routines.
env_netx50_pfl_4000k = atEnv.NETX50.Clone()
env_netx50_pfl_4000k.Replace(LDFILE = 'src/netx50/netx50_pfl_xip.ld')
env_netx50_pfl_4000k.Append(CPPPATH = aCppPath)
prn_netx50_pfl_4000k = prn_obj(env_netx50_pfl_4000k, 4000*1024/4, 'targets/netx50_pfl_4000k/prn_.bin')
src_netx50_pfl_4000k = env_netx50_pfl_4000k.SetBuildPath('targets/netx50_pfl_4000k', 'src', sources_common)
elf_netx50_pfl_4000k = env_netx50_pfl_4000k.Elf('targets/netx50_pfl_4000k/rotest_4000k.elf', src_netx50_pfl_4000k + prn_netx50_pfl_4000k + env_netx50_pfl_4000k['PLATFORM_LIBRARY'])
bb0_netx50_pfl_4000k = env_netx50_pfl_4000k.BootBlock('targets/netx50_rotest_S29GL032N90BFI030.img', elf_netx50_pfl_4000k, BOOTBLOCK_SRC=dict({0x00:0xf8beaf16, 0x01:0x01010305}), BOOTBLOCK_DST=dict({0x07:0x01010305, 0x0e:0x00000001}))
bb1_netx50_pfl_4000k = env_netx50_pfl_4000k.BootBlock('targets/netx50_rotest_S29GL032N90BFI030_and_S29GL032D70BFI030.img', elf_netx50_pfl_4000k, BOOTBLOCK_SRC=dict({0x00:0xf8beaf16, 0x01:0x01010306}), BOOTBLOCK_DST=dict({0x07:0x01010306, 0x0e:0x00000001}))


#	nx500pfl_env = env_netx500.Clone()
#	nx500pfl_env.Replace(LDFILE = 'src/netx500/netx500_pflxip2intram.ld')
#	nx500pfl_env.Replace(PRN_SIZE = 0x00080000)
#	nx500pfl_prn_bin = nx500pfl_env.Prn('targets/netx500/nx500pfl_test.bin', None)
#	nx500pfl_prn_obj = nx500pfl_env.Command('targets/netx500/nx500pfl_test.o', nx500pfl_prn_bin, '"$OBJCOPY" -v -I binary -O elf32-littlearm -B ARM --rename-section .data=.rodata --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_start"="_binary_test_bin_start" --redefine-sym "_binary_targets_netx500_nx500pfl_test_bin_end"="_binary_test_bin_end" $SOURCE $TARGET')
#	nx500pfl_env.VariantDir('targets/netx500/pfl', 'src', duplicate=0)
#	nx500pfl_all_sources  = [src.replace('src', 'targets/netx500/pfl')  for src in Split(xiptest_sources+xiptest_sources_nx500)]
#	nx500pfl_elf = nx500pfl_env.Elf('targets/nx500_pfl', nx500pfl_all_sources + nx500pfl_prn_obj)
#	nx500pfl_js28f256j3_img = nx500pfl_env.BootBlock('targets/nx500_pfl_js28f256j3.img', nx500pfl_elf, BOOTBLOCK_SRC=dict({0x00:0xf8beaf16, 0x01:0x0100010c}), BOOTBLOCK_DST=dict({0x07:0x0100010c, 0x0e:0x00000001}))

env_netx500_sdram = atEnv.NETX500.Clone()
env_netx500_sdram.Replace(LDFILE = 'src/netx500/netx500_sdram.ld')
env_netx500_sdram.Append(CPPPATH = aCppPath)

env_netx500_sdram_128k = env_netx500_sdram.Clone()
prn_netx500_sdram_128k = prn_obj(env_netx500_sdram_128k, 0x00008000, 'targets/netx500_sdram_128k/prn_128k.bin')
src_netx500_sdram_128k = env_netx500_sdram_128k.SetBuildPath('targets/netx500_sdram_128k', 'src', sources_common)
elf_netx500_sdram_128k = env_netx500_sdram_128k.Elf('targets/netx500_sdram_128k/rotest_128k.elf', src_netx500_sdram_128k + prn_netx500_sdram_128k + env_netx500_sdram_128k['PLATFORM_LIBRARY'])
bb0_netx500_sdram_128k = env_netx500_sdram_128k.BootBlock('targets/netx500_sdram_128k/MT48LC2M32B2/netx.rom', elf_netx500_sdram_128k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx500_sdram_256k = env_netx500_sdram.Clone()
prn_netx500_sdram_256k = prn_obj(env_netx500_sdram_256k, 0x00010000, 'targets/netx500_sdram_256k/prn_256k.bin')
src_netx500_sdram_256k = env_netx500_sdram_256k.SetBuildPath('targets/netx500_sdram_256k', 'src', sources_common)
elf_netx500_sdram_256k = env_netx500_sdram_256k.Elf('targets/netx500_sdram_256k/rotest_256k.elf', src_netx500_sdram_256k + prn_netx500_sdram_256k + env_netx500_sdram_256k['PLATFORM_LIBRARY'])
bb0_netx500_sdram_256k = env_netx500_sdram_256k.BootBlock('targets/netx500_sdram_256k/MT48LC2M32B2/netx.rom', elf_netx500_sdram_256k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx500_sdram_512k = env_netx500_sdram.Clone()
prn_netx500_sdram_512k = prn_obj(env_netx500_sdram_512k, 0x00020000, 'targets/netx500_sdram_512k/prn_512k.bin')
src_netx500_sdram_512k = env_netx500_sdram_512k.SetBuildPath('targets/netx500_sdram_512k', 'src', sources_common)
elf_netx500_sdram_512k = env_netx500_sdram_512k.Elf('targets/netx500_sdram_512k/rotest_512k.elf', src_netx500_sdram_512k + prn_netx500_sdram_512k + env_netx500_sdram_512k['PLATFORM_LIBRARY'])
bb0_netx500_sdram_512k = env_netx500_sdram_512k.BootBlock('targets/netx500_sdram_512k/MT48LC2M32B2/netx.rom', elf_netx500_sdram_512k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='SD_MT48LC2M32B2')
bb1_netx500_sdram_512k = env_netx500_sdram_512k.BootBlock('targets/rotest_netx500_MT48LC2M32B2_512k.img', elf_netx500_sdram_512k, BOOTBLOCK_SRC='SPI_GEN_10', BOOTBLOCK_DST='SD_MT48LC2M32B2')

#
# netX56
#

env_netx56 = atEnv.NETX56.Clone()
env_netx56.Append(CPPPATH = aCppPath)

env_netx56_sdram = env_netx56.Clone()
env_netx56_sdram.Replace(LDFILE = 'src/netx56/netx56_sdram.ld')

env_netx56_sdram_128k = env_netx56_sdram.Clone()
src_netx56_sdram_128k = env_netx56_sdram_128k.SetBuildPath('targets/netx56_sdram_128k', 'src', sources_common + sources_netx56)
prn_netx56_sdram_128k = prn_obj(env_netx56_sdram_128k, 0x00008000, 'targets/netx56_sdram_128k/prn_128k.bin')
elf_netx56_sdram_128k = env_netx56_sdram_128k.Elf('targets/netx56_sdram/rotest_128k.elf', src_netx56_sdram_128k + env_netx56_sdram_128k['PLATFORM_LIBRARY'] + prn_netx56_sdram_128k)
bb0_netx56_sdram_128k = env_netx56_sdram_128k.BootBlock('targets/rotest_netx56_MT48LC2M32B2_128k.img', elf_netx56_sdram_128k, BOOTBLOCK_SRC='SPI_GEN_10', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx56_sdram_256k = env_netx56_sdram.Clone()
src_netx56_sdram_256k = env_netx56_sdram_256k.SetBuildPath('targets/netx56_sdram_256k', 'src', sources_common + sources_netx56)
prn_netx56_sdram_256k = prn_obj(env_netx56_sdram_256k, 0x00010000, 'targets/netx56_sdram_256k/prn_256k.bin')
elf_netx56_sdram_256k = env_netx56_sdram_256k.Elf('targets/netx56_sdram/rotest_256k.elf', src_netx56_sdram_256k + env_netx56_sdram_256k['PLATFORM_LIBRARY'] + prn_netx56_sdram_256k)
bb0_netx56_sdram_256k = env_netx56_sdram_256k.BootBlock('targets/rotest_netx56_MT48LC2M32B2_256k.img', elf_netx56_sdram_256k, BOOTBLOCK_SRC='SPI_GEN_10', BOOTBLOCK_DST='SD_MT48LC2M32B2')

env_netx56_sdram_512k = env_netx56_sdram.Clone()
src_netx56_sdram_512k = env_netx56_sdram_512k.SetBuildPath('targets/netx56_sdram_512k', 'src', sources_common + sources_netx56)
prn_netx56_sdram_512k = prn_obj(env_netx56_sdram_512k, 0x00020000, 'targets/netx56_sdram_512k/prn_512k.bin')
elf_netx56_sdram_512k = env_netx56_sdram_512k.Elf('targets/netx56_sdram/rotest_512k.elf', src_netx56_sdram_512k + env_netx56_sdram_512k['PLATFORM_LIBRARY'] + prn_netx56_sdram_512k)
bb0_netx56_sdram_512k = env_netx56_sdram_512k.BootBlock('targets/rotest_netx56_MT48LC2M32B2_512k.img', elf_netx56_sdram_512k, BOOTBLOCK_SRC='SPI_GEN_10', BOOTBLOCK_DST='SD_MT48LC2M32B2')


env_netx56_sqixip = env_netx56.Clone()
env_netx56_sqixip.Replace(LDFILE = 'src/netx56/netx56_sqixip.ld')

env_netx56_sqixip_128k = env_netx56_sqixip.Clone()
src_netx56_sqixip_128k = env_netx56_sqixip_128k.SetBuildPath('targets/netx56_sqixip_128k', 'src', sources_common + sources_netx56)
prn_netx56_sqixip_128k = prn_obj(env_netx56_sqixip_128k, 0x00008000, 'targets/netx56_sqixip_128k/prn_128k.bin')
elf_netx56_sqixip_128k = env_netx56_sqixip_128k.Elf('targets/netx56_sqixip/rotest_128k.elf', src_netx56_sqixip_128k + env_netx56_sqixip_128k['PLATFORM_LIBRARY'] + prn_netx56_sqixip_128k)
bb0_netx56_sqixip_128k = env_netx56_sqixip_128k.BootBlock('targets/rotest_netx56_sqixip_128k.img', elf_netx56_sqixip_128k, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

env_netx56_sqixip_256k = env_netx56_sqixip.Clone()
src_netx56_sqixip_256k = env_netx56_sqixip_256k.SetBuildPath('targets/netx56_sqixip_256k', 'src', sources_common + sources_netx56)
prn_netx56_sqixip_256k = prn_obj(env_netx56_sqixip_256k, 0x00010000, 'targets/netx56_sqixip_256k/prn_256k.bin')
elf_netx56_sqixip_256k = env_netx56_sqixip_256k.Elf('targets/netx56_sqixip/rotest_256k.elf', src_netx56_sqixip_256k + env_netx56_sqixip_256k['PLATFORM_LIBRARY'] + prn_netx56_sqixip_256k)
bb0_netx56_sqixip_256k = env_netx56_sqixip_256k.BootBlock('targets/rotest_netx56_sqixip_256k.img', elf_netx56_sqixip_256k, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

env_netx56_sqixip_512k = env_netx56_sqixip.Clone()
src_netx56_sqixip_512k = env_netx56_sqixip_512k.SetBuildPath('targets/netx56_sqixip_512k', 'src', sources_common + sources_netx56)
prn_netx56_sqixip_512k = prn_obj(env_netx56_sqixip_512k, 0x00020000, 'targets/netx56_sqixip_512k/prn_512k.bin')
elf_netx56_sqixip_512k = env_netx56_sqixip_512k.Elf('targets/netx56_sqixip/rotest_512k.elf', src_netx56_sqixip_512k + env_netx56_sqixip_512k['PLATFORM_LIBRARY'] + prn_netx56_sqixip_512k)
bb0_netx56_sqixip_512k = env_netx56_sqixip_512k.BootBlock('targets/rotest_netx56_sqixip_512k.img', elf_netx56_sqixip_512k, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

env_netx56_sqixip_1m = env_netx56_sqixip.Clone()
src_netx56_sqixip_1m = env_netx56_sqixip_1m.SetBuildPath('targets/netx56_sqixip_1m', 'src', sources_common + sources_netx56)
prn_netx56_sqixip_1m = prn_obj(env_netx56_sqixip_1m, 0x00040000, 'targets/netx56_sqixip_1m/prn_1m.bin')
elf_netx56_sqixip_1m = env_netx56_sqixip_1m.Elf('targets/netx56_sqixip/rotest_1m.elf', src_netx56_sqixip_1m + env_netx56_sqixip_1m['PLATFORM_LIBRARY'] + prn_netx56_sqixip_1m)
bb0_netx56_sqixip_1m = env_netx56_sqixip_1m.BootBlock('targets/rotest_netx56_sqixip_1m.img', elf_netx56_sqixip_1m, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))

env_netx56_sqixip_2m = env_netx56_sqixip.Clone()
src_netx56_sqixip_2m = env_netx56_sqixip_2m.SetBuildPath('targets/netx56_sqixip_2m', 'src', sources_common + sources_netx56)
prn_netx56_sqixip_2m = prn_obj(env_netx56_sqixip_2m, 0x00080000, 'targets/netx56_sqixip_2m/prn_2m.bin')
elf_netx56_sqixip_2m = env_netx56_sqixip_2m.Elf('targets/netx56_sqixip/rotest_2m.elf', src_netx56_sqixip_2m + env_netx56_sqixip_2m['PLATFORM_LIBRARY'] + prn_netx56_sqixip_2m)
bb0_netx56_sqixip_2m = env_netx56_sqixip_2m.BootBlock('targets/rotest_netx56_sqixip_2m.img', elf_netx56_sqixip_2m, BOOTBLOCK_SRC=dict({0x01:0x00000008, 0x0e:0x00000002}), BOOTBLOCK_DST=dict({}))


env_netx56_intram = env_netx56.Clone()
env_netx56_intram.Replace(LDFILE = 'src/netx56/netx56_intram.ld')

env_netx56_intram_128k = env_netx56_intram.Clone()
src_netx56_intram_128k = env_netx56_intram_128k.SetBuildPath('targets/netx56_intram_128k', 'src', sources_common + sources_netx56)
prn_netx56_intram_128k = prn_obj(env_netx56_intram_128k, 0x00008000, 'targets/netx56_intram_128k/prn_128k.bin')
elf_netx56_intram_128k = env_netx56_intram_128k.Elf('targets/netx56_intram_128k/rotest_128k.elf', src_netx56_intram_128k + prn_netx56_intram_128k + env_netx56_intram_128k['PLATFORM_LIBRARY'])
bb0_netx56_intram_128k = env_netx56_intram_128k.BootBlock('targets/rotest_netx56_intram_128k.img', elf_netx56_intram_128k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='INTRAM')

env_netx56_intram_256k = env_netx56_intram.Clone()
src_netx56_intram_256k = env_netx56_intram_256k.SetBuildPath('targets/netx56_intram_256k', 'src', sources_common + sources_netx56)
prn_netx56_intram_256k = prn_obj(env_netx56_intram_256k, 0x00010000, 'targets/netx56_intram_256k/prn_256k.bin')
elf_netx56_intram_256k = env_netx56_intram_256k.Elf('targets/netx56_intram_256k/rotest_256k.elf', src_netx56_intram_256k + prn_netx56_intram_256k + env_netx56_intram_256k['PLATFORM_LIBRARY'])
bb0_netx56_intram_256k = env_netx56_intram_256k.BootBlock('targets/rotest_netx56_intram_256k.img', elf_netx56_intram_256k, BOOTBLOCK_SRC='MMC', BOOTBLOCK_DST='INTRAM')


#
# netX90
#

env_netx90_com = atEnv.NETX90_MPW.Clone()
env_netx90_com.Append(CPPPATH = aCppPath)

env_netx90_com_iflash = env_netx90_com.Clone()
env_netx90_com_iflash.Replace(LDFILE = 'src/netx90/netx90_com_iflash.ld')
src_netx90_com_iflash = env_netx90_com_iflash.SetBuildPath('targets/netx90_com_iflash', 'src', sources_common + sources_netx90_com)
prn_netx90_com_iflash = prn_obj(env_netx90_com_iflash, 0x0003f800, 'targets/netx90_com_iflash/prn_iflash01.bin')
elf_netx90_com_iflash = env_netx90_com_iflash.Elf('targets/netx90_com_iflash/rotest_iflash01.elf', src_netx90_com_iflash + env_netx90_com_iflash['PLATFORM_LIBRARY'] + prn_netx90_com_iflash)
env_netx90_com_iflash.HBootImage('targets/rotest_netx90_com_iflash.bin', 'src/netx90/COM_IFLASH_XIP.xml', HBOOTIMAGE_KNOWN_FILES=dict({'tElfCOM': elf_netx90_com_iflash}))
env_netx90_com_iflash.HBootImage('targets/rotest_netx90_com_iflash_appa.bin', 'src/netx90/COM_IFLASH_XIP_ACTIVATE_APP.xml', HBOOTIMAGE_KNOWN_FILES=dict({'tElfCOM': elf_netx90_com_iflash}))
