
echo Check IFC engine includes
if .%RDF_IFC_ENGINE_INCLUDE%.==.. goto IncUpdated
echo echo Update IFC engine includes 
xcopy %RDF_IFC_ENGINE_INCLUDE% IfcEngine\include /F /Y
copy %RDF_IFC_ENGINE_INCLUDE%..\ToolBox\ModelChecker.h IfcEngine\include /Y
:IncUpdated

echo Check IFC engine binaries
if .%RDF_IFC_ENGINE_LIB%.==.. goto LibUpdated
echo Update ifcengine\lib\x64\ifcengine.lib
copy %RDF_IFC_ENGINE_LIB%Debug\ifcengine.lib ifcengine\lib\x64\ifcengine.lib /Y
echo Update ifcengine\lib\x64\%2\ifcengine.dll
copy %RDF_IFC_ENGINE_LIB%%2\ifcengine.dll ifcengine\lib\x64\%2\ifcengine.dll /Y
if .%1.==.. goto LibUpdated
echo Update %1%2\ifcengine.dll
copy ifcengine\lib\x64\%2\ifcengine.dll %1%2\ifcengine.dll /Y
:LibUpdated
