#include <SFML/Graphics.hpp>
#include <iostream>

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


bool backend(double x, double y) {
    bool in_circle, in_rhomb, in_square;
    bool in_paraby, in_parabx, under_low_line, upper_high_line;

    if (pow((x - 2), 2) + pow((y + 2.5), 2) > 2.25) {
        in_circle = 0;
    }
    else {
        in_circle = 1;
    }

    if (0.5 * fabs(x + 3.5) + fabs(y - 3.5) >= 1.5) {
        in_rhomb = 0;
    }
    else {
        in_rhomb = 1;
    }

    if (x >= -1.5 and x <= 2.5 and y >= 2.5 and y <= 4.5) {
        in_square = 1;
    }
    else {
        in_square = 0;
    }

    if (-1 * y * y + 5 * y - 4.25 < x) {
        in_paraby = 0;
    }
    else {
        in_paraby = 1;
    }

    if (-1 * x * x + 5 * x - 5.25 < y) {
        in_parabx = 0;
    }
    else {
        in_parabx = 1;
    }

    if (-0.33 * x - 0.2522 > y) {
        under_low_line = 1;
    }
    else {
        under_low_line = 0;
    }

    if (1.6 * x - 4.6 < y) {
        upper_high_line = 1;
    }
    else {
        upper_high_line = 0;
    }

    if (!in_paraby && under_low_line && upper_high_line && !in_circle && !in_parabx) {
        return 1;
    }
    else if (!in_paraby && !upper_high_line && under_low_line && !in_parabx) {
        return 1;
    }
    else if (in_paraby && !in_rhomb && in_square) {
        return 1;
    }
    else if (!in_paraby && !in_parabx && !in_square && upper_high_line && (x > 0 && y > 0)) {
        return 1;
    }

    return 0;
}



void drawCoordinateAxes(RenderWindow& window) {
    RectangleShape ox(Vector2f(GRID_WIDTH, 3));
    RectangleShape oy(Vector2f(3, GRID_HEIGHT));
    CircleShape trix(12, 3);
    CircleShape triy(12, 3);
    triy.rotate(90);

    ox.setPosition(0, CENTER_Y);
    oy.setPosition(CENTER_X - 2, 0);
    trix.setPosition(CENTER_X - 11, CENTER_Y - 400);
    triy.setPosition(CENTER_X + 750, CENTER_Y - 11);

    ox.setFillColor(Color::Black);
    oy.setFillColor(Color::Black);
    trix.setFillColor(Color::Black);
    triy.setFillColor(Color::Black);

    window.draw(ox);
    window.draw(oy);
    window.draw(trix);
    window.draw(triy);
}

void drawCoordinateLabels(RenderWindow& window, Font& font) {
    Text xLabel("x", font, FONT_SIZE);
    xLabel.setFillColor(Color::Black);
    xLabel.setPosition(CENTER_X + 730, CENTER_Y + 5);

    Text yLabel("y", font, FONT_SIZE);
    yLabel.setFillColor(Color::Black);
    yLabel.setPosition(CENTER_X - 35, CENTER_Y - 400);

    window.draw(xLabel);
    window.draw(yLabel);
}

void drawGrid(RenderWindow& window) {
    VertexArray verticalLines(Lines, GRID_WIDTH / GRID_CELL_SIZE * 2);
    for (int i = 0; i < GRID_WIDTH / GRID_CELL_SIZE * 2; i += 2) {
        verticalLines[i].position = Vector2f(i * GRID_CELL_SIZE / 2 + 10, 0);
        verticalLines[i + 1].position = Vector2f(i * GRID_CELL_SIZE / 2 + 10, GRID_HEIGHT);
        verticalLines[i].color = Color::Black;
        verticalLines[i + 1].color = Color::Black;
    }

    VertexArray horizontalLines(Lines, GRID_HEIGHT / GRID_CELL_SIZE * 2);
    for (int i = 0; i < GRID_HEIGHT / GRID_CELL_SIZE * 2; i += 2) {
        horizontalLines[i].position = Vector2f(0, i * GRID_CELL_SIZE / 2);
        horizontalLines[i + 1].position = Vector2f(GRID_WIDTH, i * GRID_CELL_SIZE / 2);
        horizontalLines[i].color = Color::Black;
        horizontalLines[i + 1].color = Color::Black;
    }

    window.draw(verticalLines);
    window.draw(horizontalLines);
}

