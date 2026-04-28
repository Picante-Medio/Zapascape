@tool
extends CollisionShape3D
class_name ColliderComponent

@export var restitution:float
@export var friction:float
@export var positionOffset:Vector3
@export var rotationOffset:Quaternion
@export var active:bool
@export var useGizmo:bool
@export var info:ENUMS.ColliderType

func get_component() -> Dictionary[String, Variant]:
	var component_data:Dictionary[String, Variant]
	
	component_data.get_or_add("info", FormatWriter.format(info, "info"))
	if shape is BoxShape3D:
		var box:BoxShape3D = shape as BoxShape3D
		component_data.get_or_add("halfExtents", FormatWriter.format(box.size, "halfExtents"))
	elif shape is SphereShape3D:
		var sphere:SphereShape3D = shape as SphereShape3D
		component_data.get_or_add("radius", FormatWriter.format(sphere.radius, "radius"))
	elif shape is CapsuleShape3D:
		var capsule:CapsuleShape3D = shape as CapsuleShape3D
		component_data.get_or_add("radius", FormatWriter.format(capsule.radius, "radius"))
		component_data.get_or_add("height", FormatWriter.format(capsule.height, "height"))
	elif shape is CylinderShape3D:
		var cylinder:CylinderShape3D = shape as CylinderShape3D
		component_data.get_or_add("radius", FormatWriter.format(cylinder.radius, "radius"))
		component_data.get_or_add("height", FormatWriter.format(cylinder.height, "height"))
		
	component_data.get_or_add("restitution", FormatWriter.format(restitution, "restitution"))
	component_data.get_or_add("friction", FormatWriter.format(friction, "friction"))
	component_data.get_or_add("positionOffset", FormatWriter.format(positionOffset, "positionOffset"))
	component_data.get_or_add("rotationOffset", FormatWriter.format(rotationOffset, "rotationOffset"))
	component_data.get_or_add("active", FormatWriter.format(active, "active"))
	component_data.get_or_add("useGizmo", FormatWriter.format(useGizmo, "useGizmo"))
	
	return component_data
