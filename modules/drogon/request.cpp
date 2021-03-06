#include "request.h"

#include "web_application.h"

void DRequest::send() {
	//if (connection_closed) {
	//	DRequestPool::return_request(this);
	//	return;
	//}

	HttpResponsePtr response = HttpResponse::newHttpResponse();

	response->setBody(compiled_body);

	response->setExpiredTime(0);
	callback(response);

	pool();
}

void DRequest::send_file(const std::string &p_file_path) {
	HttpResponsePtr response = HttpResponse::newFileResponse(p_file_path, "", drogon::getContentType(p_file_path));

	callback(response);

	pool();
}

void DRequest::reset() {
	Request::reset();

	request.reset();

	//response = new HttpResponse();
}

std::string DRequest::parser_get_path() {
	return request->getPath();
}

void DRequest::update() {
	if (file_next) {
		file_next = false;
		_progress_send_file();
	}
}

DRequest *DRequest::get() {
	return _request_pool.get_request();
}
void DRequest::pool(DRequest *request) {
	return _request_pool.return_request(request);
}
void DRequest::pool() {
	DRequest::pool(this);
}

DRequest::DRequest() :
		Request() {

	//This value will need benchmarks, 2 MB seems to be just as fast for me as 4 MB, but 1MB is slower
	//It is a tradeoff on server memory though, as every active download will consume this amount of memory
	//where the file is bigger than this number
	file_chunk_size = 1 << 21; //2MB

	reset();
}

DRequest::~DRequest() {
}

void DRequest::_progress_send_file() {
	/*
	if (connection_closed) {
		DRequestPool::return_request(this);
		return;
	}

	if (current_file_progress >= file_size) {
		session->postShutdown();

		DRequestPool::return_request(this);

		return;
	}

	FILE *f = fopen(file_path.c_str(), "rb");

	if (!f) {
		printf("Error: Download: In progress file doesn't exists anymore! %s\n", file_path.c_str());

		application->unregister_request_update(this);

		session->postShutdown();

		DRequestPool::return_request(this);

		return;
	}

	fseek(f, current_file_progress, SEEK_SET);

	long nfp = current_file_progress + file_chunk_size;

	long csize = file_chunk_size;
	if (nfp >= file_size) {
		csize = (file_size - current_file_progress);
	}

	body.resize(csize);

	fread(&body[0], 1, csize, f);
	fclose(f);

	current_file_progress = nfp;

	session->send(body.c_str(), body.size(), [this]() { this->_file_chunk_sent(); });
	*/
}

void DRequest::_file_chunk_sent() {
	file_next = true;
}

RequestPool<DRequest> DRequest::_request_pool;