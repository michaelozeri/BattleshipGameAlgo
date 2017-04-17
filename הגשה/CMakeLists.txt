set(user1 mordehaiz)
set(user2 heizenberg)
set(user3 nevuhadnetzer)
set(id1 "205969710")
set(id2 "123456789")
set(id3 "123456789")
set (SOURCE_FILES
	AttackReciever.cpp
	BattleshipGameAlgo.cpp
	Bonus.cpp
	GameBordUtils.cpp
	main.cpp
	AttackReciever.h
	BattleshipGameAlgo.h
	Bonus.h
	Contants.h
	GameBordUtils.h
	IBattleshipGameAlgo.h
	IOLib.h
	Logger.h
)

### DO NOT CHANGE FROM HERE ON ###

message("user: ${user1}, id: ${id1}")
message("user: ${user2}, id: ${id2}")
message("user: ${user3}, id: ${id3}")

set(EXE_NAME "ex1.${user1}")
add_executable( ${EXE_NAME}
	${SOURCE_FILES}
)
#BUILD_BINARY_OUTPUT_DIRECTORY
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex1/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")