# sw
Simple command line *s*top*w*atch

## build
build it with make
```sh
make
```
## install
```sh
sudo make install
```

## run
```sh
./sw -p 10000 -s 20 # set precision of 10000 microseconds and 20 entries for recording time
```

## commands
While running you can use the following commands:
- `S` or `s` to stop / start the stopwatch
- `R` or `r` to reset the stopwatch
- `<space>` to record time
- `Q` or `q` to quit
