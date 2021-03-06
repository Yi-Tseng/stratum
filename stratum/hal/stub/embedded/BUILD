#
# Copyright 2018 Google LLC
# Copyright 2018-present Open Networking Foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

licenses(["notice"])  # Apache v2

load(
    "//bazel:rules.bzl",
    "STRATUM_INTERNAL",
    "stratum_cc_binary",
    "stratum_package",
)

package(
    #default_hdrs_check = "strict",
    default_visibility = STRATUM_INTERNAL,
)

stratum_cc_binary(
    name = "stratum_stub",
    srcs = ["main.cc"],
    linkopts = [
        "-lpthread",
        "-ldl",
        "-lrt",
    ],
    deps = [
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/numeric:int128",
        "@com_google_absl//absl/synchronization",
        "@com_google_protobuf//:protobuf",
        "@com_github_p4lang_PI//:code_proto",
        "@com_github_p4lang_PI//:p4runtime_v1_grpc_proto",
        "@com_github_grpc_grpc//:grpc++",
        #"//sandblaze/prebuilt/openssl",
        "//stratum/glue:init_google",
        "//stratum/glue:logging",
        "@com_github_openconfig_gnmi_proto//:gnmi_cc_grpc",
        "//stratum/glue/openconfig/proto:old_openconfig_proto",
        "//stratum/hal/lib/common:common_cc_proto",
        "//stratum/hal/lib/common:openconfig_converter",
        "//stratum/hal/lib/p4:p4_pipeline_config_cc_proto",
        "//stratum/hal/lib/p4:p4_table_mapper",
        "//stratum/lib:constants",
        "//stratum/lib:macros",
        "//stratum/lib:utils",
        "//stratum/glue/gtl:map_util",
    ],
)

stratum_package(
    name = "stratum_stub_pkg",
    bins = [
        ":stratum_stub",
    ],
)
