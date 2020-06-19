智能家居系统
  
1. 项目介绍  
    使用Linux C编程技术实现的智能家居系统     
    在虚拟LCD上显示虚拟传感器数据(烟雾浓度/温度/湿度/光照强度)并提醒，同时实现虚拟家电的控制  
    部署阿里云服务器 接受/转发 数据  
    客户端进入相应数据界面即上传对应数据，进入家居控制界面即可 本地/远程 共同控制本地设备  


2. 使用  
    make clean  
    make  
    sudo ./main  


3. 项目结构  
    在main主程序中显示初始化LCD设备  
    展示欢迎界面，等待选择功能  
    根据全局变量page_index判断选择的界面  
    进入相应的界面  
    退出  


4. 涉及知识  
    Linux的基本命令  
    Linux IO操作  
    Linux 多线程 
    Linux网络编程 
    Linux 输入子系统基本概念  
    Makefile  
    LCD屏幕的基本原理  
    LCD显示文字图片(bmp)的基本原理  
    传感器文档的阅读  
    阿里云服务端部署


5. 资料  
    环境：  
        Linux 18.04带环境系统  
        LCD模拟屏幕  
        模拟传感器  

    工具：  
        字模提取  
    链接：  
        链接：https://pan.baidu.com/s/1SS2E76q2OChV4UITfHm0Wg   
        提取码：0j40  


致谢：
    感谢小伙伴 https://github.com/lei89 的UI支持  
    感谢 粤嵌 提供的环境等相关资源  


项目状态：  
    更新中...

