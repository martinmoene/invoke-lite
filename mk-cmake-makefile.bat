cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DINVOKE_LITE_OPT_SELECT_NONSTD=ON -DINVOKE_LITE_OPT_BUILD_TESTS=ON -DINVOKE_LITE_OPT_BUILD_EXAMPLES=ON -DINVOKE_LITE_OPT_BUILD_BENCHMARKS=ON
cmake --build build --config Release
