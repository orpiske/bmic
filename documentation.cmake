file(MAKE_DIRECTORY target/share/doc)
execute_process(COMMAND doxygen ../Doxyfile)