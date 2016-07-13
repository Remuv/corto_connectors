/* $CORTO_GENERATED
 *
 * MongoCxx.cpp
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include <test.h>

/* $header() */
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <iostream>
/* $end */

corto_void _test_MongoCxx_Connect(
    test_MongoCxx _this)
{
/* $begin(test/MongoCxx/Connect) */

  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{}};

  bsoncxx::builder::stream::document document{};

  auto collection = conn["testdb"]["testcollection"];
  document << "hello" << "world";

  collection.insert_one(document.view());
  auto cursor = collection.find({});

  for (auto&& doc : cursor) {
      std::cout << bsoncxx::to_json(doc) << std::endl;
  }

/* $end */
}
