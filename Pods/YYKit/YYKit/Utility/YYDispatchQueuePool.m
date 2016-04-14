//
//  YYDispatchQueueManager.m
//  YYKit <https://github.com/ibireme/YYKit>
//
//  Created by ibireme on 15/7/18.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import "YYDispatchQueuePool.h"
#import <UIKit/UIKit.h>
#import <libkern/OSAtomic.h>

#define MAX_QUEUE_COUNT 32

static inline dispatch_queue_priority_t NSQualityOfServiceToDispatchPriority(NSQualityOfService qos) {
    switch (qos) {
        case NSQualityOfServiceUserInteractive: return DISPATCH_QUEUE_PRIORITY_HIGH;
        case NSQualityOfServiceUserInitiated: return DISPATCH_QUEUE_PRIORITY_HIGH;
        case NSQualityOfServiceUtility: return DISPATCH_QUEUE_PRIORITY_LOW;
        case NSQualityOfServiceBackground: return DISPATCH_QUEUE_PRIORITY_BACKGROUND;
        case NSQualityOfServiceDefault: return DISPATCH_QUEUE_PRIORITY_DEFAULT;
        default: return DISPATCH_QUEUE_PRIORITY_DEFAULT;
    }
}

/**
 *  创建一个 qos_class_t 也就是 dispatch_qos_class_t
 *
 *  @param qos 线程类型
 *
 *  @return 返回一个 qos_class_t
 */
static inline qos_class_t NSQualityOfServiceToQOSClass(NSQualityOfService qos) {
    switch (qos) {
        case NSQualityOfServiceUserInteractive: return QOS_CLASS_USER_INTERACTIVE;
        case NSQualityOfServiceUserInitiated: return QOS_CLASS_USER_INITIATED;
        case NSQualityOfServiceUtility: return QOS_CLASS_UTILITY;
        case NSQualityOfServiceBackground: return QOS_CLASS_BACKGROUND;
        case NSQualityOfServiceDefault: return QOS_CLASS_DEFAULT;
        default: return QOS_CLASS_UNSPECIFIED;
    }
}

typedef struct {
    const char *name;
    void **queues;  //一个放 队列 的 数组
    uint32_t queueCount;
    int32_t counter;
} YYDispatchContext;

/**
 *  创建 一个 YYDispatchContext 对象
 *
 *  @param name       队列的名字
 *  @param queueCount 队列数量
 *  @param qos        队列类型
 *
 *  @return YYDispatchContext
 */
static YYDispatchContext *YYDispatchContextCreate(const char *name,
                                                 uint32_t queueCount,
                                                 NSQualityOfService qos) {
    
    YYDispatchContext *context = calloc(1, sizeof(YYDispatchContext));
    
    
    if (!context) return NULL;
    
    //分配 queueCount 个连续 大小为 sizeof(void *) 的内存
    context->queues =  calloc(queueCount, sizeof(void *));
    
    if (!context->queues) {
        free(context);
        return NULL;
    }
    
    if ([UIDevice currentDevice].systemVersion.floatValue >= 8.0) {
        
        dispatch_qos_class_t qosClass = NSQualityOfServiceToQOSClass(qos);
        //按 队列数量 创建 队列
        for (NSUInteger i = 0; i < queueCount; i++) {
            //DISPATCH_QUEUE_SERIAL 或 NULL 串行队列     DISPATCH_QUEUE_CONCURRENT  并发
            dispatch_queue_attr_t attr = dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, qosClass, 0);
            dispatch_queue_t queue = dispatch_queue_create(name, attr);
            context->queues[i] = (__bridge_retained void *)(queue);
        }
    } else {
        long identifier = NSQualityOfServiceToDispatchPriority(qos);
        for (NSUInteger i = 0; i < queueCount; i++) {
            dispatch_queue_t queue = dispatch_queue_create(name, DISPATCH_QUEUE_SERIAL);
            dispatch_set_target_queue(queue, dispatch_get_global_queue(identifier, 0));
            context->queues[i] = (__bridge_retained void *)(queue);
        }
    }
    
    context->queueCount = queueCount;
    if (name) {
        //拷贝一个 字符串给 context -> name
         context->name = strdup(name);
    }
    return context;
}

static void YYDispatchContextRelease(YYDispatchContext *context) {
    if (!context) return;
    if (context->queues) {
        for (NSUInteger i = 0; i < context->queueCount; i++) {
            void *queuePointer = context->queues[i];
            dispatch_queue_t queue = (__bridge_transfer dispatch_queue_t)(queuePointer);
            const char *name = dispatch_queue_get_label(queue);
            if (name) strlen(name); // avoid compiler warning
            queue = nil;
        }
        free(context->queues);
        context->queues = NULL;
    }
    if (context->name) free((void *)context->name);
}

static dispatch_queue_t YYDispatchContextGetQueue(YYDispatchContext *context) {
    int32_t counter = OSAtomicIncrement32(&context->counter);
    if (counter < 0) counter = -counter;
    void *queue = context->queues[counter % context->queueCount];
    return (__bridge dispatch_queue_t)(queue);
}


/**
 *  根据 NSQualityOfService 创建一个 YYDispatchContext
 *
 *  @param qos
 *
 *  @return 返回一个 YYDispatchContext
 */
