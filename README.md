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
 str view :16 <br>
 mjzstr :80 vs<br>
 std::str :32<br>
1. mjz alloc large:154.341406 ns (19755700.000000 / 128000.000000)
2. std alloc large:141.796094 ns (18149900.000000 / 128000.000000)
3. mjz dealloc large:69.010156 ns (8833300.000000 / 128000.000000)
4. std dealloc large:58.661719 ns (7508700.000000 / 128000.000000)
5. mjz dealloc base:39.153125 ns (5011600.000000 / 128000.000000)
6. mjz alloc base:37.687500 ns (9648000.000000 / 256000.000000)
7. std dealloc base:29.756250 ns (3808800.000000 / 128000.000000)
8. _timer_:29.291406 ns (3749300.000000 / 128000.000000)
9. mjz alloc:22.847656 ns (2924500.000000 / 128000.000000)
10. std alloc base:21.542578 ns (5514900.000000 / 256000.000000)
11. mjz dealloc:9.522656 ns (1218900.000000 / 128000.000000)
12. std alloc:7.082031 ns (906500.000000 / 128000.000000)
13. mjz_str_view:5.660156 ns (724500.000000 / 128000.000000)
14. timer_:5.054688 ns (647000.000000 / 128000.000000)
15. ~mjz_str_view:4.888281 ns (625700.000000 / 128000.000000)
16. std dealloc:-0.279687 ns (-35800.000000 / 128000.000000)
1. mjz alloc large:24.266906 %
2. std alloc large:22.294422 %
3. mjz dealloc large:10.850380 %
4. std dealloc large:9.223308 %
5. mjz dealloc base:6.155997 %
6. mjz alloc base:5.925558 %
7. std dealloc base:4.678538 %
8. _timer_:4.605451 %
9. mjz alloc:3.592308 %
10. std alloc base:3.387113 %
11. mjz dealloc:1.497235 %
12. std alloc:1.113499 %
13. mjz_str_view:0.889939 %
14. timer_:0.794742 %
15. ~mjz_str_view:0.768578 %
16. std dealloc:-0.043975 %

</p>
 <br>
 <br>
