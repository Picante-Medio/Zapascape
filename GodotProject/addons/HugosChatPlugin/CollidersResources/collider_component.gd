extends CollisionShape3D

func get_component() -> Dictionary[String, Variant]:
	if shape is BoxShape3D:
		var box:BoxShape3D = shape as BoxShape3D
		return {
			"info" : FormatWriter.format(box.size, "info")
		}
	elif shape is SphereShape3D:
		var sphere:SphereShape3D = shape as SphereShape3D
		return {
			"radius" : FormatWriter.format(sphere.radius, "radius")
		}
	return {}
