#include "stdafx.h"
#include "unit.h"



bool	equalStr(wchar_t * txtI, wchar_t * txtII)
{
	int_t i = 0;
	if (txtI && txtII) {
		while (txtI[i]) {
			if (txtI[i] != txtII[i]) {
				return	false;
			}
			i++;
		}
		if (txtII[i]) {
			return	false;
		}
	}
	else if (txtI || txtII) {
		return	false;
	}

	return	true;
}

wchar_t	* copyStr(wchar_t * txt)
{
	if (txt) {
		int_t	i = 0;
		while  (txt[i]) { i++; }
		wchar_t	* rValue = new wchar_t[i+1];
		i = 0;
		while (txt[i]) {
			rValue[i] = txt[i];
			i++;
		}
		rValue[i] = 0;

		return	rValue;
	}
	else {
		return	0;
	}
}

STRUCT__PROPERTY	* CreateIfcProperty(int_t ifcInstance, wchar_t * name, wchar_t * description)
{
	STRUCT__PROPERTY	* ifcProperty = new STRUCT__PROPERTY;

	ifcProperty->structType = STRUCT_TYPE_PROPERTY;
	ifcProperty->ifcInstance = ifcInstance;
	ifcProperty->name = copyStr(name);
	ifcProperty->description = copyStr(description);

	ifcProperty->nominalValue = nullptr;
	ifcProperty->lengthValue = nullptr;
	ifcProperty->areaValue = nullptr;
	ifcProperty->volumeValue = nullptr;
	ifcProperty->countValue = nullptr;
	ifcProperty->weigthValue = nullptr;
	ifcProperty->timeValue = nullptr;
	ifcProperty->unitName = nullptr;

	ifcProperty->hTreeItem = 0;
	ifcProperty->nameBuffer = new wchar_t[512 + 1];
	ifcProperty->next = nullptr;

	return	ifcProperty;
}

STRUCT__PROPERTY__SET	* CreateIfcPropertySet(int_t ifcInstance, wchar_t * name, wchar_t * description)
{
	STRUCT__PROPERTY__SET	* ifcPropertySet = new STRUCT__PROPERTY__SET;

	ifcPropertySet->structType = STRUCT_TYPE_PROPERTY_SET;
	ifcPropertySet->ifcInstance = ifcInstance;
	ifcPropertySet->name = copyStr(name);
	ifcPropertySet->description = copyStr(description);

	ifcPropertySet->hTreeItem = 0;

	ifcPropertySet->nameBuffer = new wchar_t[512];

	ifcPropertySet->properties = 0;
	ifcPropertySet->next = 0;

	return	ifcPropertySet;
}

void	UnitAddUnitType(STRUCT__SIUNIT * unit, wchar_t * unitType)
{
	//
	//	unitType
	//
	if (equalStr(unitType, L".ABSORBEDDOSEUNIT.")) {
		unit->type = ABSORBEDDOSEUNIT;
		unit->unitType = L"ABSORBEDDOSEUNIT";
	}
	else if (equalStr(unitType, L".AREAUNIT.")) {
		unit->type = AREAUNIT;
		unit->unitType = L"AREAUNIT";
	}
	else if (equalStr(unitType, L".DOSEEQUIVALENTUNIT.")) {
		unit->type = DOSEEQUIVALENTUNIT;
		unit->unitType = L"DOSEEQUIVALENTUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCAPACITANCEUNIT.")) {
		unit->type = ELECTRICCAPACITANCEUNIT;
		unit->unitType = L"ELECTRICCAPACITANCEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCHARGEUNIT.")) {
		unit->type = ELECTRICCHARGEUNIT;
		unit->unitType = L"ELECTRICCHARGEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCONDUCTANCEUNIT.")) {
		unit->type = ELECTRICCONDUCTANCEUNIT;
		unit->unitType = L"ELECTRICCONDUCTANCEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICCURRENTUNIT.")) {
		unit->type = ELECTRICCURRENTUNIT;
		unit->unitType = L"ELECTRICCURRENTUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICRESISTANCEUNIT.")) {
		unit->type = ELECTRICRESISTANCEUNIT;
		unit->unitType = L"ELECTRICRESISTANCEUNIT";
	}
	else if (equalStr(unitType, L".ELECTRICVOLTAGEUNIT.")) {
		unit->type = ELECTRICVOLTAGEUNIT;
		unit->unitType = L"ELECTRICVOLTAGEUNIT";
	}
	else if (equalStr(unitType, L".ENERGYUNIT.")) {
		unit->type = ENERGYUNIT;
		unit->unitType = L"ENERGYUNIT";
	}
	else if (equalStr(unitType, L".FORCEUNIT.")) {
		unit->type = FORCEUNIT;
		unit->unitType = L"FORCEUNIT";
	}
	else if (equalStr(unitType, L".FREQUENCYUNIT.")) {
		unit->type = FREQUENCYUNIT;
		unit->unitType = L"FREQUENCYUNIT";
	}
	else if (equalStr(unitType, L".ILLUMINANCEUNIT.")) {
		unit->type = ILLUMINANCEUNIT;
		unit->unitType = L"ILLUMINANCEUNIT";
	}
	else if (equalStr(unitType, L".INDUCTANCEUNIT.")) {
		unit->type = INDUCTANCEUNIT;
		unit->unitType = L"INDUCTANCEUNIT";
	}
	else if (equalStr(unitType, L".LENGTHUNIT.")) {
		unit->type = LENGTHUNIT;
		unit->unitType = L"LENGTHUNIT";
	}
	else if (equalStr(unitType, L".LUMINOUSFLUXUNIT.")) {
		unit->type = LUMINOUSFLUXUNIT;
		unit->unitType = L"LUMINOUSFLUXUNIT";
	}
	else if (equalStr(unitType, L".LUMINOUSINTENSITYUNIT.")) {
		unit->type = LUMINOUSINTENSITYUNIT;
		unit->unitType = L"LUMINOUSINTENSITYUNIT";
	}
	else if (equalStr(unitType, L".MAGNETICFLUXDENSITYUNIT.")) {
		unit->type = MAGNETICFLUXDENSITYUNIT;
		unit->unitType = L"MAGNETICFLUXDENSITYUNIT";
	}
	else if (equalStr(unitType, L".MAGNETICFLUXUNIT.")) {
		unit->type = MAGNETICFLUXUNIT;
		unit->unitType = L"MAGNETICFLUXUNIT";
	}
	else if (equalStr(unitType, L".MASSUNIT.")) {
		unit->type = MASSUNIT;
		unit->unitType = L"MASSUNIT";
	}
	else if (equalStr(unitType, L".PLANEANGLEUNIT.")) {
		unit->type = PLANEANGLEUNIT;
		unit->unitType = L"PLANEANGLEUNIT";
	}
	else if (equalStr(unitType, L".POWERUNIT.")) {
		unit->type = POWERUNIT;
		unit->unitType = L"POWERUNIT";
	}
	else if (equalStr(unitType, L".PRESSUREUNIT.")) {
		unit->type = PRESSUREUNIT;
		unit->unitType = L"PRESSUREUNIT";
	}
	else if (equalStr(unitType, L".RADIOACTIVITYUNIT.")) {
		unit->type = RADIOACTIVITYUNIT;
		unit->unitType = L"RADIOACTIVITYUNIT";
	}
	else if (equalStr(unitType, L".SOLIDANGLEUNIT.")) {
		unit->type = SOLIDANGLEUNIT;
		unit->unitType = L"SOLIDANGLEUNIT";
	}
	else if (equalStr(unitType, L".THERMODYNAMICTEMPERATUREUNIT.")) {
		unit->type = THERMODYNAMICTEMPERATUREUNIT;
		unit->unitType = L"THERMODYNAMICTEMPERATUREUNIT";
	}
	else if (equalStr(unitType, L".TIMEUNIT.")) {
		unit->type = TIMEUNIT;
		unit->unitType = L"TIMEUNIT";
	}
	else if (equalStr(unitType, L".VOLUMEUNIT.")) {
		unit->type = VOLUMEUNIT;
		unit->unitType = L"VOLUMEUNIT";
	}
	else if (equalStr(unitType, L".USERDEFINED.")) {
		unit->type = USERDEFINED;
		unit->unitType = L"USERDEFINED";
	}
	else {
		ASSERT(false);
	}
}

