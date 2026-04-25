@tool
extends Component
class_name CameraComponent

@export var projectionType:ENUMS.ProjectionType = ENUMS.ProjectionType.Perspective
@export var fovY:float = 60.0
@export var nearClip:float = 1.0
@export var farClip:float = 10000.0
@export var orthoWidth:float = 10.
@export var orthoHeight:float = 10.
@export var autoAspectRatio:bool = true
@export var mainCamera:bool = false
@export var clearColor:Vector4 = Vector4(0.3, 0.3, 0.5, 1.0)

func get_component() -> Dictionary[String, Variant]:
	return {
		"projectionType" : FormatWriter.format(projectionType, "projectionType"),
		"fovY" : FormatWriter.format(fovY, "fovY"),
		"nearClip" : FormatWriter.format(nearClip, "nearClip"),
		"farClip" : FormatWriter.format(farClip, "farClip"),
		"orthoWidth" : FormatWriter.format(orthoWidth, "orthoWidth"),
		"orthoHeight" : FormatWriter.format(orthoHeight, "orthoHeight"),
		"autoAspectRatio" : FormatWriter.format(autoAspectRatio, "autoAspectRatio"),
		"mainCamera" : FormatWriter.format(mainCamera, "mainCamera"),
		"clearColor" : FormatWriter.format(clearColor, "clearColor"),
	}
