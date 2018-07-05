#if !defined(_TELNET_H_)
#define _TELNET_H_


#define TELOPTS
#define TELCMDS

#define WILL_TELOPT(x) const unsigned char x ## _WILL [] = { IAC, WILL, TELOPT_ ## x , '\0' };
#define WONT_TELOPT(x) const unsigned char x ## _WONT [] = { IAC, WONT, TELOPT_ ## x, '\0' };
#define DO_TELOPT(x) const unsigned char x ## _DO [] = { IAC, DO, TELOPT_ ## x, '\0' };
#define DONT_TELOPT(x) const unsigned char x ## _DONT [] = { IAC, DONT, TELOPT_ ## x, '\0' };
#define SB_TELOPT(x) const unsigned char x ## _SB [] = { IAC, SB, TELOPT_ ## x, '\0' };

#define DECLARE_TELOPT(x) WILL_TELOPT(x) WONT_TELOPT(x) DO_TELOPT(x) DONT_TELOPT(x)

#define EBAK (EOR)
#undef EOR

DECLARE_TELOPT(BINARY) // 8-bit data
DECLARE_TELOPT(ECHO) // Telnet Echo
DECLARE_TELOPT(RCP) // Reconnect
DECLARE_TELOPT(SGA) // suppress go ahead
DECLARE_TELOPT(EOR) // Telnet End of Record
DECLARE_TELOPT(NAWS) // Telnet Window Size
	SB_TELOPT(NAWS)

#define EOR (EBAK)


#endif
