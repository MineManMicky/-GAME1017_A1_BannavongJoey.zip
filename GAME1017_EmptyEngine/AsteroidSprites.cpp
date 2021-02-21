#include "AsteroidSprites.h"

#include "EventManager.h"
#include "SoundManager.h"
#include "MathManager.h"

Asteroid::Asteroid(SDL_Rect s, SDL_FRect d) : SpriteObject(s, d),
m_angle(0.0), m_radius(33.0)
{
	m_center = { (m_dst.x + m_dst.w / 2.0f), (m_dst.y + m_dst.h / 2.0f) };
	m_rotSpeed = (1.0 + rand() % 5) * (rand() % 2 * 2.0 - 1.0); // -1 or 1
	MAMA::SetDeltas(MAMA::Deg2Rad((rand() % 360) - 90.0), m_dx, m_dy, 2.0f, 2.0f);
}

void Asteroid::Update()
{
	// Rotate and move object.
	m_angle += m_rotSpeed;
	m_center.x += (float)m_dx;
	m_center.y += (float)m_dy;
	// Wrap on screen.
	if (m_center.x < -m_dst.w / 2) m_center.x = WIDTH + m_dst.w / 2;
	else if (m_center.x > WIDTH + m_dst.w / 2) m_center.x = 0 - m_dst.w / 2;
	else if (m_center.y < -m_dst.h / 2) m_center.y = HEIGHT + m_dst.h / 2;
	else if (m_center.y > HEIGHT + m_dst.h / 2) m_center.y = 0 - m_dst.h / 2;
	// Update dest rectangle.
	m_dst.x = m_center.x - m_dst.w / 2;
	m_dst.y = m_center.y - m_dst.h / 2;
}

void Asteroid::Render()
{
	SDL_SetTextureColorMod(TEMA::GetTexture("sprites"), 255-m_rMod, 255-m_gMod, 255-m_bMod);
	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("sprites"),
		&m_src, &m_dst, m_angle, nullptr, SDL_FLIP_NONE);
	SDL_SetTextureColorMod(TEMA::GetTexture("sprites"), 255, 255, 255); // Resetting it so not all sprites are tinted this way.
}

void Asteroid::SetColMods(Uint8 r, Uint8 g, Uint8 b)
{
	m_rMod = r; m_gMod = g; m_bMod = b;
}

AsteroidField::AsteroidField(unsigned int sz) :GameObject({ 0,0,0,0 }), m_size(sz)
{
	for (int i = 0; i < m_size; i++)
	{
		m_asteroids.push_back(new Asteroid({ 539, 0, 61, 66 },
			{ 25.0f + rand() % 901, (i % 2 == 0 ? 25.0f : 600.0f) + (rand() % 76), 
			61.0f, 66.0f }));
		m_asteroids.back()->SetColMods((rand() % 129), (rand() % 129), (rand() % 129));
	}
	m_asteroids.shrink_to_fit();
}

AsteroidField::~AsteroidField()
{
	for (auto a : m_asteroids)
	{
		delete a;
		a = nullptr; // ;)
	}
	m_asteroids.clear();
}

void AsteroidField::Update()
{
	for (const auto a : m_asteroids)
		a->Update();
}

void AsteroidField::Render()
{
	for (const auto a : m_asteroids)
		a->Render();
}

Bullet::Bullet(SDL_Rect s, SDL_FRect d, const char* key, const double angle):
	SpriteObject(s, d), m_key(key), m_angle(angle), m_radius(10),
	m_ctr(0), m_ctrMax(75)
{
	m_center = { (m_dst.x + m_dst.w / 2.0f), (m_dst.y + m_dst.h / 2.0f) };
	MAMA::SetDeltas(MAMA::Deg2Rad(m_angle - 90.0), m_dx, m_dy, 10.0f, 10.0f);
}

void Bullet::Update()
{
	// Move object.
	m_center.x += (float)m_dx;
	m_center.y += (float)m_dy;
	// Wrap on screen.
	if (m_center.x < -m_dst.w) m_center.x = WIDTH + m_dst.w / 2;
	else if (m_center.x > WIDTH + m_dst.w) m_center.x = 0 - m_dst.w / 2;
	else if (m_center.y < -m_dst.h) m_center.y = HEIGHT + m_dst.h / 2;
	else if (m_center.y > HEIGHT + m_dst.h) m_center.y = 0 - m_dst.h / 2;
	// Update dest rectangle.
	m_dst.x = m_center.x - m_dst.w / 2;
	m_dst.y = m_center.y - m_dst.h / 2;
	// Set bullet to be disabled.
	if (m_ctr++ >= m_ctrMax)
		m_enabled = false;
}

