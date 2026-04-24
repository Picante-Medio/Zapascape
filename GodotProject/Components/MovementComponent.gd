@tool
extends Component
class_name MovementComponent

@export var speed:float

func get_component() -> Dictionary[String, Variant]:
	return {
		"speed" : FormatWriter.format(speed, "speed"),
	}
