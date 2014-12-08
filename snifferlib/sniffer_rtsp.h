#pragma once
#include "sniffer_protocol.h"

#define PROTOCOL_RTSP_PORT (554)
int sniffer_rtsp_process(tcp_udp_packet_t *packet, NotifyCallback cb);