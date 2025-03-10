# Since we are now supporting specific platforms we need a
# solution to support key bindings for multiple devices.

# cycle_machine
bind_default CTRL+PAGEUP cycle_machine
bind_default CTRL+PAGEDOWN cycle_back_machine

# reverse
#  note: you can't use bindings with modifiers like SHIFT, because they
#        will already stop the replay, as they are MSX keys as well
bind_default PAGEUP   -msx -repeat "go_back_one_step"
bind_default PAGEDOWN -msx -repeat "go_forward_one_step"

# savestate
if {$tcl_platform(os) eq "Darwin"} {
	bind_default META+S savestate
	bind_default META+R loadstate
} else {
	bind_default ALT+F8 savestate
	bind_default ALT+F7 loadstate
}

# vdrive
bind_default       ALT+F9  "vdrive diska"
bind_default SHIFT+ALT+F9  "vdrive diska -1"
bind_default       ALT+F10 "vdrive diskb"
bind_default SHIFT+ALT+F10 "vdrive diskb -1"

# copy/paste (use middle-click for all platforms and also something similar to
# CTRL-C/CTRL-V, but not exactly that, as these combinations are also used on
# MSX. By adding META, the combination will be so rarely used that we can
# assume it's OK).
bind_default "mouse button2 down" type_clipboard
if {$tcl_platform(os) eq "Darwin"} { ;# Mac
	bind_default -msx "keyb META+C" copy_screen_to_clipboard
	bind_default -msx "keyb META+V" type_clipboard
} else { ;# any other
	bind_default -msx "keyb META+CTRL+C" copy_screen_to_clipboard
	bind_default -msx "keyb META+CTRL+V" type_clipboard
}
