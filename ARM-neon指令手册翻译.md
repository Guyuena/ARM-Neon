#### 注意

- 如果你完全是ARM小白，先看看Cortex-A系列的编程指导手册。
- NEON技术是ARM高级单指令多数据（SIMD）扩展的实现。
- NEON单元是处理器的组件，执行SIMD指令的。 也称为NEON媒体处理引擎（MPE）。
- 某些实现ARMv7–A或ARMv7–R体系结构配置文件的Cortex-A系列处理器不包含NEON单元。

## 1. Data [processing](https://so.csdn.net/so/search?q=processing&spm=1001.2101.3001.7020) technologies

### 1.1 Single Instruction Single Data

  **单指令单操作**
  每个操作指定要处理的单个数据，处理多个数据需要多条指令。下面的代码用4条指令把8个寄存器相加：

```markup
    add r0, r5    @ 意思是r0 = r0 + r5
    add r1, r6
    add r2, r7
    add r3, r8
    
123456
```

  这种方式很慢，并且很难看到不同寄存器之间是怎么关联的。为了提高效率和性能，媒体处理通常下放到不同的处理器，例如图形处理单元（Graphics Processing Unit、GPU）或者媒体处理单元（Media Processing Unit），这些单元可以通过单指令处理多个数据。

  在一个32位ARM处理器上，处理大量的8位或者16位单独操作很难有效地利用机器资源，因为处理器、寄存器和数据路径都是为了32位的计算而设计的。

### 1.2 Single Instruction Multiple Data（vector mode）

  **SMID单指令多数据（向量模式）**

  一个操作可以指定对多个数据源进行相同的处理。 如果控制寄存器中的`LEN`值为4，则单个向量加法指令将执行四个加法：

```markup
    VADD.F32  S24, S8, S16
    
    // 以上操作产生4条加法:
    // S24 = S8 +S16
    // S25 = S9 +S17
    // S26 = S10 +S18
    // S27 = S11 +S20
    
123456789
```

  尽管只有一条指令，但是它在4个步骤中按顺序执行了4次加法。

  **在ARM的术语中，这种叫做Vector Floating Point（VPF 向量浮点）。**

  向量浮点（VFP）扩展是在ARMv5架构中引入的，并执行了短向量指令以加快浮点操作。 源寄存器和目标寄存器可以是用于标量运算的单个寄存器，也可以是用于向量运算的两个至八个寄存器的序列。

  由于SIMD操作比VFP操作更有效地执行向量计算（vector calculation），因此从ARMv7引入以来，向量模式操作已被弃用，并被NEON技术所取代，该技术对宽寄存器执行多种操作。

  浮点（floating-point）和NEON使用公共寄存器组进行操作。

### 1.3 Single Instruction Multiple Data（packed data mode）

  **SIMD单指令多数据（打包的数据模式）**

  一个操作可以指定对存储在一个大寄存器中的多个数据字段进行相同的处理：

```markup
    VADD.I16  Q10, Q8, Q9
    // 这个操作把两个64位寄存器相加
    // 但是把寄存器中每16位作为一个lane，寄存器中4个lane被分开单独相加
    // 元素之间互不影响
12345
```

  就像图1那样，这一条指令对一个大寄存器的所有数据同时处理，这种方法更快。

