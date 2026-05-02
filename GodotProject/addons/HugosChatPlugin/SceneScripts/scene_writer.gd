@tool
extends EditorScript
class_name SceneWriter

func _run() -> void:
	write()

static func write():
	if EditorInterface.get_edited_scene_root() is not SCENE_TRACKER: return
	var root:SCENE_TRACKER = EditorInterface.get_edited_scene_root()
	if root == null:
		push_error("No edited scene root found.")
		return
	
	root.track_entities()
	
	var scene_name := root.name
	
	var global_path:String = ProjectSettings.globalize_path("res://")
	var project_path:String = global_path.path_join("../".path_join(HugoConfig.CHATS_PATH))
	
	var file_path := project_path.path_join("%s.chat" % scene_name)
	
	var file := FileAccess.open(file_path, FileAccess.WRITE)
	if file == null:
		push_error("Could not open file for writing: %s" % file_path)
		return
	
	if root is not Entity:
		var scene_text = "SCENE "
		scene_text = scene_text + root.name
		
		file.store_line(scene_text)
		for system:SYSTEMS.system in root.systems:
			var system_name:String = SYSTEMS.system.keys()[system]
			file.store_line("SYSTEM " + system_name)
		file.store_line("")
		
		for entity in root.entities:
			write_entity(entity, file, "")
	else:
		var scene_text = "PREFAB "
		scene_text = scene_text + root.name
		
		file.store_line(scene_text)
		
		for entity in root.entities:
			write_entity(entity, file, "")
	
	
	file.close()
	print("Written to: ", file_path)

static func write_entity(entity:EntityData, file, tabulations:String):
	file.store_line(tabulations+"ENTITY " + entity.entityName + " 1")
	#Component writing
	for component_key in entity.components.keys():
		#Map of key-values
		var component_data: Dictionary = entity.components[component_key]
		file.store_line(tabulations + "    COMPONENT " + str(component_key))
		for parameter_key in component_data.keys():
			var value = component_data[parameter_key]
			file.store_line(tabulations+ "        "+str(value))
	#Children writing
	file.store_line(tabulations + "    START_CHILDREN")
	for child in entity.children:
		write_entity(child, file, str(tabulations+"    "))
	file.store_line(tabulations + "    END_CHILDREN")
	
	file.store_line(tabulations + "END_ENTITY")
	file.store_line("")
	return
