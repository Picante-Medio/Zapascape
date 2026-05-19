@tool
extends Component
class_name AudioComponent

@export var audioName:String
@export var volume:float
@export var loop:bool
@export var pitch:float
@export var isPlaying:bool

func get_component() -> Dictionary[String, Variant]:
	return {
		"audioName" : FormatWriter.format(audioName, "audioName"),
		"volume" : FormatWriter.format(volume, "volume"),
		"loop" : FormatWriter.format(loop, "loop"),
		"pitch" : FormatWriter.format(pitch, "pitch"),
		"isPlaying" : FormatWriter.format(isPlaying, "isPlaying"),
	}
