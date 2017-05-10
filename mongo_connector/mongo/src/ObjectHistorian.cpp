/* $CORTO_GENERATED
 *
 * ObjectHistorian.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/mongo_connector/mongo/mongo.h>

/* $header() */
#include "mongo_util.h"
#include <recorto/mongo_connector/mongo/historian_buffer.h>

/* ********************************
 * Mongo utility
 * ********************************/
typedef MongoHistorian::CMongoHistorian CMongoHistorian;

#define CORTO_TIME_TO_MILLIS(time) time.sec*1000 + time.nanosec/1000000
#define TO_MILLISECONDS(time_point) std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch())
#define MONGO_HISTORIAN(addr) (CMongoHistorian*)(void*)addr;

static corto_time CortoTime(uint64_t millis)
{
    int32_t seconds = (millis/1000);
    uint32_t nanosec = (millis - seconds*1000)*1000000;
    return corto_time{seconds, nanosec};
}

/* ********************************
 * MONGO HISTORIAN
 * ********************************/
struct mongo_objIterData
{
    MongoHistorian::Cursor cursor;
    corto_result           result;
};

void *mongo_objIterDataNext(corto_iter *iter);
int mongo_objIterDataHasNext(corto_iter *iter);
void mongo_objIterDataRelease(corto_iter *iter);

mongo_objIterData *mongo_objIterDataNew (MongoHistorian::Cursor &&cursor);

struct mongo_objSampleData
{
    MongoHistorian::HistoryCursor cursor;
    std::string                   value;
    corto_sample                  sample;
};

void *mongo_objSampleNext(corto_iter *iter);
int mongo_objSampleHasNext(corto_iter *iter);
void mongo_objSampleRelease(corto_iter *iter);
mongo_objSampleData *mongo_objSampleDataNew(MongoHistorian::HistoryCursor &&cursor);

/* **********************************
 * MONGO ITERATOR
 * **********************************/
void *mongo_objIterDataNext (corto_iter *iter)
{
    mongo_objIterData *pData = (mongo_objIterData*)iter->udata;

    MongoHistorian::Data data = pData->cursor.GetData();

    std::string &id = data.m_name;
    std::string &type = data.m_type;

    mongo_objSampleData *pSample =  nullptr;

    try
    {
        MongoHistorian::HistoryCursor history = pData->cursor.GetHistoryCursor();
        pSample = mongo_objSampleDataNew(std::move(history));
    }
    catch (std::exception &e)
    {
        corto_error("%s", e.what());
    }

    pData->result.history.udata = pSample;
    pData->result.history.hasNext = mongo_objSampleHasNext;
    pData->result.history.next = mongo_objSampleNext;
    pData->result.history.release = mongo_objSampleRelease;

    corto_setstr(&pData->result.id, (char*)id.c_str());
    corto_setstr(&pData->result.type, (char*)type.c_str());
    corto_setstr(&pData->result.parent, ".");
    pData->result.value = NULLWORD;
    pData->result.name = NULL;

    pData->cursor.Next();

    return &pData->result;
}

int mongo_objIterDataHasNext (corto_iter *iter)
{
    mongo_objIterData *pData = (mongo_objIterData*)iter->udata;
    bool retVal = pData->cursor.HasNext();
    return retVal;
}

void mongo_objIterDataRelease (corto_iter *iter)
{
    if (iter->udata != NULLWORD)
    {
        mongo_objIterData *pData = (mongo_objIterData*)iter->udata;
        pData->cursor.~Cursor();
        free(pData);
        iter->udata = NULLWORD;
    }
}

mongo_objIterData *mongo_objIterDataNew (MongoHistorian::Cursor &&cursor)
{
    mongo_objIterData *pData = (mongo_objIterData*)calloc(1, sizeof(mongo_objIterData));
    new (&pData->cursor) MongoHistorian::Cursor(std::move(cursor));
    return pData;
}

/* *************************************
 * HISTORIAN ITERATOR
 * *************************************/
