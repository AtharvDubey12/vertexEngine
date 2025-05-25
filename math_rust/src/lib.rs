#[repr(C)]
pub struct Vec2 {
    pub x: f32,
    pub y: f32,
}

#[no_mangle]
pub extern "C" fn vec2_add(a: Vec2, b: Vec2) -> Vec2 {
    Vec2 { x: a.x + b.x, y: a.y + b.y }
}
