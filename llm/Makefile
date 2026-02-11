

all : _qtype.tex \
	_jtype.tex \
	_qdf_rec_type.tex  \
	qdf_definition.pdf 


_qtype.tex : qtype.h
	grep -v "^#" qtype.h > _qtype.tex

_jtype.tex : jtype.h
	grep -v "^#" jtype.h > _jtype.tex

_qdf_rec_type.tex : qdf_rec_type.h
	grep -v "^#" qdf_rec_type.h > _qdf_rec_type.tex

qdf_definition.pdf : qdf_definition.tex
	pdflatex qdf_definition.tex
	pdflatex qdf_definition.tex
