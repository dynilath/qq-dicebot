# DiceRobot kills you 3000

[![License](https://img.shields.io/github/license/dynilath/coolq-dicebot)](LICENSE)
[![Build Status](https://img.shields.io/appveyor/build/dynilath/coolqdicebot)](https://ci.appveyor.com/project/dynilath/coolqdicebot)  
[![dicebot开发测试](https://img.shields.io/badge/dicebot%E5%BC%80%E5%8F%91%E6%B5%8B%E8%AF%95-736910610-green.svg)](https://jq.qq.com/?_wv=1027&k=5g3t8RC)  
Just a simple dicebot for coolq in development.  
只是一个简单的酷Q骰子机器人。  
![image](/dicebot_test.gif)   
感谢@niunuinui 在文档制作上的帮助。

# 特性

* 强大的算式支持  
* 支持多行输入。  
* 使用系统随机器，如果系统随机器不可用，则改用mt19937随机器。  
* 为coc，wod，fate规则定制骰子。  
* 骰子指令支持算式，并且可以使用整数、小数、百分数。  
* 用户能够设置自定的骰子指令，以及保存一些变量。

# 安装

点击[下载cpk](https://github.com/dynilath/coolq-dicebot/releases/latest)前往下载本骰子机器人最新cpk。  
点击[获取酷Q](https://cqp.cc/)前往酷Q官网下载酷Q。    
点击[酷Q wiki](https://d.cqp.me/Pro/%E6%96%B0%E6%89%8B%E4%B8%8A%E8%B7%AF)前往酷Qwiki了解如何使用酷Q以及插件如何安装。  
部署时你可能会遇到酷Q提示126(-103)错误，这很可能是因为运行环境没有`Visual C++ Redistributable`，你可以前往[这里](https://www.microsoft.com/zh-CN/download/details.aspx?id=48145)下载。
  
# 实例  

QQ：1701687847  
它能够自动接受好友请求和加群邀请。请温柔对待它。  

# 目录

<!-- TOC -->

- [DiceRobot kills you 3000](#dicerobot-kills-you-3000)
- [特性](#特性)
- [安装](#安装)
- [实例](#实例)
- [目录](#目录)
- [构建项目](#构建项目)
    - [依赖概述](#依赖概述)
    - [构建](#构建)
    - [使用bootstrap.ps1](#使用bootstrapps1)
    - [运行测试](#运行测试)
- [使用示范](#使用示范)
    - [help](#help)
    - [基本骰子指令(`.roll`或`.r`)](#基本骰子指令roll或r)
        - [骰子基础](#骰子基础)
        - [骰子机器人回复格式](#骰子机器人回复格式)
        - [双/三/四骰取高/低](#双三四骰取高低)
        - [多行输入](#多行输入)
        - [`#`标记和多次骰](#标记和多次骰)
        - [骰子与算式](#骰子与算式)
        - [开启/关闭详细结果(`.roll source on/off`)](#开启关闭详细结果roll-source-onoff)
    - [骰子专用的昵称(`.name`或`.n`)](#骰子专用的昵称name或n)
        - [不回复地更改昵称(.name silence或.ns)](#不回复地更改昵称name-silence或ns)
    - [定义骰子(.set或.s)](#定义骰子set或s)
        - [默认骰子](#默认骰子)
    - [展示骰子(`.list`或`.l`)](#展示骰子list或l)
        - [全部展示(`.list`)](#全部展示list)
        - [展示部分骰子(`.list [筛选字段]`)](#展示部分骰子list-筛选字段)
    - [删除骰子(`.delete`)](#删除骰子delete)
        - [全部删除(`.delete`)](#全部删除delete)
        - [删除骰子(`.delete [名称]`)](#删除骰子delete-名称)
    - [为特定规则定制的骰子](#为特定规则定制的骰子)
        - [coc定制骰子(`.coc`或者`.c`)](#coc定制骰子coc或者c)
        - [wod定制骰子(`.wod`或者`.w`)](#wod定制骰子wod或者w)
        - [FATE定制骰子(`.fate`或`.f`)](#fate定制骰子fate或f)
    - [扑克(`.poker`或`.p`)](#扑克poker或p)
        - [抽牌(`.poker draw`或`.pd`)](#抽牌poker-draw或pd)
        - [洗牌(`.poker shuffle`)](#洗牌poker-shuffle)
        - [使用特殊牌](#使用特殊牌)
    - [手动骰子(`.manual`或`.m`)](#手动骰子manual或m)
        - [产生手动骰子(`.m`)](#产生手动骰子m)
        - [骰指定骰子(`.mr`)](#骰指定骰子mr)
        - [消灭指定骰子(`.mk`)](#消灭指定骰子mk)
        - [增加骰子(`.mk`)](#增加骰子mk)
        - [清空手动骰子(`.mka`)](#清空手动骰子mka)

<!-- /TOC -->

# 构建项目

## 依赖概述
本项目使用`sqlite3`作为数据库，使用`Google Test`框架构建测试。使用到的`cqcppsdk`需要windows sdk作为依赖条件，如前文所述，这也使得该插件需要`Visual C++ Redistributable`来运行。在非`win`的环境下仍然可以编译得到测试文件和`app_dev`，但无法得到插件部署所需的`dll`。

## 构建
了解`cmake`的开发者可以看看[CMakeLists.txt](/CMakeLists.txt)，准备好依赖后直接通过`cmake`构建。这里就不多做赘述了。

## 使用bootstrap.ps1
不太明白的开发者可以通过运行`bootstrap.ps1`来快捷构建。这是一个`powershell`脚本，如果你的电脑没有`powershell`请去安装一个，这里不做赘述。  
该脚本只是把准备依赖和操作`cmake`的过程都写进了脚本里，所以这仍然需要你先去安装一个[CMake](https://cmake.org/)。除此之外，你需要先去折腾一个[VCPKG](https://github.com/Microsoft/vcpkg/)，并将`vcpkg`的根目录路径设置到系统变量`VCPKG_ROOT`中。然后你运行该脚本，看依赖装完，看编译走完，完毕。  
除此之外，`bootstrap.ps1`可以接受编译配置作为参数，例如

> .\bootstrap.ps1 Debug

以此来编译`Debug`配置的本项目。

## 运行测试

本项目的单元测试项目包括

* dicebot.gtest.roll_test
* dicebot.gtest.entry_test
* dicebot.gtest.number_test
* dicebot.gtest.data_test

你可以直接运行对应的可执行文件来测试，如果使用的是`GNU`工具组也可以直接`make test`来测试（当然那个时候无法产生插件所需的`dll`文件）。  
*注：这里的`roll_test`使用到卡方检验，虽然通过概率非常非常大但仍然有1‰概率无法通过。*

# 使用示范

## help

发送`.help`获取没啥用的帮助信息。
> dynilath：`.help`

> DiceBot：

> ``` 
> DiceBot by dynilath v1.7.0
> 使用方式简介：
> .r 1d8+1d6+3 普通骰子
> .c coc骰子
> .wo4 owod骰子
> .wn4 nwod骰子
> .f FATE骰子
> 更多指令：name set list delete manual poker
> 使用 .help [指令名称] 来获得对应指令的帮助
>  
> 访问项目主页http://github.com/dynilath/coolq-dicebot
> 获得使用说明以及了解更多内容。
> 如果在使用中遇到了bug，或者有什么功能建议，欢迎在项目主页通过issue反馈，感谢您的帮助。
> ```

你可以通过`.help [指令名称]`获得具体指令的帮助。例如：
> dynilath：`.help roll`  

> DiceBot：
> ```
> 骰子指令(.roll或.r)
> 指令.r 1d20：骰一个1d20
> 指令.r 1d20+2d6+4：骰1d20+2d6+4，并输出其计算结果
> 指令.r (10d6+10)*150%：骰(10d6+10)*150%，并输出其计算结果
> 指令.r 4d6k3：骰4d6，取其中较高的三个结果
> 指令.r 2d20kl1：骰2d20，取其中较低的一个结果
> 指令.r 6#4d6kl3：骰6次4d6kl3，将它们的结果分别显示
> 指令.r {1d20,1d12,1d10,1d8,1d6}：分别骰这些骰子，结果分别显示
> 指令.r 2#d20-{1,2}：骰两次d20,结果分别减去1和2，结果分别显示
> 指令.roll source off：停止显示你的骰子的详细结果
> 指令.rs off：上述指令的较短形式
> 指令.roll source on：恢复显示你的骰子的详细结果
> 指令.rs on：上述指令的较短形式
> 指令.rs 3d6：骰3d6，无视rs off状态，仍然显示详细结果
> ```

## 基本骰子指令(`.roll`或`.r`)

### 骰子基础

让我们先骰一个d20试试。  
> dynilath：`.rd20`  
> DiceBot：` * dynilath 掷骰: d20 = 20`

`dynilath`是本骰子机器人开发者，后续会一直使用它作为示例。  
你只需要在开头写上`.r`再加上骰子（例如`d20`、`4d6`），发送该消息，就可以骰出这些骰子了。  
骰子的基本格式为`XdY`，`X`表示骰子个数，`Y`表示骰子面数。当然，你在TRPG规则书里肯定已经看过无数次了。  

这个结构也可以用下面的文字表示：  
` . r [骰子个数]d[骰子面数]`  
发送这个消息，骰子机器人就能识别你的消息并回复一个骰子结果。  

在骰子指令后面加上一些文本，可以作为描述添加到骰子结果中。  

> dynilath：`.rd20攻击`  
> DiceBot：` * dynilath 攻击 掷骰: d20 = 6`

有时候你会需要骰几种不同的骰子，可以发送如下信息。  

> dynilath：`.rd20+d6+2d4`
> DiceBot：` * dynilath 掷骰: d20 + d6 + 2d4 = [12] + [4] + [4 + 4] = 24`

### 骰子机器人回复格式

骰子机器人发送的信息组成结构如下：  
` * [你的QQ昵称/群名片] [骰子描述] 掷骰: [骰子指令] = [骰子详细] = [结果]`  
群名片就是你使用骰子所在的群中你的群名片，如果是讨论组则使用昵称。  
骰子描述即你输入的骰子（例如`d20`、`4d6`）。  
结果即最终计算得到的数值。  
需要说明的是，`4d6k3`或者`2d6`这样的组视为一个完整的“骰子标志”，它们在输出的时候会被`[   ]`包括起来，例如：  

> dynilath：`.r 4d6`  
> DiceBot：` * dynilath 掷骰: 4d6 = [4 + 4 + 2 + 6] = 16`  

### 双/三/四骰取高/低

你可以在骰子指令后加上`k[数值]`或者`kl[数值]`来只取部分结果。  
其中`k[数值]`表示保留较高结果，`kl[数值]`表示较低结果。  
下面的例子是骰4d6并保留3个较高的结果（即扔掉较低的那个）。  

> dynilath：`.r4d6k3`  
> DiceBot：` * dynilath 掷骰: 4d6k3 = [5 + 3 + 5 + 1*] = 13`  

被忽略的数值会在其后面用`*`号标记，它不会算入到总和中。  

### 多行输入

骰子机器人支持一次输入多行，如下所示：  

> dynilath：
> ```
> .r 1d20 + 1d6-3+4+11 破邪斩+猛力攻击
> .r 5d4 * 150%
> ```

> DiceBot：
> ```
> * dynilath 破邪斩+猛力攻击 掷骰: 1d20 + 1d6 - 3 + 4 + 11 = [13] + [4] - 3 + 4 + 11 = 29
> * dynilath 掷骰: 5d4 * 150% = [4 + 3 + 1 + 3 + 1] * 150% = 18
> ```

输出的顺序完全按照你的输入顺序，并且如果存在不符合规范的行，能够单独忽略。  

> dynilath：
> ```
> .r4d6k3 力量
> 这行是来捣乱的
> .r4d6k3 敏捷
> ```

> DiceBot：
> ```
> * dynilath 力量 掷骰: 4d6k3 = [6 + 1* + 6 + 6] = 18
> * dynilath 敏捷 掷骰: 4d6k3 = [3 + 5 + 2 + 2*] = 10
> ```  

### `#`标记和多次骰

使用`#`标记，或者使用`{   }`来重复数次独立掷骰。先来看个例子：  

> dynilath：`.r 6#4d6k3`     
> DiceBot：` * dynilath 掷骰: 6#4d6k3 = {[4 + 2 + 4 + 1*], [2 + 4 + 5 + 1*], [5 + 5 + 1* + 3], [1* + 2 + 2 + 3], [2 + 4 + 1* + 3], [5 + 2* + 3 + 3]} = {10, 11, 13, 7, 9, 11}`

使用`#`来表示重复的骰子不会被加起来，而是单独列出表示。你也可以用`{   }`实现类似的效果，如下所示：

> dynilath：`.r {4d6,3d6,2d6,1d6}`     
> DiceBot：` * dynilath 掷骰: {4d6, 3d6, 2d6, 1d6} = {[5 + 3 + 3 + 5], [6 + 4 + 2], [1 + 6], [5]} = {16, 12, 7, 5}`

### 骰子与算式    

本骰子机器人具有较强的算式支持。  

> dynilath：`.r(((4d6+3)/2+2d20)+4*1d6)*150%`     
> DiceBot：` * dynilath 掷骰: (((4d6 + 3) / 2 + 2d20) + 4 * 1d6) * 1.5 = ((([1 + 3 + 2 + 3] + 3) / 2 + [20 + 14]) + 4 * [3]) * 1.5 = 78`  

如果你的算式输入格式有问题，多余的部分会自动被识别成骰子描述。  

> dynilath：`.r4d6+(((4d6+3)/2+2d20)+4*1d6`     
> DiceBot：` * dynilath +(((4d6+3)/2+2d20)+4*1d6 掷骰: 4d6 = [5 + 1 + 4 + 3] = 13`  

* 注意：骰子数过多、数值不合法等问题在算式计算中才会发现，所以无法被识别为骰子描述。

除此之外，当`#`和`{  }`产生的多个独立结果用于算式时，这些结果会按顺序分别计算，下面是一个例子：

> dynilath：`.r 4#d20-{1,2,3,4}`  
> DiceBot：` * dynilath 掷骰: 4#d20 - {1, 2, 3, 4} = {[17], [20], [14], [8]} - {1, 2, 3, 4} = {16, 18, 11, 4}`

常规的骰子（例如`4d6`、`1d20`）在和多次独立骰结构进行计算时，只会骰一次，并在各个独立计算中沿用相同结果，下面是一个例子：

> dynilath：`.r d20-{1,2,3,4}`  
> DiceBot：` * dynilath 掷骰: {d20} - {1, 2, 3, 4} = {[19]} - {1, 2, 3, 4} = {18, 17, 16, 15}`

### 开启/关闭详细结果(`.roll source on/off`)

你可以使用`.roll source off`来关闭具体每个骰子结果为多少的输出。    
你也可以使用它较短的形式`.rs off`。   

> dynilath：`.roll source off`      
> DiceBot：` * dynilath 关闭骰子详细输出`  

在这之后，你的骰子指令不会再输出详细每个骰子的结果，只会给出一个最终结果。  

> dynilath：`.r(((4d6+3)/2+2d20)+4*1d6)*150%`   
> DiceBot：` * dynilath 掷骰: (((4d6 + 3) / 2 + 2d20) + 4 * 1d6) * 150% = 90`  

你可以使用`.roll source on`来重新开启详细输出。     
除此之外，即使在`.roll source off`状态下，你也可以使用`.roll source`替换本来的`.r`，此时仍然会产生详细输出。  
如果你觉得`.roll source`太长，你也可以使用较短的形式`.rs`。  

> dynilath：`.rsoff`   
> DiceBot：` * dynilath 关闭骰子详细输出`  
> dynilath：`.r4d6`   
> DiceBot：` * dynilath 掷骰: 4d6 = 16`  
> dynilath：`.rs4d6`   
> DiceBot：` * dynilath 掷骰: 4d6 = [3 + 1 + 6 + 5] = 15`
 
## 骰子专用的昵称(`.name`或`.n`)

使用`.name`指令来指定一个仅在骰子机器人的回复文本中使用的名字。  
你也可以使用它较短的形式`.n`。    
在指令后跟随的文本会被保存起来，并代替之前所述的`[你的QQ昵称/群名片]`部分  
> dynilath：`.n菜鸟PC`      
> DiceBot：` * dynilath 的新名字是 菜鸟PC`  

在这之后使用骰子指令时，昵称部分会使用这个名字。  
> dynilath：`.r4d6k3 力量`      
> DiceBot：` * 菜鸟PC 力量 掷骰: 4d6k3 = [5 + 4 + 1* + 4] = 13`  

这个昵称在每个群/讨论组之间是独立的，在A群的设置不会影响在B群的状态。  
昵称保存在数据库中，关闭开启不会取消已经设置的昵称。  
如果需要维护这部分内容，骰子机器人使用`sqlite3`数据库。在插件目录下可以找到`.db`文件。  

### 不回复地更改昵称(.name silence或.ns)
除此之外，你也可以使用`.ns`，完整指令为`.name silence`来更改昵称，这个时候骰子机器人不会回复消息。  
> dynilath：
> ```
> .ns 迷诱魔
> .r 1d20+20
> .ns 反pal魅魔
> .r 1d20+24破善斩
> ```

> DiceBot：
> ```
> * 迷诱魔 掷骰: 1d20 + 20 = [7] + 20 = 27
> * 反pal魅魔 破善斩 掷骰: 1d20 + 24 = [9] + 24 = 33
> ```

当然，这会令`.n silence`失效，你无法这样把自己的名字改成`silence`，如果你一定要把自己的昵称弄成`silence`，请使用`.name silence silence`。

## 定义骰子(.set或.s)
使用`.set`指令来设定一个骰子。  
你也可以使用它较短的形式`.s`。    

> dynilath：`.s 1d20+14 猛力攻击`   
> DiceBot：` * dynilath 设置指令: (1d20 + 14) 为 猛力攻击`  

在你已经设置了一个骰子之后，你可以直接使用`.r[指令名称]`来使用该骰子。  

> dynilath：`.r 猛力攻击`   
> DiceBot：` * dynilath 掷骰: (1d20 + 14) = ([3] + 14) = 17`  

你可以可以添加一些描述，但是注意使用空格分隔。一些情况下不分隔也是可以的。  

> dynilath：`.r 猛力攻击 他死定了`  
> DiceBot：` * dynilath 他死定了 掷骰: (1d20 + 14) = ([20] + 14) = 34` 

骰子指令可以结合在算式中使用

> dynilath：`.r 猛力攻击-2 恶心状态`  
> DiceBot：` * dynilath 恶心状态 掷骰: (1d20 + 14) - 2 = ([11] + 14) - 2 = 23`  

> dynilath：`.set 1d12+22 猛力攻击伤害`  
> DiceBot：` * dynilath 设置指令: (1d12 + 22) 为 猛力攻击伤害`  

> dynilath：`.r {猛力攻击,猛力攻击伤害}`  
> DiceBot：` * dynilath 掷骰: {(1d20 + 14), (1d12 + 22)} = {([5] + 14), ([1] + 22)} = {19, 23}`

* 注意：骰子指令名称不能使用`+-*/{},()#=<>&|:%.`和空格。
* 注意2：骰子指令中不能使用`#`和`{ }`产生独立结果，但下面的默认骰子可以。

### 默认骰子

如果你在定义骰子时，没有指定骰子名称，则会设置一个默认骰子。

> dynilath：`.s 4d6+3`  
> DiceBot：` * dynilath 设置默认骰子指令: 4d6 + 3`  

如果你已经设置了默认骰子，使用`.r`就可以使用默认骰。    

> dynilath：`.r`    
> DiceBot：` * dynilath 掷骰: 4d6 + 3 = [4 + 4 + 5 + 3] + 3 = 19`  

如果你写了一些没有和其他骰子定义重叠的描述，也会正常识别成描述。 

> dynilath：`.r blahblahblah`   
> DiceBot：` * dynilath blahblahblah 掷骰: 4d6 + 3 = [1 + 6 + 2 + 2] + 3 = 14` 

## 展示骰子(`.list`或`.l`)
### 全部展示(`.list`)
使用`.list`指令来显示你所保存的所有数据，你也可以使用较短形式`.l`。  

> dynilath：`.l`

> DiceBot：
> ``` 
> * dynilath 的个人信息如下: 
> 默认骰子: 
> * 默认 : (4d6)
> > test : (4d6)
> > test1 : (4d6)
> > 猛力攻击 : (1d20 + 14)
> ```  

### 展示部分骰子(`.list [筛选字段]`)
使用`.list [筛选字段]`指令来只显示你所保存的部分骰子。    
 
> dynilath：`.l猛力`

> DiceBot：
> ``` 
> * dynilath 已设置如下包含 "猛力" 的骰子指令:
> > 猛力攻击 : (1d20 + 14)
> ```  

## 删除骰子(`.delete`)
### 全部删除(`.delete`)
使用`.delete`指令来显示你所保存的骰子。  
另外，这不会删除你的默认骰子。  

> dynilath：`.delete`   
> DiceBot：` * dynilath 已删除所有骰子骰子指令`

### 删除骰子(`.delete [名称]`)
使用`.delete [名称]`指令来删除你所保存的特定骰子。  

> dynilath：`.dr 属性`  
> DiceBot：` * dynilath 已删除骰子指令: 属性`   

## 为特定规则定制的骰子

### coc定制骰子(`.coc`或者`.c`)
骰子机器人提供了coc定制的骰子。使用指令为`.coc`。  
你也可以使用它较短的形式`.c`。    
> dynilath：`.c图书馆利用`      
> DiceBot：` * dynilath 图书馆利用 掷骰: d100 = 11`  

除此之外，骰子机器人内置了coc7版的奖惩骰。 
使用`b[数值]`来产生奖励骰。
> dynilath：`.cb2斗殴(65)`  
> DiceBot：` * dynilath 斗殴(65) 掷骰: CoC b2 = [2 + 8* + 3*] [8] = 28`  

使用`p[数值]`来产生惩罚骰。  
> dynilath：`.cp2闪避(50)`  
> DiceBot：` * dynilath 闪避(50) 掷骰: CoC p2 = [6 + 1* + 6*] [9] = 69` 

可以自动计算奖惩相抵。  
> dynilath：`.cp5b5p2b3奖罚抵消`    
> DiceBot：` * dynilath 奖罚抵消 掷骰: CoC b1 = [4 + 9*] [0] = 40`  

如果提供一个技能数值，会根据coc7规则计算成功等级。  
> dynilath：`.c55p1闪避`    
> DiceBot：` * dynilath 闪避 掷骰: CoC (55) p1 = [2* + 5] [1] = 51 success`  

### wod定制骰子(`.wod`或者`.w`)
骰子机器人提供了wod定制的骰子。使用指令为`.wodn[骰子数量]`和`.wodo[骰子数量]`。  
分别对应nWoD和oWod。  
你也可以使用它们较短的形式`.wn[骰子数量]`和`.wo[骰子数量]`。    

nWoD规则下，默认困难度为8，出10会加骰。

> dynilath：`.wn4`  
> DiceBot：` * dynilath 掷骰: nWoD = [1* + 8 + 5* + 7*] = 1`  

oWoD规则下，默认困难度为6，出1会扣除一个成功。  

> dynilath：`.wo4`  
> DiceBot：` * dynilath 掷骰: oWoD = [10 + 10 + 4* + 6] = 3`  

当然，不会扣成负数。  

> dynilath：`.wo4`  
> DiceBot：` * dynilath 掷骰: oWoD = [3* + 10 + 2* + 9] = 2`  

无论是nWoD还是oWod，都可以加上`d[数值]`以更改困难度。  

> dynilath：`.wo4d8`  
> DiceBot：` * dynilath 掷骰: oWoD D8 = [3* + 7* + 3* + 8] = 1`  

无论是nWoD还是oWod，都可以加上`b[数值]`以设置奖励骰阈值。这里为了避免错误输入产生过长的结果，当阈值小于6时，取10。    

> dynilath：`.wo4b6`  
> DiceBot：` * dynilath 掷骰: oWoD B6 = [9 + 4* + 5* + 2* + 5*] = 1`  

### FATE定制骰子(`.fate`或`.f`)

骰子机器人提供了fate定制的骰子。使用指令为`.fate`。    
你也可以使用它较短的形式`.f`。    

它在结果上等效于`.r4d3-8`，但显示`+-o`肯定会更为合适。

> dynilath：`.f`  
> DiceBot：` * dynilath 掷骰: FATE = [+ + - o] = 1`  

也可以加上调整值

> dynilath：`.f+4`  
> DiceBot：` * dynilath 掷骰: FATE +4 = [o - o +] + 4 = 4`  

## 扑克(`.poker`或`.p`)

没错这竟然有扑克功能！  
该功能会为群组创建一套成员都可以操作的扑克牌。（当然被禁言的成员没办法操作。）  
通过`.poker init [牌组定义]`来创建一套牌。

> dynilath：`.poker init standard`  
> DiceBot：` * dynilath 已初始化牌库，总计54张牌`

`standard`表示完整的一套54张牌，你可以用`core`来表示只含四种花色（不包含joker）的牌。或者使用`diamondQ`、`heartA`等来描述特定牌。  

### 抽牌(`.poker draw`或`.pd`)

使用`.poker draw`从已经初始化的牌堆上抽一张牌。  

> dynilath：`.p init core,8joker`  
> DiceBot：` * dynilath 已初始化牌库，总计60张牌`  

> dynilath：`.poker draw`  
> DiceBot：` * dynilath 抽出了 Joker | 牌堆剩余59张，已经抽出了: Joker`  

> dynilath：`.pd`  
> DiceBot：` * dynilath 抽出了 ♣2 | 牌堆剩余58张，已经抽出了: Joker, ♣2`

如果牌库抽光了，会给出提示。  

> dynilath：`.p init joker`  
> DiceBot：` * dynilath 已初始化牌库，总计1张牌`  

> dynilath：`.pd`  
> DiceBot：` * dynilath 抽出了Joker | 牌堆剩余0张，已经抽出了: Joker`    

> dynilath：`.pd`  
> DiceBot：` * dynilath 无牌可抽 | 牌堆剩余0张，已经抽出了: Joker`

在`.pd`后增加数字，来指定一次抽牌的数量。

> dynilath：`.p init core`  
> DiceBot：` * dynilath 已初始化牌库，总计52张牌`  
> dynilath：`.pd2`  
> DiceBot：` * dynilath 抽出了 ♡7, ♢5 | 牌堆剩余50张，已经抽出了: ♡7, ♢5`  

此处，`.rsoff`也能用来关闭已抽出记录的输出。  

> dynilath：`.rsoff`  
> DiceBot：` * dynilath 关闭骰子详细输出` 
> dynilath：`.pd2`  
> DiceBot：` * dynilath 抽出了 ♢8, ♣4 | 牌堆剩余48张` 


### 洗牌(`.poker shuffle`)

使用`.poker shuffle`将牌库重新洗牌。 
这个指令可以缩写成`.p shuffle`，但是`shuffle`就不能再缩写了。  

> dynilath：`.pinit core`  
> DiceBot：` * dynilath 已初始化牌库，总计52张牌`  

> dynilath：`.pd`  
> DiceBot：` * dynilath 抽出了Heart A | 牌堆剩余51张，已经抽出了: Heart A`  

> dynilath：`.pshuffle`  
> DiceBot：` * dynilath 已将牌堆重新切洗`  

> dynilath：`.pd`  
> DiceBot：` * dynilath 抽出了Heart 4 | 牌堆剩余51张，已经抽出了: Heart 4`

### 使用特殊牌

如果在初始化时提供的卡牌名称不在标准扑克之列，就会添加一些具有特殊名称的牌进去。  

> dynilath：`.pinit 战士,法师,牧师,德,武僧,蛮子,术士,圣爹`  
> DiceBot：` * dynilath 已初始化牌库，总计8张牌`  

> dynilath：`.pd`  
> DiceBot：` * dynilath 抽出了圣爹 | 牌堆剩余7张，已经抽出了: 圣爹`  

> dynilath：`.pd`  
> DiceBot：` * dynilath 抽出了武僧 | 牌堆剩余6张，已经抽出了: 圣爹, 武僧`

这里支持使用各种各样的分隔符号，包括`,\/&;，。、；`。但请注意，只有第一个遇到的上述符号会作为分隔符，其他分隔符会被忽略。

## 手动骰子(`.manual`或`.m`)

手动骰子会产生一些会保存在数据库的骰子数据。  
你可以使用指令操作这些骰子。如同它们真的在那里。
可以使用的指令如下：

### 产生手动骰子(`.m`)

产生手动骰子，后接用`+`连接的`XdY`形式的骰子。由于这个表达式只是表示一些骰子，所以这里不支持其他算式。  

> dynilath：`.m4d6+2d8`  
> DiceBot：` * dynilath 在桌上放了这些骰子: 4d6+2d8 | 当前状态: 1(6) + 5(6) + 1(6) + 1(6) + 8(8) + 1(8) = 17`  

### 骰指定骰子(`.mr`)

骰指定骰子，后接骰子序号。

> dynilath：`.mr4`  
> DiceBot：` * dynilath 重骰桌上的第 4 个骰子 | 当前状态: 1(6) + 5(6) + 1(6) + 6(6) + 8(8) + 1(8) = 22`  


###  消灭指定骰子(`.mk`)

消灭指定骰子，后接骰子序号。

> dynilath：`.mk5`  
> DiceBot：` * dynilath 杀死桌上的第 5 个骰子 | 当前状态: 1(6) + 5(6) + 1(6) + 6(6) + 1(8) = 14`  

###  增加骰子(`.mk`)

增加骰子，如同初始化一般，但是不会清除原有的骰子。

> dynilath：`.ma2d6`  
> DiceBot：` * dynilath 在桌上增加了这些骰子: 2d6 | 当前状态: 1(6) + 5(6) + 1(6) + 6(6) + 1(8) + 4(6) + 5(6) = 23`  

###  清空手动骰子(`.mka`)

> dynilath：`.mka`  
> DiceBot：` * dynilath 杀掉了所有的骰子 | 当前状态: 没有骰子了`  

