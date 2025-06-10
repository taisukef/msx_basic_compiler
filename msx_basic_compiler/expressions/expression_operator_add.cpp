// --------------------------------------------------------------------
//	Expression
// ====================================================================
//	2023/July/29th	t.hara
// --------------------------------------------------------------------
#include <string>
#include <vector>
#include "expression_operator_add.h"
#include "expression_term.h"

// --------------------------------------------------------------------
CEXPRESSION_NODE* CEXPRESSION_OPERATOR_ADD::optimization( CCOMPILE_INFO *p_info ) {
	CEXPRESSION_NODE* p;

	if( this->p_left == nullptr || this->p_right == nullptr ) {
		return nullptr;
	}
	p = this->p_left->optimization( p_info );
	if( p != nullptr ) {
		delete (this->p_left);
		this->p_left = p;
	}

	p = this->p_right->optimization( p_info );
	if( p != nullptr ) {
		delete (this->p_right);
		this->p_right = p;
	}

	if( this->p_left->is_constant && this->p_right->is_constant ) {
		//	左右の項が両方とも定数の場合
		if( this->p_left->type == CEXPRESSION_TYPE::STRING || this->p_right->type == CEXPRESSION_TYPE::STRING ) {
			//	文字列の場合
			if( this->p_left->type != this->p_right->type ) {
				//	文字列と何かを + している場合、エラーなので何もしない
				return nullptr;
			}
			CEXPRESSION_TERM *p_term  = new CEXPRESSION_TERM();
			p_term->type = CEXPRESSION_TYPE::STRING;
			p_term->s_value = this->p_left->s_value + this->p_right->s_value;
			return p_term;
		}
		else {
			//	数値の場合
			CEXPRESSION_TERM *p_left  = reinterpret_cast<CEXPRESSION_TERM*> (this->p_left);
			CEXPRESSION_TERM *p_right = reinterpret_cast<CEXPRESSION_TERM*> (this->p_right);
			double r = p_left->get_value() + p_right->get_value();

			CEXPRESSION_TERM *p_term  = new CEXPRESSION_TERM();
			p_term->set_type( p_left->type, p_right->type );
			p_term->set_double( r );
			return p_term;
		}
	}
	return nullptr;
}

// --------------------------------------------------------------------
void CEXPRESSION_OPERATOR_ADD::compile( CCOMPILE_INFO *p_info ) {
	CASSEMBLER_LINE asm_line;

	if( this->p_left == nullptr || this->p_right == nullptr ) {
		return;
	}
	//	先に項を処理
	this->p_left->compile( p_info );
	p_info->assembler_list.push_hl( this->p_left->type );
	this->p_right->compile( p_info );

	//	この演算子の演算結果の型を決める
	this->type_adjust_2op( p_info, this->p_left, this->p_right );
	if( this->type == CEXPRESSION_TYPE::INTEGER ) {
		//	整数の場合
		asm_line.set( CMNEMONIC_TYPE::POP, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "DE", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::ADD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::REGISTER, "DE" );
		p_info->assembler_list.body.push_back( asm_line );
	}
	else if( this->type == CEXPRESSION_TYPE::STRING ) {
		//	文字列の場合
		this->activate_str_add( p_info );
		p_info->assembler_list.activate_copy_string();
		asm_line.set( CMNEMONIC_TYPE::POP, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "DE", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "str_add", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
	}
	else {
		//	実数の場合
		p_info->assembler_list.add_label( "bios_decadd", "0x0269a" );
		p_info->assembler_list.add_label( "work_dac", "0x0f7f6" );
		asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "bios_decadd", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::CONSTANT, "work_dac" );
		p_info->assembler_list.body.push_back( asm_line );
	}
}

// --------------------------------------------------------------------
//	文字列の連結 [kbuf] ← [DE]+[HL]
void CEXPRESSION_OPERATOR_ADD::activate_str_add( CCOMPILE_INFO *p_info ) {
	CASSEMBLER_LINE asm_line;
	std::string s_label;

	if( !p_info->assembler_list.add_subroutines( "str_add" ) ) {
		return;
	}
	p_info->assembler_list.activate_free_string();
	p_info->assembler_list.activate_allocate_string();
	p_info->assembler_list.add_label( "bios_errhand", "0x0406F" );
	p_info->assembler_list.add_label( "work_buf", "0x0f55e" );
	asm_line.set( CMNEMONIC_TYPE::LABEL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "str_add", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::PUSH, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "DE", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::PUSH, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	//	長さを合算して 255を越えないか調べる
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::MEMORY, "[HL]" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "A", COPERAND_TYPE::MEMORY, "[DE]" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::ADD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "A", COPERAND_TYPE::MEMORY, "C" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::JR, CCONDITION::C, COPERAND_TYPE::CONSTANT, "_str_add_error", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	//	+ 演算子の右辺を保存
	asm_line.set( CMNEMONIC_TYPE::PUSH, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	//	+ 演算子の左辺を先に KBUF へ
	asm_line.set( CMNEMONIC_TYPE::EX, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "DE", COPERAND_TYPE::REGISTER, "HL" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::CONSTANT, "[HL]" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::INC, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "DE", COPERAND_TYPE::CONSTANT, "work_buf+1" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "B", COPERAND_TYPE::CONSTANT, "0" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::INC, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::DEC, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::JR, CCONDITION::Z, COPERAND_TYPE::CONSTANT, "_str_add_s1", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LDIR, CCONDITION::NONE, COPERAND_TYPE::NONE, "", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LABEL, CCONDITION::Z, COPERAND_TYPE::CONSTANT, "_str_add_s1", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );

	asm_line.set( CMNEMONIC_TYPE::POP, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::MEMORY, "[HL]" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::INC, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::INC, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::DEC, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::JR, CCONDITION::Z, COPERAND_TYPE::CONSTANT, "_str_add_s2", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LDIR, CCONDITION::NONE, COPERAND_TYPE::NONE, "", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LABEL, CCONDITION::Z, COPERAND_TYPE::CONSTANT, "_str_add_s2", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::MEMORY, "[work_buf]", COPERAND_TYPE::REGISTER, "A" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::POP, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "free_string", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::POP, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "free_string", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "A", COPERAND_TYPE::REGISTER, "[work_buf]" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "allocate_string", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::PUSH, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "DE", COPERAND_TYPE::NONE, "work_buf" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::EX, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "DE", COPERAND_TYPE::REGISTER, "HL" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "C", COPERAND_TYPE::REGISTER, "[HL]" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "B", COPERAND_TYPE::CONSTANT, "0" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::INC, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "BC", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LDIR, CCONDITION::NONE, COPERAND_TYPE::NONE, "", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::POP, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::RET, CCONDITION::NONE, COPERAND_TYPE::NONE, "", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LABEL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "_str_add_error", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "E", COPERAND_TYPE::NONE, "15" );			//	String too long
	p_info->assembler_list.subroutines.push_back( asm_line );
	asm_line.set( CMNEMONIC_TYPE::JP, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "bios_errhand", COPERAND_TYPE::NONE, "" );
	p_info->assembler_list.subroutines.push_back( asm_line );
}
