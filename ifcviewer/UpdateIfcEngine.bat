
echo Check IFC engine includes
if .%RDF_IFC_ENGINE_INCLUDE%.==.. goto IncUpdated
echo echo Update IFC engine includes 
xcopy %RDF_IFC_ENGINE_INCLUDE% IfcEngine\include /F /Y
:IncUpdated

echo Check IFC engine binaries
if .%RDF_IFC_ENGINE_LIB%.==.. goto LibUpdated
echo Update IFC engine binaries
copy %RDF_IFC_ENGINE_LIB%Debug\ifcengine.lib ifcengine\lib\x64 /Y
if .%1.==.. goto LibUpdated
copy %RDF_IFC_ENGINE_LIB%Debug\ifcengine.dll %1Debug /Y
REM copy %RDF_IFC_ENGINE_LIB%Release\ifcengine.dll %1Release /Y
:LibUpdated
