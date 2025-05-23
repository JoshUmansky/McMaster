#
# Comp Eng 3DY4 (Computer Systems Integration Project)
#
# Copyright by Nicola Nicolici
# Department of Electrical and Computer Engineering
# McMaster University
# Ontario, Canada
#

import matplotlib.pyplot as plt
import numpy as np
from scipy.io import wavfile
from scipy import signal
import sys
from fourierTransform import DFT, IDFT
from filterDesign import lowpass

def filter_single_pass(audio_data, filter_coeff):

	# we assume the data is stereo as in the audio test file
	filtered_data = np.empty(shape = audio_data.shape)
	# filter left channel
	filtered_data[:,0] = signal.lfilter(filter_coeff, 1.0, audio_data[:,0])
	# filter stereo channel
	filtered_data[:,1] = signal.lfilter(filter_coeff, 1.0, audio_data[:,1])

	return filtered_data

def filter_block_processing(audio_data, filter_coeff, block_size):

	# we assume the data is stereo as in the audio test file
	filtered_data = np.empty(shape = audio_data.shape)

	# start at the first block (with relative position zero)
	position = 0

	# intiial filter state - state is the size of the impulse response minus 1
	# we need two channels for the state (one for each audio channel)
	filter_state = np.zeros(shape = (len(filter_coeff)-1, 2))

	while True:

		# filter both left and right channels
		filtered_data[position:position+block_size, 0], filter_state[:,0] = \
			signal.lfilter(filter_coeff, 1.0, \
			audio_data[position:position+block_size, 0], zi = filter_state[:,0])
		# the filter state has been saved only for the first channel above
		# you will need to adjust the code for the second channel below
		filtered_data[position:position+block_size, 1], filter_state[:,1] = \
			signal.lfilter(filter_coeff, 1.0, \
			audio_data[position:position+block_size, 1], zi = filter_state[:,1])

		position += block_size

		# the last incomplete block is ignored
		if position > len(audio_data):
			break

	return filtered_data
def ourFilter(x,h):
    #return IDFT(DFT(x)*DFT(h)).real

	# pad the shorter signal with zeros
	if len(x) > len(h):
		h = np.append(h, np.zeros(len(x)-len(h)))
	else:
		x = np.append(x, np.zeros(len(h)-len(x)))
		
	return np.fft.ifft(np.fft.fft(x)*np.fft.fft(h))

def our_block_processing(audio_data, filter_coeff, block_size):

	# we assume the data is stereo as in the audio test file
	filtered_data = np.empty(shape = audio_data.shape)

	# start at the first block (with relative position zero)
	position = 0

	# intiial filter state - state is the size of the impulse response minus 1
	# we need two channels for the state (one for each audio channel)
	filter_state = np.zeros(shape = (len(filter_coeff)-1, 2))

	offset = 0

	while True:

		# filter both left and right channels
		filtered_data[position:position+block_size, 0]= \
			ourFilter(filter_coeff,audio_data[position-offset:position+block_size, 0])[offset:]
		# the filter state has been saved only for the first channel above
		# you will need to adjust the code for the second channel below
		filtered_data[position:position+block_size, 1]= \
			ourFilter(filter_coeff,audio_data[position-offset:position+block_size, 1])[offset:]
		
		offset = int(block_size/2)

		position += block_size
		print(position)

		# the last incomplete block is ignored
		if position + block_size > len(audio_data):
			break

	return filtered_data

def cli_error_msg():

	# error message to provide the correct command line interface (CLI) arguments
	print('Valid arguments:')
	print('\trc:  reference code')
	print('\til1: in-lab 1')
	print('\tth:  take-home')
	sys.exit()

if __name__ == "__main__":

	if len(sys.argv[0:]) != 2:
		cli_error_msg()

	# audio test file from: https://freesound.org/people/jeremy80/sounds/230639/
	# use use wavfile from scipy.io for handling .wav files

	print('Opening audio file (.wav format)')
	audio_Fs, audio_data = wavfile.read("../data/audio_test.wav")
	print(' Audio sample rate = {0:d} \
		\n Number of channels = {1:d} \
		\n Numbef of samples = {2:d}' \
		.format(int(audio_Fs), audio_data.ndim, len(audio_data)))

	if (sys.argv[1] == 'rc'): # runs the reference code (rc)

		print('Reference code for processing streams divided in blocks')

		# derive filter coefficients (you can control the cutoff frequency and number of taps)
		N_taps = 51
		audio_Fc = 10e3
		firwin_coeff = signal.firwin(N_taps, audio_Fc/(audio_Fs/2), window=('hann'))

		single_pass_data = filter_single_pass(audio_data, filter_coeff = firwin_coeff)

		# write filtered data back to a .wav file
		wavfile.write("../data/single_pass_filtered.wav", \
					audio_Fs, \
					single_pass_data.astype(np.int16))

	elif (sys.argv[1] == 'il1'):

		print('In-lab experiment 1 for processing streams divided in blocks')

		# derive filter coefficients (you can control the cutoff frequency and number of taps)
		N_taps = 50
		audio_Fc = 10e3
		firwin_coeff = signal.firwin(N_taps, audio_Fc/(audio_Fs/2), window=('hann'))

		# you can also the block size
		block_processing_data = filter_block_processing(audio_data, filter_coeff = firwin_coeff, block_size = 1000)

		wavfile.write("../data/block_processing_filtered.wav", \
					audio_Fs, \
					block_processing_data.astype(np.int16))

	elif (sys.argv[1] == 'th'):

		print('Take-home exercise for processing streams divided in blocks')
		N_taps = 50
		audio_Fc = 10e3
		firwin_coeff = lowpass(N_taps, audio_Fc, audio_Fs)
		block_processing_data = our_block_processing(audio_data, filter_coeff = firwin_coeff, block_size = 1000)
		# for specific details check the lab document
        
		# it is suggested that you add plotting while troubleshooting
		# if you plot in the time domain, select a subset of samples,
		# from a particular channel (or both channels) e.g.,
		# audio_data[start:start+number_of_samples, 0]
		wavfile.write("../data/our_block_processing_filtered.wav", \
					audio_Fs, \
					block_processing_data.astype(np.int16))

	else:

		cli_error_msg()

	# plt.show()
