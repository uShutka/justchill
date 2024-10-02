#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <thread>
#include <atomic>

using namespace sf;

const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 800;
const int VIEW_START_X = 2500;
const int VIEW_START_Y = 2500;
const int GRID_WIDTH = 6500;
const int GRID_HEIGHT = 5800;
const float GRID_CELL_SIZE = 20.0f;
const int CENTER_X = 3250;
const int CENTER_Y = 2900;
const int FONT_SIZE = 30;
const int LINE_WIDTH = 1;

std::atomic<double> input_x(0);
std::atomic<double> input_y(0);
std::atomic<bool> new_input(false);


bool backend(double x, double y) {
    bool circle = 0, square = 0, rhomb = 0, lowline = 0, upline = 0, parabolax = 0, parabolay = 0;
    if (-4 * x - 10 < y) {
        lowline = 1;
    }

    if (4 * fabs(x + 0.5) + 2 * fabs(y - 1.5) < 10) {
        rhomb = 1;
    }

    if ((x - 1) * (x - 1) + (y - 4) * (y - 4) < 4) {
        circle = 1;
    }

    if (-x * x + 2 * x + 2 > y) {
        parabolax = 1;
    }

    if (y * y - 7 * y + 12.75 < x) {
        parabolay = 1;
    }

    if (1.4 * x - 4.8 < y) {
        upline = 1;
    }

    if (x < -1 and x > -2.5 and y > 0.5 and y < 4.5) {
        square = 1;
    }

    if (!rhomb and lowline and !square and (x < 0) and !parabolax) {
        return 1;
    }
    else if (rhomb and !square and !circle and !parabolax and (y > 0 and y < 4) and (x > -2.5)) {
        return 1;
    }
    else if (parabolay and circle and !rhomb and !parabolax) {
        return 1;
    }
    else if (parabolay and parabolax and circle and rhomb) {
        return 1;
    }
    else if (!upline and !parabolax and !parabolay) {
        return 1;
    }
    return 0;
}



void drawCoordinateAxes(RenderWindow& window) {
    RectangleShape ox(Vector2f(GRID_WIDTH, 3));
    RectangleShape oy(Vector2f(3, GRID_HEIGHT));

    ox.setPosition(0, CENTER_Y);
    oy.setPosition(CENTER_X - 2, 0);

    ox.setFillColor(Color::Black);
    oy.setFillColor(Color::Black);

    window.draw(ox);
    window.draw(oy);
}

void drawCoordinateLabels(RenderWindow& window, Font& font) {
    Vector2u window_size = window.getSize();

    CircleShape arrow_x(15, 3);
    CircleShape arrow_y(15, 3);
    arrow_x.rotate(90);
    arrow_y.rotate(120);

    arrow_x.setFillColor(Color::Black);
    arrow_y.setFillColor(Color::Black);

    Text xLabel("x", font, FONT_SIZE);
    Text yLabel("y", font, FONT_SIZE);
    xLabel.setFillColor(Color::Black);
    yLabel.setFillColor(Color::Black);

    arrow_x.setPosition(window_size.x, window_size.y / 2 - 14);
    arrow_y.setPosition(window_size.x / 2 + 20, 7);

    xLabel.setPosition(window_size.x - 20, window_size.y / 2 + 5);
    yLabel.setPosition(window_size.x / 2 + 20, 0);

    window.draw(arrow_x);
    window.draw(arrow_y);
    window.draw(xLabel);
    window.draw(yLabel);
}


void drawGrid(RenderWindow& window) {
    VertexArray vertical_lines(Lines, GRID_WIDTH / GRID_CELL_SIZE * 2);
    for (int i = 0; i < GRID_WIDTH / GRID_CELL_SIZE * 2; i += 2) {
        vertical_lines[i].position = Vector2f(i * GRID_CELL_SIZE / 2 + 10, 0);
        vertical_lines[i + 1].position = Vector2f(i * GRID_CELL_SIZE / 2 + 10, GRID_HEIGHT);
        vertical_lines[i].color = Color::Black;
        vertical_lines[i + 1].color = Color::Black;
    }

    VertexArray horizontal_lines(Lines, GRID_HEIGHT / GRID_CELL_SIZE * 2);
    for (int i = 0; i < GRID_HEIGHT / GRID_CELL_SIZE * 2; i += 2) {
        horizontal_lines[i].position = Vector2f(0, i * GRID_CELL_SIZE / 2);
        horizontal_lines[i + 1].position = Vector2f(GRID_WIDTH, i * GRID_CELL_SIZE / 2);
        horizontal_lines[i].color = Color::Black;
        horizontal_lines[i + 1].color = Color::Black;
    }

    window.draw(vertical_lines);
    window.draw(horizontal_lines);
}

