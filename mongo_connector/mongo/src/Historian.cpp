/* $CORTO_GENERATED
 *
 * Historian.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <recorto/mongo_connector/mongo/mongo.h>

/* $header() */
#include "mongo_util.h"
#include <recorto/mongo_connector/mongo/historian_buffer.h>

#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/builder/core.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/pipeline.hpp>

using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using bsoncxx::types::b_date;
using bsoncxx::types::b_array;
using bsoncxx::document::element;

/* ********************************
 * Mongo utility
 * ********************************/

#define CORTO_TIME_TO_MILLIS(time) time.sec*1000 + time.nanosec/1000000
#define TO_MILLISECONDS(time_point) std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch())
#define HISTORIAN_COLL(coll) "_history_"+coll

#define AND(lhs, rhs) "$and" \
    << open_array \
        << open_document << lhs << close_document \
        << open_document << rhs << close_document \
    << close_array

#define GTE(key, value) key \
    << open_document \
        << "$gte" \
        << value \
    << close_document

#define LTE(key, value) key \
    << open_document \
        << "$lte" \
        << value \
    << close_document

static b_date ISODate(std::chrono::milliseconds millis)
{
    return b_date(millis.count());
}

static corto_time CortoTime(uint64_t millis)
{
    int32_t seconds = (millis/1000);
    uint32_t nanosec = (millis - seconds*1000)*1000000;
    return corto_time{seconds, nanosec};
}

/* ********************************
 * MONGO HISTORIAN
 * ********************************/
struct mongo_iterData
{
    MongoClientPtr pClient;
    mongocxx::cursor cursor;
    mongocxx::cursor::iterator iter;

    // corto_request Options
    std::string database;
    std::string collection;
    optional<std::chrono::milliseconds> start;
    optional<std::chrono::milliseconds> end;
    optional<uint64_t> skip;
    optional<uint64_t> limit;
    bool reverse;

    corto_result result;
};

void *mongo_iterDataNext(corto_iter *iter);
int mongo_iterDataHasNext(corto_iter *iter);
void mongo_iterDataRelease(corto_iter *iter);
mongo_iterData *mongo_iterDataNew (
        MongoClientPtr &&pClient,
        mongocxx::cursor &&cursor,
        std::string &&database,
        std::string &&collection,
        optional<std::chrono::milliseconds> &&start,
        optional<std::chrono::milliseconds> &&end,
        optional<uint64_t> &&skip,
        optional<uint64_t> &&limit,
        bool reverse
);

struct mongo_sampleData
{
    mongocxx::cursor cursor;
    mongocxx::cursor::iterator iter;

    std::string  value;

    corto_sample sample;
};

void *mongo_sampleNext(corto_iter *iter);
int mongo_sampleHasNext(corto_iter *iter);
void mongo_sampleRelease(corto_iter *iter);
mongo_sampleData *mongo_sampleDataNew(mongocxx::cursor &&cursor);

void mongo_Historian_update (
    mongo_Historian _this,
    corto_string parent,
    corto_string name,
    corto_string type,
    corto_string json
);

void mongo_Historian_delete (
    mongo_Historian _this,
    corto_string parent,
    corto_string name
);

mongocxx::cursor mongo_Historian_find (
    MongoClientPtr &pClient,
    std::string &database,
    std::string &collection,
    std::string &id
);

mongocxx::cursor mongo_Historian_select (
    MongoClientPtr &pClient,
    std::string &database,
    std::string &collection,
    std::string &id,
    optional<std::chrono::milliseconds> start,
    optional<std::chrono::milliseconds> end,
    optional<uint64_t> skip,
    optional<uint64_t> limit,
    bool reverse
);

/* **********************************
 * MONGO ITERATOR
 * **********************************/
