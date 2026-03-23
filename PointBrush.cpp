//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointbrush.h"

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void PointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	if (!pDoc) return;
	float alpha = pDoc->m_pUI->getAlpha();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLubyte* color = pDoc->GetOriginalPixel(source);

	glBegin( GL_POINTS );
	glColor4f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, alpha);
	glVertex2d( target.x, target.y );
	glEnd();

	glDisable(GL_BLEND);

}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

