#pragma once

namespace acl{
	class redis;
	class redis_client_cluster;
}

namespace acl_redis{

class redis_util{
public:
	redis_util();

	virtual ~redis_util();

	void init(const char *addrs,
					int retry_time = 10,
					int retry_max = 10,
					int redirect_sleep = 500, //ms
					int max_threads = 10,
					int conn_timeout = 5, //s
					int rw_timeout = 5); //s

	bool redis_del(const char * key, unsigned int len);

	bool redis_expire(const char * key,  int timeout);

	bool redis_exists(const char * key);

	bool redis_set(const char * key, unsigned int key_len, const char * value, unsigned int value_len);

	bool redis_setex(const char * key, unsigned int key_len, const char * value, unsigned int value_len, int timeout);

	bool redis_get(const char * key, unsigned int key_len, char *& value, unsigned int &value_len);

public:
	acl::redis *cmd_;
	acl::redis_client_cluster *cluster_;
};
}
