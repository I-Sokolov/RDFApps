
echo Check IFC engine includes
if .%RDF_IFC_ENGINE_INCLUDE%.==.. goto IncUpdated
echo echo Update IFC engine includes 
xcopy %RDF_IFC_ENGINE_INCLUDE% IfcEngine\include /F /Y
xcopy %RDF_IFC_ENGINE_INCLUDE%..\ToolBox\ModelChecker.h IfcEngine\include /F /Y
:IncUpdated

echo Check IFC engine binaries
if .%RDF_IFC_ENGINE_LIB%.==.. goto LibUpdated
echo Update lib and dll
xcopy %RDF_IFC_ENGINE_LIB%Debug\ifcengine.lib ifcengine\lib\x64\ /F /Y
xcopy %RDF_IFC_ENGINE_LIB%%2\ifcengine.dll ifcengine\lib\x64\%2\ /F /Y
echo Copy dll to output
xcopy ifcengine\lib\x64\%2\ifcengine.dll %1 /F /Y
:LibUpdated
