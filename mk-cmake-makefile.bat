:: Note: -DINVOKE_LITE_OPT_SELECT_NONSTD=ON is non-functional
:: Note: -DINVOKE_LITE_OPT_BUILD_TESTS=OFF as there aren't tests yet
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_CXX_STANDARD=11 -DCMAKE_BUILD_TYPE=Release -DINVOKE_LITE_OPT_SELECT_NONSTD=ON -DINVOKE_LITE_OPT_BUILD_TESTS=OFF -DINVOKE_LITE_OPT_BUILD_EXAMPLES=ON
cmake --build build --config Release
