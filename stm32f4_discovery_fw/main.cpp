#include <xpcc/architecture/platform.hpp>
#include <xpcc/debug/logger.hpp>

// ----------------------------------------------------------------------------
// Set the log level
#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

// Create an IODeviceWrapper around the Uart Peripheral we want to use
xpcc::IODeviceWrapper< Usart2, xpcc::IOBuffer::BlockIfFull > loggerDevice;

// Set all four logger streams to use the UART
xpcc::log::Logger xpcc::log::debug(loggerDevice);
xpcc::log::Logger xpcc::log::info(loggerDevice);
xpcc::log::Logger xpcc::log::warning(loggerDevice);
xpcc::log::Logger xpcc::log::error(loggerDevice);


// ----------------------------------------------------------------------------


extern "C" {

typedef struct {
	// source: Embedded Trace Macrocell Architecture Specification
	//         section "3.4 The ETM registers"
	__IOM uint32_t CR; ///< (0x000) Main Control Register
} ETM_Type;

#define ETM_CR_PROGRAMMING (0x1UL << 10U)

#define ETM_BASE  (0xE0041000UL) ///< ETM Base Address
#define ETM ((ETM_Type*) ETM_BASE)

}
// ----------------------------------------------------------------------------


using systemClock = SystemClock<ExternalCrystal<MHz8>>;

int
main()
{
	// initialize
	systemClock::enable();
	xpcc::cortex::SysTickTimer::initialize<systemClock>();

	Board::LedOrange::setOutput(xpcc::Gpio::Low);
	Board::LedGreen::setOutput(xpcc::Gpio::Low);
	Board::LedRed::setOutput(xpcc::Gpio::Low);
	Board::LedBlue::setOutput(xpcc::Gpio::Low);

	Board::Button::setInput();
	Board::Button::setInputTrigger(Gpio::InputTrigger::RisingEdge);
	Board::Button::enableExternalInterrupt();


	GpioOutputA2::connect(Usart2::Tx);
	Usart2::initialize<systemClock, 9600>(10);


	XPCC_LOG_INFO << "Hello World!" << xpcc::endl;

	XPCC_LOG_INFO << "DBGMCU->CR: 0x" << xpcc::hex << static_cast<uint32_t>(DBGMCU->CR) << xpcc::endl;
	DBGMCU->CR = 0x27;
	XPCC_LOG_INFO << "DBGMCU->CR: 0x" << xpcc::hex << static_cast<uint32_t>(DBGMCU->CR) << xpcc::endl;


	Board::LedOrange::set();
	Board::LedRed::set();

	volatile int a[40];

	while (1)
	{
		//Board::LedBlue::toggle();
		//Board::LedGreen::toggle();
		//Board::LedOrange::toggle();
		//Board::LedRed::toggle();
		//xpcc::delayMilliseconds(Board::Button::read() ? 250 : 500);

		// enable ETM
		ETM->CR &= ~ETM_CR_PROGRAMMING;
		for(int ii = 0; ii < 40; ++ii) {
			a[ii] += 1;
		}
		// disable ETM
		ETM->CR |= ETM_CR_PROGRAMMING;

		xpcc::delayMilliseconds(1);

	}

	return 0;
}
