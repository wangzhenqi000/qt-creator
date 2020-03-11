Qt Creator 4.11.2
=================

Qt Creator version 4.11.2 contains bug fixes.

The most important changes are listed in this document. For a complete
list of changes, see the Git log for the Qt Creator sources that
you can check out from the public Git repository. For example:

    git clone git://code.qt.io/qt-creator/qt-creator.git
    git log --cherry-pick --pretty=oneline origin/v4.11.1..v4.11.2

Editing
-------

* Improved performance of highlighting (QTCREATORBUG-23281)
* Fixed that `Rewrap Paragraph` split on non-breaking space (QTCREATORBUG-23643)
* Fixed freeze with block selection (QTCREATORBUG-23622)
* Fixed high CPU usage after scrolling horizontally (QTCREATORBUG-23647)
* Fixed scroll position after splitting if text cursor is not visible
* Fixed position of markers in scrollbar for long documents (QTCREATORBUG-23660)

### Python

* Fixed warnings in files generated by Python file wizard

### Language Client

* Fixed issue with server restart after server crash (QTCREATORBUG-23648)

Projects
--------

* Fixed wrong default project for adding files via wizards (QTCREATORBUG-23603)

Platforms
---------

### macOS

* Fixed issues with notarization of binary package

Credits for these changes go to:
--------------------------------

André Pönitz  
Andy Shaw  
Christian Kandeler  
David Schulz  
Eike Ziller  
Leena Miettinen  
Orgad Shaneh  
Richard Weickelt  
Tobias Hunger  