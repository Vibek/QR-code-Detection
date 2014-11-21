
(cl:in-package :asdf)

(defsystem "QRcode_detection-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Pose" :depends-on ("_package_Pose"))
    (:file "_package_Pose" :depends-on ("_package"))
    (:file "Goal" :depends-on ("_package_Goal"))
    (:file "_package_Goal" :depends-on ("_package"))
  ))