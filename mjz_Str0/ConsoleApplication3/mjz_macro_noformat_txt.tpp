#define MJZ_LINE_AS_STR__(X) (#X)
#define MJZ_LINE_AS_STR_(X) (MJZ_LINE_AS_STR__(X))
#define DEBUG_MJZ_ENTITY_()                                       auto DEBUG_MJZ_ENTITY_OBJ = update_tm(MJZ_LINE_AS_STR_(started runing function at file : __FILE__ at line : __LINE__),MJZ_LINE_AS_STR_(ended runing function at file : __FILE__ at line : __LINE__))
// this is not c++ its just text
