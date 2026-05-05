@tool
extends CollisionShape3D
class_name ColliderComponent

@export var restitution:float
@export var friction:float
@export var active:bool
@export var useGizmo:bool
@export var info:Shape3D

func get_component() -> Dictionary[String, Variant]:
	var component_data:Dictionary[String, Variant]
	
	component_data.get_or_add("info", FormatWriter.format(info, "info"))
	
	component_data.get_or_add("restitution", FormatWriter.format(restitution, "restitution"))
	component_data.get_or_add("friction", FormatWriter.format(friction, "friction"))
	component_data.get_or_add("positionOffset", FormatWriter.format(position, "positionOffset"))
	component_data.get_or_add("rotationOffset", FormatWriter.format(basis.get_rotation_quaternion(), "rotationOffset"))
	component_data.get_or_add("active", FormatWriter.format(active, "active"))
	component_data.get_or_add("useGizmo", FormatWriter.format(useGizmo, "useGizmo"))
	
	return component_data
