#include "mongo_util.h"

#include <bsoncxx/json.hpp>

//Custom json deserializing
namespace mongo_json
{
    static corto_int16 jsonDeserType(void *p, corto_type t, JSON_Value *v);

    static corto_int16 jsonDeserBoolean(void* o, corto_primitive t, JSON_Value* v) {
        CORTO_UNUSED(t);

        if (json_value_get_type(v) != JSONBoolean) {
            corto_error("no deserializing JSON boolean");
            return -1;
        }

        *(corto_bool *)o = json_value_get_boolean(v);

        return 0;
    }

    static corto_int16 jsonDeserNumber(void* o, corto_primitive t, JSON_Value* v) {
        if (json_value_get_type(v) != JSONNumber) {
            corto_error("not deserializing JSON number");
            return -1;
        }

        corto_float64 number = json_value_get_number(v);
        corto_convert(
            corto_primitive(corto_float64_o),
            &number,
            t,
            o);

        return 0;
    }

    static corto_int16 jsonDeserText(void* p, corto_primitive t, JSON_Value *v) {
        const char *s = json_value_get_string(v);
        CORTO_UNUSED(t);
        corto_setstr((corto_string*)p, (corto_string)s);
        return 0;
    }

    corto_bool jsonDeserPrimitive(void* p, corto_type t, JSON_Value* v) {
        corto_assert(t->kind == CORTO_PRIMITIVE, "not deserializing primitive");

        corto_primitive ptype = corto_primitive(t);

        switch (ptype->kind) {
        case CORTO_BOOLEAN:
            if (jsonDeserBoolean(p, ptype, v)) {
                return -1;
            }
            break;
        case CORTO_CHARACTER:
            break;
        case CORTO_INTEGER:
        case CORTO_UINTEGER:
        case CORTO_FLOAT:
        case CORTO_BINARY:
            if (jsonDeserNumber(p, ptype, v)) {

            }
            break;
        case CORTO_TEXT:
            if (jsonDeserText(p, ptype, v)) {

            }
        case CORTO_ENUM:
            break;
        case CORTO_BITMASK:
            break;
        }
        return 0;
    }

    corto_int16 jsonDeserReference(void* p, corto_type t, JSON_Value* v) {
        const char* reference = json_value_get_string(v);
        if (!reference) {
            corto_error("not deserializing JSON string");
            return -1;
        }

        corto_object o = corto_resolve(NULL, (corto_string)reference);
        if (!o) {
            corto_error("cannot find reference \"%s\"", reference);
            return -1;
        }

        if (!corto_instanceof(t, o)) {
            corto_error("%s is not an instance of %s", reference, corto_idof(t));
        }

        corto_setref(p, o);

        corto_release(o);

        return 0;
    }

    static corto_int16 jsonDeserItem(void *p, corto_type t, JSON_Value *v) {

        if (t->reference) {
            if (jsonDeserReference(p, t, v)) {
                return -1;
            }
        } else {
            if (jsonDeserType(p, t, v)) {
                return -1;
            }
        }

        return 0;
    }

    static corto_int16 jsonDeserComposite(void* p, corto_type t, JSON_Value *v) {
        if (json_value_get_type(v) != JSONObject) {
            corto_error("composite value is not JSON object");
            return -1;
        }
        corto_interface ci = corto_interface(t);
        if (ci->base)
        {
            if (jsonDeserType(p, corto_type(ci->base), v))
            {
                return -1;
            }
        }
        JSON_Object* o = json_value_get_object(v);
        for (size_t i = 0; i < ci->members.length; i++)
        {
            corto_member member_o = ci->members.buffer[i];
            corto_string memberName = corto_idof(member_o);
            JSON_Value* value = json_object_get_value(o, memberName);
            if (value)
            {
                void *offset = CORTO_OFFSET(p, member_o->offset);
                if (jsonDeserItem(offset, member_o->type, value)) {
                    return -1;
                }
            }
        }
        return 0;;
    }

    static corto_int16 jsonDeserType(void *p, corto_type t, JSON_Value *v) {
        switch (t->kind) {
        case CORTO_VOID:
            /* Nothing to deserialize */
            break;
        case CORTO_PRIMITIVE:
            if (jsonDeserPrimitive(p, t, v)) {
                return -1;
            }
            break;
        case CORTO_COMPOSITE:
            if (jsonDeserComposite(p, t, v)) {
                return -1;
            }
            break;
        case CORTO_COLLECTION:
            break;
        default:
            corto_seterr(
                "unsupported type, can't serialize JSON");
            break;
        }

        return 0;
    }

    corto_int16 json_deserialize(corto_object o, corto_string s) {
        char *json = s;
        if ((json[0] != '{') && (json[1] != '[')) {
            corto_asprintf(&json, "{\"value\": %s}", s);
        }

        JSON_Value *jsonValue = json_parse_string(json);

        if (o) {
            JSON_Value *value = jsonValue;

            if (!jsonValue) {
                corto_seterr("invalid JSON '%s'", s);
                if (json != s) {
                    corto_dealloc(json);
                }

                if (jsonValue) {
                    json_value_free(jsonValue);
                }
                return -1;
            }

            if (corto_typeof(o)->kind == CORTO_PRIMITIVE) {
                JSON_Object* jsonObj = json_value_get_object(jsonValue);
                if (!jsonObj) {
                    corto_seterr("invalid JSON for primitive value '%s'", json);
                    if (json != s) {
                        corto_dealloc(json);
                    }

                    if (jsonValue) {
                        json_value_free(jsonValue);
                    }
                    return -1;
                }

                size_t count = json_object_get_count(jsonObj);
                if (count == 1) {
                    value = json_object_get_value(jsonObj, "value");
                } else {
                    corto_seterr("invalid JSON for primitive value '%s'", json);
                    if (json != s) {
                        corto_dealloc(json);
                    }

                    if (jsonValue) {
                        json_value_free(jsonValue);
                    }
                    return -1;
                }
            }

            if (jsonDeserType(o, corto_typeof(o), value)) {
                if (json != s) {
                    corto_dealloc(json);
                }

                if (jsonValue) {
                    json_value_free(jsonValue);
                }
                return -1;
            }
        } else {
            corto_seterr("no object provided to JSON deserializer");
            if (json != s) {
                corto_dealloc(json);
            }

            if (jsonValue) {
                json_value_free(jsonValue);
            }
            return -1;
        }

        if (json != s) {
            corto_dealloc(json);
        }
        if (jsonValue) {
            json_value_free(jsonValue);
        }

        return 0;
    }
}
