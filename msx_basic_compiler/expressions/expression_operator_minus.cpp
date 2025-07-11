// --------------------------------------------------------------------
//	Expression
// ====================================================================
//	2023/July/29th	t.hara
// --------------------------------------------------------------------
#include <string>
#include <vector>
#include "expression_operator_minus.h"
#include "expression_term.h"

// --------------------------------------------------------------------
CEXPRESSION_NODE* CEXPRESSION_OPERATOR_MINUS::optimization( CCOMPILE_INFO *p_info ) {
	CEXPRESSION_NODE* p;

	if( this->p_right == nullptr ) {
		return nullptr;
	}
	p = this->p_right->optimization( p_info );
	if( p != nullptr ) {
		delete (this->p_right);
		this->p_right = p;
	}

	if( this->p_right->is_constant ) {
		//	項が両方とも定数の場合
		if( this->p_right->type != CEXPRESSION_TYPE::STRING ) {
			//	数値の場合
			CEXPRESSION_TERM *p_term  = new CEXPRESSION_TERM();
			p_term->set_double( -std::stod(p_right->s_value) );
			return p_term;
		}
	}
	return nullptr;
}

// --------------------------------------------------------------------
void CEXPRESSION_OPERATOR_MINUS::compile( CCOMPILE_INFO *p_info ) {
	CASSEMBLER_LINE asm_line;

	if( this->p_right == nullptr ) {
		return;
	}
	//	先に項を処理
	this->p_right->compile( p_info );

	//	この演算子の演算結果の型を決める
	if( this->p_right->type == CEXPRESSION_TYPE::STRING ) {
		p_info->errors.add( TYPE_MISMATCH, p_info->list.get_line_no() );
		return;
	}
	p_info->assembler_list.add_label( "work_valtyp", "0x0f663" );
	p_info->assembler_list.add_label( "work_dac", "0x0f7f6" );
	p_info->assembler_list.add_label( "bios_vmovfm", "0x02f08" );
	p_info->assembler_list.add_label( "bios_neg", "0x02e8d" );
	p_info->assembler_list.add_label( "bios_frcsng", "0x0303a" );
	if( this->p_right->type == CEXPRESSION_TYPE::INTEGER ) {	//	-32768 のときにバグるけど・・・それは仕様です
		asm_line.set( "EX", "", "DE", "HL" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( "LD", "", "HL", "0" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( "OR", "", "A", "A" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( "SBC", "", "HL", "DE" );
		p_info->assembler_list.body.push_back( asm_line );
		this->type = CEXPRESSION_TYPE::INTEGER;
	}
	else if( this->p_right->type == CEXPRESSION_TYPE::SINGLE_REAL ) {
		asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "A", COPERAND_TYPE::CONSTANT, "4" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::MEMORY, "[work_valtyp]", COPERAND_TYPE::MEMORY, "A" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "bios_vmovfm", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "bios_neg", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::CONSTANT, "work_dac" );
		p_info->assembler_list.body.push_back( asm_line );
		this->type = CEXPRESSION_TYPE::SINGLE_REAL;
	}
	else {
		asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "A", COPERAND_TYPE::CONSTANT, "8" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::MEMORY, "[work_valtyp]", COPERAND_TYPE::MEMORY, "A" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "bios_vmovfm", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::CALL, CCONDITION::NONE, COPERAND_TYPE::CONSTANT, "bios_neg", COPERAND_TYPE::NONE, "" );
		p_info->assembler_list.body.push_back( asm_line );
		asm_line.set( CMNEMONIC_TYPE::LD, CCONDITION::NONE, COPERAND_TYPE::REGISTER, "HL", COPERAND_TYPE::CONSTANT, "work_dac" );
		p_info->assembler_list.body.push_back( asm_line );
		this->type = CEXPRESSION_TYPE::DOUBLE_REAL;
	}
}
