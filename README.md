# mjz_Str .<br>
a fast alternative to std::string.<br>
 based on Arduino style strings .<br>
 faster than Arduino style strings. <br>
 and in some places faster than std::string.<br>
 to use open the sll  .<br>
 and we have a example .<br>
 there are simple interpretation .<br>
 static allocation (max 64 char (can you change at compile time /code)).<br>
 modular .<br>
 and more... .<br>
 this test is from the example on my 2core laptop :(<br>
 (note i am a student 16yo)<br>
 1. std alloc large:112.567617 ns (1440865500.000000 / 12800000.000000)<br>
2. mjz alloc large:107.213422 ns (1372331800.000000 / 12800000.000000)<br>
3. std dealloc large:60.282164 ns (771611700.000000 / 12800000.000000)<br>
4. mjz dealloc large:57.744437 ns (739128800.000000 / 12800000.000000)<br>
5. mjz dealloc base:43.331180 ns (554639100.000000 / 12800000.000000)<br>
6. mjz alloc base:37.688023 ns (964813400.000000 / 25600000.000000)<br>
7. _timer_:29.905164 ns (382786100.000000 / 12800000.000000)<br>
8. std dealloc base:27.865367 ns (356676700.000000 / 12800000.000000)<br>
9. mjz alloc:23.880281 ns (305667600.000000 / 12800000.000000)<br>
10. std alloc base:22.302766 ns (570950800.000000 / 25600000.000000)<br>
11. mjz dealloc:11.701109 ns (149774200.000000 / 12800000.000000)<br>
12. timer_:11.671062 ns (298779200.000000 / 25600000.000000)<br>
13. std alloc:6.751188 ns (86415200.000000 / 12800000.000000)<br>
14. std dealloc:0.744398 ns (9528300.000000 / 12800000.000000)<br>
<br><br>
    percentage of the time token:<br>
1. std alloc large:20.331976 %<br>
2. mjz alloc large:19.364901 %<br>
3. std dealloc large:10.888172 %<br>
4. mjz dealloc large:10.429807 %<br>
5. mjz dealloc base:7.826483 %<br>
6. mjz alloc base:6.807215 %<br>
7. _timer_:5.401474 %<br>
8. std dealloc base:5.033046 %<br>
9. mjz alloc:4.313259 %<br>
10. std alloc base:4.028328 %<br>
11. mjz dealloc:2.113456 %<br>
12. timer_:2.108029 %<br>
13. std alloc:1.219400 %<br>
14. std dealloc:0.134453 %<br>
 <br>
 <br>
