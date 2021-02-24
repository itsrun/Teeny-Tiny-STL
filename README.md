#  LMSTL

基于侯捷《STL源码剖析》完成，正在完善中
based on *The Annotated STL Sources(using SGI STL)*,  not completed yet.

####  Current Progress:

#####  STL Containers:

- [ ] **list**

Performance (push_back, Visual Studio 2019, Release）

| Quantity   | LMSTL | std   |
| ---------- | ----- | ----- |
| 500,000    | 6ms   | 29ms  |
| 5,000,000  | 61ms  | 350ms |
| 10,000,000 | 114ms | 675ms |

Performance ( insert at begin, Visual Studio 2019, Release）

| Quantity   | LMSTL | std   |
| ---------- | ----- | ----- |
| 500,000    | 4ms   | 34ms  |
| 5,000,000  | 45ms  | 348ms |
| 10,000,000 | 101ms | 691ms |

- [ ] **vector**
  Performance (push_back, Visual Studio 2019, Release）  

  | Quantity   | LMSTL | std   |
  | ---------- | ----- | ----- |
  | 500,000    | 2ms   | 5ms   |
  | 5,000,000  | 40ms  | 39ms  |
  | 50,000,000 | 356ms | 423ms |

- [ ] **map**

  Performance (insert, Visual Studio 2019, Release）

  | Quantity  | LMSTL | std   |
  | :-------- | ----- | ----- |
  | 100,000   | 13ms  | 17ms  |
  | 500,000   | 81ms  | 90ms  |
  | 1,000,000 | 189ms | 219ms |

  Performance (operator[ ], Visual Studio 2019, Release）

  | Quantity  | LMSTL | std   |
  | --------- | ----- | ----- |
  | 100,000   | 14ms  | 21ms  |
  | 500,000   | 64ms  | 119ms |
  | 1,000,000 | 142ms | 168ms |