{
  "targets": [
    {
      "target_name": "zstd",
      "sources": [ "hello.cc" ],
      "include_dirs": [
        "include", "<(module_root_dir)/vendor/zstd/lib/"
      ],
      "libraries": [
        "-lzstd",
        "-L<(module_root_dir)/vendor/zstd/lib/"
      ]
    }
  ]
}
