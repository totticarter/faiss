g++-5 -fPIC -m64 -Wall -g -O3 -msse4 -mpopcnt -fopenmp -Wno-sign-compare -I/usr/local/opt/llvm/include -o test_ivfpq_indexing test_ivfpq_indexing.cpp ../libfaiss.a gtest/make/gtest_main.a -g -fPIC -fopenmp -L/usr/local/opt/llvm/lib -L/usr/local/Cellar/llvm/6.0.0/lib -framework Accelerate \
	-Igtest/include -I../..
