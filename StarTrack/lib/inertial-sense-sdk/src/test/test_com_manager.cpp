#include <gtest/gtest.h>
#include <deque>
#include "../com_manager.h"
#include "../ring_buffer.h"
#include "../protocol_nmea.h"


#if 0
extern "C"
{
#include "../../../libs-int/rtklib/src/rtklib.h"
	extern gtime_t g_gps_latest_time;
	extern int decode_rtcm3(rtcm_t *rtcm);
	extern int decode_ubx(raw_t* raw, int doChecksum);
}
#endif

#define TEST_PROTO_IS		1
#define TEST_PROTO_ASCII	1
#define TEST_PROTO_UBLOX	1
#define TEST_PROTO_RTCM3	1

#define TASK_PERIOD_MS		1				// 1 KHz
#if 0
#define DEBUG_PRINTF	printf
#else
#define DEBUG_PRINTF	
#endif

#define PORT_BUFFER_SIZE	8192

typedef struct
{
	com_manager_t			cm;
	com_manager_status_t	cmBufStatus[NUM_COM_PORTS] = { 0 };
	broadcast_msg_t			cmBufBcastMsg[MAX_NUM_BCAST_MSGS] = { 0 };
	struct  
	{
		dev_info_t			devInfo;
		nvm_flash_cfg_t		nvmFlashCfg;
		ascii_msgs_t		asciiMsgs;
	}						msgs = { 0 };

	// Used to simulate serial ports
	ring_buf_t				portRxBuf;
	uint8_t					portRxBuffer[PORT_BUFFER_SIZE];
	ring_buf_t				portTxBuf;
	uint8_t					portTxBuffer[PORT_BUFFER_SIZE];
} test_data_t;

typedef struct
{
	protocol_type_t	ptype;	// Data start byte
	uint32_t		did;
	union
	{
		uint8_t		buf[PKT_BUF_SIZE];
		uDatasets	set;
	}				data;
	uint32_t		size;		// size of data
	uint32_t		pktSize;	// size of encoded packet (pkt header, data header, data, pkt footer)
} data_holder_t;

test_data_t tcm = {};
std::deque<data_holder_t> g_testRxDeque;
std::deque<data_holder_t> g_testTxDeque;


int portRead(CMHANDLE cmHandle, int pHandle, unsigned char* buf, int len)
{
	return ringBufRead(&tcm.portRxBuf, buf, len);
}

int portWrite(CMHANDLE cmHandle, int pHandle, unsigned char* buf, int len)
{
	if (ringBufWrite(&tcm.portTxBuf, buf, len))
	{	// Buffer overflow
		DEBUG_PRINTF("tcm.portTxBuf ring buffer overflow: %d !!!\n", ringBufUsed(&tcm.portTxBuf) + len);
		EXPECT_TRUE(false);
	}
	return len;
}

void postRxRead(CMHANDLE cmHandle, int pHandle, p_data_t* dataRead)
{
	data_holder_t td = g_testRxDeque.front();
	g_testRxDeque.pop_front();

	DEBUG_PRINTF("[%2d] postRxRead() DID: %3d, size: %3d\n", (int)g_testRxDeque.size(), td.did, td.size);

	EXPECT_EQ(td.did, dataRead->hdr.id);
	EXPECT_EQ(td.size, dataRead->hdr.size);
	EXPECT_TRUE(memcmp(&td.data, dataRead->buf, td.size)==0);
}

void disableBroadcasts(CMHANDLE cmHandle, int pHandle)
{
}

int prepDevInfo(CMHANDLE cmHandle, int pHandle, p_data_hdr_t* dataHdr)
{
	return 1;
}

void writeNvrUserpageFlashCfg(CMHANDLE cmHandle, int pHandle, p_data_t* data)
{
}

// return 1 on success, 0 on failure
int msgHandlerAscii(CMHANDLE cmHandle, int pHandle, const uint8_t* msg, int msgSize)
{
	int messageIdUInt = ASCII_MESSAGEID_TO_UINT(msg + 1);
// 	comWrite(pHandle, line, lineLength); // echo back
// 	time_delay_msec(50); // give time for the echo to come back

	if (msgSize == 10)
	{	// 4 character commands (i.e. "$STPB*14\r\n")
		switch (messageIdUInt)
		{
		case ASCII_MSG_ID_ASCB:	// query ASCII message broadcast rates
		// 		writeAsciiBcastPeriod(cmHandle, pHandle, NULLPTR);
			break;

		case ASCII_MSG_ID_STPB: // stop all broadcasts on all ports
			disableBroadcasts(cmHandle, -1);
			break;

		case ASCII_MSG_ID_STPC: // stop all broadcasts on current port
			disableBroadcasts(cmHandle, pHandle);
			break;

		case ASCII_MSG_ID_BLEN: // bootloader enable
			break;

		case ASCII_MSG_ID_SRST: // soft reset
			break;

		case ASCII_MSG_ID_INFO: // query device version information
			break;

		case ASCII_MSG_ID_PERS: // Save persistent messages to flash memory
			break;
		}
	}
	else
	{	// General ASCII messages
		switch (messageIdUInt)
		{
		case ASCII_MSG_ID_NELB: // SAM bootloader assistant (SAM-BA) enable
// 			if (msgSize == 22 &&	// 16 character commands (i.e. "$NELB,!!SAM-BA!!\0*58\r\n")
// 				(pHandle == COM0_PORT_NUM || pHandle == USB_PORT_NUM) &&
// 				strncmp((const char*)(msg + 6), "!!SAM-BA!!", 6) == 0)
// 			{
// 			}
			break;
		}
	}

	data_holder_t td = g_testRxDeque.front();
	g_testRxDeque.pop_front();
	uint8_t buf[30] = {};
	memcpy(buf, msg, (msgSize<29?msgSize:29));
	DEBUG_PRINTF("[%2d] asciiMessageHandler() size: %3d, %.30s...\n", (int)g_testRxDeque.size(), msgSize, buf);

	EXPECT_EQ(td.size, msgSize);
	EXPECT_TRUE(memcmp(&td.data, msg, td.size) == 0);

	return 0;
}


int msgHandlerUblox(CMHANDLE cmHandle, int pHandle, const uint8_t* msg, int msgSize)
{
	data_holder_t td = g_testRxDeque.front();
	g_testRxDeque.pop_front();
	DEBUG_PRINTF("[%2d] ubloxMessageHandler() size: %3d, (0x%02x 0x%02x)\n", (int)g_testRxDeque.size(), msgSize, msg[2], msg[3]);

	EXPECT_EQ(td.size, msgSize);
	EXPECT_TRUE(memcmp(&td.data, msg, td.size) == 0);

#if 0
	raw_t raw = {};
	obsd_t obsData[100];
	raw.obs.data = obsData;
	raw.obs.nmax = raw.obs.n = 100;
	raw.buff = (unsigned char*)msg;
	raw.len = msgSize;

	int j = 0;
	switch (decode_ubx(&raw, 0))
	{
	case DATA_TYPE_OBSERVATION:
	case DATA_TYPE_EPHEMERIS:
	case DATA_TYPE_SBS:
	case DATA_TYPE_ANTENNA_POSITION:
	case DATA_TYPE_ION_UTC_ALMANAC:
		j = 2;
		break;

	default:
	case DATA_TYPE_ERROR:
		j = 5;
		break;
	}
#endif

	return 0;
}


