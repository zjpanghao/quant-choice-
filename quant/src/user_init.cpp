#include "kafka_client.h"
#include "glog/logging.h"
#include "user_app.h"

enum TeleCommand{
	INVALID_COMMAND = 0,
	KAFKA_COMMAND = 1,
	THREAD_NUM_COMMAND = 3,
        DELAY_MIN_CMD = 4,
        TOPIC_COMMAND = 5
};

static int kafka_init(wrapper_Info *test_info, const char* kafka_server, const char *topic);
static void glog_init(std::string name); 
 
int user_init(int argc, char *argv[]) {
  TeleCommand cmd = INVALID_COMMAND; 
  const char *kafka_server = NULL;
  const char *topic = NULL;
  glog_init(argv[0]);
  for (int i = 0; i < argc; i++) {
    switch (cmd) {
      case KAFKA_COMMAND:
        kafka_server = argv[i];
        break;
      case THREAD_NUM_COMMAND:
	break;
      case DELAY_MIN_CMD:
	break;
      case TOPIC_COMMAND:
        topic = argv[i];
        break;
    }
    cmd = INVALID_COMMAND;
    if (strcmp(argv[i], "-k") == 0) {
      cmd = KAFKA_COMMAND;
    } 
    if (strcmp(argv[i], "-p") == 0) {
      cmd = THREAD_NUM_COMMAND;
    }
    if (strcmp(argv[i], "-m") == 0) {
      cmd = DELAY_MIN_CMD;
    }
    if (strcmp(argv[i], "-t") == 0) {
      cmd = TOPIC_COMMAND;
    }
  }

  if (kafka_server == NULL || topic == NULL)
    return -1;
  wrapper_Info * test_info = new wrapper_Info;
  if (0 != kafka_init(test_info, kafka_server, topic)) {
    delete test_info;
    return -2;
  }
  set_wrapper_info(test_info);
  return 0;
}

void glog_init(std::string name) {
  google::InitGoogleLogging(name.c_str());
  if (access("telelog", 0) != 0)
    system("mkdir telelog");
  google::SetLogDestination(google::INFO, "telelog/info");	
  google::SetLogDestination(google::WARNING, "telelog/warn");   
  google::SetLogDestination(google::ERROR, "telelog/error");   
  FLAGS_logbufsecs = 10;
}

static int kafka_init(wrapper_Info *test_info, const char* kafka_server, const char *topic) {
  int partition = 0;
  /*init kafka*/
  if (PRODUCER_INIT_SUCCESS == producer_init(partition, topic, kafka_server, &test_msg_delivered, test_info)) {
    LOG(INFO) << "producer init success!";
    return 0;
  } else {
    LOG(ERROR) << "producer init failed!";
    return -1;
  }
  return 0;
}
