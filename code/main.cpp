#include "vstd/vmath.h"
#include <array>
#include "raylib.h"
#include "vstd/vtypes.h"
#include <cstdio>
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


constexpr auto BACKGROUND_COLOR = RAYWHITE;
constexpr auto NODE_COLOR_A = WHITE;
constexpr auto NODE_COLOR_B = BLACK;
constexpr auto ARC_COLOR = BLACK;
constexpr auto TEXT_COLOR = BLACK;

struct arc_info {
    i32 node_id;
    i32 arc_id;
};

struct arc {
    arc_info info;
    char val;
};


struct Node {
    NODE_KIND kind;
    vec2 position;
    f32 radius;
    std::vector<arc> arcs;
    operator bool() const { return kind != NIL; }

    void add_arc (i32 node_id, i32 arc_id) {
        for (auto& arc : arcs)
        {
            if (arc.info.arc_id == arc_id) 
            {
                arc.info.arc_id = 0;
                arc.val = ' ';
                return;
            }
        }

        arc temp_arc = {{0}, 0};
        temp_arc.info = {node_id, arc_id};
        //temp_arc.val = ' ';
        arcs.push_back(temp_arc);
    }
};


struct Mouse
{
    bool pressed;
    vec2 pressed_pos;
    vec2 actual_pos;
    i32 selected_node_idx;
    // [0] = node
    // [1] = arch
    arc_info selected_arc_info;
};

enum e_AppState {
    SELECT, 
    CREATE,
    RELATION,
    WRITE,
};

constexpr auto MAX_NUM_NODES = 50;
struct App {
    i32 width, height;
    std::array<Node, MAX_NUM_NODES> nodes;
    Mouse mouse;
    
    e_AppState state;

    void delete_arcs_to_id(i32 id)
    {
        for(auto& node: nodes)
        {
            for (auto& arc: node.arcs)
            {
                if (arc.info.arc_id == id) 
                {
                    arc = {{0}, 0};
                }
            }
        }
    }

    Node* get_node_selected()
    {
        Node* pnode = &nodes[0];
        if (mouse.selected_node_idx >= 1) 
        {
            pnode = &nodes[mouse.selected_node_idx];
        }
        return pnode;
    }

    arc_info check_arc_collision(vec2 pos)
    {
        for (int i = 1; i < nodes.size(); i++)
        {
            const auto& node = nodes[i];
            if (!node) continue;
            vec2 startpos = node.position;
            for (int j = 0; j < node.arcs.size(); j++)
            {
                if (!nodes[node.arcs[j].info.arc_id]) continue;
                vec2 endpos = nodes[node.arcs[j].info.arc_id].position;
                if (CheckCollisionPointLine({pos.x, pos.y}, { startpos.x, startpos.y }, { endpos.x, endpos.y }, 10))
                {
                    printf("JABUGA\n");
                    return { i, j };
                }
            }
        }
        return {0, 0};
    }



    i32 check_collision(vec2 pos)
    {
        int id = 0;
        for (int i = 0; i < nodes.size(); i++)
        {
            Node& node = nodes[i];
            if (Vec2Length(node.position - pos) < node.radius)
            {
                id = i;
                break;
            }
        }
        return id;
    }

