del /q ".\Release\*"
FOR /D %%p IN (".\Release\*.*") DO rmdir "%%p" /s /q

del /q ".\Debug\*"
FOR /D %%p IN (".\Debug\*.*") DO rmdir "%%p" /s /q

del /q ".\MeshViewer.VC.db"
del /q ".\MeshViewer.sdf"
rmdir Debug
rmdir Release

::echo delete successed!
::@pause