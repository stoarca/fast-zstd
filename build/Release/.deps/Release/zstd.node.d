cmd_Release/zstd.node := ln -f "Release/obj.target/zstd.node" "Release/zstd.node" 2>/dev/null || (rm -rf "Release/zstd.node" && cp -af "Release/obj.target/zstd.node" "Release/zstd.node")
