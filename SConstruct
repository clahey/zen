env = Environment(CCFLAGS="-std=c++0x -Wall")

optimizeEnv = env.Clone()
optimizeEnv.Append(CCFLAGS = ' -O4', LINKFLAGS = '-O4')

debugEnv = env.Clone()
debugEnv.Append(CCFLAGS = ' -g')

profileEnv = optimizeEnv.Clone()
profileEnv.Append(CCFLAGS = ' -g -pg', LINKFLAGS = ' -g -pg')

testEnv = env.Clone()
testEnv.Append(CCFLAGS = ' -g', LIBS="boost_unit_test_framework")

source = ['test', 'Random']
optimizeEnv.Program(target='magnets', source=[optimizeEnv.Object(i + '.cc') for i in source])
debugEnv.Program(target='magnets-debug', source=[debugEnv.Object(i + '-debug', i + '.cc') for i in source])
profileEnv.Program(target='magnets-profile', source=[profileEnv.Object(i + '-profile', i + '.cc') for i in source])
testEnv.Program(target='test', source=[testEnv.Object(i + '-test', i + '.cc') for i in ['unit', 'Random']])
