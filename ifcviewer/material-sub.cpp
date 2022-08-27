




bool	walkThroughGeometry__object(__int64 model, __int64 owlInstance, __int64 expressId, __int64 depth, __int64 transformationOwlInstance)
{
	__int64	* owlInstanceExpressID = nullptr, expressIDCard = 0;
	GetDataTypeProperty(owlInstance, owlDataTypePropertyExpressID, (void **) &owlInstanceExpressID, &expressIDCard);
	if	(expressIDCard == 1) {
		__int64	expressID = owlInstanceExpressID[0];
		if	(expressId == expressID) {
			SetObjectTypeProperty(transformationOwlInstance, owlObjectTypePropertyObject, &owlInstance, 1);
			return	true;
		} else {
			if	(GetInstanceClass(owlInstance) == rdfClassTransformation) {
				__int64	* owlInstanceObject = nullptr, objectCard = 0;
				GetObjectTypeProperty(owlInstance, owlObjectTypePropertyObject, &owlInstanceObject, &objectCard);
				if	(objectCard == 1) {
					__int64	* owlInstanceMatrix = nullptr, matrixCard = 0;
					GetObjectTypeProperty(owlInstance, owlObjectTypePropertyMatrix, &owlInstanceMatrix, &matrixCard);

					__int64	subTransformationOwlInstance = CreateInstance(rdfClassTransformation, 0);
					SetObjectTypeProperty(transformationOwlInstance, owlObjectTypePropertyObject, &subTransformationOwlInstance, 1);
					if	(matrixCard == 1) {
						SetObjectTypeProperty(subTransformationOwlInstance, owlObjectTypePropertyMatrix, owlInstanceMatrix, 1);
					} else {
						ASSERT(false);
					}

					return	walkThroughGeometry__object(model, owlInstanceObject[0], expressId, depth+1, subTransformationOwlInstance);
				} else {
					ASSERT(false);
				}
			} else if  (GetInstanceClass(owlInstance) == rdfClassCollection) {
				__int64	* owlInstanceObjects = nullptr, objectsCard = 0;
				GetObjectTypeProperty(owlInstance, owlObjectTypePropertyObjects, &owlInstanceObjects, &objectsCard);
				int_t	i = 0;
				while  (i < objectsCard) {
					if	(walkThroughGeometry__object(model, owlInstanceObjects[i], expressId, depth+1, transformationOwlInstance)) {
						return	true;
					}
					i++;
				}
			} else {
				ASSERT(false);
			}
		}
	} else {
		ASSERT(depth);
		if	(GetInstanceClass(owlInstance) == rdfClassTransformation) {
			__int64	* owlInstanceObject = nullptr, objectCard = 0;
			GetObjectTypeProperty(owlInstance, owlObjectTypePropertyObject, &owlInstanceObject, &objectCard);

			__int64	* owlInstanceMatrix = nullptr, matrixCard = 0;
			GetObjectTypeProperty(owlInstance, owlObjectTypePropertyMatrix, &owlInstanceMatrix, &matrixCard);

			__int64	subTransformationOwlInstance = CreateInstance(rdfClassTransformation, 0);
			SetObjectTypeProperty(transformationOwlInstance, owlObjectTypePropertyObject, &subTransformationOwlInstance, 1);
			if	(matrixCard == 1) {
				SetObjectTypeProperty(subTransformationOwlInstance, owlObjectTypePropertyMatrix, owlInstanceMatrix, 1);
			} else {
				ASSERT(false);
			}

			if	(objectCard == 1) {
				return	walkThroughGeometry__object(model, owlInstanceObject[0], expressId, depth+1, subTransformationOwlInstance);
			} else {
				ASSERT(false);
			}
		} else if  (GetInstanceClass(owlInstance) == rdfClassCollection) {
			__int64	* owlInstanceObjects = nullptr, objectsCard = 0;
			GetObjectTypeProperty(owlInstance, owlObjectTypePropertyObjects, &owlInstanceObjects, &objectsCard);
			int_t	i = 0;
			while  (i < objectsCard) {
				if	(walkThroughGeometry__object(model, owlInstanceObjects[i], expressId, depth+1, transformationOwlInstance)) {
					return	true;
				}
				i++;
			}
		} else {
			ASSERT(false);
		}
	}

	return	false;
}

bool	walkThroughGeometry__collection(__int64 model, __int64 owlInstance, __int64 expressId, __int64 transformationOwlInstance)
{
	if	(GetInstanceClass(owlInstance) == rdfClassCollection) {
		__int64	* owlInstanceObjects = nullptr, objectsCard = 0;
		GetObjectTypeProperty(owlInstance, owlObjectTypePropertyObjects, &owlInstanceObjects, &objectsCard);
		int_t	i = 0;
		while  (i < objectsCard) {
			if	(walkThroughGeometry__collection(model, owlInstanceObjects[i], expressId, transformationOwlInstance)) {
				return	true;
			}
			i++;
		}
	} else {
		return	walkThroughGeometry__object(model, owlInstance, expressId, 0, transformationOwlInstance);
	}

	return	false;
}

__int64		walkThroughGeometry__transformation(__int64 model, __int64 owlInstance, __int64 expressId)
{
	ASSERT(GetInstanceClass(owlInstance) == rdfClassTransformation);

	__int64	owlInstanceBase = CreateInstance(rdfClassTransformation, 0);

	__int64	* owlInstanceMatrix = nullptr, matrixCard = 0;
	GetObjectTypeProperty(owlInstance, owlObjectTypePropertyMatrix, &owlInstanceMatrix, &matrixCard);
	if	(matrixCard == 1) {
		SetObjectTypeProperty(owlInstanceBase, owlObjectTypePropertyMatrix, owlInstanceMatrix, 1);
	} else {
		ASSERT(false);
	}

	__int64	* owlInstanceObject = nullptr, objectCard = 0;
	GetObjectTypeProperty(owlInstance, owlObjectTypePropertyObject, &owlInstanceObject, &objectCard);
	if	(objectCard == 1) {
		if	(walkThroughGeometry__collection(model, owlInstanceObject[0], expressId, owlInstanceBase) == false) {
			ASSERT(false);
		}
	} else {
		ASSERT(false);
	}

	return	owlInstanceBase;
}
