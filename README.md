# CapCap
Networked wearable captioning devices for offline and online use cases with portability in mind.

### TODO
[] Resolve ./stream -- unable to open audio capture device on ODROID1.
[x] 23-05-22 Add whisper.cpp & tiny-en model [4]
* Tested to work using ```arecord -D hw:1,0 -f S24_3LE -r 44100 -c 2 --duration=10 test.wav``` 
* blocked by ```./stream -m models/ggml-tiny.en.bin --step 4000 --length 8000 -t 4 -ac 512```. Issue: Unable to open audio device for capture.

## Dev Log

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