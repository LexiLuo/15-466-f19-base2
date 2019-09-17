
#from https://stackoverflow.com/questions/714100/os-detecting-makefile
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OPUSENC = ../../nest-libs/linux/opus-tools/bin/opusenc
else
	OPUSENC = ../../nest-libs/macos/opus-tools/bin/opusenc
#endif
#../dist/wow.opus : cook-done.wav
#	$(OPUSENC) --vbr --bitrate 128 cook-done.wav ../dist/cook-done.opus
endif
#../dist/wow.opus : wow.wav
#	$(OPUSENC) --vbr --bitrate 128 wow.wav ../dist/wow.opus
#../dist/cook-done.opus : cook-done.wav
#	$(OPUSENC) --vbr --bitrate 128 cook-done.wav ../dist/cook-done.opus
#../dist/oh-no.opus : oh-no.wav
#	$(OPUSENC) --vbr --bitrate 128 oh-no.wav ../dist/oh-no.opus
../dist/water-cook.opus : water-cook.wav
	$(OPUSENC) --vbr --bitrate 128 water-cook.wav ../dist/water-cook.opus
#endif
#../dist/teafortwo-background.opus : teafortwo-background.wav
#	$(OPUSENC) --vbr --bitrate 128 teafortwo-background.wav ../dist/teafortwo-background.opus

