#ifndef BRAGI_DEEZER_CLIENT_H
#define BRAGI_DEEZER_CLIENT_H

#include "player/track/deezer_track.h"

#include <string>

class DeezerClient final {
public:
	enum Quality : char { MP3_128, MP3_320, FLAC };

	static void Init();

	static DeezerTrack* Search(const std::string &query, unsigned int start = 0);

	static std::string GetEncodedTrackUrl(const std::string &token, Quality quality = FLAC);

private:
	static constexpr char QUALITY_STR[3][8] = { "MP3_128", "MP3_320", "FLAC" };

	static inline std::string _arl_token;
	static inline std::string _headers;

	static inline std::string _session_id;
	static inline std::string _license_token;

	static inline unsigned long _session_timestamp;
	static constexpr unsigned int DELTA_TIME = 600;

	static constexpr char BASIC_HEADERS[] = "Accept: application/json, text/plain, */*\n"
	                                        "Content-Type: text/plain;charset=UTF-8\n"
	                                        "User-Agent: Deezer/7.17.0.2 CFNetwork/1098.6 Darwin/19.0.0\n"
	                                        "Cache-Control: max-age=0\n"
	                                        "Accept-Language: en-US,en;q=0.9,en-US;q=0.8,en;q=0.7\n"
	                                        "Accept-Charset: utf-8,ISO-8859-1;q=0.8,*;q=0.7\n"
	                                        "Cookie: arl=";
	static constexpr char URL_UPDATE_SESSION[] = "www.deezer.com/ajax/gw-light.php?version=8.32.0"
	                                             "&api_key=ZAIVAHCEISOHWAICUQUEXAEPICENGUAFAEZAIPHAELEEVAHPHUCUFONGUAPASUAY&output=3&input=3"
	                                             "&buildId=ios12_universal&screenHeight=480&screenWidth=320&lang=en&method=deezer.getUserData"
	                                             "&api_version=1.0&api_token";
	static constexpr char URL_TEMPLATE_SEARCH_TRACK[] = "api.deezer.com/1.0/gateway.php?"
	                                                    "api_key=ZAIVAHCEISOHWAICUQUEXAEPICENGUAFAEZAIPHAELEEVAHPHUCUFONGUAPASUAY&output=3&input=3"
	                                                    "&method=search.music&sid=";
	static constexpr char BODY_TEMPLATE_SEARCH_TRACK[] = "{{\"query\":\"{}\",\"nb\":1,\"output\":\"TRACK\",\"filter\":\"TRACK\",\"start\":{}}}";
	static constexpr char URL_TEMPLATE_GET_DECODED_TRACK_URL[] = "media.deezer.com/v1/get_url?version=8.32.0"
	                                                             "&api_key=ZAIVAHCEISOHWAICUQUEXAEPICENGUAFAEZAIPHAELEEVAHPHUCUFONGUAPASUAY&output=3&input=3"
	                                                             "&buildId=ios12_universal&screenHeight=480&screenWidth=320&lang=en&sid=";
	static constexpr char BODY_TEMPLATE_GET_DECODED_TRACK_URL[] = "{{\"license_token\":\"{}\",\"media\":[{{\"type\":\"FULL\",\"formats\":[{{\"format\":"
	                                                              "\"{}\",\"cipher\":\"BF_CBC_STRIPE\"}}]}}],\"track_tokens\":[\"{}\"]}}";

	static inline std::string _url_search_track;
	static inline std::string _url_get_decoded_track_url;

	/** Update the deezer session
	 * @param verbose true if we need to log the user data; false else
	 */
	static void UpdateSession(bool verbose = false);
};

#endif