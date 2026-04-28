@tool
extends Component
class_name MovementRBComponent

@export var force:float = 25.0

func get_component() -> Dictionary[String, Variant]:
	return {
		"force" : FormatWriter.format(force, "force"),
	}
