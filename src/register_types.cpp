#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "testing/test_case.h"
#include "testing/test_suite.h"

#include "example_test_suite.h"

using namespace godot;

void initialize_gdext_testing_module(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
        return;

    GDREGISTER_ABSTRACT_CLASS(Testing::TestCase);
    GDREGISTER_CLASS(Testing::TestSuite);

    GDREGISTER_CLASS(ExampleTestSuite);
}

void uninitialize_gdext_testing_module(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
        return;
}

extern "C"
{
    GDExtensionBool GDE_EXPORT gdext_testing_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_gdext_testing_module);
        init_obj.register_terminator(uninitialize_gdext_testing_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
