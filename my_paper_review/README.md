**Paper review draft**

[TOC]

# 100-papers-in-50-days plan

本计划开始于2021年5月13日，计划在50天内读完100篇安全相关的论文，目的是对安全领域的各研究方向有一个大致的了解，也就是看看大家都在干什么。对于每篇论文本身不求甚解。

## 1. Reassembleable Disassembling (USENIX Secutiry 15) - 2021/05/13

这篇论文的研究主题很有意思，就是如何把从commercial-off-the-shelf (COTS) binaries with most symbol and relocation information stripped反汇编得到的汇编指令改的可以再次汇编回去，这样大家以后补丁软件就可以直接修改反汇编出的汇编文件了，而不是改二进制、额外增加段。不过提出的方法不太优雅。

![](http://image.hupeiwei.com/paper/reassemble.PNG)

整体架构如图所示，分为两大模块，Disassembly Module和Analysis Module。反汇编模块采用objdump进行反汇编，其中Disassembly Validator的作用是检查objdump是否报错，有错误就假设是data gap导致的，判断data界限并重新反汇编。反汇编后的结果包括Data、Meta-Data（PLT和导出表）、Code。

![](http://image.hupeiwei.com/paper/symbol-lifting.PNG)

Analysis Module里面最重要的是Symbol Lifting过程，这个过程处理上图所示的四种符号引用。为了简化处理，提出了一系列的假设。我看的时候随手记了些，不全：

> 两种立即数：指令操作符中的常量、数据段中的字节流
>
> 超出地址空间的不会是符号的引用
>
> 引用应该指向代码段或者数据段、且指向代码段的位置应该是一些指令的起始地址（识别出了c2c c2d）
> ----接下来处理数据段（d2c d2d），思路是把它切片
> A1 - 所有存储在数据段的引用是n字节对齐的，n=4 for 32 bit and n = 8 for 64 bit
> A2 - 用户不需要修改原始二进制数据。如果这个假设成立，可以不改变数据段的起始地址，这样整体的数据段地址不变，就不需要对d2d进行符号化。
> A3 - d2c符号引用只包含对函数指针和跳转表项的引用
>
> ......

我个人感觉论文里面比较重要的就是Symbol lifting过程，以及提出的几个假设。假设与实验程序的契合程度直接影响了效果。我没有仔细看论文，也不能提出建设性的意见与问题。论文给我的直觉是主题很有意思，但是通过规则的问题解决方式很容易被defense，COTS略施小计可能效果就不行了。但是作为这一个问题的第一个解决方案，毕竟给出了可行的方法。

## 2. Automatic Firmware Emulation through Invalidity-guided Knowledge Inference (USENIX Security 21) - 2021/05/13

这篇文章针对的是固件运行时周边设备的兼容问题。对IoT设备固件进行动态测试时，因为执行测试的设备往往不具备IoT设备具备的周边设备，因此会遇到兼容性问题。现在有三个方向在发展，一是把相关请求转发到真实硬件，一是模拟Linux内核的硬件抽象层，一是全系统模拟，对未知周边设备建立交互模型，来为固件中对周边设备的请求提供响应。本文属于最后一种路线。作者认为现有第三种路线的工作提供响应时忽视了不同周边设备寄存器的共同作用。如：

```c
if (EMAC->CR & EMAC->SR == 0x1E7FF)
Enable_Ethernet_Interrupt();
```

CR寄存器和SR寄存器共同影响了运行。

作者提出的方法包括两个阶段，一是知识抽取阶段（Knowledge Extraction Phase），一是动态分析阶段（Dynamic Analysis Phase）。我感觉重点是第一个阶段。知识抽取阶段将对周边设备的访问设置为符号，通过符号执行来探索各路径，然后记录为了保持运行状态有效周边设备的响应值。第二阶段运行至某一需要访问周边设备值的位置时，询问第一阶段的保存值，给出周边设备的响应结果。

第一阶段设备响应值保存的级别有4种。T0是读设备寄存器值时返回该设备寄存器最近被写入的值。T1通过pc值和访问的设备寄存器的地址来查询返回响应值，保存格式类似T1_addr_pc_NULL_value，T1_0x40023800_0x10000_NULL_0x00代表PC为0x10000时访问了0x40023800（设备寄存器的地址）的话，这个设备响应值0x00。T2是T1加上上下文哈希，保存格式如T2_addr_pc_contextHash_value。T3是T1加上符号执行运行成功时的多个响应值（不只有一个响应值），保存格式如T3_addr_pc_null_{v1,v2,...}。它们每个有各自的道理，论文中有详细介绍。

第一阶段运行示意如下图：

![](http://image.hupeiwei.com/paper/afe.PNG)

本文提出的方式基于符号执行，符号执行有两个固有的问题，路径爆炸与约束求解。作者说运行状态无效时直接终止该路径的继续运行，因此缓解了路径爆炸问题；但是约束求解问题依然存在（我粗略看，没有看到作者介绍缓解措施）。

之前看过一个FirmAFL，提出了一个增强进程仿真（augmented process emulation）代替QEMU的全系统模拟。那个相当于是对QEMU仿真固件的改进，挖洞效果是用增强进程仿真替代AFL中的QEMU用户模式，和本文的优化层次不太一样。

## 3. Black Widow: Blackbox Data-driven Web Scanning (S&P 21) - 2021/05/14

这是一篇Web Scanner相关的论文，感觉Web安全（至少这篇）和系统安全相比，工作难度确实低一些。作者认为之前的工作聚焦在解决问题的一部分（有的只是JavaScript事件，有的只是程序状态），并且缺少对状态之间依赖关系的考虑。

作者的解决方案包括三个部分：edge-driven navigation with path-augmentation, complex workflow traversal, and fine-grained inter-state dependency tracking。其中第一部分旨在如何获得更多的程序状态，第二部分是如何复现（到达）某一个已知状态，第三部分是如何发现不同状态间的依赖关系。

其实这几个算法图就把流程说的很详细了：

![](http://image.hupeiwei.com/paper/web-alg1.PNG)

![](http://image.hupeiwei.com/paper/web-alg2.PNG)

![](http://image.hupeiwei.com/paper/web-alg3.PNG)

navigation中一个节点就是一个状态，状态包括页面的状态（即页面的URL）和JavaScript的状态（即触发了哪些事件）。navigation中的边包括一个节点和一个动作（GET requests, form submission, iframes and JavaScript events），意味着当前状态经过该动作转向下一个状态。

先看算法2，它的作用是通过向前找到一个safe的边构建从该safe边到当前位置的workflow，目的是找到能够到达某状态的路径（或者说workflow）。所谓safe的边是指当前边中的action是一个GET操作。为什么GET是safe的，要从GET开始一个workflow呢？我觉得是因为GET操作足够简单，不包含过于复杂的信息，且它往往是业务操作的开始；因此从GET操作开始到目的状态的路径中应该包含了所需的信息。算法3也很简单，一个是判断某边包含的节点状态是否有token，如果有则添加token来源到该状态的依赖关系。另一个是搜索节点参数并添加token的。算法3用于判断状态间依赖关系。算法1简单明了，更不用说了。

这篇文章不断强调XSS的问题，因为判断依赖关系有利于发现XSS。

## 4. BAP: A Binary Analysis Platform (International Conference on Computer Aided Verification 2011) - 2021/05/14

这个文章不是一个典型的顶会论文，比较简短，就当是了解了解二进制分析平台的架构了。

本篇文章介绍的BAP是作者团队开发的第三代二进制分析平台。第一代的方式是将汇编反编译成C语言，然后进行接下来的分析。这样的缺点是很多汇编里面的side-effect不能体现出来。所谓的side-effect，例如执行完指令一些标志位的变化，它们可能影响跳转是否执行。第二代的增加了中间语言，并且side-effect蕴含在了中间语言里。但是这一代的中间语言本身语义不明确，且对ARM的支持不好。第三代，也就是BAP，吸取了之前两代的经验，主要目标有：1. 将side-effect明确表示出来 2. 使用简单的具有良好语义的中间语言 3. 包含对二进制程序的分析和验证技术 4. 运行用户自定义分析。

![](http://image.hupeiwei.com/paper/BAP.PNG)

## 5. Ramblr: Making Reassembly Great Again (NDSS 17) - 2021/05/16

这篇文章是接着Uroboros做的，获得了2017年NDSS的distinguished paper award。

*Reassembleable Disassembling*里面有很多假设和粗糙的推测，比如对于反汇编代码中的立即数，如果在内存区域内，就当做是符号引用，否则当成一个常量。这篇文章主要改善这一个点-**不能正确区分一个立即数是常量还是符号引用**，使用程序分析的方法（Intra-function Data Dependence Analysis 和 Localized Value-set Analysis）替代原来粗糙的推测。

作者提出的方法中重点步骤是Content Classification和Symbolization。其中Content Classification就是使用上面提到的两种程序分析方法恢复数据类型，具体的步骤我也没太看懂，感觉介绍也不很详细。Symbolization主要介绍了两点，一是 Base Pointer Reattribution。这个应对的情况包括由于常量折叠优化，counters[input - 'A'] ++这种语句，假设counters起始地址为0x804a034，正常的取要++的值的方式是（0x804a034 + (ebx - 'A')\*4），优化后'A'\*4的值提前计算了，于是汇编变成了（0x8049f30+ebx*4）这样。但是0x8049f30可能不包括在内存区域内，导致被识别为常量。措施是以4KB增加内存区域看看能不能落在里面，如果可以，进行前向切片直至一个和该值相关的解引用操作，通过值集分析看看访问了哪个内存区域，这个应该就是优化前的base pointer。二是Data Consumer Check，其实就是一个保障性措施，对于没有符号化的立即数，看它是否涉及了一个不包含其它符号引用的指针或跳转操作；对于符号化的引用，看看有没有什么不寻常的操作。由于上面的分析heavyweight，还提出了一个快速的方法，在Fast Workarounds一节。

我对程序分析方法还不够了解，多看看相关的论文，比如值集分析和程序切片的。

## 6. (State of) The Art of War: Offensive Techniques in Binary Analysis (S&P 16) - 2021/05/17

这篇是Giovanni Vigna组的SoK，主要包括两个部分，一是当前二进制分析中的offensive tech，二是angr的设计。我着重看了第一部分，第二部分大致看了看。

首先文章介绍了自动化二进制分析中的trade-off，主要是replayability和semantic insight。关于replayability，如果replayability高了，代码覆盖率就会低；因为为了replayability就要理解如何到达某一位置，就影响覆盖更多的代码（存疑）。另一方面，如果不注重replayability，没有验证bug的输入，就容易有高的假阳性。关于semantic insight，注重语义的可能要存很多数据（如每个分支的值）。同时注重replayability和semantic insight的可扩展性又会收到影响。

在静态漏洞发现部分，分为基于图的和基于数据的，首先介绍了控制流图恢复的问题（难点在于indirect jump和衡量代码覆盖情况）及评价标准（soundness和completeness）。然后介绍了基于图的漏洞发现（往往发现已有漏洞）和基于数据的漏洞发现（值集分析，over-approximate）。

在动态漏洞发现部分，介绍了Dynamic Concrete Execution中的fuzzing和dynamic symbolic execution（Classical dynamic symbolic execution、Symbolic-assisted fuzzing、Under-constrained symbolic execution - 运行程序的一部分）。

在漏洞利用部分，首先讲了造成崩溃的样例复现时的问题，缺少数据和缺少关系。然后讲了AEG，自动从崩溃产生漏洞利用。但是由于exp缓解措施的发展，最后讲了为了应对它们的Exploit Hardening。

后面的部分是angr的设计，细节过多，暂时不研究它。

## 7. One Engine to Fuzz ’em All: Generic Language Processor Testing with Semantic Validation (S&P 21) - 2021/05/17

Wenke Lee组的工作，认为现在对language processor（如编译器/解释器）的fuzzer都是一个语言有一个，这样太繁琐，每一个fuzzer工作量都挺大，一个语言的处理器出一个fuzzer何时了？于是，提出了一个通用的针对语言处理器的fuzzer-POLYGLOT。

![](http://image.hupeiwei.com/paper/polyglot1.PNG)

主要有两步：**第一步**根据提供的BNF产生一个能够把源程序转换成IR的前端，并设计了一个注解格式让用户提供语义，这些注解被编码在IR的语义属性中。**第二步**首先对第一步获得的IR进行**constrained mutation**，并保证整个程序的语法正确和未变异部分的语义正确。首先，基于IR的类型对IR进行变异（如用if替代if）；其次，只变异具有local effects的IRs。  接下来，还要保证变异部分的语义正确，通过**semantic validation**。该阶段根据IR的语义属性收集被变异case的type和scope信息，然后生成symbol tables，其中包含types、scopes和names of every definition。这些信息帮助修正无效的变量使用。

生成的中间语言如下图最左，这个中间语言比较贴近于源文件，方便fuzz，和那种具有良好语义的自己可以编译运行的中间语言不同。下图中间是BNF，下图最右是注解，这两部分都要用户提供。

![](http://image.hupeiwei.com/paper/polyglot2.PNG)

下图是semantic validation中用到的数据结构，简明易懂。

![](http://image.hupeiwei.com/paper/polyglot3.PNG)

本文工作重点其实是如何生成良好的种子集，fuzz部分直接用的AFL逻辑。AFL能捕捉崩溃，但是语言处理器的错误可以是不造成崩溃的错误，比如运算出1+1=3，这种错误是捕捉不到的。

现在顶会上fuzz的文章由对过程的优化向不同应用场景（固件、编译器等等）转变，估计是通用的过程优化的点实在不好找了。

## 8. Jetset: Targeted Firmware Rehosting for Embedded Systems (USENIX Security 21) - 2021/05/18

这一篇文章试图通过建立硬件设备模型解决固件仿真的问题，特点是targeted，也就是用户指定代码位置，生成的硬件模型中包含的访问响应将运行路径导向该指定位置。

整体的思路比较简洁，分为两个步骤，先是peripheral inference，然后是peripheral synthesis。第一个步骤将对MMIO的内存访问（即硬件设备访问）值设定为一个符号，使用符号执行技术记录相关分支的约束条件。至于往哪一个分支走，根据callback distance和callchain distance的和来确定。不有时候距离长的可能更有利于到达target，所以当某一位置访问多次时就根据distance的相反结果选取一次分支。第二个步骤使用第一步的结果创建一个适用于仿真器（如QEMU）的硬件模型。

这篇文章的工作和*Automatic Firmware Emulation through Invalidity-guided Knowledge Inference*很像，它们也都是USENIX Security 21上的文章，核心思路都是把硬件访问当成符号，利用符号执行技术求解约束后，利用这一信息辅助之后的运行。但是它们选择运行路径的方式不同（或者说缓解路径爆炸问题的方式），这一篇是有一个target，根据到target的距离；另一篇是Invalidity-guided。

## 9. Architectures for Intrusion Tolerant Database System (ACSAC 02) - 2021/05/18

这篇文章的领域我不太懂，是入侵容忍检测课程选的论文，但是它的作者Peng Liu想必安全圈里的人都知道。这篇文章的意思是基于攻击预防的数据库系统不够用了，需要入侵容忍的，于是作者提出了5种架构的入侵容忍数据库系统，特点是聚焦于transaction-level。

第一种架构如下图所示。重点就是入侵检测模块和修复管理模块（包括评估和修复）。入侵检测部分，作者提出算法要满足1. 应用的语义可以被捕获和使用 2. 多层的入侵检测。In [15], we have developed a simple cartridge like detector where bullets are supported through DLL modules and a rule based mechanism is used to build the cartridge。修复管理模块，评估要定位受影响的transaction，修复要把被影响的值恢复到最新的undamaged version。如果停止DBMS的运行修复的话挺简单，但是会影响业务运行，因此要repair on-the-fly。难免遇到1. 因为业务还在进行，要不断向前修复 2.在攻击修复阶段被修复的数据可能又被攻击3. 修复阶段可能一直不终止。解决1. 后修复的事务不会影响之前修复的2, we must not mistake a cleaned object as damaged, and we must not mistake a re-damaged object as already cleaned. To tackle challenge 3, our study in [2] shows that when the damage spreading speed is quicker than the repair speed, the repair may never terminate. Otherwise, the repair process will terminate, and under the following three conditions we can ensure that the repair terminates: (1) every malicious transaction is cleaned; (2) every identified damaged object is cleaned; (3) further (assessment) scans will not identify any new damage (if no new attack comes) 总的来说，架构1就是检测，定位，运行中修复。

![](http://image.hupeiwei.com/paper/dbms1.PNG)

第一种架构中malicious transaction从被commit到被检测到的时间间隔会影响很多transaction，因此第二种架构进行了改进，detector有两种级别的警报，疑似与恶意。恶意警报处理与架构1相同，疑似警报后将该用户的transaction隔离起来，若最后认为恶意，直接抛弃；否则merge。

![](http://image.hupeiwei.com/paper/dbms2.PNG)

第一种架构通过不允许读取被定位到受影响的数据来防止damage的扩展，但是数据从被commit到被定位到的时间间隔还会扩散damage。因此架构3使用multi-phase damage containment technique来进行缓解。思想就是宁肯误抓不可漏抓。因为漏抓的会传播，误抓的不会造成错误、传播。damage containment 快速 contain 可能由入侵造成damage的部分（防止扩散），uncontaning阶段再释放那些误抓的。

![](http://image.hupeiwei.com/paper/dbms3.PNG)

一个良好的入侵容忍系统能够根据环境的变化调整自己，第四种架构就引入了这一功能。前面架构的可信度、恢复能力可以根据参数定义（如界定疑似与恶意的threshold），本架构结合启发式算法对前面的功能参数进行调整，实现入侵容忍系统对环境的适应。

![](http://image.hupeiwei.com/paper/dbms4.PNG)

上面的第四种架构可以根据环境适应自己了，比如设置integrity level 0.92，就是10000个数据对象里面能有800个坏的，设置0.98就是能有200个坏的。但是，DBMS中不同的用户可能对integrity level有不同的要求，这样就得按照要求最高的用户设计系统，浪费了资源。因此，作者提出了面向用户提供QoIA（Quality of Information Assurance）Service，一种关联特定信息可信度的服务，不同用户信息可信度不同。

![](http://image.hupeiwei.com/paper/dbms5.PNG)

## 10. How Far We Have Come: Testing Decompilation Correctness of C Decompilers (ISSTA 20) - 2021/05/19

这是一篇survey性质的论文，调研了四个反编译器（JEB3、IDA Pro、RetDec、Radare2/Ghidra）的反编译效果。意在回答三个问题：RQ1: how difficult is it to recompile the outputs of modern C decompilers?; RQ2: what are the characteristics of typical decompilation defects?; and RQ3: what insights can we deduce from analyzing the decompilation defects?

背景里介绍了常见**反编译器的流程**：1. 前端：把二进制反汇编，利用反汇编结果生成IR。2. 中端：根据IR，恢复变量、变量类型、控制流。可能有一些死代码消除、到达分析的优化。3. 后端：根据中端结果生成C。还有常用来寻找编译器缺陷的**Equivalence Modulo Inputs (EMI) Testing**。两个程序被定义为EMI，如果对于两个程序都合法的任何输入，输出是一样的。EMI Testing就是从代码文件A变异生成不影响运行路径的源代码文件B，看看这两个编译后运行结果是不是一样。如果不一样，说明编译器有问题。

接着介绍了Motivation，大意是工业界已经有recompile成功的案例了，学术界依然保存保守态度，即使使用也是使用decompile的中间结果IR。反汇编、反编译技术[17,31,64-67]技术成熟了，也有平台[55]做能recompile的了。因此是时候做个测试了。分析IR而不是反编译的输出的话，1. 很多基础模块算法需要重新实现（如符号执行，现在的实现基于C而不是IR）2. 即使分析IR也会遇到反编译遇到的变量与类型恢复问题（分析IR并不省事）。In short，我们要decompile then recompile。

![](http://image.hupeiwei.com/paper/howfar1.PNG)

上图是测试的流程。源文件p，先变异生成q，然后把q编译、再反编译成q'，从q'中抽取函数替代p中相应的位置，再编译成可执行文件。最后，这个可执行文件的运行结果和p直接编译的结果进行比较。图中有三个关注的错误，decompilation failure（反编译器报错或崩溃）、recompilation failure（重编译时报错或崩溃）和decompilation defects（指两个可执行运行结果不一致）。

最后测试结果如下：

![](http://image.hupeiwei.com/paper/howfar2.PNG)

最后回答三个问题：

关于RQ1与RQ2 ，直接用输出结果重编译没有一例成功，实验中有一些重编译成功是因为程序简单且做了一些代码调整。RQ1与RQ2的问题作者分开说了，其实根源就是**type recovery、variable recovery、control-flow recovery、optimization**。至于RQ3，一是很多学术界新技术并没有集成到decompiler中，一是现有decompiler输出的重编译能力和可读性都不好，可能和注重生成结构紧凑的代码有关。

记得看BAP时说自己第一代是二进制直接恢复成C，结果分析效果不好，因为缺少了汇编的side-affect（比如flag reg的变化），于是后面是恢复成IR，IR中编码了side-affect。但是本篇说IR不好，the transformed IR lacks high-level expressiveness, and this absence can impede many standard dataflow analyses and symbolic reasoning facilities [10]。目前我也不能给出自己的结论。此外，看完文章认为作者对于recompile还是过于乐观了，毕竟测试程序那么简单，实际用处不大；如果测试实际程序效果也不错，作者应该迫不及待给出实验结果了（如果我是作者的话，哈哈哈）。

## 11. Countering Kernel Rootkits with Lightweight Hook Protection (CCS 09) - 2021/05/25

由于内核rootkit的严重危害，越来越多的研究防范和发现rootkit。已有工作主要集中在以下几个方向：1. 分析rootkit的行为，如Panorama、HookFinder等，2. 通过被rootkit感染后的特征检测rootkit，如Copilot、SBCFI等，3. 阻止恶意rootkit代码的运行来保护内核代码的完整性，但是这种方式可以通过ret2plt/libc等攻击绕过。因此，作者认为，除了保护内核代码的完整性，还要通过保护内核控制数据来保护内核控制流的完整性，所谓内核控制数据是指能在内核运行种装入PC的数据，主要有函数返回地址和函数指针，本文工作重点在于保护函数指针。

本文要保护的是函数指针（本文中function pointer和kernel hook含义相同），函数指针主要存储在两个部分，一是kernel和LKM的段中，一是动态分配的区域，如内核的堆。作者通过实验有两个观察：一是kernel hook分散地分布在不同的页上，这意味着基于页的硬件保护会错杀页上很多非kernel hook；二是对kernel hook的写操作很少，这与作者方案中分开处理读操作与写操作有联系。

**总体架构**

作者提出了一个基于hypervisor的轻量架构来保护客户机的kernel hook。作者方案主要包括两大部分，一是Offline Hook Profiler，一是Online Hook Protector。

其中，Offline Hook Profiler的作用是收集kernel hook的访问信息。对于输入的每一个kernel hook，它输出一个hook access profile，其中包含了什么地址的什么指令对该kernel hook进行了什么类型的操作。这些访问kernel hook的指令被称作Hook Access Points（HAPs）。作者实现的HookSafe原型中使用QEMU的system-mode实现了HAP的分析。

Online Hook Protector模块的功能是读入Offline Hook Profiler的输出，创建所有要保护的hook的影子备份（shadow copy）；然后将HAP指令进行插桩，这样运行至这些指令时对原来hook的访问就会被重定向到影子备份。当访问被重定向后，会根据访问的类型进行不同的操作，如果是读操作，直接从影子备份返回hook值；如果是写操作，由于内存保护的存在，只有hypervisor有权力更改影子备份的内存区域，因此会发起一个hypercall，交由hypervisor判断该写操作是否合法。原型HookSafe中的策略是该被写入的值需要是Offline Hook Profiler分析过程中出现过的值。

Online Hook Protector中还有两个值得考虑的情况。一是如何发现并保护动态分配的hook（虽然在Offline Hook Profiler阶段输入了Kernel Hooks，并且也收集了动态分配的hook的信息，在实际运行中很多hook还是不能被这囊括的，因此需要在Online Hook Protector

过程发现并保护动态分配的hook）。作者的方案是对涉及到的内存分配与释放函数进行插桩并利用运行时的上下文信息判断新分配的对象是否包含hook，如果是，就创建它的影子备份。另一个问题是如果在HookSafe启动前就分配了的hook怎么分析与保护。作者认为，一个包含hook的内核对象但凡被创建出来，就一定有一个关于它的引用能够访问到它，不然它被创建出来也不能使用。因此，作者采用分析全局变量的方法寻找包含hook的内核对象。

**技术方案**

实现部分值得一提的是如何通过对HAP处进行补丁，实现hook indirection的。对于HAP指令，通过将其指令位置重写为jmp到trampoline位置的指令来实现跳转，这个跳转指令占5个字节。若HAP指令多于5字节，空余的位置填充NOP；若HAP指令小于5字节，则也重写下一条指令，然后下一条指令在从trampoline返回前执行。这样看上去是可行的，但是会遇到两个问题，一是两条HAP指令离得太近，补丁第一条时破坏了第二条；二是被重写的下一条指令可能是一个跳转指令的目标，这样直接跳过了执行会产生异常。作者的解决办法是复制原来的旧函数来产生一个新函数，并且补丁新函数的HAP，HAP空间小于5字节时，HAP后的指令不重写，而是后移。然后，在旧函数的入口处补丁为跳转到新函数的指令。这样，从头执行函数的去执行新函数了，跳转过来的能够执行原来的完整指令。

**优缺点**

1. 在offline hook profiler阶段需要提供kernel hooks作为输入，收集kernel hooks的工作并不简单，尤其是实验中第二个集合里的kernel hooks来自手动分析。
2. hypervisor认为某一kernel hook在offline hook profiler中出现过的值是可以合法写入的，这意味着如果在offline hook profiler阶段混入恶意值，HookSafe会在online hook protector阶段将恶意的值合法写入。
3. hypervisor认为某一kernel hook在offline hook profiler中出现过的值是可以合法写入的，那么如何保证offline hook profiler阶段的动态运行是充分的，收集到了所有的合法值？实际也有可能一个没出现过的值也是合法的。文章对写入hook的判断依据对实际应用是否会产生限制，能产生多少限制没有讨论。

## 12. Threshold-Optimal DSA/ECDSA Signatures and an Application to Bitcoin Wallet Security - 2021/05/30

课程读书报告要求的论文，我对这个领域并不懂，论文也没有看太懂，为了交作业必须得写。

**作者所提方案过程的理解：**该方案共分为三个阶段，初始化阶段、密钥生成协议和签名生成阶段。初始化阶段生成用于陷门承诺和DSA的一些参数。在密钥生成阶段，首先每一个参与者选择x，并计算y=gx，然后将计算出的y当作陷门承诺的信息计算[C，D]=Com(y)。然后，每一个参与者广播自己的C。根据作者的介绍，陷门承诺的一个特性是对于持有陷门（trapdoor）的人，当它发送出去C后，可以重新计算一个D’，使得当初C和D加密的信息M变为C和D’加密的信息M’。这就像将一个物品锁到密码箱并给了别人后，通过给别人不同的开箱密码，别人会看到密码箱中不同的物品。参与者广播自己的C后，在它广播自己的D前，这个被加密的y都是不确定的、无法破解的。在广播C后，每个参与者再广播D和α=E(x)。每个参与者根据别人广播的D，可以计算出别人的y，然后把每个人的y乘在一起得到一个新的y。每个参与者将别人广播的α累加起来，由于同态加密的性质，α1+α2+…=E(x1+x2+…)，最终得到一个新的α=E(x1+x2+…)。这样，密钥生成阶段最终的输出就是结合了每个人的值的y和α=E(x)，且有y=gx（因为同底幂相乘等于指数相加）。这里的两个输出，和要签名信息的哈希值，是第三步签名生成阶段的输入。

**方案的保密性：**首先，无论是第二阶段还是第三阶段，消息的广播都依赖了陷门承诺方案。广播是有延迟的，使用陷门承诺先发送C，再发送D；在解密者收到D前，密文是什么是不确定的。D和C之间的时间，是通过陷门承诺方案额外争取到的安全时间。其次，虽然签名需要进行一些计算，但是这些计算都拥有同态加密方案的保护，被广播的私钥辅助计算部分是同态加密后的，这样也在不影响计算的前提下增强了保密性。

**方案的可用性：**这篇文章如果实现t+1的话，其实是要求坏人不作恶。也就是说，坏人能够看到传递的消息，但是不能修改应作出的行为。并且，正如论文所说：we assume that if any player does not perform according to the protocol (e.g. by failing a ZK proof, or refusing to open a committed value), then the protocol aborts and stops。如果出现了不按照协议规则进行的参与者，整个过程直接终止，这意味着它鲁棒性不够强。

**文章的创新性或相关工作的理解：**我对相关领域背景知识不太了解，就这篇论文来说，它的创新点就是结合同态加密，将DSA签名方案扩展到了门限最优，比已有DSA门限方案对参与者要求更低，这样DSA签名可以适用于多方共同签名的场景。这里说对参与者要求更低，其实是从数量上来说的；但是作者提出的方案要求坏人不作恶，所以对参与者数量要求更低，但是行为要求更高了。在提出本文的DSA门限方案后，作者又提出了一个应用场景—对比特币钱包的保护，说明了本方案的实用性。

## 13. Decompilation of Binary Programs (SPE 1995) - 2021/06/02

提出了反编译中的一些问题：1. 数据与代码无法区分。2. 由于编译器和链接器导入的很多子函数。

反编译器的结构：

前端（机器相关）：读入二进制，输出low ir 和CFG。
1. indexed or indirect addressing mode
2. two levels of ir
3. idiom analysis - 组合的指令序列用功能等价的ir替换
4. type propagation - 在idiom analysis之后，将引用归并

universal decompiling machine：
1. 数据流分析 
    1.1 对分支判断进行合成，如cmp和b.ge合并成 x >= y
    1.2 对寄存器的处理，没看太懂
2. 控制流分析 - 把CFG结构变成高级语言的控制结构，包含一个chehck for short circuit evaluation grphs，能组合分支判断条件。

后端（语言相关）：读入前一阶段输出的结构化的CFG和高级IR，先进行restructure（指对于一些CFG结构生成输出高级语言特有的结构），然后生成高级语言代码。

还有一个函数签名生成判断部分来识别那些编译器和链接器自动加入的函数，解决子函数过多的问题。

## 14. DIRE: A Neural Approach to Decompiled Identifier Naming (ASE 19) - 2021/06/12

这篇文章的主要工作是使用神经网络对反编译后代码的变量进行命名。我个人认为这是很有意义的一项工作，尤其是看着ida反编译后代码中类似var1、var2的变量时。

本文所提框架如下图所示，主要包括encode和decoder两个部分，encoder部分使用lexical encoder（一个双向LSTM网络）分析代码序列，使用structural encoder分析ast获取结构信息。encoder输出两个东西，一是coder element representation（对于lexical encoder就是代码中的词，对于structural encoder就是ast中的节点），一是identifier representation（待命名变量）。decoder读入encoder的输出，预测待命名变量的名字。decoder使用了LSTM decoder和注意力机制。

![](http://image.hupeiwei.com/paper/dire1.PNG)

本文的贡献除了上述的DIRE框架，还有一个自动化生成训练数据的方法。其实里面比较值得注意的就是不同的反编译场景可能生成不同的代码结构，如何将不同代码结构下的变量对应起来（训练模型的数据有这个需求）。反编译的结果虽然不一样，但是反汇编的结果相对可靠，作者采用的方法是通过记录某一反编译后的变量在反汇编代码中被指令访问的位置，相同的访问位置集合意味着相同的变量。这个方法确实不错。

作者介绍自动生成训练数据的方法时，有一步是使用（修改后的）ast指导反编译器产生反编译代码（we instruct the decompiler to generate decompiled C code from the modified AST），我不懂怎么做的。

实验结果简示如下图：

![](http://image.hupeiwei.com/paper/dire2.PNG)

##　ａ

# git-related vulnerability discovery

## A Practical Approach to the Automatic Classification of Security-Relevant Commits (ICSME18, CCF-B)

本文为了解决软件供应链中开源软件漏洞出现后修补不及时的问题，提出了实时监控软件的commit信息并通过模型自动判断此commit是否安全相关来提示开发人员进行及时的修补。模型提取了commit的代码信息与日志信息，将代码当做自然语言文档进行分类处理。

## VCCFinder: Finding Potential Vulnerabilities in Open-Source Projects to Assist Code Audits (CCS15)

本文为了解决对commit代码的审计问题。使用传统的代码分析工具往往要一次分析整个项目，且误报率很高。本文建立了一个数据库，内含CVE漏洞和造成该漏洞的commit的映射。本文同时分析代码和github上能够获取的元信息（metadata，包含的很广泛），然后训练模型（we first create a joint representation for the heterogeneous features using a generalized bag-ofwords model and then apply a linear Support Vector Machine ）并实现对commit的安全性预测。

## Automated Identification of Security Issues from Commit Messages and Bug Reports (ESEC/FSE17, CCF-A)

本文和VCCFinder要处理的问题一样。同样的，本文也构建了一个数据库—基于来自github的commit、bug report，来自JIRA和Bugzilla的bug report。commit采用commit message作为特征，bug report采用title, description, comments, comment number, attachment number, labels, created date, and last edited date作为特征。模型采用了k-fold stacking，就是将数据分为k份训练k轮，第k轮则采用第k份数据作为测试集，其它作为训练集；并且SVM、随机森林等多个模型同时训练，这些模型的测试结果再输入到logistic回归进行最终预测。本文工具效果比VCCFinder好，作者声称是因为VCC-Finder的SVM对不对称数据效果不好。和VCCFinder相比改进了预测算法。

## When a Patch Goes Bad: Exploring the Properties of Vulnerability-Contributing Commits (ESEM13, CCF-B)

以Apache HTTP server中的68个漏洞以及与之关联的124个VCC为对象（个人感觉数据量不大），通过git记录分析得到如下结论：

![](http://image.hupeiwei.com/paper/patch_bad.PNG)

## Understanding security mistakes developers make: Qualitative analysis from Build It, Break It, Fix It (USENIX Security 20)



## VulPecker: An Automated Vulnerability Detection System Based on Code Similarity Analysis (ACSAC 16, CCF-B)

由于一个库可能被很多软件使用，库中的漏洞修复了不代表使用该库的软件也修复了漏洞，更何况提供的一些修复patch可能不包含某个版本（如3.1之前都受影响，却只提供的3.1的patch）。这篇文章试图通过代码相似性算法查找出软件中存在的已爆出漏洞。总体思路是**先**根据漏洞的diff hunk（features）选取高效的代码相似性检测算法（选取过程首先要求算法超过一个准确度threshold，并能区分unpatched/patched；随后测试他们最合适的code fragment level；最后选取false-negative最小的算法），**然后**对抽取的the patched/unpatched diff code and the unpatched code fragment corresponding to a vulnerability进行预处理并转换成代码相似性算法需要的代码表示，这就生成了漏洞签名；**最后**对目标程序同样进行预处理并转换成需要的代码表示，生成目标程序的签名，然后根据代码相似性算法就行比较，找到可能的位置。

# fuzz

## Evaluating Fuzz Testing (CCS18)

这篇文章主要是指出了对fuzzer性能进行评估时要注意的一些事项。文章的工作量看上去挺大，但是这些问题本身应该是大家心里或多或少已经意识到的；这篇文章把它们进行了表述，并用实验加以佐证。我觉得它最大的用处是提供了一个清晰的逻辑框架，以后再看其它fuzz相关论文时，可以从本文描述的角度对它们的实验设计进行评估，对其性能的真实提升进行判断。

文章从**运行的随机性**、**种子的选取**、**运行时间的设置**、**度量方法**与**目标程序的选取**上进行了讨论。

由于模糊测试的随机性，作者推荐多次运行并进行基于统计的分析，来增强结果的可信度。

从种子选取角度，作者建议公开自己的种子集及种子选取标准，并且选取多种种子，包括空种子。

对于运行时间，作者建议至少24小时；否则给出不长时运行的理由。并且，用图表表示出运行情况随时间的变化。

对于度量方法，作者驳斥了崩溃数度量，并且认为通过AFL中覆盖率计算工具或者栈哈希对崩溃进行去重都是不可靠的。作者推荐使用找出的bug数量，次之使用基本块或边缘覆盖率作为性能度量标准。

对于目标程序的选取，作者推荐已知bug的一系列基准测试程序，包括CGC、LAVA-M和能够得到bug修补的旧版本程序。

## Full-speed Fuzzing: Reducing Fuzzing Overhead through Coverage-guided Tracing (S&P 19)

这篇文章试图解决在fuzz过程中的这样一个问题：fuzzer对每个testcase的覆盖率都进行了追踪与计算，借此判断此testcase是否覆盖了新的块；但是，大多数testcase都是无法覆盖新块的，因此，大多数计算得到的覆盖率信息都是无用的，反而造成了overhead。

为了解决这一问题，作者将目标程序插桩成两个版本，一个是interest oracle，另一个是tracer。

将目标程序的每一个基本块处插入一个中断，就得到了interest oracle。当运行产生中断时，便是覆盖了新的基本块。此时，使用tracer进行覆盖信息的收集，并移除interest oracle中被覆盖基本块处的中断。随着测试的进行，未被覆盖的块越来越少，interest oracle和原始目标程序也就越来越像。如果testcase的运行没有产生中断，说明没有新的块被覆盖，则跳过覆盖信息收集，直接测试下一个testcase。

这样，大大减少了覆盖信息收集的次数，进而减少了测试过程的overhead。

![img](http://image.hupeiwei.com/paper/full-speed.png)

我的问题：移除中断产生新interest oracle的overhead有多大？

## Fuzzing JavaScript Engines with Aspect-preserving Mutation (S&P 20)

这篇文章主要解决fuzzer该如何利用poc作为种子集进行fuzz。poc作为已发现漏洞的验证，以它作为种子开始fuzz更有可能找到漏洞。但是，目前粗暴的变异方法无法保证poc保持特有的结构，尤其是对于JavaScript引擎这种对种子格式要求比较高的程序。

通过对已有漏洞的分析，作者提出了structure-preserving 和 type-preserving 两种针对poc的变异规则；第一种要求保持种子的运行结构（如控制流），第二种要求保持变量的类型。

为了实现上述变异规则，作者采用了对poc的ast节点进行变异的方法，而非常见的基于二进制位的变异，这个有意思。

![](http://image.hupeiwei.com/paper/js.png)

## Matryoshka: Fuzzing Deeply Nested Branches (CCS 19)

之前已经有很多文章通过污点分析等技术观察影响条件判断语句的字节位置，通过变异指定字节来尝试改变判断的结果。但是，**当出现深层嵌套的条件判断时，对影响目标条件判断的字节进行变异，可能会导致外层的条件判断结果改变，进而根本无法到达目标条件判断**，这是本文试图解决的问题。

对于一个条件判断语句，Matryoshka首先使用常规的求解方式，如果失败了，再依次使用prioritize reachability、prioritize satisfiability、joint optimization 这三种方式。

![img](http://image.hupeiwei.com/paper/matryoshka.png)

在上述三种方式执行之前，有以下几个信息需要收集：

1. 获得prior conditional statements。对于目标条件判断语句，首先要收集其prior conditional statements, 即在目标条件判断之前的，结果的改变能够影响目标条件判断能否到达的条件判断语句。
2. 获得effective prior conditional statements。假如条件判断语句*St*受到字节位置1影响，该位置同时也能影响*St*的一个prior conditional statement *Sp*，那么*Sp*就是*St*的一个effective prior conditional statement。显然，为了求解*St*，需要着重关注它的effective prior conditional statement，因为其它的prior conditional statements并不会受到*St*对应字节位置变异的影响，而是类似于*Sp*这样的条件判断可能会发生判断结果的改变，影响*St*的可达性。

有了上述的关键信息（原文中描述该收集过程为Determine control and taint ﬂow dependency），首先使用prioritize reachability。这是一种悲观的变异方式，它假设所有对能够同时影响目标条件判断和其effective prior conditional statements的字节的变异都会影响effective prior conditional statements的判断结果，进而影响可达性。因此，它只对能够影响目标条件判断却不能影响effective prior statements的字节位置进行变异。

如果上述过程没有成功解决，则尝试prioritize satisfiability。这时一种乐观的变异方式，它假设所有对影响目标条件判断的字节的变异都不会影响其可达性，因此，通过故意保持所有prioritize conditional statements的判断结果，判断变异是否解决了目标条件判断。如果解决了，再用原始的目标程序运行一遍该输入，检查是否真的没有影响其prioritize conditional statements。如果真的没有印象，成功解决；如果有影响，再回溯并fuzz解决各个prioritize conditional statements。

如果上述过程还没有成功解决，则尝试joint optimization。它要求同时满足由目标条件判断和其prioritize conditional statements导出的方程，并采取了一系列优化措施。

## Muzz: Thread-aware Grey-box Fuzzing for Effective Bug Hunting in Multithreaded Programs (USENIX Security 20)

本文针对多线程程序的fuzz提出了几点fuzzer的改进。

过程从对目标程序的静态分析开始，获得多线程相关函数（如多线程创建函数、锁等）的出现位置；然后根据它们的位置确定suspicious interleaving scope（Lm）。

随后是三个插桩时的改进。Coverage-oriented插桩通过概率的变化对目标程序的不同位置进行插桩。简单来说，是否在Lm内，内存操作数量影响了该部分被插桩的概率。Thread-context插桩保存线程运行的上下文环境来提供feedback。Schedule-intervention插桩通过函数Fs来给不同线程分配不同优先级，并分配ID。

![img](http://image.hupeiwei.com/paper/muzz.png)

在动态运行阶段，种子根据当前队列的新覆盖率、新线程上下文环境概率选取；在一个种子的执行阶段，如果产生了更多的不确定运行状态，则有机会获得更多的运行次数。

concurrency vulnerability 可以在fuzz过程中捕获，而像资源竞争、死锁这样的concurrency bug则要通过fuzz后再次运行种子并用TSan检测。

## Not All Coverage Measurements Are Equal: Fuzzing by Coverage Accounting for Input Prioritization (NDSS 20)

Edge coverage是指导fuzz进行的一个重要指标。有一些edge涉及的代码可能比其它edge更容易产生漏洞，但是，在很多fuzzer中不同edge却被同等对待，如何区分不同edge的重要性是本篇文章要解决的问题。

作者从提出了三个衡量措施。1. Function。作者爬取统计了易产生内存泄漏的函数（memcpy等，文章的实现着重于挖掘内存漏洞），对于一个edge，统计它所到达块包含的此类函数数量。2. Loop。如果该edge是一个back edge，记为1。3. Instruction。统计对于一个edge，它所到达块中涉及内存操作指令的数量。

如果三者的和大于0，则该edge是一个security sensitive edge，一个输入含有的security sensitive edge数量，指导它在种子队列中的优先程度。这样，不同的edge被不同对待，并指导fuzz的进行。

## ParmeSan: Sanitizer-guided Greybox Fuzzing (USENIX Security 20)

这是一篇Directed Greybox Fuzzing（DGF）的文章，题目中含有Sanitizer-guided，其实只是用在了一开始寻找target的地方。当fuzz开始后，进化方向还是根据覆盖率决定的。

-Target acquisition

作者将使用Sanitizer插桩前后的程序进行对比，那些被修改的地方，就是target。如果获得的结果过多，作者描述了两种修剪方法。一是去掉高频路径上的，因为高频路径产生bug的概率低一些；二是保留被插桩修改较多的函数中的，因为这种函数比较复杂，产生bug的概率更高。

-Dynamic CFG

动态CFG是以静态CFG为基础的，在运行中如果发现了静态CFG中不包括的路径（如：indirect call），则对其进行更新。除了CFG，测试过程中还维护一个Conditional Graph（CG），它只包含条件判断。计算距离时使用的是CG，而距离的定义很简单，不再赘述。

-Fuzzer

--种子队列的策略

种子队列中的每一项包含种子本身和它uncover的条件分支，队列按照该项种子的运行次数及其到目标的距离排序。

当经过变异的输入提高了覆盖率时，对其进行数据流分析来获得新基本块的污点信息（污点信息可以辅助到达目标分支，可以辅助目标分支的解条件约束）；当经过变异的输入遇到了新的条件分支时，则将该分支、该输入、距离放入队列。

--三个阶段

tracing phase 主要进行动态CFG的更新；directed exploration phase 尝试到达目标分支处，即target；exploitation phase 对已经到达的目标分支解条件约束，试图触发错误提示。

## REDQUEEN: Fuzzing with Input-to-State Correspondence (NDSS 19)

Fuzz中两个重要的问题是magic byte和checksum。很多fuzzer采用污点分析和符号执行的方法去处理，但是具有overhead过高、要求可以获得源代码的缺点。本文基于对输入和程序运行时状态（寄存器与内存）值联系的观察，提出了一种轻量的方法来处理magic byte和checksum。

(对于magic byte的处理)**（步骤1）** 当一个输入开始fuzz前，先单独运行一次并hook所有的比较指令及它们的参数。**（步骤2）**由于在运行时无法获得比较指令的类型是等于、大于还是小于，因此通过对参数加减1的方式获得观察值(observed)的**variation**。**（步骤3）**其次，由于程序中比较指令的参数不一定和输入完全一致，而是输入经过了某种编码，因此，本文还为REDQUEEN设置了一些常见的编码。**（步骤4）**以`if(a > 123456)`为例，假设a直接来自输入，且不经过编码，本次运行的输入为233，则可以得到运行时的observed value <233，123456>和variation <233, 123455>、<233, 123457>，第四步就是将输入中233对应的二进制部分替换为数字对右侧的值。

上述即为对于magic byte的大致处理流程。不过，假设上述`if(a > 123456)`的运行时观察值是一个一字节的0，恰巧当前输入中一字节的0又特别普遍，那么步骤4的替换就需要做很多次，且很多都是没有意义的。为了解决这个问题，本文通过用随机字节替换输入的某些部分来增加输入的熵值，并且只在目标替换位置在很多个输入中都出现时，才进行步骤4的替换。

![img](http://image.hupeiwei.com/paper/redqueen.png)

REDQUEEN不仅hook比较指令，还观察以至少两个指针为参数的函数。并以假设(1)函数要比较指针所指的前n个字节（n∈{4,5,…,32}）或假设(2)函数要比较指针所指处直到遇到第一个空字节来获得比较的操作数。

（对于checksum的处理）由于已经hook了比较指令，REDQUEEN通过一些标准猜测某比较指令是否属于checksum。对于符合的，对其进行patch运行，如果确实产生了新路径，再采用类似于magic byte的处理方式试图使输入能够自然到达该新路径。

## Send Hardest Problems My Way: Probabilistic Path Prioritization for Hybrid Fuzzing (NDSS 19)

This paper tries to find the proper strategy of invoking concolic execution in hybrid fuzzing, since its high overhead. Based on the observation of avaliable methods, demand launch and optimal switch, the authors give two principles: use concolic execution to solve the hardest problems and a lightweight way to decide which one is harder (Or which execution path is harder to arrive).

This paper proposes a new way to deicide which lucky testcase will be accepted by concolic execution and disclose some hard to arrive path. During the conventional fuzzing, the following extra tasks are done.

1. save the hit times of each branch during fuzzing loop (called *execution sampling* in original paper)
2. get every branch that avaliable testcases ever arrive and calculate their probabilities according to equation 1 (called *execution tree construction* in original paper)
3. leveraging CFG and hitted branches, calculate the probabilities (indicate the ability to be touched by conventional fuzzing) of all untouched branches and use concolic execution on branch with lowest probability. As for the detail implementation, concolic execution accepts the testcase which can touch its neghbor branch, which means most similar exectuion path. (called *probability based path prioritization* in original paper)

![img](http://image.hupeiwei.com/paper/digfuzz.png)

## Skyﬁre: Data-Driven Seed Generation for Fuzzing (S&P 17)

本文针对基于生成的fuzzer，它们能够根据提供的语法产生通过语法（syntax）检测的输入，但是却**往往不能通过目标程序的语义（semantic）检测**。因此，本文实现了skyfire，可以以语法和语料库作为输入，产生很多测试样例；这些样例则作为种子输入像AFL这样的fuzzer进行测试。

![img](http://image.hupeiwei.com/paper/skyfire.png)

skyfire分为学习和种子生成两个过程。在学习过程中，首先根据所提供的语法，生成语料库的AST；然后，遍历AST，生成probabilistic context-sensitive grammar (PCSG)。所谓PCSG，就是上下文无关文法的定义额外添加了上下文环境(曾祖节点、祖父节点、父节点、第一个兄弟节点的信息)和当前产生式出现的概率。

在学习获得PCSG后，进入种子生成阶段。首先根据PCSG从开始符号开始迭代生成可能的结构，在生成时优先选择出现少、结构简单的产生式。在获得生成的输出后，再基于覆盖率过滤掉一些；然后对它们AST上进行叶节点级别的变异，以期获得不同的语义进行fuzz。随后，结果输出做为fuzzer的种子集。

## Steelix: Program-State Based Binary Fuzzing (FSE 17)

这篇文章主要应对模糊测试中magic byte的问题。大部分fuzzer都会收集覆盖率信息作为反馈信息来调节种子变异的方向，**本文的创新点在于额外加入了对于comparison progress信息的收集，即某分支处已经有多少个从首位或末位连续匹配的字节**。例如，magic byte为“MAZE”，另一个受到输入影响的比较操作数为“MAXX”，则记为state 2。对于长度为N的magic byte，共有N+1个state（从首位或从末位连续匹配相同字节被归并为同一状态，如“MAXX”和“XXZE”）。当发生state更新时，fuzzer会对当前输入被变异字节前后两个字节进行穷举式变异，试图再次触发state更新，以确定变异方向，不断接近magic byte。

**为什么要采用从头或尾连续匹配数作为评价标准？**

还是以“MAZE”为例，从“MXZX”当然也可能变异到“MAZE”，但是这样面临着两个潜在的问题，一是状态空间过大，而是变异方向不确定。采用从头或尾连续匹配作为标准，可以将状态空间简化到N+1；并且方便确定变异方向。

**为何有效？**

如果不收集comparison progress信息，可能一个含有“MAZX”的输入由于没有触发覆盖率变化而被丢弃，大大降低了效率。comparison progress信息可以解决这个问题。

**整体工作流程？**

首先使用IDAPython进行静态分析，来获得基本块信息（覆盖率计算）和比较指令信息（comparison progress计算）。在比较指令中，过滤掉单字节的比较（简单的bitflip即可解决）和涉及函数返回值的比较（输入和magic byte对应关系过于复杂）。结合比较指令信息和Dyninst进行动态插桩，获得运行时的比较操作数，进而获得comparison progress信息，通过共享内存传递。当comparison progress发生更新时，触发上述穷举式变异部分，以确定变异方向，不断接近magic byte。

注1：fuzzer为binary-level，不需要源码。

注2：覆盖率相关部分与AFL类似，故文章没有详述。

## FIRM-AFL: High-Throughput Greybox Fuzzing of IoT Firmware via Augmented Process Emulation (USENIX Security 19)

这篇文章是第一个针对IoT程序的灰盒fuzzer，试图解决对IoT进行fuzz时的兼容性与效率的问题。具体方式是通过结合QEMU的user-mode和system-mode，大部分时间在user-mode下运行（效率高）；当遇到系统调用时，切换到system-mode运行（兼容性考虑），执行后再将CPU state传回user-mode继续运行。

为了减少内存转换的损失，两种模式的内存通过一个RAM file共享。为了减少系统调用转换到system-mode的损失，将固件文件系统挂载到宿主机使得user-mode可以直接获得，这样文件相关的系统调用在user-mode下就可以处理了。还有一些preload page（防止fork出来的子进程频繁缺页）、lightweight snapshot的优化。

pros - 第一个IoT的灰盒fuzzer，结合QEMU的user-mode和system-mode来平衡兼容性和效率。cons - 非文件相关的syscall redirection影响很大，作者实验中只用了一个程序来表现这一点；AFL不是为IoT程序设计的，也许可以增加一些针对性设计。

# others

## DEEPVSA: Facilitating Value-set Analysis with Deep Learning for Postmortem Program Analysis (USENIX Security 19)

本文的工作是用神经网络辅助值集分析时的内存别名分析（即分析哪些指令引用了同一块内存区域）。为什么要进行值集分析呢？当一个程序崩溃时，根据崩溃时留存的最后n行指令，结合值集分析，可以回溯找到导致崩溃发生的具体指令。如前所述，最后n行指令，意味着指令不完整；因此内存别名分析所依赖的上下文环境也不完整。**这里的神经网络就是预测出那些不明确的指令是属于堆、栈、还是全局。**

本文所提出的网络结构如图所示。原始指令对应的机器码经过word embedding（采用了独热编码）后，输入到EI层进行instruction embedding。EI层是一个bi-LSTM。从EI层输出后，再输入到learning model里面预测它访问的内存区域是栈、堆、还是全局。此处的learning model也是一个bi-LSTM。

![img](http://image.hupeiwei.com/paper/deepvsa1.png)

作者不断强调，自己和其他采用神经网络进行二进制分析的工作不同，因为本文网络中额外的instruction embedding考虑到了指令间的关系。

本文的实验包括两个部分，一个是对网络预测效果的评估，一个是对预测结果对值集分析辅助效果的评估。在第一个实验中，作者将本网络与传统的机器学习算法对比，说明传统的算法无法很好的解决该预测问题；同时也将本网络与其他直接读取原始机器码的双向神经网络进行对比，来说明自己的instruction embedding有效。

![img](http://image.hupeiwei.com/paper/deepvsa.png)

第二个实验中原始的VSA对40个测试程序中的27个程序成功进行了崩溃溯源，而有了预测结果辅助的DEEPVSA则对37个进行了崩溃溯源。

**Strength**

1. 作者一直强调的instruction embedding。考虑机器码所处的环境来进行向量化其实不是新鲜事了，asm2vec中也在提倡考虑上下文环境。
2. 实验设计合理。训练集和测试集进行了考虑周到的去重，多个模型进行对比说服力强。

**Weakness**

1. 本文模型比其他三个双向神经网络模型效果好的主要原因是对堆的预测特别成功，为什么它对堆的预测一枝独秀，作者没有进行相关的解释。
2. 本文模型和其他三个双向神经网络模型相比确实提高了预测的准确度，但是，百分之几的准确度提升对崩溃溯源的影响有多少，作者没有进行实验。也许使用其他三个双向神经网络模型预测的结果辅助VSA，结果也和DEEPVSA差不多。

## MAMADROID: Detecting Android Malware by Building Markov Chains of Behavioral Models (NDSS 17)

本文的工作是通过安卓软件的API调用序列，借助马尔可夫链求得转换概率，然后将概率作为输入训练一个机器学习模型，并在未来预测一个软件是不是恶意的。

本文的一个亮点是对API序列的抽象化。作者不直接处理API的名字，而是处理它们的package或者family name，同一package或family可能意味着相同的代码来源、相似的代码功能。这样，增强了训练出的模型的鲁棒性，使得它对未来的一些恶意软件仍然具有预测能力（作者强调自己的模型可以在一定程度上应对时间变化）。

**Weakness**

1. 对所有的API进行了统一的处理，可能有一些log类的api等，对判断是不是为恶意软件没有影响。如果增加一些区分可能获得更干净的数据集。
2. 根据马尔可夫求得的转换概率很难真实反映运行时的路径概率。比如从某一api到try块api的概率要明显高于到catch块的概率，但是本文是根据api出现的频次进行的计算。
3. 整个判断都基于API序列，这样是不够的。有可能恶意软件与非恶意软件的区别仅仅是一个参数不同。

## Authorship Analysis Studies: A Survey (International Journal of Computer Applications, 2014)

本文阐述了authorship analysis中的技术分类与发展。authorship analysis分为三个大类。authorship attribution：给定某人的一些已有文本，判断一篇新文本是否归属于该作者。authorship characterization：根据文本判定作者的社会属性，如性别、受教育程度等。similarity detection：判断一些文本是否归属同一作者。

**STYLOMETRIC 特征：**

词法的特征（lexical features）：根据不同的划分粒度有不同的属性，如按token划分时每个token的长度，句子的长度等。按character划分时字母计数、字母频率、数字计数、标点符号计数等。或者是考虑词汇的丰富程度、词汇出错的特点等。

句法的特征（syntactic features）：研究用来组成句子的凭借：标点和功能性词语（function words，如冠词、介词、代词等）。

结构的特征（structural features）：研究作者如何组织文档的结构，如文档段落之间的结构和段落内句子间的结构。

特定内容的特征（content-specific features）：研究文档主题相关的一些关键词汇。

![](http://image.hupeiwei.com/paper/authorship.PNG)

![](http://image.hupeiwei.com/paper/authorship1.PNG)

剩下的部分说了authorship detection tech，好像是在讲三个问题中的第一个，但是和之前命名不太统一。在解决该问题时，一种方法是把训练集中同一作者的所有文本拼接在一起（不过这样就忽略了这些文本间的差异），然后使用概率模型或压缩模型比对测试集和训练集中各作者文本的差异。还有一种方法是使用机器学习分类器、聚类算法、文本间距离等将训练集中同一作者多个文本划分成不同的集合，代表不同的风格，然后将测试集中文本进行归类。

## Accurate Malware Detection by Extreme Abstraction （ACSAC 18，CCF-B）

这篇文章研究了恶意软件检测，根本思路是不同于常规的尽量模拟软件的真实运行以规避反调试手段，TAMALES通过抽象系统，尽可能多的探索软件执行路径，但凡有一个是恶意的，不考虑可达性，这个软件就是恶意的。

文章中主要介绍了对于API的抽象，轻量级的符号执行和漏斗模型。API的抽象是指对DLL中API（大约10w个）的抽象，大部分直接返回随机值，7000个左右的关键API直接调用DLL中相应的机器码，还有1500个左右的API只实现关键功能。轻量级的符号执行不进行约束求解，遇到分支时直接两个路径都执行，并通过漏洞模型缓解路径爆炸问题（前两层不进行符号执行）。漏洞模型每层都有分类器，若达到恶意、良性的threshold直接判定，否则进入下一层。越往下层获得的执行路径越多，检测成本越高。

这种尽可能多获得执行路径，但凡有恶意路径软件就是恶意的思想不错，检测效果比较好。但是，数据集打标来自其它反病毒引擎，意味着变相躲避了对一些难检测软件的检测（它们本身就躲避了反病毒引擎）。并且，论文中只使用了漏洞模型的前两层做实验，没有涉及到轻量的符号执行，对其效率如何不可知。

