#include "ScatteredPointsBrush.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"
#include <GL/gl.h>

ScatteredPointsBrush::ScatteredPointsBrush(ImpressionistDoc* pDoc, char* name)
    :ImpBrush(pDoc, name) {
}

char* ScatteredPointsBrush::BrushName(void) {
    return "Scattered Points Brush";
}

void ScatteredPointsBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    glPointSize(1.0);
    BrushMove(source, target);

}

void ScatteredPointsBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    if (pDoc == NULL) return;
    int size = pDoc->getSize();
    int radius = size / 2;

    glBegin(GL_POINTS);

    for (int i = 0; i < size * size; i++)
    {
        int dx = rand() % size - radius;
        int dy = rand() % size - radius;

        if (dx * dx + dy * dy <= radius * radius)
        {
            Point newSource(source.x + dx, source.y + dy);
            Point newTarget(target.x + dx, target.y + dy);

            GLubyte* color = pDoc->GetOriginalPixel(newSource);

            glColor4ub(color[0], color[1], color[2], 255);
            glVertex2d(newTarget.x, newTarget.y);
        }
    }

    glEnd();
}

void ScatteredPointsBrush::BrushEnd(const Point source, const Point target) {
}