#include "database.h"

#include "query_builder.h"
#include "table_builder.h"
#include "query_result.h"

void Database::connect(const std::string &connection_str) {
}

QueryResult *Database::query(const std::string &query) {
	return nullptr;
}
void Database::query_run(const std::string &query) {
}

QueryBuilder *Database::get_query_builder() {
	return new QueryBuilder();
}

TableBuilder *Database::get_table_builder() {
	return new TableBuilder();
}

std::string Database::escape(const std::string str) {
	return std::string();
}

void Database::escape(const std::string str, std::string *to) {

}

Database::Database() {
}

Database::~Database() {
}