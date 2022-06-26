#include "hive_sdk.h"
class IGameConsole : public IBaseInterface
{
public:
	virtual void	Activate() = 0;
	virtual void	Initialize() = 0;
	virtual void	Hide() = 0;
	virtual void	Clear() = 0;
	virtual bool	IsConsoleVisible() = 0;
	virtual void	Printf(const char *format, ...) = 0;
	virtual void	DPrintf(const char *format, ...) = 0;
	virtual void	ColorPrintf(Color& clr, const char *format, ...) = 0;
	virtual void	SetParent(VPANEL parent) = 0;
};


