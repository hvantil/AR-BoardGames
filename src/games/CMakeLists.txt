add_library(games_lib
	gamesController.h gamesController.cpp
	ttt.h ttt.cpp
	c4.h c4.cpp
	battleship.h battleship.cpp
)
target_include_directories(games_lib PUBLIC .)
target_link_libraries(games_lib
	${CURL_LIBRARIES}
	${OpenCV_LIBS}
	fssimplewindow
	fslazywindow
	ysbitmapfont
	ysclass
	geblkernel
	util_lib
	trackdraw_lib
)