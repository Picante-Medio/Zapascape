@tool
extends Component
class_name MapComponent

@export var gridSize:float = 1.0
@export var numberOfRooms:int = 1
@export var wallPrefab:String
@export var mapWidth:int
@export var mapHeight:int

func get_component() -> Dictionary[String, Variant]:
	return {
		"gridSize" : FormatWriter.format(gridSize, "gridSize"),
		"numberOfRooms" : FormatWriter.format(numberOfRooms, "numberOfRooms"),
		"wallPrefab" : FormatWriter.format(wallPrefab, "wallPrefab"),
		"mapWidth" : FormatWriter.format(mapWidth, "mapWidth"),
		"mapHeight" : FormatWriter.format(mapHeight, "mapHeight"),
	}