void	UnitAddPrefix(STRUCT__SIUNIT * unit, wchar_t * prefix)
{
	//
	//	prefix
	//
	if (equalStr(prefix, L".EXA.")) {
		unit->prefix = L"Exa";
	}
	else if (equalStr(prefix, L".PETA.")) {
		unit->prefix = L"Peta";
	}
	else if (equalStr(prefix, L".TERA.")) {
		unit->prefix = L"Tera";
	}
	else if (equalStr(prefix, L".GIGA.")) {
		unit->prefix = L"Giga";
	}
	else if (equalStr(prefix, L".MEGA.")) {
		unit->prefix = L"Mega";
	}
	else if (equalStr(prefix, L".KILO.")) {
		unit->prefix = L"Kilo";
	}
	else if (equalStr(prefix, L".HECTO.")) {
		unit->prefix = L"Hecto";
	}
	else if (equalStr(prefix, L".DECA.")) {
		unit->prefix = L"Deca";
	}
	else if (equalStr(prefix, L".DECI.")) {
		unit->prefix = L"Deci";
	}
	else if (equalStr(prefix, L".CENTI.")) {
		unit->prefix = L"Centi";
	}
	else if (equalStr(prefix, L".MILLI.")) {
		unit->prefix = L"Milli";
	}
	else if (equalStr(prefix, L".MICRO.")) {
		unit->prefix = L"Micro";
	}
	else if (equalStr(prefix, L".NANO.")) {
		unit->prefix = L"Nano";
	}
	else if (equalStr(prefix, L".PICO.")) {
		unit->prefix = L"Pico";
	}
	else if (equalStr(prefix, L".FEMTO.")) {
		unit->prefix = L"Femto";
	}
	else if (equalStr(prefix, L".ATTO.")) {
		unit->prefix = L"Atto";
	}
	else {
		ASSERT(prefix == 0);
	}
}

