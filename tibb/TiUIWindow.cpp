/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2012 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#include "TiUIWindow.h"
#include "TiGenericFunctionObject.h"

#include "NativeWindowObject.h"

TiUIWindow::TiUIWindow(const char* name)
    : TiUIBase(name)
{
}

TiUIWindow::~TiUIWindow()
{
}

TiUIWindow* TiUIWindow::createWindow(NativeObjectFactory* objectFactory)
{
    TiUIWindow* obj = new TiUIWindow("");
    obj->setNativeObjectFactory(objectFactory);
    obj->initializeTiObject(NULL);
    return obj;
}

void TiUIWindow::onCreateStaticMembers()
{
    TiUIBase::onCreateStaticMembers();
    TiGenericFunctionObject::addGenericFunctionToParent(this, "open", this, _open);
    TiGenericFunctionObject::addGenericFunctionToParent(this, "close", this, _close);
    TiGenericFunctionObject::addGenericFunctionToParent(this, "addAction", this, _addAction);
}

void TiUIWindow::initializeTiObject(TiObject* parentContext)
{
    if (!isInitialized())
    {
        TiUIBase::initializeTiObject(parentContext);
        NativeObject* obj = getNativeObjectFactory()->createNativeObject(N_TYPE_WINDOW, this);
        setNativeObject(obj);
        obj->release();
    }
}

Handle<Value> TiUIWindow::_open(void* userContext, TiObject*, const Arguments&)
{
    HandleScope scope;
    TiUIWindow* self = static_cast<TiUIWindow*>(userContext);
    NativeWindowObject* window = static_cast<NativeWindowObject*>(self->getNativeObject());

    vector<ObjectEntry>::iterator it;
    for (it = self->childControls_.begin(); it != self->childControls_.end(); it++)
    {
        (*it)->setupEvents();
    }

    window->open();
    window->release(); // XXX(josh): Do we really want to release now?

    return Undefined();
}

Handle<Value> TiUIWindow::_close(void* userContext, TiObject*, const Arguments&)
{
    HandleScope scope;
    TiUIWindow* self = static_cast<TiUIWindow*>(userContext);
    NativeWindowObject* window = static_cast<NativeWindowObject*>(self->getNativeObject());

    window->close();

    return Undefined();
}

Handle<Value> TiUIWindow::_addAction(void* userContext, TiObject* caller, const Arguments& args)
{
    HandleScope scope;
    TiUIWindow* self = static_cast<TiUIWindow*>(userContext);
    NativeWindowObject* window = static_cast<NativeWindowObject*>(self->getNativeObject());

    if (args.Length() < 2) {
        return ThrowException(Exception::Error(String::New("Missing arguments: addAction(title, triggerCallback)")));
    }
    if (!args[1]->IsFunction()) {
        return ThrowException(Exception::Error(String::New("'triggerCallback' argument must be function.")));
    }

    QString title = QString::fromUtf8(*String::Utf8Value(args[0]));
    Handle<Function> triggerCallback = Handle<Function>::Cast(args[1]);
    window->addAction(title, triggerCallback);

    return Undefined();
}
