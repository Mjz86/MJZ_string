# mjz_Str .
a fast alternative to std::string.
 based on Arduino style strings .
 faster than Arduino style strings. 
 and in some places faster than std::string.
 to use open the sll  .
 and we have a example .
 there are simple interpretation .
 static allocation (max 16 char (can you change at compile time /code)).
 modular .
 and more... .
 this test is from the example on my 2core laptop :(
 (note i am a student 16yo)
 1. std alloc large:112.567617 ns (1440865500.000000 / 12800000.000000)
2. mjz alloc large:107.213422 ns (1372331800.000000 / 12800000.000000)
3. std dealloc large:60.282164 ns (771611700.000000 / 12800000.000000)
4. mjz dealloc large:57.744437 ns (739128800.000000 / 12800000.000000)
5. mjz dealloc base:43.331180 ns (554639100.000000 / 12800000.000000)
6. mjz alloc base:37.688023 ns (964813400.000000 / 25600000.000000)
7. _timer_:29.905164 ns (382786100.000000 / 12800000.000000)
8. std dealloc base:27.865367 ns (356676700.000000 / 12800000.000000)
9. mjz alloc:23.880281 ns (305667600.000000 / 12800000.000000)
10. std alloc base:22.302766 ns (570950800.000000 / 25600000.000000)
11. mjz dealloc:11.701109 ns (149774200.000000 / 12800000.000000)
12. timer_:11.671062 ns (298779200.000000 / 25600000.000000)
13. std alloc:6.751188 ns (86415200.000000 / 12800000.000000)
14. std dealloc:0.744398 ns (9528300.000000 / 12800000.000000)
1. std alloc large:20.331976 %
2. mjz alloc large:19.364901 %
3. std dealloc large:10.888172 %
4. mjz dealloc large:10.429807 %
5. mjz dealloc base:7.826483 %
6. mjz alloc base:6.807215 %
7. _timer_:5.401474 %
8. std dealloc base:5.033046 %
9. mjz alloc:4.313259 %
10. std alloc base:4.028328 %
11. mjz dealloc:2.113456 %
12. timer_:2.108029 %
13. std alloc:1.219400 %
14. std dealloc:0.134453 %
 
 