    i32 get_empty()
    {
        i32 id = -1;
        for (int i = 1; i < nodes.size(); i++)
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
constexpr auto LINES_THIKNESS = 2;
constexpr auto ARC_LABEL_FONT_SIZE = 22;
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
    if (IsKeyPressed(KEY_R)) app.state = RELATION;

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
                    rect.width * 0.5f, {}
                });
            }
        }
    } break;
    case WRITE: {      
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) app.state = SELECT;
        int key = GetCharPressed();
        if (key > 0)
        {
            app.nodes[app.mouse.selected_arc_info.node_id].arcs[app.mouse.selected_arc_info.arc_id].val = key;
            app.state = SELECT;
        }
        


                } break;
    case SELECT: {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            app.mouse.selected_node_idx = app.check_collision(GetMousePositionV());
            if (app.mouse.selected_node_idx == 0)
            {
                app.mouse.selected_arc_info = app.check_arc_collision(GetMousePositionV()); 
                if (app.mouse.selected_arc_info.node_id != 0) { app.state = WRITE; }
            }
        } 


        Node* pnode = app.get_node_selected();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && *pnode)
        {
            pnode->position = GetMousePositionV();
        }


        if (IsKeyReleased(KEY_D) && *pnode)
        {
            app.delete_arcs_to_id(app.mouse.selected_node_idx);
            app.mouse.selected_node_idx = 0;
            pnode->kind = NIL;
        }
    } break;
    case RELATION:{
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Node *pnode = app.get_node_selected();
            if (*pnode)
            {
                i32 id = app.check_collision(GetMousePositionV());
                if (app.nodes[id])
                {
                    pnode->add_arc(app.mouse.selected_node_idx, id);
                    app.mouse.selected_node_idx = 0;
                }
            }
            else
            {
                app.mouse.selected_node_idx = app.check_collision(GetMousePositionV());
            }
        } 
    } break;
    }
}
void Draw(App& app)
{
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);

    for (int i = 1; i < app.nodes.size(); i++)
    {
        const auto& node = app.nodes[i];
        if (node)
        {
            DrawCircle(node.position.x, node.position.y, node.radius, NODE_COLOR_A);
            DrawCircleLines(node.position.x, node.position.y, node.radius, NODE_COLOR_B);

            char buff[4];
            sprintf_s(buff, "q%d", i);
            DrawText(buff, node.position.x, node.position.y, node.radius * 0.5, TEXT_COLOR);
            for (const auto& arc: node.arcs)
            {
                Node endnode = app.nodes[arc.info.arc_id];
                if (!endnode) continue;

                vec2 startpos = node.position;
                vec2 endpos = endnode.position;
                vec2 direction = Vec2Dir(endpos - startpos);
                startpos.x += direction.x * node.radius;
                startpos.y += direction.y * node.radius;

                endpos.x -= direction.x * endnode.radius;
                endpos.y -= direction.y * endnode.radius;


                vec2 line_end = endnode.position - Vec2xScalar(direction, endnode.radius+ 5);
                DrawLineEx({startpos.x, startpos.y}, {line_end.x, line_end.y}, LINES_THIKNESS, ARC_COLOR);
                

                vec2 midpos = {(startpos.x + endpos.x) * 0.5f, (startpos.y + endpos.y) * 0.5f};
                midpos.y -= 40;
                char temp_str[2];
                temp_str[0] = arc.val;
                temp_str[1] = '\0';
                DrawText(temp_str, midpos.x, midpos.y, ARC_LABEL_FONT_SIZE, TEXT_COLOR);

                const auto ArrowLength = 20.0f;
                vec2 back = { -direction.x, -direction.y };
                f32 angle =  30.0f * DEG2RAD;



                vec2 left = { back.x * cosf(angle) - back.y * sinf(angle), back.x * sinf(angle) + back.y * cosf(angle)};
                vec2 right = { back.x * cosf(-angle) - back.y * sinf(-angle), back.x * sinf(-angle) + back.y * cosf(-angle)};
        
                left = Vec2xScalar(left, ArrowLength) + endpos;
                right = Vec2xScalar(right, ArrowLength) + endpos;
                
                
                DrawTriangle({endpos.x, endpos.y}, {left.x, left.y}, {right.x, right.y}, ARC_COLOR);
                //DrawLineEx({endpos.x, endpos.x}, {arrow1.x, arrow1.y}, LINES_THIKNESS, BLUE);
                //DrawLineEx({endpos.x, endpos.x}, {arrow2.x, arrow2.y}, LINES_THIKNESS, BLUE);
            }
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
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, NODE_COLOR_B);
        DrawCircle(rect.x + 0.5f * rect.width, rect.y + 0.5f * rect.height, rect.width * 0.5f, NODE_COLOR_A);
        DrawCircleLines(rect.x + 0.5f * rect.width, rect.y + 0.5f * rect.height, rect.width * 0.5f, NODE_COLOR_B);
    }

    Node *pnode = app.get_node_selected();
    if (*pnode)
    {
        DrawRectangleLines(pnode->position.x - pnode->radius, pnode->position.y - pnode->radius, pnode->radius * 2, pnode->radius * 2, RED);
    }

    

    // Draw Mode 
    i32 Size = 50;
    i32 Xpos = app.width - Size * 2;
    i32 Ypos = Size;
    DrawRectangle(Xpos, Ypos, Size, Size, NODE_COLOR_A);
    DrawRectangleLines(Xpos, Ypos, Size, Size, NODE_COLOR_B);
    switch(app.state)
    {
        case SELECT:{
            DrawText("S", Xpos + 6, Ypos, Size, TEXT_COLOR);
        }break;
        case CREATE:{
            DrawText("C", Xpos + 6, Ypos, Size, TEXT_COLOR);
        }break;
        case RELATION:{
            DrawText("R", Xpos + 6, Ypos, Size, TEXT_COLOR);
        }break;
        case WRITE:{
            DrawText("W", Xpos + 6, Ypos, Size, TEXT_COLOR);
        }break;
    }
    EndDrawing();

}


vec2 GetMousePositionV()
{
    Vector2 tmouse_pos = GetMousePosition();
    return {tmouse_pos.x, tmouse_pos.y};
}


