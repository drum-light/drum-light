# README

## Goal for Functionality
- parse input from drum head
- output various patterns on the LEDs
    - pattern selected based on an input (?)
        - a button that cycles through
        - from a bluetooth message from RPi
        - potentiometer/dial
        - switches (one per channel? an array that corresponds to bit pattern that controls all channels)
- add control for piano LEDs

## MIDI Notes
- https://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/
- each message has command and data bytes
- data byte = 1B = 8b
- command byte:
    ```
    10000000 = note off
    10010000 = note on
    10100000 = aftertouch
    10110000 = continuous controller
    11000000 = patch change
    11010000 = channel pressure
    11100000 = pitch bend
    11110000 = non-musical commands
    ```
- last 4 bits set the channel
- MIDI baud rate = 31250
- serial connection