void *mongo_iterDataNext(corto_iter *iter)
{
    mongo_iterData *pData = (mongo_iterData*)iter->udata;
    bsoncxx::document::view data = *pData->iter;

    element _id = data["_id"];
    std::string type = _id["type"].get_utf8().value.to_string();
    std::string id = _id["id"].get_utf8().value.to_string();

    mongo_sampleData *pSample =  nullptr;

    try
    {
        auto res = mongo_Historian_select(pData->pClient,
                                          pData->database,
                                          pData->collection,
                                          id,
                                          pData->start,
                                          pData->end,
                                          pData->skip,
                                          pData->limit,
                                          pData->reverse);

        pSample = mongo_sampleDataNew(std::move(res));
    }
    catch (std::exception &e)
    {
        corto_error("%s", e.what());
    }

    pData->result.history.udata = pSample;
    pData->result.history.hasNext = mongo_sampleHasNext;
    pData->result.history.next = mongo_sampleNext;
    pData->result.history.release = mongo_sampleRelease;

    corto_setstr(&pData->result.id, (char*)id.c_str());
    corto_setstr(&pData->result.type, (char*)type.c_str());
    corto_setstr(&pData->result.parent, ".");
    pData->result.value = NULLWORD;
    pData->result.name = NULL;

    pData->iter++;

    return &pData->result;
}

int mongo_iterDataHasNext(corto_iter *iter)
{
    mongo_iterData *pData = (mongo_iterData*)iter->udata;
    bool retVal = false;
    while (retVal == false && pData->iter != pData->cursor.end())
    {
        bsoncxx::document::view doc = *pData->iter;
        element _id = doc["_id"];
        element name = _id["id"];
        element type = _id["type"];
        if (name && name.type() == bsoncxx::type::k_utf8 &&
            type && type.type() == bsoncxx::type::k_utf8)
        {
            retVal = true;
        }
        else
        {
            pData->iter++;
        }
    }

    return retVal;
}

void mongo_iterDataRelease(corto_iter *iter)
{
    if (iter->udata != NULLWORD)
    {
        mongo_iterData *pData = (mongo_iterData*)iter->udata;
        typedef optional<uint64_t> Optional64;
        typedef optional<std::chrono::milliseconds> OptionalMills;
        typedef std::string String;
        typedef mongocxx::cursor::iterator Iterator;
        typedef mongocxx::cursor Cursor;

        pData->limit.~Optional64();
        pData->skip.~Optional64();
        pData->end.~OptionalMills();
        pData->start.~OptionalMills();
        pData->collection.~String();
        pData->database.~String();
        pData->iter.~Iterator();
        pData->cursor.~Cursor();
        pData->pClient.~MongoClientPtr();

        free(pData);

        iter->udata = NULLWORD;
    }
}

mongo_iterData *mongo_iterDataNew (
        MongoClientPtr &&pClient,
        mongocxx::cursor &&cursor,
        std::string &&database,
        std::string &&collection,
        optional<std::chrono::milliseconds> &&start,
        optional<std::chrono::milliseconds> &&end,
        optional<uint64_t> &&skip,
        optional<uint64_t> &&limit,
        bool reverse
)
{
    mongo_iterData *pData = (mongo_iterData*)calloc(1, sizeof(mongo_iterData));
    new (&pData->pClient) MongoClientPtr(std::move(pClient));
    new (&pData->cursor) mongocxx::cursor(std::move(cursor));
    new (&pData->iter) mongocxx::cursor::iterator(pData->cursor.begin());
    new (&pData->database) std::string(std::move(database));
    new (&pData->collection) std::string(std::move(collection));
    new (&pData->start) optional<std::chrono::milliseconds>(std::move(start));
    new (&pData->end) optional<std::chrono::milliseconds>(std::move(end));
    new (&pData->skip) optional<uint64_t>(std::move(skip));
    new (&pData->limit) optional<uint64_t>(std::move(limit));
    new (&pData->reverse) bool(reverse);

    return pData;
}

/* *************************************
 * HISTORIAN ITERATOR
 * *************************************/
