# Verslag project computer graphics & visual computing: Rainbow Road

## inleiding


## planning 
planning: trello als reference gebruiken?

## Camera

We hebben zowel een free roam camera als een POV camera geïmplementeerd. In freeroam kun je de camera bewegen met toestenbordinputs (w = vooruit, a = links, s = achteruit, d = rechts, spatie = omhoog, shift = omlaag). Muisbewegingen kunnen gebruikt worden om rond te kijken met de camera en er kan ook in- en uitgezoomed worden door te scrollen. Bij het bewegen van de camera worden yaw en pitch als Euler-hoeken bijgehouden. `updateCamraVector()` herberekent per frame de front-, right- en up-vectoren vanuit deze hoeken.

We kunnen POV modus activeren door op `Tab` te drukken. In POV modus roepen we `FollowTarget()` aan. De positie en de richting van de kart wordt dan uit de transformatiematrix gehaalt om de camerapositie en kijkrichting hierop aan te passen elke frame. Door opnieuw `Tab` in te drukken stoppen we met het volgen van de kart en blijft de camera achter. Met `SyncOrientationFromFront()` synchroniseren we yaw en pitch met de huidige front-vector, zodat de muisbesturing in de juiste richting wijst en de camera niet opeens in een andere richting springt.

## ControlsHandler

De ControlsHandler behandelt alle inputs. Hier wordt een referentie naar de camera en het venster bijgehouden om methods op aan te roepen die corresponderen met bepaalde key presses. Elke frame wordt via `processInput()` mogelijke keyboard inputs gelezen. Sommige key presses dienen als toggle, hiervoor gebruiken we 'was_pressed' variabelen om ervoor te zorgen dat één key press ook maar één keer triggert. Voorbeelden hiervan zijn de `tab` en `c` toggles voor de POV camera en de crosshair.

## Picking

Om picking makkelijker te maken voor de gebruiker hebben we een crosshair toggle geïmplementeerd. De crosshair is een post-processing effect dat over de framebuffer getekend wordt en altijd in het midden van het scherm gecentreerd staat. Deze crosshair kan zichtbaar gemaakt worden door op `c` te drukken. Voor picking uit te voeren klik je met de linkermuisknop. Dan wordt `getPickedLight()` aangeroepen. Hier worden de wereldcoördinaten van elke actieve ster geprojecteerd naar NDC-coördinaten. De ster die het minste afwijkt van het centrum (0,0) wordt gedeactiveerd. Hierbij is een drempelwaarde ingesteld. Als er geen enkele ster een kleinere afwijking heeft dan  0.08 gebeurt er niets. Sterren die gedeactiveerd zijn worden niet meer getekent in de scene. Door op `r` te drukken zullen alle sterren terug zichtbaar worden in de scene.