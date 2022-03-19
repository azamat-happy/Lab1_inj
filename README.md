# Lab1_inj
Первая лабораторная работа по компьютерной графике.
### Выполнил: Шарипов Азамат
### Группа: МО-221
# Было сделано:
## Установка необходимых библиотек OpegGL (glm, freeglut, glew)
## Создание окна
    // Инициализация glut
    glutInit(&argc, argv);
    //двойная буферизация
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    //задание размера окна
    glutInitWindowSize(1024, 768);
    //задание положения позиции окна
    glutInitWindowPosition(100, 100);
    //создание окна приложения с названием "Tutorial 01"
    glutCreateWindow("Tutorial 01");
    
    //очищение окна
    glClear(GL_COLOR_BUFFER_BIT);
    // Меняем фоновый буфер и буфер кадра местами
    glutSwapBuffers();
    
    //Этот вызов передаёт контроль GLUT'у, который теперь начнёт свой собственный цикл
    glutMainLoop();

## Рисование точки
###### Объявление глобальной переменной
    GLuint VBO;
###### main:
    // Инициализируем glew
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    //Создаем массив типа glm::vec3
    glm::vec3  Vertices[1];
    //задаём XYZ по нулям
    Vertices[0] = glm::vec3 (0.0f, 0.0f, 0.0f);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //наполняем объект данными
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    // Разрешение использования каждого атрибута вершины
    glEnableVertexAttribArray(0);
    // Обратно привязываем буфер, приготавливая для отрисовки
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Этот вызов говорит конвейеру как воспринимать данные внутри буфера
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    //вызвали функцию для отрисовки
    glDrawArrays(GL_POINTS, 0, 1);
    // Отключение каждого атрибута вершины
    glDisableVertexAttribArray(0);
## Рисование треугольника
###### В коде "Рисование точки":
    //увеличиваем массив, чтобы он мог содержать три вершины
    glm::vec3 Vertices[3];
    Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    Vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
    Vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);
    
    //принимаем 3 вершины вместо
    glDrawArrays(GL_TRIANGLES, 0, 3);
# Итог
Я научился создавать окно и рисовать простейшие объекты в OpenGL, а также писать отчеты в README!
