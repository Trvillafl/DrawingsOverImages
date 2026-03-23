#pragma once
#ifndef LINEBRUSH_H
#define LINEBRUSH_H

#include "ImpBrush.h"

class Linebrush : public ImpBrush
{
public:
	Linebrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif
