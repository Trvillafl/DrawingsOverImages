#include "ScatteredCirclesBrush.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"
#include <cstdlib>
#include <cmath>

ScatteredCirclesBrush::ScatteredCirclesBrush(ImpressionistDoc* pDoc, char* name)
    :ImpBrush(pDoc, name) {
}

char* ScatteredCirclesBrush::BrushName(void) {
    return "Scattered Circles Brush";
}

void ScatteredCirclesBrush::BrushBegin(const Point source, const Point target) {
    BrushMove(source, target);

}

void ScatteredCirclesBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    if (pDoc == NULL) return;
    int size = pDoc->getSize();

    GLubyte* color = pDoc->GetOriginalPixel(source);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(color[0], color[1], color[2], 255);

    for (int i = 0; i < 5; i++) {
        float dx = (rand() % size) - size / 2;
        float dy = (rand() % size) - size / 2;
        glBegin(GL_POLYGON);
        for (int j = 0; j < 20; j++) {
            float theta = 2.0f * 3.1415926f * j / 20;
            float x = target.x + dx + size * 0.3f * cosf(theta);
            float y = target.y + dy + size * 0.3f * sinf(theta);
            glVertex2d(x, y);
        }
        glEnd();

    }

    glDisable(GL_BLEND);
}

void ScatteredCirclesBrush::BrushEnd(const Point source, const Point target) {
}