void *mongo_sampleNext(corto_iter *iter)
{
    mongo_sampleData *pSample =(mongo_sampleData*)iter->udata;

    bsoncxx::document::view sample = *pSample->iter;
    pSample->value = sample["value"].get_utf8().value.to_string();

    pSample->sample.timestamp = CortoTime(sample["timestamp"].get_date().value);
    pSample->sample.value = (corto_word)pSample->value.c_str();

    pSample->iter++;

    return &pSample->sample;
}

int mongo_sampleHasNext(corto_iter *iter)
{
    mongo_sampleData *pSample = (mongo_sampleData*)iter->udata;
    bool retVal = false;
    while (retVal == false && pSample->iter != pSample->cursor.end())
    {
        bsoncxx::document::view doc = *pSample->iter;
        element timestamp = doc["timestamp"];
        element value = doc["value"];
        if (timestamp && timestamp.type() == bsoncxx::type::k_date &&
            value && value.type() == bsoncxx::type::k_utf8
        )
        {
            retVal = true;
        }
        else
        {
            pSample->iter++;
        }
    }
    return retVal;
}

void mongo_sampleRelease(corto_iter *iter)
{
    corto_debug("mongo_sampleRelease %li", iter->udata);

    if (iter->udata != NULLWORD)
    {
        mongo_sampleData *pSample = (mongo_sampleData*)iter->udata;

        typedef std::string String;
        typedef mongocxx::cursor::iterator Iterator;
        typedef mongocxx::cursor Cursor;

        pSample->value.~String();
        pSample->iter.~Iterator();
        pSample->cursor.~Cursor();
        free(pSample);


        iter->udata = NULLWORD;
    }
}

mongo_sampleData *mongo_sampleDataNew (mongocxx::cursor &&cursor)
{
    mongo_sampleData *pSample = (mongo_sampleData*)calloc(1,sizeof(mongo_sampleData));
    new (&pSample->cursor) mongocxx::cursor(std::move(cursor));
    new (&pSample->iter) mongocxx::cursor::iterator(pSample->cursor.begin());
    new (&pSample->value) std::string();

    corto_debug("mongo_sampleDataNew %li", (int64_t)pSample);

    return pSample;
}
/* $end */

corto_int16 _mongo_Historian_construct(
    mongo_Historian _this)
{
/* $begin(recorto/mongo_connector/mongo/Historian/construct) */
    mongo_Connector _base = mongo_Connector(_this);
    CMongoPool *pPool = new CMongoPool();
    CMongoHistorian *pHistorian = new CMongoHistorian();


    if (_this->sample_rate > 0)
    {
        corto_float64 frequency = 1.0f/(_this->sample_rate/1000.0f);
        corto_asprintf(&corto_mount(_this)->policy, "sampleRate=%f", frequency);
    }

    //
    pPool->Initialize(SAFE_STRING(_base->user),
                      SAFE_STRING(_base->password),
                      SAFE_STRING(_base->hostaddr),
                      std::to_string(_base->port));
    bool scale = 1;
    if (_this->sample_rate > 500)
    {
        scale = 60;
    }

    pHistorian->Initialize(pPool,
                           SAFE_STRING(_base->dbname),
                           _this->sync_rate,
                           scale,
                           _this->expire_after_seconds);

    _base->mongo_handle = (corto_word)pPool;
    _this->buffer_handle = (corto_word)pHistorian;
    corto_mount_setContentType(_this, "text/json");
    corto_mount(_this)->kind = CORTO_HISTORIAN;
    return corto_mount_construct(_this);
/* $end */
}

corto_void _mongo_Historian_destruct(
    mongo_Historian _this)
{
/* $begin(recorto/mongo_connector/mongo/Historian/destruct) */
    if (_this->buffer_handle != NULLWORD)
    {
        CMongoHistorian *pHistorian = (CMongoHistorian*)_this->buffer_handle;
        pHistorian->Stop();
        delete pHistorian;
        _this->buffer_handle = NULLWORD;
    }
    mongo_Connector_destruct(_this);
/* $end */
}

