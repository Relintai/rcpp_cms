#ifndef MYSQL_QUERY_BUILDER_H
#define MYSQL_QUERY_BUILDER_H

#include "core/query_builder.h"

#include <memory>
#include <string>

class MysqlQueryBuilder : public QueryBuilder {
public:
/*
	QueryBuilder *select(const std::string &params);
	QueryBuilder *where(const std::string &params);
	QueryBuilder *from(const std::string &params);
	QueryBuilder *limit(const int min, const int max);
	QueryBuilder *insert(const std::string &table_name, const std::string &params_str);
*/
    MysqlQueryBuilder();
    ~MysqlQueryBuilder();
};

#endif