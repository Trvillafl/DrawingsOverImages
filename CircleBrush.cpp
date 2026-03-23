#include "CircleBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <cstdlib>
#include <cmath>

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name)
    :ImpBrush(pDoc, name) {
}

char* CircleBrush::BrushName(void) {
    return "Circle Brush";
}

void CircleBrush::BrushBegin(const Point source, const Point target) {
    BrushMove(source, target);

}

void CircleBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    if (pDoc == NULL) return;
    int size = pDoc->getSize();

    GLubyte* color = pDoc->GetOriginalPixel(source);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(color[0], color[1], color[2], 255);

    glBegin(GL_POINTS);

    for (int i = 0; i < 20; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / 20;
        float x = target.x + size * cosf(theta);
        float y = target.y + size * sinf(theta);
        glVertex2d(x, y);
    }
    glEnd();

    glDisable(GL_BLEND);
}

void CircleBrush::BrushEnd(const Point source, const Point target) {
}