void *mongo_objSampleNext (corto_iter *iter)
{
    mongo_objSampleData *pSample =(mongo_objSampleData*)iter->udata;

    MongoHistorian::Data data = pSample->cursor.GetData();

    pSample->value = data.m_data;
    pSample->sample.timestamp = CortoTime(data.m_timestamp);
    pSample->sample.value = (corto_word)pSample->value.c_str();

    pSample->cursor.Next();

    return &pSample->sample;
}

int mongo_objSampleHasNext (corto_iter *iter)
{
    mongo_objSampleData *pSample = (mongo_objSampleData*)iter->udata;
    bool retVal = pSample->cursor.HasNext();
    return retVal;
}

void mongo_objSampleRelease (corto_iter *iter)
{
    if (iter->udata != NULLWORD)
    {
        mongo_objSampleData *pSample = (mongo_objSampleData*)iter->udata;

        typedef std::string String;
        typedef MongoHistorian::HistoryCursor Cursor;

        pSample->value.~String();
        pSample->cursor.~Cursor();

        free(pSample);
        iter->udata = NULLWORD;
    }
}

mongo_objSampleData *mongo_objSampleDataNew (MongoHistorian::HistoryCursor &&cursor)
{
    mongo_objSampleData *pSample = (mongo_objSampleData*)calloc(1,sizeof(mongo_objSampleData));

    new (&pSample->cursor) MongoHistorian::HistoryCursor(std::move(cursor));
    new (&pSample->value) std::string();

    return pSample;
}
/* $end */

corto_int16 _mongo_ObjectHistorian_construct(
    mongo_ObjectHistorian _this)
{
/* $begin(recorto/mongo_connector/mongo/ObjectHistorian/construct) */
    CMongoHistorian *pHistorian = new CMongoHistorian();

    pHistorian->Initialize(SAFE_STRING(_this->user),
                           SAFE_STRING(_this->password),
                           SAFE_STRING(_this->hostaddr),
                           std::to_string(_this->port),
                           SAFE_STRING(_this->dbname),
                           _this->sample_rate,
                           _this->sync_rate,
                           _this->expire_after_seconds);
    _this->mongo_handle = (corto_word)pHistorian;

    corto_observer(_this)->mask = CORTO_ON_TREE;
    corto_mount(_this)->kind = CORTO_HISTORIAN;
    return corto_mount_construct(_this);
/* $end */
}

corto_void _mongo_ObjectHistorian_destruct(
    mongo_ObjectHistorian _this)
{
/* $begin(recorto/mongo_connector/mongo/ObjectHistorian/destruct) */
    if (_this->mongo_handle != NULLWORD)
    {
        CMongoHistorian *pHistorian = MONGO_HISTORIAN(_this->mongo_handle);
        pHistorian->Stop();
        delete pHistorian;
        _this->mongo_handle = NULLWORD;
    }
    corto_mount_destruct(_this);
/* $end */
}

corto_void _mongo_ObjectHistorian_onNotify(
    mongo_ObjectHistorian _this,
    corto_eventMask event,
    corto_result *object)
{
/* $begin(recorto/mongo_connector/mongo/ObjectHistorian/onNotify) */
    if (_this->mongo_handle == NULLWORD)
    {
        return;
    }

    CMongoHistorian *pHistorian = MONGO_HISTORIAN(_this->mongo_handle);
    if (event & CORTO_ON_DEFINE)
    {
        std::string  parent = SAFE_STRING(object->parent);
        std::string  id = SAFE_STRING(object->id);
        std::string  type = SAFE_STRING(object->type);

        pHistorian->UpdateSample(parent, id, type, object->object);
    }
    else if (event & CORTO_ON_UPDATE)
    {
        std::string  parent = SAFE_STRING(object->parent);
        std::string  id = SAFE_STRING(object->id);
        std::string  type = SAFE_STRING(object->type);

        pHistorian->UpdateSample(parent, id, type, object->object);
    }
    else if (event & CORTO_ON_DELETE)
    {

    }
/* $end */
}

