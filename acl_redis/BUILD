# Description: OCR recog module on 2017/08/15(author:li.zhigang)

package(
    default_visibility = ["//tensorflow_serving:internal"],
    features = ["no_layering_check"],
)

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

load("//tensorflow_serving:serving.bzl", "serving_proto_library")

filegroup(
    name = "all_files",
    srcs = glob(
        ["**/*"],
        exclude = [
            "**/METADATA",
            "**/OWNERS",
        ],
    ),
)

cc_library(
    name = "redis_lib", 
    srcs = ["libacl.so",
    		"libacl_cpp.so",
    		"libprotocol.so",
    		"libredis_util.so",],
    hdrs = ["redis_util.h",],
    visibility = ["//visibility:public"], 
) 

cc_binary(
    name = "redis",
    srcs = glob([
        "main.cpp",
    ]),

    deps = [
        ":redis_lib",
        "//tensorflow_serving/apis:prediction_service_proto",
        "@grpc//:grpc++",
        "@org_tensorflow//tensorflow/core:framework",
        "@protobuf//:protobuf_lite",
    ],
)

