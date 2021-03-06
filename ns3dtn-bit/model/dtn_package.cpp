#include "dtn_package.h"

namespace ns3 {
    namespace ns3dtnbit {
        // BPHeader
        NS_OBJECT_ENSURE_REGISTERED(BPHeader);

        BPHeader::BPHeader() {

        }

        TypeId BPHeader::GetTypeId() {
            static TypeId tid = TypeId("ns3::ns3dtnbit::BPHeader")
                .SetParent<Header>()
                .AddConstructor<BPHeader>();
            return tid;
        }

        TypeId BPHeader::GetInstanceTypeId() const {
            return GetTypeId();
        }

        void BPHeader::Serialize(Buffer::Iterator start) const {
            start.WriteU32(static_cast<std::underlying_type<BundleType>::type>
                    (bundle_type_));
            start.WriteU32(hop_count_);
            start.WriteU32(spray_);
            start.WriteU32(retransmission_count_);
            WriteTo(start, destination_ip_);
            WriteTo(start, source_ip_);
            start.WriteU32(source_seqno_);
            start.WriteU32(payload_size_);
            start.WriteU32(offset_size_);
            start.WriteU32(src_time_stamp_);
            start.WriteU32(hop_time_stamp_);
            start.WriteU32(hop_ips_.size());
            for (auto const & ip : hop_ips_) {
                WriteTo(start, ip);
            }
        }

        uint32_t BPHeader::Deserialize(Buffer::Iterator start) {
            Buffer::Iterator i = start;
            // read for bundle_type_, TODO
            int ttt = i.ReadU32();
            bundle_type_ = 
                ttt == static_cast<std::underlying_type<BundleType>::type>(BundleType::BundlePacket) 
                ? BundleType::BundlePacket :
                ttt == static_cast<std::underlying_type<BundleType>::type>(BundleType::AntiPacket) 
                ? BundleType::AntiPacket :
                ttt == static_cast<std::underlying_type<BundleType>::type>(BundleType::HelloPacket)
                ? BundleType::HelloPacket :
                ttt == static_cast<std::underlying_type<BundleType>::type>(BundleType::TransmissionAck)
                ? BundleType::TransmissionAck :
                ttt == static_cast<std::underlying_type<BundleType>::type>(BundleType::StorageinfoMaintainPkt)
                ? BundleType::StorageinfoMaintainPkt :
                BundleType::UnKnow;
            hop_count_ = i.ReadU32();
            spray_ = i.ReadU32();
            retransmission_count_ = i.ReadU32();
            ReadFrom(i, destination_ip_);
            ReadFrom(i, source_ip_);
            source_seqno_ = i.ReadU32();
            payload_size_ = i.ReadU32();
            offset_size_ = i.ReadU32();
            src_time_stamp_ = i.ReadU32();
            hop_time_stamp_ = i.ReadU32();
            size_t hopips_size;
            hopips_size = i.ReadU32();
            hop_ips_.clear();
            for (size_t j = 0 ; j < hopips_size; j++) {
                Ipv4Address iptmp;
                ReadFrom(i, iptmp);
                hop_ips_.push_back(iptmp);
            }

            uint32_t dist = i.GetDistanceFrom(start);
           // std::cout << "dist = " << dist << ", GetSeri = " << GetSerializedSize() << "hopips_size=" << hopips_size<< std::endl;
            NS_ASSERT(dist == GetSerializedSize());
            return dist;
        }

        uint32_t BPHeader::GetSerializedSize() const {
            return 48 + (4 * hop_ips_.size());
        }

        void BPHeader::Print(std::ostream& os) const {
            string bt;
            bt = bundle_type_ == BundleType::BundlePacket ? "BundlePacket" : 
                bundle_type_ == BundleType::AntiPacket ? "AntiPacket" :
                bundle_type_ == BundleType::HelloPacket ? "HelloPacket" :
                bundle_type_ == BundleType::TransmissionAck ? "TransmissionAck" :
                bundle_type_ == BundleType::StorageinfoMaintainPkt ? "StorageinfoMaintainPkt" :
                "Unknown, ERROR?";

            os << ",destination ip=" << destination_ip_
                << ",source ip=" << source_ip_
                << ",source seqno=" << source_seqno_
                << ",payload size=" << payload_size_
                << ",offset size=" << offset_size_
                << ",src time stamp=" << get_src_time_stamp().GetSeconds()
                << ",hop time stamp=" << get_hop_time_stamp().GetSeconds()
                << ",hop ip=" << hop_ips_[hop_ips_.size() - 1]
                << ",bundle type=" << bt
                << std::endl;
        }

        std::ostream& operator<<(std::ostream& os, BPHeader const& rh) {
            rh.Print(os);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, BundleType&& rh) {
            string bt;
            bt = rh == BundleType::BundlePacket ? "BundlePacket" : 
                rh == BundleType::AntiPacket ? "AntiPacket" :
                rh == BundleType::HelloPacket ? "HelloPacket" :
                rh == BundleType::TransmissionAck ? "TransmissionAck" :
                rh == BundleType::StorageinfoMaintainPkt ? "StorageinfoMaintainPkt" :
                "Unknown, ERROR?";
            os << bt;
            return os;
        }

    } /* ns3dtnbit */ 

} /* ns3  */ 