void Bullet::Render()
{
	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_key),
		&m_src, &m_dst, m_angle, nullptr, SDL_FLIP_NONE);
}

ShipAsteroids::ShipAsteroids(SDL_Rect s, SDL_FRect d) :
	AnimatedSpriteObject(s, d), m_state(STATE_IDLING), m_thrust(0.5),
	m_angle(270), m_velMax(6.0), m_radius(38), m_rotSpeed(5.0)
{
	m_dx = m_dy = m_velX = m_velY = 0.0;
	m_center = { (m_dst.x + m_dst.w / 2.0f), (m_dst.y + m_dst.h / 2.0f) };
	SetAnimation(1, 0, 1);
}

ShipAsteroids::~ShipAsteroids()
{
	SOMA::StopSound(15);
	for (auto a : m_bullets)
	{
		delete a;
		a = nullptr; // ;)
	}
	m_bullets.clear();
}


void ShipAsteroids::Update()
{
	// Rotate player. State-independent.
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
		m_angle -= m_rotSpeed;
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		m_angle += m_rotSpeed;
	// Spawn bullet.
	if (EVMA::MousePressed(1))
	{
		m_bullets.push_back(new Bullet({ 500, 0, 18, 24 },
			{ m_center.x - 9, m_center.y - 12, 18, 24 }, 
			"sprites", m_angle));
		m_bullets.shrink_to_fit();
		SOMA::PlaySound("fire");
	}
	// Checking animation states.
	switch (m_state)
	{
	case STATE_IDLING:
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) || EVMA::KeyPressed(SDL_SCANCODE_W))
		{
			m_state = STATE_MOVING;
			SetAnimation(2, 0, 4);
			SOMA::PlaySound("engines", -1, 15);
		}
		break;
	case STATE_MOVING:
		m_dx = cos(MAMA::Deg2Rad(m_angle - 90));
		m_dy = sin(MAMA::Deg2Rad(m_angle - 90));
		// Update velocities.
		m_velX += (m_dx * m_thrust);
		m_velY += (m_dy * m_thrust);
		// Clamp velocities.
		m_velX = min(max(m_velX, -(m_velMax * fabs(m_dx))), (m_velMax * fabs(m_dx)));
		m_velY = min(max(m_velY, -(m_velMax * fabs(m_dy))), (m_velMax * fabs(m_dy)));
		// Check for idle transition.
		if (!EVMA::KeyHeld(SDL_SCANCODE_SPACE) && !EVMA::KeyHeld(SDL_SCANCODE_W))
		{
			m_state = STATE_IDLING;
			SetAnimation(1, 0, 1);
			SOMA::StopSound(15);
		}
		break;
	}
	// Apply drag and finish movement.
	m_velX *= 0.985;
	m_velY *= 0.985;
	m_center.x += (float)m_velX;
	m_center.y += (float)m_velY;
	// Wrap on screen.
	if (m_center.x < -m_dst.w / 2) m_center.x = WIDTH + m_dst.w / 2;
	else if (m_center.x > WIDTH + m_dst.w / 2) m_center.x = 0 - m_dst.w / 2;
	if (m_center.y < -m_dst.h / 2) m_center.y = HEIGHT + m_dst.h / 2;
	else if (m_center.y > HEIGHT + m_dst.h / 2) m_center.y = 0 - m_dst.h / 2;
	// Update dest rectangle.
	m_dst.x = m_center.x - m_dst.w / 2;
	m_dst.y = m_center.y - m_dst.h / 2;
	// Invoke the animation.
	Animate();
	// Update bullets.
	for (unsigned int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i]->Update();
		if (!m_bullets[i]->GetEnabled())
		{
			delete m_bullets[i];
			m_bullets[i] = nullptr; 
			m_bullets.erase(m_bullets.begin() + i);
			m_bullets.shrink_to_fit();
		}
	}
}

void ShipAsteroids::Render()
{
	for (const auto b : m_bullets)
		b->Render();
	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("sprites"),
		&m_src, &m_dst, m_angle, nullptr, SDL_FLIP_NONE);
}