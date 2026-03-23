// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "Linebrush.h"
#include "ScatteredPointsBrush.h"
#include "CircleBrush.h"
#include "ScatteredLinesBrush.h"
#include "ScatteredCirclesBrush.h"
#include <iostream>
#include <cmath>

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;

	m_nLineWidth = 5;
	m_nAngle = 0;
	m_nAlpha = 1.0f;
	m_nDirectionMode = DIR_SLIDER;

	Point m_lastMousePoint;
	Point m_rightClickStart;
	Point m_rightClickEnd;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );
	
	ImpBrush::c_pBrushes[BRUSH_LINES] = new Linebrush(this, "Lines");

	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS] = new ScatteredPointsBrush(this, "Scattered Points");

	ImpBrush::c_pBrushes[BRUSH_CIRCLES] = new CircleBrush(this, "Circles");

	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES] = new ScatteredLinesBrush(this, "Scattered Lines");

	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES] = new ScatteredCirclesBrush(this, "Scattered Circles");
	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	
	
	

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

void ImpressionistDoc::applyGaussianBlur()
{
	if (!m_ucBitmap) return;

	unsigned char* temp =
		new unsigned char[m_nWidth * m_nHeight * 3];

	float kernel[3][3] = {
		{1,2,1},
		{2,4,2},
		{1,2,1}
	};

	for (int y = 1; y < m_nHeight - 1; y++)
	{
		for (int x = 1; x < m_nWidth - 1; x++)
		{
			float r = 0, g = 0, b = 0;

			for (int ky = -1; ky <= 1; ky++)
				for (int kx = -1; kx <= 1; kx++)
				{
					GLubyte* p =
						GetOriginalPixel(x + kx, y + ky);
					float w = kernel[ky + 1][kx + 1];
					r += p[0] * w;
					g += p[1] * w;
					b += p[2] * w;
				}

			int idx = (y * m_nWidth + x) * 3;
			temp[idx] = r / 16;
			temp[idx + 1] = g / 16;
			temp[idx + 2] = b / 16;
		}
	}

	delete[] m_ucBitmap;
	m_ucBitmap = temp;

	m_pUI->m_origView->refresh();
}

double ImpressionistDoc::computeGradientDirection(int x, int y)
{
	if (x <= 0) x = 1;
	if (x >= m_nWidth - 1) x = m_nWidth - 2;
	if (y <= 0) y = 1;
	if (y >= m_nHeight - 1) y = m_nHeight - 2;

	auto gray = [&](int px, int py)
		{
			GLubyte* p = GetOriginalPixel(px, py);
			return 0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2];
		};

	double gx = gray(x + 1, y) - gray(x - 1, y);
	double gy = gray(x, y + 1) - gray(x, y - 1);

	double theta = atan2(gy, gx);

	return theta + M_PI / 2.0;  // perpendicular
}

double ImpressionistDoc::getStrokeDirection(Point source, Point target)
{
	switch (m_nDirectionMode)
	{
	case DIR_SLIDER:
		return m_nAngle * M_PI / 180.0;

	case DIR_MOUSE:
	{
		double dx = m_rightClickEnd.x - m_rightClickStart.x;
		double dy = m_rightClickEnd.y - m_rightClickStart.y;
		return atan2(dy, dx);
	}

	case DIR_MOVEMENT:
	{
		double dx = target.x - m_lastMousePoint.x;
		double dy = target.y - m_lastMousePoint.y;

		m_lastMousePoint = target;
		return atan2(dy, dx);
	}

	case DIR_GRADIENT:
		return computeGradientDirection(source.x, source.y);

	default:
		return 0;
	}
}
