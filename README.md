# Audio

An Appeal module project.

## Waiting for music to finish

`Audio Is_Playing` returns `1` while music is playing and `0` when it has ended,
has been stopped, or has not started. Sound effects do not affect this result.
The SDL2 plugin queries SDL_mixer on each call:

```text
Audio Play_Song "Do Not Despair"
Loop While ( Audio Is_Playing ) == 1 {
    Delay 1
}
Done
```

Use the function in the condition to query fresh status each iteration. The
command also stores its result in `Audio.Is_Playing`, but that global is only a
snapshot, not a continuously refreshed value. SDL2 treats paused music as playing.
Looping music remains playing until stopped.

## Development status

This repository was separated from the Appeal beta development tree. Its build and installer configuration will be modernized before the first standalone release.

Generated binaries and intermediate objects are intentionally excluded from version control.

