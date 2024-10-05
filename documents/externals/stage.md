
## UyghurScript stage

> source code [stage.c](../../uyghur/externals/stage.c)
---

### 1. set_log

> 📝:  set_log func of stage module

> 🔙: Empty

> 🛒: 
* level  `int`


### 2. set_fps

> 📝:  set_fps func of stage module

> 🔙: Empty

> 🛒:  none


### 3. get_fps

> 📝:  get_fps func of stage module

> 🔙: Number(bridge, GetFPS

> 🛒:  none


### 4. draw_fps

> 📝:  draw_fps func of stage module

> 🔙: Empty

> 🛒: 
* x  `int`
* y  `int`


### 5. show_window

> 📝:  show_window func of stage module

> 🔙: Empty

> 🛒: 
* w  `int`
* h  `int`
* title  `USTRING`
* mode  `int`


### 6. hide_window

> 📝:  hide_window func of stage module

> 🔙: Empty

> 🛒:  none


### 7. is_fullscreen

> 📝:  is_fullscreen func of stage module

> 🔙: Boolean(bridge, IsWindowFullscreen

> 🛒:  none


### 8. is_hidden

> 📝:  is_hidden func of stage module

> 🔙: Boolean(bridge, IsWindowHidden

> 🛒:  none


### 9. is_minimized

> 📝:  is_minimized func of stage module

> 🔙: Boolean(bridge, IsWindowMinimized

> 🛒:  none


### 10. is_maximized

> 📝:  is_maximized func of stage module

> 🔙: Boolean(bridge, IsWindowMaximized

> 🛒:  none


### 11. toggle_fullscreen

> 📝:  toggle_fullscreen func of stage module

> 🔙: Empty

> 🛒:  none


### 12. set_minimize

> 📝:  set_minimize func of stage module

> 🔙: Empty

> 🛒:  none


### 13. set_maximize

> 📝:  set_maximize func of stage module

> 🔙: Empty

> 🛒:  none


### 14. set_normalize

> 📝:  set_normalize func of stage module

> 🔙: Empty

> 🛒:  none


### 15. set_title

> 📝:  set_title func of stage module

> 🔙: Empty

> 🛒:  none


### 16. set_icon

> 📝:  set_icon func of stage module

> 🔙: Empty

> 🛒: 
* path  `USTRING`


### 17. set_position

> 📝:  set_position func of stage module

> 🔙: Empty

> 🛒: 
* x  `int`
* y  `int`


### 18. get_position

> 📝:  get_position func of stage module

> 🔙: Numbers

> 🛒:  none


### 19. set_size

> 📝:  set_size func of stage module

> 🔙: Empty

> 🛒: 
* w  `int`
* h  `int`


### 20. get_size

> 📝:  get_size func of stage module

> 🔙: Numbers(bridge, 2, GetScreenWidth(), GetScreenHeight

> 🛒:  none


### 21. set_min_size

> 📝:  set_min_size func of stage module

> 🔙: Empty

> 🛒: 
* w  `int`
* h  `int`


### 22. show_cursor

> 📝:  show_cursor func of stage module

> 🔙: Empty

> 🛒: 
* b  `bool`


### 23. enable_cursor

> 📝:  enable_cursor func of stage module

> 🔙: Empty

> 🛒: 
* b  `bool`


### 24. have_cursor

> 📝:  have_cursor func of stage module

> 🔙: Boolean(bridge, IsCursorOnScreen

> 🛒:  none


### 25. set_clipboard

> 📝:  set_clipboard func of stage module

> 🔙: Empty

> 🛒: 
* c  `USTRING`


### 26. get_clipboard

> 📝:  get_clipboard func of stage module

> 🔙: String

> 🛒:  none


### 27. set_mouse_cursor

> 📝:  set_mouse_cursor func of stage module

> 🔙: Empty

> 🛒: 
* c  `int`


### 28. get_mouse_position

> 📝:  get_mouse_position func of stage module

> 🔙: Numbers(bridge, 2, GetMouseX(), GetMouseY

> 🛒:  none


### 29. get_mouse_wheel

> 📝:  get_mouse_wheel func of stage module

> 🔙: Number(bridge, GetMouseWheelMove

> 🛒:  none


### 30. get_mouse_key_action

> 📝:  get_mouse_key_action func of stage module

> 🔙: Number

> 🛒: 
* keyCode  `int`


### 31. get_mouse_key_state

> 📝:  get_mouse_key_state func of stage module

> 🔙: Number

> 🛒: 
* keyCode  `int`


### 32. get_keyboard_key_action

> 📝:  get_keyboard_key_action func of stage module

> 🔙: Number

> 🛒: 
* keyCode  `int`


### 33. get_keyboard_key_state

> 📝:  get_keyboard_key_state func of stage module

> 🔙: Number

> 🛒: 
* keyCode  `int`


### 34. save_screenshot

> 📝:  save_screenshot func of stage module

> 🔙: Empty

> 🛒: 
* path  `USTRING`


### 35. audio_set_volume

> 📝:  audio_set_volume func of stage module

> 🔙: Empty

> 🛒: 
* v  `float`


---
> document info [document.md](../README.md)
