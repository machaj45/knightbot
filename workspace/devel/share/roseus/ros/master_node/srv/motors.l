;; Auto-generated. Do not edit!


(when (boundp 'master_node::motors)
  (if (not (find-package "MASTER_NODE"))
    (make-package "MASTER_NODE"))
  (shadow 'motors (find-package "MASTER_NODE")))
(unless (find-package "MASTER_NODE::MOTORS")
  (make-package "MASTER_NODE::MOTORS"))
(unless (find-package "MASTER_NODE::MOTORSREQUEST")
  (make-package "MASTER_NODE::MOTORSREQUEST"))
(unless (find-package "MASTER_NODE::MOTORSRESPONSE")
  (make-package "MASTER_NODE::MOTORSRESPONSE"))

(in-package "ROS")





(defclass master_node::motorsRequest
  :super ros::object
  :slots (_cmd _param ))

(defmethod master_node::motorsRequest
  (:init
   (&key
    ((:cmd __cmd) 0)
    ((:param __param) 0)
    )
   (send-super :init)
   (setq _cmd (round __cmd))
   (setq _param (round __param))
   self)
  (:cmd
   (&optional __cmd)
   (if __cmd (setq _cmd __cmd)) _cmd)
  (:param
   (&optional __param)
   (if __param (setq _param __param)) _param)
  (:serialization-length
   ()
   (+
    ;; int32 _cmd
    4
    ;; int32 _param
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _cmd
       (write-long _cmd s)
     ;; int32 _param
       (write-long _param s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _cmd
     (setq _cmd (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _param
     (setq _param (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass master_node::motorsResponse
  :super ros::object
  :slots (_success _message ))

(defmethod master_node::motorsResponse
  (:init
   (&key
    ((:success __success) nil)
    ((:message __message) "")
    )
   (send-super :init)
   (setq _success __success)
   (setq _message (string __message))
   self)
  (:success
   (&optional __success)
   (if __success (setq _success __success)) _success)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; string _message
    4 (length _message)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass master_node::motors
  :super ros::object
  :slots ())

(setf (get master_node::motors :md5sum-) "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")
(setf (get master_node::motors :datatype-) "master_node/motors")
(setf (get master_node::motors :request) master_node::motorsRequest)
(setf (get master_node::motors :response) master_node::motorsResponse)

(defmethod master_node::motorsRequest
  (:response () (instance master_node::motorsResponse :init)))

(setf (get master_node::motorsRequest :md5sum-) "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")
(setf (get master_node::motorsRequest :datatype-) "master_node/motorsRequest")
(setf (get master_node::motorsRequest :definition-)
      "int32 cmd
int32 param
---
bool success
string message

")

(setf (get master_node::motorsResponse :md5sum-) "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")
(setf (get master_node::motorsResponse :datatype-) "master_node/motorsResponse")
(setf (get master_node::motorsResponse :definition-)
      "int32 cmd
int32 param
---
bool success
string message

")



(provide :master_node/motors "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")


