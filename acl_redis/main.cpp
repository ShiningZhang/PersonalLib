#include "redis_util.h"
#include <stdio.h>

using namespace acl_redis;
int main(int argc, char* argv[])
{
	/*
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
/*
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
	long int_key = 23412534265326352534;
	char * char_key = (char *)(&int_key);
	int timeout = 5;
	bool ret = redis_setex_(cmd_, char_key, sizeof(long), value.c_str(), value.size(), timeout);
	printf("redis_set=%d\n", ret);

	gettimeofday(&end, NULL);
	cost = (end.tv_sec-begin.tv_sec)*1000 + (end.tv_usec-begin.tv_usec)/1000;
	printf("cost2=%ldms\n", cost);
	gettimeofday(&begin, NULL);
	
	char * get_value;
	size_t value_len;
	ret = redis_get_(cmd_, char_key, sizeof(long), get_value, value_len);
	printf("redis_get=%s\n", get_value);

	gettimeofday(&end, NULL);
	cost = (end.tv_sec-begin.tv_sec)*1000 + (end.tv_usec-begin.tv_usec)/1000;
	printf("cost3=%ldms\n", cost);
	gettimeofday(&begin, NULL);

	sleep(timeout);
	ret = redis_get_(cmd_, key.c_str(), key.size(), get_value, value_len);
	printf("redis_get=%s\n", get_value);
*/
	acl_redis::redis_util::pre_init("10.141.177.28:7010");
	acl_redis::redis_util redis;
	redis.init();
	char *key_1="123";
	char* value_1="456";
	bool res=false;
	char *value = NULL;
	unsigned int value_len = 0;
	res=redis.redis_setex(key_1, 3, value_1, 3, 100);
	printf( "CQueryTask::svc:test set (%s)(%s) %d\n",key_1, value_1,res);
	res=redis.redis_get(key_1, 3, value, value_len);
	printf( "CQueryTask::svc:test get(%s)(%s)(%d) %d\n",key_1, value, value_len,res);
	return 0;
}

