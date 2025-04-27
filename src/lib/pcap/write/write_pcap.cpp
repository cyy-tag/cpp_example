#include <pcap.h>
#include <iostream>
#include <cstring>

void handle_packet(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    std::string filename = "capture";
    char timestamp[20];
    pcap_t *fp;

    // 将时间戳转换为字符串
    snprintf(timestamp, sizeof(timestamp), "%lu", pkthdr->ts.tv_sec);

    // 创建或打开PCAP文件
    filename += "-";
    filename += timestamp;
    filename += ".pcap";
    if ((fp = pcap_open_dead(PCAP_VERSION_MAJOR, 65535, PCAP_OPENFLAG_WRITE | PCAP_OPENFLAG_NOCAPTURE_LOCAL)) == NULL) {
        std::cerr << "Error opening pcap file\n";
        return;
    }

    // 写入PCAP文件头
    struct pcap_file_header pfh = {
        0xa1b2c3d4,    // Magic number
        0x0002,         // Major version
        0x0004,         // Minor version
        0,              // Timezone
        0,              // Sigfigs
        65535,          // Snap length
        1               // Network type (Ethernet)
    };
    pcap_dumper_t *dumper = pcap_dump_open(fp, filename.c_str());
    if (!dumper) {
        std::cerr << "Error opening pcap dump file\n";
        pcap_close(fp);
        return;
    }

    // 写入数据包
    pcap_dump((u_char *) dumper, pkthdr, packet);

    // 关闭文件
    pcap_dump_close(dumper);
    pcap_close(fp);
}

int main(int argc, char **argv) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <interface>" << std::endl;
        return -1;
    }

    handle = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        std::cerr << "Couldn't open device " << argv[1] << ": " << errbuf << std::endl;
        return -2;
    }

    pcap_loop(handle, -1, handle_packet, NULL);

    pcap_close(handle);
    return 0;
}
