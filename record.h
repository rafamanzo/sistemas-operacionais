enum recordtype { SYSCALL_NO, ARG_INTEGER, ARG_POINTER, ARG_STRING, RET_VALUE };
#define MAX_STR_LEN (21)

struct record {
  enum recordtype type;
  union recordvalue {
    int intval;
    void *ptrval;
    char strval[MAX_STR_LEN];
  } value;
};

struct recordlist{
  struct record rec;
  struct recordlist *next;
};

typedef struct recordlist *reclist;

int startrecording();
int stoprecording();
int fetchrecords(struct record *records, int num_records);
int addrecordtolist(reclist *list, struct record rec);
reclist copyrecordslist(reclist list);
int fetchrecordslist(reclist list);
int releaserecordslist(reclist list);
