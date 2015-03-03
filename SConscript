# SConscript file
# Effectively the contents of a normal Sconstruct file -- used to allow easy use of VarientDir for changing the build output directory

execName = 'SFML Resource Manager' 
libName = 'sfml-res-man'

libSrc = Glob('src/ResourceManager/*.cpp')
mainSrc = Glob('src/*.cpp')

includeDirs = ['./include', './include/ResourceManager', '/usr/include']

env = Environment(CPPPATH = includeDirs, 
                  LIBPATH = '/usr/lib',
                  LIBS = ['sfml-window', 'sfml-system', 'sfml-graphics'],
                  CXXFLAGS = "-std=c++11")

env.Program(target = '../' + execName , source = [mainSrc, libSrc])
env.StaticLibrary(target = '../' + libName, source = libSrc)
