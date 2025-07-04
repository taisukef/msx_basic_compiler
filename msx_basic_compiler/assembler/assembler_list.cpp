// --------------------------------------------------------------------
//	MSX-BASIC compiler
// ====================================================================
//	2023/July/20th  t.hara 
// --------------------------------------------------------------------

#include "assembler_list.h"
#include "../compile_info.h"
#include "../constant_info.h"
#include "../single_real.h"
#include "../double_real.h"
#include <cstdio>

// --------------------------------------------------------------------
bool CASSEMBLER_LIST::add_subroutines( const std::string s_name ) {

	if( this->is_registered_subroutine( s_name ) ) {
		return false;
	}
	this->subrouines_list.push_back( s_name );
	return true;
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::add_data( int line_no, const std::string s_value, CCONSTANT_INFO *p_constants ) {
	CASSEMBLER_LINE asm_line;
	bool has_label = false;
	std::string s_label;

	//	既にデータラベルを配置した行番号か？
	for( auto i: this->data_lines ) {
		if( i == line_no ) {
			has_label = true;
			break;
		}
	}
	if( !has_label ) {
		this->data_lines.push_back( line_no );
		s_label = "data_" + std::to_string( line_no );
		asm_line.set( "LABEL", "", s_label, "" );
		this->datas.push_back( asm_line );
	}
	
	CSTRING value;
	value.set( s_value );
	s_label = p_constants->add( value );
	asm_line.set( "DEFW", "", s_label, "" );
	this->datas.push_back( asm_line );
}


// --------------------------------------------------------------------
void CASSEMBLER_LIST::push_hl( CEXPRESSION_TYPE type ) {
	CASSEMBLER_LINE asm_line;

	switch( type ) {
	default:
	case CEXPRESSION_TYPE::INTEGER:
		asm_line.set( "PUSH", "", "HL", "" );
		this->body.push_back( asm_line );
		break;
	case CEXPRESSION_TYPE::SINGLE_REAL:
		this->activate_push_single_real_hl();
		asm_line.set( "CALL", "", "push_single_real_hl", "" );
		this->body.push_back( asm_line );
		break;
	case CEXPRESSION_TYPE::DOUBLE_REAL:
		this->activate_push_double_real_hl();
		asm_line.set( "CALL", "", "push_double_real_hl", "" );
		this->body.push_back( asm_line );
		break;
	case CEXPRESSION_TYPE::STRING:
		asm_line.set( "PUSH", "", "HL", "" );
		this->body.push_back( asm_line );
		break;
	}
}

// --------------------------------------------------------------------
bool CASSEMBLER_LIST::is_registered_subroutine( std::string s_search_name ) {

	for( auto &p: this->subrouines_list ) {
		if( p == s_search_name ) {
			return true;
		}
	}
	return false;
}

// --------------------------------------------------------------------
//	push_single_real_hl:
void CASSEMBLER_LIST::activate_push_single_real_hl( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "push_single_real_hl" ) ) {
		return;
	}
	this->subrouines_list.push_back( "push_single_real_hl" );
	asm_line.set( "LABEL", "", "push_single_real_hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//		push_double_real_dac:
void CASSEMBLER_LIST::activate_push_double_real_hl( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "push_double_real_hl" ) ) {
		return;
	}
	this->subrouines_list.push_back( "push_double_real_hl" );
	asm_line.set( "LABEL", "", "push_double_real_hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	pop_single_real_arg:
void CASSEMBLER_LIST::activate_pop_single_real_arg( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "pop_single_real_arg" ) ) {
		return;
	}
	this->subrouines_list.push_back( "pop_single_real_arg" );
	this->add_label( "work_arg", "0x0f847" );
	asm_line.set( "LABEL", "", "pop_single_real_arg", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+0]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+4]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+6]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_convert_to_integer( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "convert_to_integer" ) ) {
		return;
	}
	this->subrouines_list.push_back( "convert_to_integer" );

	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "bios_frcint", "0x02f8a" );

	asm_line.set( "LABEL", "", "convert_to_integer", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_convert_to_integer_from_sngle_real( CCONSTANT_INFO *p_constants ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "convert_to_integer_from_sngle_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "convert_to_integer_from_sngle_real" );

	CSINGLE_REAL value;
	value.set( "32768!" );
	std::string s_label_32768 = p_constants->add( value );

	this->activate_ld_dac_single_real();
	this->activate_ld_arg_single_real();
	this->activate_convert_to_integer();

	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_arg", "0x0f7f6" );
	this->add_label( "bios_fcomp", "0x02f21" );
	this->add_label( "bios_decsub", "0x0268c" );
	this->add_label( "bios_frcint", "0x02f8a" );

	asm_line.set( "LABEL", "", "convert_to_integer_from_sngle_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_dac_single_real", "" );
	this->subroutines.push_back( asm_line );
	//	もし DAC < 32768! なら → A = -1 なら、普通に FRCINT で変換。(-32768 より小さければ、FRCINT が Overflow 出す。)
	asm_line.set( "LD", "", "BC", "0x3245" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "0x8076" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_fcomp", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "A", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "convert_to_integer", "" );
	this->subroutines.push_back( asm_line );
	//	HL = int(DAC - 32768!) xor &H8000
	asm_line.set( "LD", "", "HL", s_label_32768 );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_arg_single_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_decsub", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "0x80" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_convert_to_integer_from_double_real( CCONSTANT_INFO *p_constants ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "convert_to_integer_from_double_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "convert_to_integer_from_double_real" );

	CSINGLE_REAL value;
	value.set( "32768!" );
	std::string s_label_32768 = p_constants->add( value );

	this->activate_ld_dac_double_real();
	this->activate_ld_arg_double_real();
	this->activate_convert_to_integer();

	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_arg", "0x0f7f6" );
	this->add_label( "bios_fcomp", "0x02f21" );
	this->add_label( "bios_decsub", "0x0268c" );
	this->add_label( "bios_frcint", "0x02f8a" );

	asm_line.set( "LABEL", "", "convert_to_integer_from_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_dac_double_real", "" );
	this->subroutines.push_back( asm_line );
	//	もし DAC < 32768! なら → A = -1 なら、普通に FRCINT で変換。(-32768 より小さければ、FRCINT が Overflow 出す。)
	asm_line.set( "LD", "", "BC", "0x3245" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "0x8076" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_fcomp", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "A", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "convert_to_integer", "" );
	this->subroutines.push_back( asm_line );
	//	HL = int(DAC - 32768!) xor &H8000
	asm_line.set( "LD", "", "HL", s_label_32768 );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_arg_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_decsub", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "0x80" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	pop_single_real_dac:
void CASSEMBLER_LIST::activate_pop_single_real_dac( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "pop_single_real_dac" ) ) {
		return;
	}
	this->subrouines_list.push_back( "pop_single_real_dac" );
	this->add_label( "work_dac", "0x0f7f6" );
	asm_line.set( "LABEL", "", "pop_single_real_dac", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+0]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+4]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+6]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	pop_single_real_arg:
