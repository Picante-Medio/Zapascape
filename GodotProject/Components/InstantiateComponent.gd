@tool
extends Component
class_name InstantiateComponent

@export var prefabName:String

func get_component() -> Dictionary[String, Variant]:
	return {
		"prefabName" : FormatWriter.format(prefabName, "prefabName"),
	}
