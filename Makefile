

SO_NAME=libidol_sentence_breaking_example.so

lib:
	g++ -g -std=c++11 -fPIC -shared -o ${SO_NAME} idol_sentence_breaking_example.cpp

clean:
	rm -f ${SO_NAME}

