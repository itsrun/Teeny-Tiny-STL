#  LMSTL

基于侯捷《STL源码剖析》完成，正在施工中  
based on *The Annotated STL Sources(using SGI STL)*,  not completed yet.

####  目前进度：
#####  STL容器：
*  vector  
Performance (Visual Studio 2019, Release）  
push_back| quantity | time  
---------| -------- | ----
LMSTL    | 500,000   | 3ms
std      | 500,000   | 4ms
LMSTL    | 5,000,000 | 40ms
std      | 5,000,000 | 39ms
LMSTL    | 50,000,000| 356ms
std      | 50,000,000| 423ms