/* $header(recorto/mongo_connector/mongo/Historian/onNotify) */
void mongo_Historian_update (
    mongo_Historian _this,
    corto_string parent,
    corto_string name,
    corto_string type,
    corto_string json
)
{
    std::string _parent = SAFE_STRING(parent);
    std::string id = SAFE_STRING(name);
    std::string _type = SAFE_STRING(type);
    std::string value = SAFE_STRING(json);

    StrToLower(_parent);
    StrToLower(id);

    if (_parent == ".")
    {
        _parent = "/";
    }
    auto timeStamp = std::chrono::system_clock::now();
    uint64_t milliseconds = TO_MILLISECONDS(timeStamp).count();

    CMongoHistorian *pHistorian = (CMongoHistorian*)_this->buffer_handle;
    pHistorian->UpdateSample(std::move(_parent),
                             std::move(id),
                             std::move(_type),
                             std::move(value),
                             milliseconds);
}

void mongo_Historian_delete (
    mongo_Historian _this,
    corto_string parent,
    corto_string name
)
{
    // TODO: support multy generation objects
}


/* $end */
corto_void _mongo_Historian_onNotify(
    mongo_Historian _this,
    corto_eventMask event,
    corto_result *object)
{
/* $begin(recorto/mongo_connector/mongo/Historian/onNotify) */
    if (event & CORTO_ON_DEFINE)
    {
        corto_string json = (corto_string)(void*)object->value;
        mongo_Historian_update(_this,
                               object->parent,
                               object->id,
                               object->type,
                               json);
    }
    else if (event & CORTO_ON_UPDATE)
    {
        corto_string json = (corto_string)(void*)object->value;
        mongo_Historian_update(_this,
                               object->parent,
                               object->id,
                               object->type,
                               json);
    }
    else if (event & CORTO_ON_DELETE)
    {

    }
/* $end */
}

/* $header(recorto/mongo_connector/mongo/Historian/onRequest) */
mongocxx::cursor mongo_Historian_find (
    MongoClientPtr &pClient,
    std::string &database,
    std::string &collection,
    std::string &id
)
{
    bsoncxx::builder::stream::document filterBuilder;

    bsoncxx::builder::stream::document fieldsBuilder;
    fieldsBuilder << "_id"
                  << open_document
                     << "id" << "$id"
                     << "type" << "$type"
                  << close_document;

    mongocxx::pipeline stages;
    if (id.find("*") == std::string::npos)
    {
        filterBuilder << "id" << id;
        stages.match(filterBuilder.view());
    }
    else if (id != "*")
    {
        filterBuilder << "id" << bsoncxx::types::b_regex(id,"i");
        stages.match(filterBuilder.view());
    }

    stages.group(fieldsBuilder.view());

    mongocxx::collection coll = (*pClient)[database][collection];
    return coll.aggregate(stages);
}

