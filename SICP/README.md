# SICP note

[视频教程](https://github.com/DeathKing/Learning-SICP)   
[SICP book online](https://sarabander.github.io/sicp/html/index.xhtml)   

-----

- [第一章 构造抽象过程](#1)
  - [程序设计的基本元素](#1.1)
  - [过程与它们所产生的计算](#1.2)
  - [用高阶函数做抽象](#1.3)
- [第二章 构造数据抽象](#2)
  - [数据抽象导引](#2.1)
  - [层次性数据和闭包性质](#2.2)
  - [符号数据](#2.3)
  - [抽象数据的多重表示](#2.4)
  - [带有通用型操作的系统](#2.5)
- [第三章 模块化、对象和状态](#3)
  - [](#3.1)
  - [](#3.2)
  - [](#3.3)
  - [](#3.4)
  - [](#3.5)
- [第四章 元语言抽象](#4)
  - [](#4.1)
  - [](#4.2)
  - [](#4.3)
  - [](#4.4)
- [第五章 寄存器机器里的计算](#5)
  - [](#5.1)
  - [](#5.2)
  - [](#5.3)
  - [](#5.4)
  - [](#5.5)


&nbsp;   
&nbsp;   
<a id="1"></a>
# 第一章 构造抽象过程

&nbsp;   
<a id="1.1"></a>
## 1.1 程序设计的基本元素

- **表达式**：前缀形式
- **组合**：
- **抽象**：命名，使用名称去引用一个运算结果。（这意味着 interpreter 必须维护某种存储能力，被称为 **环境**）

过程定义：  
`(define (<name> <formal parameters>) <body>)`  

代换模型：将过程调用替换为函数体。   

**应用序** 和 **正则序**：

- 应用序：先求值参数而后应用。
- 正则序：完全展开之后规约。

>Lisp 采用应用序求值，部分原因在于这样做能避免对于表达式的重复求值，从而可以提高一些效率。更重要的是，在超出了可以采用代换方式模拟的过程范围后，正则序的处理将变得复杂得多。

&nbsp;   

**条件表达式** 和 **谓词**：

    (cond (<p1> <e1>)
          (<p2> <e2>)
          ..
          (<pn> <en>))
**条件表达式的求值方式**如下：   
首先求值谓词`<p1>`，如果它的值是`false`，那么继续向下求值，知道发现了某个谓词的值为`true`，返回相应的序列表达式`<e>`的值，以这个值作为条件表达式的值。如果没有，则没有定义。最后可以出现 `else` ，行为如同 default.  

>注：`<e>`可以是一个表达式的序列，如果对应的`<p>`为真，那么`<e>`会被顺序求值，并将最后一个表达式的值作为返回值。

&nbsp;   

特殊形式：`if`   
`if <predicate> <consequent> <alternative>`   
**在求值一个`if`表达式时**，解释器从求值其`<predicate>`部分开始，如果`<predicate>`得到真值，解释器就去求值`<consequent>`并返回其值，否则就去求值`<alternative>`并返回其值。
   

>注：`if` 是一种特殊的形式，`<consequent>`和`<alternative>`只有一个被求值。   
>而在练习1.6提出的问题中，`new-if`被定义如下：   
>(define (new-if predicate then-clause else-clause)  
&emsp;&emsp;(cond (predicate then-clause)  
&emsp;&emsp;&emsp;&emsp;&emsp;(else else-clause)))   
>`new-if`会对两个子表达式都进行求值。

&nbsp;   

其他复合运算：   
`(and <e1> ... <en>)`   
`(or <e1> ... <en>)`   
`(not <e>)`

注意`and`和`or`都是特殊形式而不是普通的过程，因为它们的子表达式不一定都求值。而`not`则是一个普通的过程。

&nbsp;   

**黑箱抽象**：

- **过程抽象**：只关心功能与接口格式。分解中的每一个过程完成了一件可以清楚标明的工作，这使它们可以用作定义其他过程的模块。
- **形参局部名**：过程的形式参数应该是相应过程体里的局部名字。
- **内部定义** 和 **块结构**：核心过程中依赖的其他过程应该被内部化，否则会污染其他库实现。（即这些辅助函数不够全局）

如果在一个完整的过程里面**约束变量**全部换名，那么过程的意义将没有任何改变。   
如果一个变量是不被约束的，我们称它为**自由**的。  
**作用域**：被声明为过程的形参的约束变量，就以这个过程的体(`body`)作为它们的作用域。

内部定义与块结构：  

    #lang sicp
    (define (square x) (* x x) )
    (define (average x y) (/ (+ x y) 2))
    
    (define  (sqrt x)
      (define (good-enough? guess x)
          (< (abs (- (square guess) x)) 0.001)
      )
      (define (improve guess x)
          (average guess (/ x guess))
      )
      (define (sqrt-iter guess x)
          (if (good-enough? guess x)
          guess
          (sqrt-iter (improve guess x) x))
      )
     (sqrt-iter 1.0 x)
    )


使用块结构的好处：

- 将辅助过程定义到内部
- 不用显式的传递某些参数，可以让这些参数作为内部定义中的自由变量（词法作用域）



&nbsp;   
<a id="1.2"></a>
## 1.2 过程与它们所产生的计算

- 递归计算过程：计算过程构造一个 **推迟进行的操作** 所形成的链条，收缩阶段表现为这些运算的实际执行。（解释器需要维护好那些以后将要执行的操作的轨迹）
- 迭代计算过程：那种其状态可以用固定数目的状态变量描述的计算过程；而与此同时，又存在着一套固定的规则，描述了计算过程在从一个状态到达下一个状态转换时，这些变量的更新方式；还有一个（可能有的）结束检测，它描述这一计算过程应该终止的条件。

对于**树形递归**，计算中的每一点，我们都只需保存树中在此之上的结点的轨迹。一般来说，树形递归的计算过程所需的步骤将正比于树中的结点树，其空间需求正比于树的最大深度。





&nbsp;   
<a id="1.3"></a>
## 1.3 用高阶函数做抽象

强大语言的需求：为公共的模式命名，建立抽象，而后直接在抽象的层次上工作。

**高阶过程**：这样的过程，让他们以过程为参数，或者以过程为返回值。

使用 **lambda** 构造匿名过程：   
`(lambda (<formal-parameters>) <body>)`
定义过程的另一种写法：  

    (define foo
        (lambda (x) 
          return-expr))

用 **let** 创建局部变量：  

    (let ((<var1><expr1>)
          (<var2><expr2>)
          ...
          (<varn><exprn>))
     <body>)

当 let 被求值时，这里的每个名字将被关联到对应表达式的值。在将这些名字约束为局部变量的情况下求值 let 体。可以认为 let 表达式描述的变量的作用域就是该 let 的体。  

- let 使人能在尽可能接近其使用的地方建立局部变量约束。
- 变量的值是在 let 之外计算的。

**抽象**和**第一级过程**：高阶过程的重要性，就在于使我们能显式地使用程序设计语言的要素去描述这些抽象，使我们能够像操作其他计算元素一样去操作它们。   

一般而言，程序设计语言总会对计算元素的可能使用方式强加上某些限制。带有最少限制的元素被称为具有*第一级*的状态。第一级元素的某些“权利或者特权”包括：

- 可以用变量命名
- 可以提供给过程做参数
- 可以由过程作为结果返回
- 可以包含在数据结构中

Lisp 给了过程完全的第一级状态，由此获得的描述能力是及其惊人的。

>实现第一级过程的主要代价是：为使过程能够作为值返回，我们就需要为过程里的自由变量保存空间，即使这一过程并不执行。



&nbsp;   
&nbsp;   
<a id="2"></a>
# 第二章 构造数据抽象

将对象数据复合起来，形成*复合数据*的方式。([AOS & SOA](https://en.wikipedia.org/wiki/AOS_and_SOA))   
提高设计的模块性，增强语言的表达能力。   


&nbsp;   
<a id="2.1"></a>
## 2.1 数据抽象导引




&nbsp;   
<a id="2.2"></a>
## 2.2 层次性数据和闭包性质





&nbsp;   
<a id="2.3"></a>
## 2.3 符号数据





&nbsp;   
<a id="2.4"></a>
## 2.4 抽象数据的多重表示





&nbsp;   
<a id="2.5"></a>
## 2.5 带有通用型操作的系统




&nbsp;   
&nbsp;   
<a id="3"></a>
# 第三章 模块化、对象和状态

&nbsp;   
<a id="3.1"></a>


&nbsp;   
<a id="3.2"></a>


&nbsp;   
<a id="3.3"></a>


&nbsp;   
<a id="3.4"></a>


&nbsp;   
<a id="3.5"></a>



&nbsp;   
&nbsp;   
<a id="4"></a>


&nbsp;   
<a id="4.1"></a>



&nbsp;   
<a id="4.2"></a>


&nbsp;   
<a id="4.3"></a>


&nbsp;   
<a id="4.4"></a>




&nbsp;   
&nbsp;   
<a id="5"></a>




&nbsp;   
<a id="5.1"></a>


&nbsp;   
<a id="5.2"></a>


&nbsp;   
<a id="5.3"></a>


&nbsp;   
<a id="5.4"></a>