![img](https://img-blog.csdnimg.cn/20200804161211992.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70,#pic_center)

lane：通道



图1

#### 注意

- 图1的几个lane的加法是完全独立的。

- 任何来自第15位（在`lane0`中）的溢出或者进位不会影响到第16位（在`lane1`中），因为每个`lane`是分隔开计算的

- 图1展示了1个64位寄存器持有4个16位的数据，分别为

  ```
  lane0~lane3
  ```

  ，NEON寄存器还支持其它的组合：

  - 单个64位寄存器中，同时操作2个32位、4个16位或8个8位整形数据。
  - 在128寄存器中，同时操作4个32位、8个16位或者16个8位整形数据。

  移动设备中使用的媒体处理器，通常将每个完整的数据寄存器分成多个子寄存器，并并行地对子寄存器执行计算。 如果对数据集的处理简单且重复很多次，则SIMD可以显着提高性能。 对于数字信号处理或多媒体算法，例如：

- 音频、视频、图像处理编解码器。
- 基于矩形像素块的2D图像。
- 3D图像。
- 颜色空间的转换
- 物理仿真。

## 2. Comparison between ARM NEON technology and other implementations

  **NEON技术和其它ARM或第三方数据处理之前的区别**

### 2.1 Comparison between NEON technology and the ARMv6 SIMD instructions

  **NEON和ARMv6 SIMD指令集的区别**

  ARMv6体系结构引入了一个很小的SIMD指令集，这些指令对打包在标准32位ARM通用寄存器中的多个16位或8位值进行操作。 这些指令允许某些操作以两倍或四倍的速度执行，而无需添加其他计算单元。

  ARMv6 SIMD指令`UADD8 R0, R1, R2`将一个32位寄存器中的四个8位值与另一个32位寄存器中的四个8位值相加，如图1-2所示。
![img](https://img-blog.csdnimg.cn/20200804164239650.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70,#pic_center)

  该操作对封装在32位寄存器`R1`和`R2`中的向量中的四个8位元素（element）（称为通道`lane`）执行并行加法运算，并将结果放入寄存器`R0`中的向量中。

  ARM NEON技术基于SIMD的概念，并支持128位向量操作，而不是ARMv6体系结构中的32位向量操作。

  Cortex-A7和Cortex-A15处理器中默认含有NEON单元，但在其他ARMv7 Cortex-A系列处理器中也可能有。

表1 NEON和ARMv6 SIMD指令集的比较

| **特性**           | **ARMv7 NEON 扩展**                                          | **ARMv6 SIMD 指令集**          |
| ------------------ | ------------------------------------------------------------ | ------------------------------ |
| 打包整形           | 8x8位，4x16位，2x32位                                        | 4x8位                          |
| 数据类型           | 支持整数，如果处理器具有VFP单元，则支持单精度浮点运算        | 仅支持整数运算                 |
| 并行操作           | 最多十六个（16 x 8位）                                       | 最多四个（4 x 8位）            |
| 专用寄存器         | 32个64位寄存器（或16个128位寄存器） 在与VFP单元共享的单独NEON寄存器文件上执行操作 | 使用32位通用ARM寄存器          |
| 流水线（Pipeline） | 具有专门为NEON执行而优化的专用流水线                         | 使用与所有其他指令相同的流水线 |

### 2.3 Comparison between NEON technology and other SIMD solutions

  **NEON和其它SIMD的比较**

  SIMD处理不是ARM独有的，考虑其它的实现，并与NEON技术进行比较很有用。

表2 NEON和MMX、Altivec的比较

|                                | Neon                           | x86 MMX/SSE                                                  | Altivec                    |
| ------------------------------ | ------------------------------ | ------------------------------------------------------------ | -------------------------- |
| 寄存器数量                     | 32 x 64位 (或者视为16 x 128位) | SSE2: 8 x 128-bit XMM (x86-32模式) x86-64模式下额外8个寄存器 | 32 x 128位                 |
| 内存/寄存器操作                | 基于寄存器的3个操作数指令      | 寄存器和内存混合操作                                         | 基于寄存器的3和4操作数指令 |
| 打包数据的加载和存储           | 支持2、3、4个元素              | 不支持                                                       | 不支持                     |
| 在向量和标量寄存器之间移动数据 | 支持                           | 支持                                                         | 不支持                     |
| 浮点支持                       | 32位单精度                     | 单精度或双精度                                               | 单精度                     |

### 2.3 Comparison of NEON technology and Digital Signal Processors

  **NEON和数字信号处理器的区别**

  许多ARM处理器还集成了数字信号处理器（DSP），或自定义的信号处理硬件，因此它们可以同时包含NEON单元和DSP。 使用NEON技术与使用DSP有一些区别：

**NEON的特点:**

- 扩展ARM处理器流水线
- 使用ARM内核寄存器进行内存寻址。
- 单线程控制，可简化开发和调试。
- 支持操作系统多任务（如果操作系统保存或还原NEON和浮点寄存器文件）。
- SMP功能。 MPCore处理器中的每个ARM内核都有一个NEON单元。 通过使用多个内核，用标准的pthread可以提供标准性能的几倍性能。
- 作为ARM处理器体系结构的一部分，NEON单元将在未来更快的处理器上使用，并得到许多基于ARM处理器的SoC的支持。
- 开源社区和ARM生态系统均提供了广泛的NEON工具支持。

**DSP的特点:**

- 与ARM处理器并行运行。
- 没啥操作系统和任务切换开销，可以提供有保证的性能，但前提是DSP系统设计必须提供可预测的性能。
- 开发应用的程序猿很难用：有两个工具集，分别用单独的调试器，潜在的同步问题。
- 和ARM处理器的集成不太紧密。 在DSP和ARM处理器之间传输数据时，可能会有一些清除缓存或刷新缓存的开销，这会降低使用DSP处理非常小的数据块的效率。

## 3. Architecture support for NEON technology

  **NEON技术的架构支持**

  NEON扩展在某些处理器上被实现，这些处理器实现了ARMv7-A或ARMv7-R体系结构配置文件。

#### 注意

- ARMv8体系结构扩展了对NEON的支持，并提供了与ARMv7实现的向后兼容性。
- 本手册仅涵盖实现了ARMv7-A和ARMv7-R的内核。
- 不保证您正在编程的ARMv7-A或ARMv7-R处理器包含NEON或VFP技术。 符合ARMv7的内核的可能组合为：
  - 不存在NEON单元或VFP单元。
  - 存在NEON单元，但没有VFP单元。
  - 不存在NEON单元，但带有VFP单元。
  - NEON单元和VFP单元都有。
- 处理器具有NEON单元，但没有VPF单元，无法在硬件中执行浮点运算。
- 由于NEON的SIMD操作可以更有效地执行向量计算，因此ARMv7的引入已不建议用VFP单元中的向量模式操作。 因此，VFP单元有时称为浮点单元（FPU）。
- 具有NEON或VFP单元的处理器可能不支持以下扩展：
  \- 半精度。
  \- 融合乘加。
- 如果存在VPF单元，则有些版本可以访问16或32个NEON双字寄存器

### 3.1 Instruction timings

  **指令时序**

  NEON体系结构未指定指令时序。 一条指令可能需要不同数量的周期才能在不同的处理器上执行。即使在同一处理器上，指令时序也会变化。 导致变化的一个常见原因是指令和数据是否保留在缓存中。

### 3.2 Support for VFP-only systems

  **对于仅含VFP系统的支持**

  一些指令是NEON和VFP扩展所共有的，这些称为共享指令。

#### **注意**

  VFP和NEON单元都使用CP10和CP11协处理器的指令空间中的指令，并且共享相同的寄存器文件，这导致两种操作之间会产生混淆：

- VFP可以提供完全兼容IEEE-754的浮点运算，根据实现的不同，可以对单精度和双精度浮点值进行运算。
- NEON单元是用于整数和单精度浮点数据的并行数据处理单元，ARMv7 NEON单元中的单精度运算不完全符合IEEE-754。
- NEON单元不能代替VFP。 VFP提供了一些专门的指令，这些指令在NEON指令集中没有等效的实现。

但是，如果您的处理器同时具有NEON单元和VFP单元：

- 所有NEON寄存器与VFP寄存器重叠。
- 所有VFP数据处理指令都可以在VFP单元上执行。
- 共享指令可以在NEON浮点流水线（pipeline）上执行。

### 3.3 Support for the Half-precision extension

  **对于半精度扩展的支持**

  半精度指令仅在包含半精度扩展的NEON和VFP系统上可用。

  半精度扩展（Half-precision extention）扩展了VFP和NEON体系结构。 它提供浮点和NEON指令，可在单精度（32位）和半精度（16位）浮点数之间执行转换。

### 3.4 Support for the Fused Multiply-Add instructions

  **对于融合乘加指令的支持**

  融合乘加指令是VFP和NEON扩展的可选扩展。 它提供了VFP和NEON指令，在执行乘法和加法时，带有一个的四舍五入步骤，因此与执行乘法随后执行加法相比，精度损失较小。

  融合乘加指令仅在实现了融合乘加扩展的NEON或VFP系统上可用。 在VFPv4和先进的SIMDv2中包括对融合乘加指令的支持。

### 3.5 Security and virtualization

  **安全性和虚拟化**

  Cortex-A9和Cortex-A15处理器包括ARM安全扩展。 Cortex-A15处理器包括ARM安全扩展和虚拟化扩展。 这些扩展会影响您为NEON和VFP单元编写代码的方式。 这些扩展超出了本文的范围，有关更多信息，请参见《 ARM体系结构参考手册》 ARMv7-A和ARMv7-R。

### 3.6 Undefined instructions

  **未定义的指令**

  NEON指令（包括半精度和融合乘加指令）在不支持必要架构扩展的系统上，被视为未定义指令。

  即使在具有NEON单元和VFP单元的系统上，如果未在CP15协处理器访问控制寄存器（CPACR）中启用这些指令，则这些指令也是被视为未定义的。 有关启用NEON和VFP单元的更多信息，请参见所编程处理器的《技术参考手册》。

### 3.6 Support for ARMv6 SIMD instructions

  **对于ARMv6 SIMD指令集的支持**

  ARMv6架构添加了许多SIMD指令，以有效地实现多媒体算法的软件。 ARMv7不推荐使用ARM6 SIMD指令。

## 4. Fundamentals of NEON technology

  **NEON技术基础**

  如果您熟悉ARM v7-A架构配置文件，您会注意到ARMv7内核是32位架构，并使用32位寄存器，但是NEON单元使用64位或128位寄存器进行SIMD处理。

  之所以可以这样做，是因为NEON单元和VFP是ARMv7指令集的扩展，这些指令集在64位寄存器的单独寄存器文件中运行。 NEON和VFP单元完全集成到处理器中，并共享处理器资源以进行整数运算，循环控制和缓存。 与硬件加速器相比，这大大减少了面积并降低了功耗。 由于NEON单元使用与应用程序相同的地址空间，因此它还使用了更为简单的编程模型。

  NEON组件的组件包括：

- NEON寄存器文件。
- NEON整数执行流水线（pipeline）。
- NEON单精度浮点执行流水线（pipeline）。
- NEON加载/存储和置换（permute）流水线（pipeline）。

### 4.1 Registers, vectors, lanes and elements

  **寄存器，向量，通道，元素**

  NEON指令和浮点指令使用相同的寄存器文件，称为NEON和浮点寄存器文件。 这与ARM核心寄存器文件不同。 NEON和浮点寄存器文件是一组寄存器的集合，可以将其作为32位，64位或128位寄存器进行访问。 一条指令可使用哪些寄存器取决于它是NEON指令还是VFP指令。 本文档将NEON和浮点寄存器称为NEON寄存器。 某些VFP和NEON指令在通用寄存器和NEON寄存器之间移动数据，或使用ARM通用寄存器来寻址存储器。

  NEON寄存器的内容是相同数据类型的元素的向量。向量分为多个通道（lane），每个通道包含一个称为元素（element）的数据值。

  通常，每个NEON指令都会导致n个操作并行发生，其中n是输入向量被划分为的通道（lane）数。 每个操作都包含在通道（lane）中。 从一个通道到另一个通道之间不会有进位或溢出。

  NEON向量中的通道的数量取决于向量的大小以及向量中的数据元素：

64位NEON向量可包含：

- 8个8位元素。
- 4个16位元素。
- 2个32位元素。
- 1个64位元素。

128位NEON向量可包含：

- 16个8位元素。
- 8个16位元素。
- 4个32位元素。
- 2个64位元素。

#### Element ordering

**元素的排列**
  图3显示向量中元素的顺序是从最低有效位开始的。 这意味着元素（element）0 使用寄存器的最低有效位。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200805111210878.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图3 通道和元素



  图4展示了`VADD.I16 Q0, Q1, Q2`指令如何从`Q1`和`Q2`中的向量执行八个通道的16位（8 x 16位 = 128位）整数（`I`）元素的并行加法，并将结果存储在`Q0`中。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200805111607982.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图4 8个16位整数加法运算

#### Register overlap

**寄存器重叠**
  图5显示了NEON和浮点寄存器文件，寄存器共享同样的内存，以不同的名称访问（**Q**或**D**或**S**）。

NEON单元把寄存器文件视作：

- 16个128位**Q**寄存器，或者叫四字寄存器，`Q0`-`Q15`。

- 32个64位D寄存器，或者叫双字寄存器，D0-D31（在VFPv3-D16中是16个64位D寄存器）

  D寄存器与Q寄存器的映射关系为为：

  - `D<2n>`映射到`Q<n>`的低位那半部分。
  - `D<2n+1>`映射到`Q<n>`的高位那半部分。

- **Q**和**D**寄存器的组合。

- NEON单元无法直接访问各个32位VPF

  S寄存器。

  S寄存器的映射关系为为：

  - `S<2n>`映射到`D<n>`的低位那半部分。
  - `S<2n+1>`映射到`D<n>`的高位那半部分。

![img](https://img-blog.csdnimg.cn/20200805114553626.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70,#pic_center)

图5 NEON和浮点寄存器文件

  所有这些寄存器都可以随时访问。软件不必在它们之间进行显式切换，因为所使用的指令确定了访问形式。在图6中，**S**，**D**和**Q**寄存器是同一寄存器`Q0`的不同命名：

- NEON可以把`Q0`当做128位寄存器来访问。
- NEON可以把`Q0`当做2个连续的64位寄存器： `D0`和`D1`来访问。
- NEON不能单独`Q0`里的32位**S**寄存器。但如果存在VPF单元，则可以将其作为`S0`，`S1`，`S2`和`S3`来访问。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200805124253759.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图6 寄存器重叠与命名关系

#### Scalar data

**标量数据**
  **标量是指单个值，而不是包含多个值的向量**。 一些NEON指令使用标量操作数。 寄存器内的标量通过索引访问值向量。 访问向量的各个元素的数组符号为D m [ x ] Dm[x]Dm[x]或Q m [ x ] Qm[x]Qm[x]，其中x xx是向量D m DmDm或Q m QmQm中的索引。

  指令`VMOV.8 D0[3], R3`将寄存器`R3`的最低有效字节移至寄存器`D0`的第4字节。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200805125023857.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70)

  NEON标量可以是8位，16位，32位或64位值。 除了乘法指令，访问标量的指令可以访问寄存器文件中的任何元素。

  乘法指令仅允许使用16位或32位标量，并且只能访问寄存器文件中的前32个标量：

- 16位标量仅限于寄存器`D0[x]` - `D7[x]`，x的范围为0到3。
- 32位标量仅限于寄存器`D0[x]` - `D15[x]`，x为0或1。

### 4.2 NEON data type specifiers

  **NEON数据类型说明符**

  尽管ARM体系结构不需要处理器来实现VFP和NEON技术，但是这些扩展的编程中的共同功能意味着，支持VFP的操作系统只需很少或根本不需要修改，即可支持NEON技术。 但是，由于NEON单元在某些Cortex-A系列处理器中是可选的，因此您不能依靠NEON代码在所有处理器上均可工作。

  在使用NEON或VFP指令并在给定处理器上编译代码之前，**必须检查是否有NEON或VFP单元**。

  NEON和VFP指令中的数据类型说明符，由表示数据类型的字母组成。通常后跟表示宽度的数字。它们与指令助记符之间有一个点，例如`VMLAL.S8`。

图3 NEON数据类型

|                  | **8位** | **16位** | **32位** | **64位** |
| ---------------- | ------- | -------- | -------- | -------- |
| 无符号整形       | U8      | U16      | U32      | U64      |
| 有符号整形       | S8      | S16      | S32      | S64      |
| 未指定类型的整数 | I8      | I16      | I32      | I64      |
| 浮点数           | 不可用  | F16      | F32 或 F | 不可用   |
| 多项式{0,1}      | P8      | P16      | 不可用   | 不可用   |

  多项式类型适用于在{0,1}上使用2次幂的有限域，或简单多项式的运算。

### 4.3 VFP views of the NEON and floating-point register file

  实现ARMv7A和ARMV-7R体系结构的处理器，可包括以下的VFP扩展之一：

表4 VFPv3的几个种类

| 64位**D**寄存器 | 有半精度扩展                                       | 没有半精度扩展                                              |
| --------------- | -------------------------------------------------- | ----------------------------------------------------------- |
| 16个            | VFPv3-D16-FP16                                     | VFPv3-D32                                                   |
| 32个            | VFPv3-D32-FP16 （也叫做VFPv4，若带有融合乘加扩展） | VFPv3-D32 （有时候就叫VFPv3，如果没有与其它扩展融合的可能） |

在 VFPv3-D16和VPFv3-D16-FP16版本中，VFP单元把NEON寄存器文件当成：

- 16个64位**D**寄存器`D0`-`D15`，**D**寄存器被称为半精度寄存器，包含半精度浮点变量。
- 32个32位**S**寄存器`S0`-`S31`，**S**寄存器被称为单精度寄存器，包含单精度浮点变量或者32位整形变量。
  对于VPFv3-D16-F16，**S**寄存器可以包含半精度浮点值。
- 以上形式的寄存器组合。

在VFPv3，VFPv3-D32，和VFPv3-D32-FP16版本中，VFP单元把NEON寄存器文件当成：

- 32个64位**D**寄存器`D0`-`D31`。
- 32个32位**S**寄存器`S0`-`S31`，`D0`-`D15`与**S**寄存器`S0`-`S31`重叠。。
  对于VPFv3-D16-F16，**S**寄存器可以包含一个半精度浮点值。
- 以上形式的寄存器组合。

#### 注意

- 不同的访问形式表示半精度，单精度和双精度值，并且NEON向量同时共存于不同的非重叠寄存器中。
- 您还可以在不同时间，使用相同的重叠寄存器存储半精度，单精度和双精度值以及NEON向量。

表5显示了VFP指令集的可用数据类型。

表5 VFP数据类型

|            | **16位** | **32位**  | **64位**  |
| ---------- | -------- | --------- | --------- |
| 无符号整形 | U16      | U32       | 不可用    |
| 有符号整形 | S16      | S32       | 不可用    |
| 浮点数     | F16      | F32 (或F) | F64 (或D) |













## 1. Vectorization

  **向量化**

  NEON被设计成一种附加的加载/存储[体系结构](https://so.csdn.net/so/search?q=体系结构&spm=1001.2101.3001.7020)，以提供对C / C ++等语言的良好向量化编译器支持。这实现了高度的并行性，可以手动编码NEON指令，实现非常高性能的应用程序。它包括低成本升级和降低数据大小，还包括结构化的加载能力，可以访问那些交错在内存的多个数据流。

  可以把NEON当做常规ARM代码的一部分来写。与使用外部硬件加速器相比，这使NEON编程更简单，更高效。NEON指令可用于读取和写入外部存储器，在NEON寄存器和其他ARM寄存器之间移动数据，以及执行SIMD操作。

  所有已编译的代码和子例程都将遵循EABI（Embedded Application Binary Interface），EABI指定了哪些寄存器可能损坏，以及哪些寄存器必须保留。

  [向量化](https://so.csdn.net/so/search?q=向量化&spm=1001.2101.3001.7020)编译器可以获取您的C或C ++源代码，并将其向量化，从而可以有效利用NEON硬件。 这意味着您可以编写可移植的C代码，同时仍然获得NEON指令所能实现的性能水平

  为了辅助向量化，请将循环迭代的次数设为向量长度的倍数。 GCC和ARM编译器工具链都有选项，可以为NEON技术启用自动向量化。但由于C和C ++标准未涵盖并发方面，因此要向编译器提供其他信息，才能得到NEON的好处。 修改的源代码也是是标准语言规范的一部分，因此不会影响代码在平台和体系结构之间的可移植性。

  当向量化编译器可以确定程序员的意图时，其效果最佳。 相比针对特定处理器进行了高度优化的代码，人们能够理解的简单代码更容易向量化。



### 1.1 Enabling auto-vectorization in ARM Compiler toolchain

  **在ARM交叉编译链中启动自动向量化**

  DS-5™Professional中的ARM编译器工具链包括对向量化编译器的支持。 要启用自动向量化，您必须针对具有NEON单元的处理器。 所需的命令行选项是：

- `--vectorize` ，启动向量化
- `--CPU 7A` 或 `--Cpu Cortex A8` ，选择指定的架构和核，支持NEON
- `-O2` 或 `-O3`，选择高级或者低级的优化
- `-Otime` ，为速度做优化（而不针对空间）

  使用armcc命令行参数`--remarks`可提供有关执行优化的更多信息，或提供有关编译器无法执行某些优化的问题。



### 1.2 Enabling auto-vectorization in GCC compiler

  **在GCC编译其中启动自动向量化**

  为了在GCC中启动自动向量化，用下面的命令行选项：

- `-ftree-vectorize`
- `-mfpu=neon`
- -`mcpu` ，指定内核或者架构

  用优化级别`-O3`编译，相当于`-ftree-vectorize`。

  如果没有指定`-mcpu`选项，则GCC将使用内置的默认内核，生成的代码可能运行缓慢或根本不运行。

  在许多支持SIMD操作的架构上，`-ftree-vectorize`是可用的。



### 1.3 C pointer aliasing

  **C指针别名**

  优化标准C（ISO C90）的主要挑战是，你可以解引用指针，而这个指针可能（根据标准）指向相同或重叠数据集。这会导致什么问题？后面的例子有讲。

  随着C标准的发展，通过在C99和C++中添加关键字`limit`来解决此问题。在指针声明中添加限制是一种保证，只有该指针才能访问其指向的地址（就像unique_ptr智能指针）。 这使编译器可以进行设置和退出限制方面的工作，可以在事先通知的情况下预加载数据，并缓存中间结果。

  ARM编译器允许在所有模式下使用关键字`__restrict`。如果指定命令行选项`--restrict`，则可以使用不带下划线的关键字`restrict`。GCC有类似的选择,有关更多信息，请参见GCC文档。



### 1.4 Natural types

  **自然类型**

  由于历史原因、内存大小、外围因素等，算法常被设计用于处理某种类型数据。通常将这些类型转换成处理器的自然类型，因为对这种大小的数据进行数学运算通常会更快，并且仅在传递或存储数据时才计算截断和溢出。



### 1.5 Array grouping

  **数组分组**

  处理器中用于存储内存指针的寄存器很少（例如x86），对于这样的处理器，通常将多个数组组合起来。 然后用一个指针，通过指针位移来访问不同部分的数据。这会让编译器误以为偏移指针导致数据重叠。除非您可以保证不对阵列进行任何写操作，才能避免这种情况。将复合数组拆分为单个数组，以简化指针的使用能消除这种风险。



### 1.6 Inside knowledge

  **内部知识**

  要把没有size信息的数组转换为NEON代码，编译器必须假定size可能在0到4GB之间。 在没有其他信息的情况下，编译器必须生成设置代码（用于测试数组是否太小而无法使用整个NEON寄存器），以及用于清理的代码（使用标量管道(pipeline)来消耗数组中的最后几项）。

  在某些情况下，数组大小在编译时是已知的，应直接指定而不是作为参数传递。在其他情况下，工程师通常比编译器更了解矩阵的布局。例如，数组通常表示为2的幂。程序员可能知道循环迭代计数将始终是2、4、8、16、32的倍数，依此类推。可以编写代码来利用它。



### 1.7 Enabling the NEON unit in bare-metal applications

  **在裸机应用中启用NEON单元**

  裸机应用程序是直接在硬件上运行的应用程序，无需内核或操作系统支持。
  默认情况下，NEON单元在重置时处于禁用状态，因此对于需要NEON指令的裸机应用，必须手动启用它。`EnableNEON`代码段显示了如何手动启用NEON设备。

```c
// hello.c:

#include <stdio.h>
// Bare-minimum start-up code to run NEON code
__asm void EnableNEON(void)
{
    MRC p15,0,r0,c1,c0,2 // Read CP Access register
    ORR r0,r0,#0x00f00000 // Enable full access to NEON/VFP by enabling access to Coprocessors 10 and 11
    MCR p15,0,r0,c1,c0,2 // Write CP Access register
    ISB
    MOV r0,#0x40000000 // Switch on the VFP and NEON hardware
    MSR FPEXC,r0 // Set EN bit in FPEXC
}
12345678910111213
```

  为带有NEON单元的处理器编译裸机应用程序时，编译器可能会使用NEON指令。例如，ARM编译器工具链armcc默认情况下使用`-O2`优化，如果指定了`-Otime`和`--vectorize`选项，则它会尝试使用NEON单元对处理器的代码进行向量化。

  可以像这样把上面的代码编译成裸机应用程序：

```markup
armcc -c --cpu=Cortex-A8 --debug hello.c -o hello.o
armlink --entry=EnableNEON hello.o -o hello.axf
12
```



### 1.8 Enabling the NEON unit in a Linux stock kernel

  **在原版Linux内核中启用NEON单元**

  原版的内核在Linux在www.kernel.org上发布，没有经过修改。如果您使用原版的Linux内核来运行应用程序，则无需手动启用NEON单元。内核在遇到第一条NEON指令时自动启用NEON单元。

  如果NEON单元被禁用，而应用程序尝试执行NEON指令，它将抛出<未定义的指令>异常。内核使用这个异常来启用NEON单元，然后执行NEON指令。NEON单元保持启用状态，直到有上下文切换。当需要上下文切换时，内核可能会禁用NEON单元以节省时间和电源。



### 1.9 Enabling the NEON unit in a Linux custom kernel

  **在自定义的Linux内核中启用NEON单元**

  如果使用修改过的的Linux内核运行应用程序，则必须启用NEON单元。要启用NEON单元，必须使用内核配置设置来选择：

- **Floating point emulation → VFP-format floating point maths**
- **Floating point emulation → Advanced SIMD (NEON) Extension support**

图1显示了通用快速板的配置设置。

图1 定制Linux内核的设置

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200806104726151.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70)

  如果存在`/proc/config.gz`，则可以使用以下命令在内核中测试对NEON的支持：

```bash
zcat /proc/config.gz | grep NEON
1
```

  如果存在NEON单元，命令会输出：

```bash
CONFIG_NEON=y
1
```

  为了确保处理器支持NEON扩展，可以使用命令：

```bash
cat /proc/cpuinfo | grep neon
1
```

  如果它支持NEON扩展，则输出：

```bash
Features : swp half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt
1
```



### 1.10 Optimizing for vectorization

  **向量化优化**

  C和C++语言没有提供指定并发行为的语法，因此编译器无法安全地生成并行代码。但是，开发人员可以提供其他信息，让编译器知道在什么地方可以安全地进行向量化。

  与内部函数（intrinsics）不同，这些修改不依赖于体系结构，并且可能会改善任何目标平台上的向量化。在那些无法进行向量化的平台上，这些修改不会对性能造成负面影响。

  下面描述主要规则：

- 短、简单的循环效果最好（即使代码中有多个循环）
- 避免用break来退出循环
- 把迭代次数设为2的幂
- 让编译器能明确迭代次数
- 在一个循环内部调用的函数，应该用内联（inlined）函数
- 在数组中，用索引好过用指针
- 间接寻址（多个索引或解引用）不会向量化。
- 使用`restrict`关键字告诉编译器，让指针不引用内存的重叠区域。



#### Indicate knowledge of number of loop iterations

**关于循环迭代次数的知识**

  如果循环具有固定的迭代计数，或者您知道迭代计数始终为2的幂，则对于编译器而言显而易见的是，这使编译器能够执行优化，否则该优化是不安全的或困难的。

  下面的代码显示了一个函数，该函数计算`int`大小的元素个数（len）。如果您知道传进来的参数`len`始终是4的倍数，在比较循环次数的时候，将最低两位置零（`len & ~3`），通过这种方式向编译器指示这一点。这样可以确保循环始终执行4的倍数次。这样编译器可以安全地向量化，与此同时：

- 无需为了在运行时检查`len`而添加代码。
- 无需添加代码来处理悬空的迭代。

```c
//  指定循环计数器是4的倍数
int accumulate(int * c, int len)
{
    int i, retval;
    for(i = 0, retval = 0; i < (len & ~3) ; i++)  // 
    {
        retval = retval + c[i]
    }
    return retval;
}
12345678910
```

例1 指定循环计数器为4的倍数



#### Avoid loop-carried dependencies

**避免循环携带的依赖**

  如果您的代码包含一个循环，某次循环受到上一次循环结果的影响，则编译器无法对其进行向量化。最好重新检查代码，消除这种循环中的依赖。



#### Use the restrict keyword

**使用`restrict`关键字**

  C99引入了`restrict`关键字，可用于通知编译器，通过特定指针访问的位置，不能通过当前作用域内的任何其他指针访问。
  下面的代码展示了一种情况，其中`e`指向要更新位置，对这个指针使用`restrict`会使向量化变得安全，否则就不安全。

```c
int accumulate2(char * c, char * d, char * restrict e, int len)
{
    int i;
    for(i=0 ; i < (len & ~3) ; i++)
    {
        e[i] = d[i] + c[i];
    }
    return i;
}
123456789
```

例2

  如果没有`restrict`关键字，则编译器会假定`e[i]`可以引用与`d[i+1]`相同的位置，这可能会产生循环依赖关系，从而阻止它对向量序列进行向量化处理。程序员可以用`restrict`告诉编译器，通过`e`访问的任何位置只能通过此函数中的指针`e`访问。这意味着编译器可以忽略混叠的可能性，并可以对序列进行向量化处理。

  使用`restrict`关键字不能使编译器对函数调用执行其他检查。因此，如果传递给函数的`c`或`d`值与`e`的值重叠，则向量化代码可能无法正确执行。

  编译的不是C99标准的代码时，GCC和ARM编译器工具链都支持替代形式`__restrict__`和`__restrict`。 当在编译命令行指定了`--restrict`时，ARM编译工具链还支持在C90和C++中使用`restrict`关键字。



#### Avoid conditions inside loops

**避免循环内的条件**

  通常，编译器无法向量化包含条件语句的循环。在最佳情况下，它会复制循环，但在许多情况下根本无法向量化。



#### Use suitable data types

**使用合适的数据类型**

  当在没有SIMD的情况下，优化某些对16位或8位数据运行的算法时，将它们当做32位变量来用，有时会获得更好的性能。在写自动地向量化的软件时，为了获得最佳性能，请始终使用可容纳所需值的最小数据类型。这样NEON寄存器可以容纳更多数据元素，并并行执行更多操作。在给定的周期内，NEON单元可以处理的8位值是16位值的两倍。

  另外，NEON技术不支持某些数据类型，而某些操作仅支持某些数据类型。例如，它不支持双精度浮点数，因此在单精度浮点数足够的情况下，使用双精度数可能会阻止编译器对代码进行向量化处理。NEON技术仅对某些操作支持64位整数，因此请尽可能避免使用`long long`变量。

#### 注意

  NEON技术包括一组可以执行结构化加载和存储操作的指令。这些指令只能用于向量访问所有成员大小均相同的数据结构。



#### Floating-point vectorization

**浮点向量化**

  浮点运算可能会导致精度降低。可以再排列一下操作的顺序或浮点输入的数据，以减少精度损失。更改操作或输入的顺序可能会导致精度进一步下降。 因此，默认情况下不会对某些浮点运算进行向量化，因为向量化会更改操作的顺序，导致程序逻辑的改变。如果算法不需要这种精度水平，则可以在命令行上为armcc指定`--fpmode=fast`或为GCC指定`-ffast-math`以启用这些优化。

  下面展示了一段代码，使用上面其中一个命令行参数来进行向量化。在这种情况下，它将执行并行累加，可能会降低结果的精度。

```c
float g(float const *a)
{
	float r = 0;
	int i;
	for (i = 0 ; i < 32 ; ++i)
		r += a[i];
	return r;
}
12345678
```

例3

  NEON单元始终以“对齐到零（Flush-to-zero）”模式运行，导致不符合IEEE754。默认情况下，armcc使用`--fpmode=std`以符合标准。 但是，如果命令行参数指定要求符合IEEE 754的模式选项，例如`--fpmode=ieee_full`，则大多数浮点操作无法向量化。



## 2. Generating NEON code using the vectorizing compiler

**使用向量化编译器生成NEON代码**

  向量化编译器会预估可向量化的循环，以及潜在的NEON应用。如果你写的C或C ++代码使编译器可以确定代码的意图，则编译器将更有效地对其进行优化。尽管编译器可以在不修改源代码的情况下生成一些NEON代码，但是某些编码样式可以促进更佳的输出。当向量化器（vectorizer）发现代码具有潜在的向量化机会，但没有足够的信息时，它可以为用户生成注释，以提示用户对源代码进行更改，以提供更多有用的信息。尽管这些修改能帮助向量化编译器，但它们都是标准C表示法，并允许使用任何符合C99*的编译器进行重新编译。解析`restrict`关键字需要用到C99。在其他编译模式下，armcc还允许使用等效的ARM特定扩展名`__restrict`。



### 2.1 Compiler command line options

**编译器命令行选项**

  如果有向量化许可证选项，可以通过使用`O2`或`O3`，`Otime`，`vectorize`和`cpu`选项告诉编译器生成NEON代码。`cpu`选项必须指定具有NEON硬件的处理器。

  由于数组清理和其他开销，SIMD代码有时大于等效的ARM代码。

  要在Cortex-A8目标上生成快速NEON代码，应使用以下命令行：

```bash
armcc --cpu=Cortex-A8 -O3 -Otime --vectorize ...
1
```

  如果您没有向量化编译器的许可证，则此命令将回复一条错误消息。



#### Using the vectorizing compiler

**使用向量化编译器**

我们可以简洁地编写C代码：

```c
/* file.c */
unsigned int vector_add_of_n(unsigned int* ptr, unsigned int items)
{
    unsigned int result=0;
    unsigned int i;
    
    for (i=0; i<(items*4); i+=1)
    {
        result += ptr[i];
    }
    
    return result;
}
12345678910111213
```

#### **注意**

- 通过使用`(items*4)`，我们告诉编译器数组的大小是4的倍数，这种就属于有效信息。尽管向量化程序不需要创建NEON代码，但它为编译器提供了有关数组的更多信息。在这种情况下，它知道该数组可以与向量数组一起使用，并且不需要任何额外的标量代码来处理任何备用项的清理。
- 传递给函数的值`items`值必须是数组中实际项目数的1 / 4 1/41/4，例如：

```c
vector_add_of_n(p_list, item_count/4);
1
```

  用以下命令编译上述代码：

```bash
armcc --cpu=Cortex-A8 -O3 –c -Otime –-vectorize file.c
1
```

  使用命令`fromelf –c file.o`查看生成的文件：

```cpp
vector_add_of_n PROC
LSLS r3,r1,#2
MOV r2,r0
MOV r0,#0
BEQ |L1.72|
LSL r3,r1,#2
VMOV.I8 q0,#0
LSRS r1,r3,#2
BEQ |L1.48| |L1.32|
VLD1.32 {d2,d3},[r2]!
VADD.I32 q0,q0,q1
SUBS r1,r1,#1
BNE |L1.32| |L1.48|
CMP r3,#4
BCC |L1.72|
VADD.I32 d0,d0,d1
VPADD.I32 d0,d0,d0
VMOV.32 r1,d0[0]
ADD r0,r0,r1 |L1.72|
BX lr
1234567891011121314151617181920
```

  尽管这代码比手写的代码长，但例程的主要部分（内部循环）的长度相同，并且包含相同的指令。 这意味着，如果数据集大小合理，则执行的时间差很小。但是在Cortex-A8处理器上，与手写代码相比，编译器生成的代码是非最佳计划的。因此，Cortex-A8处理器的性能差异随数据集大小而定。



## 3. Vectorizing examples

**使用向量化编译器生成NEON代码**



### 3.1 Vectorization example on unrolling addition function

**展开附加功能的向量化示例**

  考虑下面的代码：

```c
Void add_int (int * __restrict pa, int * __restrict pb, unsigned int n, int x)
{
    unsigned int i;
    for (i = 0; i < (n & ~3); i++)
        pa[i] = pb [i] + x;
}
123456
```

1.分析每次循环：

- 指针访问对向量化安全吗？
- 正在使用什么数据类型，它们如何映射到NEON寄存器？
- 有多少个循环迭代？

2.将循环展开到适当的迭代次数，然后执行其他转换，例如使用指针。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200806160210988.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70)

3.将每个展开的操作映射到NEON向量通道上，并生成相应的NEON指令。



### 3.2 Vectorizing example with vectorizing compilation

**向量化示例与向量化编译**

  下表显示了使用向量化编译和不使用向量化编译的代码的比较：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080616124747.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70)



### 3.3 Vectorizing examples with different command line switches

**用不同的命令行，对例子进行向量化**

  本节包含非常简单的编译器向量化例子。 这些示例显示了各种命令行开关的影响以及各种源代码更改的影响。



#### Optimized for code space

**针对代码空间进行了优化**

  在此示例中，编译器选项`-Ospace`生成较小的代码， 生成的代码未针对速度进行优化。

```c
void add_int (int *pa, int * pb, unsigned int n, int x)
{
    unsigned int i;
    for(i = 0; i < n; i++)
        pa[i] = pb[i] + x;
}
123456
```

例4 优化代码空间

  附带下面的选项来编译：

```bash
armcc --cpu=Cortex-A8 -O3 -Ospace
1
```

  汇编结果：

```markup
add_int PROC
    PUSH {r4, r5, 1r}
    MOV r4, #0
|L0.8|
    CMP r4,r2
    LDRCC r5, [r1, r4, LSL, #2]
    ADCC r5, r5, r3
    STRCC r5, [r0, r4, LSL, #2]
    ADCC r4,r4, #1
    BCC |L0.8|
    POP {r4, r5, pc}
1234567891011
```

例5 生成的汇编语言



#### Optimized for time

**针对时间优化**

  在此示例中，编译器选项`-Otime`生成代码， 结果比`例4`的代码更快更长。

```c
void add_int (int *pa, int * pb, unsigned int n, int x)
{
    unsigned int i;
    for(i = 0; i < n; i++)
        pa[i] = pb[i] + x;
}
123456
```

例6 针对时间优化

  附带下面的选项来编译：

```bash
armcc --cpu=Cortex-A8 -O3 -Otime
1
```

汇编结果：

```markup
add_int PROC
    CMP r2,#0
    BXEQ lr
    TST r2,#1
    SUB r1,r1,#4
    SUB r0,r0,#4
    PUSH {r4}
    BEQ |L0.48|
    LDR r4,[r1,#4]!
    ADD r12,r0,#4
    ADD r4,r4,r3
    STR r4,[r0,#4]
    MOV r0,r12
|L0.48|
    LSRS r2,r2,#1
    BEQ |L0.96|
|L0.56|
    LDR r12,[r1,#4]
    SUBS r2,r2,#1
    ADD r12,r12,r3
    STR r12,[r0,#4]
    ADD r12,r0,#8
    LDR r4,[r1,#8]!
    ADD r4,r4,r3
    STR r4,[r0,#8]
    MOV r0,r12
    BNE |L0.56|
|L0.96|
    POP {r4}
    BX lr
123456789101112131415161718192021222324252627282930
```

例7 生成的汇编代码



#### Optimized using knowledge of array size

**用已知数组大小来优化**

  在此示例中，将for循环迭代范围设置为`(n & ~3)`。 这告诉编译器数组`pa`的大小是4的倍数。

```c
void add_int (int *pa, int * pb, unsigned int n, int x)
{
    unsigned int i;
    for(i = 0; i < (n&~3); i++)
        pa[i] = pb[i] + x;
}
123456
```

例8 用已知数组大小来优化

  附带下面的选项来编译：

```bash
armcc --cpu=Cortex-A8 -O3 -Otime
1
```

汇编结果：

```markup
add_int
    BICS r12,r12,#3
    BXEQ lr
    LSR r2,r2,#2
    SUB r1,r1,#4
    SUB r0,r0,#4
    LSL r2,r2,#1
    PUSH {r4}
|L0.28|
    LDR r12,[r1,#4]
    SUBS r2,r2,#1
    ADD r12,r12,#3
    STR r12,[r0,#4]
    ADD r12,r0,#8
    LDR r4,[r1,#8]!
    ADD r4,r4,r3
    STR r4,[r0,#8]
    MOV r0,r12
    BNE |L0.28|
    POP {r4}
    BX lr
123456789101112131415161718192021
```

例9 生成的汇编代码



#### Optimized using auto-vectorize and knowledge of array size

**用自动向量化和已知数组大小来优化**

  在此示例中，编译器选项`--vectorize`使编译器使用NEON指令`VLD1`，`VADD`和`VST1`。

```c
void add_int (int *pa, int * pb, unsigned int n, int x)
{
    unsigned int i;
    for(i = 0; i < (n&~3); i++)
        pa[i] = pb[i] + x;
}
123456
```

例10

  附带下面的选项来编译：

```bash
armcc --cpu=Cortex-A8 -O3 -Otime --vectorize
1
```

汇编结果：

```markup
add_int
    PUSH r4,r5}
    SUB r4,r0,r1
    ASR r12,r4,#2
    CMP r12,#0
    BLE |L0.32|
    BIC r12,r2,#3
    CMP r12,r4,ASR #2
    BHI |L0.76|
|L0.32|
    BICS r12,r2,#3
    BEQ |L0.68|
    VDUP.32 q1,r3
    LSR r2,r2,#2
|L0.48|
    VLD1.32 {d0,d1},[r1]!
    SUBS r2,r2,#1
    VADD.I32 q0,q0,q1
    VST1.32 {do,d1},[r0]!
    BNE |L0.48|
|L0.68|
    POP {r4,r5}
    BX lr
|L0.76|
    BIC r2,r2,#3
    CMP r2,#0
    BEQ |L0.68|
    MOV r2,#0
    BLS |L0.68|
|L0.96|
    LDR r4,[r11,r2,LSL #2]
    ADD r5,r0,r2,LSL #2
    ADD r4,r4,r3
    STR r4,[r0,r2,LSL #2]
    ADD r4,r1,r2,LSL #2
    ADD r2,r2,#2
    LDR r4,[r4,#4]
    CMP r12,r2
    ADD r4,r4,r3
    STR r4,[r5,#4]
    BHI |L0.96|
    POP {r4,r5}
    BX lr
12345678910111213141516171819202122232425262728293031323334353637383940414243
```

例11



#### Optimized using the restrict keyword

**使用限制关键字进行了优化**

  本例中，数组指针`pa`和`pb`使用了关键字`restrict`。

```c
void add_int (int* restrict pa, int* restrict pb, unsigned int n, int x)
{
    unsigned int i;
    for(i = 0; i < (n&~3); i++)
        pa[i] = pb[i] + x;
}
123456
```

例12

  附带下面的选项来编译：

```bash
armcc --cpu=Cortex-A8 -O3 -Otime --vectorize
1
```

汇编结果：

```markup
add_int PROC
    BICS r12,r2,#3
    BEQ |L0.36|
    VDUP.32 q1,r3
    LSR r2,r3,#2
|L0.16|
    VLD1.32 {d0, d1}, [r1]!
    SUBS r2,r2,#1
    VADD.I32 q0,q0,q1
    VST1.32 {d0,d1},[r0]!
    BNE |L0.16|
|L0.36|
    BX lr
12345678910111213
```

例13



## 4. NEON assembler and ABI restrictions

**NEON汇编程序和ABI限制**

  对于极高的性能，手写NEON汇编程序是经验丰富的程序员的最佳方法。GNU汇编器（gas）和ARM编译器工具链汇编器（armasm）均支持NEON指令的汇编。编写汇编器函数时，您必须注意ARM EABI，它决定了如何使用寄存器。ARM嵌入式应用程序二进制接口（Embedded Application Binary Interface EABI） 指定哪些寄存器用于传递参数，返回结果、或必须保留。 除了ARM内核寄存器外，它还指定了32个**D**寄存器的使用，下表对此进行了总结。

| D0 - D7   | 参数寄存器和返回寄存器。 如果子例程没有参数或返回值，则这些寄存器中的值可能未初始化。 |
| --------- | ------------------------------------------------------------ |
| D8 - D15  | callee-saved寄存器（callee:被调用者）                        |
| D16 - D31 | caller-saved寄存器（caller:调用者）                          |

  通常，数据值实际上没有传递给NEON代码子例程（subroutine），因此使用NEON寄存器的最佳顺序是：

1. `D0` - `D7`
2. `D16` - `D31`
3. `D8` - `D15`（如果他们被保存了）

  子例程（subroutines）必须在使用寄存器`S16`-`S31`（也是`D8`-`D15`，`Q4`-`Q7`）之前保存（preserve）他们。
  寄存器`S0`-`S15`（`D0`-`D7`，`Q0`-`Q3`）不需要被保存（preserved），他们可以在标准程序调用的变量中，用于传递参数或者返回结果。
  寄存器`D16`-`D31`（`Q8`-`Q15`）不需要被保存（preserved），

  程序调用标准（Procedure Call Standard）指定了两种方法，可以传递浮点参数：

- 对于软件浮点，他们通过ARM寄存器`R0`-`R3`来传递，如果有要求，可以在栈上。
- 另外一种，处理器中存在浮点硬件，是在NEON寄存器中传递参数。



### 4.1 Passing arguments in NEON and floating-point registers

**在NEON和浮点寄存器中传递参数**

  这种硬件浮点变量表现为以下方式：
  整数参数的处理方式与softfp中的处理方式完全相同。因此，如果我们考虑下面的函数f，我们会看到32位值的a将传递给`R0`中的函数，并且由于值b必须在一对奇数/偶数寄存器中传递，因此它将进入`R2` / `R3`，而`R1`没有被使用。

```c
void f(uint32_t a, uint64_t b)
    r0: a
    r1: unused
    r2: b[31:0]
    r3: b[63:32]
12345
```

  FP参数填充D0-D7（或S0-S15），与任何整数参数无关。 这意味着整数参数可以流到堆栈上，并且FP参数仍将放入NEON寄存器中（如果有足够的可用空间）。

  FP参数能够回填（back-fill），因此我们看在整形参数中，很少去获取未使用的插槽。（FP arguments are able to back-fill, so it’s less common to get the unused slots that we see in integer arguments. ）考虑下面的示例：

```c
void f(float a, double b)
    d0:
        s0: a
        s1: unused
    d1: b
12345
```

  此处，`b`在被分配到`d1`的时候自动对齐（`d1`与VFP s2 / s3占用相同的物理寄存器）。
void f(fl

```c
void f(float a, double b, float c)
    d0:
    s0: a
    s1: c
    d1: b
12345
```

  在此示例中，编译器能够将`c`放入`s1`中，因此不需要将其放入`s4`中。

  实际上，这是通过对参数`s`，`d`和`q`使用单独的计数器来实现（并描述）的，并且计数器始终指向该大小的下一个可用插槽（slot）。在上面的第二个FP示例中，首先分配`a`，因为它在列表中排在第一位，并且进入第一个可用的**S**寄存器，即`s0`。接下来，将`b`分配到第一个可用的**D**寄存器，该寄存器为`d1`，因为`a`使用的是`d0`的一部分。分配`c`时，第一个可用的**S**寄存器为`s1`。随后的双或单参数将分别输入`d2`或`s4`。

  还有一种情况是把参数填到NEON寄存器：当参数必须要溢出到栈的时候，就不会发生回填，并且为其他参数分配堆栈槽的方式，与为整数参数分配方式完全相同。

```c
void f(double a, double b, double c, double d, double e, double f, float g, double h, double i, float j)
    d0: a
    d1: b
    d2: c
    d3: d
    d4: e
    d5: f
    d6:
    s12: g
    s13: unused
    d7: h
    *sp: i
    *sp+8: j
    *sp+12: unused (4 bytes of padding for 8-byte sp alignment)
1234567891011121314
```

  参数`a`-`f`按照预期分配给`d0`-`d5`。单精度的`g`分配到`s12`，`h`到`d7`去了。下一个参数`i`无法填到寄存器里，所以存到栈上。（它将会和栈上的整数参数交错到一起，要是有的话）。然而`s13`仍然没有被使用，`j`必须去到栈上，因为当FP参数到达堆栈时，我们无法回填到寄存器。

  **D**和**Q**寄存器也可用于保存矢量数据。这种情况不会发生在典型的C代码中。

  可变参数过程（即没有固定数量的参数的过程）不使用NEON寄存器。 相反，它们在softfp中被处理，因为它们在ARM内核寄存器（或堆栈）中传递。 请注意，单精度可变参数会转换为双精度，如softfp一样。



## 5. NEON libraries

**NEON库**

有一些免费的开源软件，让NEON得以使用，例如：

- **Ne10**库函数，这些函数的C接口，提供汇编程序和NEON实现。 参http://projectne10.github.com/Ne10/
- **OpenMAX**，一组用于处理音频，视频和静止图像的API。 它是Khronos组创建的标准的一部分。 有为了NEON的OpenMAX DL层的免费ARM实现。 参见http://www.khronos.org/openmax/
- **ffmpeg**，是LGPL许可下的许多不同音频和视频标准的编解码器集合，见http://ffmpeg.org/
- **Eigen3**，是线性代数、矩阵数学C++的模板库，位于eigen.tuxfamily.org/
- **Pixman**，一个2D图形库（Cairo图形的一部分），位于http://pixman.org/
- **x264**，一个免版权的GPL H.264视频编码器，位于http://www.videolan.org/developers/x264.html
- **Math-neon**，位于http://code.google.com/p/math-neon/



## 6. Intrinsics

  NEON C / C++内部函数（intrinsics）在armcc，GCC / g++和llvm中可用。 它们使用相同的语法，因此可以使用任何这些编译器，来编译使用内部函数（intrinsics）的源代码。 它们在第4章中有更详细的描述。

  NEON内部函数（intrinsics）提供了一种编写NEON代码的方法，该方法比汇编程序代码更易于维护，同时仍然可以控制生成的NEON指令。

  内部函数（intrinsics）使用新的数据类型，与NEON寄存器**D**和**Q**相对应。 数据类型允许创建C变量，这些变量直接映射到NEON寄存器。

  NEON内部函数（intrinsics）的编写类似于函数调用，该函数使用这些变量作为参数或返回值。 但是，编译器将内部函数直接转换为NEON指令，而不执行子例程调用。

  NEON内部函数（intrinsics）提供对NEON指令的低级访问。编译器帮你做了那些很难的工作，这些工作通常要用写汇编语言来做，例如：

- 寄存器分配
- 代码调度或重新排序指令，以获得最佳性能。 C编译器知道目标处理器是什么，它们可以对代码重新排序以确保最少的停顿数。

  内在函数（intrinsics）的主要缺点是无法使编译器准确输出所需的代码，因此在转至NEON汇编程序代码时仍有改进的可能性。



## 7. Detecting presence of a NEON unit

**检测NEON单元的存在**

  由于在处理器的实现中可以省去NEON单元，因此可能有必要测试NEON单元是否存在。



## 7.1 Build-time NEON unit detection

**编译时检测NEON单元**

  这是检测NEON单元是否存在的最简单方法。在ARM编译器工具链（armcc）v4.0和更高版本或GCC中，当向编译器提供了一组合适的处理器和FPU选项时，将会定义预定义的宏`__ARM_NEON__`。 armasm等效的预定义宏是`TARGET_FEATURE_NEON`。
  可以将其用于C源文件，该源文件针对具有NEON单元的系统和不具有NEON单元的系统优化了代码。



## 7.2 Run-time NEON unit detection

**运行时检测NEON单元**

  要在运行时检测NEON单元，需要操作系统的帮助。这是因为ARM体系结构故意不向用户模式的应用程序开放处理器功能。在Linux下，`/proc/cpuinfo`里用人话描述了这些信息。

  在Tegra2（带有FPU的双核Cortex-A9处理器），cat `/proc/cpuinfo`显示:

```markup
…
Features : swp half thumb fastmult vfp edsp thumbee vfpv3 vfpv3d16
…
123
```

  带有NEON单元的ARM四核Cortex-A9处理器显示不同的结果:

```markup
…
Features : swp half thumb fastmult vfp edsp thumbee neon vfpv3
…
123
```

  由于`/proc/cpuinfo`输出是基于文本的，因此通常最好查看辅助向量`/proc/self/auxv`。 它包含二进制格式的内核`hwcap`。 可以轻松地在`/proc/self/auxv`文件中搜索`AT_HWCAP`记录，以检查`HWCAP_NEON`位（4096）。

  一些Linux发行版（例如Ubuntu 09.10或更高版本）透明地利用NEON单元。修改了`ld.so`链接程序脚本，以通过`glibc`读取`hwcap`。还为了启用NEON的共享库，添加其他搜索路径。对于Ubuntu，新的搜索路径`/lib/neon/vfp`包含来自`/lib`的经过NEON优化的库版本。



## 8. Writing code to imply SIMD

## 8.1 Writing loops to imply SIMD

  当数据结构化地存储时，优秀地做法是写个寻缘同时使用结构的所有内容。这样可以更好地利用缓存。对于工作寄存器很少的机器，可能会把处理流程分到三个单独的循环：

```c
for (...) { outbuffer[i].r = ....; }
for (...) { outbuffer[i].g = ....; }
for (...) { outbuffer[i].b = ....; }
123
```

  通过简单的重写，合并为一个循环，可以在具有高速缓存的处理器上获得更好的结果。这还允许向量化编译器访问结构的每个部分并向量化循环：

```c
for (...)
{
outbuffer[i].r = ....;
outbuffer[i].g = ....;
outbuffer[i].b = ....;
}
123456
```



## 8.2 Tell the compiler where to unroll inner loops

**告诉编译器在哪里展开内部循环**

  对于armcc，可以使用以下语句：

```markup
#pragma unroll (n)
1
```

  在for语句之前，要求编译器将循环展开一定次数。您可以使用它来指示编译器可以展开内部循环，这可以使编译器在更复杂的算法中，向量化外部循环。

  其他编译器可能有不同的选项来展开内部循环。



## 8.3 Write structures to imply SIMD

  “加载”操作使用结构体的所有数据项，编译器仅对“加载”操作进行向量化。在某些情况下，会填充结构以保持对齐。下面程序显示了一个像素结构，把它扩展1个Byte，以将每个像素对齐到1个Word（这里一个word是32位）。由于有未使用的字节，编译器不会向量化它的“加载”操作。NEON的“加载”指令可以加载未对齐的结构。因此，在这种情况下，最好**删除**这个填充的字节，使编译器可以向量化“加载”操作。

```c
struct aligned_pixel 
{ 
    char r; 
    char g;
    char b; 
    char not_used; /* Padding used to keep r aligned to a 32-bit word */
}screen[10];
1234567
```

例14

  NEON的结构加载指令要求结构体的所有元素长度相同，因此下面例15的代码不会被向量化

```c
struct pixel
{
    char r;
    short g; /* Green channel contains more information */
    char b;
}screen[10];
123456
```

例15

  要是`g`必须要更高的精度，请考虑把其它变量的长度也扩展，好让这个结构体能被向量化地加载。



## 9. GCC command line options

**GCC命令行选项**
  GCC的用于ARM处理器的命令行选项最初是好多年前设计的，那时候的架构比现在的要简单。随着体系结构的发展，为了更好滴生成代码，命令行选项也改了。我们已经努力尝试确保现有选项集的含义不会被改变。 现在需要一套复杂的选项才能让编译器为Cortex-A系列处理器生成最佳的代码。主要的选项有`-mcpu`，`-mfpu`和`-mfloat-abi`。



## 9.1 Option to specify the CPU

**用于指定CPU的选项**

  编译文件的时候，必须要让编译器知道你的代码要在什么处理器上运行。所以一个主要的选项是`-mcpu=[cpu-name]`，`[cpu-name]`是小写的处理器名字，例如在Cortex-A9上就是`-mcpu=cortex-a9`，GCC支持下表的Cortex-A系列处理器：

| CPU        | 选项               |
| ---------- | ------------------ |
| Cortex-A5  | `-mcpu=cortex-a5`  |
| Cortex-A7  | `-mcpu=cortex-a7`  |
| Cortex-A8  | `-mcpu=cortex-a9`  |
| Cortex-A9  | `-mcpu=cortex-a9`  |
| Cortex-A15 | `-mcpu=cortex-a15` |
| …          | …                  |

  如果你的GCC版本不能识别上表的核，那它可能太旧了，要考虑升级。如果没有指定要使用的处理器，则GCC将使用其内置默认值。默认值可能会有所不同，具体取决于当时编译器是怎么被build的。生成的代码可能在你的CPU上无法运行，或者跑得很慢。



### 9.2 OOption to specify the FPU

**用于指定FPU的选项**

  几乎所有的Cortex-A处理器都带有浮点单元，并且大多数还带有NEON单元。但是，处理器的浮点单元（Floating-Point Unit，FPU）决定了具体的指令集是否可用。

  GCC需要另外一个单独的选项`-mfpu`来指定FPU。它不会尝试通过`-mcpu`选项确定FPU。 `-mfpu`选项控制哪种浮点和SIMD指令是可用的。

  下面的表给出了每个CPU的推荐选项：

| 处理器     | 仅FP                                      | FP + SIMD          |
| ---------- | ----------------------------------------- | ------------------ |
| Cortex-A5  | `-mfpu=vfpv3-fp16` `-mfpu=vfpv3-d16-fp16` | `-mfpu=neon-fp16`  |
| Cortex-A7  | `-mfpu=vfpv4` `-mfpu=vfpv4-d16`           | `-mfpu=neon-vfpv4` |
| Cortex-A8  | `-mfpu=vfpv3`                             | `-mfpu=neon`       |
| Cortex-A9  | `-mfpu=vfpv3-fp16` `-mfpu=vfpv3-d16-fp16` | `-mfpu=neon-fp16`  |
| Cortex-A15 | `-mfpu=vfpv4`                             | `-mfpu=neon-vfpv4` |

  VFPv3和VFPv4实现提供32个双精度寄存器。但是，当不存在NEON单元时，前16个寄存器（`D16`-`D31`）变为可选。 这由选项名称中的`-d16`表示，这意味着前16个**D**寄存器不可用。选项名称的`fp16`组件指定半精度（16位）浮点的加载、存储、转换指令的存在。这是对VFPv3的扩展，但在所有VFPv4实现中也可用。



### 9.3 Option to enable use of NEON and floating-point instructions

**用于开启NEON和浮点指令集的选项**

  GCC仅使用浮点和NEON指令，如果明确告知GCC可以安全使用。 控制它的选项是`-mfloat-abi`。注意，`-mfloat-abi`也可以更改编译器遵循的ABI。

  `-mfloat-abi`有3中选项：

- **`mfloat-abi=soft`**：不使用任何FPU和NEON指令。仅使用核心寄存器集。使用库调用模拟所有浮点运算。
- **`-mfloat-abi=softfp`**：使用与`-mfloat-abi=soft`相同的调用约定，但在适当的手使用浮点和NEON指令。用这个选项编译的应用程序可以与soft float库链接。如果有相关的硬件指令可用，则可以使用此选项来提高代码的性能，并且使代码符合soft-float环境。
- **`mfloat-abi=hard`**：适当地使用浮点和NEON指令，并更改ABI调用约定，以生成更有效的函数调用。 可以在NEON寄存器中的函数之间传递浮点和向量类型，这大大减少了复制量。 这也意味着在堆栈上传递参数时需要更少的调用。

  用于`-mfloat-abi`的正确选项取决于您的目标系统。对于以平台为目标的编译器，默认选项通常是正确和最佳的选择。Ubuntu 12.04默认使用`-mfloat-abi=hard`。有关ARM体系结构的ABI的更多信息，请参见ARM体系结构的过程调用标准（Procedure Call Standard for the ARM Architecture），http://infocenter.arm.com/help/topic/com.arm.doc.ihi0042e/index.html



### 9.4 Vectorizing floating-point operations

**向量化浮点选项**

  NEON体系结构包含适用于整数和浮点数据类型的操作。GCC具有强大的自动向量化单元，可以检测何时适合使用向量引擎来优化代码并提高性能。但是，编译器可能不希望对代码进行向量化处理。 造成这种情况的原因多种多样：



#### Optimization level for the vectorizer

**向量化器的优化级别**

  默认情况下，向量化程序仅在优化级别`-O3`启用。 有一些选项可以使向量化程序启用其他级别的优化。有关这些选项，请参见GCC文档。



#### IEEE compliance

**IEEE的符合**

  NEON单元中的浮点运算使用IEEE单精度格式来保存正常的操作范围值。 为了最小化NEON单元所需的电量并最大化性能，如果输入或结果为非正常值或NaN值（超出正常工作范围），则NEON单元将不完全符合IEEE标准。

  GCC的默认配置是生成严格符合IEEE浮点算术规则的代码。因此，即使启用了向量化程序，默认情况下，GCC也不使用NEON指令对浮点代码进行向量化。

  GCC提供了许多命令行选项来精确控制所需的IEEE遵从级别。 在大多数情况下，使用`-ffast-math`选项放宽规则并启用向量化是安全的。另外，您可以在GCC 4.6或更高版本上使用`-Ofast`选项来达到相同的效果。它打开`-O3`和许多其他优化来从您的代码中获得最佳性能。 要了解使用`-ffast-math`选项的所有效果，请参阅GCC文档。

**注意**

- NEON单元仅支持对单精度数据进行向量运算。
- 如果您的代码使用的不是单精度格式的浮点数据，则向量化可能不起作用。
- 您还必须确保浮点常量（字面的变量）不会强制编译器以双精度执行计算。在C和C++中，用`1.0F`而不是`1.0`，以确保编译器使用单精度格式。



### 9.5 Example GCC command line usage for NEON code optimization

**用于NEON代码优化的GCC命令行用法**

  确定目标环境后，可以对目标使用GCC命令行选项。下面的例子适用于具有NEON单元的Cortex-A15处理器，其中操作系统支持在NEON寄存器中传递参数。如果有一个数组具有浮点数据类型，然有浮点代码可以操作这些数据，则可以在GCC命令行上指定硬浮点处理：

```markup
arm-gcc -O3 -mcpu=cortex-a15 -mfpu=neon-vfpv4 -mfloat-abi=hard -ffast-math -o myprog.exe myprog.c
1
```

例16

  下面的例17用于带有NEON单元的Cortex-A9处理器，它支持在NEON寄存器上传递参数。如果有浮点代码，那你可以用GCC命令行指定`hard`浮点处理：

```markup
arm-gcc -O3 -mcpu=cortex-a9 -mfpu=neon-vfpv3-fp16 -mfloat-abi=hard -ffast-math -o
myprog.exe myprog.c
12
```

例17

  例18适用于不带NEON单元的Cortex-A7处理器，其中操作系统仅支持在ARM的核心寄存器中传递参数，但可以支持使用浮点硬件进行处理。如果有浮点代码，则可以在GCC命令行上指定`softfp`浮点处理：

```markup
arm-gcc -O3 -mcpu=cortex-a7 -mfpu=vfpv4-d16 -mfloat-abi=softfp -ffast-math -o
myprog2.exe myprog2.c
12
```

例18

  例19是针对Cortex-A8处理器（运行于无法使用NEON寄存器的环境中）的。这可能是因为代码位于中断处理程序的中间，并且浮点上下文为USER状态保留。在这种情况下，您可以在GCC命令行上指定soft浮点处理：

```markup
arm-gcc -O3 -mcpu=cortex-a8 -mfloat-abi=soft -c -o myfile.o myfile.c
12
```

例18



### 9.6 GCC information dump

**倒出GCC信息**

  如果要了解有关GCC正在执行的操作的更多信息，请使用`-fdump-tree-vect`和`-ftree-vectorizer-verbose=level`选项，其中`level`是1到9之间的数字。较低的值输出的信息较少。这些选项控制生成的信息量。尽管生成的大多数信息仅对编译器开发人员有意义，但您可能会在输出中找到提示，这些提示解释了为什么编译器没有按预期对代码进行向量化。











## 1. Introduction to the NEON instruction [syntax](https://so.csdn.net/so/search?q=syntax&spm=1001.2101.3001.7020)

**NEON指令语法简介**

  NEON指令（以及VFP指令）均以字母V开头。指令能够对不同的数据类型进行操作，类型在指令编码中指定，大小以指令的后缀表示，元素的数量由指定的寄存器大小指示。例如下面的指令：
   `VADD.I8 D0, D1, D2`

- `VADD`表示一个NEON的加操作。
- `I8`后缀表示相加的数据为8位的整形数。
- `D0`，`D1`，`D2`表示用的是64位寄存器（`D0`用来存结果，`D1`和`D2`用来存操作数）。
  这条指令的效果就是，并行地执行8个加法，对象是在64位寄存器中的8个8位通道（lane）。
  ![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080714531912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

   下一条指令中，输入和输出寄存器的大小是不一样的：
   `VMULL.S16 Q2, D8, D9`

   这条指令将4个16位有符号数据相乘，产生4个有符号的32位数据，存到`Q2`里面。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080715200547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

  `VCVT`指令在单精度浮点与32位整数、定点与半精度浮点（如果有实现）之间转换元素。

  NEON指令集包括用于将单个或多个值加载或存储到寄存器的指令。此外，还有一些指令可以在多个寄存器和存储器之间传输数据块。在这样的多次传输期间也可以对数据进行交错（interleave）或解交错（de-interleave）。

##   **NEON指令集包括：**

- 加法操作。
- 成对的加法，把相邻的向量元素相加。
- 乘法操作，附带增倍（doubling）和饱和（saturating）操作。
- 乘法和累加（accumulate）操作。
- 左移、右移、插入操作。
- 常用逻辑操作（AND，OR，EOR，AND NOT，OR NOT）。
- 选择最小值最大值操作。
- 统计操作，统计leading zeros，signed bits，set bits的数量。

  NEON指令集不包括：

- 除法操作（使用`VRECPE`和`VRECPS`代替，执行牛顿迭代法（Newton-Raphson iteration））。
- 平方根运算（使用`VRSQRTE`和`VRSQRTS`和乘法代替）。

  由于NEON指令执行多项操作，因此它们不能将标准ALU（算数逻辑运算单元）标志用在比较指令上。相反，可以比较两个元素，并将比较结果存储在目标寄存器中。如果测试条件为假，则目标寄存器中每个通道的所有位都设置为0，如果测试条件为真，则设置为1。然后可以对该位使用掩码，来控制随后的指令要操作的数据。有按位选择指令可以与这些位掩码一起使用。

  一些NEON指令与向量一起作用于标量。与向量一样，标量的大小可以为8位，16位，32位或64位。使用标量的指令可以访问寄存器文件中的任何元素，尽管与乘法指令有所不同。 该指令使用双字向量的索引来指定标量值。乘法指令仅支持16位或32位标量，并且只能访问寄存器文件中的前32个标量（即对于16位标量为`D0`-`D7`，对于32位标量则为`D0`-`D15`）。

  有许多不同的指令可在寄存器之间或元素之间移动数据。指令也可以交换或复制寄存器，执行逆运算，矩阵转置并提取单个矢量元素。



## 2. Instruction syntax

**指令语法**

  指令集的通用格式如下：



```bash
V{<mod>}<op>{<shape>}{<cond>}{.<dt>} <dest1>{, <dest2>}, <src1>{, <src2>}
```

`<mod>`是修饰符（前置修饰符）：

- `Q`（Saturating）
- `H`（Havling）
- `D`（Doubling）
- `R`（Rounding）

`<op>`是要执行的操作，如`ADD`、`SUB`、`MUL`等。

`<shape>`是修饰符（后置修饰符）：

- `L`（Long）
- `W`（Wide）
- `N`（Narrow）

`<cond>` 条件，与IT指令一起使用。

`<.dt>` 数据类型。

`<src1>, <src2>` 源寄存器。有一些指令含有立即数。

`<dst1>, <dst2>` 目的寄存器。目的寄存器可能是好几个寄存器。



V可能是vector的首字母V,就单纯一个标志



### 2.1 Instruction modifiers

**指令修饰符**

  对于某些指令，您可以指定一个修饰符，该修饰符会更改操作的行为。

| 修饰符 | 行为                      | 例子                     | 描述                                                         |
| ------ | ------------------------- | ------------------------ | ------------------------------------------------------------ |
| 无     | 基本操作                  | `VADD.I16 Q0, Q1, Q2`    | 这样的结果是没有任何修改的                                   |
| Q      | Saturation                | `VQADD.I16 D0, D2, D3`   | 如果结果向量中的每个元素超出可表示范围，则将其设置为一个最大值或最小值。范围取决于元素的类型（位数和符号）。如果在任何通道（lane）中发生饱和（saturation），则FPSCR中的粘性QC位置1。 |
| H      | Halved                    | `VHADD.S16 Q0, Q1, Q4`   | 每个元素右移一位（实际上是被截断除以二）。`VHADD`可用于计算两个输入的平均值。 |
| D      | Doubled before saturation | `VQDMULL.S16 Q0, D1, D3` | 在以Q15格式乘以数字时，通常需要这样做，在这种情况下，需要额外加倍以使结果转换为正确的格式。 |
| R      | Rounded                   | `VRSUBHN.I16 D0, Q1, Q3` | 该指令对结果进行舍入，以校正由截断引起的偏差。这等效于在截断之前将结果加0.5。 |



**Q** : 比如 `VQADD.I16 D0, D2, D3`   16位宽的操作数在寄存器D2、D3（64位寄存器）中，所以是4个16位数值相加，加入16位数值都是大于0的，那个D2、D3对应的lane通道间相加，若两个数相加超过16位宽的最大值，那么就只能写入为16位所能表示的最大值，并且（ FPSCR，浮点状态和控制寄存器标志）中的QC标志为置位。例如 D2的lane0 （数值为65000）和D3的lane0（数值为5000）相加，那么65000+5000=70000 > (65535)2^16-1  所在结果寄存器D0的lane0只能保存最大值2^16-1( 65535),寄存器FPSCR的QC标志位置位

**H:**`VHADD.S16 Q0, Q1, Q4`     s16 16位有符号数   Q寄存器128位，所以8个lane，lane0~lane7的16位有符号数相加，比如Q1的lane0和Q4的lane0相加后，结果存于寄存器Q0的lane0，但是这个结果不是直接的计算结果，是进行了右移一位也就是除以2，取平均；也就是这条指令可以进行多个数的求和并取平均。  **H：就是取一半，取平均**

**D：**  `VQDMULL.S16 Q0, D1, D3`    寄存器D1和D3中的操作数进行乘法计算后，其结果乘以2后再存入结果寄存器Q0中

有符号饱和加倍乘长（按元素）。该指令将第一个源 SIMD&FP 寄存器的下半部分或上半部分中的每个向量元素乘以第二个源 SIMD&FP 寄存器的指定向量元素，将结果加倍，将最终结果放入向量中，并将向量写入目标 SIMD&FP登记。该指令中的所有值都是有符号整数值。如果任何结果发生溢出，则这些结果已饱和。如果发生饱和，则设置累积饱和位 FPSR.QC。SQDMULL 指令从第一个源寄存器的下半部分提取第一个源向量，而 SQDMULL2 指令从第一个源寄存器的上半部分提取第一个源向量.根据CPACR_EL1、CPTR_EL2中的设置，
等效指令：`SQDMULL Vd.4S,Vn.4H,Vm.4H`

**R：**

### 2.2 Instruction shape

**指令shape**

  结果向量和操作数向量具有相同数量的元素。但是，结果中元素的数据类型可能不同于一个或多个操作数中元素的数据类型。因此，结果的寄存器大小也可能与一个或多个操作数的寄存器大小不同。寄存器大小中的这种关系由形状描述。对于某些说明，您可以指定`shape`修饰符（后缀）。

1. **None specified**
   操作数和结果都是同样的宽度。
   例如：`VADD.I16 Q0, Q1, Q2`
2. **Long - L** （俗称长指令?）
   操作数的宽度一样，而在结果（目的寄存器）中，每个元素的宽度是操作数元素的2倍。
   例如：`VADDL.S16 Q0, D2, D3` (**D**寄存器是64位，**Q**寄存器是128位。)
3. **Narrow - N** （俗称窄指令?）
   操作数的宽度一样，而在结果中，每个元素的宽度是操作数元素的一半。
   例如：`VADDHN.I16 D0, Q1, Q2`
4. **Wide - W** （俗称宽指令?）
   结果、操作数是第2操作数宽度的2倍。
   例如：`VADDW.I16 Q0, Q1, D4`

**详解：**

- **Normal**常规指令可以操作任何向量类型，产生的结果向量和操作数一样，也是一样的类型和尺寸。
  你可以通过在指令助记符中添加一个`Q`，来指定正常指令的操作数和结果必须全部为四字（四字，**quadword**，一般128位）。这样做的话，如果操作数或结果不是全部为四字，汇编器就会报错。
- **Long**长指令通常对双字（双字，doubleword，一般64位）向量进行运算并产生四字向量。结果元素是操作数元素宽度的2倍。通过在指令后面附加一个`L`来表示长指令。下面图1显示了一个长指令示例，其中输入操作数在操作之前被提升为四字。
  ![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080717205849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图1 长指令

- **Wide**宽指令对双字向量操作数和四字向量操作数进行运算，产生四字向量结果。结果元素和第一个操作数的宽度是第二个操作数元素的宽度的两倍。在指令后面加个`W`表示宽指令。下面图2显示了这一点，在操作之前将输入的双字操作数提升为四字。
  ![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080717310120.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图2 宽指令

- **Narrow**窄指令对四字向量操作数进行运算，并产生双字向量，结果元素是操作数元素宽度的一半。在指令后面附加一个`N`表示窄指令，图示如下。
  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200807173553335.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图3 窄指令

**NEON中指令分为正常指令、宽指令、窄指令、饱和指令、长指令这几类：**

正常指令：数据宽度不变

```
//操作数为int16x4_t,结果数为int16x4_t
int16x4_t vadd_s16 (int16x4_t __a, int16x4_t __b);
```

长指令：源操作数宽度相同 结果宽度扩展 L标记

```
//操作数为int16x4_t，结果数为int32x4_t,vaddl_s16中l标志指令为长指令
int32x4_t vaddl_s16 (int16x4_t __a, int16x4_t __b);
```

宽指令：源操作数宽度不同 结果宽度对齐 W标记

```
//操作数一个为uint32x4_t，一个为uint16x4_t，结果对齐uint32x4_t，w标志指令为宽指令
uint32x4_t vaddw_u16 (uint32x4_t __a, uint16x4_t __b);
```


窄指令：源操作数宽度相同 结果宽度变窄 N标记

```
//操作数为uint32x4_t,结果数为uint16x4_t,n标志指令为窄指令
uint16x4_t vaddhn_u32 (uint32x4_t __a, uint32x4_t __b);
```


饱和指令：当超过数据类型指定到范围则自动限制在该范围内。Q标记

## 3 Specifying data types

**指定数据类型**

  有的NEON指令比其他指令需要知道更多的数据类型。

  **数据移动 / 替换指令**：

- 无需知道如何解析数据，只要知道数据的size即可。
- 如果需要，可以指定更具体的数据类型。
- 不会在操作码中编码，不会在反汇编中显示。

  **标准加法**：

- 必须区分浮点数和整数，但不能区分有符号/无符号整数。
- 如果需要，可指定有符号或者无符号。

  **饱和(saturating)指令**：

- 必须区分符号/无符号，因此必须完全指定数据类型。



   操作数的数据类型在指令中指定。通常，只有第二个操作数的数据类型必须被指定。

- 其它的数据类型都可以从指令形状中推断出来，详情见后面的Instruction shape章节。
- 尽管所有的数据类型都能被指定，如果有需要的话。



   大多数指令的允许数据类型范围有限。 但是，数据类型的描述是很灵活的：

- 如果描述符指定了`I`，你同样可以使用`S`或`U`的数据类型。
- 如果仅仅指定了数据的size，你可以指定一种类型（`I`，`S`，`U`，`P`或`F`）。
- 如果没有指定数据类型，你可以指定一种数据类型。



   `F16`数据类型仅在实现半精度体系结构扩展的系统上可用，并且仅适用于转换。



## 4 Packing and unpacking data

**打包和解包数据**

   对于NEON指令，多个数据元素被打包到单个寄存器中，以允许单个指令同时对寄存器中的所有数据元素进行操作。

   打包数据很常见，但有时无法通过NEON指令对打包的数据进行一些处理。因此，必须将数据解压缩到寄存器中。

   一种方法是加载打包的数据，通过清除不需要的位来掩盖所需的部分，然后将结果左移或右移。这种方法是可行的，但是效率相对较低。

   NEON打包和解包指令简化了打包数据和解压缩数据之间的转换。这使得可以使用字或双字加载有效地加载存储器中的打包数据序列，将其解压缩到单独的寄存器值中，对其进行操作，然后打包回寄存器中，以有效地写出到存储器中。见图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200807192323596.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图4



**UXTH ：半字被无符号扩展到32 位(高16位清0)**  

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
```

**PKHBT**

```
PKHBT
        （Pack Halfword Bottom Top）将其第一个操作数的底部，最不重要的半字与其移位的第二个操作数的顶部（最重要）半字组合。偏移是从0到31的任意数量的左移。
    PKHTB
        （Pack Halfword Top Bottom）将其第一个操作数的顶部，最重要的半字与其移位的第二个操作数的底部（最低有效）半字组合。这个变化是一个算术右移，任意数量从1到32
```



## 5 Alignment

**对齐**

   NEON体系结构为NEON数据访问提供了完全的未对齐支持。但是，指令操作码包含了对齐指示，当地址是对齐的，并且指定了对齐指示的时候，能使对齐更快。然而，如果指定了对齐方式但地址未正确对齐，则会发生数据中止。

   基地址的指定方式为：`[<Rn>:<align>]`
   **注意**：指定了对齐提示，然后使用不正确的对齐地址是编程上的错误。

   对齐指示可以是`:64`、`:128`、`:256`位其中的一种，这要根据**D**寄存器的数量而定。

```markup
VLD1.8    {D0},           [R1:64]
VLD1.8    {D0,D1},        [R4:128]!
VLD1.8    {D0,D1,D2,D3},  [R7:256], R2
123
```

   **注意**：ARM体系指导手册用`@`标志来描述，但在源代码里面不推荐用这个。

  GNU的gas编译器仍将接受`[Rn,:128]`语法（注意冒号之前额外的“,”），但还是首选`[Rn:128]`语法。

  这适用于Cortex-A8和Cortex-A9处理器。对于Cortex-A8处理器，指定128位或更高的对齐方式可节省一个周期。对于Cortex-A9处理器，指定64位或更高的对齐方式可节省一个周期。



## 6 Saturation arithmetic

**饱和算法**

   上一节的指令修饰符中，用`Q`修饰符来指示饱和运算。这是一种算术形式，其中将数学运算的结果限制为预定的最大值和最小值。如果函数的结果大于最大饱和值，则将其设置为最大值。如果小于最小饱和值，则将其设置为最小值。

   例如，如果饱和值的范围为`[-10,10]`，则以下操作的结果如下：

- 10 + 5 = 10 10+5=1010+5=10  (15→10)
- 10 × 5 = 10 10\times5=1010×5=10  (50→10)
- 5 − 30 = 5-30=5−30= -10 1010  (-25→10)
- 3 + 6 = 9 3+6=93+6=9
- 3 × 6 = 10 3\times6=103×6=10  (18→10)
- 6 − ( 3 × 5 ) = 6-(3\times5)=6−(3×5)= -9 99  **（文档此处结果是-10，但我觉得应该是-9）**

   饱和算术常用于数字信号处理之类的应用中，避免像音频信号超出最大范围之类的情况。



## 7 Floating-point operations

**浮点操作**

   NEON的浮点不是完全符合IEEE-754的。
   非规格数（denormals）被刷新为零（flush to zero）。
   **Rounding**固定取最近的数（其实就是四舍五入），但转换操作除外。
   仅支持单精度算术(`.F32`)。
   单独的（标量）浮点指令。



### 7.1 Floating-point exceptions

**浮点异常**

  在可能导致浮点异常的指令描述符中，有一块**subsection**列出了异常。如果指令说明中没有“浮点异常”的**subsection**，则该指令不会导致任何浮点异常。



## 8 Flush-to-zero mode

**刷新到0模式**

   “刷新到0”（flush-to-zero）模式将非规格（denormalized）数替换为0。这不符合IEEE-754算术，但是在某些情况下，它可以大大提高性能。

   在NEON和VFPv3中，“刷新到0”会保留符号位。
   在VFPv2中，被刷为+ 0 +0+0。（不管符号，都刷为正？）
   NEON单元经常用“刷新到0”模式。



### 8.1 Denormals

**异常**

  NEON单元符合IEEE 754-1985标准，但仅支持舍入到最近的舍入（rounding）模式。这是大多数高级语言（例如C和Java）使用的舍入模式。此外，NEON单元始终将非规格值（denormals）视为零。

  在浮点算术中，非规格（denormal）是指浮点数有效位的前面是`0`。 这意味着数值非常小，尾数的格式为 0. m 1 m 2 m 3 m 4 . . . m p − 1 m p 0.m_1m_2m_3m_4...m_{p-1}m_p0.m1m2m3m4...mp−1mp，指数是最小可能指数。 m mm是0或1的有效数字，p是精度。

**这里的一些概念和术语请参考IEEE745标准，关于浮点数的规范**



### 8.2 The effects of using flush-to-zero mode

**“刷新到0”模式的作用**

  除某些例外，“刷新到0”模式对浮点运算有以下影响：

- 当非规格数（denormalized）作为浮点运算的输入时，它会被当做是0。源寄存器的值不会变。
- 当一个单精度浮点运算的结果在四舍五入（rounding）之前，如果范围在[ − 2 − 126 , + 2 − 126 ] [-2^{-126},+2^{-126}][−2−126,+2−126]之间，会被替代成0。
- 当一个双精度浮点运算的结果在四舍五入（rounding）之前，如果范围在[ − 2 − 1022 , + 2 − 1022 ] [-2^{-1022},+2^{-1022}][−2−1022,+2−1022]之间，会被替代成0。

  每当非规格（denormalized）数被当做操作数，或者结果被刷新到0的时候，会发生一个`inexact`异常。在“刷新到0”模式下不会发生`Underflow`异常。



### 8.3 Operations not affected by flush-to-zero mode

**不受“刷新到0”模式影响的操作**

  即使在“刷新到0”模式下，也可以对非规格化（denormalized）的数字执行以下NEON操作，不会把结果刷新为零：

- 复制，绝对值和取反（`VMOV`，`VMVN`，`V {Q} ABS`和`V {Q} NEG`）
- 重复（`VDUP`）
- 交换（`VSWP`）
- 加载和存储（`VLDR`和`VSTR`）
- 多个加载和多个存储（`VLDM`和`VSTM`）
- NEON寄存器和ARM通用寄存器之间的传输（`VMOV`）



## 9 Shift operations

**位移操作**

   本节介绍NEON移位操作，并说明如何使用它们在常用的色深之间转换图像数据。

   NEON指令集提供的强大的移位指令范围支持：

- 快速地对向量除以或者乘以2 n 2^n2n，具有取整（rounding）和饱和（saturation）。
- 从一个向量按位位移和拷贝到另一个向量。
- 以高精度进行临时计算，以低精度累加结果。



### 9.1 Shifting vectors

**位移向量**

  NEON移位操作与标量ARM代码中的移位非常相似。移位将向量的每个元素（element）中的位向左或向右移动。每个元素左侧或右侧超出的位将被丢弃，它们不会转移到相邻元素。

  移位量可以用指令中的立即数，或附加的移位向量指定。当使用移位向量时，输入向量的每个元素的移位取决于移位向量中相应元素的值。位移向量中的元素被视为有符号值，因此在每个元素的基础上可以向左，向右和零移位。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080811331151.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图5

  对有符号元素的向量进行右移操作（由指令所附的类型指示），将对每个元素进行符号扩展。这等效于ARM代码中的算术转换。应用于无符号向量的移位不对扩展符号进行符号化。



### 9.2 Shifting and inserting

**位移和插入**

  带插入操作的NEON移位提供了一种组合两个向量中的位的方法。例如，向左移动并插入（`VSLI`）将源向量的每个元素向左移动。每个元素右侧插入的新位是目标向量中的相应位。如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200808141013769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图6



### 9.3 Shifting and accumulating

**位移和累加**

  有NEON指令可将向量的元素右移，并将结果累加到另一个向量中。这对于在将结果与较低精度的累加器组合之前，以较高的精度进行临时计算的情况很有用。和图6的操作差不多，把“插入”换成了“相加”。



### 9.4 Instruction modifiers

**指令修饰符**

  每个移位指令可以带有一个或多个修饰符。这些修改器本身不会更改移位操作，但是会调整输入或输出以消除偏置（bias）或饱和（saturate）到某个范围。移位指令有五个修饰符：

- Rounding（舍入），用`R`表示
- Narrow，用`N`表示
- Long，用`L`表示
- Saturating，用`Q`表示
- Unsigned Saturating，用`Q`前缀和`U`后缀表示。这类似于饱和（saturating）修饰符，但是当给定有符号或无符号输入时，结果饱和到无符号范围。

  某些修饰符的组合是没啥用的，因此没有针对它们的NEON指令。例如，饱和右移（称为VQSHR）就没什么用，因为右移会使结果更小，因此该值不能超出可用范围。



### 9.4 Table of shifts available

**可用位移表**

  下表列出了所有NEON移位指令，它们是按照前面提到的修饰符排列的。如果仍然不确定修饰语字母的含义，请使用表3，表4和表5选择所需的说明。在表中，`Imm`是偏移量的立即数，`Reg`是位移向量的寄存器。

|                         | Not rounding | rounding |        |         |      |        |
| ----------------------- | ------------ | -------- | ------ | ------- | ---- | ------ |
|                         | Default      | Long     | Narrow | Default | Long | Narrow |
| Left（Imm）             | VSHL         | VSHLL    |        |         |      |        |
| Left（Reg）             | VSHL         |          |        | VRSHL   |      |        |
| Right（Imm）            | VSHR         |          | VSHRN  | VRSHR   |      | VRSHRN |
| Left insert（Imm）      | VSLI         |          |        |         |      |        |
| Right insert（Imm）     | VSRI         |          |        |         |      |        |
| Right accumulate（Imm） | VSRA         |          |        | VRSRA   |      |        |

表3 NEON非饱和位移操作





|              | Not rounding | rounding |        |         |      |         |
| ------------ | ------------ | -------- | ------ | ------- | ---- | ------- |
|              | Default      | Long     | Narrow | Default | Long | Narrow  |
| Left（Imm）  | VQSHL        |          |        |         |      |         |
| Left（Reg）  | VQSHL        |          |        | VQRSHL  |      |         |
| Right（Imm） |              |          | VQSHRN |         |      | VQRSHRN |

表4 NEON饱和位移操作





|              | Not rounding | rounding |         |         |      |          |
| ------------ | ------------ | -------- | ------- | ------- | ---- | -------- |
|              | Default      | Long     | Narrow  | Default | Long | Narrow   |
| Left（Imm）  | VQSHLU       |          |         |         |      |          |
| Left（Reg）  |              |          |         |         |      |          |
| Right（Imm） |              |          | VQSHRUN |         |      | VQRSHRUN |

表4 NEON无符号饱和位移操作



## 10 Polynomials

**多项式**

   多项式是由任何变量的幂和构成的表达式，其中每个被加数都有一个系数。关于变量x xx的一个多项式例子：a 2 x 2 + a 1 x + a 0 a_2x^2+a_1x+a_0a2x2+a1x+a0。

   如果数据类型位于{ 0 , 1 } \{0,1\}{0,1}上，则可以使用数据类型P8（8位多项式）和P16（16位多项式）来表示多项式。 { 0 , 1 } \{0,1\}{0,1}上的多项式是系数为0或1的多项式。对于系数不是0或1的多项式，不能使用NEON多项式算法。

   如果f ff是一个这样的多项式，使用P8数据类型中的8位，f ff可以表示系数为{ a 7 , a 6 , a 5 , a 4 , a 3 , a 2 , a 1 , a 0 } \{a_7,a_6,a_5,a_4,a_3,a_2,a_1,a_0\}{a7,a6,a5,a4,a3,a2,a1,a0}的序列，其中a n a_nan为0或1。或者，您可以使用P16数据类型将f ff表示为序列{ a 15 , a 14 , a 13 , a 12 , a 11 , a 10 , a 9 , a 8 , a 7 , a 6 , a 5 , a 4 , a 3 , a 2 , a 1 , a 0 } \{a_{15},a_{14},a_{13},a_{12},a_{11},a_{10},a_9,a_8,a_7,a_6,a_5,a_4,a_3,a_2,a_1,a_0\}{a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0}。

   因此，NEON寄存器**D**或**Q** 中的每个8位或16位通道（lane）都包含多项式的系数序列，例如f ff。

**注意：** 您可以将1s和0s的多项式序列可视化为8位或16位无符号值。但是，多项式算法与常规算法不同，并且会产生不同的结果。



### 10.1 Polynomial arithmetic over {0,1}

**{0,1}上的多项式算术**

  多项式算法在实现某些密码学或数据完整性算法时很有用。
  多项式系数0和1使用布尔算术规则进行操作：

- 0 + 0 = 1 + 1 = 0 0+0=1+1=00+0=1+1=0
- 0 + 1 = 1 + 0 = 1 0+1=1+0=10+1=1+0=1
- 0 ∗ 0 = 0 ∗ 1 = 1 ∗ 0 = 0 0*0=0*1=1*0=00∗0=0∗1=1∗0=0
- 1 ∗ 1 = 1 1*1=11∗1=1

  对{ 0 , 1 } \{0,1\}{0,1}上的两个多项式相加，相当于与按位异或（OR）。因此，多项式相加会导致与常规相加不同的值。

  对{ 0 , 1 } \{0,1\}{0,1}上的两个多项式相乘，首先确定按常规乘法完成的部分乘积，然后对部分乘积进行异或运算，而不是按常规方式进行相加。多项式乘法的结果与常规乘法不同，因为它需要部分乘积的多项式加法。

  多项式类型可以帮助那些必须使用2的幂的有限域或简单多项式的任何事物。普通的ARM整数代码通常用查表的方法来进行有限域的算术运行，但大型的查找表无法向量化。多项式运算很难从其他运算中合成出来，因此具有基本的乘法运算（加法运算为按位异或）是有用的，从中可以合成较大的乘法或其他运算。

  多项式算法包括：

- 纠错，例如Reed Solomon编码。
- 循环冗余校验（Cycle Redundancy Checking，CRC）。
- 椭圆曲线密码学



### 10.2 NEON instructions that can perform polynomial arithmetic

**可以进行多项式计算的NEON指令**

  有一些NEON指令可对多项式数据类型P8和P16进行操作。乘法指令是唯一的指令，能够改变多项式数据类型的行为，把它从常规的乘法转为多项式乘法。当使用多项式数据类型时，`VMUL`和`VMULL`是仅有的两个可以执行多项式乘法的指令。

  `VADD`指令执行常规加法，不能用于执行多项式加法。多项式加法与按位异或运算完全相同，因此对于多项式加法，必须使用`VEOR`指令。

  但是，`VEOR`内部函数（instrinsic）不接受多项式数据类型P8和P16。因此，在使用内部函数（instrinsic）时，必须将数据类型从P8或P16重新解释为`VEOR`内部函数（instrinsic）接受的数据类型之一。 为此，请使用NEON内部函数（instrinsic）`vreinterpret`。



### 10.3 Difference between polynomial multiply and conventional multiply

**多项式乘法与常规乘法之间的区别**

  多项式加法表现为按位异或（无进位加法），而不是常规加法（带有进位的加法）。即使在单个位上的多项式乘法与在单个位上的常规乘法相同，这也会导致多项式乘法的差异。部分结果必须是按位异或运算，而不是常规地相加。在某些系统中，这称为无进位乘法。

  下面的例子显示了常规算术中3乘3的乘积。乘法以二进制显示，以突出显示差异。 乘法的结果是`b01001`，也就是十进制的9，这是常规乘法所期望的。 不会用latex打竖式公式，直接截图了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200808163118907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70)

  下一个例子显示多项式算术中3与3的乘积。乘法以二进制显示，以突出显示差异。当对位值进行运算时，多项式乘法与常规乘法相同，对中间过程得到的乘积的多项式加法是异或。多项式相乘的结果为`b00101`，等于5。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080816363419.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70)



## 11 Instructions to permute vectors

**置换向量的指令**

  当可用的算术指令与寄存器中的数据格式不匹配时，在向量处理中有时需要进行置换（permutation）或更改向量中元素的顺序。他们从一个寄存器或跨多个寄存器中选择单个元素，以形成一个新向量，该向量与处理器提供的NEON指令更好地匹配。

  置换（permutation）指令与移动（move）指令相似，因为它们用于准备或重新排列数据，而不是修改数据的值。好的算法设计可以无需重新排列数据。因此，请考虑在您的代码中是否需要置换指令。降低对置换和移动指令的需求通常是一种更好的方式。



### 11.1 Alternatives

**备用方案**

  下面有一些方法可以避免不必要的置换（permutation）：

- 重新排列您的输入数据。 通常，以更合适的格式存储数据无需花费任何开销，也不需要在加载和存储时对它进行置换。但是，在更改数据结构之前，请考虑数据局部性及其对缓存性能的影响。
- 重新设计算法。 可以用数量上差不多的处理步骤，但以不同格式处理数据的不同算法。
- 修改前面的处理步骤。 在较早的处理阶段进行很小的更改，即调整将数据存储到内存的方式，可以减少或消除对置换操作的需求。
- 使用交错（interleaving）加载和存储。 加载和存储指令具有交错和解交错的能力。如果这不能完全消除置换的需要，则可能会减少所需的其他指令数量。
- 结合方法。 与其他置换指令相比，结合上面几种方法可能仍然更有效。

  如果你考虑了上述所有方法，还是不能得到合适的数据格式，那就不得不用用置换指令了。



### 11.2 Instructions

**指令**

**本节内容请参考我的**[另外一篇文章](https://blog.csdn.net/Yemiekai/article/details/107771285) **，有配图。**

  置换指令有很多，从简单的翻转到随意地重构向量。简单的置换（permutation）可以在一个指令周期内搞定，而更复杂的操作则需要多个周期，并且可能需要设置其他寄存器。 与往常一样，请查看处理器的《技术参考手册》以获取性能详细信息。

#### VMOV and VSWP: Move and Swap

  `VMOV`和`VSWP`是最简单的置换（permute）指令，可以将整个寄存器的内容复制到另一个寄存器中，或者交换一对寄存器中的值。
  尽管您可能不会将它们视为置换指令，但是可以利用两个**D**寄存器来交换一个**Q**寄存器的高低位。 例如，`VSWP d0,d1`交换`q0`的最高和最低有效64位。

#### VREV: Reverse

  `VREV`反转向量中8位，16位或32位元素的顺序，有三种变体：

  `VREV16`反转构成向量的16位元素的每对8位子元素。
  `VREV32`反转构成向量的32位元素的每对16位子元素，或4个8位子元素。
  `VREV64`反转构成向量的64位元素的每对32位子元素，或4个16位子元素，或8个8位子元素。

  `VREV`反转数据的字节序，通常用于重新排列颜色分量或交换音频样本的通道。

#### VEXT: Extract

  `VEXT`从一对现有向量中提取字节组成新的向量。新向量中的字节从第一个操作数的顶部到第二个操作数的底部。这使您可以生成一个新向量，其中包含跨越一对现有向量的元素。

  `VEXT`可以用来在两个向量上实现滑动窗口，在**FIR**滤波器上非常有用。对于置换（permutation），当两个输入操作数使用相同的向量时，也可以用来模拟按字节旋转的操作。

#### VTRN: Transpose

  `VTRN`在一对向量之间转置8、16或32位元素。 它将向量的元素视为2x2矩阵，并转置每个矩阵。

  使用多个`VTRN`指令来转置较大的矩阵。例如，由16位元素组成的4x4矩阵可以使用三条`VTRN`指令进行转置。

  在加载向量之后或存储向量之前，这与`VLD4`和`VST4`执行的操作相同。 由于它们需要较少的指令，因此请尽可能尝试使用这些结构化的内存访问功能，而不要使用一系列`VTRN`指令。

#### VZIP and VUZP: Zip and Unzip

  `VZIP`交错（interleaves）一对向量的8、16或32位元素。在存储数据之前，该操作与`VST2`执行的操作相同。所以如果需要在写回内存之前立即压缩数据，请使用`VST2`，而非`VZIP`。

  `VUZP`是`VZIP`的反向操作，反交错（deinterleaving）一对向量的8位，16位或32位元素。在从内存加载数据之后，该操作与`VLD2`所做操作相同。

#### VTBL, VTBX: Table and Table Extend

  `VTBL` 从向量表和索引向量构造一个新的向量。这是一个逐字节的表查找操作。

  该表由1到4个相邻的**D**寄存器组成。 索引向量中的每个字节都用于索引向量表中的一个字节。 将索引值插入到结果向量中与索引向量中原始索引位置相对应的位置。

  `VTBL`和`VTBX`在处理超出范围索引的方式上有所不同。如果一个索引超过了表的长度，`VTBL`会在结果向量的相应位置插入0，但是`VTBX`保持结果向量中的值不变（如上图，`d2`原来是多少就是多少）。

  如果将单个源向量用作表，则`VTBL`允许您实现向量的任意排列，但要以设置索引寄存器为代价。 如果是在一个循环里面使用该操作，并且排列的类型不变，则可以在循环的外部初始化索引寄存器，并消除设置开销。





## 1. Introduction

**简介**

  内部函数（***intrinsics***）由ARM编译器工具链提供，用*intrinsics*来写NEON代码，比写汇编程序更容易维护，同时仍然可以控制生成哪些NEON指令。

  NEON的*intrinsics*像一种函数调用，编译器会用适当的NEON指令或NEON指令序列来替换它。但是你必须优化你的代码，以充分利用NEON带来速度的提高。

  这里有一些数据类型来对应NEON的寄存器（包括**D**寄存器和**Q**寄存器），包括不同size的元素。然后你可以创建一些C语言的变量直接映射到NEON寄存器，这些变量会传到NEON *intrinsics*函数里。**编译器会直接生成NEON指令，而不是执行一个函数调用。**

  *intrinsics*的函数和数据类型，以及简短形式的*intrinsics*，为C / C++代码访问低层的NEON功能提供了支持。软件可以传递NEON向量，就像处理函数参数或者返回值那样。也可以像一般变量那样声明NEON向量。

  *intrinsics*提供了绝大多数控制，和写汇编差不多，但是把寄存器的分配留给编译器来做，这样你就可以专注于你的算法了。另外，编译器会像普通C / C++代码那样优化*intrinsics*，有可能的话，会把你写的代码替换成更高效的序列。

  NEON的*intrinsics*都在`arm_neon.h`头文件里面定义了，里面也包含了向量类型。

**注意：**
在ARMv7架构之前是不支持NEON指令集的。在早期的架构上面编译，或者在不包含NEON单元的ARMv7（有的处理器虽然是ARMv7架构，但是不带NEON）架构上编译，编译器会把NEON *intrinsics*当成普通的函数调用，这样会导致错误。



## 2. Vector data types for NEON intrinsics

**简介**

  NEON向量数据类型根据以下模式命名：
`<type><size>x<number_of_lanes>_t`

  例如：

- `int16x4_t`是一个向量，其中包含4个16位的短整型变量（4个lane，每个lane存16位的数）。
- `float32x4_t`是一个向量，其中包含4个32位的浮点型变量（4个lane，每个lane存32位的数）。

下表列出了向量数据类型：

表1

| 64位类型（**D**寄存器） | 128位类型（**Q**寄存器） |
| ----------------------- | ------------------------ |
| `int8x8_t`              | `int8x16_t`              |
| `int16x4_t`             | `int16x8_t`              |
| `int32x2_t`             | `int32x4_t`              |
| `int64x1_t`             | `int64x2_t`              |
| `uint8x8_t`             | `uint8x16_t`             |
| `uint16x4_t`            | `uint16x8_t`             |
| `uint32x2_t`            | `uint32x4_t`             |
| `uint64x1_t`            | `uint64x2_t`             |
| `float16x4_t`           | `float16x8_t`            |
| `float32x2_t`           | `float32x4_t`            |
| `poly8x8_t`             | `poly8x16_t`             |
| `poly16x4_t`            | `poly16x8_t`             |

  你可以用上述向量类型来指定*intrinsics*的输入和输出。一些*intrinsics*可以用向量类型的数组，它们组合了2个、3个或者4个相同的向量类型：
`<type><size>x<number_of_lanes>x<length_of_array>_t`

  这些类型是原始的C结构体，包含一个叫做`val`的元素。
  这些类型映射到<被NEON加载和存储时所访问的>寄存器上，以实现用一条指令加载 / 访问多达4个寄存器。
  该结构体的一个定义示例如下：

```cpp
struct int16x4x2_t
{
    int16x4_t val[2];
} <var_name>;
1234
```

  这些类型仅在加载、存储、转置（transpose）、交错（interleave）、反交错（de-interleave）指令中使用，为了对实际数据执行操作，从单个寄存器中选取元素，例如`<var_name>.val[0]`和`<var_name>.val[1]`。

  有的数组类型长度为2~4，其中包含的向量类型都列在`表1`了。

**注意：**
**向量数据类型**和**向量数据类型的数组**不能通过直接文本赋值初始化。您可以使用一个‘加载’ *intrinsics* 或使用`vcreate` *intrinsics*来初始化它们。



## 3. Prototype of NEON Intrinsics

**NEON \*intrinsics\* 的原型**

   *intrinsics*使用与NEON统一汇编语法类似的命名规则：
`<opname><flags>_<type>`

   它提供了一个额外的标志`q`来指示*intrinsics*操作128位的向量。

例如：

- `vmul_s16`将两个有符号16位向量的值相乘。
  这会被编译成`VMUL.I16 d2, d0, d1`
- `vaddl_u8`是一个长指令，将两个包含无符号8位值的64位向量相加。结果是128位的向量，内含无符号16位的值。
  这会被编译成`VADDL.U8 q1, d0, d1`

   除了指令中写到的寄存器，别的寄存器也可能会被用到，这要看编译器怎么优化。它甚至会以某些方式得到不同的编译结果，以便提高性能。

**注意**：
那些用到`__fp16`的*intrinsics*，仅在拥有NEON半精度VFP扩展的平台上可用。
要开启`__fp16`的话，要在命令行输入`--fp16_format`选项，详情见ARM编译器工具链的文档。



## 4. Using NEON intrinsics

**使用NEON \*intrinsics\***

   ARM编译工具链定义了*intrinsics*，详情在`arm_neon.h`头文件里，`表4.1`中的向量类型也包含在内。

   *intrinsics*是ARM ABI的一部分，因此可以在ARM编译工具链和GCC之间移植。

   带`q`后缀的*intrinsics*通常操作于**Q**寄存器， 不带`q`后缀的*intrinsics*通常操作于**D**寄存器， 然而有一些还是用的**Q**寄存器。

   下面的例子展示了一个*intrinsics*，带有两个不同变量：
   `uint8x8_t vadd_u8(uint8x8_t a, uint8x8_t b);`
   这个`vadd_u8`不含`q`后缀，在此例中，输入输出向量都是64位向量，用的是**D**寄存器。

   `uint8x16_t vaddq_u8(uint8x16_t a, uint8x16_t b);`
   上面这个`vaddq_u8`带有`q`后缀，因此输入和输出向量都是128位，用了**Q**寄存器。

   `uint16x8_t vaddl_u8(uint8x8_t a, uint8x8_t b);`
   上面这个`vaddl_u8`不含`q`后缀，输入向量是64位，输出向量是128位。

   一些NEON *intrinsics*使用32位ARM通用寄存器作为输入参数来保存标量值。例如有的*intrinsics*从一个向量提取单个值（`vget_lane_u8`）、设置向量的某个通道（`vset_lane_u8`）、从一个字面的值创建一个向量（`vcreate_u8`）、把向量的所有通道设置到某个值（`vdup_n_u8`）。

   对每种类型使用不同的*intrinsics*，会导致很难在不兼容的类型之间进行突发性操作，因为编译器会对“哪些寄存器持有哪些类型”保持追踪。

   编译器同样会重新安排程序流，使用可选的更快的指令集。不能保证生成的指令集一定与*intrinsic*所指示的指令集相匹配。这在从一个微体系结构转移到另一个微体系结构的时候特别有用。

   `例1`的代码展示了一个函数，用一个4通道（每个通道是32位无符号整形）的向量作为输入参数，返回一个值（所有通道都被加倍了）。

```cpp
#include <arm_neon.h>
uint32x4_t double_elements(uint32x4_t input)
{
    return(vaddq_u32(input, input));
}
12345
```

例1



   `例2`是从`例1`生成的反汇编代码，该代码（指`例1`）是为hard float ABI编译的。`double_elements()`函数转换成一条NEON指令和一个返回序列。

```cpp
double_elements  PROC
VADD.I32  q0,q0,q0
BX    lr
ENDP
1234
```

例2



   `例3`也是`例1`的反汇编版本，该代码（指`例1`）是为software linkage编译的。在这种情况下，在使用前必须把参数从ARM通用寄存器拷贝到一个NEON寄存器。完成计算后，再把返回值从NEON寄存器拷贝到ARM通用寄存器。

```cpp
double_elements  PROC
VMOV  d0, r0, r1
VMOV  d1, r2, r3
VADD.I32  q0, q0, q0
VMOV  r0, r1, d0
VMOV  r2, r3, d1
BX lr
ENDP
12345678
```

例3



   GCC和armcc支持相同的*intrinsic*，因此用NEON *intrinsic*编写的代码在工具链之间是完全可移植的。必须在每个使用*intrinsic*的源文件中包含`arm_neon.h`头文件，并且必须指定命令行选项。

   使用*intrinsic*优化源模块是很有用的，就算处理器没有实现NEON技术，也可以为它编译。`__ARM_NEON__`宏是GCC定义的，在为实现可NEON技术的目标编译的时候定义。RVCT 4.0 build 591或更高版本，以及ARM编译器工具链也定义了这个宏。软件可以使用这个宏来提供文件中的<优化过的或者纯C/C++版本的>函数，这些函数由传递给编译器的命令行参数选择。

   对于*intrinsic*函数和向量数据类型的更多信息，请查看[《ARM Compiler toolchain Compiler Reference Guide》](http://infocenter.arm.com/)，[GCC文档](http://gcc.gnu.org/onlinedocs/gcc/ARM-NEON-Intrinsics.html)。



## 5. Variables and constants in NEON code

**NEON代码里的变量和内容**

   本节是一些例子，用NEON代码访问变量数据和常量数据。



### 5.1 Declaring a variable

**声明变量**

   声明一个新的变量，和C代码里面的一样简单：

```c
uint32x2_t vec64a, vec64b;  // create two D-register variables
1
```



### 5.2 Using constants

**使用常量**

   使用常量很简单，下面的代码把一个常量复制到一个向量的每个元素里：

```c
uint8x8 start_value = vdup_n_u8(0);
1
```



   要将一般64位常量加载到向量中，请使用：

```c
uint8x8 start_value = vreinterpret_u8_u64(vcreate_u64(0x123456789ABCDEFULL));
1
```



### 5.3 Moving results back to normal C variables

**把结果返回到普通的C变量**

   若要从NEON寄存器访问结果，请用`VST`把它保存到内存中，或者用 “get lane” 类型的操作把它移回ARM寄存器。

```c
result = vget_lane_u32(vec64a, 0); // extract lane 0
1
```



### 5.4 Accessing D registers from a Q register

**从Q寄存器中访问D寄存器**

   用`vget_low`和`vget_high`从**D**寄存器访问**Q**寄存器。

```c
vec64a = vget_low_u32(vec128); // split 128-bit vector 
vec64b = vget_high_u32(vec128); // into 2x 64-bit vectors
12
```



### 5.5 Casting NEON variables between different types

**在不同的NEON变量类型之间转换**

   NEON的*intrinsic*是强类型的，在不同的向量类型之间必须显式转换。在**D**寄存器上用`vreinterpret`来转换，在**Q**寄存器上用`vreinterpretq`。这些*intrinsic*不生成任何代码，只是让你可以转换NEON类型。

```cpp
uint8x8_t byteval;
uint32x2_t wordval;
byteval = vreinterpret_u8_u32(wordval);

uint8x16_t byteval2;
uint32x4_t wordval2;
byteval2 = vreinterpretq_u8_u32(wordval2);
1234567
```

**注意：**
输出类型**u8**是在`vreinterpret`后面列出来的，在输出类型**u32**之前。



## 6. Accessing vector types from C

**从C访问向量类型**

  必须包含`arm_neon.h`头文件，才能用*intrinsic*和C风格的类型进行向量操作。C类型以下面的形式编写：

- `uint8x16_t`：这是一个向量，包含无符号8位整形。向量中有16个元素，因此该向量必须用128位**Q**寄存器
- `int16x4_t`：这是一个向量，包含有符号16位整形，向量中有4个元素，因此向量必须在64位的**D**寄存器中。

  由于ARM标量类型和NEON向量类型之间是不兼容的，就算它们之间具有相同的位长，也无法把标量分配到向量中。标量值和指针只能和直接使用标量的NEON指令一起使用。

  例如，要从NEON向量的通道0取出无符号的32位整数，用：

```cpp
result = vget_lane_u32(vec64a, 0)
1
```

  在armcc中，除了赋值以外，不能用标准C运算符来操作向量类型。因此用`VADD`*intrinsic* 更为合适，而不是用操作符`+`。但是，GCC允许标准C运算符对NEON向量类型进行操作，从而使代码更具可读性。

  如果向量类型只是元素的数量不同`(uint32x2_t, uint32x4_t)`，有专门的指令将128位值的上半部分或者下半部分的向量元素分配给64位寄存器，反之亦然。如果可以将寄存器安排为别名（scheduled as aliases），则此操作不使用任何代码空间。要用128位寄存器的低64位，如下：

```c
vec64 = vget_low_u32(vec128);
1
```



## 7. Loading data from memory into vectors

**从内存加载到向量**

  本节介绍如何使用NEON *intrinsic* 创建向量。内存中的连续数据可以加载到单个向量或多个向量， NEON *intrinsic* 干这件事的是`vld1_datatype`。 例如，要加载一个向量（4个16位无符号数据），请使用NEON *intrinsic* `vld1_u16`。

  下面的例子中，数组A包含8个16位元素，此例展示了如何把数据从这个数组加载到一个向量中。

```cpp
#include <stdio.h>
#include <arm_neon.h>
unsigned short int A[ ] = {1,2,3,4}; // array with 4 elements
int main(void)
{
    uint16x4_t v;       // declare a vector of four 16-bit lanes
    v = vld1_u16(A);    // load the array from memory into a vector
    v = vadd_u16(v,v);  // double each element in the vector
    vst1_u16(A, v);     // store the vector back to memory
    return 0;
}
1234567891011
```

  你可以查看*intrinsic*的说明（在文档附录），看看`vldl_datatype`可以用哪些`datatype`。



## 8. Constructing a vector from a literal bit pattern

**从字面位模板创建向量**

  你可以从字面值来创建向量，NEON的*intrinsic*用`vcreate_datatype`来做这件事。例如你想要加载一个向量（8个无符号8位数据），你可以用*intrinsic*`vcreate_u8`。下面例子展示了如何从字面数据创建一个向量。

```c
#include <arm_neon.h>
int main (void)
{
    uint8x8_t v;  // define v as a vector with 8 lanes of 8-bit data
    unsigned char A[8];  // allocate memory for eight 8-bit data
    v = vcreate_u8(0x0102030405060708);  // create a vector that contains the values 1,2,3,4,5,6,7,8
    vst1_u8(A, v);  // store the vector to memory, in this case, to array A
    return 0;
}
123456789
```

  更多关于`vcreate_datatype`支持的类型请看文档附录。



## 9. Constructing multiple vectors from interleaved memory

**从交错的内存中构建多个向量**

  在内存中的数据经常是交错（interleaved）的，NEON*intrinsic*支持2路，3路，4路交错模式。

  例如内存的一块区域中可能包含立体声数据，左右通道的数据是交错的，这是一个2路交错模式的例子。

  还有一个例子：内存中包含24位RGB图像，它是从红、绿、蓝通道3路交错的8位数据。当内存中包含交错数据的时候，“解交错（de-interleaving）”可以让你加载一个向量存储所有红色值，一个单独的向量存储所有绿色值，一个单独的向量存储所有蓝色值。

  NEON 解交错的 *intrinsic* 是`vldn_datatype`，其中`n`表示交错模式，可以是`2`，`3`，`4`，如果你想把24位RGB图像解交错到3个不同的向量中，可以用`vld3_u8`。下面看看怎么做：

```cpp
#include <arm_neon.h>
int main (void)
{
    uint8x8x3_t v; // This represents 3 vectors. 
                   // Each vector has eight lanes of 8-bit data.
    unsigned char A[24]; // This array represents a 24-bit RGB image. 
    v = vld3_u8(A); // This de-interleaves the 24-bit image from array A 
                    // and stores them in 3 separate vectors
                    // v.val[0] is the first vector in V. It is for the red channel
                    // v.val[1] is the second vector in V. It is for the green channel
                    // v.val[2] is the third vector in V. It is for the blue channel.

    v.val[0] = vadd_u8(v.val[0],v.val[0]);  // Double the red channel
    vst3_u8(A, v);  // store the vector back into the array, with the red channel doubled.
    return 0;
}
12345678910111213141516
```

  更多关于`vldn_datatype` *intrinsic*的类型看文档附录的Load and store部分。



## 10. Loading a single lane of a vector from memory

**从内存中加载单个通道的向量**

  如果要从分散在内存中的数据构造向量，则必须使用单独的 *intrinsic* 分别加载每个通道。

  实现这个的NEON *intrinsic* 是`vld1_lane_datatype`。例如你要加载向量的一个通道（8位无符号数据），要用`vld1_lane_u8`。下面的例子展示了怎样从内存中加载向量的单个通道：

```cpp
#include <stdio.h>
#include <arm_neon.h>
????（文档里面没东西）
123
```



## 11. Programming using NEON intrinsics

**编程**

  直接在汇编器中或通过 *intrinsic* 函数接口编写NEON代码，需要对使用的数据类型以及可用的NEON指令有透彻的了解。要知道要使用什么NEON运算，查看如何将算法拆分为并行运算很有用。从SIMD的角度来看，交换操作（例如加，最小和最大）特别容易。

  数组中的8个成员相加：

```cpp
unsigned int acc=0;

for (i=0; i<8; i+=1)
{
    acc += array[i]; // a + b + c + d + e + f + g + h
}
123456
```

  利用加法的结合律，可以把这个循环展开成几个加法`((a+e) + (b+f)) + ((c+g) + (d+h))`：

```cpp
unsigned int acc1=0; 
unsigned int acc2=0; 
unsigned int acc3=0; 
unsigned int acc4=0; 

for (i=0; i<8; i+=4)
{
    acc1 += array[i]; // (a, e)
    acc2 += array[i+1]; // (b, f)
    acc3 += array[i+2]; // (c, g)
    acc4 += array[i+3]; // (d, h)
}

acc1+=acc2; // (a + e) + (b + f)
acc3+=acc4; // (c + g) + (d + h)
acc1+=acc3; // ((a + e) + (b + f))+((c + g) + (d + h))

1234567891011121314151617
```

  上面的代码展示了一个向量持有4个32位值，用于累加器和临时寄存器。假定相加的数组元素是在32位通道的，然后可以使用SIMD指令进行操作。 将代码扩展为4的任意倍数：

```cpp
#include <arm_neon.h>

uint32_t vector_add_of_n(uint32_t* ptr, uint32_t items)
{
    uint32_t result,* i;
    uint32x2_t vec64a, vec64b;
    uint32x4_t vec128 = vdupq_n_u32(0); // clear accumulators
    
    for (i=ptr; i<(ptr+(items/4)); i+=4)
    {
        uint32x4_t temp128 = vld1q_u32(i); // load four 32-bit values
        vec128=vaddq_u32(vec128, temp128); // add 128-bit vectors
    }
    
    vec64a = vget_low_u32(vec128); // split 128-bit vector
    vec64b = vget_high_u32(vec128); // into two 64-bit vectors
    vec64a = vadd_u32(vec64a, vec64b); // add 64-bit vectors together
    
    result = vget_lane_u32(vec64a, 0); // extract lanes and
    result += vget_lane_u32(vec64a, 1); // add together scalars
    return result;
}
12345678910111213141516171819202122
```

  `vget_high_u32`和`vget_low_u32`与任何NEON指令都不相似。这些 *intrinsic* 指示编译器从输入**Q**寄存器中引用**D**寄存器的高位或者低位。因此，这些操作不会转换为实际的代码，但是会影响用于存储`vec64a`和`vec64b`的寄存器。

  根据编译器的版本，目标处理器和优化选项，生成的代码如下：

```markup
vector_add_of_n PROC
    VMOV.I8    q0,#0
    BIC        r1,r1,#3
    ADD        r1,r1,r0
    CMP        r1,r0
    BLS        |L1.36|
|L1.20|
    VLD1.32    {d2,d3},[r0]!
    VADD.I32   q0,q0,q1
    CMP        r1,r0
    BHI        |L1.20|
|L1.36|
    VADD.I32   d0,d0,d1
    VMOV.32    r1,d0[1]
    VMOV.32    r0,d0[0]
    ADD        r0,r0,r1
    BX         lr
    ENDP
123456789101112131415161718
```



## 12. Instructions without an equivalent intrinsic

**一些指令没有对等的 \*intrinsics\***

  大多数的NEON指令都有等效的*intrinsics*，但是下面的没有：
`VSWP`， `VLDM`， `VLDR`， `VMRS`，`VMSR`， `VPOP`， `VPUSH`， `VSTM`， `VSTR`， `VBIT`， `VBIF`

**注意：**
无法明确生成`VBIF`和`VBIT`，但是 *intrinsics* `VBSL`可以生成任意`VBSL`、`VBIT`或者`VBIF`指令。

  `VSWP`指令没有对应的*intrinsics*，因为编译器可以在需要的时候生成`VSWP`指令，例如用C风格变量分配来交换变量的时候。

  `VLDM`，`VLDR`，`VSTM`和`VSTR`主要用于上下文切换，这些指令具有对齐约束。 编写 *intrinsics* 时，使用`vldx` *intrinsics*更简单。除非明确指定，否则`vldx` *intrinsics*不需要对齐。

  `VMRS`和`VMSR`访问NEON的条件标志，数据处理的时候不需要用NEON *intrinsics*

  `VPOP`和`VPUSH`用于传递给函数的参数中。减少变量重用，或使用更多NEON *intrinsics* 变量，可使寄存器分配器保持对活动的寄存器的跟踪。





## 1. Optimizing NEON assembler code

**优化NEON汇编代码**

  因为一串优化的指令在不同处理器上有不同的时间特性。为了让手写NEON的代码性能最佳，有必要注意一些底层硬件特性。尤其是，程序员应该注意流水线和调度问题，内存访问行为和调度冲突（hazards）。



### 1.1 NEON pipeline differences between Cortex-A processors

**不同Cortex-A处理器之间的NEON流水线差异**

  Cortex-A8和Cortex-A9处理器共享相同的基本NEON流水线，尽管它们在“如何集成到处理器流水线”方面有点不同。Cortex-A5处理器包含完全兼容的简化的NEON执行流水线，但它是为了实现最小和最低功耗而设计的。



### 1.2 Memory access optimizations

**内存访问优化**

  NEON单元可能会处理大量数据，例如数字图像。一种重要的优化是确保算法以尽可能最易于缓存的方式访问数据。这将从L1和L2缓存中获得最大命中率。考虑活动内存位置的数量也很重要。在Linux下，每个4KB页面将需要一个单独的TLB（Translation Lookaside Buffer）入口。Cortex-A9处理器具有32个元素的微型TLB和一个128个元素的主TLB，之后它将开始使用L1缓存加载页表入口。典型的优化是，安排算法以适当大小来处理图像数据，以最大化缓存和TLB命中率。

  交错（instructions）和解交错（de-interleaving）的指令可以为性能改进提供很大的空间。`VLD1`从内存加载数据到加载寄存器，没有解交错。 但是，其他`VLDn`操作使我们能够加载，存储和解交错包含两个，三个或四个大小相等元素（element），元素可以是8位，16位或32位的结构。 `VLD2`加载两个或四个寄存器，以对偶数和奇数元素进行解交错。 例如，这可以用于拆分左右声道立体声音频数据，如`图1`所示。类似地，`VLD3`可用于将RGB像素分割为单独的通道，`VLD4`可与ARGB或CMYK图像一起使用。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200812163447502.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

图1



  `图1`展示了用`VLD2.16`指令，把`R1`指向的内存的数据，加载到两个NEON寄存器。结果在第一个寄存器产生4个16位元素，在第二个寄存器产生4个16位元素。



## 2. Scheduling

**调度（or规划）**

  要从NEON单元获得最佳性能，您必须了解如何为所使用的特定ARM处理器 schedule 代码。建议仔细地通过手动 schedule，让你写的NEON汇编代码最佳，特别是对于性能关键的应用程序，如视频编解码器。

  如果你写的是C或者NEON *intrinsics* ，编译器（GCC或armcc）会自动地 schedule 来自NEON *intrinsics* 或者可向量化的C源码，但仍会帮你使源码尽可能对调度优化友好。



### 2.1 NEON instruction scheduling

**NEON指令调度**

  NEON指令流经ARM流水线，然后进入ARM和NEON流水线之间的NEON指令队列。尽管从ARM流水线的角度来看，NEON指令队列中的指令已完成，NEON单元仍必须对该指令进行解码和调度。

  只要这些队列没有满，处理器就可以跑，执行ARM指令和NEON指令。当NEON指令或数据队列已满时，处理器将暂停下一条NEON指令的执行，直到队列中有空间容纳此指令为止。以这种方式，在NEON单元中调度的NEON指令的周期耗时可以影响指令序列的总体耗时，但前提是，指令或数据队列填满了足够多的NEON指令。

**注意：**
当处理器配置为不带NEON单元时，所有尝试的NEON和VFP指令都会导致`Undefined Instruction`异常。



### 2.2 Mixed ARM and NEON instruction sequences

**混合的ARM和NEON指令序列**

  如果一个序列中的大多数指令都是NEON指令，那么NEON单元决定了序列所需的时间。序列中偶尔出现的ARM指令与NEON指令并行发生。如果序列中的大多数指令都是ARM指令，那么它们将控制序列的时序，NEON数据处理指令通常需要一个周期。在计算循环计时时，必须考虑是ARM指令还是NEON指令控制了序列。数据冲突（data hazards）会延长指令的执行时间。



### 2.3 Passing data between ARM general-purpose registers and NEON registers

**在ARM通用寄存器和NEON寄存器之间传递数据**

  使用`VMOV`指令将数据从NEON寄存器传递到ARM寄存器。然而，这是缓慢的，尤其是在Cortex-A8。数据从NEON寄存器文件（在NEON流水线的back）移动到ARM通用寄存器文件（在ARM流水线的begin）。多个back-to-back传输可以隐藏一些这种延迟。处理器继续在VMOV指令之后发出指令，直到遇到必须读写ARM通用寄存器文件的指令。此时，指令发出暂停，直到所有从NEON寄存器传输到ARM通用寄存器被挂起的寄存器完成。

  使用`VMOV`指令同样地将数据从ARM通用寄存器传递到NEON寄存器。对于NEON单元，传输过程与NEON加载指令相似。



### 2.4 Dual issue for NEON instructions

**NEON指令集的双开**

**dual issue** 的定义：To increase instruction throughput（吞吐率）, the processor can issue certain pairs of instructions simultaneously. This is called dual issue. When this happens, the instruction with the smaller cycle count is assumed to execute in zero cycles. If a pair of instructions can be dual-issued, they are always dual-issued unless dual-issuing is disabled。
(就是双开、并行的意思)

  NEON单元具有有限的 dual issue 功能，具体取决于实现。（加载、存储、置换（permute）、MCR或者MRC-type的指令可以与NEON数据处理指令一起 **dual issue**。一条加载、存储、置换、`MCR`或者类`MRC`指令执行于NEON的加载和存储转置流水线上。一条NEON数据处理指令执行于NEON的整型ALU、位移、MAC、浮点加法或乘法流水线上。以上是唯一允许的dual issue配对。

（这里翻译不过来，水平有限谢谢😕，请看原文）

  NEON单元可能在以下两种情况下在dual issue：多周期指令的第一个周期（使用较旧的指令）和多周期指令的最后一个周期（使用较新的指令）。多周期指令的中间周期不能被配对dual起来，只能被single issue。





### 2.5 Example of how to read NEON instruction tables

**例子：如何看NEON指令表**

  本节给出一个例子告诉你怎么看NEON指令表。你可以看ARM架构参考手册，看看指令集的汇编语法。在NEON指令表里，`Q<n>Lo`映射到`D<2n>`，`Q<n>Hi`映射到`D<2n+2>`。例如`Q0`寄存器的低64位就是`D0`寄存器，`Q0`寄存器的高64位就是`D1`寄存器。



#### NEON integer ALU instruction

   `VADDL.S16 Q2, D1, D2`

这是一个整型NEON向量，长指令。这里`D1`是Source1，`D2`是Source2，他们都在N1中需要。结果存放在`Q2`中，在N3中可用，为了下一条指令，下一条指令需要这个寄存器（`Q2`）作为源操作数。



#### NEON floating-point multiply instruction

   `VMUL.F32 Q0, Q1, D4[0]`

这是一条浮点NEON向量与标量相乘的指令。这是一个多周期的指令，在第一次和第二次循环中都要求有源操作数。这里Source1是`Q1Lo`或`D2`，要求在N2中。Source2是`D4`，要求在N1中。第2次循环的时候，Source1是`Q1Hi`或者`D3`，被N2要求。乘法的结果存在`Q0`中，被N5需要，下一条指令需要这个寄存器作为源操作数。结果的低位`Q0Lo`或`D0`是在**第1次**循环中算出来的。高位`Q0Hi`或`D1`是在**第2次**循环中算出来的。假设没有数据冲突（data hazards），这个指令花费最小2个周期执行，由周期队列中的值指定。



#### Result-use scheduling

这是写NEON代码时主要的性能优化方法。NEON指令通常在一个周期里搞定，但是生成的结果要等到下个周期才出来，除了一些最简单的NEON指令以外，如`VADD`和`VMOV`。

在某些情况下会有相当长的延迟，尤其是`VMLA`乘加指令（整形数要5个周期，浮点数要7个周期）。用到这些指令的时候要优化一下代码，避免在结果准备好之前使用它的结果，否则会产生停顿问题。尽管有几个周期会导致延迟，但是这些指令可以完全流水线化，因此可以一次“执行”多个操作。

对于大多数指令，Cortex-A8和Cortex-A9处理器之间的结果延迟相同。Cortex-A5处理器使用简化的NEON架构，该架构更适合减少功耗和面积，并且大多数NEON指令具有3个周期的结果延迟。



#### Dual-issue scheduling

在Cortex-A8处理器上，可以并行（在一个周期内）发出某些类型的NEON指令。加载/存储，置换或`MCR` / 类`MRC`的指令可以与NEON数据处理指令（例如浮点加法或乘法）或NEON整数ALU，移位或乘法累加指令 dual issue。程序员可以通过排列（ordering）代码来利用这一点，从而节省周期。



### 2.6 Optimizations by variable spreading

**通过变量扩展进行优化**

  编程的时候，通常有这么一种诱惑：减少变量的使用。但这在写NEON *intrinsics* 的时候未必有益。

  下面演示了将两个 4 × 4 4\times44×4 浮点矩阵相乘的函数。 每个矩阵是列序（column-major）的格式存储。这意味着矩阵以十六个浮点数的线性数组形式存储，其中每列的元素都连续存储。

```cpp
void altneonmult(const float *matrixA, const float *matrixB, float *matrixR)
{
	float32x4_t a, b0, b1, b2, b3, r;
	
	a0 = vld1q_f32(matrixA); /* col 0 of matrixA */
	a1 = vld1q_f32(matrixA + 4); /* col 1 of matrixA */
	a2 = vld1q_f32(matrixA + 8); /* col 2 of matrixA */
	a3 = vld1q_f32(matrixA + 12); /* col 3 of matrixA */
	
	b = vld1q_f32(matrixB); /* load col 0 of matrixB */
	r = vmulq_lane_f32(a0, vget_low_f32(b), 0);
	r = vmlaq_lane_f32(r, a1, vget_low_f32(b), 1);
	r = vmlaq_lane_f32(r, a2, vget_high_f32(b), 0);
	r = vmlaq_lane_f32(r, a3, vget_high_f32(b), 1);
	vst1q_f32(matrixR, r); /* store col 0 of result */
	
	b = vld1q_f32(matrixB + 4); /* load col 1 of matrixB */
	r = vmulq_lane_f32(a0, vget_low_f32(b), 0);
	r = vmlaq_lane_f32(r, a1, vget_low_f32(b), 1);
	r = vmlaq_lane_f32(r, a2, vget_high_f32(b), 0);
	r = vmlaq_lane_f32(r, a3, vget_high_f32(b), 1);
	vst1q_f32(matrixR + 4, r); /* store col 1 of result */
	
	b = vld1q_f32(matrixB + 8); /* load col 2 of matrixB */
	r = vmulq_lane_f32(a0, vget_low_f32(b), 0);
	r = vmlaq_lane_f32(r, a1, vget_low_f32(b), 1);
	r = vmlaq_lane_f32(r, a2, vget_high_f32(b), 0);
	r = vmlaq_lane_f32(r, a3, vget_high_f32(b), 1);
	vst1q_f32(matrixR + 8, r); /* store col 2 of result */
	
	b = vld1q_f32(matrixB + 12); /* load col 3 of matrixB */
	r = vmulq_lane_f32(a0, vget_low_f32(b), 0);
	r = vmlaq_lane_f32(r, a1, vget_low_f32(b), 1);
	r = vmlaq_lane_f32(r, a2, vget_high_f32(b), 0);
	r = vmlaq_lane_f32(r, a3, vget_high_f32(b), 1);
	vst1q_f32(matrixR + 12, r); /* store col 3 of result */
}
12345678910111213141516171819202122232425262728293031323334353637
```

这个函数计算`matrixR = matrixA * matrixB`，一次一列。上面的代码由于对向量变量`r`的复用，对编译器的 schedule 造成了约束。在进入下一列之前，必须完全计算完这一列。由于每个浮点乘法（`vmulq`）或者乘加（`vmalq`）取决于上一条指令的结果，因此NEON单元无法将更多的指令调度到流水线中，这意味着NEON单元要停下来，等上一个操作完成。



#### Loading data into different variables

改进上述问题的另外一个方法是：加载两个矩阵的所有列到不同的变量中，在函数开始的地方：

```cpp
void neonmult(const float *matrixA, const float *matrixB, float *matrixR)
{
	float32x4_t a0, a1, a2, a3, b0, b1, b2, b3, r0, r1, r2, r3;
	
	a0 = vld1q_f32(matrixA); /* col 0 of matrixA */
	a1 = vld1q_f32(matrixA + 4); /* col 1 of matrixA */
	a2 = vld1q_f32(matrixA + 8); /* col 2 of matrixA */
	a3 = vld1q_f32(matrixA + 12); /* col 3 of matrixA */
	
	b0 = vld1q_f32(matrixB); /* col 0 of matrixB */
	b1 = vld1q_f32(matrixB + 4); /* col 1 of matrixB */
	b2 = vld1q_f32(matrixB + 8); /* col 2 of matrixB */
	b3 = vld1q_f32(matrixB + 12); /* col 3 of matrixB */
	
	/* compute all the cols in the order specified by compiler */
	r0 = vmulq_lane_f32(a0, vget_low_f32(b0), 0);
	r0 = vmlaq_lane_f32(r0, a1, vget_low_f32(b0), 1);
	r0 = vmlaq_lane_f32(r0, a2, vget_high_f32(b0), 0);
	r0 = vmlaq_lane_f32(r0, a3, vget_high_f32(b0), 1);
	
	r1 = vmulq_lane_f32(a0, vget_low_f32(b1), 0);
	r1 = vmlaq_lane_f32(r1, a1, vget_low_f32(b1), 1);
	r1 = vmlaq_lane_f32(r1, a2, vget_high_f32(b1), 0);
	r1 = vmlaq_lane_f32(r1, a3, vget_high_f32(b1), 1);
	
	r2 = vmulq_lane_f32(a0, vget_low_f32(b2), 0);
	r2 = vmlaq_lane_f32(r2, a1, vget_low_f32(b2), 1);
	r2 = vmlaq_lane_f32(r2, a2, vget_high_f32(b2), 0);
	r2 = vmlaq_lane_f32(r2, a3, vget_high_f32(b2), 1);
	
	r3 = vmulq_lane_f32(a0, vget_low_f32(b3), 0);
	r3 = vmlaq_lane_f32(r3, a1, vget_low_f32(b3), 1);
	r3 = vmlaq_lane_f32(r3, a2, vget_high_f32(b3), 0);
	r3 = vmlaq_lane_f32(r3, a3, vget_high_f32(b3), 1);
	
	vst1q_f32(matrixR, r0);
	vst1q_f32(matrixR + 4, r1);
	vst1q_f32(matrixR + 8, r2);
	vst1q_f32(matrixR + 12, r3);
}
12345678910111213141516171819202122232425262728293031323334353637383940
```

这个实现与第一个实现有相同数量的加载、存储、乘法操作。但编译器有更大的自由度来调度（schedule）代码。例如，他可以在函数开始之前执行所有加载，以便在需要之前完成加载。而且，他可以在备用列上执行乘加指令（`vmalq`），这样在相邻的指令之间就没有数据依赖性，这样减少了流水线的停歇（stalling）。例如在下面4个 *intrinsics* 中没有数据依赖性，因为使用了不同的变量`r0`,`r1`,`r2`,`r3`，而不是同一个变量`r`。因此编译器可以同时调度下面4个 *intrinsics* ：

```cpp
r0 = vmlaq_lane_f32(r0, a1, vget_low_f32(b0), 1); 
r1 = vmlaq_lane_f32(r1, a1, vget_low_f32(b1), 1); 
r2 = vmlaq_lane_f32(r2, a1, vget_low_f32(b2), 1); 
r3 = vmlaq_lane_f32(r3, a1, vget_low_f32(b3), 1);
1234
```



### 2.7 Optimizations when using lengthening instructions

**使用加长指令时的优化**

可以用单条NEON指令代替两条NEON指令。例如，`vmovl`加长的操作可以作为另外一个加长指令的一部分来执行。下面的例子展示了单独的`vmovl`和`vshl`指令：

```markup
vmovl.u16 q7, d31
...
vshl.s32 q7, q7, #8
123
```

**注意：** 指令`vshl`用`q7`寄存器作为输入，它来自指令`vmovl`的输出。这造成了对寄存器`q7`的数据依赖。所以在这两条指令之间，插入一些其它的没有用到`q7`的指令是个好主意。



两条指令可以用一个`vshll`指令代替：`vshll.s32 q7, d31, #8`













 Arm的Neon技术是一种64/128位混合[SIMD](https://so.csdn.net/so/search?q=SIMD&spm=1001.2101.3001.7020)架构，旨在加速多媒体和信号处理应用程序的性能，包括视频编解码、音频编解码、3D图形、语音和图像处理。👍
  本文是系列文章的第一部分，教大家如何用汇编语言为Neon编写SIMD（Single Instruction Multiple Data单指令集多数据）代码。本系列将涵盖Neon的入门、高效使用，以及为更有经验的码农提供指示。我们将从内存操作开始，以及如何使用灵活的加载和存储置换指令。



## An Example

  给你一个24位的RGB图像，像素在内存中排列为`RGBRGB...`，现在需要做一个简单的图像处理操作，例如交换红色和蓝色通道。如何使用Neon高效地做到这一点？
  用一个加载操作将RGB数据从内存线性地拉到寄存器，这种交换红/蓝的方式看起来很愚蠢。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020072909102921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  对于这种RGB排列的输入，编写通道交换的代码不会很优雅，例如掩码(masks)、位移(shifting)、组合(combing)等，不太可能有效率。
  Neon提供了结构化加载和存储指令，为上述情况提供帮助。它们从内存中提取数据，将这些值同时分离到不同的寄存器中。对于这个例子你可以用`VLD3`指令，在加载红、绿、蓝的同时将它们拆分。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729093233848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  然后使用指令`VSWP d0, d2`交换红色和蓝色寄存器，再用类似命名的存储指令(`VST3`)，将交织后的数据写回内存。



## The Details

### Overview（概述）

  Neon结构将这些从内存中读取的数据，加载到64位Neon寄存器中，同时可以选择性地将数据交错(deinterleaving)（如上面的例子）。存储的工作原理也类似，在写入内存之前，再次交错(reinterleaving)来自寄存器的数据。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729101555864.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)



### Syntax（语法）

  结构加载和存储指令的语法由5个部分组成。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729100131791.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

- **助记指令符（Instruction mnemonic）**，`VLD`表示加载，`VST`表示存储。
- 数字**交织模式（Interleave pattern）**，在每个结构中，对应元素之间的间隔。
- **元素类型（Element type）**，指定被访问元素的位数。
- 一组要读或者写的**Neon寄存器列表（Neon register list）**，最多可以列出4个寄存器，主要看交织模式的选择。
- **Arm地址寄存器(Arm address register)**，包含了内存中要访问的地址。访问后可以更新地址。



### Interleave Pattern（交错模式）

  指令可以用于加载、存储和解交错(deinterleave)一块结构，结构中包含1到4个大小相等的元素（也就是寄存器列表的数量?），这些元素通常是Neon支持的8、16、32位宽度数据。

- `VLD1`，是最简单的形式，它从内存中加载1到4个寄存器的数据，没有解交错(deinterleaving)。处理一组没有交错(non-interleaved)的数据的时候使用这种模式。
- `VLD2`，加载2到4个寄存器的数据，将偶数和奇数元素解交错(deinterleaving)到这些寄存器中。使用此选项可将立体声音频数据分为左右声道。
- `VLD3`，加载3个寄存器的数据，并且解交错(deinterleaving)，用于将RGB像素分割为通道。
- `VLD4`，加载4个寄存器的数据，并且解交错(deinterleaving)，用于处理ARGB的图像数据。

  存储指令支持同样的选项，并且在写入内存之前，交错(interleave)来自寄存器的数据。



### Element Types（元素类型）

  根据指令指定的大小，加载或存储交错的元素。例如用`VLD 2 .16`加载2个Neon寄存器的数据，会在第一个寄存器中产生4个16位元素，在第二个寄存器中产生4个16位元素，相邻的一对（奇和偶）分离到每个寄存器中。（看图↓↓↓）

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729110059424.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  若将元素大小改为32位（以上图为例把16位改成32位），则会加载同样数量的数据，但是每个Neon寄存器里面只有2个元素。另外，仍然像上面一样，同时又分为奇数和偶数元素。（看图↓↓↓）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729111516378.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  元素大小也会影响字节序处理。通常，如果为加载和存储指令指定了正确的元素大小，则将以适当的顺序从内存中读取字节，以同样的代码在大端或者小端系统上运行。（这段不知道想讲什么，反正要注意大小端系统的字节顺序）。
  最后，元素的大小会影响指针对齐。与元素大小对齐通常可以提供更好的性能，或者你的目标操作系统要求这样做。例如，加载32位元素的时候，第一个元素的地址至少对齐到32位。



### Single or Multiple Elements（单个或多个元素）

  除了加载多个元素外，结构加载还可以通过解交错（deinterleaving）从内存中读取单个元素，将其读取到Neon寄存器的所有通道或者单个通道，而其它通道保持完整。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729114718483.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  当你需要根据分散在内存中的数据构造向量时，后一种形式很有用。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729114726786.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  存储也相似，通过交错(interleaving)，为写入单个或多个元素提供支持。



### Addressing（寻址）

  结构加载和存储指令支持3种形式，用于指定寻址。

- 寄存器：`[{,:}]`
  这是最简单的形式，数据将会被加载或者存储到指定的地址。
- 后面加个叹号：`[{,:}]!`
  这种方式在加载或存储数据后更新指针，准备加载或存储下一个元素。指针增量等于指令加载或存储的字节数。
- 后面加个逗号：`[{,:}],`
  在内存访问后，指针的增量等于`Rm`寄存器的值。在读取或写入以固定宽度分隔的一组元素时，这种方式很有效。例如，读取一张图像中的垂直线。

  你还可以使用可选的“`:`”参数，为`Rn`寄存器中的指针指定对齐方式，这通常会加速内存访问。



### Other Loads and Stores（其它加载和存储）

  在本文中，我们仅讲述了加载和存储，Neon还提供了：

- `VLDR` 和`VSTR`
  加载和存储单个寄存器（以64位值）。
- `VLDM` 和`VSTM`
  以64位值加载多个寄存器。对于从栈上存取寄存器很有用。

==============================================================



  有关支持的加载和存储操作的更多详细信息，请参见[《 Arm Architecture参考手册》](https://developer.arm.com/documentation/ddi0406/b/)。 有关说明的详细循环计时信息，请参见每个内核的《技术参考手册》。










  [**第1部分**](https://blog.csdn.net/Yemiekai/article/details/107653354)中，探讨了如何在Neon处理单元和[内存](https://so.csdn.net/so/search?q=内存&spm=1001.2101.3001.7020)之间传输数据。
  在本文中，我们处理一个经常遇到的问题：**输入数据不是要处理的向量长度的倍数**。你需要在[数组](https://so.csdn.net/so/search?q=数组&spm=1001.2101.3001.7020)的开头或结尾处，处理剩余的元素——在Neon上处理这个问题的最佳方法是什么？😁



## Leftovers（剩菜剩饭👺）

  使用Neon通常涉及这样的操作：处理长度为4~16个元素(位)的向量数据。通常你会发现你的数组不是该长度的倍数，然后不得不另外处理那些剩余的元素。
  例如，你想要用Neon在每次迭代中加载、处理或存储8个元素，但是你的数组长度是21个元素。前2次迭代进行得很好，但是到了第3次，仅剩下5个元素要处理，你要怎么做？



## Fixing Up（解决办法）

  有3个办法来处理这些剩下来的东西，这些方法在需求、性能、代码大小上各不相同。下面依次列出这几种方法，最快的方法排在前面。

### 1. Larger Arrays（增大数组）

  如果你可以改变你正在处理的数组的长度，可通过增加元素（padding elements）将数组的长度增加到向量大小的倍数。这可以让你在数据的末尾读取和写入数据，而不会破坏相邻的存储。
  在上面的例子中，把数组的长度增加到24个元素，让第3次迭代得以完成而不会造成潜在的数据破坏。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729141912902.png#pic_center)

#### Notes（注意）

- 分配更大的数组将会消耗更多内存。在短数组很多的时候，这种开销会变得非常显著。
- 为了不影响计算结果，在数组末尾新增元素后，这些元素应该被初始化为指定的值。例如 **(1)** 如果你正在对数组求和，新增元素必须被初始化为0，从而不影响计算结果。 **(2)** 如果正在寻找数组最小值，新增元素必须设到Max这么大。
- 在一些情况下，想要不影响计算结果，但是没办法找到合适的初始值，例如求数组均值的时候。

#### Code Fragment（代码片段）

以上图为例

```markup
@ r0 = 输入数组指针
@ r1 = 输出数组指针
@ r2 = 数组的数据长度

@ 我们可以假设数组长度比0要大，是个整形向量，比数组中数据的长度大或者相等
    add  r2, r2, #7      @ r2 = r2 + 7， 7的由来：向量长度-1
    lsr  r2, r2, #3      @ r2 = r2 / 8， 总长除以每一段的长度，算出有几段向量要处理（右移3就是除以8）

loop:
    subs r2, r2, #1      @ r2 = r2 - 1， 剩余要循环的次数, 每次减一
    vld1.8 {d0}, [r0]!   @ 从r0指向的地址加载8个元素(位)到d0寄存器，!表示马上刷新r0指向下一块地址

    ...
    ...                  @ 处理加载到d0的数据
    ...
    vst1.8 {d0}, [r1]!   @ 写8个元素到输出数组，更新r1的值，指向下一块地址

    bne loop             @ 如果r2不为0，继续循环
123456789101112131415161718
```



### 2. Overlapping（重叠）

  如果操作合适，可以用重叠（overlapping）来处理剩余元素。这涉及到两次处理数组中的一些元素。
  在下图的例子中，第一次迭代处理[0-7]的元素，第二次处理[5-12]的元素，第三次处理[13-20]的元素。注意[5-7]的元素在第1和第2个向量中重复了，被处理了2次。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729152832143.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)



#### Notes（注意）

- 当对输入数据的操作，不随操作次数的变化而变化时，才能使用重叠（overlapping），操作必须是幂等的。例如， **(1)** 当你正在找数组的最大元素时，就可以使用。 **(2)** 对数组求和就不能使用，重叠部分将被计数2次。
- 数组中元素的数量必须能够填满至少1个完整的向量。

#### Code Fragment（代码片段）

```markup
@ r0 = 输入数组指针
@ r1 = 输出数组指针
@ r2 = 数组的数据长度

@ 假设操作是幂等的，数组的长度至少为1个向量的长度
    ands  r3, r2, #7      @ 求处理完所有整长的向量后，数组还剩多少剩余没法整除的。即求r2与8的余数
                          @ r3 = r2 & 7, 7的由来：向量长度-1.
                          @ and：逻辑与.  ands：逻辑与之后立即更新cpsr中的标志位.
    beq  loopsetup        @ 如果上面`ands`操作的结果是0, 数组的长度就是向量长度的整数倍
                          @ 所以没有重叠, 可以跳到下面`loopsetup`循环哪里开始处理

                            @ 否则单独分开处理第一个向量
    vld1.8  {d0}, [r0], r3  @ 从数组加载第一组8个元素, 根据元素重叠数量数量更新指针, r3就是重叠的数量
    
    ...
    ...                     @ 处理加载到d0的数据
    ...
    
    vst1.8  {d0}, [r1], r3  @ 写8个元素到输出数组, 更新输出数组指针的位置
 
                            
loopsetup:                @ 循环前的处理
    lsr  r2, r2, #3       @ r2 = r2 / 8, 算算有多少个向量需要处理(即循环多少次)
 
                          
loop:                     @ 像前一个example一样正常循环了
    subs r2, r2, #1       @ r2 = r2 - 1， 剩余要循环的次数, 每次减一
    vld1.8 {d0}, [r0]!    @ 从r0指向的地址加载8个元素(位)到d0寄存器，!表示马上刷新r0指向下一块地址

    ...
    ...                   @ 处理加载到d0的数据
    ...
    vst1.8 {d0}, [r1]!    @ 写8个元素到输出数组，更新r1的值，指向下一块地址

    bne loop              @ 如果r2不为0，继续循环
1234567891011121314151617181920212223242526272829303132333435
```

解读：也就是首先对重叠部分进行处理、指针偏移，然后就跟上一个例子一样开始正常循环。



### 3. Single Elements（单个元素）

  Neon提供加载和存储的操作，可以处理向量里的单个元素。通过这个方法，可以加载包含1个元素的部分向量，对其进行操作，然后把该元素写回到内存中。
  对于下图的问题，前两次迭代正常执行，处理[0-7]和[8-15]的元素。第三次迭代只需要处理5个元素。他们在单独的循环中处理：加载、处理和存储单个元素。
![在这里插入图片描述](https://img-blog.csdnimg.cn/202007291610016.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

#### Notes（注意）

- 这个方法比之前的两种方法慢，因为每个元素都要被单独地加载、处理和存储。
- 这种处理剩余元素的方式要两次循环：1次是处理向量，第2次是处理单个元素。这导致代码的数量会翻倍。
- Neon加载单数据仅改变目标元素的值，向量的其余部分保持不变。如果某些向量化的计算指令如`VPADD`，需要跨向量运作，那在载入第一个元素之前，该寄存器必须先被初始化。

#### Code Fragment（代码片段）

```markup
@ r0 = 输入数组指针
@ r1 = 输出数组指针
@ r2 = 数组的数据长度

    lsrs    r3, r2, #3           @  算出可以处理多少个完整的向量(lsrs是新的语法, 与lsr是一样的)
                                 @  这行代码看是把r2逻辑右移3位，即r3=r2/3
    beq  singlesetup             @  如果上面`lsrs`的结果为0, 则代表没有一个完整的向量
                                 @  (向量长度为8个element或以上)，则跳到下面`singlesetup`开始运行
    
                            
 vectors:                        @ 循环处理向量
    subs    r3, r3, #1           @ 更新剩余循环次数(r3=r3-1)
    vld1.8  {d0}, [r0]!          @ 从r0指向的内存加载8个element到d0寄存器, 并且更新指针 
    
    ...
    ...                          @ 处理d0中的数据
    ...
 
    vst1.8  {d0}, [r1]!          @ 把d0中的8个元素写到r1指向的内存, 并且更新指针
    bne  vectors                 @ 如果r3不为0, 则跳到`vectors`处继续循环
 
 singlesetup:
    ands    r3, r2, #7           @ 求r2和8的余数, 得到r3表示剩余几个单独的element要处理
    beq  exit                    @ 若r3的结果为0, 则跳到下面`exit`处
     
                                 
 singles:                        @ 循环, 处理单个element
    subs    r3, r3, #1           @ 更新循环计数器, r3=r3-1
    vld1.8  {d0[0]}, [r0]!       @ 加载单个element到d0, 然后更新指针
    ...
    ...                          @ 处理输入到d0[0]的数据
    ...
 
    vst1.8  {d0[0]}, [r1]!       @ 写单个element到r1, 然后更新指针
    bne  singles                 @ 若r3的结果不为0, 则跳到`singles`处继续循环
 
 exit:
12345678910111213141516171819202122232425262728293031323334353637
```



## Further Considerations（进一步思考）

### Beginning or End（开始或结束）

  重叠（overlapping）和单元素（single element）技术可以在处理数组的开始或者结束时使用。上面的代码很容易修改，在数组的开始或结束处理元素，以适配你的应用。

### Alignment（对齐）

  加载和存储地址应与高速缓存行对齐，以实现更有效的内存访问。
  这要求在Cortex-A8上至少对齐16个字节。如果你不能在输入或输出数组的开头对齐，那你必须在数组的开头或结尾处理元素。在开头处理是为了对齐，在结尾处理是为了处理那些剩余的元素。
  如果想要地址对齐以加快内存访问，记住要在加载和存储指令中使用`:64`或`:128`或`:256`地址限定符，以实现最佳性能。你可以参考对应内核的技术手册，来看看它们加载或存储时所需周期数。
[Cortex-A8技术参考手册相关页面](https://developer.arm.com/documentation/ddi0344/k/instruction-cycle-timing/instruction-specific-scheduling-for-advanced-simd-instructions/advanced-simd-load-store-instructions)

### Using Arm to Fix Up（用Arm的时候）

  对于单元素的情况，可以用Arm指令集单独操作每个元素。然而，同时用Arm和Neon指令集来保存同样一块内存，会降低性能。因为Neon的流水线完成后才到轮到Arm。（as the writes from the Arm pipeline are delayed until writes from the Neon pipeline have been completed）
  总的来说，应该避免同时用Arm和Neon的代码来写同一块内存，特别是同一行缓存、



================================================







 [**第1部分**](https://blog.csdn.net/Yemiekai/article/details/107653354)讲了如何用Neon来加载和存储数据，[**第2部分**](https://blog.csdn.net/Yemiekai/article/details/107662224)讲了对向量进行操作后，如何处理最后多出来的数据（指数据不够，无法凑成一个完整的向量）。
  本文介绍一种有用的数据处理：矩阵乘法。

## Matrices（矩阵）

  我们来看看怎样高效滴做4x4矩阵的乘法，这种操作在3D图像处理中经常用到。假设矩阵是按照列序（[column-major order](https://en.wikipedia.org/wiki/Row-_and_column-major_order#Column-major_order)）的形式存储在内存里——[OpenGL-ES](https://www.khronos.org/opengles/)就是采用这种格式。
![img](https://img-blog.csdnimg.cn/20200731090034807.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70,#pic_center)

## Algorithm（算法）

  我们对矩阵乘法进行分解，看看其中哪些操作可以通过Neon指令来实现。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200731092722420.jpg#pic_center)

  上图显示了结果矩阵C的第一列的计算方法。按照常识（行乘以列），矩阵C的第1列的第1个元素为矩阵A的第1行乘以矩阵B的第1列。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200731100816395.jpg#pic_center)

  如上图所示，从另外一个角度来理解，矩阵C的第1列可以这样算：它由4个列向量相加，第1个列向量为矩阵A的第1列乘以矩阵B第1列的第1个元素，如图中蓝色框框；第2个列向量为矩阵A的第2列乘以矩阵B的第1列的第2个元素，如图中紫色框框；剩下两个灰色框框的列向量请结合图示理解。这种思路可以用Neon来实现。↓↓↓
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200731101335334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

  如果矩阵的每一列数据，都是存储在Neon寄存器中的一个向量，我们可以用向量-标量乘法（vector-by-scalar multiplication）指令来完成这种操作，如上图所示。最后还要把这几个列向量的对应元素相加，这可以通过同样指令的加法版本来实现。
  当我们对第一个矩阵的列进行操作，产生一列结果时，从内存中读写元素是一种线性操作，不需要交错（interleaving）加载或存储指令。

## Code（代码）

### Floating Point（浮点型）

  先来看看单精度浮点矩阵乘法的实现。首先将矩阵从内存加载到Neon寄存器中，矩阵是列序（column-major order）的，所以矩阵的列是线性存储在内存里的。每一列可以通过`VLD1`加载到Neon寄存器中，然后用`VST1`写回到内存。

```markup
@ r是地址寄存器, r1指向矩阵0(也就就是矩阵A), r2指向矩阵(也就是矩阵B)

    vld1.32  {d16-d19}, [r1]!            @ 加载矩阵A的前8个元素,矩阵是列序的,也就是加载了两列. 然后更新指针
    vld1.32  {d20-d23}, [r1]!            @ 加载矩阵A的接下来8个元素
    vld1.32  {d0-d3}, [r2]!              @ 同理加载矩阵B的前两列
    vld1.32  {d4-d7}, [r2]!              @ 同理加载矩阵B的后两列
    
12345678
```

  Neon有32个64位寄存器，这里可以把两个矩阵的全部数据加载到寄存器里，并且还有多的寄存器可以用作累加器。这里`d16`到`d23`存储第一个矩阵的16个数字，`d0`到`d7`存储第二个矩阵的16个数字。（单精度浮点数是4个字节32位，每个寄存器是64位，可以存2个数字，所以8个寄存器可以存16个数字）

### An Aside: D and Q registers（插一段介绍：**D**和**Q**寄存器）

  大多数Neon指令可以有两种方式使用寄存器组：

- 分成16个**Quad-word**（4字长）寄存器，每个寄存器128位，名称为`q0`到`q15`
- 分成32个**Double-word**寄存器（2字长），每个寄存器64位，名称为`d0`到`d31`

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200731111054765.jpg#pic_center)

> 32位系统里一个字长（word）是32位，64位系统是64位。
> Reference：https://blog.csdn.net/fabulous1111/article/details/79525384

  这些`d`和`q`只是别名，其实`q0`中的数据与`d0` `d1`的数据相同，通过`d0`可以访问`q0`的前面64位数据，通过`d1`可以访问`q0`的后面64位数据。在C语言中这和联合体（union）很像。
  在本文的浮点矩阵乘法例子中，常使用**Q**寄存器，因为本文矩阵的一列是4个32位的浮点数，刚好存进一个128位的**Q**寄存器里。

### Back to the Code（看回代码）

  用4行Neon乘法指令算出矩阵C的第一列：

```markup
    vmul.f32    q12, q8, d0[0]      @ q8就是d16d17, 是矩阵A的第一列, q12 = q8 * d0[0]
    vmla.f32    q12, q9, d0[1]      @ q9就是矩阵B的第二列, q12 += q9 * d0[1]
    vmla.f32    q12, q10, d1[0]     @ 同理
    vmla.f32    q12, q11, d1[1]     @ 算完了
    
123456
```

  注意在本代码中，乘法指令中通过**D**寄存器来获取和使用标量。尽管`q0[3]`和`d0[1]`是一样的值，并且从语义上更好理解（第1列的第4个值）。但是作者正在使用的GNU汇编器不支持这种格式，所以只能从**D**寄存器来指定标量，或许你的汇编器不必如此。

  所以第一行代码的意思就是q 12 = [ x 0 y 0 , x 1 y 0 , x 2 y 0 , x 3 y 0 ] T q_{12} =[ x_0y_0,x_1y_0,x_2y_0,x_3y_0]^Tq12=[x0y0,x1y0,x2y0,x3y0]T，`q8`就是矩阵A的第一列的4个数字，`d0`就是矩阵B第一列的前2个数字，`d1`就是矩阵B第一列的后2个数字。
  后面几行代码差不多意思，自行理解，用的是乘加指令。4行代码实现了下面的运算：

q 12 = [ x 0 x 1 x 2 x 3 ] × y 0 + [ x 4 x 5 x 6 x 7 ] × y 1 + [ x 8 x 9 x A x B ] × y 2 + [ x C x D x E x F ] × y 3 = [ x 0 y 0 + x 4 y 1 + x 8 y 2 + x C y 3 x 1 y 0 + x 5 y 1 + x 9 y 2 + x D y 3 x 2 y 0 + x 6 y 1 + x A y 2 + x E y 3 x 3 y 0 + x 7 y 1 + x B y 2 + x F y 3 ] = [ x 0 x 4 x 8 x C x 1 x 5 x 9 x D x 2 x 6 x A x E x 3 x 7 x B x F ] ⋅ [ y 0 y 1 y 2 y 3 ] q_{12}= \left[

x0x1x2x3�0�1�2�3

\right]\times y_0 + \left[

x4x5x6x7�4�5�6�7

\right]\times y_1 + \left[

x8x9xAxB�8�9����

\right]\times y_2 + \left[

xCxDxExF��������

\right]\times y_3 \\= \left[

x0y0+x4y1+x8y2+xCy3x1y0+x5y1+x9y2+xDy3x2y0+x6y1+xAy2+xEy3x3y0+x7y1+xBy2+xFy3�0�0+�4�1+�8�2+���3�1�0+�5�1+�9�2+���3�2�0+�6�1+���2+���3�3�0+�7�1+���2+���3

\right] = \left[

x0x1x2x3x4x5x6x7x8x9xAxBxCxDxExF�0�4�8���1�5�9���2�6�����3�7����

\right] \cdot \left[

y0y1y2y3�0�1�2�3

\right]q12=⎣⎢⎢⎡x0x1x2x3⎦⎥⎥⎤×y0+⎣⎢⎢⎡x4x5x6x7⎦⎥⎥⎤×y1+⎣⎢⎢⎡x8x9xAxB⎦⎥⎥⎤×y2+⎣⎢⎢⎡xCxDxExF⎦⎥⎥⎤×y3=⎣⎢⎢⎡x0y0+x4y1+x8y2+xCy3x1y0+x5y1+x9y2+xDy3x2y0+x6y1+xAy2+xEy3x3y0+x7y1+xBy2+xFy3⎦⎥⎥⎤=⎣⎢⎢⎡x0x1x2x3x4x5x6x7x8x9xAxBxCxDxExF⎦⎥⎥⎤⋅⎣⎢⎢⎡y0y1y2y3⎦⎥⎥⎤



  如果只是算矩阵和一个向量的相乘（3D图形中另外一种常见的操作），那上面几行代码就搞定了，并且可以把结果向量保存到内存中。然而，要完成矩阵乘矩阵，还需要迭代几次，算完`q1`到`q3`寄存器中`y4`到`yF`的值。

  现在把上面这段代码封装成函数：

```markup
@ 上面的懂这个就懂了, 不写注释了
@ 函数名是mul_col_f32, 后面3个是参数

    .macro mul_col_f32 res_q, col0_d, col1_d
    vmul.f32    res_q, q8,  col0_d[0]      
    vmla.f32    res_q, q9,  col0_d[1]     
    vmla.f32    res_q, q10, col1_d[0]      
    vmla.f32    res_q, q11, col1_d[1]      
    .endm
    
12345678910
```

  4 × 4 4\times44×4 浮点数矩阵的乘法的代码看起来像这样：

```markup
@ 上面的懂这个就懂了, 不写注释了
@ 函数名是mul_col_f32, 后面3个是参数

    @ 加载数据
    vld1.32  {d16-d19}, [r1]!            
    vld1.32  {d20-d23}, [r1]!           
    vld1.32  {d0-d3}, [r2]!             
    vld1.32  {d4-d7}, [r2]!       
          
    @ 调用函数, 实现矩阵与向量的乘法
    mul_col_f32 q12, d0, d1           
    mul_col_f32 q13, d2, d3           
    mul_col_f32 q14, d4, d5           
    mul_col_f32 q15, d6, d7           
 
    @ 保存数据
    vst1.32  {d24-d27}, [r0]!  
    vst1.32  {d28-d31}, [r0]!  
 
12345678910111213141516171819
```

### Fixed Point（定点数）

  使用定点（fixed point）算术来计算，通常比浮点运算快，因为它使用较少的内存带宽来读写位数较少的值。对于相同的操作，整数的乘法通常快过浮点数的乘法。
  但是在用定点算术时，必须谨慎选择表示方式，以避免溢出或饱和，保证应用程序所需的精度。
  用定点算术实现矩阵乘法，与浮点相似。在本例中使用[Q1.14](https://en.wikipedia.org/wiki/Q_(number_format))定点数格式（Q1.14数字具有1个整数位和14个小数位），其操作与浮点数相似，只需要更改累加器最后部分的位移，下面是封装的函数：

```markup
    .macro mul_col_s16 res_d, col_d
    vmull.s16   q12, d16, \col_d[0]   @ q12 = d16 * col_d[0], d16是64位寄存器, 刚好存4个整形数
    vmlal.s16   q12, d17, \col_d[1]
    vmlal.s16   q12, d18, \col_d[2]
    vmlal.s16   q12, d19, \col_d[3]
    vqrshrn.s32 \res_d, q12, #14      @ shift right and narrow accumulator into
                                      @ Q1.14 fixed point format, with saturation
    .endm
 
12345678910
```

  相比于前面浮点数封装的函数，可以看到主要的区别如下：

- 现在一个数字是16位的，所以一个**D**寄存器可以**hold**得住4个数字。
- 两个16位数乘以两个16位数，结果是一个32位数。这里用`VMULL`和`VMLAL`，因为结果存储在**Q**寄存器，用两倍大小的元素保护结果的所有位数。
- 最后的结果必须是16位，但是累加器是32位的。用`VQRSHRN`得到一个16位的结果
- 

  每个元素从32位缩小到16位也会对内存访问有所影响；可以使用较少的指令加载和存储数据。定点数矩阵乘法的代码如下：

```markup
    vld1.16  {d16-d19}, [r1]       @ load sixteen elements of matrix 0
    vld1.16  {d0-d3}, [r2]         @ load sixteen elements of matrix 1
 
    mul_col_s16 d4, d0                      @ matrix 0 * matrix 1 col 0
    mul_col_s16 d5, d1                      @ matrix 0 * matrix 1 col 1
    mul_col_s16 d6, d2                      @ matrix 0 * matrix 1 col 2
    mul_col_s16 d7, d3                      @ matrix 0 * matrix 1 col 3
 
    vst1.16  {d4-d7}, [r0]         @ store sixteen elements of result
     
1234567891011
```

## Scheduling（调度）

  后面再详细讲讲调度（scheduling），现在先来看看改进后的调度指令的效果。
  在上面的`.macro`函数里，相邻的乘法指令写进同样的寄存器，由于指令集周期的问题，Neon（pipeline）必须等到每次乘法结束，才能开始下一条指令。
  如果从函数里面取出这些指令，重新排列，则可以用其它互不依赖的指令，来分离这些相互依赖的指令，就可以并行地进行。（原文不是这样讲的，但我没读透，不好翻译）
  在本例中我们可以重排代码，把访问累加寄存器的时机错开。

```markup
    vmul.f32    q12, q8, d0[0]              @ rslt col0  = (mat0 col0) * (mat1 col0 elt0)
    vmul.f32    q13, q8, d2[0]              @ rslt col1  = (mat0 col0) * (mat1 col1 elt0)
    vmul.f32    q14, q8, d4[0]              @ rslt col2  = (mat0 col0) * (mat1 col2 elt0)
    vmul.f32    q15, q8, d6[0]              @ rslt col3  = (mat0 col0) * (mat1 col3 elt0)
 
    vmla.f32    q12, q9, d0[1]              @ rslt col0 += (mat0 col1) * (mat1 col0 elt1)
    vmla.f32    q13, q9, d2[1]              @ rslt col1 += (mat0 col1) * (mat1 col1 elt1)
     ...
     ...
     
1234567891011
```

  这个版本的代码在基于Cortex-A8的系统上性能翻倍。

  你可以看看[你的内核的技术参考手册](https://developer.arm.com/documentation/ddi0337/e/DDI0337E_cortex_m3_r1p1_trm.pdf)，里面有关于指令周期延迟的详细描述。你值得花点时间熟悉它，以找到一些潜在的性能改进办法。



## Source（源码）

这里懒得翻译了，其细节在前面都分析了，这里是完整源码。保留原文保留原味。

```markup
@
@ NEON matrix multiplication examples
@

.syntax unified

@
@ matrix_mul_float:
@ Calculate 4x4 (matrix 0) * (matrix 1) and store to result 4x4 matrix.
@  matrix 0, matrix 1 and result pointers can be the same,
@  ie. my_matrix = my_matrix * my_matrix is possible.
@
@ r0 = pointer to 4x4 result matrix, single precision floats, column major order
@ r1 = pointer to 4x4 matrix 0, single precision floats, column major order
@ r2 = pointer to 4x4 matrix 1, single precision floats, column major order
@

    .global matrix_mul_float
matrix_mul_float:
    vld1.32     {d16-d19}, [r1]!            @ load first eight elements of matrix 0
    vld1.32     {d20-d23}, [r1]!            @ load second eight elements of matrix 0
    vld1.32     {d0-d3}, [r2]!              @ load first eight elements of matrix 1
    vld1.32     {d4-d7}, [r2]!              @ load second eight elements of matrix 1

    vmul.f32    q12, q8, d0[0]              @ rslt col0  = (mat0 col0) * (mat1 col0 elt0)
    vmul.f32    q13, q8, d2[0]              @ rslt col1  = (mat0 col0) * (mat1 col1 elt0)
    vmul.f32    q14, q8, d4[0]              @ rslt col2  = (mat0 col0) * (mat1 col2 elt0)
    vmul.f32    q15, q8, d6[0]              @ rslt col3  = (mat0 col0) * (mat1 col3 elt0)

    vmla.f32    q12, q9, d0[1]              @ rslt col0 += (mat0 col1) * (mat1 col0 elt1)
    vmla.f32    q13, q9, d2[1]              @ rslt col1 += (mat0 col1) * (mat1 col1 elt1)
    vmla.f32    q14, q9, d4[1]              @ rslt col2 += (mat0 col1) * (mat1 col2 elt1)
    vmla.f32    q15, q9, d6[1]              @ rslt col3 += (mat0 col1) * (mat1 col3 elt1)

    vmla.f32    q12, q10, d1[0]             @ rslt col0 += (mat0 col2) * (mat1 col0 elt2)
    vmla.f32    q13, q10, d3[0]             @ rslt col1 += (mat0 col2) * (mat1 col1 elt2)
    vmla.f32    q14, q10, d5[0]             @ rslt col2 += (mat0 col2) * (mat1 col2 elt2)
    vmla.f32    q15, q10, d7[0]             @ rslt col3 += (mat0 col2) * (mat1 col2 elt2)

    vmla.f32    q12, q11, d1[1]             @ rslt col0 += (mat0 col3) * (mat1 col0 elt3)
    vmla.f32    q13, q11, d3[1]             @ rslt col1 += (mat0 col3) * (mat1 col1 elt3)
    vmla.f32    q14, q11, d5[1]             @ rslt col2 += (mat0 col3) * (mat1 col2 elt3)
    vmla.f32    q15, q11, d7[1]             @ rslt col3 += (mat0 col3) * (mat1 col3 elt3)

    vst1.32     {d24-d27}, [r0]!            @ store first eight elements of result
    vst1.32     {d28-d31}, [r0]!            @ store second eight elements of result

    mov         pc, lr                      @ return to caller





@ Macro: mul_col_s16
@
@ Multiply a four s16 element column of a matrix by the columns of a second matrix
@ to give a column of results. Elements are assumed to be in Q1.14 format.
@ Inputs:   col_d - d register containing a column of the matrix
@ Outputs:  res_d - d register containing the column of results 
@ Corrupts: register q12
@ Assumes:  the second matrix columns are in registers d16-d19 in column major order
@

    .macro mul_col_s16 res_d, col_d
    vmull.s16   q12, d16, \col_d[0]         @ multiply col element 0 by matrix col 0
    vmlal.s16   q12, d17, \col_d[1]         @ multiply-acc col element 1 by matrix col 1
    vmlal.s16   q12, d18, \col_d[2]         @ multiply-acc col element 2 by matrix col 2
    vmlal.s16   q12, d19, \col_d[3]         @ multiply-acc col element 3 by matrix col 3
    vqrshrn.s32 \res_d, q12, #14            @ shift right and narrow accumulator into
                                            @  Q1.14 fixed point format, with saturation
    .endm

@
@ matrix_mul_fixed:
@ Calculate 4x4 (matrix 0) * (matrix 1) and store to result 4x4 matrix.
@  matrix 0, matrix 1 and result pointers can be the same,
@  ie. my_matrix = my_matrix * my_matrix is possible
@
@ r0 = pointer to 4x4 result matrix, Q1.14 fixed point, column major order
@ r1 = pointer to 4x4 matrix 0, Q1.14 fixed point, column major order
@ r2 = pointer to 4x4 matrix 1, Q1.14 fixed point, column major order
@

    .global matrix_mul_fixed
    
matrix_mul_fixed:
    vld1.16     {d16-d19}, [r1]             @ load sixteen elements of matrix 0
    vld1.16     {d0-d3}, [r2]               @ load sixteen elements of matrix 1

    mul_col_s16 d4, d0                      @ matrix 0 * matrix 1 col 0
    mul_col_s16 d5, d1                      @ matrix 0 * matrix 1 col 1
    mul_col_s16 d6, d2                      @ matrix 0 * matrix 1 col 2
    mul_col_s16 d7, d3                      @ matrix 0 * matrix 1 col 3

    vst1.16     {d4-d7}, [r0]               @ store sixteen elements of result

    mov         pc, lr                      @ return to caller
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970717273747576777879808182838485868788899091929394959697
```

================================================

下一篇文章：[Coding for Neon - Part 4: Shifting Left and Right](https://blog.csdn.net/Yemiekai/article/details/107717238)





## Shifting Vectors（向量的位移）

  Neon上的位移操作和Arm代码中位移非常相似。位移操作使向量中每个元素的每一位都向左或向右移动，左边或右边超界的位会被丢弃，不会影响到相邻元素的数据。
  位移量用指令中的立即数指定，或者由附加的位移向量来决定。使用位移向量时，输入向量中每个元素的位移，取决于位移向量中对应元素的值。位移向量里的元素被视为有符号值，所以每个元素都能进行左移、右移或者不动。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200731171552355.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  对有符号元素向量进行右移（指令上会附加类型指示），将会对每个元素进行符号扩展。这和Arm代码中算术位移是一样的。对无符号向量进行位移则不会进行符号扩展。

### Shifting and Inserting（位移和插入）

  Neon还支持插入位移，提供了一种方法来组合两个向量的位数据。例如左移插入（`VSLI`）将源向量的每个元素左移，空出来的位置由目标向量的对应位填充。详见下图↓↓↓
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200731172542896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

### Shifting and Accumulation（位移和累加）

  最后，Neon支持将向量的元素右移，然后把结果累加到另一个向量上。对于这种情况是很有用的：在组合低精度累加器的结果之前，以高精度进行中期计算。

## Instruction Modifiers（指令修饰符）

  每个位移指令可以带有1个或多个修饰符。这些修饰符不会改变位移操作本身，但是输入或输出会被调整，去除偏差或者限制（saturate）至某个范围。下面是5种位移修饰符：

- **Rounding**，用`R`前缀表示，修正右移舍位导致的偏差。（就是四舍五入？）
- **Narrow**，用`N`后缀表示，使结果的每个元素中的位数减半，从**Q**寄存器（128位）到**D**寄存器（64位）的时候使用。
- **Long**，用`L`后缀表示，使结果的每个元素中的位数翻倍，从**D**寄存器到**Q**寄存器的时候使用。
- **Saturating**，用`Q`前缀表示，当结果超出指定范围时，将每个结果元素设置到指定的上限和下限范围内。这里用到向量的位数和符号类型来决定`saturating`范围。
- **Unsigned Saturating**，用`Q`前缀加`U`后缀表示，和上面的`saturation`指令相似，但是结果会被saturate到一个无符号范围内，无论输入是有符号还是无符号。

  一些组合的修饰符没啥用，因此Neon也没有提供这些指令。例如饱和右移（`VQSHR`）就没有必要，因为右移会使结果更小，因此值根本不会超出有效范围（饱和指令不工作）。

## Table of Shifts Available（可用位移表）

  Neon提供的所有位移指令显示在下面的表格了。他们是根据前面提到的修饰符来排列的。如果你还不确定这些修饰符的含义，请利用下表来选择你需要的指令。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803113028353.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

## An Example: Converting Color Depth（例子：转换颜色深度）

  转换颜色深度是图像处理中常用的操作。输入或输出数据通常是16位的RGB565彩色格式，而处理RGB888的数据更加容易。特别是在Neon上，没有对类似RGB565这种类型数据的原生支持。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080311404136.png#pic_center)
  然而，Neon仍然可以有效地处理RGB565数据，上面提到的向量位移就提供了一种方法来处理。

### From 565 to 888（565转888）

  先来看看RGB565到RGB888的转换。假设有8个16位像素点在`q0`寄存器中，准备拆分红、绿、蓝，分别变成8位的元素，存到`d2`到`d4`3个寄存器中。

```markup
    vshr.u8      q1, q0, #3      @ shift red elements right by three bits,
                                 @ discarding the green bits at the bottom of the red 8-bit elements.
    
    vshrn.i16    d2, q1, #5      @ shift red elements right and narrow,
                                 @ discarding the blue and green bits.
    
    vshrn.i16    d3, q0, #5      @ shift green elements right and narrow,
                                 @ discarding the blue bits and some red bits due to narrowing.
    
    vshl.i8      d3, d3, #2      @ shift green elements left, discarding the remaining red bits, 
                                 @ and placing green bits in the correct place.
    
    vshl.i16     q0, q0, #3      @ shift blue elements left to most-significant bits of 8-bit color channel.
    
    vmovn.i16    d4, q0          @ remove remaining red and green bits by narrowing to 8 bits.
    
1234567891011121314151617
```

  每条指令的作用都写在注释上面了，总的来说，作用在每个通道的操作是：

1. 通过位移来移动颜色数据，相邻通道的颜色数据将会被舍弃掉。
2. 使用第二次位移将颜色数据定位在每个元素的最高有效位，并缩小元素，从16位减小到8位

  注意，为了实现某些掩码运算，这些操作的元素大小会对齐到8位或者16位。

**下面是关于这段代码的图解：**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803180137841.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)



#### A small problem（一个小问题）

  你可能会注意到，如果用上面的代码来转换成RGB888图像，白色可能不够白。这是因为对于每个通道，最低的两到三位是0，而不是1，RGB565中的白色是 （0x1F, 0x3F, 0x1F），在RGB888中变成了（0xF8, 0xFC, 0xF8）。这可以通过位移来解决，将一些最重要的位插入到低位中。

### From 888 to 565（888转565）

  来看看反过来的操作，RGB565转到RGB888，这里RGB888的数据是上面代码产生的格式，数据分开存放在`d0`到`d2`3个寄存器上，每个寄存器保存每种颜色的8个元素。结果以8个16位RGB565的形式存在`q2`上。

```markup
     vshll.u8  q2, d0, #8      @ shift red elements left to most-significant bits of wider 16-bit elements.
     
     vshll.u8  q3, d1, #8      @ shift green elements left to most-significant bits of wider 16-bit elements.
     
     vsri.16   q2, q3, #5      @ shift green elements right and insert into red elements.
     
     vshll.u8  q3, d2, #8      @ shift blue elements left to most-significant bits of wider 16-bit elements.
     
     vsri.16   q2, q3, #11     @ shift blue elements right and insert into red and green elements.
     
1234567891011
```

  每条指令都写了注释，总地来说，对于每个通道：

1. 每个元素被扩展到16位，把颜色数据移动到最重要的位置上。
2. 使用插入位移，将每个颜色通道放置到结果寄存器中。

## Conclusion（总结）

  Neon提供的强大位移指令让你可以：

- 快速地将向量乘以或者除以2的倍数，附带rounding和saturation（四舍五入和限定大小）。
- 将一个向量的位，移动或者拷贝到另一个向量。
- 以高精度进行中期计算，以较低精度积累结果。

================================================







## Introduction（介绍）

  写Neon代码的时候，有时候你会发现，在[寄存器](https://so.csdn.net/so/search?q=寄存器&spm=1001.2101.3001.7020)中的数据格式和你算法的要求不太符合。你可能需要重新排列向量中的元素，好让后面的算法可以将正确的部分相加。或者传给函数的格式很奇怪，必须重新排序，让高速的SIMD代码能够处理。
  这种重新排序（reordering）的操作叫做置换（permutation）。Permutation指令重新排列单个元素，这些元素来自单个或多个寄存器，组成一个新的向量。

## Before we begin（在开始之前）

  在使用Neon提供的重排指令之前，考虑一下是不是真的需要使用。置换指令和移动（move）指令很像，因为它们通常表示准备数据而不是处理数据所消耗的CPU周期。
  在使用最少的周期来完成一项任务之前，你的代码速度不是最优的。重新安排指令的顺序通常能够优化速度。（[第3部分](https://blog.csdn.net/Yemiekai/article/details/107705430)的调度章节有提到过）

### Alternatives（可选方案）

  如何避免不必要的重排？下面有一些方案：

- **Rearrange your input data**（重新安排输入数据）。用更合适的格式存储数据通常不需要花费任何成本，避免了在读取或者存储时置换数据。然而，在修改数据结构之前，请考虑到数据的局部性，以及对缓存性能的影响。
- **Redesign your algorithm**（重新设计算法）。可以考虑使用不同的算法，它用差不多相同数量的处理步骤，但是可以处理不同格式的数据。
- **Modify the previous processing stage**（修改前面的处理策略）。在早期处理阶段的一个小改动，调整数据存储到内存的方式，可以减少或者消除对置换的需求。
- **Use interleaving loads and stores**（使用交错加载和存储）。像[前面](https://blog.csdn.net/Yemiekai/article/details/107653354)讲的，加载和存储指令有交错（interleave）和反交错（deinterleave）功能。即使这不能完全消除置换的需要，但也能减少额外的指令。
- **Combine approaches**（组合方法）。使用上面提到的一种或多种技术仍然比置换（permutation）指令有效。

  要是这些方案都考虑过了，还是无法合适地处理数据，那就要用置换指令（permutation instructions）了。

## Instructions（指令）

  Neon提供了一系列置换（permutation）指令，从基本的翻转到任意的向量重建。简单的重排可以通过单个周期的指令来实现，复杂的操作就要通过多个周期，还可能用到额外的寄存器。与往常一样，请定期对代码进行[基准测试或分析](https://developer.arm.com/tools-and-software/embedded/arm-development-studio/components/streamline-performance-analyzer)，并检查处理器的技术参考手册（[Cortex-A8](https://developer.arm.com/ip-products/processors/cortex-a/cortex-a8?_ga=2.30091899.594786909.1596423316-241183949.1595923255)，[Cortex-A9](https://developer.arm.com/products/processors/cortex-a/cortex-a9?_ga=2.268132938.594786909.1596423316-241183949.1595923255)）以获取性能详细信息。

### VMOV and VSWP: Move and Swap（移动和交换）

  `VMOV`和`VSWP`是最简单的置换（permute）指令，拷贝整个寄存器的内容到另外的寄存器，或者交换一对寄存器的值。
  尽管你可能不认为他们是置换指令，但可以用来改变两个**D**寄存器的值，这两个寄存器构成一个**Q**寄存器。例如`VSWP d0, d1`交换了`q0`寄存器的高64和低64位。

### VREV: Reverse（反转）

  `VREV`反转向量中8、16或32位元素的顺序，有3种不同的方式：

- `VREV16`反转每对8位子元素，组成向量中的16位元素。
- `VREV32`反转4个8位子元素或者2个16位子元素，组成向量中的32位元素。
- `VREV64`同理，可反转8个8、4个16或者2个32位子元素。

  使用`VREV`可以反转数据的字节序，重新排列颜色分量或交换音频样本的通道。

![在这里插入图片描述](https://img-blog.csdnimg.cn/2020080320120782.png#pic_center)

### VEXT: Extract（提取）

  `VEXT`从一对现有的向量中提取字节，组成一个新的向量。这允许你生成一个新的向量，其中包含的元素跨越了一对现有的向量。
  `VEXT`可以用来在两个向量上实现滑动窗口，在**FIR**滤波器上非常有用。对于置换（permutation），当两个输入操作数使用相同的向量时，也可以用来模拟按字节旋转的操作。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803202241245.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

### VTRN: Transpose（转置）

  `VTRN`在一对向量之间转置8、16或32位元素。 它将向量的元素视为2x2矩阵，并转置每个矩阵。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803202520622.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  使用多个`VTRN`指令来转置较大的矩阵。例如，由16位元素组成的4x4矩阵可以使用三条`VTRN`指令进行转置。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803202656478.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  在加载向量之后或存储向量之前，这与`VLD4`和`VST4`执行的操作相同。 由于它们需要较少的指令，因此请尽可能尝试使用这些结构化的内存访问功能，而不要使用一系列`VTRN`指令。

### VZIP and VUZP: Zip and Unzip（压缩和解压）

  `VZIP`交错（interleaves）一对向量的8、16或32位元素。在存储数据之前，该操作与`VST2`执行的操作相同。所以如果需要在写回内存之前立即压缩数据，请使用`VST2`，而非`VZIP`。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803203213163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  `VUZP`是`VZIP`的反向操作，反交错（deinterleaving）一对向量的8位，16位或32位元素。在从内存加载数据之后，该操作与`VLD2`所做操作相同。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803203420627.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)

### VTBL, VTBX: Table and Table Extend

  `VTBL` 从向量表和索引向量构造一个新的向量。这是一个逐字节的表查找操作。
  该表由1到4个相邻的**D**寄存器组成。 索引向量中的每个字节都用于索引向量表中的一个字节。 将索引值插入到结果向量中与索引向量中原始索引位置相对应的位置。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200803203823948.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1llbWlla2Fp,size_16,color_FFFFFF,t_70#pic_center)
  `VTBL`和`VTBX`在处理超出范围索引的方式上有所不同。如果一个索引超过了表的长度，`VTBL`会在结果向量的相应位置插入0，但是`VTBX`保持结果向量中的值不变（如上图，`d2`原来是多少就是多少）。
  如果将单个源向量用作表，则`VTBL`允许您实现向量的任意排列，但要以设置索引寄存器为代价。 如果是在一个循环里面使用该操作，并且排列的类型不变，则可以在循环的外部初始化索引寄存器，并消除设置开销。

### Others

  虽然还有其他方法可以实现类似置换的操作，例如使用加载和存储指令对单个向量元素进行操作，但是这些方法所需的重复内存访问会使它们的速度明显变慢，因此不建议使用这些方法。

## Conclusion（总结）

  明智的做法是仔细考虑代码是否真的需要做数据置换（permute）。但是，当您的算法需要时，置换指令提供了一种有效的方法来将数据转换为正确的格式。