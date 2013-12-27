#include "rsconnect_tools.h"

namespace tools
{
	long T_SELECT 	= 20000;	// min 17 µs
	long T_SETUP	=  2000;	// min 1,4 µs
	long T_CLK 	=  1000;	// max 100 ns

	RSConnect _con = 0x03f8;

	void RSpoweron()
	{
		_con.setTxD();
	}

	void RSpoweroff()
	{
		_con.unsetTxD();
	}
	
	void RSselect()
	{
		_con.setRTS();
		wait(T_SELECT);
		_con.unsetRTS();
		wait(T_SETUP);
	}

	void RStick()
	{
		_con.setDTR();
		wait(T_CLK);
		_con.unsetDTR();
		wait(T_CLK);
	}
	
	void RStickOnce()
	{
		_con.setDTR();
	}
	
	void RStickOnceOut()
	{
		_con.unsetDTR();
	}
	
	void RSselectOnce()
	{
		_con.setRTS();
	}
	
	void RSselectOnceOut()
	{
		_con.unsetRTS();
	}
	
	bool RSread()
	{
		return (_con.getCTS());
	}

}
