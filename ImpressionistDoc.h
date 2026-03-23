// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	char*	getImageName();					// get the current image name

	enum DirectionMode {
		DIR_SLIDER = 0,
		DIR_MOUSE,
		DIR_MOVEMENT,
		DIR_GRADIENT
	};


	

// Attributes
public:

	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// Size of the brush.
	int m_nSize;							

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  

	void setLineWidth(int w) { m_nLineWidth = w; }
	void setAngle(int a) { m_nAngle = a; }
	void setAlpha(float a) { m_nAlpha = a; }
	void setDirectionMode(int d) { m_nDirectionMode = d; }

	int getLineWidth() { return m_nLineWidth; }
	int getAngle() { return m_nAngle; }
	float getAlpha() { return m_nAlpha; }
	int getDirectionMode() { return m_nDirectionMode; }
	double getStrokeDirection(Point source, Point target);
	double computeGradientDirection(int x, int y);

	//added more.
	void applyGaussianBlur();


private:
	int m_nLineWidth;
	int m_nAngle;
	float m_nAlpha;
	int m_nDirectionMode;

	Point m_lastMousePoint;
	Point m_rightClickStart;
	Point m_rightClickEnd;
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
