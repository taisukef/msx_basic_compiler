// --------------------------------------------------------------------
//	Expression
// ====================================================================
//	2023/July/29th	t.hara
// --------------------------------------------------------------------
#include <string>
#include <vector>
#include "../compile_info.h"

#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

// --------------------------------------------------------------------
class CEXPRESSION_NODE {
public:
	// ----------------------------------------------------------------
	//	この式の型
	CEXPRESSION_TYPE type;

	// ----------------------------------------------------------------
	//	この式は定数か。定数であれば、s_value にその値。
	bool is_constant;
	std::string s_value;

	// ----------------------------------------------------------------
	//	この式は変数か。
	bool is_variable;

	// ----------------------------------------------------------------
	//	コンストラクタ
	CEXPRESSION_NODE() {
		this->type = CEXPRESSION_TYPE::UNKNOWN;
		this->is_constant = false;
		this->is_variable = false;
	}

	// ----------------------------------------------------------------
	//	解放
	virtual void release( void ) {
	}

	// ----------------------------------------------------------------
	//	デストラクタ
	virtual ~CEXPRESSION_NODE() {
		this->release();
	}

	// ----------------------------------------------------------------
	//	コンパイル時の型変換処理
	static void convert_type( CCOMPILE_INFO *p_info, CEXPRESSION_TYPE target, CEXPRESSION_TYPE current );

	// ----------------------------------------------------------------
	//	演算子のオペランドの型揃え(2項演算子用)
	void type_adjust_2op( CCOMPILE_INFO *p_info, CEXPRESSION_NODE *p_left, CEXPRESSION_NODE *p_right );

	// ----------------------------------------------------------------
	//	コンパイル処理
	virtual void compile( CCOMPILE_INFO *p_info ) = 0;

	// ----------------------------------------------------------------
	//	演算式ツリーの中で事前に演算可能なモノは演算してしまう
	virtual CEXPRESSION_NODE *optimization( CCOMPILE_INFO *p_info ) = 0;
};

// --------------------------------------------------------------------
class CEXPRESSION {
private:
	// ----------------------------------------------------------------
	//	演算式ツリーの実体
	CEXPRESSION_NODE *p_top_node;

	// ----------------------------------------------------------------
	//	次が指定の単語で無ければ指定のエラーにする
	bool check_word( CCOMPILE_INFO *p_info, std::string s, int error_id = CERROR_ID::SYNTAX_ERROR );

	// ----------------------------------------------------------------
	//	演算式ツリーの中で事前に演算可能なモノは演算してしまう
	void optimization( void );

	// ----------------------------------------------------------------
	//	演算子のノード生成処理
	CEXPRESSION_NODE *makeup_node_term( CCOMPILE_INFO *p_info );						//	関数, FN関数, ( ) 括弧
	CEXPRESSION_NODE *makeup_node_operator_power( CCOMPILE_INFO *p_info );				//	^ 累乗
	CEXPRESSION_NODE *makeup_node_operator_minus_plus( CCOMPILE_INFO *p_info );			//	- + 符号
	CEXPRESSION_NODE *makeup_node_operator_mul_div( CCOMPILE_INFO *p_info );			//	* / 乗算、除算
	CEXPRESSION_NODE *makeup_node_operator_intdiv( CCOMPILE_INFO *p_info );				//	￥ 整数除算
	CEXPRESSION_NODE *makeup_node_operator_mod( CCOMPILE_INFO *p_info );				//	MOD 余り
	CEXPRESSION_NODE *makeup_node_operator_add_sub( CCOMPILE_INFO *p_info );			//	+ - 加減算
	CEXPRESSION_NODE *makeup_node_operator_compare( CCOMPILE_INFO *p_info );			//	= <> >< < <= =< > >= => 比較
	CEXPRESSION_NODE *makeup_node_operator_not( CCOMPILE_INFO *p_info );				//	NOT 反転
	CEXPRESSION_NODE *makeup_node_operator_and( CCOMPILE_INFO *p_info );				//	AND 論理積
	CEXPRESSION_NODE *makeup_node_operator_or( CCOMPILE_INFO *p_info );					//	OR 論理和
	CEXPRESSION_NODE *makeup_node_operator_xor( CCOMPILE_INFO *p_info );				//	XOR 排他的論理和
	CEXPRESSION_NODE *makeup_node_operator_imp( CCOMPILE_INFO *p_info );				//	IMP 包含
	CEXPRESSION_NODE *makeup_node_operator_eqv( CCOMPILE_INFO *p_info );				//	EQV 同値

public:
	// ----------------------------------------------------------------
	//	コンストラクタ
	CEXPRESSION() {
		this->p_top_node = nullptr;
	}

	// ----------------------------------------------------------------
	//	デストラクタ
	~CEXPRESSION() {
		this->release();
	}

	// ----------------------------------------------------------------
	//	解放
	void release( void ) {
		if( this->p_top_node != nullptr ) {
			delete (this->p_top_node);
			this->p_top_node = nullptr;
		}
	}

	// ----------------------------------------------------------------
	//	ソースコードを解釈して、演算式ツリーを形成する
	void makeup_node( CCOMPILE_INFO *p_info );

	// ----------------------------------------------------------------
	//	演算式ツリーからアセンブリコードを生成する
	//	式が省略されていた場合は、false を返す
	bool compile( CCOMPILE_INFO *p_info, CEXPRESSION_TYPE target = CEXPRESSION_TYPE::INTEGER );

	// ----------------------------------------------------------------
	CEXPRESSION_NODE *get_top_node( void ) {
		return this->p_top_node;
	}

	// ----------------------------------------------------------------
	//	演算結果の型を返す
	CEXPRESSION_TYPE get_type( void ) const {
		if( this->p_top_node == nullptr ) {
			return CEXPRESSION_TYPE::UNKNOWN;
		}
		return this->p_top_node->type;
	}

	// ----------------------------------------------------------------
	void convert_type( CCOMPILE_INFO *p_info, CEXPRESSION_TYPE target ) {
		this->p_top_node->convert_type( p_info, target, this->get_type() );
	}

	// ----------------------------------------------------------------
	void convert_type( CCOMPILE_INFO *p_info, CEXPRESSION_TYPE target, CEXPRESSION_TYPE current ) {
		this->p_top_node->convert_type( p_info, target, current );
	}
};

#endif
