// --------------------------------------------------------------------
//	Compiler collection: Call
// ====================================================================
//	2023/July/25th	t.hara
// --------------------------------------------------------------------
#include "../compiler.h"

#ifndef __CALL_H__
#define __CALL_H__

class CCALL: public CCOMPILER_CONTAINER {
private:
	//	コマンド処理
	void iotinit( CCOMPILE_INFO *p_info );
	void iotget( CCOMPILE_INFO *p_info );
	void iotput( CCOMPILE_INFO *p_info );
	void iotfind( CCOMPILE_INFO *p_info );
	void turbo( CCOMPILE_INFO *p_info );
public:
	bool exec( CCOMPILE_INFO *p_info );
};

#endif
