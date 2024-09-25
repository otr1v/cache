### Cache
Simple implementation of Least Frequently Used caching algorithm(LFU) and Perfect Cachcing algorithm(PCA)
### How to clone and Build:
 - First of all you should clone my repository: <code> git clone https://github.com/otr1v/cache.git </code>
 - Secondly, type <code> cd cache </code>
 - Finally, type <code> cmake . </code>
   - If you want to test my algorithm you can <code> cmake --build . --target target_name </code> , where the "target_name" is "my_cache" or "my_tests" (if you want to run my tests), also i have added targets "my_LFU" and "my_PCA"
   - Launch it using <code> ./my_cache</code> or <code> ./my_tests </code> respectively(added ./my_LFU and ./my_PCA)
#### Debugging my code:
If you want to debug my code use <code> cmake -DCMAKE_BUILD_TYPE=debug . </code>, than just do <code> make </code>
### How to input
First of all write cache size, then input size and after that write desired elements of input.
- Example of input:
- <code> 1 5 1 2 1 2 1 </code>
- Example of output:
- <code> LFU_hits - 0 </code>
- <code> PCA_hits - 2 </code>
