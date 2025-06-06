/***************************************************************
 * Name:      huint4960.h
 * Purpose:   声明huint4960接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-23
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HUINT4960_H__
#define __HUINT4960_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define HUINT4960_BITS_COUNT 4960

typedef struct huint4960 huint4960_t;
struct huint4960
{
    uint32_t val[(HUINT4960_BITS_COUNT/(sizeof(uint32_t)*8))+((HUINT4960_BITS_COUNT%(sizeof(uint32_t)*8))!=0?1:0)];
};


/** \brief 从字节中加载大数
 *
 * \param num huint4960_t* 大数指针
 * \param bytes const uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4960_load(huint4960_t *num,const uint8_t *bytes,size_t bytes_count);

/** \brief 从字节中加载大数
 *
 * \param num huint4960_t* 大数指针
 * \param bytes const uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4960_load_be(huint4960_t *num,const uint8_t *bytes,size_t bytes_count);


/** \brief 从数字中加载大数
 *
 * \param num huint4960_t* 大数
 * \param uint32_num uint32_t 数字
 *
 */
void huint4960_load_uint32(huint4960_t *num,uint32_t uint32_num);

/** \brief 从数字中加载大数
 *
 * \param num huint4960_t* 大数
 * \param uint64_num uint64_t 数字
 *
 */
void huint4960_load_uint64(huint4960_t *num,uint64_t uint64_num);

/** \brief 保存大数到字节
 *
 * \param num huint4960_t* 大数指针
 * \param bytes uint8_t* 字节（小端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4960_store(huint4960_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数到字节
 *
 * \param num huint4960_t* 大数指针
 * \param bytes uint8_t* 字节（大端模式）
 * \param bytes_count size_t 字节长度
 *
 */
void huint4960_store_be(huint4960_t *num,uint8_t *bytes,size_t bytes_count);


/** \brief 保存大数数字
 *
 * \param num huint4960_t* 大数
 * \param uint32_num uint32_t* 数字
 *
 */
void huint4960_store_uint32(const huint4960_t *num,uint32_t* uint32_num);

/** \brief 保存大数数字
 *
 * \param num huint4960_t* 大数
 * \param uint64_num uint64_t* 数字
 *
 */
void huint4960_store_uint64(const huint4960_t *num,uint64_t* uint64_num);

/** \brief 复制
 *
 * \param dst huint4960_t* 目标大数
 * \param src const huint4960_t* 源大数
 *
 */
void huint4960_copy(huint4960_t *dst,const huint4960_t *src);

/** \brief 按位非
 *
 * \param dst huint4960_t* 目标大数
 * \param src const huint4960_t* 源大数
 *
 */
void huint4960_bitwise_not(huint4960_t *dst,const huint4960_t *src);

/** \brief 按位与
 *
 * \param dst huint4960_t* 目标大数
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_bitwise_and(huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);

/** \brief 按位或
 *
 * \param dst huint4960_t* 目标大数
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_bitwise_or(huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);

/** \brief 按位异或
 *
 * \param dst huint4960_t* 目标大数
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_bitwise_xor(huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);

/** \brief 比较
 *
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 * \return int 0=相等，1=源大数1大于源大数2,-1表示源大数2大于源大数1，-2=参数出错
 *
 */
int huint4960_compare(const huint4960_t *src1,const huint4960_t *src2);

/** \brief 求补码
 *
 * \param dst huint4960_t* 目标大数
 * \param src const huint4960_t* 源大数
 *
 */
void huint4960_complement(huint4960_t *dst,const huint4960_t *src);

/** \brief 左移
 *
 * \param dst huint4960_t* 目标大数
 * \param src const huint4960_t* 源大数
 * \param bits size_t 左移位数
 *
 */
void huint4960_left_shift(huint4960_t *dst,const huint4960_t *src,size_t bits);

/** \brief 右移
 *
 * \param dst huint4960_t* 目标大数
 * \param src const huint4960_t* 源大数
 * \param bits size_t 右移位数
 *
 */
void huint4960_right_shift(huint4960_t *dst,const huint4960_t *src,size_t bits);

/** \brief 设置位
 *
 * \param dst huint4960_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint4960_bit_set(huint4960_t *dst,size_t bits);

/** \brief 清除位
 *
 * \param dst huint4960_t* 目标大数
 * \param bits size_t 位
 *
 */
void huint4960_bit_clear(huint4960_t *dst,size_t bits);

/** \brief 获取位
 *
 * \param dst huint4960_t* 目标大数
 * \param bits size_t 位
 * \return bool 位上的值
 *
 */
bool huint4960_bit(const huint4960_t *dst,size_t bits);

/** \brief 前导0的个数
 *
 * \param dst huint4960_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint4960_clz(const huint4960_t *dst);

/** \brief 尾部0的个数
 *
 * \param dst huint4960_t* 目标大数
 * \return size_t 前导0的个数
 *
 */
size_t huint4960_ctz(const huint4960_t *dst);

/** \brief 加
 *
 * \param dst huint4960_t* 目标大数
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_add(huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);


/** \brief 减
 *
 * \param dst huint4960_t* 目标大数,dst=src1-src2
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_sub(huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);

/** \brief 乘
 *
 * \param state huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4960_t* 目标大数,dst=src1*src2
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_mul(huint4960_t *state,huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);


/** \brief 除
 *
 * \param state huint4960_t* 状态值，存储余数。state=src1%src2
 * \param state1 huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4960_t* 目标大数,dst=src1/src2
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_div(huint4960_t *state,huint4960_t *state1,huint4960_t *state2,huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);


/** \brief 幂函数
 *
 * \param state huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4960_t* 目标大数,dst=src1的src2次方
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 *
 */
void huint4960_power(huint4960_t *state,huint4960_t *state1,huint4960_t *state2,huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2);


/** \brief 幂取模函数（常用于RSA等加密算法）
 *
 * \param state huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param state1 huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param state2 huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param state3 huint4960_t* 状态值，用于中间状态存储,不可为空
 * \param dst huint4960_t* 目标大数,dst=src1的src2次方对src3取模
 * \param src1 const huint4960_t* 源大数1
 * \param src2 const huint4960_t* 源大数2
 * \param src3 const huint4960_t* 源大数3
 *
 */
void huint4960_power_mod(huint4960_t *state,huint4960_t *state1,huint4960_t *state2,huint4960_t *state3,huint4960_t *dst,const huint4960_t *src1,const huint4960_t *src2,const huint4960_t *src3);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HUINT4960_H__