corto_resultIter _mongo_ObjectHistorian_onRequest(
    mongo_ObjectHistorian _this,
    corto_request *request)
{
/* $begin(recorto/mongo_connector/mongo/ObjectHistorian/onRequest) */
    corto_resultIter result;
    if (request->from.kind == CORTO_FRAME_NOW &&
        request->to.kind == CORTO_FRAME_NOW)
    {
        return CORTO_ITERATOR_EMPTY;
    }

    std::string parent = SAFE_STRING(request->parent);
    std::string expr = SAFE_STRING(request->expr);

    bool reverse = false;
    optional<std::chrono::milliseconds> start;
    optional<std::chrono::milliseconds> end;
    optional<uint64_t> skip;
    optional<uint64_t> limit;

    switch (request->from.kind) {
        case CORTO_FRAME_NOW:
        {
            auto now = std::chrono::system_clock::now();
            end.setValue(TO_MILLISECONDS(now));
            reverse = true;
            break;
        }
        case CORTO_FRAME_TIME:
        {
            corto_time from = corto_frame_getTime(&request->from);
            uint64_t millis = from.sec*1000+from.nanosec/1000000;
            start.setValue(std::chrono::milliseconds(millis));
            break;
        }
        case CORTO_FRAME_DEPTH:
        case CORTO_FRAME_SAMPLE:
        {
            skip.setValue(request->from.value);
            break;
        }
        default:
        {
            break;
        }
    }
    switch (request->to.kind) {
        case CORTO_FRAME_NOW:
        {
            break;
        }
        case CORTO_FRAME_TIME:
        {
            corto_time to = corto_frame_getTime(&request->to);
            uint64_t millis = CORTO_TIME_TO_MILLIS(to);

            if (start && (uint64_t)start.value().count() > millis)
            {
                auto t = start.value();
                start.setValue(std::chrono::milliseconds(millis));
                end.setValue(t);

                reverse = true;
            }
            else if (reverse)
            {
                start.setValue(std::chrono::milliseconds(millis));
            }
            else
            {
                end.setValue(std::chrono::milliseconds(millis));
            }

            break;
        }
        case CORTO_FRAME_DURATION:
        {
            corto_time to = corto_frame_getTime(&request->to);
            uint64_t millis = to.sec*1000+to.nanosec/1000000;
            // CORTO_FRAME_NOW && CORTO_FRAME_TIME
            if (start)
            {
                if (reverse)
                {
                    millis = end.value().count() - millis;
                    start.setValue(std::chrono::milliseconds(millis));
                }
                else
                {
                    millis = start.value().count() + millis;
                    end.setValue(std::chrono::milliseconds(millis));
                }
            }
            // TODO: relative to sample

            break;
        }
        case CORTO_FRAME_SAMPLE:
        {
            if (reverse)
            {
                // TODO: fromNow to Sample(x) reverse
            }
            else if (skip)
            {
                limit.setValue(request->to.value - skip.value());
            }
            else
            {
                limit.setValue(request->to.value);
            }

            break;
        }
        case CORTO_FRAME_DEPTH:
        {
            limit.setValue(request->to.value);
            break;
        }
        default:
        {
            break;
        }
    }

    mongo_objIterData *pData = nullptr;

    if (parent == ".")
    {
        parent = "/";
    }

    StrToLower(parent);
    StrToLower(expr);

    try
    {
        CMongoHistorian *pHistorian = MONGO_HISTORIAN(_this->mongo_handle);
        typedef MongoHistorian::Cursor Cursor;

        Cursor cursor = pHistorian->GetCursor(parent,
                                              expr,
                                              start,
                                              end,
                                              skip,
                                              limit,
                                              reverse);

        pData = mongo_objIterDataNew(std::move(cursor));
    }
    catch (std::exception &e)
    {
        corto_error("%s", e.what());
    }

    result.udata = pData;
    result.hasNext = mongo_objIterDataHasNext;
    result.next = mongo_objIterDataNext;
    result.release = mongo_objIterDataRelease;

    return result;
/* $end */
}
