// --------------------------------------------------------------------
//	MSX-BASIC compiler
// ====================================================================
//	2023/July/20th  t.hara 
// --------------------------------------------------------------------

#include <vector>
#include <cstdio>
#include "assembler_line.h"

#ifndef __ASSEMBLER_LIST_H__
#define __ASSEMBLER_LIST_H__

class CASSEMBLER_LIST {
private:
	std::vector< std::string >			label_list;
	std::vector< std::string >			subrouines_list;

	bool save_sub( FILE *p_file, std::vector< CASSEMBLER_LINE > *p_list );

public:
	std::vector< CASSEMBLER_LINE >		header;
	std::vector< CASSEMBLER_LINE >		define_labels;
	std::vector< CASSEMBLER_LINE >		body;
	std::vector< CASSEMBLER_LINE >		subroutines;
	std::vector< CASSEMBLER_LINE >		datas;
	std::vector< CASSEMBLER_LINE >		const_single_area;
	std::vector< CASSEMBLER_LINE >		const_double_area;
	std::vector< CASSEMBLER_LINE >		const_string_area;
	std::vector< CASSEMBLER_LINE >		variables_area;
	std::vector< CASSEMBLER_LINE >		footer;

	std::vector<int> data_lines;

	void add_label( const std::string s_name, const std::string s_value );
	bool add_subroutines( const std::string s_name );
	void add_data( int line_no, const std::string s_value, class CCONSTANT_INFO *p_constants );
	void push_hl( CEXPRESSION_TYPE type );

	bool save( const std::string s_file_name );

	bool is_registered_subroutine( std::string s_search_name );
	void activate_push_single_real_hl( void );
	void activate_push_double_real_hl( void );
	void activate_pop_single_real_arg( void );
	void activate_pop_single_real_dac( void );
	void activate_pop_double_real_arg( void );
	void activate_pop_double_real_dac( void );
	void activate_ld_dac_single_real( void );
	void activate_ld_dac_double_real( void );
	void activate_ld_arg_single_real( void );
	void activate_ld_arg_double_real( void );
	void activate_ld_de_single_real( void );
	void activate_ld_de_double_real( void );
	void activate_puts( void );
	void activate_allocate_string( void );
	void activate_free_string( void );
	void activate_free_array( void );
	void activate_free_sarray( void );
	void activate_copy_string( void );
	void activate_get_writeable_string( void );
	void activate_allocate_heap( void );
	void activate_free_heap( void );
	void activate_init_string_array( void );
	void activate_str( void );
	void activate_string( void );
	void activate_convert_to_integer( void );
	void activate_convert_to_integer_from_sngle_real( class CCONSTANT_INFO *p_constants );
	void activate_convert_to_integer_from_double_real( class CCONSTANT_INFO *p_constants );
	void activate_single_real_is_zero( void );
	void activate_double_real_is_zero( void );
	void activate_check_array( void );
	void activate_check_sarray( class CCONSTANT_INFO *p_constants );
	void activate_calc_array_top( void );
	void activate_sub_input( void );
	void activate_bload_r( void );
	void activate_bload( void );
	void activate_paint( void );
	void activate_read_string( void );
	void activate_read_integer( void );
	void activate_read_single( void );
	void activate_read_double( void );
	void activate_val( void );
	void activate_circle( class CCOMPILE_INFO *p_info );
	void activate_close( void );
	void activate_all_close( void );
	void activate_maxfiles( void );
	void activate_init_files( void );
	void activate_open_for_input( void );
	void activate_open_for_output( void );
	void activate_open_for_append( void );
	void activate_open_for_none( void );
	void activate_open_sub( void );
	void activate_file_number( void );
	void activate_put_integer( class CCOMPILE_INFO *p_info );
	void activate_put_single_real( class CCOMPILE_INFO *p_info );
	void activate_put_double_real( class CCOMPILE_INFO *p_info );
	void activate_put_string( class CCOMPILE_INFO *p_info );
	void activate_field( void );
	void activate_space( void );
	void activate_get( void );
};

#endif
