#include "Primitives.h"
#include "Engine.h"

void Image::Render()
{
	SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_key), &m_src, &m_dst);
}

void Line::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderDrawLineF(Engine::Instance().GetRenderer(), m_start.x, m_start.y, m_end.x, m_end.y);
}

void Rectangle::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	if (m_fill)
		SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_dst);
	else
		SDL_RenderDrawRectF(Engine::Instance().GetRenderer(), &m_dst);
}