void	UnitAddName(STRUCT__SIUNIT * unit, wchar_t * name)
{
	//
	//	name
	//
	if (equalStr(name, L".AMPERE.")) {
		unit->name = L"Ampere";
	}
	else if (equalStr(name, L".BECQUEREL.")) {
		unit->name = L"Becquerel";
	}
	else if (equalStr(name, L".CANDELA.")) {
		unit->name = L"Candela";
	}
	else if (equalStr(name, L".COULOMB.")) {
		unit->name = L"Coulomb";
	}
	else if (equalStr(name, L".CUBIC_METRE.")) {
		unit->name = L"Cubic Metre";
	}
	else if (equalStr(name, L".DEGREE_CELSIUS.")) {
		unit->name = L"Degree Celcius";
	}
	else if (equalStr(name, L".FARAD.")) {
		unit->name = L"Farad";
	}
	else if (equalStr(name, L".GRAM.")) {
		unit->name = L"Gram";
	}
	else if (equalStr(name, L".GRAY.")) {
		unit->name = L"Gray";
	}
	else if (equalStr(name, L".HENRY.")) {
		unit->name = L"Henry";
	}
	else if (equalStr(name, L".HERTZ.")) {
		unit->name = L"Hertz";
	}
	else if (equalStr(name, L".JOULE.")) {
		unit->name = L"Joule";
	}
	else if (equalStr(name, L".KELVIN.")) {
		unit->name = L"Kelvin";
	}
	else if (equalStr(name, L".LUMEN.")) {
		unit->name = L"Lumen";
	}
	else if (equalStr(name, L".LUX.")) {
		unit->name = L"Lux";
	}
	else if (equalStr(name, L".METRE.")) {
		unit->name = L"Metre";
	}
	else if (equalStr(name, L".MOLE.")) {
		unit->name = L"Mole";
	}
	else if (equalStr(name, L".NEWTON.")) {
		unit->name = L"Newton";
	}
	else if (equalStr(name, L".OHM.")) {
		unit->name = L"Ohm";
	}
	else if (equalStr(name, L".PASCAL.")) {
		unit->name = L"Pascal";
	}
	else if (equalStr(name, L".RADIAN.")) {
		unit->name = L"Radian";
	}
	else if (equalStr(name, L".SECOND.")) {
		unit->name = L"Second";	
	}
	else if (equalStr(name, L".SIEMENS.")) {
		unit->name = L"Siemens";
	}
	else if (equalStr(name, L".SIEVERT.")) {
		unit->name = L"Sievert";
	}
	else if (equalStr(name, L".SQUARE_METRE.")) {
		unit->name = L"Square Metre";
	}
	else if (equalStr(name, L".STERADIAN.")) {
		unit->name = L"Steradian";
	}
	else if (equalStr(name, L".TESLA.")) {
		unit->name = L"Tesla";
	}
	else if (equalStr(name, L".VOLT.")) {
		unit->name = L"Volt";
	}
	else if (equalStr(name, L".WATT.")) {
		unit->name = L"Watt";
	}
	else if (equalStr(name, L".WEBER.")) {
		unit->name = L"Weber";
	}
	else {
		ASSERT(false);
	}
}

void	CleanUnits__MemoryStructure(
				STRUCT__SIUNIT			* units
			)
{
	while (units) {
		STRUCT__SIUNIT	* unitToRemove = units;

		units = units->next;
		delete unitToRemove;
	}
}

STRUCT__SIUNIT	* GetUnits(int_t ifcModel, int_t ifcProjectInstance)
{
	STRUCT__SIUNIT	* firstUnit = 0;

	int_t	ifcUnitAssignmentInstance = 0;
	sdaiGetAttrBN(ifcProjectInstance, (char*) L"UnitsInContext", sdaiINSTANCE, &ifcUnitAssignmentInstance);

	int_t	ifcConversianBasedUnit_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCCONVERSIONBASEDUNIT"),
			ifcSIUnit_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCSIUNIT");

	int_t	* unit_set = 0, unit_cnt, i = 0;

	sdaiGetAttrBN(ifcUnitAssignmentInstance, (char*) L"Units", sdaiAGGR, &unit_set);

	unit_cnt = sdaiGetMemberCount(unit_set);
	for (i = 0; i < unit_cnt; ++i) {
		int_t	ifcUnitInstance = 0;
		engiGetAggrElement(unit_set, i, sdaiINSTANCE, &ifcUnitInstance);
		if (sdaiGetInstanceType(ifcUnitInstance) == ifcConversianBasedUnit_TYPE) {
			STRUCT__SIUNIT	* unit = new STRUCT__SIUNIT();
			unit->unitType = 0;
			unit->prefix = 0;
			unit->name = 0;
			unit->next = firstUnit;
			firstUnit = unit;

			int_t	ifcMeasureWithUnitInstance = 0;

			sdaiGetAttrBN(ifcUnitInstance, (char*) L"ConversionFactor", sdaiINSTANCE, &ifcMeasureWithUnitInstance);

			if (ifcMeasureWithUnitInstance) {
				int_t	ifcSIUnitInstance = 0;

				int_t  * adb = 0;
				sdaiGetAttrBN(ifcMeasureWithUnitInstance, (char*)L"UnitComponent", sdaiINSTANCE, &ifcSIUnitInstance);
				sdaiGetAttrBN(ifcMeasureWithUnitInstance, (char*)L"ValueComponent", sdaiADB, &adb);

				double value = 0;
				sdaiGetADBValue(adb, sdaiREAL, &value);

				if (sdaiGetInstanceType(ifcSIUnitInstance) == ifcSIUnit_TYPE) {
					wchar_t	* unitType = 0, * prefix = 0, * name = 0;

					sdaiGetAttrBN(ifcSIUnitInstance, (char*) L"UnitType", sdaiUNICODE, &unitType);
					sdaiGetAttrBN(ifcSIUnitInstance, (char*) L"Prefix", sdaiUNICODE, &prefix);
					sdaiGetAttrBN(ifcSIUnitInstance, (char*) L"Name", sdaiUNICODE, &name);

					UnitAddUnitType(unit, unitType);
					UnitAddPrefix(unit, prefix);
					UnitAddName(unit, name);
				}
				else {
					ASSERT(false);
				}
			}
			else {
				ASSERT(false);
			}
		}
		else if (sdaiGetInstanceType(ifcUnitInstance) == ifcSIUnit_TYPE) {
			STRUCT__SIUNIT	* unit = new STRUCT__SIUNIT();
			unit->unitType = 0;
			unit->prefix = 0;
			unit->name = 0;
			unit->next = firstUnit;
			firstUnit = unit;

			wchar_t	* unitType = 0, * prefix = 0, * name = 0;

			sdaiGetAttrBN(ifcUnitInstance, (char*) L"UnitType", sdaiUNICODE, &unitType);
			sdaiGetAttrBN(ifcUnitInstance, (char*) L"Prefix", sdaiUNICODE, &prefix);
			sdaiGetAttrBN(ifcUnitInstance, (char*) L"Name", sdaiUNICODE, &name);

			UnitAddUnitType(unit, unitType);
			UnitAddPrefix(unit, prefix);
			UnitAddName(unit, name);
		}
		else {
///////////////////			ASSERT(false);
		}
	}

	return	firstUnit;
}

