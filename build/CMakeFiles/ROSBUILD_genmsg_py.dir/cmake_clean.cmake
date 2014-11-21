FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/QRcode_detection/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/QRcode_detection/msg/__init__.py"
  "../src/QRcode_detection/msg/_Pose.py"
  "../src/QRcode_detection/msg/_Goal.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
