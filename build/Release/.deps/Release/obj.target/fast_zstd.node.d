cmd_Release/obj.target/fast_zstd.node := g++ -o Release/obj.target/fast_zstd.node -shared -pthread -rdynamic -m64  -Wl,-soname=fast_zstd.node -Wl,--start-group Release/obj.target/fast_zstd/hello.o -Wl,--end-group 