wchar_t	* GetUnit(STRUCT__SIUNIT * units, wchar_t * unitType)
{
	STRUCT__SIUNIT	* unit = units;

	while (unit) {
		if (equalStr(unit->unitType, unitType)) {
			int_t i = 0, j = 0;
			if (unit->prefix) {
				while (unit->prefix[i]) { i++; }
				i++;
			}
			if (unit->name) {
				while (unit->name[j]) { j++; }
			}

			wchar_t	* rValue = new wchar_t[i + j + 1];

			i = 0;
			if (unit->prefix) {
				while (unit->prefix[i]) { rValue[i++] = unit->prefix[i]; }
				rValue[i++] = ' ';
			}
			j = 0;
			if (unit->name) {
				while (unit->name[j]) { rValue[i + j++] = unit->name[j]; }
				rValue[i + j] = 0;
			}
			return	rValue;
		}
		unit = unit->next;
	}

	return	nullptr;
}

void	CreateIfcPropertySingleValue(int_t ifcPropertySingleValue, STRUCT__PROPERTY * ifcProperty, STRUCT__SIUNIT * units)
{
	wchar_t	* nominalValue = nullptr,
			* unitName = nullptr,
			* typePath = nullptr;
	int_t	* nominalValueADB = nullptr;

	sdaiGetAttrBN(ifcPropertySingleValue, (char*) L"NominalValue", sdaiUNICODE, &nominalValue);
	if (nominalValue) {
		sdaiGetAttrBN(ifcPropertySingleValue, (char*) L"NominalValue", sdaiADB, &nominalValueADB);
		typePath = (wchar_t*) sdaiGetADBTypePath(nominalValueADB, 0);
		sdaiGetAttrBN(ifcPropertySingleValue, (char*) L"Unit", sdaiUNICODE, &unitName);

		if (unitName == nullptr) {
			if (equalStr(typePath, L"IFCBOOLEAN")) {
			}
			else if (equalStr(typePath, L"IFCIDENTIFIER")) {
			}
			else if (equalStr(typePath, L"IFCINTEGER")) {
			}
			else if (equalStr(typePath, L"IFCLABEL")) {
			}
			else if (equalStr(typePath, L"IFCLOGICAL")) {
			}
			else if (equalStr(typePath, L"IFCTEXT")) {
			}
			else if (equalStr(typePath, L"IFCREAL")) {
			}
			else if (equalStr(typePath, L"IFCCOUNTMEASURE")) {
			}
			else if (equalStr(typePath, L"IFCPOSITIVERATIOMEASURE")) {
			}
			else if (equalStr(typePath, L"IFCVOLUMETRICFLOWRATEMEASURE")) {
			}
			else if (equalStr(typePath, L"IFCABSORBEDDOSEMEASURE")) {
				unitName = GetUnit(units, L"ABSORBEDDOSEUNIT");
			}
			else if (equalStr(typePath, L"IFCAMOUNTOFSUBSTANCEMEASURE")) {
				unitName = GetUnit(units, L"AMOUNTOFSUBSTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCAREAMEASURE")) {
				unitName = GetUnit(units, L"AREAUNIT");
			}
			else if (equalStr(typePath, L"IFCDOSEEQUIVALENTMEASURE")) {
				unitName = GetUnit(units, L"DOSEEQUIVALENTUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCAPACITANCEMEASURE")) {
				unitName = GetUnit(units, L"ELECTRICCAPACITANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCHARGEMEASURE")) {
				unitName = GetUnit(units, L"ELECTRICCHARGEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCONDUCTANCEMEASURE")) {
				unitName = GetUnit(units, L"ELECTRICCONDUCTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICCURRENTMEASURE")) {
				unitName = GetUnit(units, L"ELECTRICCURRENTUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICRESISTANCEMEASURE")) {
				unitName = GetUnit(units, L"ELECTRICRESISTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCELECTRICVOLTAGEMEASURE")) {
				unitName = GetUnit(units, L"ELECTRICVOLTAGEUNIT");
			}
			else if (equalStr(typePath, L"IFCENERGYMEASURE")) {
				unitName = GetUnit(units, L"ENERGYUNIT");
			}
			else if (equalStr(typePath, L"IFCFORCEMEASURE")) {
				unitName = GetUnit(units, L"FORCEUNIT");
			}
			else if (equalStr(typePath, L"IFCFREQUENCYMEASURE")) {
				unitName = GetUnit(units, L"FREQUENCYUNIT");
			}
			else if (equalStr(typePath, L"IFCILLUMINANCEMEASURE")) {
				unitName = GetUnit(units, L"ILLUMINANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCINDUCTANCEMEASURE")) {
				unitName = GetUnit(units, L"INDUCTANCEUNIT");
			}
			else if (equalStr(typePath, L"IFCLENGTHMEASURE")  ||  equalStr(typePath, L"IFCPOSITIVELENGTHMEASURE")) {
				unitName = GetUnit(units, L"LENGTHUNIT");
			}
			else if (equalStr(typePath, L"IFCLUMINOUSFLUXMEASURE")) {
				unitName = GetUnit(units, L"LUMINOUSFLUXUNIT");
			}
			else if (equalStr(typePath, L"IFCLUMINOUSINTENSITYMEASURE")) {
				unitName = GetUnit(units, L"LUMINOUSINTENSITYUNIT");
			}
			else if (equalStr(typePath, L"IFCMAGNETICFLUXDENSITYMEASURE")) {
				unitName = GetUnit(units, L"MAGNETICFLUXDENSITYUNIT");
			}
			else if (equalStr(typePath, L"IFCMAGNETICFLUXMEASURE")) {
				unitName = GetUnit(units, L"MAGNETICFLUXUNIT");
			}
			else if (equalStr(typePath, L"IFCMASSMEASURE")) {
				unitName = GetUnit(units, L"MASSUNIT");
			}
			else if (equalStr(typePath, L"IFCPLANEANGLEMEASURE")) {
				unitName = GetUnit(units, L"PLANEANGLEUNIT");
			}
			else if (equalStr(typePath, L"IFPOWERCMEASURE")) {
				unitName = GetUnit(units, L"POWERUNIT");
			}
			else if (equalStr(typePath, L"IFCPRESSUREMEASURE")) {
				unitName = GetUnit(units, L"PRESSUREUNIT");
			}
			else if (equalStr(typePath, L"IFCRADIOACTIVITYMEASURE")) {
				unitName = GetUnit(units, L"RADIOACTIVITYUNIT");
			}
			else if (equalStr(typePath, L"IFCSOLIDANGLEMEASURE")) {
				unitName = GetUnit(units, L"SOLIDANGLEUNIT");
			}
			else if (equalStr(typePath, L"IFCTHERMODYNAMICTEMPERATUREMEASURE")) {
				unitName = GetUnit(units, L"THERMODYNAMICTEMPERATUREUNIT");
			}
			else if (equalStr(typePath, L"IFCTIMEMEASURE")) {
				unitName = GetUnit(units, L"TIMEUNIT");
			}
			else if (equalStr(typePath, L"IFCVOLUMEMEASURE")) {
				unitName = GetUnit(units, L"VOLUMEUNIT");
			}
			else if (equalStr(typePath, L"IFCUSERDEFINEDMEASURE")) {
				unitName = GetUnit(units, L"USERDEFINED");
			}
			else if (equalStr(typePath, L"IFCTHERMALTRANSMITTANCEMEASURE")) {
				unitName = GetUnit(units, L"???");
			}
			else {
//				ASSERT(false);
			}
		}
		else {
			ASSERT(false);
		}
		ifcProperty->nominalValue = copyStr(nominalValue);
		ifcProperty->unitName = unitName;
	}
	else {
		ifcProperty->nominalValue = nullptr;
		ifcProperty->unitName = nullptr;
	}
}

