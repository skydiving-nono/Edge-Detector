main:
	g++ bitmap.cpp main.cpp -o EdgeDetector

default:
	g++ main.cpp 4103.bmp 1 bitmap.cpp -o EdgeDetector

clean:
	rm ./EdgeDetector test.bmp