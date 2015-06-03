#include "BsRect2I.h"
#include "BsVector2I.h"
#include "BsMatrix4.h"
#include "BsMath.h"

namespace BansheeEngine
{
	const Rect2I Rect2I::EMPTY = Rect2I();

	Rect2I::Rect2I()
		:x(0), y(0), width(0), height(0)
	{ }

	Rect2I::Rect2I(int _x, int _y, int _width, int _height)
		:x(_x), y(_y), width(_width), height(_height)
	{ }

	bool Rect2I::contains(const Vector2I& point) const
	{
		if(point.x >= x && point.x < (x + width))
		{
			if(point.y >= y && point.y < (y + height))
				return true;
		}

		return false;
	}

	bool Rect2I::overlaps(const Rect2I& other) const
	{
		INT32 otherRight = other.x + other.width;
		INT32 myRight = x + width;

		INT32 otherBottom = other.y + other.height;
		INT32 myBottom = y + height;

		if(x < otherRight && myRight > other.x &&
			y < otherBottom && myBottom > other.y)
			return true;

		return false;
	}

	void Rect2I::encapsulate(const Rect2I& other)
	{
		int myRight = x + width;
		int myBottom = y + height;
		int otherRight = other.x + other.width;
		int otherBottom = other.y + other.height;

		if(other.x < x)
			x = other.x;

		if(other.y < y)
			y = other.y;

		if(otherRight > myRight)
			width = otherRight - x;
		else
			width = myRight - x;

		if(otherBottom > myBottom)
			height = otherBottom - y;
		else
			height = myBottom - y;
	}

	void Rect2I::clip(const Rect2I& clipRect)
	{
		int newLeft = std::max(x, clipRect.x);
		int newTop = std::max(y, clipRect.y);

		int newRight = std::min(x + width, clipRect.x + clipRect.width);
		int newBottom = std::min(y + height, clipRect.y + clipRect.height);

		x = std::min(newLeft, newRight);
		y = std::min(newTop, newBottom);
		width = std::max(0, newRight - newLeft);
		height = std::max(0, newBottom - newTop);
	}

	void Rect2I::transform(const Matrix4& matrix)
	{
		Vector4 verts[4];
		verts[0] = Vector4((float)x, (float)y, 0.0f, 1.0f);
		verts[1] = Vector4((float)x + width, (float)y, 0.0f, 1.0f);
		verts[2] = Vector4((float)x, (float)y + height, 0.0f, 1.0f);
		verts[3] = Vector4((float)x + width, (float)y + height, 0.0f, 1.0f);

		for(UINT32 i = 0; i < 4; i++)
			verts[i] = matrix.multiply(verts[i]);

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();

		for(UINT32 i = 0; i < 4; i++)
		{
			if(verts[i].x < minX)
				minX = verts[i].x;

			if(verts[i].y < minY)
				minY = verts[i].y;

			if(verts[i].x > maxX)
				maxX = verts[i].x;

			if(verts[i].y > maxY)
				maxY = verts[i].y;
		}

		x = Math::floorToInt(minX);
		y = Math::floorToInt(minY);
		width = Math::ceilToInt(maxX) - x;
		height = Math::ceilToInt(maxY) - y;
	}
}