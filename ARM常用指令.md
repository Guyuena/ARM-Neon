![img](https://pic1.zhimg.com/v2-7683227587c14a9c14679c09d2c0cb1c_r.jpg)



**https://developer.arm.com/architectures/instruction-sets/intrinsics/**

**ARM官方指令查询，基本的支持的指令都能查到其用法**



# ARM-Neon指令查询工具

https://developer.arm.com/architectures/instruction-sets/intrinsics/#f:@navigationhierarchiessimdisa=[Neon]

https://docs.unity3d.com/Packages/com.unity.burst@1.8/api/Unity.Burst.Intrinsics.Arm.Neon.html

https://arm-software.github.io/acle/neon_intrinsics/advsimd.html

https://doc.rust-lang.org/core/arch/aarch64/

https://doc.rust-lang.org/core/arch/arm/index.html



# SIMD指令查询工具

[Other architectures](https://doc.rust-lang.org/core/arch/#other-architectures)

This documentation is only for one particular architecture, you can find others at:

- [`x86`](https://doc.rust-lang.org/core/arch/x86/index.html)
- [`x86_64`](https://doc.rust-lang.org/core/arch/x86_64/index.html)
- [`arm`](https://doc.rust-lang.org/core/arch/arm/index.html)
- [`aarch64`](https://doc.rust-lang.org/core/arch/aarch64/index.html)
- [`riscv32`](https://doc.rust-lang.org/core/arch/riscv32/index.html)
- [`riscv64`](https://doc.rust-lang.org/core/arch/riscv64/index.html)
- [`mips`](https://doc.rust-lang.org/core/arch/mips/index.html)
- [`mips64`](https://doc.rust-lang.org/core/arch/mips64/index.html)
- [`powerpc`](https://doc.rust-lang.org/core/arch/powerpc/index.html)
- [`powerpc64`](https://doc.rust-lang.org/core/arch/powerpc64/index.html)
- [`nvptx`](https://doc.rust-lang.org/core/arch/nvptx/index.html)
- [`wasm32`](https://doc.rust-lang.org/core/arch/wasm32/index.html)





### 常见指令

b 指令

`bl 指令` 跳转到标号出执行

`b.le` ：判断上面cmp的值是小于等于 执行标号，否则直接往下走

`b.ge` 大于等于 执行地址 否则往下

`b.lt` 判断上面camp的值是 小于 执行后面的地址中的方法 否则直接往下走

`b.gt` 大于 执行地址 否则往下

`b.eq` 等于 执行地址 否则往下

`b.hi` 比较结果是无符号大于，执行地址中的方法，否则不跳转

`b.hs` 指令是判断是否无符号小于

`b.ls` 指令是判断是否无符号大于

`b.lo` 指令是判断是否无符号大于等于

### ret 返回

```
mov x0,#0x10 -> x0 = 0x10
```

`str w10 ,[sp]` 将w10寄存器的值存到 sp栈空间内存

`stp x0，x1,[sp.#0x10]* `: x0、x1 的值存入 sp + 0x10

`orr x0，wzr,#0x1` : x0 = wzr | 0x1

`stur w10 ,[sp]` 将w10寄存器的值存到 sp栈空间内存

`ldr w10 ,[sp]` w10 = sp栈内存中的值

`ldp x0，x1,[sp]` x0、x1 = sp栈内存中的值



### `adrp`通过基地址 + 偏移 获得一个字符串(全局变量)

`cbz` 比较，为零则跳转；

`cbnz`: 比较，为非零则跳转。

```
cmp: 比较功能` 例如 : `cmp OPR1 , OPR2. = （OPR1）-（OPR2）
```

`ccmp` : 双重比较. `ccmp x13, x12, #0x0, ne` 判断x13 和 x12 同时存在



### 16位数据操作指令



| 名字 | 功能 |
| ---- | ---- |
|      |      |

| ADC          | 带进位加法(ADD with Carry)                                   |
| ------------ | ------------------------------------------------------------ |
| ADD          | 加法                                                         |
| AND          | 按位与。这里的按位与和C的”&”功能相同                         |
| ASR          | 算术右移(Arithmetic Shift Right)                             |
| BIC          | 按位清零(把一个数跟另一个无符号数的反码按位与)               |
| CMN          | 负向比较(把一个数跟另一个数据的二进制补码相比较)             |
| CMP          | 比较(Compare，比较两个数并且更新标志)                        |
| cmp(Compare) | 比较指令                                                     |
| CMP          | 把一个寄存器的内容和另一个寄存器的内容或立即数进行比较。但不存储结果，只是正确的更改标志。一般CMP做完判断后会进行跳转，后面通常会跟上B指令！ |
| CPY          | 把一个寄存器的值拷贝(COPY)到另一个寄存器中                   |
| EOR          | 近位异或                                                     |
| LSL          | 逻辑左移(Logic Shift Left)                                   |
| LSR          | 逻辑右移(Logic Shift Right)                                  |
| MOV          | 寄存器加载数据，既能用于寄存器间的传输，也能用于加载立即数   |
| MUL          | 乘法(Multiplication)                                         |
| MVN          | 加载一个数的 NOT值(取到逻辑反的值)                           |
| NEG          | 取二进制补码                                                 |
| ORR          | 按位或                                                       |
| ROR          | 循环右移                                                     |
| SBC          | 带借位的减法                                                 |
| SUB          | 减法(Subtraction)                                            |
| TST          | 测试(Test，执行按位与操作，并且根据结果更新Z)                |
| REV          | 在一个32位寄存器中反转(Reverse)字节序                        |
| REVH         | 把一个32位寄存器分成两个(Half)16位数，在每个16位数中反转字节序 |
| REVSH        | 把一个32位寄存器的低16位半字进行字节反转，然后带符号扩展到32位 |
| SXTB         | 带符号(Signed)扩展一个字节(Byte)到 32位                      |
| SXTH         | 带符号(Signed)扩展一个半字(Half)到 32位                      |
| UXTB         | 无符号(Unsigned)扩展一个字节(Byte)到 32位                    |
| UXTH         | 无符号(Unsigned)扩展一个半字(Half)到 32位                    |

### 16位转移指令

| 名字 | 功能 |
| ---- | ---- |
|      |      |

| B    | 无条件转移(Branch)                                          |
| ---- | ----------------------------------------------------------- |
| B    | 有条件(Condition)转移                                       |
| BL   | 转移并连接(Link)。用于呼叫一个子程序，返回地址被存储在LR中  |
| CBZ  | 比较(Compare)，如果结果为零(Zero)就转移(只能跳到后面的指令) |
| CBNZ | 比较，如果结果非零(Non Zero)就转移(只能跳到后面的指令)      |
| IT   | If-Then                                                     |



### 16位存储器数据传送指令

| 名字 | 功能 |
| ---- | ---- |
|      |      |



| LDR   | 从存储器中加载(Load)字到一个寄存器(Register)中         |
| ----- | ------------------------------------------------------ |
| LDRH  | 从存储器中加载半(Half)字到一个寄存器中                 |
| LDRB  | 从存储器中加载字节(Byte)到一个寄存器中                 |
| LDRSH | 从存储器中加载半字，再经过带符号扩展后存储一个寄存器中 |
| LDRSB | 从存储器中加载字节，再经过带符号扩展后存储一个寄存器中 |
| STR   | 把一个寄存器按字存储(Store)到存储器中                  |
| STRH  | 把一个寄存器存器的低半字存储到存储器中                 |
| STRB  | 把一个寄存器的低字节存储到存储器中                     |
| LDMIA | 加载多个字，并且在加载后自增基址寄存器                 |
| STMIA | 存储多个字，并且在存储后自增基址寄存器                 |
| PUSH  | 压入多个寄存器到栈中                                   |
| POP   | 从栈中弹出多个值到寄存器中                             |

### 其它16位指令

| 名字 | 功能 |
| ---- | ---- |
|      |      |

| SVC   | 系统服务调用(Service Call)                                   |
| ----- | ------------------------------------------------------------ |
| BKPT  | 断点(Break Point)指令。如果调试被使能，则进入调试状态(停机)。 |
| NOP   | 无操作(No Operation)                                         |
| CPSIE | 使能 PRIMASK(CPSIE i)/FAULTMASK(CPSIE f)——清零相应的位       |
| CPSID | 除能 PRIMASK(CPSID i)/FAULTMASK(CPSID f)——置位相应的位       |

### 32位数据操作指令

| 名字 | 功能 |
| ---- | ---- |
|      |      |

| ADC        | 带进位加法                                                   |
| ---------- | ------------------------------------------------------------ |
| ADD        | 加法                                                         |
| ADDW       | 宽加法(可以加 12 位立即数)                                   |
| AND        | 按位与                                                       |
| ASR        | 算术右移                                                     |
| BIC        | 位清零(把一个数按位取反后，与另一个数逻辑与)                 |
| BFC        | 位段清零                                                     |
| BFI        | 位段插入                                                     |
| CMN        | 负向比较(把一个数和另一个数的二进制补码比较，并更新标志位)   |
| CMP        | 比较两个数并更新标志位                                       |
| CLZ        | 计算前导零的数目                                             |
| EOR        | 按位异或                                                     |
| LSL        | 逻辑左移                                                     |
| LSR        | 逻辑右移                                                     |
| MLA        | 乘加                                                         |
| MLS        | 乘减                                                         |
| MOVW       | 把 16 位立即数放到寄存器的底16位，高16位清0                  |
| MOV        | 加载16位立即数到寄存器(其实汇编器会产生MOVW)                 |
| MOVT       | 把 16 位立即数放到寄存器的高16位，低 16位不影响              |
| MVN        | 移动一个数的补码                                             |
| MUL        | 乘法                                                         |
| ORR        | 按位或                                                       |
| ORN        | 把源操作数按位取反后，再执行按位或                           |
| RBIT       | 位反转(把一个 32 位整数先用2 进制表达，再旋转180度)          |
| REV        | 对一个32 位整数做按字节反转                                  |
| REVH/REV16 | 对一个32 位整数的高低半字都执行字节反转                      |
| REVSH      | 对一个32 位整数的低半字执行字节反转，再带符号扩展成32位数    |
| ROR        | 圆圈右移                                                     |
| RRX        | 带进位的逻辑右移一格(最高位用C 填充，且不影响C的值)          |
| SFBX       | 从一个32 位整数中提取任意的位段，并且带符号扩展成 32 位整数  |
| SDIV       | 带符号除法                                                   |
| SMLAL      | 带符号长乘加(两个带符号的 32 位整数相乘得到 64 位的带符号积，再把积加到另一个带符号 64位整数中) |
| SMULL      | 带符号长乘法(两个带符号的 32 位整数相乘得到 64位的带符号积)  |
| SSAT       | 带符号的饱和运算                                             |
| SBC        | 带借位的减法                                                 |
| SUB        | 减法                                                         |
| SUBW       | 宽减法，可以减 12 位立即数                                   |
| SXTB       | 字节带符号扩展到32位数                                       |
| TEQ        | 测试是否相等(对两个数执行异或，更新标志但不存储结果)         |
| TST        | 测试(对两个数执行按位与，更新Z 标志但不存储结果)             |
| UBFX       | 无符号位段提取                                               |
| UDIV       | 无符号除法                                                   |
| UMLAL      | 无符号长乘加(两个无符号的 32 位整数相乘得到 64 位的无符号积，再把积加到另一个无符号 64位整数中) |
| UMULL      | 无符号长乘法(两个无符号的 32 位整数相乘得到 64位的无符号积)  |
| USAT       | 无符号饱和操作(但是源操作数是带符号的)                       |
| UXTB       | 字节被无符号扩展到32 位(高24位清0)                           |
| UXTH       | 半字被无符号扩展到32 位(高16位清0)                           |

### 32位存储器数据传送指令

| 名字 | 功能 |
| ---- | ---- |
|      |      |

| LDR   | 加载字到寄存器                                  |
| ----- | ----------------------------------------------- |
| LDRB  | 加载字节到寄存器                                |
| LDRH  | 加载半字到寄存器                                |
| LDRSH | 加载半字到寄存器，再带符号扩展到 32位           |
| LDM   | 从一片连续的地址空间中加载多个字到若干寄存器    |
| LDRD  | 从连续的地址空间加载双字(64 位整数)到2 个寄存器 |
| STR   | 存储寄存器中的字                                |
| STRB  | 存储寄存器中的低字节                            |
| STRH  | 存储寄存器中的低半字                            |
| STM   | 存储若干寄存器中的字到一片连续的地址空间中      |
| STRD  | 存储2 个寄存器组成的双字到连续的地址空间中      |
| PUSH  | 把若干寄存器的值压入堆栈中                      |
| POP   | 从堆栈中弹出若干的寄存器的值                    |

### 32位转移指令

| 名字 | 功能                                                         |
| ---- | ------------------------------------------------------------ |
| B    | 无条件转移                                                   |
| BL   | 转移并连接(呼叫子程序)                                       |
| TBB  | 以字节为单位的查表转移。从一个字节数组中选一个8位前向跳转地址并转移 |
| TBH  | 以半字为单位的查表转移。从一个半字数组中选一个16 位前向跳转的地址并转移 |



### 其它32位指令

| 名字 | 功能 |
| ---- | ---- |
|      |      |



| LDREX  | 加载字到寄存器，并且在内核中标明一段地址进入了互斥访问状态   |
| ------ | ------------------------------------------------------------ |
| LDREXH | 加载半字到寄存器，并且在内核中标明一段地址进入了互斥访问状态 |
| LDREXB | 加载字节到寄存器，并且在内核中标明一段地址进入了互斥访问状态 |
| STREX  | 检查将要写入的地址是否已进入了互斥访问状态，如果是则存储寄存器的字 |
| STREXH | 检查将要写入的地址是否已进入了互斥访问状态，如果是则存储寄存器的半字 |
| STREXB | 检查将要写入的地址是否已进入了互斥访问状态，如果是则存储寄存器的字节 |
| CLREX  | 在本地的处理上清除互斥访问状态的标记(先前由 LDREX/LDREXH/LDREXB做的标记) |
| MRS    | 加载特殊功能寄存器的值到通用寄存器                           |
| MSR    | 存储通用寄存器的值到特殊功能寄存器                           |
| NOP    | 无操作                                                       |
| SEV    | 发送事件                                                     |
| WFE    | 休眠并且在发生事件时被唤醒                                   |
| WFI    | 休眠并且在发生中断时被唤醒                                   |
| ISB    | 指令同步隔离(与流水线和 MPU等)                               |
| DSB    | 数据同步隔离(与流水线、MPU 和cache等)                        |
| DMB    | 数据存储隔离(与流水线、MPU 和cache等)                        |









### 并行加减指令

```
除了正常的数据处理和乘法指令之外，ARMv6还引入了一组并行加减指令
下面介绍6种基本的指令
    ADD16
        添加两个寄存器的顶部半字形成结果的上半部分。
        添加相同两个寄存器的底部半字形成结果的底部半字部分
    ADDSUBX
        交换第二个操作数寄存器的半字，加上半字，减半字。
        为啥这样设计，这里暂时不明白,后续章节有详细介绍
        <opcode3>{<cond>} {S} <Rd> <Rn> <shifter_operand>
            参照上面描述的指令格式，第二操作数实际上交换的shifter_operand
    SUBADDX
        交换第二个操作数寄存器的半字，减去顶部半字，并增加底部半字
    SUB16
        从第二个操作数寄存器的上半部分减去第一个操作数寄存器的上半部分，形成结果的上半部分。
        从第一个操作数寄存器的下半部分减去第二个操作数寄存器的下部半字，形成结果的下半部分。
    ADD8 
        将第二个操作数寄存器的每个字节添加到第一个操作数寄存器的相应字节，以形成结果的相应字节
    SUB8 
        将第一个操作数寄存器的相应字节中减去第二个操作数寄存器的每个字节，以形成结果的相应字节。

六种指令中的每一个可用于以下变体，即为这六种指令添加前缀：   
    有符号指示前缀：
        S  符号算术模2^8或2^16.设置CPSR GE位（请参见第A2-13页的GE [3：0]位）。
        Q  有符号饱和算术
        SH  有符号算术，将结果减半以避免溢出
    无符号指示前缀：
        U  无符号运算模2^8或2^16.设置CPSR GE位（参见第A2-13页的GE [3：0]位）
        UQ  无符号饱和算术
        UH  无符号算术，将结果减半以避免溢出
仅仅拿有符号前缀S Q SH举例：(后续章节由对这些指令用法有详细的文档说明) 
    SADD16 SADDSUBX SUBADDX SSUB16 SADD8 SSUB8 
    QADD16 QADDSUBX QSUBADDX QSUB16 QADD8 QSUB8
    SHADD16 SHADDSUBX SHSUBADDX SHSUB16 SHADD8 SHSUB8
    后续章节指令有详细介绍

```



### 扩展指令

```
ARMv6及更高版本提供了几个指令，用于通过符号或零扩展字节到半字或字，以及将字的半字打包数据。
您可以选择将结果添加到另一个寄存器的内容。您可以在扩展之前将操作数寄存器的任意倍数旋转8位

下面介绍6种基本的指令：
    XTAB16 将一个寄存器的位[23:16]和位[7：0]扩展为16位，并将相应的半字添加到另一个寄存器中的值。
    XTAB 将一个寄存器的位[7：0]扩展为32位，并将其添加到另一个寄存器中的值。
    XTAH 将一个寄存器的位[15：0]扩展为32位，并将其添加到另一个寄存器中的值。
    XTB16 将位[23:16]和位[7：0]分别扩展为16位
    XTB 将扩展位[7：0]至32位
    XTH 将位[15：0]扩展为32位

六种指令中的每一个可用于以下变体，即为这六种指令添加前缀：
    S 符号扩展，带或不带加法模 数2^16或2^32。
    U 零（无符号）扩展名，带或不带加法模 数2^16或2^32
仅拿符号前缀U举例
    UTXAB16 UTXAB UXTAH UXTB16 UXTB UXTH
    后续章节有详细指令介绍

```

### 杂项算术指令

```
ARMv5及以上版本包括几个其他算术指令。   
Count Leading zeros（CLZ）指令。
    该指令在遇到第一个1位之前返回其操作数最高有效位的0个数（如果操作数为0，则返回32位）。
    大白话，对寄存器直进行移位操作，直到最高位为1时停止移位，并记下移了多少次。

这两个典型的应用是：
    1、要确定操作数应该向左移动多少位以使其正常化，以使其最高有效位为1（可用于整数除法程序）
    2、要定位最高优先级位在哪一个掩码位（置）上

Unsigned sum of absolute differences(USAD)指令 
    ARMv6引入了无符号绝对差值（USAD8）指令和无符号绝对差值和累加（USADA8）指令

这些指令执行以下操作：
    1.从两个寄存器取相应字节。
    2.找出每对字节的无符号值之间的绝对差值。
    3.求和四个绝对值。 
    4.可选地，将绝对差的和累积到第三寄存器中的值

其他杂项指令
    PKHBT
        （Pack Halfword Bottom Top）将其第一个操作数的底部，最不重要的半字与其移位的第二个操作数的顶部（最重要）半字组合。偏移是从0到31的任意数量的左移。
    PKHTB
        （Pack Halfword Top Bottom）将其第一个操作数的顶部，最重要的半字与其移位的第二个操作数的底部（最低有效）半字组合。这个变化是一个算术右移，任意数量从1到32
    REV 
        （Byte-Reverse Word）反转32位寄存器中的字节顺序    有意思的指令额^_^
    REV16
        （Byte-Reverse Packed Halfword）反转32位寄存器的每个16位半字的字节顺序。

    REVSH（Byte-Reverse Signed Halfword）
        反转32位寄存器的低16位半字的字节顺序，符号将结果扩展为32位。
    SEL
        根据GE标志的值，从其第一个操作数或其第二个操作数中选择其结果的每个字节。

    SSAT（Signed Saturate）
        将有符号的值饱和为有符号范围。您可以选择饱和发生的位位置。您可以在饱和发生之前对值进行移位。
    USAT（Unsigned Saturate）
        将有符号值饱和为无符号范围。您可以选择饱和发生的位位置。您可以在饱和发生之前对值进行移位
    USAT16
        将两个带符号的16位值饱和为无符号范围。您可以选择饱和发生的位位置
    后续章节有详细指令介绍

```

### CPSR访问指令

```
有两个指令用于将程序状态寄存器的内容移动到通用寄存器或从通用寄存器移出。可以访问CPSR和SPSR
    此外，在ARMv6中，有几个指令可以直接写入CPSR中的特定位或位组。
    每个状态寄存器传统上分为四个8位字段，可以单独写入：
        Bits[31:24] 标志字段
        Bits[23:16] 状态字段
        Bits[15:8] 扩展字段
        Bits[7:0] 控制字段

    CPSR value
        改变CPSR的值有五个用途：
            •将条件代码标志（以及存在的Q标志）的值设置为已知值
            •启用或禁用中断
            •更改处理器模式（例如，初始化堆栈指针）
            •更改加载和存储操作的字节顺序
            •更改处理器状态（J和T位）

        注意：
            只有通过BX，BLX或BXJ指令写入CPSR才能直接更改T和J位，并且在针对异常返回的指令中，将隐式SPSR转换为CPSR。
            通过直接更改T或J位来尝试进入或离开Thumb或Jazelle状态将产生不可预测的后果。
        例子： 
            这些示例假定ARM处理器已处于特权模式。如果ARM处理器以用户模式启动，只有标志更新有效果

        MRS R0,CPSR                     ;读CPSR到R0寄存器
        BIC R0,R0,#0XF0000000           ;清除 the N Z C and V 标志位(R0 = R0 & !0xF0000000)
        MSR CPSR_f,R0                   ;更新CPSR状态寄存器的N Z C V标志位

        MRS R0,CPSR                     ;read the cpsr
        ORR R0,R0,#0X80                 ;set the interrupt disabled bit
        MSR CPSR_f,R0                   ; Update the contrl bits in the cpsr //interrupt(IRQ) now diabbled

        MRS R0,CPSR 
        BIC R0,R0,#0X1F                 ; r0 = r0 & !0x1f  清除模式位
        ORR R0,R0,#0X11                 ; R0 =  R0 | 0x11  设置模式为FIQ模式
        MSR CPSR_c，R0                   ;更新CPSR状态寄存器控制模式位，进入FIQ模式

    />
    状态寄存器访问指令列表
        MRS 将PSR移动到通用寄存器
        MSR 将通用寄存器移动到PSR寄存器
        CPS 更改处理器状态。更改CPSR的一个或多个处理器模式和中断使能位，而不改变其他CPSR位。
        SETEND  修改CPSR字节顺序E，位，而不改变CPSR中的任何其他位

        处理器状态位也可以通过更新PC的各种分支(BX)，加载和返回指令进行更新。
        当它们用于Jazelle状态进入/退出和Thumb交互工作时，发生更改。

```





# arm_neon.h 支持的操作

```
add         加法
mul         乘法
sub         减法
mla         乘加
mls         乘减
ceq         比较，类似与 ==
cge         比较，类似与 >=
cle         比较，类似与 <=
cgt         比较，类似与 >
clt         比较，类似与 <
tst         做与运算后，判断是否等于0 ,ri = (ai & bi != 0) ? 1…1:0…0;
abd         两个向量相减后的绝对值，vabd -> ri = |ai - bi|;
max         求最大值，ri = ai >= bi ? ai : bi;
min         求最小值，ri = ai >= bi ? bi : ai;
shl         左移位， ri = ai << b;
shr         右移位， ri = ai >> b;
abs         求绝对值，ri = |ai|;
neg         取反，ri = -ai;
mvn         按位取反，ri = ~ai;
and         与运算，ri = ai & bi;
orr         或运算，ri = ai | bi;
eor         异或运算，ri = ai ^ bi;
cls         计算连续相同的位数
get         取值，从向量中取出一个值，所谓的向量可以认为是一个数组，给数组中的某个元素赋值
set         赋值，给向量中赋值
dup         构造一个向量，并赋上初始值，ri = a;
comb        ine 合并操作，把两个向量合并
mov         改变数据类型，数据范围，比如把u8 变成u16，或者u16变成u8
zip         压缩操作
uzp         解压操作
ld1         加载数据，给定的buffer 指针中拷贝数据，注意是ld后面的是数字1，而不是字母l
st1         拷贝数据，将neon数据类型拷贝到指定buffer中

```











| 固有的                                                       | 论证准备                    | AArch64 指令               | 结果                                                         |
| :----------------------------------------------------------- | :-------------------------- | :------------------------- | :----------------------------------------------------------- |
| `int8x8_t vadd_s8(   int8x8_t a,   int8x8_t b)`   <br />64位的D寄存器划分为8个int8长度，<br />Vn.8B 和Vm.8B应该表示是说Vn、Vm寄存器<br />存放8个B比特（8bit）int8数值<br /><br /><br /> | `a -> Vn.8B` `b -> Vm.8B`   | `ADD Vd.8B,Vn.8B,Vm.8B`    | `Vd.8B -> result`  <br />返回值的大小就是最前面指明的int8x8_t的int8 |
| `int8x16_t vaddq_s8(   int8x16_t a,   int8x16_t b)`    <br />128位的Q寄存器划分为16个int8长度，<br />Vn.16B 和Vm.16B应该表示是说Vn、Vm寄存器<br />存放16个B比特（8bit）int8数值<br /> | `a -> Vn.16B` `b -> Vm.16B` | `ADD Vd.16B,Vn.16B,Vm.16B` | `Vd.16B -> result`   <br />返回值的大小就是最前面指明的int8x8_t的int8 |
| `int16x4_t vadd_s16(   int16x4_t a,   int16x4_t b)`  <br />64位的D寄存器划分为4个int16长度，<br />Vn.4H 和Vm.4H应该表示是说Vn、Vm寄存器<br />存放4个16bit的int16数值，因为16bit是32bit的一半也就是Half所以用H表示 | `a -> Vn.4H` `b -> Vm.4H`   | `ADD Vd.4H,Vn.4H,Vm.4H`    | `Vd.4H -> result`                                            |
| `int16x8_t vaddq_s16(   int16x8_t a,   int16x8_t b)`  <br />128位的Q寄存器划分为8个int16长度，<br />Vn.8H 和Vm.8H应该表示是说Vn、Vm寄存器<br />存放8个16bit的int16数值，因为16bit是32bit的一半也就是Half所以用H表示 | `a -> Vn.8H` `b -> Vm.8H`   | `ADD Vd.8H,Vn.8H,Vm.8H`    | `Vd.8H -> result`                                            |
| `int32x2_t vadd_s32(   int32x2_t a,   int32x2_t b)`  <br />64位的D寄存器划分为2个int32长度，<br />Vn.2S和Vm.2S应该表示是说Vn、Vm寄存器<br />存放2个32bit的int32数值，vadd_s32的s32表示有符号的32位整数，所以是2S | `a -> Vn.2S` `b -> Vm.2S`   | `ADD Vd.2S,Vn.2S,Vm.2S`    | `Vd.2S -> result`                                            |
| `int32x4_t vaddq_s32(   int32x4_t a,   int32x4_t b)`         | `a -> Vn.4S` `b -> Vm.4S`   | `ADD Vd.4S,Vn.4S,Vm.4S`    | `Vd.4S -> result`                                            |
| `int64x1_t vadd_s64(   int64x1_t a,   int64x1_t b)` <br />64位的D寄存器没有划分，所以直接表示为Dn、Dm | `a -> Dn` `b -> Dm`         | `ADD Dd,Dn,Dm`             | `Dd -> result`                                               |
|                                                              |                             |                            |                                                              |
| `int64x2_t vaddq_s64(   int64x2_t a,   int64x2_t b)`    <br />128位的Q寄存器划分为2个int64长度，<br />Vn.2D 和Vm.2D应该表示是说Vn、Vm寄存器<br />存放2个64bit的int64数值，相当于将一个Q寄存器划分为2个D寄存器 | `a -> Vn.2D` `b -> Vm.2D`   | `ADD Vd.2D,Vn.2D,Vm.2D`    | `Vd.2D -> result`                                            |
| `uint8x8_t vadd_u8(   uint8x8_t a,   uint8x8_t b)`         <br />64位的D寄存器划分为8个无符号int8长度，<br />Vn.8B 和Vm.8B应该表示是说Vn、Vm寄存器<br />存放8个B比特（8bit）int8数值<br /> | `a -> Vn.8B` `b -> Vm.8B`   | `ADD Vd.8B,Vn.8B,Vm.8B`    | `Vd.8B -> result`                                            |
| `uint8x16_t vaddq_u8(   uint8x16_t a,   uint8x16_t b)`     <br />128位的Q寄存器划分为16个无符号int8长度，<br />Vn.16B 和Vm.16B应该表示是说Vn、Vm寄存器<br />存放16个B比特（8bit）无符号int8数值 | `a -> Vn.16B` `b -> Vm.16B` | `ADD Vd.16B,Vn.16B,Vm.16B` | `Vd.16B -> result`                                           |
| `uint16x4_t vadd_u16(   uint16x4_t a,   uint16x4_t b)`       | `a -> Vn.4H` `b -> Vm.4H`   | `ADD Vd.4H,Vn.4H,Vm.4H`    | `Vd.4H -> result`                                            |
| `uint16x8_t vaddq_u16(   uint16x8_t a,   uint16x8_t b)`      | `a -> Vn.8H` `b -> Vm.8H`   | `ADD Vd.8H,Vn.8H,Vm.8H`    | `Vd.8H -> result`                                            |
| `uint32x2_t vadd_u32(   uint32x2_t a,   uint32x2_t b)`       | `a -> Vn.2S` `b -> Vm.2S`   | `ADD Vd.2S,Vn.2S,Vm.2S`    | `Vd.2S -> result`                                            |
| `uint32x4_t vaddq_u32(   uint32x4_t a,   uint32x4_t b)`      | `a -> Vn.4S` `b -> Vm.4S`   | `ADD Vd.4S,Vn.4S,Vm.4S`    | `Vd.4S -> result`                                            |
| `uint64x1_t vadd_u64(   uint64x1_t a,   uint64x1_t b)`       | `a -> Dn` `b -> Dm`         | `ADD Dd,Dn,Dm`             | `Dd -> result`                                               |
| `uint64x2_t vaddq_u64(   uint64x2_t a,   uint64x2_t b)`      | `a -> Vn.2D` `b -> Vm.2D`   | `ADD Vd.2D,Vn.2D,Vm.2D`    | `Vd.2D -> result`                                            |
| `float32x2_t vadd_f32(   float32x2_t a,   float32x2_t b)`    | `a -> Vn.2S` `b -> Vm.2S`   | `FADD Vd.2S,Vn.2S,Vm.2S`   | `Vd.2S -> result`                                            |
| `float32x4_t vaddq_f32(   float32x4_t a,   float32x4_t b)`   | `a -> Vn.4S` `b -> Vm.4S`   | `FADD Vd.4S,Vn.4S,Vm.4S`   | `Vd.4S -> result`                                            |
| `float64x1_t vadd_f64(   float64x1_t a,   float64x1_t b)`    | `a -> Dn` `b -> Dm`         | `FADD Dd,Dn,Dm`            | `Dd -> result`                                               |
| `float64x2_t vaddq_f64(   float64x2_t a,   float64x2_t b)`   | `a -> Vn.2D` `b -> Vm.2D`   | `FADD Vd.2D,Vn.2D,Vm.2D`   | `Vd.2D -> result`                                            |
| `int64_t vaddd_s64(   int64_t a,   int64_t b)`               | `a -> Dn` `b -> Dm`         | `ADD Dd,Dn,Dm`             | `Dd -> result`                                               |
| `uint64_t vaddd_u64(   uint64_t a,   uint64_t b)`            | `a -> Dn` `b -> Dm`         | `ADD Dd,Dn,Dm`             | `Dd -> result`                                               |

#####  扩大加法[^](https://arm-software.github.io/acle/neon_intrinsics/advsimd.html#markdown-toc-widening-addition)

| 固有的                                                       | 论证准备                                                     | AArch64 指令                 | 结果              |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :--------------------------- | :---------------- |
| `int16x8_t vaddl_s8(   int8x8_t a,   int8x8_t b)`   <br />8个int8相加，但是需要保留进位，所以需要大于8bit位宽，就要用到128位的Q寄存器，划分为16x8来存放结果值     vaddl的"l"表示长指令的L，结果是操作数的2倍宽度<br /> <br />pub unsafe fn vaddl_s8(a: int8x8_t, b: int8x8_t) -> int16x8_t  {                                                        let a: int16x8_t = simd_cast(a);     let b: int16x8_t = simd_cast(b);     simd_add(a, b)                                   }<br />simd_cast()数据类型转换 | `a -> Vn.8B` `b -> Vm.8B`                                    | `SADDL Vd.8H,Vn.8B,Vm.8B`    | `Vd.8H -> result` |
| [vaddl_s8（v64，v64）](https://docs.unity3d.com/Packages/com.unity.burst@1.8/api/Unity.Burst.Intrinsics.Arm.Neon.vaddl_s8.html#Unity_Burst_Intrinsics_Arm_Neon_vaddl_s8_Unity_Burst_Intrinsics_v64_Unity_Burst_Intrinsics_v64_) | 有符号加长（向量）。该指令将第一个源 SIMD&FP 寄存器的下半部分或上半部分中的每个向量元素与第二个源 SIMD&FP 寄存器的相应向量元素相加，将结果放入向量中，并将向量写入目标 SIMD&FP 寄存器。目标向量元素的长度是源向量元素的两倍。该指令中的所有值都是有符号整数值。SADDL 指令从每个源寄存器的下半部分提取每个源向量，而 SADDL2 指令从每个源寄存器的上半部分提取每个源向量。取决于在CPACR_EL1、CPTR_EL2 和 CPTR_EL3 寄存器，以及当前的安全状态和异常级别，执行指令的尝试可能会被捕获。 等效指令：`SADDL Vd.8H,Vn.8B,Vm.8B` |                              |                   |
| `int32x4_t vaddl_s16(   int16x4_t a,   int16x4_t b)`         | `a -> Vn.4H` `b -> Vm.4H`                                    | `SADDL Vd.4S,Vn.4H,Vm.4H`    | `Vd.4S -> result` |
| `int64x2_t vaddl_s32(   int32x2_t a,   int32x2_t b)`         | `a -> Vn.2S` `b -> Vm.2S`                                    | `SADDL Vd.2D,Vn.2S,Vm.2S`    | `Vd.2D -> result` |
| `uint16x8_t vaddl_u8(   uint8x8_t a,   uint8x8_t b)`         | `a -> Vn.8B` `b -> Vm.8B`                                    | `UADDL Vd.8H,Vn.8B,Vm.8B`    | `Vd.8H -> result` |
| `uint32x4_t vaddl_u16(   uint16x4_t a,   uint16x4_t b)`      | `a -> Vn.4H` `b -> Vm.4H`                                    | `UADDL Vd.4S,Vn.4H,Vm.4H`    | `Vd.4S -> result` |
| `uint64x2_t vaddl_u32(   uint32x2_t a,   uint32x2_t b)`      | `a -> Vn.2S` `b -> Vm.2S`                                    | `UADDL Vd.2D,Vn.2S,Vm.2S`    | `Vd.2D -> result` |
| `int16x8_t vaddl_high_s8(   int8x16_t a,   int8x16_t b)`     | `a -> Vn.16B` `b -> Vm.16B`                                  | `SADDL2 Vd.8H,Vn.16B,Vm.16B` | `Vd.8H -> result` |
| pub unsafe fn vaddl_high_s8(a: int8x16_t, b: int8x16_t) -> int16x8_t {                                      let a: int8x8_t = simd_shuffle8!(a, a, [8, 9, 10, 11, 12, 13, 14, 15]);     let b: int8x8_t = simd_shuffle8!(b, b, [8, 9, 10, 11, 12, 13, 14, 15]);     let a: int16x8_t = simd_cast(a);     let b: int16x8_t = simd_cast(b);     simd_add(a, b)                                         } | 而 SADDL2 指令从每个源寄存器的上半部分提取每个源向量,  所以  vaddl_high_  的底层实现中先进行一个simd_shuffle8!(a, a, [8, 9, 10, 11, 12, 13, 14, 15]) 运算操作 |                              |                   |
| `int32x4_t vaddl_high_s16(   int16x8_t a,   int16x8_t b)`    | `a -> Vn.8H` `b -> Vm.8H`                                    | `SADDL2 Vd.4S,Vn.8H,Vm.8H`   | `Vd.4S -> result` |
| `int64x2_t vaddl_high_s32(   int32x4_t a,   int32x4_t b)`    | `a -> Vn.4S` `b -> Vm.4S`                                    | `SADDL2 Vd.2D,Vn.4S,Vm.4S`   | `Vd.2D -> result` |
| `uint16x8_t vaddl_high_u8(   uint8x16_t a,   uint8x16_t b)`  | `a -> Vn.16B` `b -> Vm.16B`                                  | `UADDL2 Vd.8H,Vn.16B,Vm.16B` | `Vd.8H -> result` |
| `uint32x4_t vaddl_high_u16(   uint16x8_t a,   uint16x8_t b)` | `a -> Vn.8H` `b -> Vm.8H`                                    | `UADDL2 Vd.4S,Vn.8H,Vm.8H`   | `Vd.4S -> result` |
| `uint64x2_t vaddl_high_u32(   uint32x4_t a,   uint32x4_t b)` | `a -> Vn.4S` `b -> Vm.4S`                                    | `UADDL2 Vd.2D,Vn.4S,Vm.4S`   | `Vd.2D -> result` |
| `int16x8_t vaddw_s8(   int16x8_t a,   int8x8_t b)`     <br />vaddw的w表示是宽指令，结果是第二个操作数位宽的两倍 | `a -> Vn.8H` `b -> Vm.8B`                                    | `SADDW Vd.8H,Vn.8H,Vm.8B`    | `Vd.8H -> result` |
| `int32x4_t vaddw_s16(   int32x4_t a,   int16x4_t b)`         | `a -> Vn.4S` `b -> Vm.4H`                                    | `SADDW Vd.4S,Vn.4S,Vm.4H`    | `Vd.4S -> result` |
| `int64x2_t vaddw_s32(   int64x2_t a,   int32x2_t b)`         | `a -> Vn.2D` `b -> Vm.2S`                                    | `SADDW Vd.2D,Vn.2D,Vm.2S`    | `Vd.2D -> result` |
| `uint16x8_t vaddw_u8(   uint16x8_t a,   uint8x8_t b)`        | `a -> Vn.8H` `b -> Vm.8B`                                    | `UADDW Vd.8H,Vn.8H,Vm.8B`    | `Vd.8H -> result` |
| `uint32x4_t vaddw_u16(   uint32x4_t a,   uint16x4_t b)`      | `a -> Vn.4S` `b -> Vm.4H`                                    | `UADDW Vd.4S,Vn.4S,Vm.4H`    | `Vd.4S -> result` |
| `uint64x2_t vaddw_u32(   uint64x2_t a,   uint32x2_t b)`      | `a -> Vn.2D` `b -> Vm.2S`                                    | `UADDW Vd.2D,Vn.2D,Vm.2S`    | `Vd.2D -> result` |
| `int16x8_t vaddw_high_s8(   int16x8_t a,   int8x16_t b)`     <br />pub unsafe fn vaddw_high_s8(a: int16x8_t, b: int8x16_t) -> int16x8_t              {     <br /> let b: int8x8_t = simd_shuffle8!(b, b, [8, 9, 10, 11, 12, 13, 14, 15]);                      let b: int16x8_t = simd_cast(b);                                                                  simd_add(a, b) }                                                                                                             可以看出是将第二个操作数的高8个int8数字给取出来，并转换为int16类型再进行加法计算，而且结果是第二操作数8位宽的2倍，16位宽<br />**SADDW指令从第二源寄存器的下半部分提取第二源向量，而SADDW2指令从第二源寄存器的上半部分提取第二源向量**。 一个是去第二操作数的高半部，一个是取地半部  <br />S:有符号    U：无符号<br /><br />**Signed Add Wide ： 有符号加宽** | `a -> Vn.8H` `b -> Vm.16B`                                   | `SADDW2 Vd.8H,Vn.8H,Vm.16B`  | `Vd.8H -> result` |
| `int32x4_t vaddw_high_s16(   int32x4_t a,   int16x8_t b)`    | `a -> Vn.4S` `b -> Vm.8H`                                    | `SADDW2 Vd.4S,Vn.4S,Vm.8H`   | `Vd.4S -> result` |
| `int64x2_t vaddw_high_s32(   int64x2_t a,   int32x4_t b)`    | `a -> Vn.2D` `b -> Vm.4S`                                    | `SADDW2 Vd.2D,Vn.2D,Vm.4S`   | `Vd.2D -> result` |
| `uint16x8_t vaddw_high_u8(   uint16x8_t a,   uint8x16_t b)`    <br />**Unsigned Add Wide :无符号加宽**    第二操作数加宽，结果加宽 | `a -> Vn.8H` `b -> Vm.16B`                                   | `UADDW2 Vd.8H,Vn.8H,Vm.16B`  | `Vd.8H -> result` |
| `uint32x4_t vaddw_high_u16(   uint32x4_t a,   uint16x8_t b)` | `a -> Vn.4S` `b -> Vm.8H`                                    | `UADDW2 Vd.4S,Vn.4S,Vm.8H`   | `Vd.4S -> result` |
| `uint64x2_t vaddw_high_u32(   uint64x2_t a,   uint32x4_t b)` | `a -> Vn.2D` `b -> Vm.4S`                                    | `UADDW2 Vd.2D,Vn.2D,Vm.4S`   | `Vd.2D -> result` |





































