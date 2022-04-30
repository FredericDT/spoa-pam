#include "spoa_redis.h"
#include <hiredis/hiredis.h>

#include <stdio.h>
#include <string.h>

// redisContext *redis_context;

int KEY_TTL = 1200; // TODO

int init_redis_context(redisContext **redis_context_p, const char *ip, int port, char *password) {
    *redis_context_p = redisConnect(ip, port);
    redisContext *redis_context = *redis_context_p;
    if (redis_context != NULL && redis_context->err) {
        fprintf(stderr, "Error: %s\n", redis_context->errstr);
        return 0;
    }

    if (password != NULL) {

        redisReply *reply;
        reply = redisCommand(redis_context, "AUTH %s", password);
        freeReplyObject(reply);
    }

    return 1;
}

void free_redis_context(redisContext *redis_context) {
    redisFree(redis_context);
}

int spoa_redis_check(redisContext *redis_context, char *key) {
    redisReply *reply;
    // printf("spoa_redis_check pre: %s\n", key);
    reply = redisCommand(redis_context, "EXISTS %s", key);
    // printf("spoa_redis_check post: %s, ret: %lld\n", key, reply->integer);
    int retval = reply->integer;
    freeReplyObject(reply);

    return retval;
}

int spoa_redis_check_then_update(redisContext *redis_context, char *key) {
    if (spoa_redis_check(redis_context, key)) {
        redisReply *reply;
        reply = redisCommand(redis_context, "EXPIRE %s %d", key, KEY_TTL);
        freeReplyObject(reply);
        return 1;
    }
    return 0;
}

int spoa_redis_update(redisContext *redis_context, char *key) {
    redisReply *reply;
    reply = redisCommand(redis_context, "SET %s %d", key, 1);
    freeReplyObject(reply);
    reply = redisCommand(redis_context, "EXPIRE %s %d", key, KEY_TTL);
    freeReplyObject(reply);
    return 1;
}