void updateImagePixels(Image& image) {
    Color customColor(173, 255, 213);

    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            double x = (i - CENTER_X) / GRID_CELL_SIZE;
            double y = (-j + CENTER_Y) / GRID_CELL_SIZE;
            if (backend(x, y)) {
                image.setPixel(i, j, customColor);
            }
        }
    }
}

void drawCircle(RenderWindow& window) {
    // Радиус окружности
    float radius = 1.5 * GRID_CELL_SIZE;  // 1.5 в физических координатах, умножаем на масштаб (20)

    // Центр окружности (2, -2.5), переведенный в экранные координаты
    float centerX = CENTER_X + 2 * GRID_CELL_SIZE;
    float centerY = CENTER_Y - (-2.5) * GRID_CELL_SIZE;

    // Создаем окружность
    CircleShape circle(radius);
    circle.setPosition(centerX - radius, centerY - radius);  // Учитываем радиус при позиционировании
    circle.setFillColor(Color::Transparent);  // Отключаем заливку
    circle.setOutlineThickness(LINE_WIDTH);  // Толщина линии
    circle.setOutlineColor(Color::Black);

    window.draw(circle);  // Рисуем окружность
}

void drawRhombus(RenderWindow& window) {
    float RHOMBUS_CENTER_X = -3.5f;  // Центр ромба по оси X
    float RHOMBUS_CENTER_Y = 3.5f;   // Центр ромба по оси Y
    float RHOMBUS_HORIZONTAL_OFFSET = 1.5f * 2;  // Влияние по оси X с коэффициентом 0.5
    float RHOMBUS_VERTICAL_OFFSET = 1.5f;  // Влияние по оси Y с коэффициентом 1.0

    // Вычисляем вершины ромба
    Vector2f right((RHOMBUS_CENTER_X + RHOMBUS_HORIZONTAL_OFFSET) * GRID_CELL_SIZE + CENTER_X,
        CENTER_Y - RHOMBUS_CENTER_Y * GRID_CELL_SIZE);

    Vector2f top(RHOMBUS_CENTER_X * GRID_CELL_SIZE + CENTER_X,
        CENTER_Y - (RHOMBUS_CENTER_Y + RHOMBUS_VERTICAL_OFFSET) * GRID_CELL_SIZE);

    Vector2f left((RHOMBUS_CENTER_X - RHOMBUS_HORIZONTAL_OFFSET) * GRID_CELL_SIZE + CENTER_X,
        CENTER_Y - RHOMBUS_CENTER_Y * GRID_CELL_SIZE);

    Vector2f bottom(RHOMBUS_CENTER_X * GRID_CELL_SIZE + CENTER_X,
        CENTER_Y - (RHOMBUS_CENTER_Y - RHOMBUS_VERTICAL_OFFSET) * GRID_CELL_SIZE);

    // Создаем прямоугольники для каждой линии ромба (имитируем толстую линию)

    // Правая линия (right -> top)
    RectangleShape line1;
    Vector2f direction1 = top - right;
    float length1 = std::sqrt(direction1.x * direction1.x + direction1.y * direction1.y);

    line1.setSize(Vector2f(length1, LINE_WIDTH));
    line1.setOrigin(0, LINE_WIDTH / 2);  // Центрируем по линии
    line1.setPosition(right);
    line1.setRotation(std::atan2(direction1.y, direction1.x) * 180 / 3.14159);  // Устанавливаем угол поворота
    line1.setFillColor(Color::Black);

    // Верхняя линия (top -> left)
    RectangleShape line2;
    Vector2f direction2 = left - top;
    float length2 = std::sqrt(direction2.x * direction2.x + direction2.y * direction2.y);
    line2.setSize(Vector2f(length2, LINE_WIDTH));
    line2.setOrigin(0, LINE_WIDTH / 2);
    line2.setPosition(top);
    line2.setRotation(std::atan2(direction2.y, direction2.x) * 180 / 3.14159);
    line2.setFillColor(Color::Black);

    // Левая линия (left -> bottom)
    RectangleShape line3;
    Vector2f direction3 = bottom - left;
    float length3 = std::sqrt(direction3.x * direction3.x + direction3.y * direction3.y);
    line3.setSize(Vector2f(length3, LINE_WIDTH));
    line3.setOrigin(0, LINE_WIDTH / 2);
    line3.setPosition(left);
    line3.setRotation(std::atan2(direction3.y, direction3.x) * 180 / 3.14159);
    line3.setFillColor(Color::Black);

    // Нижняя линия (bottom -> right)
    RectangleShape line4;
    Vector2f direction4 = right - bottom;
    float length4 = std::sqrt(direction4.x * direction4.x + direction4.y * direction4.y);
    line4.setSize(Vector2f(length4, LINE_WIDTH));
    line4.setOrigin(0, LINE_WIDTH / 2);
    line4.setPosition(bottom);
    line4.setRotation(std::atan2(direction4.y, direction4.x) * 180 / 3.14159);
    line4.setFillColor(Color::Black);

    // Рисуем все линии
    window.draw(line1);
    window.draw(line2);
    window.draw(line3);
    window.draw(line4);
}

