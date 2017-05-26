build:
	latexmk -pdf

notes:
	latexmk -pdf notes/notes

cards:
	latexmk -pdf cards/cards

watch-notes:
	latexmk -pdf -pvc notes/notes

watch-cards:
	latexmk -pdf -pvc cards/cards

clean:
	latexmk -c