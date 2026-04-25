@tool
extends Component
class_name MapComponent

@export var gridSize:float
@export var wallPrefab:String
@export var mapWidth:int
@export var mapHeight:int

func get_component() -> Dictionary[String, Variant]:
	return {
		"gridSize" : FormatWriter.format(gridSize, "gridSize"),
		"wallPrefab" : FormatWriter.format(wallPrefab, "wallPrefab"),
		"mapWidth" : FormatWriter.format(mapWidth, "mapWidth"),
		"mapHeight" : FormatWriter.format(mapHeight, "mapHeight"),
	}
