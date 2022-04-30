#ifndef SPOA_REDIS_H
#define SPOA_REDIS_H

#include <hiredis/hiredis.h>

// extern redisContext *redis_context;
int init_redis_context(redisContext **redis_context_p, const char *ip, int port, char *password);
void free_redis_context(redisContext *redis_context);

int spoa_redis_check(redisContext *redis_context, char *key);
int spoa_redis_update(redisContext *redis_context, char *key);
int spoa_redis_check_then_update(redisContext *redis_context, char *key);

#endif