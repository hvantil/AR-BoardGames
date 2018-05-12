add_library(trackdraw_lib 
	cvStuff.h cvStuff.cpp
	objectStl.h objectStl.cpp
)
target_include_directories(trackdraw_lib PUBLIC .)
target_link_libraries(trackdraw_lib 
	${OpenCV_LIBS}
	fssimplewindow
	fslazywindow
	ysbitmapfont
	ysclass
	geblkernel
	util_lib
)
