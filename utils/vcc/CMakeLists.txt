add_executable(vcc
	../../source/common.h
	../../source/vc_class.cpp
	../../source/vc_class.h
	../../source/vc_constant.cpp
	../../source/vc_constant.h
	../../source/vc_emit_context.cpp
	../../source/vc_emit_context.h
	../../source/vc_error.cpp
	../../source/vc_error.h
	../../source/vc_expr_array.cpp
	../../source/vc_expr_array.h
	../../source/vc_expr_assign.cpp
	../../source/vc_expr_assign.h
	../../source/vc_expr_base.cpp
	../../source/vc_expr_base.h
	../../source/vc_expr_cast.cpp
	../../source/vc_expr_cast.h
	../../source/vc_expr_field.cpp
	../../source/vc_expr_field.h
	../../source/vc_expr_invoke.cpp
	../../source/vc_expr_invoke.h
	../../source/vc_expr_literal.cpp
	../../source/vc_expr_literal.h
	../../source/vc_expr_local.cpp
	../../source/vc_expr_local.h
	../../source/vc_expr_misc.cpp
	../../source/vc_expr_misc.h
	../../source/vc_expr_type.cpp
	../../source/vc_expr_type.h
	../../source/vc_expr_unary_binary.cpp
	../../source/vc_expr_unary_binary.h
	../../source/vc_field.cpp
	../../source/vc_field.h
	../../source/vc_lexer.cpp
	../../source/vc_lexer.h
	../../source/vc_location.cpp
	../../source/vc_location.h
	../../source/vc_member.cpp
	../../source/vc_member.h
	../../source/vc_method.cpp
	../../source/vc_method.h
	../../source/vc_modifiers.cpp
	../../source/vc_modifiers.h
	../../source/vc_package.cpp
	../../source/vc_package.h
	../../source/vc_parser.cpp
	../../source/vc_parser.h
	../../source/vc_property.cpp
	../../source/vc_property.h
	../../source/vc_state.cpp
	../../source/vc_state.h
	../../source/vc_statement.cpp
	../../source/vc_statement.h
	../../source/vc_struct.cpp
	../../source/vc_struct.h
	../../source/vc_type.cpp
	../../source/vc_type.h
	vcc.cpp
	vcc.h
)
set_target_properties(vcc PROPERTIES OUTPUT_NAME ../bin/vcc)
set_target_properties(vcc PROPERTIES COMPILE_FLAGS -DIN_VCC)
target_link_libraries(vcc core)
add_dependencies(vcc core)

#EXTRA_DIST = vcc.txt
