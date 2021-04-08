**Paper review draft**

[TOC]

# git-related vulnerability discover

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

