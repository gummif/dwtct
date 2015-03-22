# make | grep -A 3 -iE '!|Warning|Error|failed|Success|dvips:'
OUT=definitions
SRC=$(OUT).tex

LATEX=latex
LATEXFLAGS=-interaction=nonstopmode
DVIPS=dvips
DVIPSFLAGS=
PDFLATEX=pdflatex
PDFLATEXFLAGS=-synctex=1 -interaction=nonstopmode
BIBTEX=bibtex
PS2PDF=ps2pdf
SCCSS=Success

.PHONE: all latex pdflatex doxygen ps2pdf clean

all: pdflatex doxygen

latex:
	@$(LATEX) $(LATEXFLAGS) $(SRC) && $(DVIPS) -q -o $(OUT).ps $(OUT).dvi && echo Success

pdflatex:
	$(PDFLATEX) $(PDFLATEXFLAGS) $(SRC)

doxygen:
	doxygen Doxyfile

ps2pdf:
	$(PS2PDF) $(OUT).ps

clean:
	rm -f $(OUT).log $(OUT).pdf $(OUT).dvi $(OUT).ps $(OUT).aux $(OUT).out $(OUT).synctex.gz
	rm -rf html
