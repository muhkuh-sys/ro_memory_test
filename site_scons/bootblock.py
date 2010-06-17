# -*- coding: utf-8 -*-

import array
import os
import re
import subprocess

from SCons.Script import *


def get_segment_table(env, strFileName):
	atSegments = []
	aCmd = [env['OBJDUMP'], '-h', '-w', strFileName]
	proc = subprocess.Popen(aCmd, stdout=subprocess.PIPE)
	strOutput = proc.communicate()[0]
	for match_obj in re.finditer('[ \t]*([0-9]+)[ \t]+([^ \t]+)[ \t]+([0-9a-fA-F]+)[ \t]+([0-9a-fA-F]+)[ \t]+([0-9a-fA-F]+)[ \t]+([0-9a-fA-F]+)[ \t]+([0-9*]+)[ \t]+([a-zA-Z ,]+)', strOutput):
		uiAlign = eval(match_obj.group(7))
		astrFlags = match_obj.group(8).split(', ')
		atSegments.append(dict({
			'idx':		long(match_obj.group(1)),
			'name' :	match_obj.group(2),
			'size' :	long(match_obj.group(3),16),
			'vma' :		long(match_obj.group(4),16),
			'lma' :		long(match_obj.group(5),16),
			'file_off' :	long(match_obj.group(6),16),
			'align' :	uiAlign,
			'flags' :	astrFlags
		}))
	return atSegments


def get_load_address(atSegments):
	# Set an invalid lma
	ulLowestLma = 0x100000000
	
	# Loop over all segments.
	for tSegment in atSegments:
		# Get the segment with the lowest 'lma' entry which has also the flags 'CONTENTS', 'ALLOC' and 'LOAD'.
		if (tSegment['lma']<ulLowestLma) and ('CONTENTS' in tSegment['flags']) and ('ALLOC' in tSegment['flags']) and ('LOAD' in tSegment['flags']):
			ulLowestLma = tSegment['lma']
	
	if ulLowestLma==0x100000000:
		raise Exception("failed to extract load address!")
	
	return ulLowestLma


def get_estimated_bin_size(atSegments):
	ulLoadAddress = get_load_address(atSegments)
	ulBiggestOffset = 0
	
	# Loop over all segments.
	for tSegment in atSegments:
		# Get the segment with the biggest offset to ulLoadAddress which has also the flags 'CONTENTS', 'ALLOC' and 'LOAD'.
		if ('CONTENTS' in tSegment['flags']) and ('ALLOC' in tSegment['flags']) and ('LOAD' in tSegment['flags']):
			ulOffset = tSegment['lma'] + tSegment['size'] - ulLoadAddress
			if ulOffset>ulBiggestOffset:
				ulBiggestOffset = ulOffset
	
	return ulBiggestOffset


