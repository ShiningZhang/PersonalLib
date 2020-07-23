#include "redis_util.h"

#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"

namespace acl_redis
{

__attribute__((unused)) static bool redis_del_(acl::redis& cmd, const char * key, size_t len)
{
	cmd.clear();

	int ret = cmd.del_one(key, len);
	if (ret < 0)
	{
		printf("del key: %s error: %s\r\n",
			key, cmd.result_error());
		return false;
	}
	printf("del ok, key: %s\r\n", key);
	return true;
}

__attribute__((unused)) static bool redis_expire_(acl::redis& cmd, const char * key,  int timeout)
{
	cmd.clear();

	if (cmd.expire(key, timeout) < 0)
	{
		printf("expire key: %s error: %s\r\n",
			key, cmd.result_error());
		return false;
	}
	printf("expire ok, key: %s\r\n", key);
	return true;
}

__attribute__((unused)) static bool redis_exists_(acl::redis& cmd, const char * key)
{
	cmd.clear();

	if (cmd.exists(key) == false)
	{
		printf("no exists key: %s\r\n", key);
	}
	else
	{
		printf("exists key: %s\r\n", key);
	}
	return true;
}

__attribute__((unused)) static bool redis_set_(acl::redis& cmd, const char * key, size_t key_len, const char * value, size_t value_len)
{
	cmd.clear();

	bool ret = cmd.set(key, key_len, value, value_len);
	printf("set key: %s, value: %s %s\r\n", key,
			value, ret ? "ok" : "error");
	return ret;
}

__attribute__((unused)) static bool redis_setex_(acl::redis& cmd, const char * key, size_t key_len, const char * value, size_t value_len, int timeout)
{
	cmd.clear();

	bool ret = cmd.setex(key, key_len, value, value_len, timeout);
	printf("setex key: %s, value: %s %s\r\n", key,
			value, ret ? "ok" : "error");
	return ret;
}

__attribute__((unused)) static bool redis_get_(acl::redis& cmd, const char * key, size_t key_len, char *& value, size_t &value_len)
{
	cmd.clear();
	bool ret = false;
	int length = 0;
	value_len = 0;
	value = NULL;

	const acl::redis_result* result = cmd.get(key, key_len);

	if (result != NULL)
	{
		value_len = result->get_length() + 1;
		if (value_len > 1)
		{
			value = (char *)malloc(value_len);
			length = result->argv_to_string(value, value_len);
			--value_len;
			if ((int) value_len == length)
				ret = true;
			else
			{
				printf("get key: %s failed because length(%d) != value_len(%d).\n", key, length, (int) value_len);
				free(value);
				value = NULL;
			}
		}
		else
		{
			printf("get key: %s failed because value_len(%d) < 1.\n", key, (int) value_len);
		}
	}

	printf("get key: %s, value: %s, len: %d\r\n",
			key, ret ? "ok" : "error",
			(int) value_len);

	return ret;
}

acl::redis_client_cluster *redis_util::cluster_ = NULL;

redis_util::redis_util(): 
			cmd_(NULL)
{
	cmd_ = new acl::redis();
}

redis_util::~redis_util()
{
	if (cmd_ != NULL)
	{
		delete cmd_;
		cmd_ = NULL;
	}
}

void redis_util::init( const char *addrs, int retry_time, int retry_max, int redirect_sleep, int max_threads, int conn_timeout, int rw_timeout)
{
	acl::acl_cpp_init();
	static acl::redis_client_cluster cluster;
	cluster_ = &cluster;
	// 当某个连接池结点出问题，设置探测该连接结点是否恢复的时间间隔(秒)，当该值
	// 为 0 时，则不检测
	cluster.set_retry_inter(retry_time);

	// 设置重定向的最大阀值，若重定向次数超过此阀值则报错
	cluster.set_redirect_max(retry_max);

	// 当重定向次数 >= 2 时每次再重定向此函数设置休息的时间(毫秒)
	cluster.set_redirect_sleep(redirect_sleep);

	cluster.init(NULL, addrs, max_threads, conn_timeout, rw_timeout);

	// 是否需要将所有哈希槽的对应关系提前设置好，这样可以去掉运行时动态添加
	// 哈希槽的过程，从而可以提高运行时的效率
	acl::string str_addrs(addrs);
	const std::vector<acl::string>& token = str_addrs.split2(",; \t");
	cluster.set_all_slot(token[0], max_threads);
}

bool redis_util::redis_del(const char * key, unsigned int len)
{
	return redis_del_(*cmd_, key, len);
}

bool redis_util::redis_expire(const char * key,  int timeout)
{
	return redis_expire_(*cmd_, key, timeout);
}

bool redis_util::redis_exists(const char * key)
{
	return redis_exists_(*cmd_, key);
}

bool redis_util::redis_set(const char * key, unsigned int key_len, const char * value, unsigned int value_len)
{
	return redis_set_(*cmd_, key, key_len, value, value_len);
}

bool redis_util::redis_setex(const char * key, unsigned int key_len, const char * value, unsigned int value_len, int timeout)
{
	return redis_setex_(*cmd_, key, key_len, value, value_len, timeout);
}

bool redis_util::redis_get(const char * key, unsigned int key_len, char *& value, unsigned int &value_len)
{
	size_t value_len_ = 0;
	bool ret = false;
	ret =  redis_get_(*cmd_, key, key_len, value, value_len_);
	value_len = value_len_;
	return ret;
}


} // namespace acl_redis

