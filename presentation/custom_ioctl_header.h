
#define MAGIC 255
#define WRITE_VALUE _IOW(MAGIC, 'a' , int32_t*)
#define READ_VALUE  _IOW(MAGIC ,'b' , int32_t*)
#define REG_CURRENT_TASK _IOW(MAGIC,'c',int32_t*)
#define CREATE_TASKLET _IOW(MAGIC,'d',int32_t*)
#define CREATE_TIMER _IOW(MAGIC,'e',int32_t*)

