
(cl:in-package :asdf)

(defsystem "master_node-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "buttons" :depends-on ("_package_buttons"))
    (:file "_package_buttons" :depends-on ("_package"))
    (:file "motors" :depends-on ("_package_motors"))
    (:file "_package_motors" :depends-on ("_package"))
  ))