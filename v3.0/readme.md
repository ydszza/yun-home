更新：  
    修改数据上传为客户端公用一个连接，采用自定义帧格式上传数据与家具控制命令
    家居控制与同步本地状态需并行运行，但云端采用终端形式，所以采用多线程获取控制指令与同步本地家居状态
    