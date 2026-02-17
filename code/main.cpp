#include "vstd/vmath.h"
#include <array>
#include "raylib.h"
#include "vstd/vtypes.h"
#include <vector>
#include <iostream>
// TODO: Poder crear tanto arcos como nodos.
// TODO: Poder guardar imagenes en JPG/PNG.
// TODO: Poder hacer redo.


// DEFINE WAY OF DRAWING
enum NODE_KIND: u32 {
    NIL = 0,
    NORMAL,
    INIT,
    GOAL,
};

struct Node {
    NODE_KIND kind;
    vec2 position;
    f32 radius;
    std::string name;
    std::vector<i32> arcs;
    operator bool() const { return kind != NIL; }
};

constexpr auto MAX_NUM_NODES = 50;
struct Mouse
{
    bool pressed;
    vec2 pressed_pos;
    vec2 actual_pos;
    i32 selected_node_idx;
};

enum e_AppState {
    SELECT, 
    CREATE
};

struct App {
    i32 width, height;
    std::array<Node, MAX_NUM_NODES> nodes;
    Mouse mouse;
    
    e_AppState state;
    
    i32 get_empty()
    {
        i32 id = -1;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (!nodes[i])
            {
                id = i;
                break;
            }
        }
        return id;
    }

    i32 add( Node &&node ){
        i32 id = get_empty();
        nodes[id] = node; 
        return id;
    }
};

void Input(App& app);
void Draw(App& app);
vec2 GetMousePositionV();

constexpr auto SCR_WIDTH = 500;
constexpr auto SCR_HEIGHT = 500;

constexpr auto NODE_MIN_SIZE = 50;


int main(void)
{
    App app = { 0 };
    app.width = SCR_WIDTH;
    app.height = SCR_HEIGHT;
    InitWindow(app.width, app.height, "PAINTOMATRON");

    SetTargetFPS(60);


    while(!WindowShouldClose())
    {
        Input(app);
        Draw(app);
    }

    CloseWindow();

    return 0;
}

void Input(App& app)
{
    if (IsKeyPressed(KEY_S)) app.state = SELECT;
    if (IsKeyPressed(KEY_C)) app.state = CREATE;


    switch (app.state)
    {
    case CREATE: {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            app.mouse.pressed = true;
            app.mouse.pressed_pos = GetMousePositionV();
        }
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            app.mouse.actual_pos = GetMousePositionV();
        }
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            app.mouse.pressed = false;

            std::cout<< app.mouse.pressed_pos.to_string()<< "  " << app.mouse.actual_pos.to_string() << std::endl;
            f32 size = fmin(  
                app.mouse.actual_pos.x - app.mouse.pressed_pos.x,
                app.mouse.actual_pos.y - app.mouse.pressed_pos.y 
            );

            if (size >= NODE_MIN_SIZE)
            {
                std::cout << size<< std::endl;
                Rectangle rect = { 
                    app.mouse.pressed_pos.x,
                    app.mouse.pressed_pos.y, 
                    size, size
                };

                app.add({
                    NORMAL, 
                    {rect.x + 0.5f * rect.width, rect.y + 0.5f * rect.height},
                    rect.width * 0.5f, "", {}
                });
       
            }
    } break;
    case SELECT: {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            vec2 mouse_pos = GetMousePositionV();
            for (int i = 0; i < app.nodes.size(); i++)
            {
                Node& node = app.nodes[i];
                if (Vec2Length(node.position - mouse_pos) < node.radius)
                {
                    app.mouse.selected_node_idx = i;
                    break;
                }
            }
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){ app.mouse.selected_node_idx = -1; }
    } break;
        if (app.mouse.selected_node_idx >= 0)
        {
            vec2 mouse_pos = GetMousePositionV();
            app.nodes[app.mouse.selected_node_idx].position = mouse_pos;
        }
    }
    
    }
}
void Draw(App& app)
{
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < app.nodes.size(); i++)
    {
        const auto& node = app.nodes[i];
        if (node)
        {
            DrawCircle(node.position.x, node.position.y, node.radius, GREEN);
        }
    }
    if (app.mouse.pressed)
    {
        f32 size = fmax(fmin(  
            app.mouse.actual_pos.x - app.mouse.pressed_pos.x,
            app.mouse.actual_pos.y - app.mouse.pressed_pos.y 
        ), NODE_MIN_SIZE);

        Rectangle rect = { 
            app.mouse.pressed_pos.x,
            app.mouse.pressed_pos.y, 
            size, size
        };
        DrawRectangleRec(rect, RED);
        DrawCircle(rect.x + 0.5f * rect.width, rect.y + 0.5f * rect.height, rect.width * 0.5f, GREEN);
    }


    // Draw Mode 
    i32 Size = 50;
    i32 Xpos = app.width - Size * 2;
    i32 Ypos = Size;
    DrawRectangle(Xpos, Ypos, Size, Size, RED);
    switch(app.state)
    {
        case SELECT:{
            DrawText("S", Xpos + 4, Ypos, Size, YELLOW);
        }break;
        case CREATE:{
            DrawText("C", Xpos + 4, Ypos, Size, YELLOW);
        }break;
    }
    EndDrawing();

}


vec2 GetMousePositionV()
{
    Vector2 tmouse_pos = GetMousePosition();
    return {tmouse_pos.x, tmouse_pos.y};
}
