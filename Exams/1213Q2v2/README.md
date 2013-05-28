# IDI. Prova de Laboratori. 2012-2013 2Q. Temps: 1h 15min

## Tingeu en compte el següent (altrament la nota serà un zero)

1. El code del vostre **programa principal** ha d'estar en un arxiu de nom
**examen.cpp**

2. **Copieu** els fitxers **legoman.obj** i **lelgoman.mtl** en el **mateix
directori** que el fitxer **examen.cpp** (els trobareu al directori
assig/idi/Model)

3. El **path per a carregar el model HA DE SER relatiu**.

4. Heu de **liurar** un fitxer **<vostreusuari>GL.tar.gz** amb **TOTS els
fitxers** que calen per a poder compilar i executar la vostra pràctica (**inclòs
els fitxers model.h, model.cpp, legoman.obj i legoman.mtl**)

5. Per a construir l'executable i corregir l'examen farem[^compilation_footnote]:

    g++ -o examen examen.cpp model.cpp -lglut -lGL -lGLU

6. En finalitxar, lliureu el fitxer a **https://examens.fib.upc.edu** 


## Enunciat de la prova

Cal que feu un programa que en començar la seva execució mostra:

- Un terra de les mateixes característiques que heu fet servir en les pràctiques
(polígon centrat en origen de coordenades de mides 10x10, de metall blau)

- Un cub *glut* d'aresta 4, amb cares paral·leles als plans de coordenades,
situat sobre el terra. El centre de la seva cara inferior ha d'estar en el punt
(0, 0, 0). El seu material ha de ser metall gris.

- Un *legoman* d'alçada 4, ubicat damunt de la cara superior del cub. centre de
la seva base al (0, 4, 0).


La càmera ha de ser axiometrica. Ha de permetre ceure tota l'escena sense
retallar en un *viewport* que ocupa tota la finestra gràfica.


NOMÉS cal que hi hagi ENCESA una llum d'escena de color blanc, ubicada sobre una
cantonada del terra, en la posició (5, 9, 5).


L'aplicació ha de tenir, a més a més, les següents funcionalitats[^code_footnote]:

1. Cada vegada que es prem la tecla "d" es traslladen 1 unitat, conjuntament, el
cub i el *legoman* en la direcció de l'eix X+ de l'escena; i cada vegada que es
prem "e" es desplaen 1 unitat en la direcció de l'eix X-.

2. Cal poder inspeccionar l'escena en mode absolut, posicionant la càmera
(arrosegant el ratolí) amb *gluLookAt* girant en torn al centre de l'escena.

3. Cal poder encendre i apagar el focus tot prement la tecla "f".

4. Cada vegada que es prem la tecla "s" la posició de la llum es modifica, i es
posiciona sobre el següent vèrtex del terra (en el sentit cíclic dels vèrtexs
del rectangle) a la mateixa alçada; a més, abans de tornar al vèrtex inicial, es
situarà -a la mateixa alçada- sobre el centre del terra.


Informació complementària:

    void glutSolidCube(GLdouble size);

"Render a solid cube. The cube is centered at the modeling coordinates origin
with sides of lenght size."

[^compilation_footnote]: Si teniu més fitxers *.cpp, cal que també lliureu el
Makefile que es requereix per a construir l'executable.

[^code_footnote]:Les funcionalitats 2, 3 i 4 les haurieu de tenir implementades
en els blocs 3 i 4.