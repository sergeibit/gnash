// 
//   Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#include "asClass.h"
#include "as_object.h"
#include "ClassHierarchy.h"
#include "VM.h"
#include "namedStrings.h"
#include "as_value.h"
#include "asNamespace.h"
#include "Global_as.h"

#ifdef ENABLE_AVM2
#include "asMethod.h"
#include "abc_function.h"
#endif

namespace gnash {

#ifdef ENABLE_AVM2    
bool
asClass::addValue(string_table::key name, asNamespace *ns,
        boost::uint32_t slotId, asClass *type, as_value& val, bool isconst,
        bool isstatic)
{
    Global_as* g = VM::get().getGlobal();

    if (val.is_object()) {
		val.to_object(*g)->set_member(NSV::INTERNAL_TYPE, 
			std::size_t(type->getName()));
    }

	string_table::key nsname = ns ? ns->getURI() : string_table::key(0);

	int flags = PropFlags::dontDelete;
	if (isconst)
		flags |= PropFlags::readOnly;
	if (isstatic)
		flags |= PropFlags::staticProp;

	if(slotId == 0){
		_prototype->init_member(name, val, flags, nsname);
	}
	else{
		_prototype->init_member(name, val, flags, nsname, slotId);
	}
	return true;
}
bool
asClass::addMemberClass(string_table::key name, asNamespace *ns,
	boost::uint32_t slotId, asClass *type, bool isstatic)
{
	return addSlot(name, ns, slotId, type, isstatic);
}

bool
asClass::addSlotFunction(string_table::key name, asNamespace *ns,
	boost::uint32_t slotId, asMethod *method, bool isstatic)
{
	asClass a;
	a.setName(NSV::CLASS_FUNCTION);
	as_value b(method->getPrototype());
	return addValue(name, ns, slotId, &a, b, false, isstatic);
}

bool
asClass::addSlot(string_table::key name, asNamespace* ns,
        boost::uint32_t slotId, asClass* /*type*/, bool /*isstatic*/)
{
	string_table::key nsname = ns ? ns->getURI() : 0;

	//TODO: Set flags.
	if (slotId == 0) {
		_prototype->init_member(name, as_value(), 0, nsname);
	}
	else {
		_prototype->reserveSlot(name, nsname, slotId);
	}
	return true;
}

    bool
asClass::addMethod(string_table::key name, asNamespace* /*ns*/,
        asMethod* method, bool /*isstatic*/)
{
	as_value val = new abc_function(method, getVM(*_prototype).getMachine());
	_prototype->init_member(name, val);
//	int flags = PropFlags::readOnly | PropFlags::dontDelete
//		| PropFlags::dontEnum;
//	if (isstatic)
//		flags |= PropFlags::staticProp;

	return true;
}


bool
asClass::addGetter(string_table::key name, asNamespace *ns, asMethod *method,
	bool isstatic)
{
	string_table::key nsname = ns ? ns->getURI() : string_table::key(0);

	Property *getset = _prototype->getOwnProperty(name, nsname);

	if (getset)
		getset->setGetter(method->getPrototype());
	else
	{
		int flags = PropFlags::dontDelete | PropFlags::dontEnum;
		if (isstatic)
			flags |= PropFlags::staticProp;
		_prototype->init_property(name, *method->getPrototype(), 
			*method->getPrototype(), flags, nsname);
	}
	return true;
}

bool
asClass::addSetter(string_table::key name, asNamespace *ns, asMethod *method,
	bool isstatic)
{
	string_table::key nsname = ns ? ns->getURI() : string_table::key(0);

	Property *getset = _prototype->getOwnProperty(name, nsname);

	if (getset)
		getset->setSetter(method->getPrototype());
	else
	{
		int flags = PropFlags::dontDelete | PropFlags::dontEnum;
		if (isstatic)
			flags |= PropFlags::staticProp;
		_prototype->init_property(name, *method->getPrototype(), 
			*method->getPrototype(), flags, nsname);
	}
	return true;
}

#if 0 // TODO
void
asClass::buildFromPrototype(as_object *o, string_table::key name,
	ClassHierarchy *pCH)
{
	setName(name);
	PropertyList *pList = &o->_members;
	PropertyList::iterator i = pList->begin();

	for ( ; i != pList->end(); ++i)
	{
		Property *pProp = i->second;
		fprintf(stderr, "Evaluating property %s.\n", STV(i->first));
		if (pProp->isDestructive())
		{
			fprintf(stderr, "%s is destructive.\n", STV(i->first));
		}
		if (pProp->isGetterSetter())
		{
			fprintf(stderr, "%s is a getset.\n", STV(i->first));
		}
		if (pProp->isReadOnly())
		{
			fprintf(stderr, "%s is read only.\n", STV(i->first));
		}
	}
}

bool
asClass::addValue(string_table::key name, asNamespace *ns, boost::uint32_t slotId,
	asClass *type, as_value& val, bool isconst, bool isstatic,
	ClassHierarchy *CH)
{
	asBoundValue *bv = CH->newBoundValue();
	bv->setType(type);
	bv->setValue(val);
	if (!isstatic)
		return addBinding(name, asBinding(ns, bv, slotId, isconst, isstatic));
	return addStaticBinding(name, asBinding(ns, bv, slotId, isconst, isstatic));
}

bool
asClass::addSlot(string_table::key name, asNamespace *ns, boost::uint32_t slotId,
	asClass *type, bool isstatic, ClassHierarchy *CH)
{
	asBoundValue *bv = CH->newBoundValue();
	bv->setType(type);
	if (!isstatic)
		return addBinding(name, asBinding(ns, bv, slotId, isstatic));
	return addStaticBinding(name, asBinding(ns, bv, slotId, isstatic));
}

bool
asClass::addMethod(string_table::key name, asNamespace *ns, asMethod *method,
	bool isstatic)
{
	if (!isstatic)
		return addBinding(name, asBinding(ns, method, isstatic));
	else
		return addStaticBinding(name, asBinding(ns, method, isstatic));
}

bool
asClass::addMemberClass(string_table::key name, asNamespace *ns,
	boost::uint32_t slotId, asClass *type, bool isstatic)
{
	if (!isstatic)
		return addBinding(name, asBinding(ns, type, slotId, isstatic));
	return addStaticBinding(name, asBinding(ns, type, slotId, isstatic));
}

// TODO: Figure out how this differs from addMethod
bool
asClass::addSlotFunction(string_table::key name, asNamespace *ns,
	boost::uint32_t slotId, asMethod *method, bool isstatic)
{
	if (!isstatic)
		return addBinding(name, asBinding(ns, method, slotId, isstatic));
	return addStaticBinding(name, asBinding(ns, method, slotId, isstatic));
}
#endif 

#endif

} /* namespace gnash */
