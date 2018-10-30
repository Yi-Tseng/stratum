/*
 * Copyright 2018 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef STRATUM_HAL_LIB_COMMON_UTILS_H_
#define STRATUM_HAL_LIB_COMMON_UTILS_H_

#include <functional>
#include <string>

#include "stratum/hal/lib/common/common.pb.h"
#include "stratum/glue/integral_types.h"
#include "absl/strings/str_cat.h"

namespace stratum {
namespace hal {

// PortKey is a generic data structure which is meant to be used as a key that
// uniquely identifies a "port" in hercules code base. By "port" we mean a
// singleton port, a transceiver port, a flex/non-flex port group (i.e. a set
// of ports with the same (slot, port) which are either flex or non-flex), etc.
// The port whose key is identified by this data strcuture can be channelized (
// in which case there is a non-zero channel number), or non-channelized (in
// which case channel number is set to zero). In case the key is used when the
// channel is not important at all (e.g. when the key identifies a flex or
// non-flex port group), we use the default value of -1 for the channel.
struct PortKey {
  int slot;
  int port;
  int channel;
  PortKey(int _slot, int _port, int _channel)
      : slot(_slot), port(_port), channel(_channel) {}
  PortKey(int _slot, int _port) : slot(_slot), port(_port), channel(-1) {}
  PortKey() : slot(-1), port(-1), channel(-1) {}
  bool operator<(const PortKey& other) const {
    return (slot < other.slot ||
            (slot == other.slot &&
             (port < other.port ||
              (port == other.port && channel < other.channel))));
  }
  bool operator==(const PortKey& other) const {
    return (slot == other.slot && port == other.port &&
            channel == other.channel);
  }
  std::string ToString() const {
    if (channel > 0) {
      return absl::StrCat("(slot: ", slot, ", port: ", port,
                          ", channel: ", channel, ")");
    } else {
      return absl::StrCat("(slot: ", slot, ", port: ", port, ")");
    }
  }
};

// A custom hash functor for SingletonPort proto message in hal.proto.
class SingletonPortHash {
 public:
  size_t operator()(const SingletonPort& port) const {
    size_t hash_val = 0;
    std::hash<int> integer_hasher;
    hash_val ^= integer_hasher(port.slot());
    hash_val ^= integer_hasher(port.port());
    hash_val ^= integer_hasher(port.channel());
    // Use middle 32 bits of speed_bps
    hash_val ^=
        integer_hasher(static_cast<int>((port.speed_bps() >> 16) & 0xFFFFFFFF));
    return hash_val;
  }
};

// A custom equal functor for SingletonPort proto messages in hal.proto.
class SingletonPortEqual {
 public:
  bool operator()(const SingletonPort& lhs, const SingletonPort& rhs) const {
    return (lhs.slot() == rhs.slot()) && (lhs.port() == rhs.port()) &&
           (lhs.channel() == rhs.channel()) &&
           (lhs.speed_bps() == rhs.speed_bps());
  }
};

// Functor for comparing two SingletonPort instances based on slot, port,
// channel and speed_bps values in that order. Returns true if the first
// argument precedes the second in order, false otherwise.
class SingletonPortLess {
 public:
  // Returns true if the first argument precedes the second; false otherwise.
  bool operator()(const SingletonPort& x, const SingletonPort& y) const {
    return ComparePorts(x, y);
  }

 private:
  // Compares slot, port, channel and speed_bps in that order.
  // Returns true if the first agrument precedes the second, false otherwise.
  bool ComparePorts(const SingletonPort& x, const SingletonPort& y) const {
    if (x.slot() != y.slot()) {
      return x.slot() < y.slot();
    } else if (x.port() != y.port()) {
      return x.port() < y.port();
    } else if (x.channel() != y.channel()) {
      return x.channel() < y.channel();
    } else {
      return x.speed_bps() < y.speed_bps();
    }
  }
};

// Prints a Node proto message in a consistent and readable format.
std::string PrintNode(const Node& n);

// Prints a SingletonPort proto message in a consistent and readable format.
std::string PrintSingletonPort(const SingletonPort& p);

// Prints a TrunkPort proto message in a consistent and readable format.
std::string PrintTrunkPort(const TrunkPort& p);

// A set of helper functions to print a superset of node/port/trunk properties
// that are worth logging, in a consistent and readable way. These methods
// check and ignores the invalid args passed to it when printing. Other
// printer function make use of these helpers to not duplicate the priting
// logic.
std::string PrintNodeProperties(uint64 id, int slot, int index);

std::string PrintPortProperties(uint64 node_id, uint32 port_id, int slot,
                                int port, int channel, int unit,
                                int logical_port, uint64 speed_bps);

std::string PrintTrunkProperties(uint64 node_id, uint32 trunk_id, int unit,
                                 int trunk_port, uint64 speed_bps);

// Prints PortState in a consistent format.
std::string PrintPortState(PortState state);

// Builds a SingletonPort proto message with the given field values. No sanity
// checking is performed that the parameters are valid for the switch.
SingletonPort BuildSingletonPort(int slot, int port, int channel,
                                 uint64 speed_bps);

// An alias for the pair of (LedColor, LedState) for a front panel port LED.
using PortLedConfig = std::pair<LedColor, LedState>;

// A util function that translates the state(s) of a channelized/non-channelized
// singleton port to a pair of (LedColor, LedState) to be shown on the front
// panel port LED.
PortLedConfig FindPortLedColorAndState(AdminState admin_state,
                                       PortState oper_state,
                                       HealthState health_state,
                                       TrunkMemberBlockState block_state);

// A util function that aggregate the (LedColor, LedState) pairs, corresponding
// to different channels of a front panel, into one single (LedColor, LedState)
// pair. This method is used when each front panel port has only one LED and
// the per-channel (LedColor, LedState) pairs need to be aggregated to be shown
// on this single LED.
PortLedConfig AggregatePortLedColorsStatePairs(
    const std::vector<PortLedConfig>& color_state_pairs);

}  // namespace hal
}  // namespace stratum

#endif  // STRATUM_HAL_LIB_COMMON_UTILS_H_
