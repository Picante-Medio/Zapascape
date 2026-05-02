@tool
extends Component
class_name TimeComponent

@export var remainingGameTime:int = 0

func get_component() -> Dictionary[String, Variant]:
	return {
		"remainingGameTime" : FormatWriter.format(remainingGameTime, "remainingGameTime"),
	}
