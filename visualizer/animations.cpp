#include "animations.h"
#include "galapagos.h"
#include "easing_equations.h"
#include "glm/gtx/fast_square_root.hpp"
#include <sstream>
#include <string>

namespace visualizer
{
  // todo: move this to the core?
  template< class T >
  static string toString(const T& in)
  {
    stringstream ss;
    ss << in;
    return ss.str();
  }
   
  void StartAnim::animate( const float& /* t */, AnimData * /* d */, IGame* /*game*/ )
  {
  }

  void DrawMap::animate( const float& t, AnimData * /*d*/, IGame* game )
  {
    int middleY = m_Map->GetHeight() / 2;
    
    bool lighting = game->options->getNumber("Enable Sun") > 0.0f;
    float color = linearTween(t,m_Map->GetPrevMapColor(),m_Map->GetMapColor() - m_Map->GetPrevMapColor(),1.0);
  
    for (int x = 0; x < m_Map->GetWidth(); x++)
    {
      for (int y = 0; y < m_Map->GetHeight(); y++)
      {
        const Map::Tile& tile = (*m_Map)(y,x);
        float d = 1.0f;
        
        if(lighting)
        {
          d = glm::fastInverseSqrt((float)((m_Map->GetxPos() - x)*(m_Map->GetxPos() - x)+(middleY-y)*(middleY-y)))*10.0f;
        }
      
        game->renderer->setColor( Color(color*d, color, 0.2f,1.0f ) );
          
        game->renderer->drawTexturedQuad( x, y, 1, 1, (tile.turn + 7) < game->timeManager->getTurn() ? "grass" : tile.texture );
      }
    }
    
  }
  
  void DrawPlant::animate( const float& t, AnimData* /*d*/, IGame* game )
  {
    game->renderer->setColor( Color( 1, 1, 1, 1 ) );
    game->renderer->drawTexturedQuad( m_Plant->x, m_Plant->y, 1, 1, "leaf" );
    game->renderer->drawText( m_Plant->x, m_Plant->y, "Roboto", toString(m_Plant->size), 3, IRenderer::Left);
  }
  
  void DrawCreature::animate(const float& t, AnimData* d, IGame* game )
  {
    Color color = m_Creature->owner == 0 ? Color( 0.8, 0.1, 0.1, 1 ) : Color( 0.1, 0.1, 0.8, 1 );
  	game->renderer->setColor( color );
  	
  	int frame = game->timeManager->getTurn() % 7;
  	
    game->renderer->drawAnimQuad( m_Creature->x, m_Creature->y, 1, 1, "creatureani" , frame);
    
  }
  
}
