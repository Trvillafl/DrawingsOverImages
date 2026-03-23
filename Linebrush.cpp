#include "Linebrush.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"
#include <GL/gl.h>
#include <iostream>
#include <cmath>

Linebrush::Linebrush(ImpressionistDoc* pDoc, char* name)
	:ImpBrush(pDoc, name) {
}

char* Linebrush::BrushName(void) {
	return "Line Brush";
}

void Linebrush::BrushBegin(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	glPointSize(1.0);
	BrushMove(source, target);

}

void Linebrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	if (pDoc == NULL) return;
    float angle = 0;
    int mode = pDoc->m_pUI->getDirectionType();

    if (mode == 0) // Slider
    {
        angle = pDoc->m_pUI->getLineAngle();
    }
    else if (mode == 2) // Cursor Movement
    {
        angle = atan2(target.y - source.y, target.x - source.x) * 180.0 / M_PI;
    }
    else if (mode == 3) // Gradient
    {
        GLubyte* pixel = pDoc->GetOriginalPixel(source);
        float gray = 0.299f * pixel[0] + 0.587f * pixel[1] + 0.114f * pixel[2];
        float gx = pDoc->GetOriginalPixel(source.x + 1, source.y)[0] - pDoc->GetOriginalPixel(source.x - 1, source.y)[0];
        float gy = pDoc->GetOriginalPixel(source.x, source.y + 1)[0] - pDoc->GetOriginalPixel(source.x, source.y - 1)[0];

        angle = atan2(gy, gx) * 180.0 / M_PI + 90;
    }
    int size = pDoc->getSize();
    int width = pDoc->m_pUI->getLineWidth();

    float rad = angle * M_PI / 180.0f;
    float dx = cos(rad);
    float dy = sin(rad);

    glBegin(GL_POINTS);

    for (int i = -size / 2; i <= size / 2; i++)
    {
        for (int w = -width / 2; w <= width / 2; w++)
        {
            SetColor(source);
            glVertex2d(target.x + i * dx - w * dy, target.y + i * dy + w * dx);
        }
    }

    glEnd();
}

void Linebrush::BrushEnd(const Point source, const Point target){
}

