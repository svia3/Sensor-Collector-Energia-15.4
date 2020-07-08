#ifndef _FifteenDotFourCollector_h
#define _FifteenDotFourCollector_h

#include <software_stack/ti15_4stack/stack_user_api/api_mac/api_mac.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <software_stack/ti15_4stack/radio_configuration/mac_user_config.h>
#include <utils/buffer_c/buffer.h>

#define MIN_PERSISTENCE_TIME_USEC 2000000
#define INDIRECT_PERSISTENT_TIME (MAX((5 * 1000 * CONFIG_POLLING_INTERVAL / 2), MIN_PERSISTENCE_TIME_USEC)/ \
                                  (BASE_SUPER_FRAME_DURATION * \
                                   SYMBOL_DURATION_50_kbps))

#define CLLC_OFFSET_TIMESLOT     0
#define CLLC_EBEACONORDER        15
#define CLLC_NBPANEBEACONORDER   16383

#define START_EVENT (1 << 0)
#define OPEN_EVENT (1 << 1)

/*
 *     memset(Cllc_associatedDevList, 0xFF,
           (sizeof(Cllc_associated_devices_t) * CONFIG_MAX_DEVICES));
 *
 */
class FifteenDotFourCollector
{
    public:
        FifteenDotFourCollector(void);
        void begin(void);
        void start(void);
        void setChannel(uint8_t c) {channel = c;};
        uint8_t getChannel(void) {return channel;};
        void setPanID(uint16_t id) {panID = id;};
        uint16_t getPanID(void) {return panID;};
        void getAddressExt(ApiMac_sAddrExt_t *addr) {memcpy(addr, address.addr.extAddr, sizeof(ApiMac_sAddrExt_t));};
        void setAddressExt(ApiMac_sAddrExt_t *addr) {memcpy(address.addr.extAddr, addr, sizeof(ApiMac_sAddrExt_t));};
        void process(void);
        void beginTransmission(uint16_t address);
        bool endTransmission();
        /* API MAC Callbacks */
        static void orphanIndCb(ApiMac_mlmeOrphanInd_t *pData);
        static void assocIndCb(ApiMac_mlmeAssociateInd_t *pData);
        static void startCnfCb(ApiMac_mlmeStartCnf_t *pData);
        static void disassocIndCb(ApiMac_mlmeDisassociateInd_t *pData);
        static void disassocCnfCb(ApiMac_mlmeDisassociateCnf_t *pData);
        static void commStatusIndCB(ApiMac_mlmeCommStatusInd_t *pCommStatusInd);
        static void pollIndCB(ApiMac_mlmePollInd_t *pPollInd);
        static void dataCnfCB(ApiMac_mcpsDataCnf_t *pDataCnf);
        static void dataIndCB(ApiMac_mcpsDataInd_t *pDataInd);
        static void beaconNotifyIndCb(ApiMac_mlmeBeaconNotifyInd_t *pData);
        static void scanCnfCb(ApiMac_mlmeScanCnf_t *pData);
        uint16_t revents = 0;
    private:
        uint8_t channel = 0;
        uint16_t panID = 0x0001;
        /* We assume short address mode */
        ApiMac_sAddr_t address ={{.shortAddr = 0xAABB}, ApiMac_addrType_short};
    protected:
        Semaphore_Handle sem;
};
#endif
