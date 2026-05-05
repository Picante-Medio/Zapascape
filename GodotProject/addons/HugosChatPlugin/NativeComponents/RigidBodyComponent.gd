@tool
extends Component
class_name RigidBodyComponent

@export var mass:float
@export var linearDamping:float
@export var angularDamping:float
@export var linearVelocity:Vector3
@export var angularVelocity:Vector3
@export var gravity:Vector3
@export var linearSleepingThreshold:float
@export var angularSleepingThreshold:float
@export var type:ENUMS.BodyType
@export var dontRotateWithPhysicsOnDynamic:bool

func get_component() -> Dictionary[String, Variant]:
	return {
		"mass" : FormatWriter.format(mass, "mass"),
		"linearDamping" : FormatWriter.format(linearDamping, "linearDamping"),
		"angularDamping" : FormatWriter.format(angularDamping, "angularDamping"),
		"linearVelocity" : FormatWriter.format(linearVelocity, "linearVelocity"),
		"angularVelocity" : FormatWriter.format(angularVelocity, "angularVelocity"),
		"gravity" : FormatWriter.format(gravity, "gravity"),
		"linearSleepingThreshold" : FormatWriter.format(linearSleepingThreshold, "linearSleepingThreshold"),
		"angularSleepingThreshold" : FormatWriter.format(angularSleepingThreshold, "angularSleepingThreshold"),
		"type" : FormatWriter.format(type, "type"),
		"dontRotateWithPhysicsOnDynamic" : FormatWriter.format(dontRotateWithPhysicsOnDynamic, "dontRotateWithPhysicsOnDynamic")
	}
