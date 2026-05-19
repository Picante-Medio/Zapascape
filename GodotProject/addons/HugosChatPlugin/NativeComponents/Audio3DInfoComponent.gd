@tool
extends Component
class_name Audio3DInfoComponent

@export var minDistance:float
@export var maxDistance:float
@export var rolloff:float

func get_component() -> Dictionary[String, Variant]:
	return {
		"minDistance" : FormatWriter.format(minDistance, "minDistance"),
		"maxDistance" : FormatWriter.format(maxDistance, "maxDistance"),
		"rolloff" : FormatWriter.format(rolloff, "rolloff")
	}
