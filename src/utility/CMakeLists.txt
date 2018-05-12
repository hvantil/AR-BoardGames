add_library(util_lib
	binstl.h binstl.cpp
	ysglfontdata.h ysglfontdata.c
	glutil.h glutil.cpp
)
target_include_directories(util_lib PUBLIC .)
target_link_libraries(util_lib 
	${OpenCV_LIBS}
	fssimplewindow
	fslazywindow
	ysbitmapfont
	ysclass
	geblkernel
)