void	CreateIfcQuantityLength(int_t ifcQuantityLength, STRUCT__PROPERTY * ifcProperty, STRUCT__SIUNIT * units)
{
	wchar_t	* lengthValue = 0,
			* unit = 0;
	int_t	ifcUnitInstance = 0;

	sdaiGetAttrBN(ifcQuantityLength, (char*) L"LengthValue", sdaiUNICODE, &lengthValue);
	sdaiGetAttrBN(ifcQuantityLength, (char*) L"Unit", sdaiUNICODE, &unit);
	sdaiGetAttrBN(ifcQuantityLength, (char*) L"Unit", sdaiINSTANCE, &ifcUnitInstance);

	ASSERT(ifcUnitInstance == 0);

	ifcProperty->lengthValue = copyStr(lengthValue);
	ifcProperty->unitName = copyStr(unit);
	if (unit == 0  ||  unit[0] == 0) {
		while (units) {
			if (units->type == LENGTHUNIT) {
				if (ifcProperty->unitName) {
					delete[] ifcProperty->unitName;
				}
				ifcProperty->unitName = copyStr(units->name);
			}
			units = units->next;
		}
	}
}

void	CreateIfcQuantityArea(int_t ifcQuantityArea, STRUCT__PROPERTY * ifcProperty, STRUCT__SIUNIT * units)
{
	wchar_t	* areaValue = 0,
			* unit = 0;

	sdaiGetAttrBN(ifcQuantityArea, (char*) L"AreaValue", sdaiUNICODE, &areaValue);
	sdaiGetAttrBN(ifcQuantityArea, (char*) L"Unit", sdaiUNICODE, &unit);

	ifcProperty->areaValue = copyStr(areaValue);
	ifcProperty->unitName = copyStr(unit);
	if (unit == 0  ||  unit[0] == 0) {
		while (units) {
			if (units->type == AREAUNIT) {
				if (ifcProperty->unitName) {
					delete[] ifcProperty->unitName;
				}
				ifcProperty->unitName = copyStr(units->name);
			}
			units = units->next;
		}
	}
}

void	CreateIfcQuantityVolume(int_t ifcQuantityVolume, STRUCT__PROPERTY * ifcProperty, STRUCT__SIUNIT * units)
{
	wchar_t	* volumeValue = 0,
			* unit = 0;

	sdaiGetAttrBN(ifcQuantityVolume, (char*) L"VolumeValue", sdaiUNICODE, &volumeValue);
	sdaiGetAttrBN(ifcQuantityVolume, (char*) L"Unit", sdaiUNICODE, &unit);

	ifcProperty->volumeValue = copyStr(volumeValue);
	ifcProperty->unitName = copyStr(unit);
	if (unit == 0  ||  unit[0] == 0) {
		while (units) {
			if (units->type == VOLUMEUNIT) {
				if (ifcProperty->unitName) {
					delete[] ifcProperty->unitName;
				}
				ifcProperty->unitName = copyStr(units->name);
			}
			units = units->next;
		}
	}
}

void	CreateIfcQuantityCount(int_t ifcQuantityCount, STRUCT__PROPERTY * ifcProperty)//, STRUCT__SIUNIT * units)
{
	wchar_t	* countValue = 0,
			* unit = 0;

	sdaiGetAttrBN(ifcQuantityCount, (char*) L"CountValue", sdaiUNICODE, &countValue);
	sdaiGetAttrBN(ifcQuantityCount, (char*) L"Unit", sdaiUNICODE, &unit);

	ifcProperty->countValue = copyStr(countValue);
	ifcProperty->unitName = copyStr(unit);
}

