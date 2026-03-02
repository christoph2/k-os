k_os – Ein OSEK/VDX-kompatibles Echtzeitbetriebssystem
=====================================================

Kurzüberblick
-------------
- OSEK/VDX-konforme RTOS-Implementierung (Tasks, Events, Ressourcen, Alarme/Counter, Schedule-Tables, Hooks, Fehlercodes, Interrupt-Management).
- Deterministischer O(1)-Scheduler mit Null-Takt Jitter bei Kontextwechseln (plattformabhängig). 
- Geringe Hardware-Abhängigkeiten, dadurch leicht auf neue MCUs/Plattformen portierbar.
- Niedrige Interrupt-Latenz; präemptives Scheduling, alle OSEK-Konformitätsklassen vorgesehen.
- Kompakte, hoch konfigurierbare Implementierung, auf Determinismus optimiert.
- Optional: OSEK-COM (Nachrichtenkommunikation) und ergänzende Werkzeuge für OIL/Code-Generierung.

Was ist OSEK/VDX?
------------------
OSEK/VDX ist ein in der Automobilindustrie entstandener Standard für Echtzeitbetriebssysteme und Kommunikations-Stacks. Kernelemente sind:
- Aufgabenmodell (Basis/erweiterte Tasks) mit Prioritäten und klar definierten Zuständen.
- Ereignisse (Events) für erweiterte Tasks.
- Ressourcen (Priority Ceiling Protocol) zur Vermeidung von Prioritätsinversionen.
- Alarme & Counter, Schedule Tables für zeitgesteuerte Aktivitäten.
- Service-Hooks (StartupHook, ShutdownHook, ErrorHook, PreTaskHook, PostTaskHook).
- Konfigurationsgetriebener Ansatz über OIL-Dateien (OSEK Implementation Language).

Projektstruktur (Auswahl)
-------------------------
- inc/ … Öffentliche Header (OS-API, Typen, Konfigurationseinschalter).
- src/ … OS-Implementierung (Scheduler, Hooks, Fehlerbehandlung, COM-IF u.a.).
- port/ … Hardware/Plattform-Ports (z. B. cpu12, msp430, stm32, windows/pthreads/i386 msvc).
- samples/ … Beispielanwendungen (u. a. OSEKBumser, first, Slick).
- k-dk/ … „Kernel-Development Kit“: Common Types, Compiler/Platform-Abstraktionen, Tools.
- kosek/ … OIL/OSEK-Tooling, Definitionen, Code-Generator (py2/py3).
- tools/ und pySART/ … Zusätzliche Skripte und Hilfsprogramme.