// Функция для рисования прямоугольника
void drawRectangle(RenderWindow& window) {
    // Рассчитываем координаты и размеры прямоугольника
    float RECT_LEFT_X = -1.5f;  // Левая граница по оси X
    float RECT_RIGHT_X = 2.5f;  // Правая граница по оси X
    float RECT_BOTTOM_Y = 2.5f;  // Нижняя граница по оси Y
    float RECT_TOP_Y = 4.5f;     // Верхняя граница по оси Y

    float width = (RECT_RIGHT_X - RECT_LEFT_X) * GRID_CELL_SIZE;
    float height = (RECT_TOP_Y - RECT_BOTTOM_Y) * GRID_CELL_SIZE;

    // Левый верхний угол прямоугольника
    float leftX = RECT_LEFT_X * GRID_CELL_SIZE + 3250;
    float topY = 2900 - RECT_TOP_Y * GRID_CELL_SIZE;

    // Создаем прямоугольник
    RectangleShape rectangle(Vector2f(width, height));
    rectangle.setPosition(leftX, topY);
    rectangle.setFillColor(Color::Transparent);  // Отключаем заливку
    rectangle.setOutlineThickness(1);  // Толщина линии
    rectangle.setOutlineColor(Color::Black);

    window.draw(rectangle);  // Рисуем прямоугольник
}
float calculateX(float y) {
    return -1 * y * y + 5 * y - 4.25;
}

