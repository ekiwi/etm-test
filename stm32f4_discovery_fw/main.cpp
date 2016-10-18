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

		for(int ii = 0; ii < 40; ++ii) {
			a[ii] += 1;
		}

	}

	return 0;
}
