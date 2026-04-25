@tool
extends Control
class_name HugoConfig

static var COMPONENTS_PATH:String
static var SYSTEMS_PATH:String
static var CHATS_PATH:String
static var LUA_PATH:String

func _ready() -> void:
	var file := FileAccess.open("res://hugo_editor.config", FileAccess.READ)
	COMPONENTS_PATH = file.get_line()
	SYSTEMS_PATH = file.get_line()
	CHATS_PATH = file.get_line()
