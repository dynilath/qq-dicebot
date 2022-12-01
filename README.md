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
* 支持多行输入  
* 使用`xoroshiro128**`算法产生随机数
* 为coc，wod，fate规则定制骰子  
* 骰子指令支持算式，并且可以使用整数、小数、百分数  
* 支持`#`多骰，并且支持更进一步的分别计算
* 用户能够设置自定的骰子指令，以及保存一些变量

# 安装

请部署在`windows`环境，安装`nodejs`。`nodejs`版本至少为`16.14.2`，且必须是64位版本。

1. 克隆本仓库，找到你希望部署的位置，打开控制台，输入`git clone https://github.com/dynilath/qq-dicebot.git`
2. 在控制台输入`cd qq-dicebot`，进入仓库目录。
3. 在控制台输入`npm install`，等待依赖安装完成。
4. 在控制台输入`npm run build`，等待构建完成。
5. 在控制台输入`npm run test`，输入账户、密码、主管账户。这会生成一个`config.json`文件，里面保存了你的用户信息。
6. 现在你已经可以使用骰子了。

你可以使用`npm run start`来启动一个服务，在后台常驻运行机器人。使用`npm run stop`来关闭这个机器人。注意，至少使用`npm run test`来完成一次登陆后才能这样使用。

# 实例  

QQ：1701687847  
它能够自动接受好友请求和加群邀请。请温柔对待它。  

# 使用手册

[点击这里](REFERENCE.md)查看使用帮助。