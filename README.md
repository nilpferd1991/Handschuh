Kommunikation über Endpoint 0

	- Typischer Kommunikationskanal
	- Jede Nachricht besteht aus einem Setup-Block (8 bytes) und einem Daten-Block (jegliche Länge)
	- Setup-Block: Typ (Richtung, Anlegestelle, Codestelle -> sollte Vendor sein, damit es im eigenen Code landet, nicht im Treiber), Requestnummer, Wert und Index (können zur Erweiterung der Requestnummer genutzt werden), Datenblocklänge
	
	1. Szenario: Daten an den Controller senden
		Entweder direkt in usbFunctionSetup
		Oder über usbFunctionWrite(uchar *data, uchar len) und die Länge steht in rq->wLength.word
		
	2. Szenario: Daten vom Controller empfangen (interessant!)
		Entweder direkt in usbFunctionSetup durch setzen von usbMsgPtr und return Länge
		Oder über usbFunctionRead(uchar *data, uchar len) und die angeforderte Länge steht in rq->wLength.word
		
	


USB-Kabel: 
    Grün D+
    Weiß D-
    Rot  Vcc
    Schwarz Gnd
