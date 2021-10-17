#define CMD_SET 1
#define CMD_GET 2
#define CMD_DELETE 3


struct stru_task
{
  int cmd;
  void *data;
};

struct stru_task_set
{
  char *key;
  void *data;
};

struct stru_task_get
{
  char *key;
  void *data;
};
