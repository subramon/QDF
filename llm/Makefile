
all : _qtype.tex \
	_jtype.tex \
	_q2c.tex \
	_qdf_rec_type.tex  \
	qdf_definition.pdf 


_qtype.tex : qtype.h
	grep -v "^#" qtype.h > _qtype.tex

_q2c.tex : q2c.lua tbl_Lua_to_LaTeX.lua 
	 lua tbl_Lua_to_LaTeX.lua q2c.lua "q2c" "Qtypes to C Types" > _q2c.tex


_jtype.tex : jtype.h
	grep -v "^#" jtype.h > _jtype.tex

_qdf_rec_type.tex : qdf_rec_type.h
	grep -v "^#" qdf_rec_type.h > _qdf_rec_type.tex

qdf_definition.pdf : qdf_definition.tex \
	helpers.tex \
	coalesce.tex \
	_qtype.tex _jtype.tex _q2c.tex
	pdflatex qdf_definition.tex
	pdflatex qdf_definition.tex

clean : 
	rm -f  \
	_qtype.tex \
	_jtype.tex \
	_q2c.tex \
	_qdf_rec_type.tex  \
	qdf_definition.pdf 
