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

	static void init(const char *addrs, int retry_time, int retry_max, int redirect_sleep, int max_threads, int conn_timeout, int rw_timeout);

	bool redis_del(const char * key, unsigned int len);

	bool redis_expire(const char * key,  int timeout);

	bool redis_exists(const char * key);

	bool redis_set(const char * key, unsigned int key_len, const char * value, unsigned int value_len);

	bool redis_setex(const char * key, unsigned int key_len, const char * value, unsigned int value_len, int timeout);

	bool redis_get(const char * key, unsigned int key_len, char *& value, unsigned int &value_len);

public:
	acl::redis *cmd_;
	static acl::redis_client_cluster *cluster_;
};
}
