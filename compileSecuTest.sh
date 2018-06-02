
rm -rf demos/read_search
#compile write
/usr/local/opt/llvm/bin/clang++ -o demos/read_search -fPIC -m64 -Wall -g  -msse4 -mpopcnt -fopenmp -Wno-sign-compare -I/usr/local/opt/llvm/include -std=c++11 demos/read_search.cpp libfaiss.a -g -fPIC -fopenmp -L/usr/local/opt/llvm/lib -L/usr/local/Cellar/llvm/6.0.0/lib -framework Accelerate

rm -rf demos/write_ivf_ip
#compile write
/usr/local/opt/llvm/bin/clang++ -o demos/write_ivf_ip -fPIC -m64 -Wall -g  -msse4 -mpopcnt -fopenmp -Wno-sign-compare -I/usr/local/opt/llvm/include -std=c++11 demos/write_ivf_ip.cpp libfaiss.a -g -fPIC -fopenmp -L/usr/local/opt/llvm/lib -L/usr/local/Cellar/llvm/6.0.0/lib -framework Accelerate

rm -rf demos/indextrain
#compile indextrain
/usr/local/opt/llvm/bin/clang++ -o demos/indextrain -fPIC -m64 -Wall -g  -msse4 -mpopcnt -fopenmp -Wno-sign-compare -I/usr/local/opt/llvm/include -std=c++11 demos/indextrain.cpp libfaiss.a -g -fPIC -fopenmp -L/usr/local/opt/llvm/lib -L/usr/local/Cellar/llvm/6.0.0/lib -framework Accelerate


rm -rf demos/indexadd
#compile indexadd
/usr/local/opt/llvm/bin/clang++ -o demos/indexadd -fPIC -m64 -Wall -g  -msse4 -mpopcnt -fopenmp -Wno-sign-compare -I/usr/local/opt/llvm/include -std=c++11 demos/indexadd.cpp  libfaiss.a -g -fPIC -fopenmp -L/usr/local/opt/llvm/lib -L/usr/local/Cellar/llvm/6.0.0/lib -framework Accelerate
