wch ch32v203c8t6 使用 arduino platformio 的简单例子
--------------------------------------------

使用方法：
安装platformio或则arduino，以下只说明使用platformio的方式
src目录中有文件：
使用哪个将.cpp-xxx 改成 .cpp 编译即可

<<<<<<< HEAD

----------


 **1. main.cpp--hello**

  PA15 灯闪烁

 **2. main.cpp-LED**
=======
 1. main.cpp--hello

  PA15 灯闪烁

 2. main.cpp-LED
>>>>>>> eec10e607f54275dc5d58188e7f326ec8265c184

  PA15与PB3灯闪烁
  

<<<<<<< HEAD
 **3. main.cpp-sd-001-info**
=======
 3. main.cpp-sd-001-info
>>>>>>> eec10e607f54275dc5d58188e7f326ec8265c184

  获取sd卡信息
  

<<<<<<< HEAD
 **4. main.cpp-sd-002-create-del**

  建立删除sd卡中的文件

 **5. main.cpp-sd-003-read**

  读取sd卡中的文件

 **6. main.cpp-w25q**

  测试w25q支持--暂未测试

 **7. main.cpp-ws2812**

  测试ws2812 rgb灯

 **8. oled.cpp-remap i2c pin**

  测试oled屏，这里如果是原生sda\scl引脚的话，可以注释掉复用引脚代码

 **9. st7735.cpp 0.96 180x60**
=======
 4. main.cpp-sd-002-create-del

  建立删除sd卡中的文件

 5. main.cpp-sd-003-read

  读取sd卡中的文件

 6. main.cpp-w25q

  测试w25q支持--暂未测试

 7. main.cpp-ws2812

  测试ws2812 rgb灯

 8. oled.cpp-remap i2c pin

  测试oled屏，这里如果是原生sda\scl引脚的话，可以注释掉复用引脚代码

 9. st7735.cpp 0.96 180x60
>>>>>>> eec10e607f54275dc5d58188e7f326ec8265c184

  彩屏测试代码

------
以上代码均采用自制pcb与扩展板测试
![开发板][1]
<<<<<<< HEAD
![扩展板][2]
  [1]: pic/ch32vx03.jpg
  [2]: pic/ch32vx03-ext.jpg
  
  
  
------
下面请忽略
git add .
git commit -m "2026-3-16-2"
git push

=======


  [1]: pic/ch32vx03.jpg
>>>>>>> eec10e607f54275dc5d58188e7f326ec8265c184