using namespace acl_redis;
int main(int argc, char* argv[])
{
	int  ch, conn_timeout = 10, rw_timeout = 10;
	int  max_threads = 10, nsleep = 500, nretry = 10;
	acl::string addrs("10.141.177.28:7010"), cmd;
	bool preset = false;

	while ((ch = getopt(argc, argv, "s:n:C:I:c:a:w:r:p")) > 0)
	{
		switch (ch)
		{
		case 's':
			addrs = optarg;
			break;
		case 'C':
			conn_timeout = atoi(optarg);
			break;
		case 'I':
			rw_timeout = atoi(optarg);
			break;
		case 'c':
			max_threads = atoi(optarg);
			break;
		case 'a':
			cmd = optarg;
			break;
		case 'w':
			nsleep = atoi(optarg);
			break;
		case 'r':
			nretry = atoi(optarg);
			break;
		case 'p':
			preset = true;
			break;
		default:
			break;
		}
	}

	acl::acl_cpp_init();

	acl::redis_client_cluster cluster;

	// ��ĳ�����ӳؽ������⣬����̽������ӽ���Ƿ�ָ���ʱ����(��)������ֵ
	// Ϊ 0 ʱ���򲻼��
	cluster.set_retry_inter(1);

	// �����ض�������ֵ�����ض�����������˷�ֵ�򱨴�
	cluster.set_redirect_max(nretry);

	// ���ض������ >= 2 ʱÿ�����ض���˺���������Ϣ��ʱ��(����)
	cluster.set_redirect_sleep(nsleep);

	cluster.init(NULL, addrs.c_str(), max_threads, conn_timeout, rw_timeout);

	// �Ƿ���Ҫ�����й�ϣ�۵Ķ�Ӧ��ϵ��ǰ���úã���������ȥ������ʱ��̬���
	// ��ϣ�۵Ĺ��̣��Ӷ������������ʱ��Ч��
	if (preset)
	{
		const std::vector<acl::string>& token = addrs.split2(",; \t");
		cluster.set_all_slot(token[0], max_threads);
	}

	struct timeval begin, end;
	long cost = 0;
	gettimeofday(&begin, NULL);

    acl::redis cmd_;
	cmd_.set_cluster(&cluster, max_threads);

	gettimeofday(&end, NULL);
	cost = (end.tv_sec-begin.tv_sec)*1000 + (end.tv_usec-begin.tv_usec)/1000;
	printf("cost1=%ldms\n", cost);
	gettimeofday(&begin, NULL);

	acl::string key("key1");
	acl::string value("acl_redis2");
	int timeout = 5;
	bool ret = redis_setex_(cmd_, key.c_str(), key.size(), value.c_str(), value.size(), timeout);
	printf("redis_set=%d\n", ret);

	gettimeofday(&end, NULL);
	cost = (end.tv_sec-begin.tv_sec)*1000 + (end.tv_usec-begin.tv_usec)/1000;
	printf("cost2=%ldms\n", cost);
	gettimeofday(&begin, NULL);
	
	char * get_value;
	size_t value_len;
	ret = redis_get_(cmd_, key.c_str(), key.size(), get_value, value_len);
	printf("redis_get=%s\n", get_value);

	gettimeofday(&end, NULL);
	cost = (end.tv_sec-begin.tv_sec)*1000 + (end.tv_usec-begin.tv_usec)/1000;
	printf("cost3=%ldms\n", cost);
	gettimeofday(&begin, NULL);

	sleep(timeout);
	ret = redis_get_(cmd_, key.c_str(), key.size(), get_value, value_len);
	printf("redis_get=%s\n", get_value);

	return 0;
}

