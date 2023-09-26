# ROS_K60
1
为ROS提供底层支持的K60工程\
支持两种编译器:

* armcc
* armgcc

## 使用 armcc 编译工程

armcc是直接利用提供 ARM 的 MDK, 只需要下载 keil_5 然后进入 MDK-ARM目录，用 keil_5 直接打开里面的工程配置文件，就可以使用了。

## 使用 armgcc 编译工程

* Linux平台

  * 下载gcc-arm-none-eabi

    Linux 平台下有几种安装方式

    1. 可打开命令行直接输入

       `sudo apt-get install gcc-arm-none-eabi`

        测试过在不添加其它ppa的情况下，ubuntu16.04和ubuntu18.04都可以自动安装，只是默认版本号高低不同。\

        Ubuntu18.04下为gcc version 6.3.1 20170620；\
        ubuntu 16.04下为gcc version 4.9.3 20141119。

    2. 添加新的PPA进行安装

       参考[官方网站](https://launchpad.net/~team-gcc-arm-embedded/+archive/ubuntu/ppa)，有详细的教程

    3. 安装指定的版本

       版本[下载](https://launchpad.net/gcc-arm-embedded/+download)官方网站

       * 下载源码包后解压，如本机解压到目录 `/usr/lib/gcc`
       * 添加环境变量 `vi /etc/profile`
       * 在文件最后添加

         ```bash
         export PATH=$PATH:/usr/lib/gcc/gcc-arm-none-eabi-4_9-2014q4/bin
         ```

       * 使能环境变量 `source /etc/profile`\
       此命令只在当前终端有效，若需要在其它终端中使用，需要重启计算机。

  * 查看是否安装成功，通过查询版本号指令 `arm-none-eabi-gcc -v`

  * 添加 ARMGCC_DIR 环境变量

    `gedit ~/.bashrc`

    在末尾添加

    ```bash
    export ARMGCC_DIR=/usr
    ```

    生效环境变量

    `source ~/.bashrc`

    **注：** ARMGCC_DIR 变量是根据 `gcc-arm-none-eabi` 所在的安装目录来写的，如果是自动安装，则一般写 `/usr` 。如果是下载安装包安装，则写相应的安装包的位置 比如根据上面的安装位置可以写 `/usr/lib/gcc/gcc-arm-none-eabi-4_9-2014q4`
  
  * 下载JLink

    打开 [SERGGER 官网](https://www.segger.com/downloads/jlink/) 下载地址

    下拉找到 J-Link Software and Documentation Pack，点击右边 + 号，展开下载列表。选择对应的 Linux 版本即可。

  * 安装 JLink
  
    选择一个安装位置安装deb安装包，安装完成后添加环境变量

    `gedit ~/.bashrc`

    在末尾添加安装位置

    ```bash
    export JLINK_PATH=/opt/SEGGER/JLink_V644h
    ```

     生效环境变量

    `source ~/.bashrc`

  * 安装好 armgcc 和JLink 以及配置好环境后就可以编译和下载了

    * 编译代码

      进入工程目录的cmake文件夹下，打开命令行执行

      `sh build.sh`

      成功后将会生成 hex文件然后将K60板插上 Jlink 执行

      `sh flash.sh`

      下载