void CASSEMBLER_LIST::activate_pop_double_real_arg( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "pop_double_real_arg" ) ) {
		return;
	}
	this->subrouines_list.push_back( "pop_double_real_arg" );
	this->add_label( "work_arg", "0x0f847" );
	asm_line.set( "LABEL", "", "pop_double_real_arg", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+6]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+4]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+0]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	pop_double_real_dac:
void CASSEMBLER_LIST::activate_pop_double_real_dac( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "pop_double_real_dac" ) ) {
		return;
	}
	this->subrouines_list.push_back( "pop_double_real_dac" );
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_valtyp", "0x0f663" );
	asm_line.set( "LABEL", "", "pop_double_real_dac", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+6]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+4]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+0]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	single real [HL] → [DAC]
void CASSEMBLER_LIST::activate_ld_dac_single_real( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "ld_dac_single_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "ld_dac_single_real" );
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_valtyp", "0x0f663" );
	asm_line.set( "LABEL", "", "ld_dac_single_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "4" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+4]", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac+6]", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	double real [HL] → [DAC]
void CASSEMBLER_LIST::activate_ld_dac_double_real( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "ld_dac_double_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "ld_dac_double_real" );
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_valtyp", "0x0f663" );
	asm_line.set( "LABEL", "", "ld_dac_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	single real [HL] → [ARG]
void CASSEMBLER_LIST::activate_ld_arg_single_real( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "ld_arg_single_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "ld_arg_single_real" );
	this->add_label( "work_arg", "0x0f847" );
	this->add_label( "work_valtyp", "0x0f663" );
	asm_line.set( "LABEL", "", "ld_arg_single_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "work_arg" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "4" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+4]", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg+6]", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	double real [HL] → [ARG]
void CASSEMBLER_LIST::activate_ld_arg_double_real( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "ld_arg_double_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "ld_arg_double_real" );
	this->add_label( "work_arg", "0x0f847" );
	asm_line.set( "LABEL", "", "ld_arg_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "work_arg" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	single real [HL] → [DE]
void CASSEMBLER_LIST::activate_ld_de_single_real( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "ld_de_single_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "ld_de_single_real" );
	asm_line.set( "LABEL", "", "ld_de_single_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "4" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	double real [HL] → [DE]
void CASSEMBLER_LIST::activate_ld_de_double_real( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "ld_de_double_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "ld_de_double_real" );
	asm_line.set( "LABEL", "", "ld_de_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_puts( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "puts" ) ) {
		return;
	}
	this->subrouines_list.push_back( "puts" );
	asm_line.set( "LABEL", "", "puts", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "B", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "B", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "Z", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_puts_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x18", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DJNZ", "", "_puts_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	A に文字列長 1〜255 を入れて呼ぶ。 HL に確保したアドレスを返す。
void CASSEMBLER_LIST::activate_allocate_string( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "allocate_string" ) ) {
		return;
	}
	this->subrouines_list.push_back( "allocate_string" );
	this->add_label( "bios_errhand", "0x0406F" );
	asm_line.set( "COMMENT", "", "Allocate memory for strings. A: Length" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "allocate_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[heap_next]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[heap_end]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_allocate_string_error", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[heap_next]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_allocate_string_error", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "7" );			//	Out of memory.
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "bios_errhand", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	BC に確保するサイズを入れて呼ぶ。 HL に確保したアドレスを返す。
void CASSEMBLER_LIST::activate_allocate_heap( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "allocate_heap" ) ) {
		return;
	}
	this->subrouines_list.push_back( "allocate_heap" );
	this->add_label( "bios_errhand", "0x0406F" );
	asm_line.set( "LABEL", "", "allocate_heap", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[heap_next]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_allocate_heap_error", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[heap_end]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_allocate_heap_error", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[heap_next]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_allocate_heap_error", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "7" );			//	Out of memory.
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "bios_errhand", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_free_string( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "free_string" ) ) {
		return;
	}
	this->subrouines_list.push_back( "free_string" );
	asm_line.set( "LABEL", "", "free_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "heap_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[heap_next]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "free_heap", "" );
	this->subroutines.push_back( asm_line );
	this->activate_free_heap();
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_free_array( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "free_array" ) ) {
		return;
	}
	this->subrouines_list.push_back( "free_array" );
	asm_line.set( "LABEL", "", "free_array", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "heap_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[heap_next]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "BC", "" );				//	[サイズ] の 2byte を追加
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "free_heap", "" );
	this->subroutines.push_back( asm_line );
	this->activate_free_heap();
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_free_sarray( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "free_sarray" ) ) {
		return;
	}
	this->activate_free_string();
	this->subrouines_list.push_back( "free_sarray" );
	asm_line.set( "LABEL", "", "free_sarray", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "heap_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[heap_next]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );				//	配列そのもののアドレスを保存
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RR", "", "H", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RR", "", "L", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_free_sarray_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "free_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_free_sarray_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "free_heap", "" );
	this->subroutines.push_back( asm_line );
	this->activate_free_heap();
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_free_heap( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "free_heap" ) ) {
		return;
	}
	//	HL に解放するメモリのアドレス、BC に解放するメモリのサイズを入れて呼び出す
	this->subrouines_list.push_back( "free_heap" );
	asm_line.set( "LABEL", "", "free_heap", "" );
	this->subroutines.push_back( asm_line );
	//	ブロック転送の情報をメモリに保存する
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[heap_move_size]", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[heap_remap_address]", "HL" );
	this->subroutines.push_back( asm_line );
	//	ブロック転送のサイズやアドレスを計算する
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[heap_next]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_free_heap_loop0", "" );
	this->subroutines.push_back( asm_line );
	//	ブロック転送する
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_free_heap_loop0", "" );
	this->subroutines.push_back( asm_line );
	//	文字列変数/数値配列変数/文字列配列変数の実体アドレスを更新する
	asm_line.set( "LD", "", "[heap_next]", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "vars_area_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_free_heap_loop1", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "varsa_area_end" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_free_heap_loop1_end", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[heap_remap_address]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_free_heap_loop1_next", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[heap_move_size]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_free_heap_loop1_next", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_free_heap_loop1", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_free_heap_loop1_end", "" );
	this->subroutines.push_back( asm_line );
	//	文字列配列変数の中身に入っている実体アドレスを更新する
	asm_line.set( "LD", "", "HL", "varsa_area_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_free_heap_loop2", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "varsa_area_end" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_free_heap_loop2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	//	実体のアドレスに変換する
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RR", "", "H", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RR", "", "L", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	//	文字列配列変数１つ分
	asm_line.set( "LABEL", "", "_free_heap_sarray_elements", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	//	文字列配列変数の中の要素１つ分
	//	heap_remap_address 以上の値なら対象
	asm_line.set( "LD", "", "HL", "[heap_remap_address]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_free_heap_loop2_next", "" );
	this->subroutines.push_back( asm_line );
	//	対象のアドレスなので処理する
	asm_line.set( "LD", "", "DE", "[heap_move_size]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_free_heap_loop2_next", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_free_heap_sarray_elements", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_free_heap_loop2", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	新たに確保した文字列配列に、空文字列を詰める
//	HL ... 文字列配列の実体の先頭 (要素 (0, 0, ..., 0) のアドレス)
//	DE ... 詰めるサイズ (byte数)
void CASSEMBLER_LIST::activate_init_string_array( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "init_string_array" ) ) {
		return;
	}
	this->subrouines_list.push_back( "init_string_array" );
	asm_line.set( "LABEL", "", "init_string_array", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "str_0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_init_string_array_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_init_string_array_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	[HL] の文字列を格納できる領域を確保してコピー後、HLにアドレスを返す。
void CASSEMBLER_LIST::activate_copy_string( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "copy_string" ) ) {
		return;
	}
	this->subrouines_list.push_back( "copy_string" );
	this->activate_allocate_string();
	asm_line.set( "LABEL", "", "copy_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "allocate_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );

}

// --------------------------------------------------------------------
//	HLに変数などの文字列のアドレスを指定して呼ぶ
//	更新可能な文字列であれば、それをそのまま HL に返す
//	更新不可能な文字列であれば、コピーを作成してそのアドレスを HL に返す
void CASSEMBLER_LIST::activate_get_writeable_string( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "get_writeable_string" ) ) {
		return;
	}
	this->subrouines_list.push_back( "get_writeable_string" );
	this->activate_copy_string();
	asm_line.set( "LABEL", "", "get_writeable_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "heap_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_get_writeable_string_make_copy" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[heap_next]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_get_writeable_string_make_copy" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "copy_string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_str( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "str" ) ) {
		return;
	}
	// DACに入っている数値を文字列に変換する STR(dac)
	// 結果は一時バッファへカエされるので free_string の必要は無いが
	// 変数などへ代入する場合は、コピーを作成して代入すること。
	this->subrouines_list.push_back( "str" );
	this->add_label( "bios_fout", "0x03425" );
	asm_line.set( "LABEL", "", "str", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_fout", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "fout_adjust", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_str_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_str_loop_exit", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "B", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_str_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_str_loop_exit", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_string( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "string" ) ) {
		return;
	}
	this->activate_allocate_string();
	this->add_label( "bios_errhand", "0x0406F" );
	this->subrouines_list.push_back( "string" );
	asm_line.set( "LABEL", "", "string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "string_a", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "5" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "bios_errhand", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "string_a", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "allocate_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "B", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "string_loop_enter", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "string_loop_enter", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DJNZ", "", "string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_single_real_is_zero( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "single_real_is_zero" ) ) {
		return;
	}
	this->subrouines_list.push_back( "single_real_is_zero" );
	asm_line.set( "LABEL", "", "single_real_is_zero", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_double_real_is_zero( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "double_real_is_zero" ) ) {
		return;
	}
	this->subrouines_list.push_back( "double_real_is_zero" );
	asm_line.set( "LABEL", "", "double_real_is_zero", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_check_array( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "check_array" ) ) {
		return;
	}
	this->activate_allocate_heap();
	this->subrouines_list.push_back( "check_array" );

	asm_line.set( "LABEL", "", "check_array", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NZ", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "allocate_heap", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "11" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_check_array_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DJNZ", "", "_check_array_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	文字列配列をチェックする
//	HL ... 文字列配列変数の格納アドレス
//	D .... 次元数
//	BC ... 
void CASSEMBLER_LIST::activate_check_sarray( CCONSTANT_INFO *p_constants ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "check_sarray" ) ) {
		return;
	}
	this->activate_allocate_heap();
	this->subrouines_list.push_back( "check_sarray" );

	asm_line.set( "LABEL", "", "check_sarray", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NZ", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "allocate_heap", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RR", "", "B", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RR", "", "C", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "11" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_check_sarray_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "A", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_check_sarray_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", p_constants->s_blank_string );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_calc_array_top( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "calc_array_top" ) ) {
		return;
	}
	this->subrouines_list.push_back( "calc_array_top" );

	asm_line.set( "LABEL", "", "calc_array_top", "" );
	this->subroutines.push_back( asm_line );
	//	変数の指し示す先を読む
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	//	サイズフィールドを読み飛ばす
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	//	次元数フィールドを取得
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "E" );
	this->subroutines.push_back( asm_line );
	//	戻りアドレスをスタックから DE へ待避
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	//	最初の要素のアドレスをスタックへ積む
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	//	要素数フィールドを読み取ってスタックに積む
	asm_line.set( "JR", "", "_calc_array_top_l2", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_calc_array_top_l1", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_calc_array_top_l2", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "A", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_calc_array_top_l1", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_sub_input( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_input" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_input" );
	this->activate_puts();
	this->activate_allocate_string();
	this->activate_free_string();
	this->add_label( "bios_qinlin", "0x00B4" );
	this->add_label( "bios_fin", "0x3299" );
	this->add_label( "bios_frcint", "0x2f8a" );
	this->add_label( "bios_frcsng", "0x2fb2" );
	this->add_label( "bios_frcdbl", "0x303a" );
	this->add_label( "work_dac", "0xf7f6" );
	this->add_label( "work_valtyp", "0xf663" );

	asm_line.set( "COMMENT", "", "INPUT instruction entity" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_input", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_qinlin", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "3" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_input_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_number", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_fin", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "Z", "_sub_input_all_blank", "" );		//	何も入力していない場合は、残りの変数全て 0 や "" にする
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_input_skip_white", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_input_number_branch", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "', '" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_input_number_branch", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "_sub_input_redo_from_start", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_number_branch", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[de]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "4" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "Z", "_sub_input_single_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_input_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "\t-- Case of INTEGER", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "de", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[hl]", "e" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[hl]", "d" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "_check_next_data", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "\t-- Case of SINGLE REAL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_single_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcsng", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "hl", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "bc", "4" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_check_next_data", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "\t-- Case of DOUBLE REAL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcdbl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "hl", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "bc", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_check_next_data", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "\t-- Case of STRING", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_input_skip_white", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "'\"'" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_get_quote_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_get_normal_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "e", "l" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "d", "h" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "c", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_get_normal_string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_get_string_loop_exit", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "', '" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_get_string_loop_exit", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "c", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_get_normal_string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_get_quote_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "e", "l" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "d", "h" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "c", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_get_quote_string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_get_string_loop_exit", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "'\"'" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_get_string_loop_exit", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "c", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_get_quote_string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_get_string_loop_exit", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "c" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_get_quote_string_zero", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "allocate_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "b", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "bc", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_input_copy_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_get_quote_string_zero", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "bc", "str_0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_copy_string", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "de" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "e", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[hl]", "c" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "d", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[hl]", "b" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "hl", "svars_input_free_str0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_SUB_INPUT_FREE_STRING" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_SUB_INPUT_SET_FREE_STRING" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_SUB_INPUT_FREE_STRING" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_SUB_INPUT_SET_FREE_STRING" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[hl]", "d" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[hl]", "e" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_check_next_data", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "'\"'" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_check_next_data2", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_check_next_data2", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_input_skip_white", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_check_next_parameter", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "de", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[de]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_input_extra_ignored", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "_sub_input_string_loop", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_check_next_parameter", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_SUB_INPUT_ALL_FREE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_retype" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "'?'" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x18", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "sub_input", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_redo_from_start", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "hl", "_str_redo_from_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "puts", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_input_retype", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_str_redo_from_start", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEFB", "", "18", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEFB", "", "\"?Redo from start\"", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEFB", "", "13, 10", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_extra_ignored", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "hl", "_str_extra_ignored" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "puts", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_SUB_INPUT_ALL_FREE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_str_extra_ignored", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEFB", "", "16", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEFB", "", "\"?Extra ignored\"", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEFB", "", "13, 10", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_skip_white", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "a", "' '" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NZ", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_input_skip_white", "" );
	this->subroutines.push_back( asm_line );
	//	全ての残りの変数に 0 または "" を格納する
	asm_line.set( "COMMENT", "", "Store 0 or \"\" in all remaining variables" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_blank" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", ";\t-- Get return address (address with type information)" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "pop", "", "bc" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_blank_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "a", "[bc]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "bc" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "or", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "z", "_sub_input_all_blank_exit" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "cp", "", "a", "3" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "", "nz, _sub_input_all_blank_put" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "\t-- If it is a string, add it to the candidates to be freed later, and then stuff \"\"." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "pop", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "e", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "d", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "[hl]", "str_0 >> 8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "dec", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "[hl]", "str_0 & 255" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "hl", "SVARS_INPUT_FREE_STR0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_blank_regist_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "or", "", "a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "z", "_sub_input_all_blank_regist" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "", "_sub_input_all_blank_regist_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_blank_regist" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "[hl]", "d" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "dec", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "[hl]", "e" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "", "_sub_input_all_blank_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_blank_put" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "pop", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "e", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_blank_fill" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "[hl]", "e" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "dec", "", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "nz", "_sub_input_all_blank_fill" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "", "_sub_input_all_blank_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_blank_exit" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "bc" );
	this->subroutines.push_back( asm_line );

	//	不要になった文字列変数領域の開放処理
	asm_line.set( "COMMENT", "", "Release of unneeded string variable areas" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_free" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "de", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "hl", "svars_input_free_str0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "a", "7" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_free_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "c", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "b", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "or", "", "a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ex", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "push", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "sbc", "", "hl", "bc" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "pop", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ex", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "nc", "_sub_input_all_free_no_swap" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "e", "c" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "d", "b" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "[hl]", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "dec", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "[hl]", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_input_all_free_no_swap" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "inc", "", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "dec", "", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "nz", "_sub_input_all_free_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ld", "", "a", "e" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "or", "", "a", "d" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ret", "Z" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ex", "", "de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "call", "", "free_string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "jr", "", "_sub_input_all_free" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_bload_r( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_bload_r" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_bload_r" );
	this->add_label( "work_buf", "0x0F55E" );
	this->add_label( "work_himem", "0x0FC4A" );
	this->add_label( "blib_bload", "0x04054" );
	asm_line.set( "LABEL", "", "sub_bload_r", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "restore_h_erro", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "restore_h_timi", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "sub_bload_r_trans_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "sub_bload_r_trans" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "sub_bload_r_trans_end - sub_bload_r_trans" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_bload_r_trans", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DI", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "setup_h_timi", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "setup_h_erro", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EI", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );

	asm_line.set( "LABEL", "", "sub_bload_r_trans_start", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ORG", "", "work_buf + 50", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_bload_r_trans", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "iy", "[work_blibslot - 1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "ix", "blib_bload" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_calslt", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "hl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_bload_r_trans_end", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ORG", "", "sub_bload_r_trans_start + sub_bload_r_trans_end - sub_bload_r_trans", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_bload( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_bload" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_bload" );
	this->add_label( "work_buf", "0x0F55E" );
	this->add_label( "work_himem", "0x0FC4A" );
	this->add_label( "blib_bload", "0x04054" );
	asm_line.set( "LABEL", "", "sub_bload", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "restore_h_erro", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "restore_h_timi", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "sub_bload_trans_start" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "sub_bload_trans" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "sub_bload_trans_end - sub_bload_trans" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_bload_trans", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DI", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "setup_h_timi", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "setup_h_erro", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EI", "", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "", "", "" );
	this->subroutines.push_back( asm_line );

	asm_line.set( "LABEL", "", "sub_bload_trans_start", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ORG", "", "work_buf + 50", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_bload_trans", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "iy", "[work_blibslot - 1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "ix", "blib_bload" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_calslt", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "hl", "[work_himem]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "de", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RST", "", "0x20", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC", "", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "_bload_basic_end" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_newstt", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_bload_basic_end", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEFB", "", "':', 0x81, 0x00", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_bload_trans_end", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ORG", "", "sub_bload_trans_start + sub_bload_trans_end - sub_bload_trans", "" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_paint( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_paint" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_paint" );

	this->add_label( "work_forclr", "0x0F3E9" );
	this->add_label( "work_brdatr", "0x0FCB2" );
	this->add_label( "work_gxpos", "0x0FCB3" );
	this->add_label( "work_gypos", "0x0FCB5" );
	this->add_label( "work_scrmod", "0x0FCAF" );
	this->add_label( "work_atrbyt", "0x0F3F2" );
	this->add_label( "bios_setatr", "0x0011A" );
	this->add_label( "bios_paint", "0x059E3" );
	this->add_label( "subrom_paint", "0x0266E" );
	this->add_label( "bios_extrom", "0x0015F" );

	asm_line.set( "COMMENT", "", "Paint subroutine" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_paint" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_gxpos]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_gypos]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_brdatr]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_scrmod]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "5" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "sub_paint_vdpcmd" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "Case of SCREEN2...4 (Without VDP Command)" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_atrbyt]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "bios_paint" );
	this->subroutines.push_back( asm_line );

	asm_line.set( "COMMENT", "", "Case of SCREEN5...12 (With VDP Command)" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_paint_vdpcmd" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_brdatr]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "subrom_paint" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_extrom" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EI" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_read_string(void) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_read_string" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_read_string" );

	asm_line.set( "COMMENT", "", "Read data for string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_read_string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[data_ptr]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[data_ptr]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_val(void) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_val" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_val" );

	this->add_label( "bios_fin", "0x3299" );
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_buf", "0x0f55e" );

	asm_line.set( "COMMENT", "", "Val function" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_val" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "work_buf" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[DE]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_buf" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_fin", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_read_integer(void) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_read_integer" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_read_integer" );

	this->activate_val();
	this->add_label( "bios_frcint", "0x2f8a" );
	this->add_label( "work_dac", "0x0f7f6" );

	asm_line.set( "COMMENT", "", "Read data for integer" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_read_integer" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[data_ptr]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[data_ptr]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_val", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_read_single(void) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_read_single" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_read_single" );

	this->activate_val();
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "bios_frcsng", "0x2fb2" );
	this->activate_ld_de_single_real();

	asm_line.set( "COMMENT", "", "Read data for single-real value" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_read_single" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[data_ptr]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[data_ptr]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_val", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcsng", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_de_single_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_read_double(void) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_read_double" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_read_double" );

	this->activate_val();
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "bios_frcdbl", "0x303a" );
	this->activate_ld_de_double_real();

	asm_line.set( "COMMENT", "", "Read data for double-real value" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_read_double" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[data_ptr]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[data_ptr]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_val", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcdbl", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_de_double_real", "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_circle( class CCOMPILE_INFO *p_info ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_circle" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_circle" );

	this->add_label( "bios_frcsng", "0x02FB2" );
	this->add_label( "bios_decmul", "0x027e6" );
	this->add_label( "bios_frcint", "0x02f8a" );
	this->add_label( "blib_get_sin_table", "0x040de" );
	this->add_label( "bios_line", "0x058FC" );
	this->add_label( "work_rg9sav", "0x0FFE8" );
	this->add_label( "work_aspct1", "0x0f40b" );
	this->add_label( "work_aspct2", "0x0f40d" );
	this->add_label( "work_buf", "0x0f55e" );
	this->add_label( "work_valtyp", "0x0f663" );
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_scrmod", "0x0fcaf" );
	this->add_label( "work_circle_x_shift", "work_buf + 65" );
	this->add_label( "work_circle_quadrant0", "work_buf + 66" );
	this->add_label( "work_circle_quadrant1", "work_buf + 67" );
	this->add_label( "work_circle_quadrant2", "work_buf + 68" );
	this->add_label( "work_circle_quadrant3", "work_buf + 69" );
	this->add_label( "work_circle_centerx", "work_buf + 70" );
	this->add_label( "work_circle_centery", "work_buf + 72" );
	this->add_label( "work_circle_radiusx", "work_buf + 74" );
	this->add_label( "work_circle_radiusy", "work_buf + 76" );
	this->add_label( "work_circle_prev_cxoff1", "work_buf + 78" );
	this->add_label( "work_circle_prev_cyoff1", "work_buf + 80" );
	this->add_label( "work_circle_prev_cxoff2", "work_buf + 82" );
	this->add_label( "work_circle_prev_cyoff2", "work_buf + 84" );
	this->add_label( "work_circle_cxoff1", "work_buf + 86" );
	this->add_label( "work_circle_cyoff1", "work_buf + 88" );
	this->add_label( "work_circle_cxoff2", "work_buf + 90" );
	this->add_label( "work_circle_cyoff2", "work_buf + 92" );
	this->add_label( "work_arg", "0x0F847" );
	this->add_label( "bios_chgmod", "0x0005F" );
	this->add_label( "work_gxpos", "0x0FCB3" );
	this->add_label( "work_gypos", "0x0FCB5" );
	this->add_label( "work_aspect", "0x0F931" );
	this->add_label( "work_cpcnt", "0x0F939" );
	this->add_label( "work_crcsum", "0x0F93D" );
	this->add_label( "work_csclxy", "0x0F941" );
	this->add_label( "work_savea", "0x0F942" );
	this->add_label( "work_savem", "0x0F944" );
	this->add_label( "work_cxoff", "0x0F945" );
	this->add_label( "work_cyoff", "0x0F947" );
	this->add_label( "bios_setatr", "0x0011A" );

	CSINGLE_REAL value;
	value.set( "40.7437" );
	std::string s_407437 = p_info->constants.add( value );

	this->activate_ld_arg_single_real();
	this->activate_ld_dac_single_real();
	this->activate_ld_de_single_real();

	asm_line.set( "COMMENT", "", " Circle routine --------------------------------------------------------------" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_circle" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	Calclate vertical radius" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_aspect" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_arg_single_real" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_radiusx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac + 2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcsng" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_decmul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_aspct2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RLCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SRL", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RR", "", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_radiusy]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	Convert Start angle to 0...255." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_cpcnt" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_arg_single_real" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "CONST_42407437" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_dac_single_real" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "4" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_cpcnt]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "0x7F" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_decmul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_cpcnt]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "0x80" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_cpcnt + 1]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	Convert End angle to 0〜255." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_crcsum" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_arg_single_real" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "CONST_42407437" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "ld_dac_single_real" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "4" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_crcsum]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "0x7F" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_arg]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_decmul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_frcint" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_dac + 2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_crcsum]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "0x80" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_crcsum + 1]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	Compare Start and End angle" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_cpcnt]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RLA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_savem]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	Get sin table" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_get_sin_table" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "call_blib" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	Initial value of Horizontal radius." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_radiusx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cxoff1]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_radiusy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cyoff2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cxoff2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cyoff1]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	θ = 0[deg]→45[deg]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_csclxy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_theta_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "		Move previous value to PREV section." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_circle_cxoff1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "work_circle_prev_cxoff1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "		X1 = cos(THETA) * Horizontal radius, Y2 = cos(THETA) * Vertical radius" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_cos" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_circle_radiusx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_mul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cxoff1]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_circle_radiusy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_mul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cyoff2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "		Y1 = sin(THETA) * Horizontal radius, X2 = sin(THETA) * Vertical radius" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_sin" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_circle_radiusy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_mul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cyoff1]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_circle_radiusx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_mul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cxoff2]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "		Quadrant 0 (0[deg]...90[deg])" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant0_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_circle_quadrant0]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_sub_circle_quadrant1_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "3" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_quadrant_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "		Quadrant 1 (90[deg]...180[deg])" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant1_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_circle_quadrant1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_sub_circle_quadrant2_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "128" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SUB", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_quadrant_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "		Quadrant 2 (180[deg]...270[deg])" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant2_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_circle_quadrant2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_sub_circle_quadrant3_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "A", "128" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_quadrant_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "		Quadrant 3 (270[deg]〜360[deg])" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant3_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_circle_quadrant3]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_sub_circle_quadrant_end" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "NEG" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_quadrant_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_end" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "BIT", "", "5", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "NZ", "_sub_circle_line_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_csclxy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "_sub_circle_theta_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Is angle B a drawing target?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Cy = 0: target, 1: not target" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_check_angle" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Which is larger, the start or end angle?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_savem]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_circle_check_angle_cond_or" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_check_angle_cond_and" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Is the target angle greater than the starting angle?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_cpcnt]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_circle_check_angle_left1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CCF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_check_angle_left1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " The target angle is greater than the start angle. Then, is the angle of target smaller than the end angle?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_crcsum]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "Z" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_check_angle_cond_or" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Is the target angle greater than the start angle?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_cpcnt]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CCF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " The target angle is smaller than the start angle. So is the target angle smaller than the end angle?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_crcsum]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " If bit0 of A is 0, X sign is inverted; if bit1 is 0, Y sign is inverted." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Angle 0 to 31 (0[deg] to 45[deg]) on B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_check_angle" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_quadrant_line1_skip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Start point X1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centerx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_cxoff1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_quadrant_process_add_cx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_cx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_cx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_cx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_cx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gxpos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Start point Y1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centery]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_cyoff1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_quadrant_process_add_cy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_cy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_cy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_cy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_cy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gypos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " End point X1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centerx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_prev_cxoff1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_quadrant_process_add_pcx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_pcx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_pcx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_pcx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_pcx1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " End point Y1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centery]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_prev_cyoff1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_quadrant_process_add_pcy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_pcy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_pcy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_pcy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_pcy1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_draw_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_line1_skip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "63" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_check_angle" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_quadrant_line2_skip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Start point X2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centerx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_cxoff2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_quadrant_process_add_cx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_cx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_cx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_cx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_cx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gxpos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Start point Y2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centery]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_cyoff2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_quadrant_process_add_cy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_cy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_cy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_cy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_cy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gypos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " End point X2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centerx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_prev_cxoff2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_quadrant_process_add_pcx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_pcx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_pcx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_pcx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_pcx2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " End point Y2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centery]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_prev_cyoff2]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_quadrant_process_add_pcy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_sub_pcy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SBC", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_quadrant_process_set_pcy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_add_pcy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_process_set_pcy2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_draw_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_quadrant_line2_skip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_draw_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_gxpos]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_x_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gxpos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_x_reject_check" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_x_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "", "_sub_circle_draw_line_y" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_x_reject_check" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_x_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_draw_line_y" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_gxpos+1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "Z" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_draw_line_y" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_gypos]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_y_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gypos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "C", "_sub_circle_y_reject_check" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_y_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "bios_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_y_reject_check" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_y_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "NC", "bios_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_gypos+1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "NZ", "bios_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Clipping X-coordinates in HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " If clipping, return with Cy=1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_x_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RLA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_x_clip_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_x_clip_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_aspct1 + 1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_aspct1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SCF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Clipping Y-coordinates in HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " If clipping, return with Cy=1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_y_clip" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RLA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_y_clip_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_y_clip_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_circle_y_clip_skip2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "211" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_rg9sav]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RLA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "191" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CCF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_y_clip_skip2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "192" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CCF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_rg9sav]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RLA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_y_clip_192" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "212" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CCF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "NC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "211" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_y_clip_192" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "191" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SCF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_line_process" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Connect the center point and the end angle with a line segment?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_crcsum]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "NZ", "_sub_circle_draw_radius" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_end_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Connect the center point and the starting angle with a line segment?" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_cpcnt]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "Z" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Correction of problems with clockwise and counterclockwise shift depending on the position of the 8 quadrants" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "BIT", "", "5", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_circle_draw_radius" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_csclxy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_draw_radius" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Correct angle to resolution" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_csclxy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CPL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Correction of problems with clockwise and counterclockwise shift depending on the position of the 8 quadrants" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RRCA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "XOR", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "BIT", "", "5", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_sub_circle_draw_radius_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_draw_radius_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_cos			; HL = cos(THETA)" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_circle_radiusx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_mul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cxoff1]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_sin			; HL = sin(THETA)" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_circle_radiusy]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "_sub_circle_mul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_circle_cyoff1]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Start point X1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centerx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_cxoff1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gxpos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " Start pointY1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "[work_circle_centery]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_cyoff1]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_gypos]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " End point X1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "BC", "[work_circle_centerx]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", " 終点Y1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[work_circle_centery]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "_sub_circle_draw_line" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	HL = HL * BC >> 8, HL is signed, BC is unsigned." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	   = (HL * C >> 8) + HL * B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_mul" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "P", "_sub_circle_skip_abs" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CPL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CPL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_skip_abs" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX", "", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_mul_1st8bit" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SLA", "", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RL", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SLA", "", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_mul_1st8bit_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_mul_1st8bit_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_sub_circle_mul_1st8bit" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RL", "", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_mul_2nd8bit" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_mul_2nd8bit" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SRL", "", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_circle_mul_2nd8bit_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_mul_2nd8bit_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SLA", "", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RL", "", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DJNZ", "", "_sub_circle_mul_2nd8bit" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "P" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CPL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CPL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	Get cos(THETA): A = THETA (0:0[deg]〜256:360[deg]) → A = cos(THETA)" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_cos" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "SUB", "", "A", "64" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "COMMENT", "", "	sinθを返す: A = THETA (0:0[deg]〜256:360[deg]) → A = sin(THETA)" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_sin" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "0x3F" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_circle_sin_special" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "BIT", "", "6", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "Z", "_sub_circle_sin_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "NEG" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_sin_skip1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "0x3F" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "A", "work_buf & 0x0FF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "work_buf >> 8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RL", "", "B" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CPL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_circle_sin_special" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "BIT", "", "6", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "Z" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RLA" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	開いているファイルを閉じる
