import socket
import struct
import sys

UDP_SYNC_HEADER = b"WLED\0"
UDP_SYNC_PORT = 11988

class WLEDSync:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind(("192.168.4.207", UDP_SYNC_PORT))
    
    def read(self):
        data, _ = self.socket.recvfrom(1024)
        if len(data) > 5 and data[:6] == UDP_SYNC_HEADER:
            self.decodeAudioData(data[6:])
            return True
        return False
    
    def decodeAudioData(self, audioData):
        audioSyncPacketFormat = struct.Struct("<fffffff?")
        receivedPacket = audioSyncPacketFormat.unpack(audioData)
        
        # Extract the received audio data values
        volumeSmth, volumeRaw, sampleRaw, sampleAvg, sampleAgc, rawSampleAgc, multAgc, samplePeak = receivedPacket[:8]
        fftResult = receivedPacket[8:8+NUM_GEQ_CHANNELS]
        my_magnitude, FFT_Magnitude, FFT_MajorPeak = receivedPacket[8+NUM_GEQ_CHANNELS:]

        # Process the received audio data as needed
        print("Received audio data:")
        print(f"volumeSmth: {volumeSmth}")
        print(f"volumeRaw: {volumeRaw}")
        # ...

# Example usage
sync = WLEDSync()

try:
    print("Script is listening for audio data. Press Ctrl+C to exit.")
    while True:
        if sync.read():
            # Audio data received, process it
            print("Audio data received!")
            # ...
except KeyboardInterrupt:
    print("Exiting the script...")
    sys.exit(0)
