# CapCap
Networked wearable captioning devices for offline and online use cases with portability in mind.

CapCap (Captioning Cap) is a ground-breaking initiative driven by a dedicated team of Deaf creative problem solvers from the non-profit organization CymaSpace. This project stands out by providing an accessible, affordable, and open-source solution to enhance communication for individuals who are Deaf or hard-of-hearing.

Key to CapCap's innovation is its commitment to reducing the economic barriers that often limit access to captioning technology. By leveraging open-source hardware and software and incorporating a permissive open source license, the project aims to bring down the cost of the necessary hardware from the typical thousands of dollars to several hundred. This opens up opportunities for a wider range of individuals and organizations to access and benefit from this crucial communication tool.

In addition to its affordability, CapCap offers a distinctive approach to communication accessibility with its open captioning design. Rather than restricting captions to personal devices or AR glasses where only the user can view them, CapCap uses badges and captioning wearables that make the captions visible to everyone. This creates an inclusive and interactive environment, encouraging everyone to be mindful of their speech clarity, turn-taking, and the overall auditory environment to improve the accuracy of captions.

The technical elements of CapCap further support its mission of inclusivity and accessibility. The project utilizes OpenAI's Whisper AI captioning model, known for its offline capability, to convert spoken language into captions in real-time. This will aleviate common privacy concerns that conflict with the mass of adoption of captioning in public spaces since no audio data or transcripts are stored or sent over the internet. This model is integrated into wearable, battery-powered badges that can work on affordable SoC boards, specifically ESP32 microcontrollers for the wearable badges with microphones and a captioning server running on a Raspberry Pi.

Moreover, the CapCap system is designed to enable offline synchronization of captions and accurate speaker identification, ensuring a seamless communication experience. With a strong emphasis on user-friendliness and accessibility, these badges are designed with the unique needs of the Deaf and hard-of-hearing community in mind.

In conclusion, CapCap's approach, which balances affordability, inclusivity, and advanced technology, promises a major shift in the landscape of communication for individuals who are Deaf or hard-of-hearing. By leading with its key benefits, the project provides a solution that empowers more individuals and communities to foster inclusive communication environments.

### Resources
[1] https://betterprogramming.pub/color-your-captions-streamlining-live-transcriptions-with-diart-and-openais-whisper-6203350234ef -- python implementation of whisper AI plus real-time (padding 30 secs minimum with silence), colorization of speaker
[2] 

### TODO
[ ] 23-05-29 Create python script to convert Audio Tools Library Audio from ESP32 -> diart_whisper_mqtt.python
[ ] Add | before each line with color coding provided from Diart
[ ] Modify Lilypad T-Display S3 Case to include lipo battery, i2s mic & magnet/clip attachment.
[ ] Make Caption text bigger (3-4 lines & 30 characters max) on Lilypad ESP32
[x] MQTT -> ESP32 line by line captions will display 10 lines including most recent line.
[x] 23-05-23 Resolve ./stream -- unable to open audio capture device on ODROID1.
[x] 23-05-22 Add whisper.cpp & tiny-en model [4]
* Tested to work using ```arecord -D hw:1,0 -f S24_3LE -r 44100 -c 2 --duration=10 test.wav``` 
* blocked by ```./stream -m models/ggml-tiny.en.bin --step 4000 --length 8000 -t 4 -ac 512```. Issue: Unable to open audio device for capture.

## Dev Log

23-05-30 
* Implemention of Diart -> WhisperAI-Timestamped -> MQTT [1]
* Ell got ESP32 with Audio Tools Library to stream to CymaProd
* Printed Lilypad ESP32 case for test fit.


Progress update Day #2: I now have Whisper AI model running on an ODROID N2 (beefier RPI alternative) with Ubuntu minimal installed. Verified it was able to caption my test recording test.wav (recording by connecting the Blue Yeti X USB mic to the ODROID1 and recorded using:
```arecord -D hw:1,0 -f S24_3LE -r 44100 -c 2 --duration=10 test.wav```

here is the output:

```
root@odroid:~/whisper.cpp# ./main -m models/ggml-tiny.en.bin -f test16bit.wav
whisper_init_from_file_no_state: loading model from 'models/ggml-tiny.en.bin'
whisper_model_load: loading model
whisper_model_load: n_vocab       = 51864
whisper_model_load: n_audio_ctx   = 1500
whisper_model_load: n_audio_state = 384
whisper_model_load: n_audio_head  = 6
whisper_model_load: n_audio_layer = 4
whisper_model_load: n_text_ctx    = 448
whisper_model_load: n_text_state  = 384
whisper_model_load: n_text_head   = 6
whisper_model_load: n_text_layer  = 4
whisper_model_load: n_mels        = 80
whisper_model_load: ftype         = 1
whisper_model_load: qntvr         = 0
whisper_model_load: type          = 1
whisper_model_load: mem required  =  201.00 MB (+    3.00 MB per decoder)
whisper_model_load: adding 1607 extra tokens
whisper_model_load: model ctx     =   73.62 MB
whisper_model_load: model size    =   73.54 MB
whisper_init_state: kv self size  =    2.62 MB
whisper_init_state: kv cross size =    8.79 MB

system_info: n_threads = 4 / 6 | AVX = 0 | AVX2 = 0 | AVX512 = 0 | FMA = 0 | NEON = 1 | ARM_FMA = 1 | F16C = 0 | FP16_VA = 0 | WASM_SIMD = 0 | BLAS = 0 | SSE3 = 0 | VSX = 0 | COREML = 0 |

main: processing 'test16bit.wav' (160000 samples, 10.0 sec), 4 threads, 1 processors, lang = en, task = transcribe, timestamps = 1 ...


[00:00:00.000 --> 00:00:06.640]   Hello, what are the testing and the captions? My name is Miles de Bastion and we are
[00:00:06.640 --> 00:00:09.440]   at the nonprofit side of this in Portland, Oregon.


whisper_print_timings:     load time =   138.24 ms
whisper_print_timings:     fallbacks =   0 p /   0 h
whisper_print_timings:      mel time =   461.59 ms
whisper_print_timings:   sample time =    46.42 ms /    36 runs (    1.29 ms per run)
whisper_print_timings:   encode time =  4615.14 ms /     1 runs ( 4615.14 ms per run)
whisper_print_timings:   decode time =   619.16 ms /    36 runs (   17.20 ms per run)
whisper_print_timings:    total time =  5918.57 ms
root@odroid:~/whisper.cpp#
```

It took ~6 seconds to transcribe a 10 second file so we have better than real-time performance with the tiny model on the odroid1.