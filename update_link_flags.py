from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

env.Append(
    LINKFLAGS=[
        "-mfloat-abi=soft",
        #"-mfpu=fpv4-sp-d16"
    ]

)
