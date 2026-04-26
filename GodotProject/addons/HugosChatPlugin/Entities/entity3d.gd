@tool
extends Entity
class_name Entity3D

func get_component() -> Dictionary[String, Variant]:
	return {
		"worldPosition" : FormatWriter.format(global_position, "worldPosition"),
		"worldRotation" : FormatWriter.format(global_basis.get_rotation_quaternion(), "worldRotation"),
		"worldScale" : FormatWriter.format(scale, "worldScale")
	}
