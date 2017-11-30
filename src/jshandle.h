// Credit: https://github.com/gabrielschulhof/bindings-principles

template <class T> class JSHandle {

    // This is the V8 template from which all our instances will be created
    static Nan::Persistent<v8::FunctionTemplate> &theTemplate()
    {
        static Nan::Persistent<v8::FunctionTemplate> returnValue;

        // We only create the class the first time it's needed
        if (returnValue.IsEmpty()) {
            v8::Local<v8::FunctionTemplate> theTemplate =
                Nan::New<v8::FunctionTemplate>();

            // This is why we're using a C++ template class. The specific class
            // provides a static method named jsClassName() which we can use
            // to give our class a nice-looking name
            theTemplate
                ->SetClassName(Nan::New(T::jsClassName()).ToLocalChecked());

            // This is important. This is where we tell V8 to reserve one
            // slot for an arbitrary pointer.
            theTemplate->InstanceTemplate()->SetInternalFieldCount(1);

            // We also set the "displayName" property so our instances do not
            // simply appear as "Object" in the debugger
            Nan::Set(Nan::GetFunction(theTemplate).ToLocalChecked(),
                Nan::New("displayName").ToLocalChecked(),
                Nan::New(T::jsClassName()).ToLocalChecked());

            returnValue.Reset(theTemplate);
        }

        return returnValue;
    }

public:

    // This is the API we will use. In our bindings we will instantiate new
    // handles with HandleClassName::New(native_handle);
    static v8::Local<v8::Object> New(void *data)
    {
        v8::Local<v8::Context> context = Nan::GetCurrentContext();
        v8::Local<v8::Object> returnValue =
            Nan::GetFunction(Nan::New(theTemplate())).ToLocalChecked()
            ->NewInstance(context).ToLocalChecked();
        Nan::SetInternalFieldPointer(returnValue, 0, data);

        return returnValue;
    }

    // This is how we retrieve the pointer we've stored during instantiation.
    // If the object is not of the expected type, or if the pointer inside the
    // object has already been removed, then we must throw an error. Note that
    // we assume that a Javascript handle containing a null pointer is not a
    // valid Javascript handle at all, so this setup is not adequate for
    // wrapping NULL handles.
    static void *Resolve(v8::Local<v8::Object> jsObject)
    {
        void *returnValue = 0;

        if (Nan::New(theTemplate())->HasInstance(jsObject)) {
            returnValue = Nan::GetInternalFieldPointer(jsObject, 0);
        }
        if (!returnValue) {
            Nan::ThrowTypeError((std::string("Object is not of type ") +
                T::jsClassName()).c_str());
        }
        return returnValue;
    }

};