void	CreateIfcQuantityWeigth(int_t ifcQuantityWeigth, STRUCT__PROPERTY * ifcProperty, STRUCT__SIUNIT * units)
{
	wchar_t	* weigthValue = 0,
			* unit = 0;

	sdaiGetAttrBN(ifcQuantityWeigth, (char*) L"WeigthValue", sdaiUNICODE, &weigthValue);
	sdaiGetAttrBN(ifcQuantityWeigth, (char*) L"Unit", sdaiUNICODE, &unit);

	ifcProperty->weigthValue = copyStr(weigthValue);
	ifcProperty->unitName = copyStr(unit);
	if	(unit == 0  ||  unit[0] == 0) {
		while  (units) {
			if	(units->type == MASSUNIT) {
				if (ifcProperty->unitName) {
					delete[] ifcProperty->unitName;
				}
				ifcProperty->unitName = copyStr(units->name);
			}
			units = units->next;
		}
	}
}

void	CreateIfcQuantityTime(int_t ifcQuantityTime, STRUCT__PROPERTY * ifcProperty, STRUCT__SIUNIT * units)
{
	wchar_t	* timeValue = 0,
			* unit = 0;

	sdaiGetAttrBN(ifcQuantityTime, (char*) L"TimeValue", sdaiUNICODE, &timeValue);
	sdaiGetAttrBN(ifcQuantityTime, (char*) L"Unit", sdaiUNICODE, &unit);

	ifcProperty->timeValue = copyStr(timeValue);
	ifcProperty->unitName = copyStr(unit);
	if	(unit == 0  ||  unit[0] == 0) {
		while  (units) {
			if	(units->type == TIMEUNIT) {
				if (ifcProperty->unitName) {
					delete[] ifcProperty->unitName;
				}
				ifcProperty->unitName = copyStr(units->name);
			}
			units = units->next;
		}
	}
}

