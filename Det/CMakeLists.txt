cmake_minimum_required(VERSION 3.22)
project(NotDSMToDSM)
add_executable(
        NotDSMToDSM
        main.cpp
        MachineSaver/MachineSaver.cpp
        MachineSaver/MachineSaver.h
        TextParser/CSVTextParser.h
        Machine/Machine.h
        Converter/DSMConverter.cpp
        Converter/DSMConverter.h
        Common/Lettering.h
        TextParser/CSVTextParser.cpp
        Converter/EmptyStateRemover/EmptyStateRemover.cpp
        Converter/EmptyStateRemover/EmptyStateRemover.h
)
