
## UyghurScript pencil

> source code [pencil.c](../../uyghur/externals/pencil.c)
---

### 1. draw_pixel

> 📝:  draw_pixel func of pencil module

> 🔙: Empty

> 🛒: 
* point  `UGPoint`
* color  `UGColor`


### 2. draw_line

> 📝:  draw_line func of pencil module

> 🔙: Empty

> 🛒: 
* point1  `UGPoint`
* point2  `UGPoint`
* thickness  `int`
* color  `UGColor`
* controll  `UGPoint`
* _controll  `UGPoint`


### 3. draw_rectangle_fill

> 📝:  draw_rectangle_fill func of pencil module

> 🔙: Empty

> 🛒: 
* rect  `UGRect`
* color  `UGColor`


### 4. draw_rectangle_fill_transformed

> 📝:  draw_rectangle_fill_transformed func of pencil module

> 🔙: Empty

> 🛒: 
* rect  `UGRect`
* color  `UGColor`
* anchor  `UGPoint`
* rotation  `float`


### 5. draw_rectangle_fill_colorful

> 📝:  draw_rectangle_fill_colorful func of pencil module

> 🔙: Empty

> 🛒: 
* rect  `UGRect`
* leftTop  `UGColor`
* leftBottom  `UGColor`
* rightBottom  `UGColor`
* rightTop  `UGColor`


### 6. draw_rectangle_fill_round

> 📝:  draw_rectangle_fill_round func of pencil module

> 🔙: Empty

> 🛒: 
* rect  `UGRect`
* color  `UGColor`
* roundness  `int`


### 7. draw_rectangle_stroke

> 📝:  draw_rectangle_stroke func of pencil module

> 🔙: Empty

> 🛒: 
* rect  `UGRect`
* color  `UGColor`
* thickness  `double`


### 8. draw_circle_fill

> 📝:  draw_circle_fill func of pencil module

> 🔙: Empty

> 🛒: 
* point  `UGPoint`
* size  `UGSize`
* color  `UGColor`


### 9. draw_circle_stroke

> 📝:  draw_circle_stroke func of pencil module

> 🔙: Empty

> 🛒: 
* point  `UGPoint`
* size  `UGSize`
* color  `UGColor`
* thickness  `double`


### 10. draw_triangle_fill

> 📝:  draw_triangle_fill func of pencil module

> 🔙: Empty

> 🛒: 
* point1  `UGPoint`
* point2  `UGPoint`
* point3  `UGPoint`
* color  `UGColor`


### 11. draw_triangle_stroke

> 📝:  draw_triangle_stroke func of pencil module

> 🔙: Empty

> 🛒: 
* point1  `UGPoint`
* point2  `UGPoint`
* point3  `UGPoint`
* color  `UGColor`
* thickness  `double`


### 12. draw_polygon_fill

> 📝:  draw_polygon_fill func of pencil module

> 🔙: Empty

> 🛒: 
* center  `UGPoint`
* sides  `int`
* radius  `double`
* rotation  `double`
* color  `UGColor`


### 13. draw_polygon_stroke

> 📝:  draw_polygon_stroke func of pencil module

> 🔙: Empty

> 🛒: 
* center  `UGPoint`
* sides  `int`
* radius  `double`
* rotation  `double`
* color  `UGColor`
* thickness  `double`


### 14. load_image

> 📝:  load_image func of pencil module

> 🔙: Value

> 🛒: 
* path  `CString`


### 15. load_font

> 📝:  load_font func of pencil module

> 🔙: Value

> 🛒: 
* path  `CString`


### 16. draw_text

> 📝:  draw_text func of pencil module

> 🔙: Empty

> 🛒: 
* font  `UGFont`
* text  `CString`
* size  `float`
* spacing  `float`
* color  `UGColor`
* position  `UGPoint`


### 17. measure_text

> 📝:  measure_text func of pencil module

> 🔙: Number

> 🛒: 
* font  `UGFont`
* text  `CString`
* size  `float`
* spacing  `float`


### 18. draw_image

> 📝:  draw_image func of pencil module

> 🔙: Empty

> 🛒: 
* image  `UGImage`
* x  `int`
* y  `int`
* anchorX  `float`
* anchorY  `float`
* color  `UGColor`
* rotation  `float`
* scale  `float`


---
> document info [document.md](../README.md)