// Функция для рисования параболы с толстыми линиями
void drawParabola(RenderWindow& window) {
    // Начнем с y = -10 и будем идти вверх
    float y = -10.0f;
    float prevX = calculateX(y);
    float prevY = y;

    y += 0.1f;

    while (y <= 15.0f) {  // Будем рисовать параболу в диапазоне от -10 до 15 по оси Y
        // Вычисляем x для текущего y
        float x = calculateX(y);

        // Преобразуем координаты из математических в экранные
        float screenX1 = CENTER_X + prevX * GRID_CELL_SIZE;
        float screenY1 = CENTER_Y - prevY * GRID_CELL_SIZE;
        float screenX2 = CENTER_X + x * GRID_CELL_SIZE;
        float screenY2 = CENTER_Y - y * GRID_CELL_SIZE;

        // Если точка параболы выходит за пределы экрана по y или x, пропускаем эту итерацию
        if (screenY1 > GRID_HEIGHT || screenX1 < 0 || screenX1 > GRID_WIDTH) {
            prevX = x;
            prevY = y;
            y += 0.1f;
            continue;
        }

        // Создаем прямоугольник между двумя точками параболы
        RectangleShape lineSegment;
        Vector2f direction(screenX2 - screenX1, screenY2 - screenY1);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        lineSegment.setSize(Vector2f(length, LINE_WIDTH));
        lineSegment.setOrigin(0, LINE_WIDTH / 2);  // Центрируем по линии
        lineSegment.setPosition(screenX1, screenY1);
        lineSegment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);  // Устанавливаем угол поворота
        lineSegment.setFillColor(Color::Black);  // Цвет линии

        // Рисуем сегмент параболы
        window.draw(lineSegment);

        // Обновляем предыдущие точки
        prevX = x;
        prevY = y;

        // Увеличиваем y для следующей точки
        y += 0.1f;
    }
}

float calculateY(float x) {
    return -1 * x * x + 5 * x - 5.25;  // Уравнение параболы
}

// Функция для рисования параболы
void drawParabolaXY(RenderWindow& window) {
    // Создаем объект для хранения точек параболы
    VertexArray parabola(LinesStrip);

    // Начнем с x = -10 и будем идти вправо
    float x = -10.0f;
    float prevX = x;
    float prevY = calculateY(x);

    x += 0.1f;

    while (x <= 10.0f) {  // Будем рисовать параболу в диапазоне от -10 до 10 по оси X
        // Вычисляем y для текущего x
        float y = calculateY(x);

        // Преобразуем координаты из математических в экранные
        float screenX1 = CENTER_X + prevX * GRID_CELL_SIZE;
        float screenY1 = CENTER_Y - prevY * GRID_CELL_SIZE;
        float screenX2 = CENTER_X + x * GRID_CELL_SIZE;
        float screenY2 = CENTER_Y - y * GRID_CELL_SIZE;

        // Если точка параболы выходит за пределы экрана по y или x, пропускаем эту итерацию
        if (screenY1 > GRID_HEIGHT || screenX1 < 0 || screenX1 > GRID_WIDTH) {
            prevX = x;
            prevY = y;
            x += 0.1f;
            continue;
        }

        // Создаем прямоугольник между двумя точками параболы для имитации "толстой" линии
        RectangleShape lineSegment;
        Vector2f direction(screenX2 - screenX1, screenY2 - screenY1);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        lineSegment.setSize(Vector2f(length, LINE_WIDTH));
        lineSegment.setOrigin(0, LINE_WIDTH / 2);  // Центрируем по линии
        lineSegment.setPosition(screenX1, screenY1);
        lineSegment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);  // Устанавливаем угол поворота
        lineSegment.setFillColor(Color::Black);  // Цвет линии

        // Рисуем сегмент параболы
        window.draw(lineSegment);

        // Обновляем предыдущие точки
        prevX = x;
        prevY = y;

        // Увеличиваем x для следующей точки
        x += 0.1f;
    }
}

float calculateYForLine(float x) {
    return -0.33 * x - 0.2522;  // Уравнение линии
}

// Функция для рисования прямой линии на всём окне
float calculateYForFirstLine(float x) {
    return -0.33 * x - 0.2522;  // Уравнение первой линии
}