//	L = 1〜15 : CLOSE #L
//
void CASSEMBLER_LIST::activate_close( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_close" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_close" );

	this->add_label( "bios_errhand", "0x0406F" );
	this->add_label( "work_maxfil", "0x0f85f" );
	this->add_label( "bios_imult", "0x03193" );
	this->add_label( "blib_fclose", "0x04063" );

	asm_line.set( "LABEL", "", "sub_close" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_maxfil]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "52" );				//	Bad file number
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "C", "bios_errhand" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "NZ", "bios_errhand" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "Z" );
	this->subroutines.push_back( asm_line );			//	CLOSE#0 は、何もせずに戻る(エラーにはならない)
	asm_line.set( "DEC", "", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "file_info_size" );				//	FCB 37 + BUFFER 256
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_imult" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[svaria_file_info]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[HL]" );				//	0: No Open, 1〜8: FILE, 128: GRP, 129: CON, 130: CRT, 255: NUL
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "8" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "sub_close_end" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_fclose" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "call_blib" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_close_end" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "0" );				//	No Open にマークする
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	開いている全てのファイルを閉じる
void CASSEMBLER_LIST::activate_all_close( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_all_close" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_all_close" );
	this->activate_close();

	this->add_label( "work_maxfil", "0x0f85f" );
	asm_line.set( "LABEL", "", "sub_all_close" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "[work_maxfil]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET", "Z" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "H", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "L", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "sub_all_close_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_close" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "sub_all_close_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	HL = 新しい MAXFILES の値
void CASSEMBLER_LIST::activate_maxfiles( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_maxfiles" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_maxfiles" );
	this->activate_all_close();
	this->activate_init_files();
	this->add_label( "work_maxfil", "0x0f85f" );

	asm_line.set( "LABEL", "", "sub_maxfiles" );
	this->subroutines.push_back( asm_line );

	//	まず現在開いているファイルを全て close する
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_all_close" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL" );
	this->subroutines.push_back( asm_line );
	//	maxfil を更新
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "AND", "", "A", "15" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_maxfil]", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "sub_init_files" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	
void CASSEMBLER_LIST::activate_init_files( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_init_files" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_init_files" );
	this->add_label( "bios_imult", "0x03193" );
	this->activate_allocate_heap();
	this->activate_free_heap();

	asm_line.set( "LABEL", "", "sub_init_files" );
	this->subroutines.push_back( asm_line );

	//	メモリが確保済みであれば解放する
	asm_line.set( "LD", "", "HL", "[svaria_file_info]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "BC" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "NZ", "free_heap" );
	this->subroutines.push_back( asm_line );

	//	メモリを確保する。ゼロクリアされるので、全て閉じた状態になる。
	asm_line.set( "LD", "", "A", "[work_maxfil]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "D", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "file_info_size" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_imult" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "C", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "B", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "allocate_heap" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[svaria_file_info]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[HL]", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_open_for_input( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_open_for_input" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_open_for_input" );
	this->add_label( "blib_open_for_input", "0x40e4" );
	this->activate_open_sub();

	//	HL ... ファイル名
	//	DE ... ファイル番号
	asm_line.set( "LABEL", "", "sub_open_for_input" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_open_sub" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_open_for_output" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "call_blib" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_open_for_output( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_open_for_output" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_open_for_output" );
	this->add_label( "blib_open_for_output", "0x40e7" );
	this->activate_open_sub();

	//	HL ... ファイル名
	//	DE ... ファイル番号
	asm_line.set( "LABEL", "", "sub_open_for_output" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_open_sub" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_open_for_output" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "call_blib" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_open_for_append( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_open_for_append" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_open_for_append" );
	this->add_label( "blib_open_for_append", "0x40ea" );
	this->activate_open_sub();

	//	HL ... ファイル名
	//	DE ... ファイル番号
	asm_line.set( "LABEL", "", "sub_open_for_append" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_open_sub" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_open_for_append" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "call_blib" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_open_for_none( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_open_for_none" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_open_for_none" );
	this->add_label( "blib_open_for_none", "0x40f9" );
	this->activate_open_sub();

	//	HL ... ファイル名
	//	DE ... ファイル番号
	//	A .... LEN
	asm_line.set( "LABEL", "", "sub_open_for_none" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "sub_open_sub" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_open_for_none" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "call_blib" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_open_sub( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_open_sub" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_open_sub" );
	this->add_label( "bios_imult", "0x03193" );
	this->add_label( "bios_errhand", "0x0406F" );
	this->add_label( "work_ptrfil", "0x0f864" );	//	結果の格納先

	//	HL ... ファイル名
	//	DE ... ファイル番号
	//	→ work_ptrfil に FILE_INFO
	asm_line.set( "LABEL", "", "sub_open_sub" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "D" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR", "", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NZ", "_sub_open_bad_file_number" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "E" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "15" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR", "NC", "_sub_open_bad_file_number" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "file_info_size" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_imult" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[SVARIA_FILE_INFO]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_ptrfil]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL", "", "_sub_open_bad_file_number" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "52" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "bios_errhand" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
//	ファイル番号 #n の n から FILE_INFO のアドレスを求めるルーチン
//	HL = 1〜15
void CASSEMBLER_LIST::activate_file_number( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_file_number" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_file_number" );
	this->add_label( "bios_imult", "0x03193" );
	this->add_label( "work_ptrfil", "0x0f864" );	//	結果の格納先

	asm_line.set( "LABEL", "", "sub_file_number" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP", "", "A", "15" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "E", "52" );			//	Bad file number
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "NC", "bios_errhand" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC", "", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "NZ", "bios_errhand" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "file_info_size" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "", "bios_imult" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "DE", "[svaria_file_info]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD", "", "HL", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_ptrfil]", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_put_integer( class CCOMPILE_INFO* p_info ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "put_integer" ) ) {
		return;
	}
	this->subrouines_list.push_back( "put_integer" );

	this->activate_str();
	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_valtyp", "0x0f663" );
	this->add_label( "blib_put_digits", "0x040f6" );

	asm_line.set( "LABEL", "", "put_integer" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_dac + 2]",  "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "A", "2" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "[work_valtyp]",  "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "","str",  "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_put_digits" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "","call_blib" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_put_single_real( class CCOMPILE_INFO* p_info ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "put_single_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "put_single_real" );

	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_valtyp", "0x0f663" );
	this->add_label( "blib_put_digits", "0x040f6" );

	this->activate_str();
	this->activate_ld_dac_single_real();

	asm_line.set( "LABEL", "", "put_single_real" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "","ld_dac_single_real",  "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "","str",  "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_put_digits" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "","call_blib" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_put_double_real( class CCOMPILE_INFO *p_info ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "put_double_real" ) ) {
		return;
	}
	this->subrouines_list.push_back( "put_double_real" );

	this->add_label( "work_dac", "0x0f7f6" );
	this->add_label( "work_valtyp", "0x0f663" );
	this->add_label( "blib_put_digits", "0x040f6" );

	this->activate_str();
	this->activate_ld_dac_double_real();

	asm_line.set( "LABEL", "", "put_double_real" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "","ld_dac_double_real",  "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "HL", "work_dac" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "","str",  "" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_put_digits" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "","call_blib" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_put_string( class CCOMPILE_INFO* p_info ) {
	CASSEMBLER_LINE asm_line;
	static const char image[] = { 13, 10 };
	CSTRING value;

	if( this->is_registered_subroutine( "put_string" ) ) {
		return;
	}
	this->subrouines_list.push_back( "put_string" );

	std::string s_label_crlf = p_info->constants.add( value );

	p_info->assembler_list.activate_free_string();
	s_label_crlf = p_info->constants.add( value );

	asm_line.set( "LABEL", "", "put_string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD", "", "IX", "blib_file_puts" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL", "","call_blib" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP", "", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP", "", "free_string" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_field( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_field" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_field" );

	this->activate_space();
	this->activate_free_string();
	asm_line.set( "COMMENT",	"", "Auxiliary processing of FIELD instruction. HL: Address of variable, DE: Address of variable list, A: Length" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL",	"", "sub_field" );
	this->subroutines.push_back( asm_line );
	// 変数リストに変数のアドレスを書き込む
	asm_line.set( "EX",		"", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "[HL]", "A" );			// サイズを書き込む
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "[HL]", "E" );			// 変数アドレス下位
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "[HL]", "D" );			// 変数アドレス上位
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH",	"", "HL" );					// (1) 変数リストのアドレス保存
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH",	"", "DE" );					// (2) 変数のアドレス保存
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH",	"", "AF" );					// (3) サイズ保存
	this->subroutines.push_back( asm_line );
	// 変数のアドレスから文字列のアドレスを得る
	asm_line.set( "EX",		"", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "E", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "D", "[HL]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX",		"", "DE", "HL" );
	this->subroutines.push_back( asm_line );
	// 文字列を解放する
	asm_line.set( "CALL",	"", "free_string" );
		this->subroutines.push_back( asm_line );
	// 指定の長さの文字列を得る ( SPACE$() )
	asm_line.set( "POP",	"", "AF" );					// [3] サイズ復帰
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "L", "A" );				// L = サイズ
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL",	"", "sub_space" );			// HL = space$(A)
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP",	"", "DE" );					// [2] 変数のアドレスを復帰
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX",		"", "DE", "HL" );			// HL = 変数のアドレス, DE = SPACE$() で得た文字列のアドレス
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "[HL]", "E" );			// 変数に文字列のアドレス下位をセット
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "[HL]", "D" );			// 変数に文字列のアドレス上位をセット
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP",	"", "HL" );					// [1] 変数リストのアドレス
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_space( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_space" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_space" );

	this->activate_allocate_string();
	asm_line.set( "COMMENT",	"", "SPACE$ processing. L: Length. RETURN HL: Result string." );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL",	"", "sub_space" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "A", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH",	"", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL",	"", "allocate_string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP",	"", "AF" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR",		"", "A", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET",	"Z" );						//	指定の長さが 0 なら何もせずに戻る
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "C", "A" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "B", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "[HL]", "' '" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC",	"", "C" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET",	"Z" );						//	指定の長さが 1 ならここで戻る
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "E", "L" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"", "D", "H" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"", "DE" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LDIR" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC",	"", "HL" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::activate_get( void ) {
	CASSEMBLER_LINE asm_line;

	if( this->is_registered_subroutine( "sub_get" ) ) {
		return;
	}
	this->subrouines_list.push_back( "sub_get" );

	this->add_label( "_set_dta", "0x1A" );
	this->add_label( "_rdblk", "0x27" );
	this->add_label( "bdos", "0xF37D" );
	this->add_label( "work_buf", "0x0F55E" );
	this->add_label( "work_ptrfil", "0x0f864" );
	this->add_label( "bios_errhand", "0x0406F" );

	asm_line.set( "COMMENT","",		"[work_ptrfil] ... Address of FILE_INFO" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL",	"",		"sub_get" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"de", "[work_ptrfil]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"a", "[de]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR",		"",		"a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"e", "59" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP",		"z",	"bios_errhand" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CP",		"",		"a", "9" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"e", "61" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JP",		"nc",	"bios_errhand" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC",	"",		"hl" );					//	レコード番号を 0 オリジンに変える
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX",		"",		"de", "hl" );			//	DE = レコード番号
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"hl", "[work_ptrfil]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"bc", "33" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD",	"",		"hl", "bc" );			//	HL = FCB の ランダムレコードフィールド
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"[hl]", "e" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"[hl]", "d" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"de", "work_buf + 1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"c", "_set_dta" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL",	"",		"bdos" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"de", "[work_ptrfil]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"hl", "1" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"c", "_rdblk" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL",	"",		"bdos" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"hl", "[work_ptrfil]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"de", "37" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD",	"",		"hl", "de" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"de", "work_buf" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"[work_buf + 256]", "de" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LABEL",	"",		"sub_get_loop" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"a", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "OR",		"",		"a", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "RET",	"z" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"[de]", "a" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"e", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"d", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX",		"",		"de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"e", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"d", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "DEC",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "PUSH",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX",		"",		"de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL",	"",		"free_string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"hl", "[work_buf + 256]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"e", "[hl]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"d", "0" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "ADD",	"",		"hl", "de" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"de", "[work_buf + 256]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"[work_buf + 256]", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX",		"",		"de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "CALL",	"",		"copy_string" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP",	"",		"de" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "EX",		"",		"de", "hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"[hl]", "e" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "INC",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"[hl]", "d" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "POP",	"",		"hl" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "LD",		"",		"de", "[work_buf + 256]" );
	this->subroutines.push_back( asm_line );
	asm_line.set( "JR",		"",		"sub_get_loop" );
	this->subroutines.push_back( asm_line );
}

// --------------------------------------------------------------------
bool CASSEMBLER_LIST::save_sub( FILE *p_file, std::vector< CASSEMBLER_LINE > *p_list ) {
	bool b_result = true;
	std::vector< CASSEMBLER_LINE >::iterator p;

	for( p = p_list->begin(); p != p_list->end(); p++ ) {
		b_result = p->save( p_file ) && b_result;
	}
	return b_result;
}

// --------------------------------------------------------------------
void CASSEMBLER_LIST::add_label( const std::string s_name, const std::string s_value ) {

	//	既に存在しているラベルなのか調べる
	for( auto &p: this->label_list ) {
		if( p == s_name ) {
			//	存在している場合は何もしない
			return;
		}
	}
	//	リストに追加する
	this->label_list.push_back( s_name );

	CASSEMBLER_LINE asm_line;
	asm_line.set( "CONSTANT", "", s_name, s_value );
	this->define_labels.push_back( asm_line );
}

// --------------------------------------------------------------------
bool CASSEMBLER_LIST::save( const std::string s_file_name ) {
	FILE *p_file;
	bool result = true;

	p_file = fopen( s_file_name.c_str(), "w" );
	if( p_file == NULL ) {
		fprintf( stderr, "ERROR: Cannot create the '%s'.\n", s_file_name.c_str() );
		return false;
	}

	result &= this->save_sub( p_file, &(this->header) );
	result &= this->save_sub( p_file, &(this->define_labels) );
	result &= this->save_sub( p_file, &(this->body) );
	result &= this->save_sub( p_file, &(this->subroutines) );
	result &= this->save_sub( p_file, &(this->datas) );
	result &= this->save_sub( p_file, &(this->const_single_area) );
	result &= this->save_sub( p_file, &(this->const_double_area) );
	result &= this->save_sub( p_file, &(this->const_string_area) );
	result &= this->save_sub( p_file, &(this->variables_area) );
	result &= this->save_sub( p_file, &(this->footer) );
	fclose( p_file );

	return result;
}
