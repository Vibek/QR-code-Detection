FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/QRcode_detection/msg"
  "CMakeFiles/ROSBUILD_genmsg_lisp"
  "../msg_gen/lisp/Pose.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_Pose.lisp"
  "../msg_gen/lisp/Goal.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_Goal.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
