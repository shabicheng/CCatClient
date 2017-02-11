#include "CatMessigeIdHelper.h"
#include "ZRMultiThreadUtility.h"
#include "CatClientConfig.h"
#include "CatMessageManager.h"
#include "TimeUtility.h"

extern CatMessageManager g_cat_messageManager;

static volatile unsigned long long g_last_hour = 0;
static volatile int g_next_start = 10000;
static volatile int g_id_index = 0;
static sds g_index_filePath = NULL;
static ZRCRITICALSECTION g_id_lock = NULL;
static sds g_id_prefix = NULL; // 默认为"%s-%s-%ll-", g_cat_messageManager.m_domain, g_cat_messageManager.m_ipX, g_last_hour

static void save() {
    FILE *file = fopen(g_index_filePath, "w");
    if (file == NULL)
    {
        return;
    }
    fprintf(file, "%ll %d", g_last_hour, g_next_start);
    fclose(file);
}

static void load() {
    FILE *file = fopen(g_index_filePath, "r");
    if (file == NULL) return;
    fscanf(file, "%ll %d", &g_last_hour, &g_next_start);
    fclose(file);
}

void initMessageIdHelper()
{
    g_index_filePath = sdscatsds(g_config.dataDir, g_config.indexFileName);
    g_id_lock = ZRCreateCriticalSection();
    load();
    unsigned long long nowT = GetTime64();
    unsigned long long nowHour = catTrimToHour(nowT);
    if (nowHour > g_last_hour)
    {
        g_last_hour = nowHour;
        g_next_start = 10000;
        g_id_index = 0;
        save();
    }
    else
    {
        g_id_index = g_next_start;
        g_next_start = g_next_start + 10000;
        save();

    }
    // 在初始化的时候就创建出来
    g_id_prefix = sdsnewlen(NULL, 64);
    g_id_prefix = sdscatprintf(g_id_prefix, "%s-%s-%ll-", g_cat_messageManager.m_domain, g_cat_messageManager.m_ipX, g_last_hour);

}

void flushMessageIdMark()
{
    save();
}

sds getNextMessageId()
{
    int tmpId = 0;
    int saveFlag = 0;

    ZRCS_ENTER(g_id_lock);

    unsigned long long nowT = GetTime64();
    unsigned long long nowHour = catTrimToHour(nowT);
    // 在小时改变的时候需要更新g_id_prefix
    if (nowHour > g_last_hour) 
    {
        g_last_hour = nowHour;
        g_next_start = 10000;
        g_id_index = 0;
        saveFlag = 1;
        g_id_prefix = sdscatprintf(g_id_prefix, "%s-%s-%ll-", g_cat_messageManager.m_domain, g_cat_messageManager.m_ipX, g_last_hour);
    }

    ++g_id_index;
    if (g_id_index % 10000 == 0)
    {
        saveFlag = 1;
    }
    tmpId = g_id_index;


    ZRCS_LEAVE(g_id_lock);
    if (saveFlag)
    {
        save();
    }
    sds msgIdStr = sdsnewlen(NULL, 128);
    return sdscatprintf(msgIdStr, "%s%d", g_id_index);
}

void reuseMessageId(sds msgId)
{
    // 暂时不做这个
    return;
}