void updateImagePixels(Image& image) {
    Color custom_color(252, 164, 194);

    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            double x = (i - CENTER_X) / GRID_CELL_SIZE;
            double y = (-j + CENTER_Y) / GRID_CELL_SIZE;
            if (backend(x, y)) {
                image.setPixel(i, j, custom_color);
            }
        }
    }
}

void drawCircle(RenderWindow& window) {
    float radius = 2.0f * GRID_CELL_SIZE;

    float center_x = CENTER_X + 1.0f * GRID_CELL_SIZE;
    float center_y = CENTER_Y - 4.0f * GRID_CELL_SIZE;

    CircleShape circle(radius);
    circle.setPosition(center_x - radius, center_y - radius);
    circle.setFillColor(Color::Transparent);
    circle.setOutlineThickness(LINE_WIDTH);
    circle.setOutlineColor(Color::Black);

    window.draw(circle);
}

void drawRhombus(RenderWindow& window) {
    float center_x = CENTER_X + (-0.5f) * GRID_CELL_SIZE;
    float center_y = CENTER_Y - 1.5f * GRID_CELL_SIZE;

    float rhombus_width = 2.5f * GRID_CELL_SIZE;
    float rhombus_height = 5.0f * GRID_CELL_SIZE;

    Vector2f right(center_x + rhombus_width, center_y);
    Vector2f left(center_x - rhombus_width, center_y);
    Vector2f top(center_x, center_y - rhombus_height);
    Vector2f bottom(center_x, center_y + rhombus_height);

    RectangleShape line1;
    Vector2f direction1 = top - right;
    float length1 = std::sqrt(direction1.x * direction1.x + direction1.y * direction1.y);

    line1.setSize(Vector2f(length1, LINE_WIDTH));
    line1.setOrigin(0, LINE_WIDTH / 2);
    line1.setPosition(right);
    line1.setRotation(std::atan2(direction1.y, direction1.x) * 180 / 3.14159);
    line1.setFillColor(Color::Black);

    RectangleShape line2;
    Vector2f direction2 = left - top;
    float length2 = std::sqrt(direction2.x * direction2.x + direction2.y * direction2.y);
    line2.setSize(Vector2f(length2, LINE_WIDTH));
    line2.setOrigin(0, LINE_WIDTH / 2);
    line2.setPosition(top);
    line2.setRotation(std::atan2(direction2.y, direction2.x) * 180 / 3.14159);
    line2.setFillColor(Color::Black);

    RectangleShape line3;
    Vector2f direction3 = bottom - left;
    float length3 = std::sqrt(direction3.x * direction3.x + direction3.y * direction3.y);
    line3.setSize(Vector2f(length3, LINE_WIDTH));
    line3.setOrigin(0, LINE_WIDTH / 2);
    line3.setPosition(left);
    line3.setRotation(std::atan2(direction3.y, direction3.x) * 180 / 3.14159);
    line3.setFillColor(Color::Black);

    RectangleShape line4;
    Vector2f direction4 = right - bottom;
    float length4 = std::sqrt(direction4.x * direction4.x + direction4.y * direction4.y);
    line4.setSize(Vector2f(length4, LINE_WIDTH));
    line4.setOrigin(0, LINE_WIDTH / 2);
    line4.setPosition(bottom);
    line4.setRotation(std::atan2(direction4.y, direction4.x) * 180 / 3.14159);
    line4.setFillColor(Color::Black);

    window.draw(line1);
    window.draw(line2);
    window.draw(line3);
    window.draw(line4);
}

