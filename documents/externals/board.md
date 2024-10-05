
## UyghurScript board

> source code [board.c](../../uyghur/externals/board.c)
---

### 1. draw_start

> 📝:  draw_start func of board module

> 🔙: Empty

> 🛒:  none


### 2. draw_end

> 📝:  draw_end func of board module

> 🔙: Empty

> 🛒:  none


### 3. draw_pixel

> 📝:  draw_pixel func of board module

> 🔙: Empty

> 🛒: 
* poit  `Vector2`
* color  `Color`


### 4. draw_line_no_controll

> 📝:  draw_line_no_controll func of board module

> 🔙: Empty

> 🛒: 
* poit1  `Vector2`
* poit2  `Vector2`
* thickness  `int`
* color  `Color`


### 5. draw_line_one_controll

> 📝:  draw_line_one_controll func of board module

> 🔙: Empty

> 🛒: 
* poit1  `Vector2`
* poit2  `Vector2`
* thickness  `int`
* color  `Color`
* controll1  `Vector2`


### 6. draw_line_two_controll

> 📝:  draw_line_two_controll func of board module

> 🔙: Empty

> 🛒: 
* poit1  `Vector2`
* poit2  `Vector2`
* thickness  `int`
* color  `Color`
* controll1  `Vector2`
* controll2  `Vector2`


### 7. draw_rectangle_fill_transformed

> 📝:  draw_rectangle_fill_transformed func of board module

> 🔙: Empty

> 🛒: 
* rectangle  `Rectangle`
* color  `Color`
* anchor  `Vector2`
* rotation  `float`


### 8. draw_rectangle_fill_colorful

> 📝:  draw_rectangle_fill_colorful func of board module

> 🔙: Empty

> 🛒: 
* rectangle  `Rectangle`
* leftTop  `Color`
* leftBottom  `Color`
* rightBottom  `Color`
* rightTop  `Color`


### 9. draw_rectangle_fill_round

> 📝:  draw_rectangle_fill_round func of board module

> 🔙: Empty

> 🛒: 
* rectangle  `Rectangle`
* color  `Color`
* roundness  `int`


### 10. draw_rectangle_stroke

> 📝:  draw_rectangle_stroke func of board module

> 🔙: Empty

> 🛒: 
* rectangle  `Rectangle`
* color  `Color`
* roundness  `double`
* thickness  `double`


### 11. draw_circle_fill

> 📝:  draw_circle_fill func of board module

> 🔙: Empty

> 🛒: 
* centerX  `int`
* centerY  `int`
* radiusH  `int`
* radiusV  `int`
* color  `Color`


### 12. draw_circle_stroke

> 📝:  draw_circle_stroke func of board module

> 🔙: Empty

> 🛒: 
* centerX  `int`
* centerY  `int`
* radiusH  `int`
* radiusV  `int`
* color  `Color`


### 13. draw_ring_fill

> 📝:  draw_ring_fill func of board module

> 🔙: Empty

> 🛒: 
* center  `Vector2`
* innerRadius  `double`
* outerRadius  `double`
* color  `Color`
* startAngle  `double`
* endAngle  `double`


### 14. draw_ring_stroke

> 📝:  draw_ring_stroke func of board module

> 🔙: Empty

> 🛒: 
* center  `Vector2`
* innerRadius  `double`
* outerRadius  `double`
* color  `Color`
* startAngle  `double`
* endAngle  `double`


### 15. draw_triangle_fill

> 📝:  draw_triangle_fill func of board module

> 🔙: Empty

> 🛒: 
* point1  `Vector2`
* point2  `Vector2`
* point3  `Vector2`
* color  `Color`


### 16. draw_triangle_stroke

> 📝:  draw_triangle_stroke func of board module

> 🔙: Empty

> 🛒: 
* point1  `Vector2`
* point2  `Vector2`
* point3  `Vector2`
* color  `Color`


### 17. draw_polygon_fill

> 📝:  draw_polygon_fill func of board module

> 🔙: Empty

> 🛒: 
* center  `Vector2`
* sides  `int`
* radius  `double`
* rotation  `double`
* color  `Color`


### 18. draw_polygon_stroke

> 📝:  draw_polygon_stroke func of board module

> 🔙: Empty

> 🛒: 
* center  `Vector2`
* sides  `int`
* radius  `double`
* rotation  `double`
* thickness  `double`
* color  `Color`


### 19. draw_text

> 📝:  draw_text func of board module

> 🔙: Empty

> 🛒: 
* font  `USTRING`
* text  `USTRING`
* size  `float`
* spacing  `float`
* color  `Color`
* position  `Vector2`


### 20. measure_text

> 📝:  measure_text func of board module

> 🔙: Number

> 🛒: 
* font  `USTRING`
* text  `USTRING`
* size  `float`
* spacing  `float`


### 21. texturize_image

> 📝:  texturize_image func of board module

> 🔙: String

> 🛒: 
* image  `USTRING`
* x  `int`
* y  `int`
* w  `int`
* h  `int`
* flipX  `bool`
* flipY  `bool`


### 22. texturize_text

> 📝:  texturize_text func of board module

> 🔙: String

> 🛒: 
* font  `USTRING`
* text  `USTRING`
* size  `float`
* spacing  `float`


### 23. draw_texture

> 📝:  draw_texture func of board module

> 🔙: Empty

> 🛒: 
* tag  `USTRING`
* x  `int`
* y  `int`
* anchorX  `float`
* anchorY  `float`
* color  `Color`
* fromX  `int`
* fromY  `int`
* width  `int`
* height  `int`
* rotation  `float`
* scale  `float`


---
> document info [document.md](../README.md)
