# mjz_Str .<br>
a fast alternative to std::string.<br>
 based on Arduino style strings .<br>
 faster than Arduino style strings . <br>
 and in some places faster than std::string.<br>
 to use open the sll  .<br>
 and we have a example .<br>
 there are simple interpretation .<br>
 static allocation (max 64 char (can you change at compile time /code)).<br>
 modular .<br>
 and more... .<br>
 this test is from the example on my 2core laptop :(<br>
 (note i am a student 16yo)<br>
 [TIMER] total - 4.3974e+08ns
shredptr :16 mjzstr :80 vs
 std::str 32
1. mjz alloc large:395.757031 ns (50656900.000000 / 128000.000000)
2. std alloc large:179.942969 ns (23032700.000000 / 128000.000000)
3. mjz dealloc large:84.958594 ns (10874700.000000 / 128000.000000)
4. std dealloc large:71.544531 ns (9157700.000000 / 128000.000000)
5. mjz dealloc base:43.943750 ns (5624800.000000 / 128000.000000)
6. mjz alloc base:41.360547 ns (10588300.000000 / 256000.000000)
7. _timer_:37.071875 ns (4745200.000000 / 128000.000000)
8. std dealloc base:36.661719 ns (4692700.000000 / 128000.000000)
9. mjz alloc:27.127344 ns (3472300.000000 / 128000.000000)
10. std alloc base:26.041016 ns (6666500.000000 / 256000.000000)
11. mjz dealloc:10.259375 ns (1313200.000000 / 128000.000000)
12. std alloc:9.551562 ns (1222600.000000 / 128000.000000)
13. timer_:4.303125 ns (550800.000000 / 128000.000000)
14. std dealloc:2.267187 ns (290200.000000 / 128000.000000)
1. mjz alloc large:40.766466 %
2. std alloc large:18.535713 %
3. mjz dealloc large:8.751485 %
4. std dealloc large:7.369718 %
5. mjz dealloc base:4.526594 %
6. mjz alloc base:4.260501 %
7. _timer_:3.818730 %
8. std dealloc base:3.776481 %
9. mjz alloc:2.794356 %
10. std alloc base:2.682454 %
11. mjz dealloc:1.056806 %
12. std alloc:0.983895 %
13. timer_:0.443260 %
14. std dealloc:0.233540 %

</p>
 <br>
 <br>
