#
# internalsoccermonitor.rb
#
mfilename = "internalsoccermonitor.rb"
mprefix = "("+mfilename+") "

logNormal(mprefix + "Setting up soccer HUD rendering.")

# create the font server (if not present)
sparkGetFontServer()

# register the internal soccer render plugin to draw the game state on the
# screen
sparkRegisterCustomRender 'InternalSoccerRender'

# register the internal soccer input plugin to process soccer specific key
# presses
sparkRegisterCustomInput 'InternalSoccerInput'

# bind keys to soccer commands
run "internalsoccerbindings.rb"
