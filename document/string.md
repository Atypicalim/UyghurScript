# string

```python
# replace
fonkisiye @xet[almashturush] bilen [12345-12345-12345-12345] [234] [333] 1 -1 ishlitilsun we r elinsun
# 13335-13335-13335-13335
fonkisiye @xet[birinchisiniAlmashturush] bilen [12345-12345-12345-12345] [234] [333] ishlitilsun we r elinsun
# 13335-12345-12345-12345
fonkisiye @xet[axirqisiniAlmashturush] bilen [12345-12345-12345-12345] [234] [333] ishlitilsun we r elinsun
# 12345-12345-12345-13335
fonkisiye @xet[hemmisiniAlmashturush] bilen [12345-12345-12345-12345] [234] [333] ishlitilsun we r elinsun
# 13335-13335-13335-13335
```

```python
# find
fonkisiye @xet[tepish] bilen [12345-12345-12345-12345] [234] 1 -1 1 ishlitilsun we r elinsun
# 2
fonkisiye @xet[birinchisiniTepish] bilen [12345-12345-12345-12345] [234] ishlitilsun we r elinsun
# 2
fonkisiye @xet[axirqisiniTepish] bilen [12345-12345-12345-12345] [234] ishlitilsun we r elinsun
# 20
```

```python
# cut
fonkisiye @xet[kisish] bilen [abcde] 2 4 ishlitilsun we r elinsun
# bcd
```

```python
# count
fonkisiye @xet[sanash] bilen [abcde] ishlitilsun we r elinsun
# 5
```

```python
# link
fonkisiye @xet[ulash] bilen [abcde] [123] ishlitilsun we r elinsun
# abcde123
```

```python
# format
fonkisiye @xet[formatlash] bilen [=%.2f=] 12.5 ishlitilsun we r elinsun
# =12.50=
```
