#include "stdafx.h"
#include "GameObject.h"
#include "app.h"

using namespace App;

void GameObject::CheckForWrap(float& x, float& y, float radius)
{
	if (x + radius < 0)
		x = GetScreenWidth();

	if (x - radius > GetScreenWidth())
		x = 1;

	if (y + radius < 0)
		y = GetScreenHeight() - 1;

	if (y - radius > GetScreenHeight())
		y = 1;
}