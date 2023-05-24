import socket
import struct
import sys

UDP_SYNC_HEADER_V1 = b"00001\0"  # Include the trailing null character
UDP_SYNC_HEADER_V2 = b"00002\0"  # Include the trailing null character
UDP_SYNC_PORT = 11988

class WLEDSync:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind(("0.0.0.0", UDP_SYNC_PORT))
        self.socket.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, socket.inet_aton("239.0.0.1") + socket.inet_aton("0.0.0.0"))
    
    def read(self):
        data, _ = self.socket.recvfrom(1024)
        if len(data) > 6:  # The minimum length should be 6 to accommodate the header
            header = data[:6]  # The header length is 6, including the null character
            if header == UDP_SYNC_HEADER_V1:
                self.decodeAudioDataV1(data[6:])
                return True
            elif header == UDP_SYNC_HEADER_V2:
                self.decodeAudioDataV2(data[6:])
                return True
        return False
    
    def decodeAudioDataV1(self, audioData):
        audioSyncPacketFormat = struct.Struct("<32BiiifB16Bdd")  # Adjusted format to match documentation
        receivedPacket = audioSyncPacketFormat.unpack(audioData)
        
        # Extract the received audio data values
        myVals = receivedPacket[:32]
        sampleAgc, sampleRaw, sampleAvg, samplePeak = receivedPacket[32:36]
        fftResult = receivedPacket[36:52]
        FFT_Magnitude, FFT_MajorPeak = receivedPacket[52:54]

        print("Received audio data (V1 format):")
        print(f'myVals: {myVals}')
        print(f'sampleAgc: {sampleAgc}, sampleRaw: {sampleRaw}, sampleAvg: {sampleAvg}, samplePeak: {samplePeak}')
        print(f'fftResult: {fftResult}')
        print(f'FFT_Magnitude: {FFT_Magnitude}, FFT_MajorPeak: {FFT_MajorPeak}')

    def decodeAudioDataV2(self, audioData):
        audioSyncPacketFormat = struct.Struct("<ffBB16Bff")  # Adjusted format to match documentation
        receivedPacket = audioSyncPacketFormat.unpack(audioData)
        
        # Extract the received audio data values
        sampleRaw, sampleSmth, samplePeak, reserved1 = receivedPacket[:4]
        fftResult = receivedPacket[4:20]
        FFT_Magnitude, FFT_MajorPeak = receivedPacket[20:22]

        print("Received audio data (V2 format):")
        print(f'sampleRaw: {sampleRaw}, sampleSmth: {sampleSmth}, samplePeak: {samplePeak}, reserved1: {reserved1}')
        print(f'fftResult: {fftResult}')
        print(f'FFT_Magnitude: {FFT_Magnitude}, FFT_MajorPeak: {FFT_MajorPeak}')

# Example usage
sync = WLEDSync()

try:
    print("Script is listening for audio data. Press Ctrl+C to exit.")
    while True:
        if sync.read():
            print("Audio data received!")
except KeyboardInterrupt:
    print("Exiting the script...")
    sys.exit(0)

