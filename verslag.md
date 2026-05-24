# Verslag project computer graphics & visual computing: Rainbow Road

## Inleiding

Voor ons project hebben we ons gebaseerd op Rainbow Road een map uit mario kart. 
We hebben dit geïmplementeerd door een track te maken van bezier curves en hier de rainbow road texture op te zetten en als object mario in een kart gekozen die over deze baan rijdt. ook hebben we lakitu die met een greenscreen voor de mario hovert. We hebben alle requirements geïmplementeerd en een youtube video gemaakt met uitleg over het project deze video kunt u [hier](https://youtu.be/cUjKAljiI6w) terug vinden.

## Controls

### Window

| Key | Actie |
|-----|-------|
| `esc` | window sluiten |

### Camera

| Key | Actie |
|-----|-------|
| `w` | Vooruit |
| `a` | Links |
| `s` | Achteruit |
| `d` | Rechts |
| `spatie` | Omhoog |
| `shift` | Omlaag |
| Muis bewegen | Rondkijken |
| Scroll | In-/uitzoomen |
| `Tab` | POV modus aan/uit |

### Post-processing (mode)

| Key | Actie |
|-----|-------|
| `g` | Gaussian blur |
| `l` | Laplaciaan |
| `n` | Geen effecten |
| `b` | Bloom |

Bij post-processing werken we met modes dus je verandert hier de mode mee, je kan niet twee effecten tegenlijkertijd aan hebben staan.

### Picking

| Key | Actie |
|-----|-------|
| `c` | Crosshair aan/uit |
| Linkermuisknop | Ster deactiveren |
| `r` | Alle sterren resetten |


## Planning

### Gerben Geurts

- Free-roam camera & `ControlsHandler`
  - 28 april: 2u
  - 30 april: 4u

- POV camera
  - 19 mei: 2u

- Crosshair & picking
  - 22 mei: 4u
  - 23 mei: 1u

- `CMakeLists.txt` voor windows support + windows testen
  - 23 mei: 2u

- Opnemen demovideo
  - 23 mei: 2u

### Maria Hullenkremer

- window class met de main loop en basic shaders (nog niet volledig)
  - 21 april: 3u

- `BezierCurve` klasse en `Track`
  - 21 april: 4u
  - 22 april: 6u

- Objecten laden met `Mesh`, `ObjLoader` etc.
  - 23 april: 8u
  - 24 april: 3u

- Arclength logica geïmplementeerd en objecten laten bewegen over track
  - 27 april: 4u

- Lakitu met zijn greenscreen (`ChromaObject`) en de `LightObject` class
  - 2 mei: 6u

- Eerste versie convolutie logica.
  - 16 april: 7u

- Finale versie convolutie met `addEffect` functie
  - 20 mei: 4u

- Visuele crosshair gemaakt (post processing) en de finale track soepeler gemaakt.
  - 21 mei: 2u

- Laplaciaan veranderd om het beeld te vergrijzen.
  - 22 mei: 1u

- Bug met de bloom gefixt en de code verschoond.
  - 23 mei: 3u

## Curve

Voor de bezier curve heb ik een klasse gemaakt `BezierCurve` deze heeft 4 controlepunten en dan een functie om de curve te tekenen aan de hand van forward differencing. Dit gebruiken we dan voor het maken van onze track die bestaat uit een heleboel curves aan elkaar verbonden om zo een baan te vormen. De `trackCurves.h` file bevat een lijst van alle curven waaruit de track bestaat deze lijst van curves (de controlepunten dus) is gegenereerd door AI, Dit vergt geen skill en is gewoon veel werk om zelf te doen om al die punten te bedenken en daar een vloeiende baan van te vormen. Deze lijst van curves wordt dan aan de `Track` class gegeven om zo een track te maken en tekenen wat simpelweg de baan van deze curves is en dan verbreed als lint om echt een baan te maken. De `drawcurves` heeft een parameter `steps` om te kunnen kiezen hoeveel steps je gebruikt bij de forward differencing we hebben dit een standaardwaarde gegeven van 500 voor een zeer soepele baan.

## Animatie

Om onze objecten consistent over de track te laten bewegen heeft de track een lijst met de booglengtes die berekend zijn in de `build` functie tijdens het maken van de track deze lijst wordt dan gebruikt door de `getTransformAtDistance` functie deze krijgt als input de distance travelled wat de tijd delta * de snelheid van het object is. hiermee zoeken we dan in de arclengths en interpoleren we en zetten het allemaal in een matrix die een object dan kan gebruiken als model.


## Modellen/Textures

Voor onze modellen hebben we een `ObjLoader` class gemaakt die twee functies bevat een om obj files te laden en dan een `meshdata` (enum) terug te geven die dan geldt als input voor het construeren van een `Mesh` en een functie om textures te laden. om obj files te laden gebruiken wij tiny object loader omdat je hier simpelweg de file moet toevoegen en het dan kunt gebruiken. en voor het laden van images voor de textures gebruiken wij stb image zoals in de opengl tutorial.

Als we de `meshdata` hebben geladen dan zetten we dit in de `Mesh` class deze bevat alle vertices en indexes, de texture en de buffers om het te kunnen tekenen en deze heeft dan ook een functie om de mesh te tekenen om zo de meshes in onze wereld te kunnen krijgen. Voor meshes in dit project hebben we alleen de Mario in zijn kart, de Lakitu en de grand stars voor lichten, deze heb ik beide een eigen klasse gegeven om alles wat bij elkaar hoort makkelijk te kunnen tekenen en de model matrix etc te kunnen doorgeven om over de track te kunnen bewegen.

## Visualisatie

Voor visualisatie van onze curves hebben we zoals eerder besproken een track gemaakt van deze curves dit verbreed om een lint te krijgen en hier zetten we tot slot dan een texture van rainbow road uit mario kart op. dit visualiseert duidelijk de curven waar de kart over beweegt. 

## Camera

We hebben zowel een free roam camera als een POV camera geïmplementeerd. In freeroam kun je de camera bewegen met toetsenbordinputs (`w` = vooruit, `a` = links, `s` = achteruit, `d` = rechts, `spatie` = omhoog, `shift` = omlaag). Muisbewegingen kunnen gebruikt worden om rond te kijken met de camera en er kan ook in- en uitgezoomed worden door te scrollen. Bij het bewegen van de camera worden yaw en pitch als Euler-hoeken bijgehouden. `updateCameraVector()` herberekent per frame de front-, right- en up-vectoren vanuit deze hoeken.

We kunnen POV modus activeren door op `Tab` te drukken. In POV modus roepen we `FollowTarget()` aan. De positie en de richting van de kart wordt dan uit de transformatiematrix gehaald om de camerapositie en kijkrichting hierop aan te passen elke frame. Door opnieuw `Tab` in te drukken stoppen we met het volgen van de kart en blijft de camera achter. Met `SyncOrientationFromFront()` synchroniseren we yaw en pitch met de huidige front-vector, zodat de muisbesturing in de juiste richting wijst en de camera niet opeens in een andere richting springt.


## Belichting

Voor de belichting hebben wij een lijst met verschillende locaties waarop lichten staan in de main gedefinieerd. Deze lijst van punten is ook door AI gegenereerd. Dit zijn ook gewoon coördinaten in een lijst voor de locatie van de lampen omdat we genoeg lichten wouden voor onze grote track hebben we dit dus door AI laten doen omdat het veel en onhandig werk is om lichten te plaatsen en verplaatsen om te kijken of ze goed in de wereld staan. Voor de lichten duidelijk te maken hebben we er een Grand star mesh over gedaan van mario deze heeft zijn eigen shader om te zorgen dat deze zelf niet door de lichtbron wordt beïnvloed.
Deze lijst van lichtbronnen wordt ook meegegeven aan de main shaders waardoor ze effect hebben op de baan, kart etc. dit is ook te zien in de scene. 

## Convolutie

Voor convolutie hebben we een klasse gemaakt `PostProcessEffect` die dan als parent geldt voor specifieke klassen van een bepaald effect zoals `GaussianBlur` en `LaplaciaanEffect` deze klasse implementeren dan elk de virtual method `addEffect` die een framebuffer krijgt en deze aanpast met hun eigen shaders om zo het effect in de framebuffer te krijgen. Voor de gaussian blur shader werk ik in verschillende passes om zo eerst horizontaal en dan verticaal te bluren omdat dit efficiënter is dan een hele grote grid. Ook de laplaciaan heb ik gesplitst in verschillende passes, omdat dit eigenlijk gewoon de som is van de tweede afgeleiden in x en y kan je deze apart berekenen en dan gewoon optellen dit is ook efficiënter. Om dan deze effecten toe te passen kun je `g` drukken voor gaussian blur, `l` voor de laplaciaan en `n` om terug geen effecten. Om deze effecten te kunnen doen gebruiken we een framebuffer en een screen-aligned quad. We tekenen alles naar de framebuffers waarop we dan makkelijk de effecten kunnen zetten en die tekenen we dan op de quad die een passthrough shader heeft om zo gewoon de framebuffer mogelijk met effect naar de window te kunnen tekenen.

## Post-processing

We hebben ook een bloom geïmplementeerd hiervoor hebben we een aparte buffer waar we de lichtbronnen nog eens in tekenen en deze FBO geven we dan een gaussian blur en dan blenden we deze fbo met de main fbo om zo een bloom effect te geven aan de lichten om dit te enablen druk je op `b`.

## Chroma-keying

Voor chroma-keying hebben we het wat anders gedaan dan in de opgaven stond zoals een overlay. We hebben hier een klasse `ChromaObject` gemaakt dit is een Quad in 3D space die volledig groen is en hier geven we dan een image aan mee. Deze class heeft een aparte shader die alle groene fragments vervangt door de image die is meegegeven. Ik had dit voor de convolutie gedaan waardoor ik dit dus anders heb geïmplementeerd dan bijvoorbeeld de gaussian blur ook is het bij ons zo dat je deze niet kan activeren maar dat die dus altijd aanstaat bij de chromaobjecten en ook is het zo dat we standaard gewoon een groene quad hebben waarop we het toepassen dus je kunt niet echt zien dat het chroma keying doet in plaats van simpelweg die image op de quad te zetten, maar het is wel effectief chromakeying in de shader.

## ControlsHandler

De `ControlsHandler` behandelt alle inputs. Hier wordt een referentie naar de camera en het venster bijgehouden om methods op aan te roepen die corresponderen met bepaalde key presses. Elke frame wordt via `processInput()` mogelijke keyboard inputs gelezen. Sommige key presses dienen als toggle, hiervoor gebruiken we `was_pressed` variabelen om ervoor te zorgen dat één key press ook maar één keer triggert. Voorbeelden hiervan zijn de `Tab` en `c` toggles voor de POV camera en de crosshair.

## Picking

Om picking makkelijker te maken voor de gebruiker hebben we een crosshair toggle geïmplementeerd. De crosshair is een post-processing effect dat over de framebuffer getekend wordt en altijd in het midden van het scherm gecentreerd staat. Deze crosshair kan zichtbaar gemaakt worden door op `c` te drukken. Voor picking uit te voeren klik je met de linkermuisknop. Dan wordt `getPickedLight()` aangeroepen. Hier worden de wereldcoördinaten van elke actieve ster geprojecteerd naar NDC-coördinaten. De ster die het minste afwijkt van het centrum (0,0) wordt gedeactiveerd. Hierbij is een drempelwaarde ingesteld. Als er geen enkele ster een kleinere afwijking heeft dan 0.08 gebeurt er niets. Sterren die gedeactiveerd zijn worden niet meer getekend in de scene. Door op `r` te drukken zullen alle sterren terug zichtbaar worden in de scene.