void drawRectangle(RenderWindow& window) {
    float RECT_LEFT_X = -2.5f;
    float RECT_RIGHT_X = -1.0f;
    float RECT_BOTTOM_Y = 0.5f;
    float RECT_TOP_Y = 4.5f;

    float width = (RECT_RIGHT_X - RECT_LEFT_X) * GRID_CELL_SIZE;
    float height = (RECT_TOP_Y - RECT_BOTTOM_Y) * GRID_CELL_SIZE;

    float left_x = RECT_LEFT_X * GRID_CELL_SIZE + CENTER_X;
    float top_y = CENTER_Y - RECT_TOP_Y * GRID_CELL_SIZE;

    RectangleShape rectangle(Vector2f(width, height));
    rectangle.setPosition(left_x, top_y);
    rectangle.setFillColor(Color::Transparent);
    rectangle.setOutlineThickness(LINE_WIDTH);
    rectangle.setOutlineColor(Color::Black);

    window.draw(rectangle);
}

float calculateForParabolaX(float y) {
    return y * y - 7 * y + 12.75;
}

void drawParabolaX(RenderWindow& window) {
    VertexArray parabola(LinesStrip);

    float y = -10.0f;
    float prev_y = y;
    float prev_x = calculateForParabolaX(y);

    y += 0.1f;

    while (y <= 10.0f) {
        float x = calculateForParabolaX(y);

        float screen_x1 = CENTER_X + prev_x * GRID_CELL_SIZE;
        float screen_y1 = CENTER_Y - prev_y * GRID_CELL_SIZE;
        float screen_x2 = CENTER_X + x * GRID_CELL_SIZE;
        float screen_y2 = CENTER_Y - y * GRID_CELL_SIZE;

        if (screen_y1 > GRID_HEIGHT || screen_x1 < 0 || screen_x1 > GRID_WIDTH) {
            prev_x = x;
            prev_y = y;
            y += 0.1f;
            continue;
        }

        RectangleShape line_segment;
        Vector2f direction(screen_x2 - screen_x1, screen_y2 - screen_y1);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        line_segment.setSize(Vector2f(length, LINE_WIDTH));
        line_segment.setOrigin(0, LINE_WIDTH / 2);
        line_segment.setPosition(screen_x1, screen_y1);
        line_segment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);
        line_segment.setFillColor(Color::Black);

        window.draw(line_segment);

        prev_x = x;
        prev_y = y;

        y += 0.1f;
    }
}


float calculateYForParabola(float x) {
    return -x * x + 2 * x + 2;
}

void drawParabolaY(RenderWindow& window) {
    VertexArray parabola(LinesStrip);

    float x = -10.0f;
    float prev_x = x;
    float prev_y = calculateYForParabola(x);

    x += 0.1f;

    while (x <= 10.0f) {
        float y = calculateYForParabola(x);

        float screen_x1 = CENTER_X + prev_x * GRID_CELL_SIZE;
        float screen_y1 = CENTER_Y - prev_y * GRID_CELL_SIZE;
        float screen_x2 = CENTER_X + x * GRID_CELL_SIZE;
        float screen_y2 = CENTER_Y - y * GRID_CELL_SIZE;

        if (screen_y1 > GRID_HEIGHT || screen_x1 < 0 || screen_x1 > GRID_WIDTH) {
            prev_x = x;
            prev_y = y;
            x += 0.1f;
            continue;
        }

        RectangleShape line_segment;
        Vector2f direction(screen_x2 - screen_x1, screen_y2 - screen_y1);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        line_segment.setSize(Vector2f(length, LINE_WIDTH));
        line_segment.setOrigin(0, LINE_WIDTH / 2);
        line_segment.setPosition(screen_x1, screen_y1);
        line_segment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);
        line_segment.setFillColor(Color::Black);

        window.draw(line_segment);

        prev_x = x;
        prev_y = y;

        x += 0.1f;
    }
}


float calculateYForFirstLine(float x) {
    return -4 * x - 10;
}