mongocxx::cursor mongo_Historian_select (
    MongoClientPtr &pClient,
    std::string &database,
    std::string &collection,
    std::string &id,
    optional<std::chrono::milliseconds> start,
    optional<std::chrono::milliseconds> end,
    optional<uint64_t> skip,
    optional<uint64_t> limit,
    bool reverse
)
{
    bsoncxx::builder::stream::document filterHoursBuilder;
    bsoncxx::builder::stream::document filterMinutesBuilder;
    bsoncxx::builder::stream::document filterSecondsBuilder;
    bsoncxx::builder::stream::document filterPropertyBuilder;
    bsoncxx::builder::stream::document filterGroupBuilder;
    bsoncxx::builder::stream::document filterSortBuilder;

    // SELECT samples.samples.timestamp AS timestamp,
    //        samples.samples.value AS value
    //        samples.samples.type AS type

    filterPropertyBuilder << "samples.samples.timestamp" << 1
                          << "samples.samples.value" << 1;

    filterGroupBuilder << "_id" << "$samples.samples.timestamp"
                       << "timestamp" << open_document
                          << "$last" << "$samples.samples.timestamp"
                       << close_document
                       << "value" << open_document
                          << "$last" << "$samples.samples.value"
                       << close_document;

    // WHERE id = id
    filterHoursBuilder << "id" << id;
    if (start)
    {
        if (end)
        {
            // timestamp > start and timestamp  < end
            filterHoursBuilder << AND(GTE("timestamp", ISODate(start.value())),
                                      LTE("timestamp", ISODate(end.value())));
            filterMinutesBuilder << AND(GTE("samples.timestamp", ISODate(start.value())),
                                      LTE("samples.timestamp", ISODate(end.value())));
            filterSecondsBuilder << AND(GTE("samples.samples.timestamp", ISODate(start.value())),
                                      LTE("samples.samples.timestamp", ISODate(end.value())));
        }
        else
        {
            // timestamp > start
            filterHoursBuilder << GTE("timestamp", ISODate(start.value()));
            filterMinutesBuilder << GTE("samples.timestamp", ISODate(start.value()));
            filterSecondsBuilder << GTE("samples.samples.timestamp", ISODate(start.value()));
        }
    }
    else if (end)
    {
        // timestamp  < end
        filterHoursBuilder << LTE("timestamp", ISODate(end.value()));
        filterMinutesBuilder << LTE("samples.timestamp", ISODate(end.value()));
        filterSecondsBuilder << LTE("samples.samples.timestamp", ISODate(end.value()));
    }

    bool filterTime = start || end;

    if (reverse)
    {
        filterSortBuilder << "timestamp" << -1;
    }
    else
    {
        filterSortBuilder << "timestamp" << 1;
    }

    mongocxx::pipeline stages;
    stages.match(filterHoursBuilder.view());
    stages.unwind("$samples");

    if (filterTime)
    {
        stages.match(filterMinutesBuilder.view());
    }

    stages.unwind("$samples.samples");

    if (filterTime)
    {
        stages.match(filterSecondsBuilder.view());
    }

    stages.project(filterPropertyBuilder.view());
    stages.group(filterGroupBuilder.view());
    stages.sort(filterSortBuilder.view());

    if (limit)
    {
        if (skip)
        {
            limit.setValue(limit.value() + skip.value());
        }
        stages.limit(limit.value());
    }

    if (skip)
    {
        stages.skip(skip.value());
    }

    mongocxx::collection coll = (*pClient)[database][collection];

    return coll.aggregate(stages);
}
/* $end */
corto_resultIter _mongo_Historian_onRequest(
    mongo_Historian _this,
    corto_request *request)
{
/* $begin(recorto/mongo_connector/mongo/Historian/onRequest) */
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

    mongo_iterData *pData = nullptr;

    if (parent == ".")
    {
        parent = "/";
    }

    StrToLower(parent);
    StrToLower(expr);

    mongo_Connector _base = mongo_Connector(_this);
    CMongoPool *pPool = (CMongoPool*) _base->mongo_handle;

    try
    {
        MongoClientPtr pClient = pPool->GetClient();
        std::string database = SAFE_STRING(_base->dbname);

        auto res = mongo_Historian_find(pClient, database, parent, expr);

        pData = mongo_iterDataNew(std::move(pClient),
                                  std::move(res),
                                  std::move(database),
                                  std::move(parent),
                                  std::move(start),
                                  std::move(end),
                                  std::move(skip),
                                  std::move(limit),
                                  reverse);
    }
    catch (std::exception &e)
    {
        corto_error("%s", e.what());
    }

    result.udata = pData;
    result.hasNext = mongo_iterDataHasNext;
    result.next = mongo_iterDataNext;
    result.release = mongo_iterDataRelease;

    return result;
/* $end */
}
