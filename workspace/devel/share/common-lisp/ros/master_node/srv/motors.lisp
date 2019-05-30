; Auto-generated. Do not edit!


(cl:in-package master_node-srv)


;//! \htmlinclude motors-request.msg.html

(cl:defclass <motors-request> (roslisp-msg-protocol:ros-message)
  ((cmd
    :reader cmd
    :initarg :cmd
    :type cl:integer
    :initform 0)
   (param
    :reader param
    :initarg :param
    :type cl:integer
    :initform 0))
)

(cl:defclass motors-request (<motors-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <motors-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'motors-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name master_node-srv:<motors-request> is deprecated: use master_node-srv:motors-request instead.")))

(cl:ensure-generic-function 'cmd-val :lambda-list '(m))
(cl:defmethod cmd-val ((m <motors-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader master_node-srv:cmd-val is deprecated.  Use master_node-srv:cmd instead.")
  (cmd m))

(cl:ensure-generic-function 'param-val :lambda-list '(m))
(cl:defmethod param-val ((m <motors-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader master_node-srv:param-val is deprecated.  Use master_node-srv:param instead.")
  (param m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <motors-request>) ostream)
  "Serializes a message object of type '<motors-request>"
  (cl:let* ((signed (cl:slot-value msg 'cmd)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'param)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <motors-request>) istream)
  "Deserializes a message object of type '<motors-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'cmd) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'param) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<motors-request>)))
  "Returns string type for a service object of type '<motors-request>"
  "master_node/motorsRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'motors-request)))
  "Returns string type for a service object of type 'motors-request"
  "master_node/motorsRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<motors-request>)))
  "Returns md5sum for a message object of type '<motors-request>"
  "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'motors-request)))
  "Returns md5sum for a message object of type 'motors-request"
  "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<motors-request>)))
  "Returns full string definition for message of type '<motors-request>"
  (cl:format cl:nil "int32 cmd~%int32 param~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'motors-request)))
  "Returns full string definition for message of type 'motors-request"
  (cl:format cl:nil "int32 cmd~%int32 param~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <motors-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <motors-request>))
  "Converts a ROS message object to a list"
  (cl:list 'motors-request
    (cl:cons ':cmd (cmd msg))
    (cl:cons ':param (param msg))
))
;//! \htmlinclude motors-response.msg.html

(cl:defclass <motors-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (message
    :reader message
    :initarg :message
    :type cl:string
    :initform ""))
)

(cl:defclass motors-response (<motors-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <motors-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'motors-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name master_node-srv:<motors-response> is deprecated: use master_node-srv:motors-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <motors-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader master_node-srv:success-val is deprecated.  Use master_node-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <motors-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader master_node-srv:message-val is deprecated.  Use master_node-srv:message instead.")
  (message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <motors-response>) ostream)
  "Serializes a message object of type '<motors-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <motors-response>) istream)
  "Deserializes a message object of type '<motors-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'message) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'message) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<motors-response>)))
  "Returns string type for a service object of type '<motors-response>"
  "master_node/motorsResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'motors-response)))
  "Returns string type for a service object of type 'motors-response"
  "master_node/motorsResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<motors-response>)))
  "Returns md5sum for a message object of type '<motors-response>"
  "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'motors-response)))
  "Returns md5sum for a message object of type 'motors-response"
  "1dc0bbce23ad0191e8ff5d2a2e5aa5e7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<motors-response>)))
  "Returns full string definition for message of type '<motors-response>"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'motors-response)))
  "Returns full string definition for message of type 'motors-response"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <motors-response>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <motors-response>))
  "Converts a ROS message object to a list"
  (cl:list 'motors-response
    (cl:cons ':success (success msg))
    (cl:cons ':message (message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'motors)))
  'motors-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'motors)))
  'motors-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'motors)))
  "Returns string type for a service object of type '<motors>"
  "master_node/motors")