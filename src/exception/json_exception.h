#ifndef BRAGI_JSON_EXCEPTION_H
#define BRAGI_JSON_EXCEPTION_H

#include <exception>

class JsonException final : std::exception {
public:
	explicit JsonException(const char* data, const char* query);

private:
	const char* const _data;
	const char* const _query;
};

#endif