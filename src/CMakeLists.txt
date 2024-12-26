#Add source files
add_executable (merkle main.c
					   merkle-tree.h
					   merkle-tree.c)

# Link the crypto library
target_link_libraries (merkle crypto)

# Specify output directory for the executable
set_target_properties (merkle PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
