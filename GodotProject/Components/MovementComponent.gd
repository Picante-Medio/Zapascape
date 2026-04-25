@tool
extends Component
class_name MovementComponent

@export var speed:float = 1.0

func get_component() -> Dictionary[String, Variant]:
	return {
		"speed" : FormatWriter.format(speed, "speed"),
	}
