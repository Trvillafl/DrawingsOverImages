#include "ScatteredLinesBrush.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"
#include <cstdlib>

ScatteredLinesBrush::ScatteredLinesBrush(ImpressionistDoc* pDoc, char* name)
    :ImpBrush(pDoc, name) {
}

char* ScatteredLinesBrush::BrushName(void) {
    return "Scattered Lines Brush";
}

void ScatteredLinesBrush::BrushBegin(const Point source, const Point target) {
    BrushMove(source, target);

}

void ScatteredLinesBrush::BrushMove(const Point source, const Point target) {
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
        glBegin(GL_LINES);
        glVertex2d(target.x, target.y);
        glVertex2d(target.x + dx, target.y + dy);
        glEnd();

    }

    glDisable(GL_BLEND);
}

void ScatteredLinesBrush::BrushEnd(const Point source, const Point target) {
}