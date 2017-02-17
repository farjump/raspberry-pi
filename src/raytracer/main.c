#include "Raytracing.h"
#include "VC.h"

T_RAYT_LIGHT A_LIGHT[] =
{
  {
    /* S_ORIGIN */
    {
      0.0,
      6.0,
      6.0
    },
    /* COLOR */
    {
      0.80,
      0.80,
      0.80
    }
  },
  {
    /* S_ORIGIN */
    {
      20.0,
      10.0,
      10.0
    },
    /* COLOR */
    {
      0.50,
      0.50,
      0.50
    }
  },
};


T_RAYT_SPHERE A_SPHERE[] =
{
  {
    /* S_ORIGIN */
    {
      15.0,
      0.0,
      0.0
    },
    /* RADIUS */
    1.0,
    /* PROPERTY */
    {
      /* ambiant coef */
      {
        255/255.0,
        0/255.0,
        0/255.0,
      },
      {
        /* diffusion coef */
        255/255.0,
        0/255.0,
        0/255.0,

      },
      /* reflexion coef */
      {
        1.0,
        1.0,
        1.0,
      },
      /* Phong  coef */
      {
        1.0,
        1.0,
        1.0,
      },
      5
    }
  },

  {
    /* S_ORIGIN */
    {
      12.0,
      -1.0,
      2.0
    },
    /* RADIUS */
    0.7,
    /* PROPERTY */
    {
      /* ambiant coef */
      {
        0/255.0,
        0/255.0,
        255/255.0,
      },
      {
        /* diffusion coef */
        0/255.0,
        0/255.0,
        255/255.0,

      },
      /* reflexion coef */
      {
        0.2,
        0.2,
        0.2,
      },
      /* Phong  coef */
      {
        0.0,
        0.0,
        0.0,
      },
      8
    }
  },

  {
    /* S_ORIGIN */
    {
      12.0,
      1.0,
      1.0
    },
    /* RADIUS */
    0.5,
    /* PROPERTY */
    {
      /* ambiant coef */
      {
        0/255.0,
        255/255.0,
        0/255.0,
      },
      {
        /* diffusion coef */
        0/255.0,
        255/255.0,
        0/255.0,
      },
      /* reflexion coef */
      {
        0.0,
        0.0,
        0.0,
      },
      /* Phong  coef */
      {
        1.0,
        1.0,
        1.0,
      },
      3
    },

  }
};


T_RAYT_WORLD  S_WORLD =
{
  /* S_EYE */
  {
    0.0,
    0.0,
    0.0
  },

  /* S_WINDOW */
  {
    /* S_ORIGIN */
    {
      10.0,
      -2.0,
      -2.0
    },
    /* S_HORIZONTAL_VECTOR */
    {
      0.0,
      0.0,
      4.0
    },
    /* S_VERTICAL_VECTOR */
    {
      0.0,
      4.0,
      0.0
    }
  },

  /* S_AMBIANT_LIGHT */
  {
    0.4,
    0.4,
    0.4
  },

  /* NB LIGHT */
  sizeof(A_LIGHT)/sizeof(T_RAYT_LIGHT),

  /* P_LIGHT */
  A_LIGHT,

  /* NB SPHERE */
  sizeof(A_SPHERE)/sizeof(T_RAYT_SPHERE),

  /* P_SPHERE */
  A_SPHERE,
};


#define K_Z_BLUE_MIN  ((T_FLOAT)-2.0)
#define K_Z_BLUE_MAX  ((T_FLOAT) 2.0)
#define K_Z_GREEN_MIN ((T_FLOAT)-1.0)
#define K_Z_GREEN_MAX ((T_FLOAT) 1.0)

T_UINT32  I_RESOLUTION_FACTOR = 2;

void main()
{
  T_UINT32 I_H = I_RESOLUTION_FACTOR*320;
  T_UINT32 I_V = I_RESOLUTION_FACTOR*240;

  T_FLOAT  F_Z_BLUE = K_Z_BLUE_MAX;
  T_FLOAT  F_Z_GREEN = K_Z_GREEN_MAX;
  T_UINT32 B_BLUE_INC = 0;
  T_UINT32 B_GREEN_INC = 0;

  VC_init(I_H,I_V);

  while (1)
  {
    /* set sphere Z position */
    S_WORLD.P_SPHERE[1].S_ORIGIN.F_Z =  F_Z_BLUE;
    S_WORLD.P_SPHERE[2].S_ORIGIN.F_Z =  F_Z_GREEN;

    /* render image */
    RAYT_RENDER(&S_WORLD,4,I_H,I_V);


    /* modify sphere position */
    if (B_BLUE_INC == 0)
    {
      F_Z_BLUE -= 0.1;
      if (F_Z_BLUE <  K_Z_BLUE_MIN )
      {
        B_BLUE_INC = 1;
      }
    }
    else
    {
      F_Z_BLUE += 0.1;
      if (F_Z_BLUE >  K_Z_BLUE_MAX )
      {
        B_BLUE_INC = 0;
      }
    }

    if (B_GREEN_INC == 0)
    {
      F_Z_GREEN -= 0.1;
      if (F_Z_GREEN <  K_Z_GREEN_MIN )
      {
        B_GREEN_INC = 1;
      }
    }
    else
    {
      F_Z_GREEN += 0.1;
      if (F_Z_GREEN >  K_Z_GREEN_MAX )
      {
        B_GREEN_INC = 0;
      }
    }

  };
  return;
}
