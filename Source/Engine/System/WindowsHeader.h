#pragma once

#define NOCOMM
#define WIN32_LEAN_AND_MEAN
#define OEMRESOURCE

/*
// Modificare le seguenti definizioni se è necessario creare una piattaforma prima di quelle specificate di seguito.
// Fare riferimento a MSDN per informazioni aggiornate sui valori corrispondenti per le differenti piattaforme.
#ifndef WINVER				// Consente l'uso delle caratteristiche specifiche di Windows XP o versioni successive.
#define WINVER 0x0501		// Modificarlo con il valore appropriato per altre versioni di Windows.
#endif

#ifndef _WIN32_WINNT		// Consente l'uso delle caratteristiche specifiche di Windows XP o versioni successive.                   
#define _WIN32_WINNT 0x0501	// Modificarlo con il valore appropriato per altre versioni di Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Consente l'uso delle caratteristiche specifiche di Windows 98 o versioni successive.
#define _WIN32_WINDOWS 0x0410 // Modificarlo con il valore appropriato per Windows Me o versioni successive.
#endif

#ifndef _WIN32_IE			// Consente l'uso delle caratteristiche specifiche di IE 6.0 o versioni successive.
#define _WIN32_IE 0x0600	// Modificarlo con il valore appropriato per altre versioni di IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Escludere gli elementi utilizzati di rado dalle intestazioni di Windows
*/
#define WINVER			_WIN32_WINNT_WINXP
#define _WIN32_WINNT	_WIN32_WINNT_WINXP
#define NTDDI_VERSION	NTDDI_WINXP
#define _WIN32_IE		_WIN32_IE_IE60


#include <Windows.h>
#include <Ws2tcpip.h>


#undef min
#undef max