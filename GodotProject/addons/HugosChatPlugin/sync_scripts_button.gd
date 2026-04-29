@tool
extends Button
class_name SyncScriptsButton


func _pressed() -> void:
	ScriptsSyncronizer.sync()
