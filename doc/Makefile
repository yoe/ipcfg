TEXFILES=$(wildcard *.tex)
BASES=$(basename $(TEXFILES))
DVIFILES=$(addsuffix .dvi, $(BASES))
PDFFILES=$(addsuffix .pdf, $(BASES))
PSPDFFILES=$(addsuffix .ps.pdf, $(BASES))
PDFPSFILES=$(addsuffix .pdf.ps, $(BASES))
G3FILES=$(addsuffix .g3, $(BASES))
AUXFILES=$(addsuffix .aux, $(BASES))
LOGFILES=$(addsuffix .log, $(BASES))
TOCFILES=$(addsuffix .toc, $(BASES))
PSFILES=$(addsuffix .ps, $(BASES))
PRINTS=$(addprefix print-, $(BASES))
WORKFILES=$(wildcard *~) $(wildcard *.bak)
RTFFILES=$(addsuffix .rtf, $(BASES))
HTMLFILES=$(addsuffix .html, $(BASES))
DJFILES=$(addsuffix .dj, $(BASES))
PCLFILES=$(addsuffix .pcl, $(BASES))
IDXFILES=$(addsuffix .idx, $(BASES))
ILGFILES=$(addsuffix .ilg, $(BASES))
INDFILES=$(addsuffix .ind, $(BASES))
SNMFILES=$(addsuffix .snm, $(BASES))
NAVFILES=$(addsuffix .nav, $(BASES))
OUTFILES=$(addsuffix .out, $(BASES))
CRUFTFILES=$(AUXFILES) $(LOGFILES) $(WORKFILES) $(TOCFILES) $(IDXFILES) $(INDFILES) $(ILGFILES) $(OUTFILES) $(SNMFILES) $(NAVFILES)
OUTPUTFILES=$(PSFILES) $(PDFFILES) $(DVIFILES) $(HTMLFILES) $(RTFFILES) $(DJFILES) $(PCLFILES) $(PSPDFFILES) $(G3FILES) $(PDFPSFILES)
DVIFLAGS=
PDFFLAGS=
PSFLAGS=
all: alldvis allpdfs allpss
alldvis: $(DVIFILES)
allpdfs: $(PDFFILES)
allpss: $(PSFILES)
allprints: $(PRINTS)
allrtfs: $(RTFFILES)
allpspdfs: $(PSPDFFILES)
allpdfpss: $(PDFPSFILES)
allpsspecials: allpspdfs allpdfpss
alldviview: $(addsuffix .view, $(DVIFILES))
allpdfview: $(addsuffix .view, $(PDFFILES))
svnsync:
	rm -f foo
	for i in $(CRUFTFILES) $(OUTPUTFILES); do echo $$i >> foo; done
	svn ps svn:ignore -F foo .
	rm -f foo
%.dvi: %.tex
	latex $< && latex $<
	-makeindex $(basename $<).idx
	latex $(DVIFLAGS) $<
%.aux: %.tex
	latex $<
%.pdf: %.tex
	pdflatex $< && pdflatex $< 
	-makeindex $(basename $<).idx
	pdflatex $(PDFFLAGS) $< 
%.ps.pdf: %.ps
	ps2pdf $< $@
%.pdf.ps: %.pdf
	pdf2ps $< $@
%.ps: %.dvi
	dvips $(PSFLAGS) $< -o $@
%.dj: %.ps deskjet.ppd
	foomatic-rip --ppd $(<D)/deskjet.ppd $< > $@
%.pcl: %.ps
	gs -sDEVICE=ljet3 -sOutputFile=$@ -dNOPAUSE -dBATCH $<
%.g3: %.ps
	gs -sDEVICE=dfaxlow -sOutputFile=$@ -dNOPAUSE -dBATCH $<
%.rtf: %.tex
	latex2rtf -o $@ $<
%.print: %.ps
	lpr $<
%.dvi.view: %.dvi
	xdvi $<
%.pdf.view: %.pdf
	xpdf $<
%.ps.view: %.ps
	gv $<
%.pdf.beam: %.pdf
	xpdf -fullscreen $<
all-%: %.tex %.ps %.pdf %.dj %.rtf %.dvi
clean: cruftclean
	rm -f $(OUTPUTFILES)
cruftclean:
	rm -f $(CRUFTFILES)
.PHONY: alldvis allpdfs allpss allprints all clean cruftclean