def bootblock_action(target, source, env):
	# Get the source filename.
	strElfFileName = source[0].get_path()
	
	# Generate a temp filename for the binary.
	strBinFileName = strElfFileName + '.bin'
	
	# Extract the segments.
	atSegments = get_segment_table(env, strElfFileName)
	# Get the estimated binary size from the segments.
	ulEstimatedBinSize = get_estimated_bin_size(atSegments)
	# Do not create files larger than 512MB.
	if ulEstimatedBinSize>=0x20000000:
		raise Exception("The resulting file seems to extend 512MBytes. Too scared to continue!")
	
	# Extract the binaries.
	subprocess.check_call([env['OBJCOPY'], '-O', 'binary', strElfFileName, strBinFileName])
	
	# Get the start address.
	aCmd = [env['OBJDUMP'], '-f', strElfFileName]
	proc = subprocess.Popen(aCmd, stdout=subprocess.PIPE)
	strOutput = proc.communicate()[0]
	match_obj = re.search('start address 0x([0-9a-fA-F]+)', strOutput)
	if not match_obj:
		print 'Failed to extract start address.'
		print 'Command:', aCmd
		print 'Output:', strOutput
		raise Exception('Failed to extract start address.')
	
	ulExecAddress = long(match_obj.group(1), 16)
	ulLoadAddress = get_load_address(atSegments)
	
	# Get the application data.
	ulApplicationSize = os.stat(strBinFileName).st_size
	if (ulApplicationSize&3)!=0:
		raise Exception("The application size is no multiple of dwords!")
	fApplicationFile = open(strBinFileName, 'rb')
	aulApplicationData = array.array('L')
	aulApplicationData.fromfile(fApplicationFile, ulApplicationSize/4)
	
	# Build the application checksum.
	ulApplicationChecksum = 0
	for ulData in aulApplicationData:
		ulApplicationChecksum += ulData
		ulApplicationChecksum &= 0xffffffff
	# TODO: Read the xml file.
	# TODO: extract the options.
	
	aBootBlock = array.array('L', [0]*16)
	aBootBlock[0x00] = 0xf8beaf16			# Magic cookie.
	aBootBlock[0x01] = 0x0100010c			# unCtrl
	aBootBlock[0x02] = ulExecAddress		# execution address
	aBootBlock[0x03] = ulApplicationChecksum	# application checksum
	aBootBlock[0x04] = ulApplicationSize/4		# application dword size
	aBootBlock[0x05] = ulLoadAddress		# load address
	aBootBlock[0x06] = 0x5854454e			# 'NETX' signature
	aBootBlock[0x07] = 0x0100010c			# krams
	aBootBlock[0x08] = 0x00000000			# krams
	aBootBlock[0x09] = 0x00000000			# krams
	aBootBlock[0x0a] = 0x00000000			# krams
	aBootBlock[0x0b] = 0x00000000			# krams
	aBootBlock[0x0c] = 0x00000001			# misc_asic_ctrl dummy
	aBootBlock[0x0d] = 0x00000000			# user data
	aBootBlock[0x0e] = 0x00000001			# src type
	
	# Build the bootblock checksum.
	ulBootblockChecksum = 0
	for ulData in aBootBlock:
		ulBootblockChecksum += ulData
		ulBootblockChecksum &= 0xffffffff
	ulBootblockChecksum = (ulBootblockChecksum-1)^0xffffffff
	aBootBlock[0x0f] = ulBootblockChecksum
	
	# Write the bootimage.
	fOutput = open(target[0].get_path(), 'wb')
	aBootBlock.tofile(fOutput)
	aulApplicationData.tofile(fOutput)
	fOutput.close()
	
	return None


def bootblock_emitter(target, source, env):
	# Make the target depend on the xml file and the parameter.
	Depends(target, env['BOOTBLOCK_XML'])
	Depends(target, SCons.Node.Python.Value(env['BOOTBLOCK_SRC']))
	Depends(target, SCons.Node.Python.Value(env['BOOTBLOCK_DST']))
	Depends(target, SCons.Node.Python.Value(env['BOOTBLOCK_USERDATA']))
	
	return target, source


def bootblock_string(target, source, env):
	return 'BootBlock %s' % target[0].get_path()


def ApplyToEnv(env):                                                                                                                                                                                            
	#----------------------------------------------------------------------------                                                                                                                           
	#                                                                                                                                                                                                       
	# Add secmem builder.                                                                                                                                                                                 
	#                                                                                                                                                                                                       
	env['BOOTBLOCK_XML'] = 'site_scons/netx.xml'
	env['BOOTBLOCK_SRC'] = ''
	env['BOOTBLOCK_DST'] = ''
	env['BOOTBLOCK_USERDATA'] = 0
	
	bootblock_act = SCons.Action.Action(bootblock_action, bootblock_string)
	bootblock_bld = Builder(action=bootblock_act, emitter=bootblock_emitter, suffix='.bin', single_source=1, src_suffix='.elf', src_builder='Elf')
	env['BUILDERS']['BootBlock'] = bootblock_bld