int msgHandlerRtcm3(CMHANDLE cmHandle, int pHandle, const uint8_t* msg, int msgSize)
{
	data_holder_t td = g_testRxDeque.front();
	g_testRxDeque.pop_front();
	DEBUG_PRINTF("[%2d] rtcm3MessageHandler() size: %3d, (0x%02x 0x%02x 0x%02x 0x%02x)\n", (int)g_testRxDeque.size(), msgSize, msg[0], msg[1], msg[2], msg[3]);

	EXPECT_EQ(td.size, msgSize);
	EXPECT_TRUE(memcmp(&td.data, msg, td.size) == 0);

#if 0
	rtcm_t raw = {};
	obsd_t obsData[100];
	raw.obs.data = obsData;
	raw.obs.nmax = raw.obs.n = 100;
	raw.buff = (unsigned char*)msg;
	raw.len = msgSize;

	int j = 0;
	switch (decode_rtcm3(&raw))
	{
	case DATA_TYPE_OBSERVATION:
	case DATA_TYPE_EPHEMERIS:
	case DATA_TYPE_SBS:
	case DATA_TYPE_ANTENNA_POSITION:
	case DATA_TYPE_ION_UTC_ALMANAC:
		j = 2;
		break;

	default:
	case DATA_TYPE_ERROR:
		j = 5;
		break;
	}
#endif

	return 0;
}


#define NUM_HANDLES			1
static is_comm_instance_t   s_comm[NUM_HANDLES] = { 0 };
static uint8_t				s_comm_buffer[NUM_HANDLES*PKT_BUF_SIZE] = { 0 };
static com_manager_port_t	s_cmPort = {};

bool initComManager(test_data_t &t)
{
	// Init ComManager
	com_manager_init_t cmInit = {};
	cmInit.broadcastMsg = t.cmBufBcastMsg;
	cmInit.broadcastMsgSize = sizeof(t.cmBufBcastMsg);
	if (comManagerInitInstance(&(t.cm), NUM_HANDLES, 0, TASK_PERIOD_MS, 0, portRead, portWrite, 0, postRxRead, 0, disableBroadcasts, &cmInit, &s_cmPort))
	{	// Fail to init
		return false;
	}

	comManagerRegisterInstance(&(t.cm), DID_DEV_INFO, prepDevInfo, 0, &(t.msgs.devInfo), 0, sizeof(dev_info_t), 0);
	comManagerRegisterInstance(&(t.cm), DID_FLASH_CONFIG, 0, writeNvrUserpageFlashCfg, &t.msgs.nvmFlashCfg, 0, sizeof(nvm_flash_cfg_t), 0);

	comManagerSetCallbacksInstance(&(t.cm), NULL, msgHandlerAscii, msgHandlerUblox, msgHandlerRtcm3);

	// Enable/disable protocols
	s_cmPort.comm.config.enableISB = TEST_PROTO_IS;
	s_cmPort.comm.config.enableASCII = TEST_PROTO_ASCII;
	s_cmPort.comm.config.enableUblox = TEST_PROTO_UBLOX;
	s_cmPort.comm.config.enableRTCM3 = TEST_PROTO_RTCM3;

	return true;
}

bool init(test_data_t &t)
{
	// Init Port Buffers
	ringBufInit(&(t.portTxBuf), t.portTxBuffer, sizeof(t.portTxBuffer), 1);
	ringBufInit(&(t.portRxBuf), t.portRxBuffer, sizeof(t.portRxBuffer), 1);

	return initComManager(t);
}


