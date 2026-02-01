# load soccer specific material
# run "scripts/rcs-materials.rb"
run "scripts/rcs-materials-textures.rb"

# prepare the soccer monitor
importBundle 'soccermonitor'

# register the soccer monitor plugin to parse the soccer game
# state
sparkRegisterCustomMonitor 'SoccerMonitor'

if ($logPlayerMode == true)
  # register the soccer input logplayer plugin for playback
  # specific keys
  sparkRegisterCustomInput 'SoccerInputLogPlayer'
else
  # register the soccer input plugin to process soccer specific key
  # presses
  sparkRegisterCustomInput 'SoccerInput'
end
  # bind keys to soccer commands
  run "soccerbindings.rb"

# register the soccer render plugin to draw the game state on the
# screen
sparkRegisterCustomRender  'SoccerRender'