void	CreateIfcElementQuantity(int_t ifcModel, STRUCT__PROPERTY__SET ** propertySets, int_t ifcPropertySetInstance, STRUCT__SIUNIT * units)
{
	STRUCT__PROPERTY__SET	** ppPropertySet = propertySets;
	while  ((* ppPropertySet)) {
		ppPropertySet = &(* ppPropertySet)->next;
	}

	wchar_t	* name = 0, * description = 0;
	sdaiGetAttrBN(ifcPropertySetInstance, (char*) L"Name", sdaiUNICODE, &name);
	sdaiGetAttrBN(ifcPropertySetInstance, (char*) L"Description", sdaiUNICODE, &description);

	ASSERT((* ppPropertySet) == 0);
	(* ppPropertySet) = CreateIfcPropertySet(ifcPropertySetInstance, name, description);
	STRUCT__PROPERTY	** ppProperty = &(* ppPropertySet)->properties;

	int_t	* ifcQuantityInstances = 0;
#ifdef	_DEBUG
//	int_t	ifcRelDefinesByType_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYTYPE"),
//			ifcRelDefinesByProperties_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYPROPERTIES");
#endif

	sdaiGetAttrBN(ifcPropertySetInstance, (char*) L"Quantities", sdaiAGGR, &ifcQuantityInstances);

	if	(ifcQuantityInstances) {
		int_t	ifcQuantityInstancesCnt = sdaiGetMemberCount(ifcQuantityInstances);
		for  (int_t i = 0; i < ifcQuantityInstancesCnt; ++i) {
			int_t	ifcQuantityInstance = 0,
					ifcQuantityLength_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCQUANTITYLENGTH"),
					ifcQuantityArea_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCQUANTITYAREA"),
					ifcQuantityVolume_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCQUANTITYVOLUME"),
					ifcQuantityCount_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCQUANTITYCOUNT"),
					ifcQuantityWeigth_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCQUANTITYWEIGHT"),
					ifcQuantityTime_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCQUANTITYTIME");

			engiGetAggrElement(ifcQuantityInstances, i, sdaiINSTANCE, &ifcQuantityInstance);

			wchar_t	* quantityName = 0, * quantityDescription = 0;
			sdaiGetAttrBN(ifcQuantityInstance, (char*) L"Name", sdaiUNICODE, &quantityName);
			sdaiGetAttrBN(ifcQuantityInstance, (char*) L"Description", sdaiUNICODE, &quantityDescription);

			ASSERT((* ppProperty) == 0);
			(* ppProperty) = CreateIfcProperty(ifcQuantityInstance, quantityName, quantityDescription);

			if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityLength_TYPE) {
				CreateIfcQuantityLength(ifcQuantityInstance, (* ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityArea_TYPE) {
				CreateIfcQuantityArea(ifcQuantityInstance, (* ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityVolume_TYPE) {
				CreateIfcQuantityVolume(ifcQuantityInstance, (* ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityCount_TYPE) {
				CreateIfcQuantityCount(ifcQuantityInstance, (* ppProperty));
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityWeigth_TYPE) {
				CreateIfcQuantityWeigth(ifcQuantityInstance, (* ppProperty), units);
			}
			else if (sdaiGetInstanceType(ifcQuantityInstance) == ifcQuantityTime_TYPE) {
				CreateIfcQuantityTime(ifcQuantityInstance, (* ppProperty), units);
			}
			else {
				ASSERT(false);
			}

			ppProperty = &(* ppProperty)->next;
		}
	}
}

STRUCT__PROPERTY	** CreateIfcComplexProperty(int_t ifcModel, int_t ifcComplexProperty, STRUCT__PROPERTY ** ppProperty, STRUCT__SIUNIT * units)
{
	int_t	* ifcPropertyInstances = 0,
			ifcPropertySingleValue_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCPROPERTYSINGLEVALUE"),
			ifcComplexProperty_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCCOMPLEXPROPERTY");

	sdaiGetAttrBN(ifcComplexProperty, (char*) L"HasProperties", sdaiAGGR, &ifcPropertyInstances);

	if	(ifcPropertyInstances) {
		int_t	ifcPropertyInstancesCnt = sdaiGetMemberCount(ifcPropertyInstances);
		for  (int_t i = 0; i < ifcPropertyInstancesCnt; ++i) {
			int_t	ifcPropertyInstance = 0;
			engiGetAggrElement(ifcPropertyInstances, i, sdaiINSTANCE, &ifcPropertyInstance);

			wchar_t	* propertyName = 0, * propertyDescription = 0;
			sdaiGetAttrBN(ifcPropertyInstance, (char*) L"Name", sdaiUNICODE, &propertyName);
			sdaiGetAttrBN(ifcPropertyInstance, (char*) L"Description", sdaiUNICODE, &propertyDescription);

			if (sdaiGetInstanceType(ifcPropertyInstance) == ifcPropertySingleValue_TYPE) {
				(*ppProperty) = CreateIfcProperty(ifcPropertyInstance, propertyName, propertyDescription);
				CreateIfcPropertySingleValue(ifcPropertyInstance, (* ppProperty), units);
				ppProperty = &(*ppProperty)->next;
			}
			else if (sdaiGetInstanceType(ifcPropertyInstance) == ifcComplexProperty_TYPE) {
				ppProperty = CreateIfcComplexProperty(ifcModel, ifcPropertyInstance, ppProperty, units);
			}
			else {
				ASSERT(false);
			}
		}
	}

	return	ppProperty;
}

void	CreateIfcPropertySet(int_t ifcModel, STRUCT__PROPERTY__SET ** propertySets, int_t ifcPropertySetInstance, STRUCT__SIUNIT * units)
{
	STRUCT__PROPERTY__SET	** ppPropertySet = propertySets;
	while  ((* ppPropertySet)) {
		ppPropertySet = &(* ppPropertySet)->next;
	}

	wchar_t	* name = 0, * description = 0;
	sdaiGetAttrBN(ifcPropertySetInstance, (char*) L"Name", sdaiUNICODE, &name);
	sdaiGetAttrBN(ifcPropertySetInstance, (char*) L"Description", sdaiUNICODE, &description);

	ASSERT((* ppPropertySet) == 0);
	(* ppPropertySet) = CreateIfcPropertySet(ifcPropertySetInstance, name, description);
	STRUCT__PROPERTY	** ppProperty = &(* ppPropertySet)->properties;

	int_t	* ifcPropertyInstances = 0;
#ifdef	_DEBUG
//	int_t	ifcRelDefinesByType_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYTYPE"),
//			ifcRelDefinesByProperties_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYPROPERTIES");
#endif

	sdaiGetAttrBN(ifcPropertySetInstance, (char*) L"HasProperties", sdaiAGGR, &ifcPropertyInstances);

	if	(ifcPropertyInstances) {
		int_t	ifcPropertyInstancesCnt = sdaiGetMemberCount(ifcPropertyInstances);
		for  (int_t i = 0; i < ifcPropertyInstancesCnt; ++i) {
			int_t	ifcPropertyInstance = 0,
					ifcPropertySingleValue_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCPROPERTYSINGLEVALUE"),
					ifcComplexProperty_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCCOMPLEXPROPERTY");

			engiGetAggrElement(ifcPropertyInstances, i, sdaiINSTANCE, &ifcPropertyInstance);

			wchar_t	* propertyName = 0, * propertyDescription = 0;
			sdaiGetAttrBN(ifcPropertyInstance, (char*) L"Name", sdaiUNICODE, &propertyName);
			sdaiGetAttrBN(ifcPropertyInstance, (char*) L"Description", sdaiUNICODE, &propertyDescription);

			ASSERT((* ppProperty) == 0);

			if (sdaiGetInstanceType(ifcPropertyInstance) == ifcPropertySingleValue_TYPE) {
				(*ppProperty) = CreateIfcProperty(ifcPropertyInstance, propertyName, propertyDescription);
				CreateIfcPropertySingleValue(ifcPropertyInstance, (* ppProperty), units);
				ppProperty = &(*ppProperty)->next;
			}
			else if (sdaiGetInstanceType(ifcPropertyInstance) == ifcComplexProperty_TYPE) {
				ppProperty = CreateIfcComplexProperty(ifcModel, ifcPropertyInstance, ppProperty, units);
			}
			else {
				ASSERT(false);
			}
		}
	}
}

void	CreateTypeObjectInstance(int_t ifcModel, STRUCT__PROPERTY__SET ** propertySets, int_t ifcTypeObjectInstance, STRUCT__SIUNIT * units)
{
	if	(ifcTypeObjectInstance) {
		int_t	* hasPropertySets = 0, hasPropertySetsCnt,
				ifcElementQuantity_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCELEMENTQUANTITY"),
				ifcPropertySet_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCPROPERTYSET");

		sdaiGetAttrBN(ifcTypeObjectInstance, (char*) L"HasPropertySets", sdaiAGGR, &hasPropertySets);

		hasPropertySetsCnt = sdaiGetMemberCount(hasPropertySets);
		for  (int_t i = 0; i < hasPropertySetsCnt; ++i) {
			int_t	hasPropertySetInstance = 0;
			engiGetAggrElement(hasPropertySets, i, sdaiINSTANCE, &hasPropertySetInstance);
			if (sdaiGetInstanceType(hasPropertySetInstance) == ifcElementQuantity_TYPE) {
				CreateIfcElementQuantity(ifcModel, propertySets, hasPropertySetInstance, units);
			}
			else if (sdaiGetInstanceType(hasPropertySetInstance) == ifcPropertySet_TYPE) {
				CreateIfcPropertySet(ifcModel, propertySets, hasPropertySetInstance, units);
			}
			else {
				ASSERT(false);
			}
		}
	}
}

void	CreateRelDefinesByProperties(int_t ifcModel, STRUCT__PROPERTY__SET ** propertySets, int_t ifcRelDefinesByProperties, STRUCT__SIUNIT * units)
{
	if (ifcRelDefinesByProperties) {
		int_t	ifcPropertySetInstance = 0,
				ifcElementQuantity_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCELEMENTQUANTITY"),
				ifcPropertySet_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCPROPERTYSET");

		sdaiGetAttrBN(ifcRelDefinesByProperties, (char*) L"RelatingPropertyDefinition", sdaiINSTANCE, &ifcPropertySetInstance);

		if (sdaiGetInstanceType(ifcPropertySetInstance) == ifcElementQuantity_TYPE) {
			CreateIfcElementQuantity(ifcModel, propertySets, ifcPropertySetInstance, units);
		}
		else if (sdaiGetInstanceType(ifcPropertySetInstance) == ifcPropertySet_TYPE) {
			CreateIfcPropertySet(ifcModel, propertySets, ifcPropertySetInstance, units);
		}
		else {
			ASSERT(false);
		}
	}
	else {
		ASSERT(false);
	}
}

void			DeleteIfcInstanceProperties(
						STRUCT__PROPERTY__SET	* propertySet
					)
{
	while (propertySet) {
		STRUCT__PROPERTY__SET	* pSetToRemove = propertySet;
		STRUCT__PROPERTY		* properties = propertySet->properties;
		while (properties) {
			STRUCT__PROPERTY	* pToRemove = properties;
			properties = properties->next;

			if (pToRemove->name) {
				delete pToRemove->name;
			}
			if (pToRemove->description) {
				delete pToRemove->description;
			}
			if (pToRemove->nominalValue) {
				delete pToRemove->nominalValue;
			}
			if (pToRemove->lengthValue) {
				delete pToRemove->lengthValue;
			}
			if (pToRemove->areaValue) {
				delete pToRemove->areaValue;
			}
			if (pToRemove->volumeValue) {
				delete pToRemove->volumeValue;
			}
			if (pToRemove->countValue) {
				delete pToRemove->countValue;
			}
			if (pToRemove->weigthValue) {
				delete pToRemove->weigthValue;
			}
			if (pToRemove->timeValue) {
				delete pToRemove->timeValue;
			}
			if (pToRemove->unitName) {
				delete pToRemove->unitName;
			}
			delete pToRemove;
		}
		propertySet = propertySet->next;

		if (pSetToRemove->name) {
			delete[] pSetToRemove->name;
		}
		if (pSetToRemove->description) {
			delete[] pSetToRemove->description;
		}
		delete pSetToRemove;
	}
}

void	CreateIfcInstanceProperties(int_t ifcModel, STRUCT__PROPERTY__SET ** propertySets, int_t ifcObjectInstance, STRUCT__SIUNIT * units)
{
	int_t	* isDefinedByInstances = 0,
			ifcRelDefinesByType_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYTYPE"),
			ifcRelDefinesByProperties_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYPROPERTIES");

	sdaiGetAttrBN(ifcObjectInstance, (char*) L"IsDefinedBy", sdaiAGGR, &isDefinedByInstances);

	if (isDefinedByInstances) {
		int_t	isDefinedByInstancesCnt = sdaiGetMemberCount(isDefinedByInstances);
		for (int_t i = 0; i < isDefinedByInstancesCnt; ++i) {
			int_t	isDefinedByInstance = 0;
			engiGetAggrElement(isDefinedByInstances, i, sdaiINSTANCE, &isDefinedByInstance);

			if (sdaiGetInstanceType(isDefinedByInstance) == ifcRelDefinesByType_TYPE) {
				int_t	typeObjectInstance = 0;

				sdaiGetAttrBN(isDefinedByInstance, (char*) L"RelatingType", sdaiINSTANCE, &typeObjectInstance);

				CreateTypeObjectInstance(ifcModel, propertySets, typeObjectInstance, units);
			}
			else if (sdaiGetInstanceType(isDefinedByInstance) == ifcRelDefinesByProperties_TYPE) {
				CreateRelDefinesByProperties(ifcModel, propertySets, isDefinedByInstance, units);
			}
			else {
				ASSERT(false);
			}
		}
	}
}

bool	IfcInstanceHasProperties(
				int_t	ifcModel,
				int_t	ifcObjectInstance
			)
{
	int_t	* isDefinedByInstances = 0,
			ifcRelDefinesByType_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYTYPE"),
			ifcRelDefinesByProperties_TYPE = sdaiGetEntity(ifcModel, (char*) L"IFCRELDEFINESBYPROPERTIES");

	sdaiGetAttrBN(ifcObjectInstance, (char*) L"IsDefinedBy", sdaiAGGR, &isDefinedByInstances);

	if (isDefinedByInstances) {
		int_t	isDefinedByInstancesCnt = sdaiGetMemberCount(isDefinedByInstances);
		for (int_t i = 0; i < isDefinedByInstancesCnt; ++i) {
			int_t	isDefinedByInstance = 0;
			engiGetAggrElement(isDefinedByInstances, i, sdaiINSTANCE, &isDefinedByInstance);

			if (sdaiGetInstanceType(isDefinedByInstance) == ifcRelDefinesByType_TYPE) {
				//...
			}
			else if (sdaiGetInstanceType(isDefinedByInstance) == ifcRelDefinesByProperties_TYPE) {
				return	true;
			}
			else {
				ASSERT(false);
			}
		}
	}

	return	false;
}
 