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
	
	root.create_entities()
	var entity_map:Dictionary[String, int] = root.entities_map
	
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
	else:
		var scene_text = "PREFAB "
		scene_text = scene_text + root.name
		
		file.store_line(scene_text)
	
	var parents:Array[String]
	var current_offset:int = 0
	var o:String
	
	for entity_key in root.entities.keys():
		var entity: Dictionary = root.entities[entity_key]
		
		var entity_cache:String = (o + "ENTITY " + str(entity_key) + " " + str(entity_map[entity_key]) + "\n")
		var has_parent_component:bool = false
		for component_key in entity.keys():
			var component: Dictionary = entity[component_key]
			if component_key == "ParentComponent":
				has_parent_component = true
				
				var parent = component["parent"]
				if parents.has(parent):
					if parents.back() != parent:
						while(not parents.is_empty() and parent != parents.pop_back()):
							current_offset -= 1
							o = get_offset_string(current_offset)
							entity_cache += (o + "END_CHILDREN\n")
				else:
					parents.push_back(parent)
					file.store_line(o + "START_CHILDREN\n")
					current_offset += 1
					o = get_offset_string(current_offset)
					
			else:
				entity_cache += (o + "\tCOMPONENT " + str(component_key) + "\n")
			
			for parameter_key in component.keys():
				var value = component[parameter_key]
				entity_cache += (o + "\t\t%s" % [str(value)] + "\n")
		
		if not has_parent_component:
			for parent in parents:
				file.store_line(o + "END_CHILDREN")
				current_offset -= 1
			o = get_offset_string(current_offset)
			parents.clear()
		entity_cache += "ENDTITY\n"
		
		file.store_string(entity_cache)	
		file.store_line("")
	
	for parent in parents:
		file.store_line(o + "END_CHILDREN")
		current_offset -= 1
	parents.clear()
	
	file.close()
	print("Written to: ", file_path)

static func get_offset_string(count:int):
	var offset:String = ""
	for i in range(0, count):
		offset += ""
	return offset
