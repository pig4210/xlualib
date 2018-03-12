# xlua

---- ---- ---- ----

由于Lua没有强制类型，扩展函数以伪代码形式展示

注意到所有简化函数定义为

```
functionXX            = function( ... ) return functionX                   ( ... ); end 
```
而不定义为

```
functionXX            = functionX;
```
是考虑当`functionX`被修改时，`functionXX`能随之改变。此为记
