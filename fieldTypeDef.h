#include <stdio.h>
#include <stdlib.h>

typedef struct _value_string
{
    int   value;
    char* fieldType;
}value_string;

//TODO: use function query, not fixed

/* If you want sort of safely to send enterprise specific element IDs
   using v9 you need to stake a claim in the wilds with the high bit
   set.  Still no naming authority, but at least it will never collide
   with valid IPFIX */
#define NTOP_BASE 57472u                /* nprobe >= 5.5.6 */

static const value_string v9_v10_template_types[] = {
    {   1, "BYTES" },
    {   2, "PKTS" },
    {   3, "FLOWS" },
    {   4, "PROTOCOL" },
    {   5, "IP_TOS" },
    {   6, "TCP_FLAGS" },
    {   7, "L4_SRC_PORT" },
    {   8, "IP_SRC_ADDR" },
    {   9, "SRC_MASK" },
    {  10, "INPUT_SNMP" },
    {  11, "L4_DST_PORT" },
    {  12, "IP_DST_ADDR" },
    {  13, "DST_MASK" },
    {  14, "OUTPUT_SNMP" },
    {  15, "IP_NEXT_HOP" },
    {  16, "SRC_AS" },
    {  17, "DST_AS" },
    {  18, "BGP_NEXT_HOP" },
    {  19, "MUL_DPKTS" },
    {  20, "MUL_DOCTETS" },
    {  21, "LAST_SWITCHED" },
    {  22, "FIRST_SWITCHED" },
    {  23, "OUT_BYTES" },
    {  24, "OUT_PKTS" },
    {  25, "IP LENGTH MINIMUM" },
    {  26, "IP LENGTH MAXIMUM" },
    {  27, "IPV6_SRC_ADDR" },
    {  28, "IPV6_DST_ADDR" },
    {  29, "IPV6_SRC_MASK" },
    {  30, "IPV6_DST_MASK" },
    {  31, "FLOW_LABEL" },
    {  32, "ICMP_TYPE" },
    {  33, "IGMP_TYPE" },
    {  34, "SAMPLING_INTERVAL" },
    {  35, "SAMPLING_ALGORITHM" },
    {  36, "FLOW_ACTIVE_TIMEOUT" },
    {  37, "FLOW_INACTIVE_TIMEOUT" },
    {  38, "ENGINE_TYPE" },
    {  39, "ENGINE_ID" },
    {  40, "TOTAL_BYTES_EXP" },
    {  41, "TOTAL_PKTS_EXP" },
    {  42, "TOTAL_FLOWS_EXP" },
    {  44, "IP_SRC_PREFIX" },
    {  45, "IP_DST_PREFIX" },
    {  46, "MPLS_TOP_LABEL_TYPE" },
    {  47, "MPLS_TOP_LABEL_ADDR" },
    {  48, "FLOW_SAMPLER_ID" },
    {  49, "FLOW_SAMPLER_MODE" },
    {  50, "FLOW_SAMPLER_RANDOM_INTERVAL" },
    {  51, "FLOW_CLASS" },
    {  52, "IP TTL MINIMUM" },
    {  53, "IP TTL MAXIMUM" },
    {  54, "IPv4 ID" },
    {  55, "DST_TOS" },
    {  56, "SRC_MAC" },
    {  57, "DST_MAC" },
    {  58, "SRC_VLAN" },
    {  59, "DST_VLAN" },
    {  60, "IP_PROTOCOL_VERSION" },
    {  61, "DIRECTION" },
    {  62, "IPV6_NEXT_HOP" },
    {  63, "BGP_IPV6_NEXT_HOP" },
    {  64, "IPV6_OPTION_HEADERS" },
    {  70, "MPLS_LABEL_1" },
    {  71, "MPLS_LABEL_2" },
    {  72, "MPLS_LABEL_3" },
    {  73, "MPLS_LABEL_4" },
    {  74, "MPLS_LABEL_5" },
    {  75, "MPLS_LABEL_6" },
    {  76, "MPLS_LABEL_7" },
    {  77, "MPLS_LABEL_8" },
    {  78, "MPLS_LABEL_9" },
    {  79, "MPLS_LABEL_10" },
    {  80, "DESTINATION_MAC" },
    {  81, "SOURCE_MAC" },
    {  82, "IF_NAME" },
    {  83, "IF_DESC" },
    {  84, "SAMPLER_NAME" },
    {  85, "BYTES_TOTAL" },
    {  86, "PACKETS_TOTAL" },
    {  88, "FRAGMENT_OFFSET" },
    {  89, "FORWARDING_STATUS" },
    {  90, "VPN_ROUTE_DISTINGUISHER" },
    {  91, "mplsTopLabelPrefixLength" },
    {  92, "SRC_TRAFFIC_INDEX" },
    {  93, "DST_TRAFFIC_INDEX" },
    {  94, "APPLICATION_DESC" },
    {  95, "APPLICATION_ID" },
    {  96, "APPLICATION_NAME" },
    {  98, "postIpDiffServCodePoint" },
    {  99, "multicastReplicationFactor" },
    { 128, "DST_AS_PEER" },
    { 129, "SRC_AS_PEER" },
    { 130, "exporterIPv4Address" },
    { 131, "exporterIPv6Address" },
    { 132, "DROPPED_BYTES" },
    { 133, "DROPPED_PACKETS" },
    { 134, "DROPPED_BYTES_TOTAL" },
    { 135, "DROPPED_PACKETS_TOTAL" },
    { 136, "flowEndReason" },
    { 137, "commonPropertiesId" },
    { 138, "observationPointId" },
    { 139, "icmpTypeCodeIPv6" },
    { 140, "MPLS_TOP_LABEL_IPv6_ADDRESS" },
    { 141, "lineCardId" },
    { 142, "portId" },
    { 143, "meteringProcessId" },
    { 144, "FLOW_EXPORTER" },
    { 145, "templateId" },
    { 146, "wlanChannelId" },
    { 147, "wlanSSID" },
    { 148, "flowId" },
    { 149, "observationDomainId" },
    { 150, "flowStartSeconds" },
    { 151, "flowEndSeconds" },
    { 152, "flowStartMilliseconds" },
    { 153, "flowEndMilliseconds" },
    { 154, "flowStartMicroseconds" },
    { 155, "flowEndMicroseconds" },
    { 156, "flowStartNanoseconds" },
    { 157, "flowEndNanoseconds" },
    { 158, "flowStartDeltaMicroseconds" },
    { 159, "flowEndDeltaMicroseconds" },
    { 160, "systemInitTimeMilliseconds" },
    { 161, "flowDurationMilliseconds" },
    { 162, "flowDurationMicroseconds" },
    { 163, "observedFlowTotalCount" },
    { 164, "ignoredPacketTotalCount" },
    { 165, "ignoredOctetTotalCount" },
    { 166, "notSentFlowTotalCount" },
    { 167, "notSentPacketTotalCount" },
    { 168, "notSentOctetTotalCount" },
    { 169, "destinationIPv6Prefix" },
    { 170, "sourceIPv6Prefix" },
    { 171, "postOctetTotalCount" },
    { 172, "postPacketTotalCount" },
    { 173, "flowKeyIndicator" },
    { 174, "postMCastPacketTotalCount" },
    { 175, "postMCastOctetTotalCount" },
    { 176, "ICMP_IPv4_TYPE" },
    { 177, "ICMP_IPv4_CODE" },
    { 178, "ICMP_IPv6_TYPE" },
    { 179, "ICMP_IPv6_CODE" },
    { 180, "UDP_SRC_PORT" },
    { 181, "UDP_DST_PORT" },
    { 182, "TCP_SRC_PORT" },
    { 183, "TCP_DST_PORT" },
    { 184, "TCP_SEQ_NUM" },
    { 185, "TCP_ACK_NUM" },
    { 186, "TCP_WINDOW_SIZE" },
    { 187, "TCP_URGENT_PTR" },
    { 188, "TCP_HEADER_LEN" },
    { 189, "IP_HEADER_LEN" },
    { 190, "IP_TOTAL_LEN" },
    { 191, "payloadLengthIPv6" },
    { 192, "IP_TTL" },
    { 193, "nextHeaderIPv6" },
    { 194, "IP_TOS" },
    { 195, "IP_DSCP" },
    { 196, "IP_PRECEDENCE" },
    { 197, "IP_FRAGMENT_FLAGS" },
    { 198, "BYTES_SQUARED" },
    { 199, "BYTES_SQUARED_PERMANENT" },
    { 200, "MPLS_TOP_LABEL_TTL" },
    { 201, "MPLS_LABEL_STACK_OCTETS" },
    { 202, "MPLS_LABEL_STACK_DEPTH" },
    { 203, "MPLS_TOP_LABEL_EXP" },
    { 204, "IP_PAYLOAD_LENGTH" },
    { 205, "UDP_LENGTH" },
    { 206, "IS_MULTICAST" },
    { 207, "IP_HEADER_WORDS" },
    { 208, "IP_OPTION_MAP" },
    { 209, "TCP_OPTION_MAP" },
    { 210, "paddingOctets" },
    { 211, "collectorIPv4Address" },
    { 212, "collectorIPv6Address" },
    { 213, "collectorInterface" },
    { 214, "collectorProtocolVersion" },
    { 215, "collectorTransportProtocol" },
    { 216, "collectorTransportPort" },
    { 217, "exporterTransportPort" },
    { 218, "tcpSynTotalCount" },
    { 219, "tcpFinTotalCount" },
    { 220, "tcpRstTotalCount" },
    { 221, "tcpPshTotalCount" },
    { 222, "tcpAckTotalCount" },
    { 223, "tcpUrgTotalCount" },
    { 224, "ipTotalLength" },
    { 225, "postNATSourceIPv4Address" },
    { 226, "postNATDestinationIPv4Address" },
    { 227, "postNAPTSourceTransportPort" },
    { 228, "postNAPTDestinationTransportPort" },
    { 229, "natOriginatingAddressRealm" },
    { 230, "natEvent" },
    { 231, "initiatorOctets" },
    { 232, "responderOctets" },
    { 233, "firewallEvent" },
    { 234, "ingressVRFID" },
    { 235, "egressVRFID" },
    { 236, "VRFname" },
    { 237, "postMplsTopLabelExp" },
    { 238, "tcpWindowScale" },
    { 239, "biflowDirection" },
    { 240, "ethernetHeaderLength" },
    { 241, "ethernetPayloadLength" },
    { 242, "ethernetTotalLength" },
    { 243, "dot1qVlanId" },
    { 244, "dot1qPriority" },
    { 245, "dot1qCustomerVlanId" },
    { 246, "dot1qCustomerPriority" },
    { 247, "metroEvcId" },
    { 248, "metroEvcType" },
    { 249, "pseudoWireId" },
    { 250, "pseudoWireType" },
    { 251, "pseudoWireControlWord" },
    { 252, "ingressPhysicalInterface" },
    { 253, "egressPhysicalInterface" },
    { 254, "postDot1qVlanId" },
    { 255, "postDot1qCustomerVlanId" },
    { 256, "ethernetType" },
    { 257, "postIpPrecedence" },
    { 258, "collectionTimeMilliseconds" },
    { 259, "exportSctpStreamId" },
    { 260, "maxExportSeconds" },
    { 261, "maxFlowEndSeconds" },
    { 262, "messageMD5Checksum" },
    { 263, "messageScope" },
    { 264, "minExportSeconds" },
    { 265, "minFlowStartSeconds" },
    { 266, "opaqueOctets" },
    { 267, "sessionScope" },
    { 268, "maxFlowEndMicroseconds" },
    { 269, "maxFlowEndMilliseconds" },
    { 270, "maxFlowEndNanoseconds" },
    { 271, "minFlowStartMicroseconds" },
    { 272, "minFlowStartMilliseconds" },
    { 273, "minFlowStartNanoseconds" },
    { 274, "collectorCertificate" },
    { 275, "exporterCertificate" },
    { 301, "selectionSequenceId" },
    { 302, "selectorId" },
    { 303, "informationElementId" },
    { 304, "selectorAlgorithm" },
    { 305, "samplingPacketInterval" },
    { 306, "samplingPacketSpace" },
    { 307, "samplingTimeInterval" },
    { 308, "samplingTimeSpace" },
    { 309, "samplingSize" },
    { 310, "samplingPopulation" },
    { 311, "samplingProbability" },
    { 313, "IP_SECTION HEADER" },
    { 314, "IP_SECTION PAYLOAD" },
    { 316, "mplsLabelStackSection" },
    { 317, "mplsPayloadPacketSection" },
    { 318, "selectorIdTotalPktsObserved" },
    { 319, "selectorIdTotalPktsSelected" },
    { 320, "absoluteError" },
    { 321, "relativeError" },
    { 322, "observationTimeSeconds" },
    { 323, "observationTimeMilliseconds" },
    { 324, "observationTimeMicroseconds" },
    { 325, "observationTimeNanoseconds" },
    { 326, "digestHashValue" },
    { 327, "hashIPPayloadOffset" },
    { 328, "hashIPPayloadSize" },
    { 329, "hashOutputRangeMin" },
    { 330, "hashOutputRangeMax" },
    { 331, "hashSelectedRangeMin" },
    { 332, "hashSelectedRangeMax" },
    { 333, "hashDigestOutput" },
    { 334, "hashInitialiserValue" },
    { 335, "selectorName" },
    { 336, "upperCILimit" },
    { 337, "lowerCILimit" },
    { 338, "confidenceLevel" },
    { 339, "informationElementDataType" },
    { 340, "informationElementDescription" },
    { 341, "informationElementName" },
    { 342, "informationElementRangeBegin" },
    { 343, "informationElementRangeEnd" },
    { 344, "informationElementSemantics" },
    { 345, "informationElementUnits" },
    { 346, "privateEnterpriseNumber" },
    /* Ericsson NAT Logging */
    { 24628, "NAT_LOG_FIELD_IDX_CONTEXT_ID" },
    { 24629, "NAT_LOG_FIELD_IDX_CONTEXT_NAME" },
    { 24630, "NAT_LOG_FIELD_IDX_ASSIGN_TS_SEC" },
    { 24631, "NAT_LOG_FIELD_IDX_UNASSIGN_TS_SEC" },
    { 24632, "NAT_LOG_FIELD_IDX_IPV4_INT_ADDR" },
    { 24633, "NAT_LOG_FIELD_IDX_IPV4_EXT_ADDR" },
    { 24634, "NAT_LOG_FIELD_IDX_EXT_PORT_FIRST" },
    { 24635, "NAT_LOG_FIELD_IDX_EXT_PORT_LAST" },
    /* Cisco ASA5500 Series NetFlow */
    { 33000, "INGRESS_ACL_ID" },
    { 33001, "EGRESS_ACL_ID" },
    { 33002, "FW_EXT_EVENT" },
    /* Cisco TrustSec */
    { 34000, "SGT_SOURCE_TAG" },
    { 34001, "SGT_DESTINATION_TAG" },
    { 34002, "SGT_SOURCE_NAME" },
    { 34003, "SGT_DESTINATION_NAME" },
    /* Boundary bprobe */
    { 33610, "METER_VERSION"},
    { 33611, "METER_OS_SYSNAME"},
    { 33612, "METER_OS_NODENAME"},
    { 33613, "METER_OS_RELEASE"},
    { 33614, "METER_OS_VERSION"},
    { 33615, "METER_OS_MACHINE"},
    { 33623, "EPOCH_SECOND"},
    { 33624, "NIC_NAME"},
    { 33625, "NIC_ID"},
    { 33626, "NIC_MAC"},
    { 33627, "NIC_IP"},
    { 33628, "COLLISIONS"},
    { 33629, "ERRORS"},
    { 33630, "NIC_DRIVER_NAME"},
    { 33631, "NIC_DRIVER_VERSION"},
    { 33632, "NIC_FIRMWARE_VERSION"},
    { 33633, "METER_OS_DISTRIBUTION_NAME"},
    { 33634, "BOND_INTERFACE_MODE"},
    { 33635, "BOND_INTERFACE_PHYSICAL_NIC_COUNT"},
    { 33636, "BOND_INTERFACE_ID"},
    /* medianet performance monitor */
    { 37000, "PACKETS_DROPPED" },
    { 37003, "BYTE_RATE" },
    { 37004, "APPLICATION_MEDIA_BYTES" },
    { 37006, "APPLICATION_MEDIA_BYTE_RATE" },
    { 37007, "APPLICATION_MEDIA_PACKETS" },
    { 37009, "APPLICATION_MEDIA_PACKET_RATE" },
    { 37011, "APPLICATION_MEDIA_EVENT" },
    { 37012, "MONITOR_EVENT" },
    { 37013, "TIMESTAMP_INTERVAL" },
    { 37014, "TRANSPORT_PACKETS_EXPECTED" },
    { 37016, "TRANSPORT_ROUND_TRIP_TIME" },
    { 37017, "TRANSPORT_EVENT_PACKET_LOSS" },
    { 37019, "TRANSPORT_PACKETS_LOST" },
    { 37021, "TRANSPORT_PACKETS_LOST_RATE" },
    { 37022, "TRANSPORT_RTP_SSRC" },
    { 37023, "TRANSPORT_RTP_JITTER_MEAN" },
    { 37024, "TRANSPORT_RTP_JITTER_MIN" },
    { 37025, "TRANSPORT_RTP_JITTER_MAX" },
    { 37041, "TRANSPORT_RTP_PAYLOAD_TYPE" },
    { 37071, "TRANSPORT_BYTES_OUT_OF_ORDER" },
    { 37074, "TRANSPORT_PACKETS_OUT_OF_ORDER" },
    { 37083, "TRANSPORT_TCP_WINDOWS_SIZE_MIN" },
    { 37084, "TRANSPORT_TCP_WINDOWS_SIZE_MAX" },
    { 37085, "TRANSPORT_TCP_WINDOWS_SIZE_MEAN" },
    { 37086, "TRANSPORT_TCP_MAXIMUM_SEGMENT_SIZE" },
    { 40000, "AAA_USERNAME" },
    { 40001, "XLATE_SRC_ADDR_IPV4" },
    { 40002, "XLATE_DST_ADDR_IPV4" },
    { 40003, "XLATE_SRC_PORT" },
    { 40004, "XLATE_DST_PORT" },
    { 40005, "FW_EVENT" },
    /* v9 nTop extensions. */
    {  80 + NTOP_BASE, "FRAGMENTS" },
    {  82 + NTOP_BASE, "CLIENT_NW_DELAY_SEC" },
    {  83 + NTOP_BASE, "CLIENT_NW_DELAY_USEC" },
    {  84 + NTOP_BASE, "SERVER_NW_DELAY_SEC" },
    {  85 + NTOP_BASE, "SERVER_NW_DELAY_USEC" },
    {  86 + NTOP_BASE, "APPL_LATENCY_SEC" },
    {  87 + NTOP_BASE, "APPL_LATENCY_USEC" },
    {  98 + NTOP_BASE, "ICMP_FLAGS" },
    { 101 + NTOP_BASE, "SRC_IP_COUNTRY" },
    { 102 + NTOP_BASE, "SRC_IP_CITY" },
    { 103 + NTOP_BASE, "DST_IP_COUNTRY" },
    { 104 + NTOP_BASE, "DST_IP_CITY" },
    { 105 + NTOP_BASE, "FLOW_PROTO_PORT" },
    { 106 + NTOP_BASE, "TUNNEL_ID" },
    { 107 + NTOP_BASE, "LONGEST_FLOW_PKT" },
    { 108 + NTOP_BASE, "SHORTEST_FLOW_PKT" },
    { 109 + NTOP_BASE, "RETRANSMITTED_IN_PKTS" },
    { 110 + NTOP_BASE, "RETRANSMITTED_OUT_PKTS" },
    { 111 + NTOP_BASE, "OOORDER_IN_PKTS" },
    { 112 + NTOP_BASE, "OOORDER_OUT_PKTS" },
    { 113 + NTOP_BASE, "UNTUNNELED_PROTOCOL" },
    { 114 + NTOP_BASE, "UNTUNNELED_IPV4_SRC_ADDR" },
    { 115 + NTOP_BASE, "UNTUNNELED_L4_SRC_PORT" },
    { 116 + NTOP_BASE, "UNTUNNELED_IPV4_DST_ADDR" },
    { 117 + NTOP_BASE, "UNTUNNELED_L4_DST_PORT" },
    { 120 + NTOP_BASE, "DUMP_PATH" },
    { 130 + NTOP_BASE, "SIP_CALL_ID" },
    { 131 + NTOP_BASE, "SIP_CALLING_PARTY" },
    { 132 + NTOP_BASE, "SIP_CALLED_PARTY" },
    { 133 + NTOP_BASE, "SIP_RTP_CODECS" },
    { 134 + NTOP_BASE, "SIP_INVITE_TIME" },
    { 135 + NTOP_BASE, "SIP_TRYING_TIME" },
    { 136 + NTOP_BASE, "SIP_RINGING_TIME" },
    { 137 + NTOP_BASE, "SIP_OK_TIME" },
    { 138 + NTOP_BASE, "SIP_BYE_TIME" },
    { 139 + NTOP_BASE, "SIP_RTP_SRC_IP" },
    { 140 + NTOP_BASE, "SIP_RTP_SRC_PORT" },
    { 141 + NTOP_BASE, "SIP_RTP_DST_IP" },
    { 142 + NTOP_BASE, "SIP_RTP_DST_PORT" },
    { 150 + NTOP_BASE, "RTP_FIRST_SSRC" },
    { 151 + NTOP_BASE, "RTP_FIRST_TS" },
    { 152 + NTOP_BASE, "RTP_LAST_SSRC" },
    { 153 + NTOP_BASE, "RTP_LAST_TS" },
    { 154 + NTOP_BASE, "RTP_IN_JITTER" },
    { 155 + NTOP_BASE, "RTP_OUT_JITTER" },
    { 156 + NTOP_BASE, "RTP_IN_PKT_LOST" },
    { 157 + NTOP_BASE, "RTP_OUT_PKT_LOST" },
    { 158 + NTOP_BASE, "RTP_OUT_PAYLOAD_TYPE" },
    { 159 + NTOP_BASE, "RTP_IN_MAX_DELTA" },
    { 160 + NTOP_BASE, "RTP_OUT_MAX_DELTA" },
    { 165 + NTOP_BASE, "L7_PROTO" },
    { 180 + NTOP_BASE, "HTTP_URL" },
    { 181 + NTOP_BASE, "HTTP_RET_CODE" },
    { 182 + NTOP_BASE, "HTTP_REFERER" },
    { 183 + NTOP_BASE, "HTTP_UA" },
    { 184 + NTOP_BASE, "HTTP_MIME" },
    { 185 + NTOP_BASE, "SMTP_MAIL_FROM" },
    { 186 + NTOP_BASE, "SMTP_RCPT_TO" },
    { 195 + NTOP_BASE, "MYSQL_SERVER_VERSION" },
    { 196 + NTOP_BASE, "MYSQL_USERNAME" },
    { 197 + NTOP_BASE, "MYSQL_DB" },
    { 198 + NTOP_BASE, "MYSQL_QUERY" },
    { 199 + NTOP_BASE, "MYSQL_RESPONSE" },
    { 0, NULL }
};

static const value_string v9_scope_field_types[] = {
    { 1, "System" },
    { 2, "Interface" },
    { 3, "Line Card" },
    { 4, "NetFlow Cache" },
    { 5, "Template" },
    { 0, NULL }
};

static const value_string v9_sampler_mode[] = {
    { 0, "Deterministic" },
    { 1, "Unknown" },  /* "Time-Based" ?? */
    { 2, "Random" },
    { 0, NULL }
};

static const value_string v9_direction[] = {
    { 0, "Ingress" },
    { 1, "Egress" },
    { 0, NULL }
};