void generateData(std::deque<data_holder_t> &testDeque)
{
	testDeque.clear();
	int byteSize = 0;

	DEBUG_PRINTF("===============  generateData()  ===============\n");

	// Generate data and add to test deque
	for (int i = 0; ; i++)
	{
		data_holder_t td = {};
		ins_1_t ins1 = { 0 };
		gps_pos_t gps = { 0 };

		int j = i % 17;
		switch (j)
		{	
		case 0:
		case 1:
		case 2:
		case 3:
			// INS 1
			ins1.timeOfWeek = (double)i;
			ins1.week = i;
			ins1.insStatus = i;
			ins1.hdwStatus = i;
			ins1.theta[0] = i * 2.0f;
			ins1.theta[1] = i * 3.0f;
			ins1.theta[2] = i * 4.0f;
			ins1.uvw[0] = i * 5.0f;
			ins1.uvw[1] = i * 6.0f;
			ins1.uvw[2] = i * 7.0f;
			ins1.lla[0] = 40.330565516;
			ins1.lla[1] = -111.725787806;
			ins1.lla[2] = 1408.565264;
			ins1.ned[0] = i * 1.234f;
			ins1.ned[1] = i * 2.345f;
			ins1.ned[2] = i * 3.456f;

			if (j == 3 )
			{	// ASCII
#if TEST_PROTO_ASCII
				td.ptype = _PTYPE_ASCII_NMEA;
				td.size = did_ins1_to_nmea_pins1((char*)td.data.buf, sizeof(td.data.buf), ins1);
#endif
			}
			else
			{	// Binary
#if TEST_PROTO_IS
				td.did = DID_INS_1;
				td.ptype = _PTYPE_INERTIAL_SENSE_DATA;
				td.data.set.ins1 = ins1;
				td.size = sizeof(ins_1_t);
#endif
			}
			break;

		case 4:
		case 5:		// GPS
			gps.timeOfWeekMs = i * 1000;
			gps.week = i * 10;
			gps.status = i;
			gps.ecef[0] = (double)i*1.234;
			gps.ecef[1] = (double)i*2.345;
			gps.ecef[2] = (double)i*3.456;
			gps.lla[0] = (double)i*1.234;
			gps.lla[1] = (double)i*2.345;
			gps.lla[2] = (double)i*3.456;
			gps.hAcc = (float)i;
			gps.cnoMean = (float)i;
			gps.hMSL = (float)i;
			gps.pDop = (float)i;
			gps.towOffset = (double)i*123.4;
			gps.leapS = (uint8_t)i;

			if ((j == 5 || TEST_PROTO_IS == 0) && TEST_PROTO_ASCII)
			{	// ASCII
#if TEST_PROTO_ASCII
				td.ptype = _PTYPE_ASCII_NMEA;
				td.size = did_gps_to_nmea_gga((char*)td.data.buf, sizeof(td.data.buf), gps);
#endif
			}
			else
			{	// Binary
#if TEST_PROTO_IS
				td.did = DID_GPS1_POS;
				td.ptype = _PTYPE_INERTIAL_SENSE_DATA;
				td.data.set.gpsPos = gps;
				td.size = sizeof(gps_pos_t);
#endif
			}
			break;

#if TEST_PROTO_ASCII
		case 6:		// ASCII command - Stop all broadcasts
			td.ptype = _PTYPE_ASCII_NMEA;
			td.size = snprintf((char*)td.data.buf, PKT_BUF_SIZE, "$STPB*15\r\n");
			break;

		case 7:		// ASCII command - Stop broadcast on current port
			td.ptype = _PTYPE_ASCII_NMEA;
			td.size = snprintf((char*)td.data.buf, PKT_BUF_SIZE, "$STPC*14\r\n");
			break;
#endif

#if TEST_PROTO_UBLOX

		case 8:		// Ublox - UBX-NAV-POSLLH (0x01 0x02)
			td.ptype = _PTYPE_UBLOX;
			td.size = 36;
			{
				uint8_t buf[] = { 0xb5,0x62,0x1,0x2,0x1c,0x0,0x0,0xa1,0xad,0x10,0x6a,0xff,0x67,0xbd,0xb7,0xf4,0x9,0x18,0x35,0x7e,0x15,0x0,0xe8,0xc5,0x15,0x0,0x4f,0x1,0x0,0x0,0xa8,0x1,0x0,0x0,0x59,0xbc };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;

		case 9:		// Ublox - UBX-RXM-RAWX (0x02 0x15)
			td.ptype = _PTYPE_UBLOX;
			td.size = 952;
			{
				uint8_t buf[] = { 0xb5,0x62,0x2,0x15,0xb0,0x3,0x1b,0x2f,0xdd,0x24,0x1b,0x14,0x11,0x41,0x2b,0x8,0x12,0x1d,0x1,0x1,0x82,0x9,0x5d,0x61,0x9b,0x3f,0xab,0xb,0x80,0x41,0x60,0x1a,0xa8,0x8a,0x7c,0x14,0xa5,0x41,0xa8,0x6e,0x9c,0x43,0x1,0x85,0x0,0x0,0xf4,0xfb,0x2c,0x5,0x1,0x8,0xf,0x0,0x39,0xe2,0x7d,0x36,0x68,0xc,0x83,0x41,0x45,0xc6,0xb1,0x7,0x6c,0x6,0xa9,0x41,0xa0,0xe6,0xc,0x44,0x5,0x2,0x0,0x0,0xf4,0xfb,0x25,0x6,0x1,0x8,0x7,0x0,0x56,0x5c,0xa2,0x55,0x34,0x9f,0x70,0x41,0x24,0xe2,0x8b,0xa8,0x50,0xd6,0x95,0x41,0x34,0x1d,0x31,0x45,0x0,0x5,0x0,0x0,0xf4,0xfb,0x30,0x5,0x1,0x8,0x7,0x0,0x1a,0xaa,0xe6,0x47,0x4,0xa7,0x73,0x41,0x5c,0x99,0x57,0xbc,0x8b,0xd1,0x99,0x41,0x96,0xcd,0x25,0xc5,0x0,0x13,0x0,0x0,0xf4,0xfb,0x26,0x5,0x1,0x8,0x7,0x0,0x79,0x9,0xfb,0xf3,0x4a,0x11,0x70,0x41,0x8e,0x98,0x96,0x32,0xe0,0x1b,0x95,0x41,0xb2,0x6,0xcf,0x44,0x0,0x19,0x0,0x0,0xf4,0xfb,0x2c,0x5,0x1,0x8,0x7,0x0,0x28,0x7c,0xed,0xa,0xb2,0xc3,0x73,0x41,0x7e,0x1f,0x51,0xa7,0x37,0xf7,0x99,0x41,0x16,0x48,0x95,0x44,0x2,0x1,0x0,0x0,0xf4,0xfb,0x29,0x5,0x1,0x8,0x7,0x0,0xe6,0x7a,0x56,0x4c,0x6f,0x33,0x6f,0x41,0xbe,0xd9,0x7f,0x82,0x8c,0xd3,0x94,0x41,0x20,0x25,0x5d,0x44,0x6,0x9,0x0,0x5,0xf4,0xfb,0x26,0x5,0x1,0x8,0x7,0x0,0xa9,0x32,0x13,0x58,0xfd,0x3e,0x6e,0x41,0xa7,0x9f,0x32,0xb6,0x77,0x39,0x94,0x41,0x7c,0x44,0x12,0x44,0x6,0x13,0x0,0xa,0xf4,0xfb,0x21,0x7,0x2,0x8,0x7,0x0,0xb,0x37,0x40,0xe6,0x53,0x54,0x74,0x41,0x50,0xb1,0xc8,0x15,0x3b,0xb5,0x9a,0x41,0xeb,0x3f,0x11,0x45,0x2,0x8,0x0,0x0,0xf4,0xfb,0x27,0x5,0x1,0x8,0x7,0x0,0x62,0xb8,0x14,0xb2,0x1a,0xd,0x72,0x41,0x6e,0xb2,0xbc,0x21,0x4,0xb7,0x97,0x41,0x5e,0x39,0xa5,0x44,0x2,0x1a,0x0,0x0,0xf4,0xfb,0x2a,0x5,0x1,0x8,0x7,0x0,0xa2,0xe1,0xa9,0xf7,0x62,0xe1,0x70,0x41,0x44,0x73,0xc5,0xd4,0x2d,0x97,0x96,0x41,0x1,0xff,0xf4,0x44,0x6,0x3,0x0,0xc,0xf4,0xfb,0x2b,0x5,0x1,0x8,0x7,0x0,0x18,0x57,0x42,0xb8,0x58,0x5e,0x73,0x41,0x68,0x95,0x61,0xc0,0x11,0x72,0x99,0x41,0xc2,0xe6,0xcb,0xc4,0x2,0x15,0x0,0x0,0xf4,0xfb,0x2a,0x5,0x1,0x8,0x7,0x0,0x43,0x99,0x17,0x5c,0xb5,0x22,0x72,0x41,0x8,0xc6,0x9b,0x64,0x5f,0x47,0x98,0x41,0x1e,0x2e,0x86,0x45,0x6,0x4,0x0,0xd,0xf4,0xfb,0x26,0x6,0x1,0x8,0x7,0x0,0x20,0xf5,0x6a,0x72,0xd9,0x46,0x77,0x41,0x42,0xe2,0x94,0x2e,0x7d,0x94,0x9e,0x41,0xd5,0xbb,0x3f,0x45,0x2,0x1f,0x0,0x0,0xf4,0xfb,0x23,0x6,0x1,0x8,0x7,0x0,0x74,0x1,0x2f,0x4d,0x45,0x5f,0x71,0x41,0x2c,0xa5,0xec,0x21,0xb2,0x28,0x97,0x41,0x5b,0x67,0x27,0x45,0x6,0xff,0x0,0x1,0xc0,0x12,0x27,0x5,0x1,0x8,0xf,0x0,0x7,0x49,0x2b,0x5d,0x78,0x3d,0x76,0x41,0x8,0xff,0xb7,0x8e,0xd8,0x37,0x9d,0x41,0xa4,0xc2,0xfa,0xc4,0x2,0xf,0x0,0x0,0xf4,0xfb,0x25,0x5,0x1,0x8,0x7,0x0,0x37,0x34,0x4e,0x78,0xb4,0xe,0x72,0x41,0xe5,0x50,0xde,0xfc,0x1e,0xb9,0x97,0x41,0x33,0xd4,0x35,0x45,0x0,0x1d,0x0,0x0,0xf4,0xfb,0x29,0x5,0x1,0x8,0x7,0x0,0x7b,0x1e,0x56,0x93,0xc0,0x37,0x72,0x41,0x95,0x61,0x9f,0xee,0xa,0xef,0x97,0x41,0xec,0x17,0x2a,0xc5,0x0,0x6,0x0,0x0,0xf4,0xfb,0x2a,0x5,0x1,0x8,0xf,0x0,0xe4,0x3e,0x36,0xf9,0x6,0xdf,0x7f,0x41,0xf5,0x4a,0xe7,0xa1,0x7e,0xef,0xa4,0x41,0x20,0xe0,0x9a,0x43,0x1,0x8a,0x0,0x0,0xf4,0xfb,0x2b,0x5,0x1,0x8,0xf,0x0,0xd,0xbd,0xcc,0xd2,0xc6,0xcd,0x72,0x41,0x9c,0x48,0x6a,0x1c,0x24,0xb4,0x98,0x41,0xd8,0xdd,0x5e,0xc4,0x2,0xd,0x0,0x0,0xf4,0xfb,0x28,0x5,0x1,0x8,0x7,0x0,0xaa,0x1b,0x81,0x57,0xb1,0x3f,0x74,0x41,0x4c,0xe0,0x4e,0x1e,0x1e,0x9a,0x9a,0x41,0xa9,0x3b,0x25,0xc5,0x0,0x18,0x0,0x0,0xf4,0xfb,0x24,0x6,0x1,0x8,0xf,0x0,0x22,0x41,0x5e,0xea,0xaf,0x9f,0x70,0x41,0xcb,0x9f,0xa7,0x62,0x45,0x33,0x96,0x41,0xe0,0xe9,0x5a,0xc5,0x6,0x10,0x0,0x6,0xf4,0xfb,0x24,0x6,0x1,0x8,0xf,0x0,0xce,0x25,0x25,0xa7,0xcf,0x6f,0x70,0x41,0x4e,0x82,0x95,0x5c,0xc,0x98,0x95,0x41,0x8e,0x1c,0x91,0xc4,0x0,0x2,0x0,0x0,0xf4,0xfb,0x2c,0x5,0x1,0x8,0x7,0x0,0x15,0x97,0x78,0xd7,0x1c,0x13,0x72,0x41,0x93,0xcc,0x58,0x82,0xc9,0x1c,0x98,0x41,0xae,0x75,0xae,0xc4,0x6,0x2,0x0,0x3,0xf4,0xfb,0x2a,0x5,0x1,0x8,0xf,0x0,0x71,0x2d,0xf8,0xb7,0xdd,0x23,0x72,0x41,0xd,0xbb,0xcd,0x6b,0x34,0x40,0x98,0x41,0xe1,0xa1,0x38,0x45,0x6,0x14,0x0,0x9,0xf4,0xfb,0x1e,0x8,0x3,0x8,0xf,0x0,0xba,0x40,0xb0,0xd2,0x7d,0x56,0x76,0x41,0xd7,0x44,0xd7,0xff,0xb7,0x58,0x9d,0x41,0x16,0x2f,0x35,0xc5,0x2,0x1b,0x0,0x0,0xf4,0xfb,0x24,0x5,0x1,0x8,0x7,0x0,0x36,0xb9,0xcd,0x0,0x91,0x19,0x74,0x41,0xf2,0xd,0x57,0x62,0x8,0x68,0x9a,0x41,0x8c,0x29,0xb7,0x44,0x0,0x1f,0x0,0x0,0xf4,0xfb,0x29,0x5,0x1,0x8,0xf,0x0,0xfe,0x54,0xb5,0xd5,0x99,0x2e,0x70,0x41,0xa0,0x27,0x9e,0x1c,0x5e,0x98,0x95,0x41,0x7,0x51,0xe5,0xc4,0x6,0x12,0x0,0x4,0xf4,0xfb,0x2c,0x5,0x1,0x8,0xf,0x0,0xd7,0x76,0x9,0xb8,0xcd,0xe7,0x6e,0x41,0x92,0x38,0xc7,0xe,0x19,0x4d,0x94,0x41,0x50,0x98,0x5a,0xc4,0x0,0xc,0x0,0x0,0xf4,0xfb,0x30,0x5,0x1,0x8,0xf,0x0,0xa7,0xd5 };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;

		case 10:	// Ublox - UBX-NAV-POSLLH (0x01 0x02)
			td.ptype = _PTYPE_UBLOX;
			td.size = 36;
			{
				uint8_t buf[] = { 0xb5,0x62,0x1,0x2,0x1c,0x0,0x90,0xa2,0xad,0x10,0x6a,0xff,0x67,0xbd,0xb7,0xf4,0x9,0x18,0x35,0x7e,0x15,0x0,0xdf,0xc5,0x15,0x0,0x4f,0x1,0x0,0x0,0xa8,0x1,0x0,0x0,0xe1,0x2b };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;

		case 11:	// Ublox - UBX-RXM-RAWX (0x02 0x15)
			td.ptype = _PTYPE_UBLOX;
			td.size = 952;
			{
				uint8_t buf[] = { 0xb5,0x62,0x2,0x15,0xb0,0x3,0x81,0x95,0x43,0x8b,0x19,0x14,0x11,0x41,0x2b,0x8,0x12,0x1d,0x1,0x1,0xf2,0x7,0x6f,0x1f,0x77,0xfd,0xab,0xb,0x80,0x41,0xa0,0xcc,0x14,0x85,0x7d,0x14,0xa5,0x41,0x78,0x85,0x9c,0x43,0x1,0x85,0x0,0x0,0xf4,0xfb,0x2c,0x5,0x1,0x8,0xf,0x0,0x17,0xbd,0xc5,0x8d,0x69,0xc,0x83,0x41,0x12,0x42,0x8c,0xca,0x6d,0x6,0xa9,0x41,0x44,0xfc,0xc,0x44,0x5,0x2,0x0,0x0,0xf4,0xfb,0x25,0x6,0x1,0x8,0x7,0x0,0xae,0xc2,0xb4,0xd1,0x41,0x9f,0x70,0x41,0x2c,0xb7,0xc2,0x5e,0x62,0xd6,0x95,0x41,0x4e,0x22,0x31,0x45,0x0,0x5,0x0,0x0,0xf4,0xfb,0x30,0x5,0x1,0x8,0x7,0x0,0x94,0x60,0x66,0xa8,0xf7,0xa6,0x73,0x41,0x9d,0xff,0x92,0x27,0x7b,0xd1,0x99,0x41,0x8f,0xca,0x25,0xc5,0x0,0x13,0x0,0x0,0xf4,0xfb,0x27,0x5,0x1,0x8,0x7,0x0,0xb4,0xf1,0x8c,0xd3,0x52,0x11,0x70,0x41,0xc0,0x58,0x83,0x8c,0xea,0x1b,0x95,0x41,0x18,0xf,0xcf,0x44,0x0,0x19,0x0,0x0,0xf4,0xfb,0x2c,0x5,0x1,0x8,0x7,0x0,0x51,0x66,0xdb,0xb9,0xb7,0xc3,0x73,0x41,0x96,0x38,0x72,0x1e,0x3f,0xf7,0x99,0x41,0xb0,0x5c,0x95,0x44,0x2,0x1,0x0,0x0,0xf4,0xfb,0x2a,0x5,0x1,0x8,0x7,0x0,0x4c,0x65,0xc0,0x9a,0x77,0x33,0x6f,0x41,0xf0,0xbc,0x3d,0xa,0x92,0xd3,0x94,0x41,0x11,0x44,0x5d,0x44,0x6,0x9,0x0,0x5,0xf4,0xfb,0x26,0x5,0x1,0x8,0x7,0x0,0x0,0x9c,0xa6,0xcc,0x2,0x3f,0x6e,0x41,0x36,0x84,0x72,0x5f,0x7b,0x39,0x94,0x41,0x79,0x69,0x12,0x44,0x6,0x13,0x0,0xa,0xf4,0xfb,0x21,0x7,0x2,0x8,0x7,0x0,0xca,0x12,0x6,0xf2,0x5e,0x54,0x74,0x41,0x96,0x34,0xdd,0x9b,0x49,0xb5,0x9a,0x41,0x86,0x3e,0x11,0x45,0x2,0x8,0x0,0x0,0xf4,0xfb,0x27,0x5,0x1,0x8,0x7,0x0,0x47,0x7c,0x92,0xfa,0x20,0xd,0x72,0x41,0x92,0xd2,0x89,0x64,0xc,0xb7,0x97,0x41,0x6c,0x36,0xa5,0x44,0x2,0x1a,0x0,0x0,0xf4,0xfb,0x2a,0x5,0x1,0x8,0x7,0x0,0x8a,0x3e,0xbd,0x1e,0x6c,0xe1,0x70,0x41,0x3e,0x74,0x9b,0x14,0x3a,0x97,0x96,0x41,0x14,0x9,0xf5,0x44,0x6,0x3,0x0,0xc,0xf4,0xfb,0x2b,0x5,0x1,0x8,0x7,0x0,0x55,0xf2,0xb9,0xf5,0x50,0x5e,0x73,0x41,0x62,0x2b,0x75,0x8e,0x7,0x72,0x99,0x41,0x8c,0xe4,0xcb,0xc4,0x2,0x15,0x0,0x0,0xf4,0xfb,0x2a,0x5,0x1,0x8,0x7,0x0,0x6b,0x98,0xb0,0x68,0xc9,0x22,0x72,0x41,0x1b,0xd5,0xb,0x3b,0x7a,0x47,0x98,0x41,0xf6,0x33,0x86,0x45,0x6,0x4,0x0,0xd,0xf4,0xfb,0x26,0x6,0x1,0x8,0x7,0x0,0xc5,0x1c,0x77,0xf,0xe8,0x46,0x77,0x41,0xc2,0xde,0xec,0x5a,0x90,0x94,0x9e,0x41,0xc,0xbd,0x3f,0x45,0x2,0x1f,0x0,0x0,0xf4,0xfb,0x23,0x6,0x1,0x8,0x7,0x0,0xd9,0x27,0xc5,0xdc,0x51,0x5f,0x71,0x41,0x41,0x5b,0x7d,0xdf,0xc2,0x28,0x97,0x41,0xa7,0x6b,0x27,0x45,0x6,0xff,0x0,0x1,0x30,0x11,0x27,0x5,0x1,0x8,0xf,0x0,0xdc,0x19,0xbe,0xd0,0x6e,0x3d,0x76,0x41,0x2f,0x34,0xb3,0x4,0xcc,0x37,0x9d,0x41,0x44,0xca,0xfa,0xc4,0x2,0xf,0x0,0x0,0xf4,0xfb,0x25,0x5,0x1,0x8,0x7,0x0,0x86,0xdb,0xd6,0x4e,0xc2,0xe,0x72,0x41,0x6f,0x79,0x97,0x2b,0x31,0xb9,0x97,0x41,0xd,0xd4,0x35,0x45,0x0,0x1d,0x0,0x0,0xf4,0xfb,0x29,0x5,0x1,0x8,0x7,0x0,0x26,0xe9,0x53,0x9b,0xb3,0x37,0x72,0x41,0x6f,0xf4,0x7a,0xec,0xf9,0xee,0x97,0x41,0xa4,0x13,0x2a,0xc5,0x0,0x6,0x0,0x0,0xf4,0xfb,0x2a,0x5,0x1,0x8,0xf,0x0,0xe0,0x65,0xf9,0x72,0x8,0xdf,0x7f,0x41,0x94,0xd4,0xd0,0x99,0x7f,0xef,0xa4,0x41,0x78,0x1f,0x9b,0x43,0x1,0x8a,0x0,0x0,0xf4,0xfb,0x2b,0x5,0x1,0x8,0xf,0x0,0xd1,0x74,0x2f,0x92,0xc2,0xcd,0x72,0x41,0xe5,0xe7,0xd5,0x89,0x1e,0xb4,0x98,0x41,0xbc,0xdf,0x5e,0xc4,0x2,0xd,0x0,0x0,0xf4,0xfb,0x28,0x5,0x1,0x8,0x7,0x0,0xae,0x6f,0x55,0xc2,0xa4,0x3f,0x74,0x41,0xab,0x6,0x32,0x98,0xd,0x9a,0x9a,0x41,0x7a,0x3b,0x25,0xc5,0x0,0x18,0x0,0x0,0xf4,0xfb,0x24,0x6,0x1,0x8,0xf,0x0,0x3e,0xa4,0x61,0x85,0x9f,0x9f,0x70,0x41,0x3e,0xa4,0xf,0x7e,0x2f,0x33,0x96,0x41,0xca,0xe8,0x5a,0xc5,0x6,0x10,0x0,0x6,0xf4,0xfb,0x24,0x6,0x1,0x8,0xf,0x0,0xef,0x37,0x42,0x21,0xca,0x6f,0x70,0x41,0x9,0x31,0x4f,0x1b,0x5,0x98,0x95,0x41,0xea,0x10,0x91,0xc4,0x0,0x2,0x0,0x0,0xf4,0xfb,0x2c,0x5,0x1,0x8,0x7,0x0,0x61,0x6b,0xa5,0x4d,0x16,0x13,0x72,0x41,0x68,0x6d,0x8a,0xc9,0xc0,0x1c,0x98,0x41,0xda,0x67,0xae,0xc4,0x6,0x2,0x0,0x3,0xf4,0xfb,0x2a,0x5,0x1,0x8,0xf,0x0,0xe1,0x73,0x3e,0x87,0xeb,0x23,0x72,0x41,0x4e,0x95,0x47,0xe2,0x46,0x40,0x98,0x41,0x61,0x96,0x38,0x45,0x6,0x14,0x0,0x9,0xf4,0xfb,0x1e,0x8,0x3,0x8,0xf,0x0,0xcb,0xc8,0x82,0xb,0x70,0x56,0x76,0x41,0x7b,0x7b,0x3e,0xe1,0xa5,0x58,0x9d,0x41,0xbd,0x2f,0x35,0xc5,0x2,0x1b,0x0,0x0,0xf4,0xfb,0x24,0x5,0x1,0x8,0x7,0x0,0x7a,0x48,0xde,0xfa,0x97,0x19,0x74,0x41,0xd1,0x34,0xaf,0x8a,0x11,0x68,0x9a,0x41,0xf2,0x33,0xb7,0x44,0x0,0x1f,0x0,0x0,0xf4,0xfb,0x29,0x5,0x1,0x8,0xf,0x0,0xc3,0x4c,0xbf,0x3c,0x91,0x2e,0x70,0x41,0xf1,0x6c,0xab,0xa5,0x52,0x98,0x95,0x41,0x8c,0x45,0xe5,0xc4,0x6,0x12,0x0,0x4,0xf4,0xfb,0x2c,0x5,0x1,0x8,0xf,0x0,0x5e,0xee,0x27,0x65,0xc5,0xe7,0x6e,0x41,0xe8,0xc4,0xd,0x98,0x13,0x4d,0x94,0x41,0x33,0x7d,0x5a,0xc4,0x0,0xc,0x0,0x0,0xf4,0xfb,0x30,0x5,0x1,0x8,0xf,0x0,0x9c,0x9a };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;
#endif

#if TEST_PROTO_RTCM3
		case 12:	// RTCM3 (0x19 0x3e)
			td.ptype = _PTYPE_RTCM3;
			td.size = 31;
			{
				uint8_t buf[] = { 0xd3,0x0,0x19,0x3e,0xfc,0x2f,0x14,0x41,0x44,0x56,0x4e,0x55,0x4c,0x4c,0x41,0x4e,0x54,0x45,0x4e,0x4e,0x41,0x20,0x20,0x4e,0x4f,0x4e,0x45,0x0,0xc4,0xe,0xe1 };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;

		case 13:	// RTCM3 (0x48 0x40)
			td.ptype = _PTYPE_RTCM3;
			td.size = 78;
			{
				uint8_t buf[] = { 0xd3,0x0,0x48,0x40,0x9c,0x2f,0x14,0x41,0x44,0x56,0x4e,0x55,0x4c,0x4c,0x41,0x4e,0x54,0x45,0x4e,0x4e,0x41,0x20,0x20,0x4e,0x4f,0x4e,0x45,0x0,0x0,0xd,0x54,0x52,0x49,0x4d,0x42,0x4c,0x45,0x20,0x4e,0x45,0x54,0x52,0x39,0x14,0x4e,0x61,0x76,0x20,0x34,0x2e,0x36,0x32,0x20,0x2f,0x20,0x42,0x6f,0x6f,0x74,0x20,0x34,0x2e,0x36,0x32,0xa,0x35,0x33,0x32,0x39,0x4b,0x34,0x34,0x33,0x35,0x32,0xfc,0xca,0x3f };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;

		case 14:	// RTCM3 (0x13 0x3e)
			td.ptype = _PTYPE_RTCM3;
			td.size = 25;
			{
				uint8_t buf[] = { 0xd3,0x0,0x13,0x3e,0xdc,0x2f,0x3,0x7b,0xcd,0x79,0xd5,0x47,0x35,0x77,0x5f,0x93,0x4d,0x49,0x8f,0xf1,0xb3,0x1d,0xff,0x10,0x3d };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;

		case 15:	// RTCM3 (0x3a 0x44)
			td.ptype = _PTYPE_RTCM3;
			td.size = 320;
			{
				uint8_t buf[] = { 0xd3,0x1,0x3a,0x44,0x7c,0x2f,0x52,0x3,0x6c,0x22,0x0,0x20,0x34,0x98,0x0,0xc0,0x0,0x0,0x0,0x0,0x4,0x0,0x88,0x80,0x7f,0xff,0xff,0xff,0xac,0x2a,0xad,0x29,0xa9,0xa7,0xa6,0xa7,0x80,0x0,0x0,0x0,0x6e,0x64,0x72,0xc7,0x4a,0xea,0x56,0xa3,0x32,0xfe,0xc0,0x1,0x0,0x4,0x0,0x10,0x0,0x40,0x1,0x0,0x4,0x0,0x10,0x0,0x3,0xf7,0x8,0x96,0x12,0x90,0x23,0xd7,0x25,0xbe,0x66,0xbc,0xd0,0x79,0xb9,0xc,0xbb,0x1b,0x5e,0x37,0x74,0x6d,0xe2,0x1d,0x94,0x45,0xc8,0xa2,0x51,0x5d,0xe3,0x3a,0xc6,0x81,0x8d,0xbb,0x19,0x8e,0x7b,0x5c,0xf3,0xd9,0xf1,0xf3,0xe0,0x6c,0x46,0xde,0x85,0xbd,0x4f,0x7c,0x1f,0x4,0x9e,0x16,0xfc,0x44,0xb8,0x81,0xff,0xff,0xf5,0xff,0xff,0x8f,0xff,0xeb,0xc0,0x0,0x15,0x7f,0xfe,0xe5,0xff,0xfa,0x28,0x0,0x11,0xdf,0xff,0xde,0x7f,0xfe,0xd8,0x0,0x1,0xcf,0xff,0xeb,0xdf,0xff,0xf1,0xff,0xff,0x8,0x0,0x6,0x10,0x0,0x6,0x3f,0xff,0x9c,0x80,0x0,0x47,0xff,0xfe,0xf0,0x0,0x14,0xff,0xff,0xa6,0x0,0x1,0x29,0xff,0xfc,0x58,0x0,0x9,0x0,0x0,0x56,0xff,0xff,0xe5,0xff,0xfb,0x50,0x0,0x12,0x3f,0xff,0xcc,0xff,0xff,0x2b,0xff,0xfb,0x17,0xff,0xfa,0x3f,0xff,0xf1,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x59,0x5e,0x15,0x61,0x6e,0xb7,0x55,0x66,0x35,0xd5,0x56,0x36,0xd9,0x5e,0x36,0x59,0x56,0x15,0xe3,0x7e,0xf8,0x65,0x87,0x19,0x40,0x0,0x80,0x1,0x0,0x2,0x0,0x4,0x0,0x8,0x0,0x10,0x0,0x20,0x0,0x40,0x0,0x80,0x1,0x0,0x2,0x0,0x4,0x0,0x8,0x0,0x10,0x0,0x20,0x0,0x40,0x0,0x80,0x1,0x0,0x2,0x0,0x4,0x0,0x8,0x0,0x10,0x0,0x20,0x0,0x40,0x0,0x80,0x1,0x0,0x2,0x0,0x4,0x0,0x8,0x0,0x10,0x0,0x20,0x0,0x0,0x1d,0x23,0x6c };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;

		case 16:	// RTCM3 (0xcc 0x43)
			td.ptype = _PTYPE_RTCM3;
			td.size = 210;
			{
				uint8_t buf[] = { 0xd3,0x0,0xcc,0x43,0xdc,0x2f,0x82,0x2f,0xc0,0xe2,0x0,0x20,0x4,0x3,0x41,0x0,0x0,0x0,0x0,0x0,0x30,0xc0,0x0,0x0,0x7f,0xff,0xfa,0x4a,0x1a,0x22,0x2a,0x54,0x3,0xdd,0x28,0xec,0x62,0x15,0xe,0xbb,0x90,0x0,0x40,0x1,0x0,0x4,0x0,0x10,0x0,0x75,0x28,0xea,0x45,0xd6,0x33,0xac,0x20,0x91,0x41,0x1d,0xa2,0x6b,0x84,0xcc,0xed,0x28,0xda,0xb,0xb5,0xc3,0x6b,0x87,0x2f,0x3e,0x57,0x1c,0xe3,0x79,0xd9,0x10,0xec,0x21,0xaa,0x47,0xa0,0x90,0xdf,0xff,0xfb,0x5f,0xff,0xee,0x7f,0xff,0xaf,0xff,0xfe,0xaf,0xff,0xfa,0xdf,0xff,0xea,0x0,0x1,0xa2,0x0,0x6,0x68,0x0,0xf,0x40,0x0,0x41,0x0,0x0,0x5c,0x0,0x1,0x68,0x0,0x14,0x9f,0xff,0xaf,0x0,0x1,0x2e,0x0,0x4,0xc0,0x0,0x13,0xe0,0x0,0x52,0x0,0x1,0x8a,0x0,0x6,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x5f,0x75,0x96,0x67,0x96,0x38,0x65,0x8d,0xd7,0x5f,0x75,0x95,0xd9,0x64,0xd2,0xc0,0x0,0x80,0x1,0x0,0x2,0x0,0x4,0x0,0x8,0x0,0x10,0x0,0x20,0x0,0x40,0x0,0x80,0x1,0x0,0x2,0x0,0x4,0x0,0x8,0x0,0x10,0x0,0x20,0x0,0x40,0x0,0x80,0x1,0x0,0x2,0x0,0x0,0x9d,0xc4,0x6a };
				memcpy(td.data.buf, buf, td.size);		// < PKT_BUF_SIZE (2048)
			}
			break;
#endif
		}

		if (td.size)
		{
			DEBUG_PRINTF("[%2d] ", (int)testDeque.size());
			switch (td.ptype)
			{
			case PSC_START_BYTE:
				DEBUG_PRINTF("DID: %3d, size: %3d\n", td.did, td.size);
				break;
			case _PTYPE_ASCII_NMEA:
				DEBUG_PRINTF("NMEA: %.30s...\n", td.data.buf);
				break;
			case _PTYPE_UBLOX:
				DEBUG_PRINTF("UBLOX: size %d, (0x%02x 0x%02x)\n", td.size, td.data.buf[2], td.data.buf[3]);
				break;
			case _PTYPE_RTCM3:
				DEBUG_PRINTF("RTCM3: size %d, (%02x %02x %02x %02x)\n", td.size, td.data.buf[0], td.data.buf[1], td.data.buf[2], td.data.buf[3]);
				break;
			}

			if (byteSize + td.size < (PORT_BUFFER_SIZE*3)/4)	// Allow room for packet headers
			{
				byteSize += td.size;

				// Add data to deque
				testDeque.push_back(td);
			}
			else
			{	// Buffer is full
				return;
			}
		}
	}
}


void addDequeToRingBuf(std::deque<data_holder_t> &testDeque, ring_buf_t *rbuf)
{
	is_comm_instance_t		comm;
	uint8_t					comm_buffer[2048] = { 0 };
	is_comm_init(&comm, comm_buffer, sizeof(comm_buffer));

	int k=0;

	for (int i = 0; i < testDeque.size(); i++)
	{
		data_holder_t &td = testDeque[i];
		int n = 0;

		// Add packetized data to ring buffer
		switch (td.ptype)
		{
		case _PTYPE_INERTIAL_SENSE_DATA:
			// Packetize data 
			n = is_comm_data(&comm, td.did, 0, td.size, (void*)&(td.data));
			td.pktSize = n;
			EXPECT_FALSE(ringBufWrite(rbuf, comm.buf.start, n));
			break;

		case _PTYPE_ASCII_NMEA:
		case _PTYPE_UBLOX:
		case _PTYPE_RTCM3:
			td.pktSize = td.size;
			EXPECT_FALSE(ringBufWrite(rbuf, td.data.buf, td.size));
			break;
		}
	}

	// Buffer overflow not allowed for test
	ASSERT_TRUE(ringBufFree(rbuf));
}


void parseDataPortTxBuf(std::deque<data_holder_t> &testDeque, test_data_t &t)
{
	is_comm_instance_t		comm;
	uint8_t					comm_buffer[2048] = { 0 };
	is_comm_init(&comm, comm_buffer, sizeof(comm_buffer));
	unsigned char c;
	protocol_type_t ptype;
	uDatasets dataWritten;

	while (ringBufUsed(&t.portTxBuf)>0)
	{
		ringBufRead(&t.portTxBuf, &c, 1);

		if((ptype = is_comm_parse_byte(&comm, c)) != _PTYPE_NONE)
		{
			data_holder_t td = testDeque.front();
			testDeque.pop_front();

			switch (ptype)
			{
			case _PTYPE_INERTIAL_SENSE_DATA:
				// Found data
				DEBUG_PRINTF("Found data: did %3d, size %3d\n", comm.dataHdr.id, comm.dataHdr.size);

				is_comm_copy_to_struct(&dataWritten, &comm, sizeof(uDatasets));

				EXPECT_EQ(td.did, comm.dataHdr.id);
				break;

			case _PTYPE_UBLOX:
			case _PTYPE_RTCM3:
				break;

			case _PTYPE_ASCII_NMEA:
				DEBUG_PRINTF("Found data: %.30s...\n", comm.dataPtr);
				break;
			}

			EXPECT_EQ(td.size, comm.dataHdr.size);
			EXPECT_TRUE(memcmp(td.data.buf, comm.dataPtr, td.size) == 0);
		}
	}
}


void ringBuftoRingBufWrite(ring_buf_t *dst, ring_buf_t *src, int len)
{
	uint8_t *buf = new uint8_t[len];

	len = ringBufRead(src, buf, len);
	EXPECT_FALSE(ringBufWrite(dst, buf, len));
}


#if 1
TEST(ComManager, BasicTxTest)
{
	// Initialize Com Manager
	init(tcm);

	// Generate and add data to deque
	generateData(g_testTxDeque);

	// Use Com Manager to send deque data to Tx port ring buffer
	for(int i=0; i<g_testTxDeque.size(); i++)
	{
		data_holder_t td = g_testTxDeque[i];

		// Send data - writes data to tcm.txBuf
		switch (td.ptype)
		{
		default:	// IS binary
			comManagerSendDataNoAckInstance(&tcm.cm, 0, td.did, &td.data, td.size, 0);
			break;

		case _PTYPE_ASCII_NMEA:
		case _PTYPE_UBLOX:
		case _PTYPE_RTCM3:
			portWrite(&(tcm.cm), 0, td.data.buf, td.size);
			break;
		}

	}

	// Test that data parsed from Tx port matches deque data
	parseDataPortTxBuf(g_testTxDeque, tcm);

	// Check that we got all data
	EXPECT_TRUE(g_testTxDeque.empty());
	EXPECT_TRUE(ringBufUsed(&tcm.portTxBuf) == 0);
}
#endif


#if 1
TEST(ComManager, BasicRxTest)
{
	// Initialize Com Manager
	init(tcm);

	// Generate and add data to deque
	generateData(g_testRxDeque);

	// Add deque data to Rx port ring buffer
	addDequeToRingBuf(g_testRxDeque, &tcm.portRxBuf);

	DEBUG_PRINTF("===============  Checking Data.  Size: %d  ===============\n", ringBufUsed(&tcm.portRxBuf));

	while (!ringBufEmpty(&tcm.portRxBuf))
	{
		// Step Com Manager and check that was received correctly inside postRxRead()
		comManagerStepInstance(&tcm.cm);	// 2048 byte limit each step
	}

	// Check that no data was left behind 
	EXPECT_TRUE(g_testRxDeque.empty());
	EXPECT_TRUE(ringBufUsed(&tcm.portRxBuf) == 0);
}
#endif


#if 1
// Tests that ComManager handles segmented serial data properly
TEST(ComManager, SegmentedRxTest)
{
	ring_buf_t tmpRBuf;
	uint8_t buffer[8192];

	// Initialize temporary ring buffer
	ringBufInit(&tmpRBuf, buffer, sizeof(buffer), 1);

	// Initialize Com Manager
	init(tcm);

	// Generate and add data to deque
	generateData(g_testRxDeque);

	// Add deque data to temporary ring buffer
	addDequeToRingBuf(g_testRxDeque, &tmpRBuf);

	DEBUG_PRINTF("Checking Data:\n");

	// Divide data written to Com Manager into pieces
#define TIMES_TO_DIVIDE_DATA 10
	int bytesToWrite = ringBufUsed(&tmpRBuf) / TIMES_TO_DIVIDE_DATA;
	while (!ringBufEmpty(&tmpRBuf) && !g_testRxDeque.empty())
	{
		// Partial write of data
		ringBuftoRingBufWrite(&tcm.portRxBuf, &tmpRBuf, bytesToWrite);

		while (!ringBufEmpty(&tcm.portRxBuf))
		{
			// Step Com Manager and check that was received correctly inside postRxRead()
			comManagerStepInstance(&tcm.cm);
			comManagerStepInstance(&tcm.cm);
		}
	}

	// Check that no data was left behind 
	EXPECT_TRUE(g_testRxDeque.empty());
	EXPECT_TRUE(ringBufEmpty(&tcm.portRxBuf));
}
#endif


#if 1
TEST(ComManager, RxWithGarbageTest)
{
	// Initialize Com Manager
	init(tcm);

	// Generate and add data to deque
	generateData(g_testRxDeque);

	// Insert garbage
	std::deque<data_holder_t> testRxDequeWithGarbage;
	data_holder_t td = {};
	td.size = 24;
	td.data.buf[1] = 128;
	td.data.buf[19] = 128;
	td.data.buf[23] = 128;

	for (int i = 0; i < g_testRxDeque.size(); i++)
	{
		if (i % 5 == 0)
		{
			// Insert garbage data
			testRxDequeWithGarbage.push_back(td);
		}

		// Copy good data
		testRxDequeWithGarbage.push_back(g_testRxDeque[i]);
	}

	// Add deque data to Rx port ring buffer
	addDequeToRingBuf(testRxDequeWithGarbage, &tcm.portRxBuf);

	DEBUG_PRINTF("===============  Checking Data.  Size: %d  ===============\n", ringBufUsed(&tcm.portRxBuf));

	while (!ringBufEmpty(&tcm.portRxBuf))
	{
		// Step Com Manager and check that was received correctly inside postRxRead()
		comManagerStepInstance(&tcm.cm);	// 2048 byte limit each step
	}

	// Check that no data was left behind 
	EXPECT_TRUE(g_testRxDeque.empty());
	EXPECT_TRUE(ringBufUsed(&tcm.portRxBuf) == 0);
}
#endif


#if 1
TEST(ComManager, Evb2AltDecodeBufferTest)
{
	// This test ensures that packets can be read and decoded to the alternate buffer (not in the default comm.buf.start buffer).

	// Init Port Buffers
	ringBufInit(&(tcm.portTxBuf), tcm.portTxBuffer, sizeof(tcm.portTxBuffer), 1);
	ringBufInit(&(tcm.portRxBuf), tcm.portRxBuffer, sizeof(tcm.portRxBuffer), 1);
	initComManager(tcm);

	is_comm_instance_t &comm = (tcm.cm.ports[0].comm);
	uint8_t altDecodBuf[PKT_BUF_SIZE] = {};
	comm.altDecodeBuf = altDecodBuf;

	// Generate and add data to deque
	generateData(g_testRxDeque);

	// Add deque data to Rx port ring buffer
	addDequeToRingBuf(g_testRxDeque, &tcm.portRxBuf);

	DEBUG_PRINTF("===============  Checking Data.  Size: %d  ===============\n", ringBufUsed(&tcm.portRxBuf));

	while (!ringBufEmpty(&tcm.portRxBuf))
	{
		// Get available size of comm buffer
		int n = is_comm_free(&comm);

		// Read 5 bytes at a time
		n = _MIN(n, 5);

		// Read data directly into comm buffer
		if ((n = ringBufRead(&tcm.portRxBuf, comm.buf.tail, n)))
		{
			// Update comm buffer tail pointer
			comm.buf.tail += n;

			protocol_type_t ptype = _PTYPE_NONE;
			uDatasets dataWritten;

			// Search comm buffer for valid packets
			while ((ptype = is_comm_parse(&comm)) != _PTYPE_NONE)
			{
				uint8_t error = 0;
				uint8_t *dataPtr = comm.dataPtr + comm.dataHdr.offset;
				uint32_t dataSize = comm.dataHdr.size;

				data_holder_t td = g_testRxDeque.front();
				g_testRxDeque.pop_front();

				switch (ptype)
				{
				case _PTYPE_INERTIAL_SENSE_DATA:
					// Found data
					DEBUG_PRINTF("Found data: did %3d, size %3d\n", comm.dataHdr.id, comm.dataHdr.size);

					is_comm_copy_to_struct(&dataWritten, &comm, sizeof(uDatasets));

					EXPECT_EQ(td.did, comm.dataHdr.id);
					EXPECT_TRUE(memcmp(td.data.buf, comm.dataPtr, comm.dataHdr.size)==0);
					break;

				case _PTYPE_UBLOX:
				case _PTYPE_RTCM3:
					EXPECT_TRUE(memcmp(td.data.buf, comm.dataPtr, comm.dataHdr.size) == 0);
					break;

				case _PTYPE_ASCII_NMEA:
					DEBUG_PRINTF("Found data: %.30s...\n", comm.dataPtr);
					EXPECT_TRUE(memcmp(td.data.buf, comm.dataPtr, comm.dataHdr.size) == 0);
					break;

				default:
					EXPECT_TRUE(false);
					break;
				}
			}
		}

	}

	// Check that no data was left behind 
	EXPECT_TRUE(g_testRxDeque.empty());
	EXPECT_TRUE(ringBufUsed(&tcm.portRxBuf) == 0);
}
#endif



#if 1
TEST(ComManager, Evb2DataForwardTest)
{
	// This test ensures that packets can be read, decoded, and forwarded for EVB-2 com_bridge without corrupting the data.

	// Init Port Buffers
	ringBufInit(&(tcm.portTxBuf), tcm.portTxBuffer, sizeof(tcm.portTxBuffer), 1);
	ringBufInit(&(tcm.portRxBuf), tcm.portRxBuffer, sizeof(tcm.portRxBuffer), 1);
	initComManager(tcm);

	is_comm_instance_t &comm = (tcm.cm.ports[0].comm);
	uint8_t altDecodBuf[PKT_BUF_SIZE] = {};
	comm.altDecodeBuf = altDecodBuf;

	// Generate and add data to deque
	generateData(g_testRxDeque);

	ring_buf_t evbRbuf;
	uint8_t evbRBuffer[PORT_BUFFER_SIZE] = {};
	ringBufInit(&evbRbuf, evbRBuffer, PORT_BUFFER_SIZE, 1);

	// Add deque data to EVB port ring buffer
	addDequeToRingBuf(g_testRxDeque, &evbRbuf);


	DEBUG_PRINTF("===============  EVB input Data.  Size: %d  ===============\n", ringBufUsed(&evbRbuf));

	int originalRingBufferUsed = ringBufUsed(&evbRbuf);

	std::deque<data_holder_t> testRxDequeCopy = g_testRxDeque;

	while (!ringBufEmpty(&evbRbuf))
	{
		// Get available size of comm buffer
		int n = is_comm_free(&comm);

		// Read 5 bytes at a time
		n = _MIN(n, 5);

		// Read data directly into comm buffer
		if ((n = ringBufRead(&evbRbuf, comm.buf.tail, n)))
		{
			// Update comm buffer tail pointer
			comm.buf.tail += n;

			protocol_type_t ptype = _PTYPE_NONE;

			// Search comm buffer for valid packets
			while ((ptype = is_comm_parse(&comm)) != _PTYPE_NONE)
			{
				uint8_t error = 0;
				uint8_t *dataPtr = comm.dataPtr + comm.dataHdr.offset;
				uint32_t dataSize = comm.dataHdr.size;

				data_holder_t td = testRxDequeCopy.front();
				testRxDequeCopy.pop_front();

				switch (ptype)
				{
				case _PTYPE_INERTIAL_SENSE_DATA:
				case _PTYPE_ASCII_NMEA:
				case _PTYPE_UBLOX:
				case _PTYPE_RTCM3:
				{
					uint32_t pktSize = (uint32_t)_MIN(comm.buf.scan - comm.pktPtr, PKT_BUF_SIZE);
					ringBufWrite(&tcm.portRxBuf, comm.pktPtr, pktSize);

					if (td.pktSize != pktSize)
					{
						ASSERT_TRUE(false);
					}
				}
					break;

				default:
					ASSERT_TRUE(false);
				}
			}
		}
	}

	int forwardRingBufferUsed = ringBufUsed(&tcm.portRxBuf);
	ASSERT_TRUE(forwardRingBufferUsed == originalRingBufferUsed);
	

	DEBUG_PRINTF("===============  Checking Data.  Size: %d  ===============\n", ringBufUsed(&tcm.portRxBuf));

	while (!ringBufEmpty(&tcm.portRxBuf))
	{
		// Get available size of comm buffer
		int n = is_comm_free(&comm);

		// Read 5 bytes at a time
		n = _MIN(n, 5);

		// Read data directly into comm buffer
		if ((n = ringBufRead(&tcm.portRxBuf, comm.buf.tail, n)))
		{
			// Update comm buffer tail pointer
			comm.buf.tail += n;

			protocol_type_t ptype = _PTYPE_NONE;
			uDatasets dataWritten;

			// Search comm buffer for valid packets
			while ((ptype = is_comm_parse(&comm)) != _PTYPE_NONE)
			{
				uint8_t error = 0;
				uint8_t *dataPtr = comm.dataPtr + comm.dataHdr.offset;
				uint32_t dataSize = comm.dataHdr.size;

				data_holder_t td = g_testRxDeque.front();
				g_testRxDeque.pop_front();

				switch (ptype)
				{
				case _PTYPE_INERTIAL_SENSE_DATA:
					// Found data
					DEBUG_PRINTF("Found data: did %3d, size %3d\n", comm.dataHdr.id, comm.dataHdr.size);

					is_comm_copy_to_struct(&dataWritten, &comm, sizeof(uDatasets));

					EXPECT_EQ(td.did, comm.dataHdr.id);
					EXPECT_TRUE(memcmp(td.data.buf, comm.dataPtr, comm.dataHdr.size) == 0);
					break;

				case _PTYPE_UBLOX:
				case _PTYPE_RTCM3:
					EXPECT_TRUE(memcmp(td.data.buf, comm.dataPtr, comm.dataHdr.size) == 0);
					break;

				case _PTYPE_ASCII_NMEA:
					DEBUG_PRINTF("Found data: %.30s...\n", comm.dataPtr);
					EXPECT_TRUE(memcmp(td.data.buf, comm.dataPtr, comm.dataHdr.size) == 0);
					break;

				default:
					EXPECT_TRUE(false);
					break;
				}
			}
		}

	}

	// Check that no data was left behind 
	EXPECT_TRUE(g_testRxDeque.empty());
	EXPECT_TRUE(ringBufUsed(&tcm.portRxBuf) == 0);
}
#endif

