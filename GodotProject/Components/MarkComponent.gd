@tool
extends Component
class_name MarkComponent

@export var maxNum:int = 10

func get_component() -> Dictionary[String, Variant]:
	return {
		"maxNum" : FormatWriter.format(maxNum, "maxNum"),
	}
