# 对 read 和 mmap 文件读性能测试
# 测试环境 操作系统 Centos7.9 内核 3.10
## 读取磁盘文件测试
+ 测试方法：echo 3 > /proc/sys/vm/drop_caches 释放页缓存，避免缓存的影响
+ yum install time 安装更多信息输出time指令
+ 对一个1G 网络文件进行读
  + 采用mmap方式 /usr/bin/time -v ./mmap_file 结果输出
  	Command being timed: "./mmap_file"
  ```
	User time (seconds): 0.68
	System time (seconds): 0.83
	Percent of CPU this job got: 23% //cpu 占比
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:06.39 //执行命令所花费的时间
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 1090864 //分配内存大小
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 6 //主要页错误，未到swap分区
	Minor (reclaiming a frame) page faults: 272792 //次要页错误，在swap中，但未访问过
	Voluntary context switches: 1075
	Involuntary context switches: 182
	Swaps: 0
	File system inputs: 2179752
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
  ```
  + 采用read方式 /usr/bin/time -v ./read_file
  ```
	Command being timed: "./read_file"
	User time (seconds): 0.87
	System time (seconds): 0.75
	Percent of CPU this job got: 27%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:05.96
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 1376
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 3
	Minor (reclaiming a frame) page faults: 395
	Voluntary context switches: 1070
	Involuntary context switches: 173
	Swaps: 0
	File system inputs: 2179384
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0

  ```
+ 分析
  read 和 mmap的区别，read相对于mmap执行了更多的系统调用，read将数据从内核缓冲区中复制到应用缓冲区。而mmap则直接把一个文件映射到进程的地址空间，当引用尚不存在内存页时，在读的过程中就会作为处理页错误的结果而出现(每次错页读发生一次错误)
+ 因此俩者的效率的比较主要在于read 系统调用和额外的复制操作（从fread缓存复制到应用buffer中） 和 mmap读页错误的开销和分配大内存开销 之间的比较。哪一个开销小就表现好。这里从磁盘读取网络文件来看，每次读取一个网络包属于小块 I/O 模型，使用mmap页错误的开销 比 read系统调用和额外复制开销要大。

## 优化mmap 减少页错误
+ 在上面可以看到mmap主要开销在于页错误的开销，查找资料可以添加MAP_POPULATE 预先读 和 MAP_FIXED 内存对齐标志来优化读策略
  + 优化过后mmap /usr/bin/time -v ./mmap_file 的输出 同样先清空系统缓存
  ```
	Command being timed: "./mmap_file"
	User time (seconds): 0.35
	System time (seconds): 0.41
	Percent of CPU this job got: 11%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:06.48
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 1090864
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 6
	Minor (reclaiming a frame) page faults: 278795
	Voluntary context switches: 5962
	Involuntary context switches: 173
	Swaps: 0
	File system inputs: 2179840
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
  ```
  + 相比较MAP_SHARED标志，改用MAP_PRIVATE|MAP_POPULATE|MAP_FIXED 或去掉 MAP_FIXED 执行时间反而略大于之前的，并没有体现出优化的效果。
## 读取内存文件测试
+ 将目标文件放入在 /dev/shm 内存目录下，然后再启动程序进行测试，每次测试前都执行 echo 3 > /proc/sys/vm/drop_caches 释放当前系统缓存来避免干扰
+ 对一个1G 网络文件进行读
  + 采用mmap方式 /usr/bin/time -v ./mmap_file 结果输出
  ```
    Command being timed: "./mmap_file"
	User time (seconds): 0.54
	System time (seconds): 0.25
	Percent of CPU this job got: 96%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.83
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 1090868
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 5
	Minor (reclaiming a frame) page faults: 272766
	Voluntary context switches: 6
	Involuntary context switches: 170
	Swaps: 0
	File system inputs: 832
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
  ```
  + 采用read方式 /usr/bin/time -v ./read_file
  ```
	Command being timed: "./read_file"
	User time (seconds): 0.56
	System time (seconds): 0.38
	Percent of CPU this job got: 99%
	Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.95
	Average shared text size (kbytes): 0
	Average unshared data size (kbytes): 0
	Average stack size (kbytes): 0
	Average total size (kbytes): 0
	Maximum resident set size (kbytes): 1376
	Average resident set size (kbytes): 0
	Major (requiring I/O) page faults: 5
	Minor (reclaiming a frame) page faults: 392
	Voluntary context switches: 6
	Involuntary context switches: 172
	Swaps: 0
	File system inputs: 832
	File system outputs: 0
	Socket messages sent: 0
	Socket messages received: 0
	Signals delivered: 0
	Page size (bytes): 4096
	Exit status: 0
  ```

+ 可以看到当文件在 /dev/shm 内存中，mmap页错误的消耗要小于 fread的系统调用和额外复制操作。

## 如何选择
mmap应用于已经存在于缓存的数据，页错误的开销要小于fread系统调用和额外复制的开销，且编程逻辑会比fread更加简单，应选择mmap
而当数据未缓存在磁盘中，那么使用read的系统调用和额外复制开销会小于mmap的页错误开销，因此应该选择read。
