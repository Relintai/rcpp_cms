#include "http_server.h"

#include "core/http/web_application.h"
#include "bry_request.h"

#define LOG_VERBOSE 0

void HTTPServer::http_callback_handler(Request *request) {
	application->handle_request(request);
}

void HTTPServer::httpEnterCallbackDefault(const HTTPParser &httpParser, const HttpSession::Ptr &session) {
	std::unique_lock<std::mutex> lock(_request_map_mutex, std::defer_lock);

	BryRequest *request = BryRequest::get();

	HttpSession *s = session.get();

	lock.lock();
	_request_map[s] = request;
	lock.unlock();

	request->application = application;
	request->http_parser = std::make_shared<HTTPParser>(httpParser);
	request->session = session;

	request->setup_url_stack();

#if LOG_VERBOSE
	std::cout << "method:" << http_method_str(static_cast<http_method>(httpParser.method())) << std::endl;
#endif

	http_callback_handler(request);
}

void HTTPServer::wsEnterCallbackDefault(const HttpSession::Ptr &httpSession, WebSocketFormat::WebSocketFrameType opcode, const std::string &payload) {

	std::cout << "frame enter of type:" << int(opcode) << std::endl;
	std::cout << "payload is:" << payload << std::endl;
	// echo frame
	//auto frame = std::make_shared<std::string>();

	//WebSocketFormat::wsFrameBuild(payload.c_str(), payload.size(), *frame, WebSocketFormat::WebSocketFrameType::TEXT_FRAME, true, false);

	//httpSession->send(frame);
}

void HTTPServer::closedCallbackDefault(const HttpSession::Ptr &session) {
	HttpSession *s = session.get();

	std::unique_lock<std::mutex> lock(_request_map_mutex, std::defer_lock);

	lock.lock();
	Request *r = _request_map[s];

	if (r == nullptr) {
		lock.unlock();
		//printf("Error HTTPServer::closedCallbackDefault: r == nullptr!\n");

		return;
	}

	_request_map.erase(s);
	lock.unlock();

	r->connection_closed = true;
}

void HTTPServer::configure() {
}

void HTTPServer::initialize() {
	if (service)
		return;

	configure();

	service = TcpService::Create();
	service->startWorkerThread(threads);

	int p_port = port;

	//!
	if (listenBuilder)
		delete listenBuilder;

	listenBuilder = new HttpListenerBuilder();
	listenBuilder->WithService(service);

	listenBuilder->AddSocketProcess([](TcpSocket &socket) {
				socket.setNodelay();
				socket.setNonblock();
			});

	listenBuilder->WithAddr(false, "0.0.0.0", p_port);

	listenBuilder->WithEnterCallback([this](const HttpSession::Ptr &httpSession, HttpSessionHandlers &handlers) {
		handlers.setHttpCallback([this](const HTTPParser &httpParser, const HttpSession::Ptr &session){ this->httpEnterCallbackDefault(httpParser, session); });
		handlers.setWSCallback([this](const HttpSession::Ptr &httpSession, WebSocketFormat::WebSocketFrameType opcode, const std::string &payload){ this->wsEnterCallbackDefault(httpSession, opcode, payload); });
		handlers.setClosedCallback([this](const HttpSession::Ptr &session){ this->closedCallbackDefault(session); });
	});

	listenBuilder->asyncRun();
}

void HTTPServer::loop_once() {
	application->update();
}

void HTTPServer::main_loop() {
	while (true) {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		if (app_kbhit()) {
			break;
		}

		application->update();
	}
}

HTTPServer::HTTPServer() {
	port = 80;
	threads = 4;
	listenBuilder = nullptr;

	application = nullptr;
}

HTTPServer::~HTTPServer() {
	delete listenBuilder;
}