static YYDispatchContext *YYDispatchContextGetForQOS(NSQualityOfService qos) {
    static YYDispatchContext *context[5] = {0};
    switch (qos) {
            //与用户交互的任务，这些任务通常跟UI级别的刷新相关，比如动画，这些任务需要在一瞬间完成
        case NSQualityOfServiceUserInteractive: {
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                //可用的处理 的 核数
                int count = (int)[NSProcessInfo processInfo].activeProcessorCount;
                count = count < 1 ? 1 : count > MAX_QUEUE_COUNT ? MAX_QUEUE_COUNT : count; //如果不够1 就按1 如果超过32 就按32
                context[0] = YYDispatchContextCreate("com.ibireme.yykit.user-interactive", count, qos);
            });
            return context[0];
        } break;
            //由用户发起的并且需要立即得到结果的任务，比如滑动scroll view时去加载数据用于后续cell的显示，这些任务通常跟后续的用户交互相关，在几秒或者更短的时间内完成
        case NSQualityOfServiceUserInitiated: {
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                int count = (int)[NSProcessInfo processInfo].activeProcessorCount;
                count = count < 1 ? 1 : count > MAX_QUEUE_COUNT ? MAX_QUEUE_COUNT : count;
                context[1] = YYDispatchContextCreate("com.ibireme.yykit.user-initiated", count, qos);
            });
            return context[1];
        } break;
            //一些可能需要花点时间的任务，这些任务不需要马上返回结果，比如下载的任务，这些任务可能花费几秒或者几分钟的时间
        case NSQualityOfServiceUtility: {
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                int count = (int)[NSProcessInfo processInfo].activeProcessorCount;
                count = count < 1 ? 1 : count > MAX_QUEUE_COUNT ? MAX_QUEUE_COUNT : count;
                context[2] = YYDispatchContextCreate("com.ibireme.yykit.utility", count, qos);
            });
            return context[2];
        } break;
            //这些任务对用户不可见，比如后台进行备份的操作，这些任务可能需要较长的时间，几分钟甚至几个小时
        case NSQualityOfServiceBackground: {
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                int count = (int)[NSProcessInfo processInfo].activeProcessorCount;
                count = count < 1 ? 1 : count > MAX_QUEUE_COUNT ? MAX_QUEUE_COUNT : count;
                context[3] = YYDispatchContextCreate("com.ibireme.yykit.background", count, qos);
            });
            return context[3];
        } break;
            //优先级介于user-initiated 和 utility，当没有 QoS信息时默认使用，开发者不应该使用这个值来设置自己的任务
        case NSQualityOfServiceDefault:
        default: {
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                int count = (int)[NSProcessInfo processInfo].activeProcessorCount;
                count = count < 1 ? 1 : count > MAX_QUEUE_COUNT ? MAX_QUEUE_COUNT : count;
                context[4] = YYDispatchContextCreate("com.ibireme.yykit.default", count, qos);
            });
            return context[4];
        } break;
    }
}


@implementation YYDispatchQueuePool {
    @public
    YYDispatchContext *_context;
}

- (void)dealloc {
    if (_context) {
        YYDispatchContextRelease(_context);
        _context = NULL;
    }
}

- (instancetype)initWithContext:(YYDispatchContext *)context {
    self = [super init];
    if (!context) return nil;
    self->_context = context;
    _name = context->name ? [NSString stringWithUTF8String:context->name] : nil;
    return self;
}

- (instancetype)initWithName:(NSString *)name queueCount:(NSUInteger)queueCount qos:(NSQualityOfService)qos {
    if (queueCount == 0 || queueCount > MAX_QUEUE_COUNT) return nil;
    self = [super init];
    _context = YYDispatchContextCreate(name.UTF8String, (uint32_t)queueCount, qos);
    if (!_context) return nil;
    _name = name;
    return self;
}

- (dispatch_queue_t)queue {
    return YYDispatchContextGetQueue(_context);
}

+ (instancetype)defaultPoolForQOS:(NSQualityOfService)qos {
    switch (qos) {
        case NSQualityOfServiceUserInteractive: {
            static YYDispatchQueuePool *pool;
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                pool = [[YYDispatchQueuePool alloc] initWithContext:YYDispatchContextGetForQOS(qos)];
            });
            return pool;
        } break;
        case NSQualityOfServiceUserInitiated: {
            static YYDispatchQueuePool *pool;
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                pool = [[YYDispatchQueuePool alloc] initWithContext:YYDispatchContextGetForQOS(qos)];
            });
            return pool;
        } break;
        case NSQualityOfServiceUtility: {
            static YYDispatchQueuePool *pool;
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                pool = [[YYDispatchQueuePool alloc] initWithContext:YYDispatchContextGetForQOS(qos)];
            });
            return pool;
        } break;
        case NSQualityOfServiceBackground: {
            static YYDispatchQueuePool *pool;
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                pool = [[YYDispatchQueuePool alloc] initWithContext:YYDispatchContextGetForQOS(qos)];
            });
            return pool;
        } break;
        case NSQualityOfServiceDefault:
        default: {
            static YYDispatchQueuePool *pool;
            static dispatch_once_t onceToken;
            dispatch_once(&onceToken, ^{
                pool = [[YYDispatchQueuePool alloc] initWithContext:YYDispatchContextGetForQOS(NSQualityOfServiceDefault)];
            });
            return pool;
        } break;
    }
    
}

@end

dispatch_queue_t YYDispatchQueueGetForQOS(NSQualityOfService qos) {
    return YYDispatchContextGetQueue(YYDispatchContextGetForQOS(qos));
}