// Функция для рисования первой линии на всём окне
void drawFirstLine(RenderWindow& window) {
    // Получаем размеры окна
    Vector2u windowSize = window.getSize();

    // Начнем с крайних значений X, чтобы линия покрывала всё окно
    float startX = -static_cast<float>(windowSize.x) / GRID_CELL_SIZE;  // Минимальное значение X
    float endX = static_cast<float>(windowSize.x) / GRID_CELL_SIZE;     // Максимальное значение X

    // Рассчитаем Y для крайней левой и крайней правой точек
    float startY = calculateYForFirstLine(startX);
    float endY = calculateYForFirstLine(endX);

    // Преобразуем координаты из математических в экранные
    float screenX1 = CENTER_X + startX * GRID_CELL_SIZE;
    float screenY1 = CENTER_Y - startY * GRID_CELL_SIZE;
    float screenX2 = CENTER_X + endX * GRID_CELL_SIZE;
    float screenY2 = CENTER_Y - endY * GRID_CELL_SIZE;

    // Создаем прямоугольник между двумя точками для рисования толстой линии
    RectangleShape lineSegment;
    Vector2f direction(screenX2 - screenX1, screenY2 - screenY1);
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    lineSegment.setSize(Vector2f(length, LINE_WIDTH));
    lineSegment.setOrigin(0, LINE_WIDTH / 2);  // Центрируем по линии
    lineSegment.setPosition(screenX1, screenY1);
    lineSegment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);  // Устанавливаем угол поворота
    lineSegment.setFillColor(Color::Black);

    // Рисуем линию
    window.draw(lineSegment);
}


float calculateYForSecondLine(float x) {
    return 1.6 * x - 4.6;  // Уравнение второй линии
}

// Функция для рисования второй линии на всём окне
void drawSecondLine(RenderWindow& window) {
    // Получаем размеры окна
    Vector2u windowSize = window.getSize();

    // Начнем с крайних значений X, чтобы линия покрывала всё окно
    float startX = -static_cast<float>(windowSize.x) / GRID_CELL_SIZE;  // Минимальное значение X
    float endX = static_cast<float>(windowSize.x) / GRID_CELL_SIZE;     // Максимальное значение X

    // Рассчитаем Y для крайней левой и крайней правой точек
    float startY = calculateYForSecondLine(startX);
    float endY = calculateYForSecondLine(endX);

    // Преобразуем координаты из математических в экранные
    float screenX1 = CENTER_X + startX * GRID_CELL_SIZE;
    float screenY1 = CENTER_Y - startY * GRID_CELL_SIZE;
    float screenX2 = CENTER_X + endX * GRID_CELL_SIZE;
    float screenY2 = CENTER_Y - endY * GRID_CELL_SIZE;

    // Создаем прямоугольник между двумя точками для рисования толстой линии
    RectangleShape lineSegment;
    Vector2f direction(screenX2 - screenX1, screenY2 - screenY1);
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    lineSegment.setSize(Vector2f(length, LINE_WIDTH));
    lineSegment.setOrigin(0, LINE_WIDTH / 2);  // Центрируем по линии
    lineSegment.setPosition(screenX1, screenY1);
    lineSegment.setRotation(std::atan2(direction.y, direction.x) * 180 / 3.14159);  // Устанавливаем угол поворота
    lineSegment.setFillColor(Color::Black);

    // Рисуем линию
    window.draw(lineSegment);
}

void drawShapes(RenderWindow& window) {
    drawCircle(window);
    drawRhombus(window);
    drawRectangle(window);
    drawParabola(window);
    drawParabolaXY(window);
    drawFirstLine(window);
    drawSecondLine(window);
}

int main() {
    Font font;
    if (!font.loadFromFile("Seravek.otf")) {
        return -1;
    }

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), L"Новый проект", Style::Default);
    window.setVerticalSyncEnabled(true);

    View view(FloatRect(VIEW_START_X, VIEW_START_Y, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view);

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
                    view.zoom(0.99);
                else
                    view.zoom(1.01);
                window.setView(view);
            }
        }

        texture.loadFromImage(image);
        sprite.setTexture(texture);

        window.clear(Color::White);
        window.draw(sprite);
        drawCoordinateAxes(window);
        drawCoordinateLabels(window, font);
        drawGrid(window);
        drawShapes(window);

        window.display();
    }

    return 0;
}