void drawFirstLine(RenderWindow& window) {
    Vector2u window_size = window.getSize();

    float start_x = -static_cast<float>(window_size.x) / GRID_CELL_SIZE;
    float end_x = static_cast<float>(window_size.x) / GRID_CELL_SIZE;

    float start_y = calculateYForFirstLine(start_x);
    float end_y = calculateYForFirstLine(end_x);

    float screen_x1 = CENTER_X + start_x * GRID_CELL_SIZE;
    float screen_y1 = CENTER_Y - start_y * GRID_CELL_SIZE;
    float screen_x2 = CENTER_X + end_x * GRID_CELL_SIZE;
    float screen_y2 = CENTER_Y - end_y * GRID_CELL_SIZE;

    RectangleShape line_segment;
    Vector2f direction(screen_x2 - screen_x1, screen_y2 - screen_y1);
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    line_segment.setSize(Vector2f(length, LINE_WIDTH));
    line_segment.setOrigin(0, LINE_WIDTH / 2);
    line_segment.setPosition(screen_x1, screen_y1);
    line_segment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);
    line_segment.setFillColor(Color::Black);

    window.draw(line_segment);
}

float calculateYForSecondLine(float x) {
    return 1.4 * x - 4.8;
}

void drawSecondLine(RenderWindow& window) {
    Vector2u window_size = window.getSize();

    float start_x = -static_cast<float>(window_size.x) / GRID_CELL_SIZE;
    float end_x = static_cast<float>(window_size.x) / GRID_CELL_SIZE;

    float start_y = calculateYForSecondLine(start_x);
    float end_y = calculateYForSecondLine(end_x);

    float screen_x1 = CENTER_X + start_x * GRID_CELL_SIZE;
    float screen_y1 = CENTER_Y - start_y * GRID_CELL_SIZE;
    float screen_x2 = CENTER_X + end_x * GRID_CELL_SIZE;
    float screen_y2 = CENTER_Y - end_y * GRID_CELL_SIZE;

    RectangleShape line_segment;
    Vector2f direction(screen_x2 - screen_x1, screen_y2 - screen_y1);
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    line_segment.setSize(Vector2f(length, LINE_WIDTH));
    line_segment.setOrigin(0, LINE_WIDTH / 2);
    line_segment.setPosition(screen_x1, screen_y1);
    line_segment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);
    line_segment.setFillColor(Color::Black);

    window.draw(line_segment);
}

void drawShapes(RenderWindow& window) {
    drawCircle(window);
    drawRhombus(window);
    drawRectangle(window);
    drawParabolaX(window);
    drawParabolaY(window);
    drawFirstLine(window);
    drawSecondLine(window);
}

void inputThread() {
    while (true) {
        double x, y;
        std::cout << "Введите координаты (x, y): ";
        std::cin >> x >> y;
        input_x = x;
        input_y = y;
        new_input = true;
    }
}

void drawPoint(RenderWindow& window, double x, double y, bool inZone) {
    float screen_x = CENTER_X + x * GRID_CELL_SIZE;
    float screen_y = CENTER_Y - y * GRID_CELL_SIZE;

    CircleShape point(5); 
    point.setPosition(screen_x - 5, screen_y - 5);  
    point.setFillColor(inZone ? Color::Green : Color::Red); 
    window.draw(point);
}

int main() {
    setlocale(LC_ALL, "Russian");

    Font font;
    if (!font.loadFromFile("Seravek.otf")) {
        return -1;
    }

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "katafalk");
    window.setVerticalSyncEnabled(true);

    View main_view(FloatRect(VIEW_START_X, VIEW_START_Y, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(main_view);

    std::thread input(inputThread);
    input.detach();

    bool point_in_zone = false;

    Image image;
    image.create(GRID_WIDTH, GRID_HEIGHT, Color::White);
    updateImagePixels(image);

    Texture texture;
    texture.loadFromImage(image);

    Sprite sprite(texture);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
                if (event.mouseWheelScroll.delta > 0)
                    main_view.zoom(0.99);
                else
                    main_view.zoom(1.01);
                window.setView(main_view);
            }
        }

        if (new_input) {
            double x = input_x.load();
            double y = input_y.load();
            point_in_zone = backend(x, y);
            std::cout << "Точка (" << x << ", " << y << ") " << (point_in_zone ? "входит" : "не входит") << " в зону" << std::endl;
            new_input = false;
        }

        window.clear(Color::White);

        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);

        drawCoordinateAxes(window);
        drawGrid(window);
        drawShapes(window);

        if (input_x.load() && input_y.load()) {
            drawPoint(window, input_x.load(), input_y.load(), point_in_zone);
        }

        window.setView(window.getDefaultView());
        drawCoordinateLabels(window, font);

        window.setView(main_view);

        window.display();
    }

    return 0;
}