Wichtige Module und Dateien
---------------------------
- inc/Os_Types.h: Zentrale OSEK/OS-Typen inkl. StatusType und Fehlercodes (E_OK, E_OS_…).
- src/Os_Error.c: Aufruf der ErrorHook/COMErrorHook inkl. Interruptschutz.
- src/Com_If.c: OSEK-COM Interface/Dispatcher (falls COM aktiviert ist).
- port/<arch>/*: Portierungsschicht, Interrupt-Enable/Disable, Kontextwechsel, Timer usw.
- k-dk/inc/kdk/common/Std_Types.h: AUTOSAR-nahe Standardtypen (Std_ReturnType etc.).

Feature-Überblick
-----------------
- Tasks: Basis- und erweiterte Tasks, Aktivierung per API und/oder Autostart (AppMode).
- Ereignisse: SetEvent/WaitEvent/ClearEvent für Synchronisation erweiterter Tasks.
- Ressourcen: GetResource/ReleaseResource mit PCP zur Deadlock-/Inversions-Vermeidung.
- Alarme & Counter: Start/Stop/Increment für zyklische/relative/absolute Zeitsteuerung.
- Schedule Tables: Zeitgesteuerte Sequenzen (abhängig von Konfiguration/Port).
- Hooks: StartupHook, ShutdownHook, ErrorHook, PreTaskHook, PostTaskHook.
- Interrupt-Management: Disable/Enable All/OS Interrupts auf Port-Ebene.
- OSEK-COM (optional): Nachrichtenobjekte, Benachrichtigungsmechanismen (SetEvent etc.).

Konformitätsklassen
-------------------
k_os ist so ausgelegt, dass alle OSEK-Konformitätsklassen unterstützt werden können (BCCA, BCCB, ECC1, ECC2 etc.). Welche Features aktiv sind, steuern die Konfigurationsschalter sowie die generierte Konfiguration aus OIL.

Build und Quickstart (CLion/CMake)
----------------------------------
Voraussetzungen:
- Windows + CLion mit CMake-Toolchain.
- In diesem Repository ist ein CMake-Ziel vorhanden: k_os (Executable).

Schnellstart (mit bereitgestelltem Profil „Debug“):
- Zielverzeichnis: C:\csprojects\k-os\cmake-build-debug
- Build-Befehl (aus CLion bzw. via Konsole):
  cmake --build C:\csprojects\k-os\cmake-build-debug --target k_os

Hinweise:
- In manchen Konfigurationen ist OSEK-COM aktiviert, es fehlen jedoch Beispiel-Implementierungen (z. B. src/Com_Ext.c). Falls der Build wegen einer fehlenden Quelldatei fehlschlägt, liegt dies typischerweise an optionalen Modulen (COM/Erweiterungen). In diesem Fall:
  - Entweder die betreffenden Module/Schalter in der Konfiguration (CMake oder Os_Cfg/Feature-Schalter) deaktivieren,
  - oder die fehlenden Dateien/Port-spezifischen Erweiterungen bereitstellen.
- Das Ziel k_os ist primär zur Demonstration/Tests gedacht. Für konkrete Boards/Samples nutzen Sie die Projekte unter samples/.

Beispiele ausführen
-------------------
- samples/OSEKBumser, samples/first, samples/Slick etc. enthalten lauffähige Konfigurationen.
- Typischer Ablauf:
  1) Passenden Port/Board wählen (port/<arch>),
  2) OIL/Config generieren (siehe „Konfiguration & OIL“),
  3) Build durchführen, Binary flashen/ausführen.

Konfiguration & OIL
-------------------
- OSEK-OS wird über OIL-Dateien beschrieben (k_os/OIL, kosek/*, config/oil).
- Der Ordner kosek enthält Definitionen/Generatoren (Python) zur Ableitung von C-Headern/-Quellen aus OIL.
- Typische Schritte:
  - OIL-Modell erstellen oder anpassen (Tasks, Events, Ressourcen, Alarme, AppModes).
  - Generator aus kosek/py3 bzw. py2 benutzen, um Os_Cfg.h/Os_Cfg.c (und ggf. Com-Cfg) zu erzeugen.
  - Generierte Dateien in das Build integrieren (siehe samples/ für Referenzstruktur).

API-Kern (Auszug)
-----------------
- Startup/Shutdown: StartOS(AppMode), ShutdownOS(Status).
- Tasks: ActivateTask, TerminateTask, ChainTask, GetTaskID, GetTaskState.
- Events (erweiterte Tasks): SetEvent, ClearEvent, GetEvent, WaitEvent.
- Ressourcen: GetResource, ReleaseResource.
- Alarme/Counter: GetAlarmBase, GetAlarm, SetRelAlarm, SetAbsAlarm, CancelAlarm, IncrementCounter.
- Interrupts: DisableAllInterrupts, EnableAllInterrupts, SuspendAllInterrupts, ResumeAllInterrupts, Disable/EnableOSInterrupts.
- Hooks: ErrorHook(StatusType), StartupHook(), ShutdownHook(StatusType), PreTaskHook(), PostTaskHook().

Fehlerbehandlung
----------------
- StatusType-Fehlercodes gemäß OSEK/VDX (E_OK, E_OS_* …) in inc/Os_Types.h.
- ErrorHook/COMErrorHook werden innerhalb kritischer Abschnitte mit gesichertem Call-Level aufgerufen (siehe src/Os_Error.c).

Speicherabbild/Memory Mapping
-----------------------------
- Optionales Memory-Mapping via MemMap.h (KOS_MEMORY_MAPPING). Sektionen: OSEK_OS_CODE, OSEK_COM_CODE, VAR-Sektionen etc.
- Diese Mechanik erlaubt Port-spezifische Platzierung (z. B. in bestimmte Speicherbereiche/Segmente).

Portierungen
------------
- port/cpu12 – HCS12/HC12-Derivate.
- port/msp430 – TI MSP430.
- port/stm32 – ARM Cortex-M (Header + Port-Glue).
- port/pthreads – POSIX/PThreads (für Host-Simulation/Test).
- port/windows und port/i386/msvc – Windows/Win32-Hosts, Tools & IPC-Mockups.

Werkzeuge & Generatoren
-----------------------
- k-dk: Typendefinitionen (Compiler.h, Platform_Types.h, Std_Types.h), Makros und Utilities.
- kosek: OIL- und Implementierungs-Definitionen, Python-Generatoren (py2/py3).
- pySART: Zusätzliche Scripting-Unterstützung.

Kompatibilität und Namensräume
------------------------------
- Das Projekt orientiert sich an AUTOSAR-nahen Basistypen (Std_Types), nutzt jedoch eigene OS-Typen (StatusType etc.).
- Achtung bei Symbolkollisionen (z. B. E_OK/E_NOT_OK). In neueren Anpassungen wurden Standardtypen gesäubert, um doppelte Definitionen zu vermeiden. Module, die E_OK benötigen, sollen die passenden OS-Header einbinden oder eigene, konfliktfreie Konstanten verwenden.

Troubleshooting
---------------
- Build bricht mit fehlenden Quellen ab (z. B. src/Com_Ext.c):
  - COM in der Konfiguration deaktivieren oder Dummy/Port-spezifische Implementierung ergänzen.
- Undefinierte Symbole/Mehrfachdefinitionen:
  - Prüfen, ob Std_Types.h und Os_Types.h korrekt eingebunden sind und keine Makro-Redefinitionen stattfinden.
- Host-Ports (pthreads/windows) dienen primär zu Test/Simulation; Timing kann von Ziel-HW abweichen.

Lizenz
------
- GPLv2 (FLOSS-Exception). Details siehe COPYING und FLOSS-EXCEPTION.txt.

Autoren
-------
- Christoph Schueler (und Mitwirkende). Siehe AUTHORS.

Beitragen
---------
- Issues/Pull Requests willkommen. Bitte:
  - Konkrete Zielplattform/Port angeben,
  - Reproduzierbare OIL/Configs und Minimalbeispiele beilegen,
  - Stil und Struktur der vorhandenen Module beachten.

Weiterführende Hinweise (OSEK/VDX)
----------------------------------
- OSEK/VDX Spezifikation (öffentliche Quellen/Spiegel nutzen).
- Literatur zu Echtzeitplanung (Fixed Priority Scheduling, PCP, Reaktionszeiten).
- AUTOSAR OS Spezifikationen für moderne Erweiterungen (Schutz-/Zeitüberwachung, Service-IDs etc.).

Changelog (Auszug)
------------------
- Bereinigung von Symbolkollisionen bei E_OK/E_NOT_OK in Basistypen/Port-Code.
- Hinweis zu optionalen COM-Quellen im Build ergänzt.
