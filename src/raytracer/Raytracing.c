#include "Raytracing.h"
#include <math.h>
#include <float.h>
#include "VC.h"

T_RAYT_WORLD *P_RAYT_WORLD;

void RAYT_GET_3D(
  T_RAYT_RAY  *P_RAY,
  T_FLOAT      F_PARAMETER,
  T_3D        *P_3D)
{
  P_3D->F_X = P_RAY->S_ORIGIN.F_X + P_RAY->S_VECTOR.F_X * F_PARAMETER;// - 0.1;
  P_3D->F_Y = P_RAY->S_ORIGIN.F_Y + P_RAY->S_VECTOR.F_Y * F_PARAMETER;// - 0.1;
  P_3D->F_Z = P_RAY->S_ORIGIN.F_Z + P_RAY->S_VECTOR.F_Z * F_PARAMETER;// - 0.1;

  return;
}


void RAYT_GET_FIRST_INTERSECTION_WITH_SPHERE(
  T_RAYT_RAY        *P_RAY,
  T_RAYT_SPHERE     *P_SPHERE,
  T_UINT32          *P_NB_INTERSECTION,
  T_FLOAT           *P_INTERSECTION_PARAM,
  T_3D              *P_INTERSECTION,
  T_3D              *P_NORMAL_VECTOR,
  T_RAYT_PROPERTY  **P_PROPERTY)
{
  T_FLOAT           A,B,C;
  T_3D              S_VECTOR;
  T_FLOAT           F_DESCRIMINANT;
  T_FLOAT           F_INTERSECTION;
  T_FLOAT           F_INTERSECTION0;
  T_FLOAT           F_INTERSECTION1;
  T_UINT32          I_NB_INTERSECTION;
  T_3D              S_INTERSECTION;

  S_VECTOR = P_RAY->S_VECTOR;
  A = 1.0;   /*because RAY vector is normalized */
  B = 2.0 * ( P_RAY->S_VECTOR.F_X *(P_RAY->S_ORIGIN.F_X - P_SPHERE->S_ORIGIN.F_X) +
              P_RAY->S_VECTOR.F_Y *(P_RAY->S_ORIGIN.F_Y - P_SPHERE->S_ORIGIN.F_Y) +
              P_RAY->S_VECTOR.F_Z *(P_RAY->S_ORIGIN.F_Z - P_SPHERE->S_ORIGIN.F_Z) );
  C =( (P_RAY->S_ORIGIN.F_X - P_SPHERE->S_ORIGIN.F_X) * (P_RAY->S_ORIGIN.F_X - P_SPHERE->S_ORIGIN.F_X) +
       (P_RAY->S_ORIGIN.F_Y - P_SPHERE->S_ORIGIN.F_Y) * (P_RAY->S_ORIGIN.F_Y - P_SPHERE->S_ORIGIN.F_Y) +
       (P_RAY->S_ORIGIN.F_Z - P_SPHERE->S_ORIGIN.F_Z) * (P_RAY->S_ORIGIN.F_Z - P_SPHERE->S_ORIGIN.F_Z) -
       (P_SPHERE->F_RADIUS * P_SPHERE->F_RADIUS) );

  F_DESCRIMINANT = B*B - 4.0*A*C;
  *P_PROPERTY = &(P_SPHERE->S_PROPERTY);

  I_NB_INTERSECTION = 0;
  F_INTERSECTION = 0;

  if (F_DESCRIMINANT < 0.0)
  {
    I_NB_INTERSECTION = 0;
  }
  else if (F_DESCRIMINANT == 0.0)
  {
    F_INTERSECTION = -B / 2.0;

    if (F_INTERSECTION < 0.0)
    {
      I_NB_INTERSECTION = 0;
    }
    else
    {
      I_NB_INTERSECTION = 1;
    }
  }
  else if (F_DESCRIMINANT > 0.0)
  {
    F_INTERSECTION0 = (-B - sqrt(F_DESCRIMINANT) ) / 2.0;
    F_INTERSECTION1 = (-B + sqrt(F_DESCRIMINANT) ) / 2.0;
    if ( (F_INTERSECTION0 < 0.0) && (F_INTERSECTION1 < 0.0))
    {
      I_NB_INTERSECTION = 0;
    }
    else if ( (F_INTERSECTION0 < 0.0) && (F_INTERSECTION1 >= 0.0))
    {
      I_NB_INTERSECTION = 1;
      F_INTERSECTION = F_INTERSECTION1;
    }
    else if ( (F_INTERSECTION0 >= 0.0) && (F_INTERSECTION1 < 0.0))
    {
      I_NB_INTERSECTION = 1;
      F_INTERSECTION = F_INTERSECTION0;

    }
    else if ( (F_INTERSECTION0 >= 0.0) && (F_INTERSECTION1 >= 0.0))
    {
      I_NB_INTERSECTION = 1;
      if (F_INTERSECTION0 < F_INTERSECTION1)
      {
        F_INTERSECTION = F_INTERSECTION0;
      }
      else
      {
        F_INTERSECTION = F_INTERSECTION1;
      }
    }
  }

  *P_NB_INTERSECTION = I_NB_INTERSECTION;
  *P_INTERSECTION_PARAM = F_INTERSECTION;

  if (I_NB_INTERSECTION != 0)
  {
    RAYT_GET_3D(P_RAY,F_INTERSECTION,&S_INTERSECTION);
    S_VECTOR.F_X = S_INTERSECTION.F_X - P_SPHERE->S_ORIGIN.F_X;
    S_VECTOR.F_Y = S_INTERSECTION.F_Y - P_SPHERE->S_ORIGIN.F_Y;
    S_VECTOR.F_Z = S_INTERSECTION.F_Z - P_SPHERE->S_ORIGIN.F_Z;
    RAYT_NORMALIZE_VECTOR(S_VECTOR,P_NORMAL_VECTOR);
    *P_INTERSECTION = S_INTERSECTION;
  }
  return;
}


void RAYT_GET_PRIMARY_RAY(
  T_UINT32      I_HORIZONTAL_PIXEL,
  T_UINT32      I_VERTICAL_PIXEL,
  T_UINT32      I_NB_HORIZONTAL_PIXEL,
  T_UINT32      I_NB_VERTICAL_PIXEL,
  T_RAYT_RAY   *P_RAY)
{
  T_3D   S_VECTOR,S_NORMALIZED_VECTOR;
  T_RAYT_WINDOW   * P_WINDOW;

  P_RAY->S_ORIGIN = P_RAYT_WORLD->S_EYE;

  P_WINDOW = &(P_RAYT_WORLD->S_WINDOW);
  S_VECTOR.F_X = P_WINDOW->S_ORIGIN.F_X +
    ( (I_HORIZONTAL_PIXEL * (P_WINDOW->S_HORIZONTAL_VECTOR.F_X)) / (I_NB_HORIZONTAL_PIXEL - 1)) +
    ( (I_VERTICAL_PIXEL * (P_WINDOW->S_VERTICAL_VECTOR.F_X)) / (I_NB_VERTICAL_PIXEL - 1)) -
    P_RAYT_WORLD->S_EYE.F_X;
  S_VECTOR.F_Y = P_WINDOW->S_ORIGIN.F_Y +
    ( (I_HORIZONTAL_PIXEL * (P_WINDOW->S_HORIZONTAL_VECTOR.F_Y)) / (I_NB_HORIZONTAL_PIXEL - 1)) +
    ( (I_VERTICAL_PIXEL * (P_WINDOW->S_VERTICAL_VECTOR.F_Y)) / (I_NB_VERTICAL_PIXEL - 1)) -
    P_RAYT_WORLD->S_EYE.F_Y;
  S_VECTOR.F_Z = P_WINDOW->S_ORIGIN.F_Z +
    ( (I_HORIZONTAL_PIXEL * (P_WINDOW->S_HORIZONTAL_VECTOR.F_Z)) / (I_NB_HORIZONTAL_PIXEL - 1)) +
    ( (I_VERTICAL_PIXEL * (P_WINDOW->S_VERTICAL_VECTOR.F_Z)) / (I_NB_VERTICAL_PIXEL - 1)) -
    P_RAYT_WORLD->S_EYE.F_Z;

  RAYT_NORMALIZE_VECTOR(S_VECTOR,&S_NORMALIZED_VECTOR);
  P_RAY->S_VECTOR = S_NORMALIZED_VECTOR;
  return;
}

void RAYT_GET_RGB_FROM_LIGHT(
  T_PTR           P_OBJECT,
  T_3D           *P_ORIGIN,
  T_RAYT_LIGHT   *P_LIGHT,
  T_3D           *P_VECTOR,
  T_RAYT_RGB     *P_RGB)
{

  T_RAYT_RAY       S_RAY;
  T_3D             S_VECTOR;
  T_BOOLEAN        B_INTERSECTION;
  T_UINT32          I_INDEX;
  T_UINT32          I_TMP_NB_INTERSECTION;
  T_FLOAT           F_TMP_INTERSECTION;
  T_3D              S_TMP_INTERSECTION;
  T_3D              S_TMP_VECTOR;
  T_RAYT_PROPERTY  *P_TMP_PROPERTY;


  S_RAY.S_ORIGIN = *P_ORIGIN;
  S_VECTOR.F_X = P_LIGHT->S_ORIGIN.F_X - P_ORIGIN->F_X;
  S_VECTOR.F_Y = P_LIGHT->S_ORIGIN.F_Y - P_ORIGIN->F_Y;
  S_VECTOR.F_Z = P_LIGHT->S_ORIGIN.F_Z - P_ORIGIN->F_Z;
  RAYT_NORMALIZE_VECTOR(S_VECTOR,&(S_RAY.S_VECTOR));

  *P_VECTOR = S_RAY.S_VECTOR;

  B_INTERSECTION = K_FALSE;

  for (I_INDEX = 0; (I_INDEX < P_RAYT_WORLD->I_NB_SPHERE) && (B_INTERSECTION == K_FALSE) ; I_INDEX ++)
  {
    if (P_OBJECT != &(P_RAYT_WORLD->P_SPHERE[I_INDEX]))
    {
      RAYT_GET_FIRST_INTERSECTION_WITH_SPHERE(&S_RAY,
                                              &(P_RAYT_WORLD->P_SPHERE[I_INDEX]),
                                              &I_TMP_NB_INTERSECTION,
                                              &F_TMP_INTERSECTION,
                                              &S_TMP_INTERSECTION,
                                              &S_TMP_VECTOR,
                                              &P_TMP_PROPERTY);

      if (I_TMP_NB_INTERSECTION != 0)
      {
        B_INTERSECTION = K_TRUE;
      }
    }
  }

  P_RGB->F_RED = 0.0;
  P_RGB->F_GREEN = 0.0;
  P_RGB->F_BLUE = 0.0;

  if (B_INTERSECTION == K_FALSE)
  {
    *P_RGB = P_LIGHT->S_RGB;

  }

  return;
}

void RAYT_NORMALIZE_VECTOR(
  T_3D     S_VECTOR,
  T_3D    *P_NORMALIZED_VECTOR)
{
  T_FLOAT           F_VECTOR_NORME;

  F_VECTOR_NORME = ( (S_VECTOR.F_X * S_VECTOR.F_X) +
                     (S_VECTOR.F_Y * S_VECTOR.F_Y) +
                     (S_VECTOR.F_Z * S_VECTOR.F_Z) );

  F_VECTOR_NORME = sqrt(F_VECTOR_NORME);
  P_NORMALIZED_VECTOR->F_X = S_VECTOR.F_X / F_VECTOR_NORME;
  P_NORMALIZED_VECTOR->F_Y = S_VECTOR.F_Y / F_VECTOR_NORME;
  P_NORMALIZED_VECTOR->F_Z = S_VECTOR.F_Z / F_VECTOR_NORME;

  return;
}


void RAYT_TRACE(
  T_RAYT_RAY     S_RAY,
  T_PTR          P_OBJECT,
  T_UINT32       I_ITERATION,
  T_RAYT_RGB    *P_RGB)
{
  T_UINT32          I_INDEX,I_INDEX2;
  T_BOOLEAN         B_INTERSECTION;
  T_UINT32          I_TMP_NB_INTERSECTION;
  T_FLOAT           F_TMP_INTERSECTION;
  T_3D              S_TMP_VECTOR;
  T_RAYT_PROPERTY  *P_TMP_PROPERTY;
  T_3D              S_TMP_INTERSECTION;
  T_FLOAT           F_INTERSECTION;
  T_3D              S_NORMAL_VECTOR;
  T_RAYT_PROPERTY  *P_PROPERTY;
  T_3D              S_INTERSECTION;
  T_RAYT_RGB        S_TMP_RGB_LIGHT;
  T_RAYT_RGB        S_RGB;

  T_RAYT_RAY        S_R_RAY;
  T_RAYT_RAY        S_R_RAY_FROM_LIGHT;
  T_RAYT_RGB        S_R_RGB;

  T_FLOAT           F_LAMBERT;
  T_FLOAT           F_SCAL_PHONG;
  T_FLOAT           F_SCAL_N_I;
  T_PTR             P_OBJECT2;


  F_INTERSECTION = FLT_MAX;

  B_INTERSECTION = K_FALSE;

  I_ITERATION --;

  for (I_INDEX = 0; I_INDEX < P_RAYT_WORLD->I_NB_SPHERE; I_INDEX ++)
  {
    if (P_OBJECT != &(P_RAYT_WORLD->P_SPHERE[I_INDEX]))
    {
      RAYT_GET_FIRST_INTERSECTION_WITH_SPHERE(&S_RAY,
                                              &(P_RAYT_WORLD->P_SPHERE[I_INDEX]),
                                              &I_TMP_NB_INTERSECTION,
                                              &F_TMP_INTERSECTION,
                                              &S_TMP_INTERSECTION,
                                              &S_TMP_VECTOR,
                                              &P_TMP_PROPERTY);

      if (I_TMP_NB_INTERSECTION != 0)
      {
        if ((F_TMP_INTERSECTION <= F_INTERSECTION) || (B_INTERSECTION == K_FALSE))
        {
          F_INTERSECTION = F_TMP_INTERSECTION;
          P_PROPERTY = P_TMP_PROPERTY;
          S_NORMAL_VECTOR = S_TMP_VECTOR;
          B_INTERSECTION = K_TRUE;
          S_INTERSECTION = S_TMP_INTERSECTION;
          P_OBJECT2 = &(P_RAYT_WORLD->P_SPHERE[I_INDEX]);
        }
      }
    }
  }


  /* Check for other intersection and return F_INTERSECTION, P_PROPERTY and S_NORMAL_VECTOR */
  S_RGB.F_RED = 0.0;
  S_RGB.F_GREEN = 0.0;
  S_RGB.F_BLUE = 0.0;

  if (B_INTERSECTION == K_TRUE)
  {
    /* set ambiant color */
    S_RGB.F_RED = (P_PROPERTY->S_A.F_RED * P_RAYT_WORLD->S_AMBIANT_LIGHT.F_RED);
    S_RGB.F_GREEN = (P_PROPERTY->S_A.F_GREEN * P_RAYT_WORLD->S_AMBIANT_LIGHT.F_GREEN);
    S_RGB.F_BLUE = (P_PROPERTY->S_A.F_BLUE * P_RAYT_WORLD->S_AMBIANT_LIGHT.F_BLUE);

    /* COMPUTE NEW REFLECTED RAY */
    F_SCAL_N_I = ( S_RAY.S_VECTOR.F_X * S_NORMAL_VECTOR.F_X +
                   S_RAY.S_VECTOR.F_Y * S_NORMAL_VECTOR.F_Y +
                   S_RAY.S_VECTOR.F_Z * S_NORMAL_VECTOR.F_Z );



    S_R_RAY.S_VECTOR.F_X = -2.0 * S_NORMAL_VECTOR.F_X * F_SCAL_N_I + S_RAY.S_VECTOR.F_X;
    S_R_RAY.S_VECTOR.F_Y = -2.0 * S_NORMAL_VECTOR.F_Y * F_SCAL_N_I + S_RAY.S_VECTOR.F_Y;
    S_R_RAY.S_VECTOR.F_Z = -2.0 * S_NORMAL_VECTOR.F_Z * F_SCAL_N_I + S_RAY.S_VECTOR.F_Z;

    S_R_RAY.S_ORIGIN = S_INTERSECTION;

    /* Add lambert color */
    for (I_INDEX = 0; I_INDEX < P_RAYT_WORLD->I_NB_LIGHT; I_INDEX ++)
    {
      RAYT_GET_RGB_FROM_LIGHT(P_OBJECT2,
                              &S_INTERSECTION,
                              &(P_RAYT_WORLD->P_LIGHT[I_INDEX]),
                              &S_TMP_VECTOR,
                              &S_TMP_RGB_LIGHT);


      if ( (S_TMP_RGB_LIGHT.F_RED != 0.0) && (S_TMP_RGB_LIGHT.F_GREEN != 0.0) && (S_TMP_RGB_LIGHT.F_BLUE != 0.0) )
      {


        F_LAMBERT = S_TMP_VECTOR.F_X * S_NORMAL_VECTOR.F_X +
          S_TMP_VECTOR.F_Y * S_NORMAL_VECTOR.F_Y +
          S_TMP_VECTOR.F_Z * S_NORMAL_VECTOR.F_Z;

        if (F_LAMBERT >= 0.0)
        {
          S_RGB.F_RED += ( (S_TMP_RGB_LIGHT.F_RED * P_PROPERTY->S_D.F_RED) * F_LAMBERT );
          S_RGB.F_GREEN += ( (S_TMP_RGB_LIGHT.F_GREEN * P_PROPERTY->S_D.F_GREEN  * F_LAMBERT ));
          S_RGB.F_BLUE += ( (S_TMP_RGB_LIGHT.F_BLUE * P_PROPERTY->S_D.F_BLUE * F_LAMBERT ));


          /* Calculate Refected ray form light */
          S_R_RAY_FROM_LIGHT.S_VECTOR.F_X = -2.0 * S_NORMAL_VECTOR.F_X * -F_LAMBERT - S_TMP_VECTOR.F_X;
          S_R_RAY_FROM_LIGHT.S_VECTOR.F_Y = -2.0 * S_NORMAL_VECTOR.F_Y * -F_LAMBERT - S_TMP_VECTOR.F_Y;
          S_R_RAY_FROM_LIGHT.S_VECTOR.F_Z = -2.0 * S_NORMAL_VECTOR.F_Z * -F_LAMBERT - S_TMP_VECTOR.F_Z;


          F_SCAL_PHONG = -S_R_RAY_FROM_LIGHT.S_VECTOR.F_X * S_RAY.S_VECTOR.F_X +
            -S_R_RAY_FROM_LIGHT.S_VECTOR.F_Y * S_RAY.S_VECTOR.F_Y +
            -S_R_RAY_FROM_LIGHT.S_VECTOR.F_Z * S_RAY.S_VECTOR.F_Z;

          if (F_SCAL_PHONG >= 0.0)
          {
            for(I_INDEX2 = 0; I_INDEX2 < P_PROPERTY->I_PHONG_NUMBER; I_INDEX2++)
            {
              F_SCAL_PHONG *= F_SCAL_PHONG;
            }
            S_RGB.F_RED += ( (S_TMP_RGB_LIGHT.F_RED * P_PROPERTY->S_P.F_RED) * F_SCAL_PHONG );
            S_RGB.F_GREEN += ( (S_TMP_RGB_LIGHT.F_GREEN * P_PROPERTY->S_P.F_GREEN  * F_SCAL_PHONG ));
            S_RGB.F_BLUE += ( (S_TMP_RGB_LIGHT.F_BLUE * P_PROPERTY->S_P.F_BLUE * F_SCAL_PHONG ));
          }

        }
      }
    }
    /* Add reflexion color */
    if   ( ( (P_PROPERTY->S_R.F_RED != 0.0) || (P_PROPERTY->S_R.F_GREEN != 0.0) || (P_PROPERTY->S_R.F_BLUE != 0.0) ) && (I_ITERATION > 0) )
    {


      /* trace reflected ray */
      RAYT_TRACE(S_R_RAY,
                 P_OBJECT2,
                 I_ITERATION,
                 &S_R_RGB);
      S_RGB.F_RED += (P_PROPERTY->S_R.F_RED * S_R_RGB.F_RED);
      S_RGB.F_GREEN += (P_PROPERTY->S_R.F_GREEN * S_R_RGB.F_GREEN);
      S_RGB.F_BLUE += (P_PROPERTY->S_R.F_BLUE * S_R_RGB.F_BLUE);

    }

    /* Add transmission color */
    //	  if  ( (P_PROPERTY->S_T.F_RED != 0.0) || (P_PROPERTY->S_T.F_GREEN != 0.0) || (P_PROPERTY->S_T.F_BLUE != 0.0) )
    //{
    /* calculate S_T_RAY transmitted RAY */
    //  S_T_RAY.S_ORIGIN =  S_INTERSECTION;

    //}

  }
  *P_RGB= S_RGB;
}

void RAYT_RENDER(
  T_RAYT_WORLD  * P_WORLD,
  T_UINT32        I_ITERATION,
  T_UINT32        I_NB_HORIZONTAL_PIXEL,
  T_UINT32        I_NB_VERTICAL_PIXEL)
{
  T_UINT32        I_HORIZONTAL_INDEX;
  T_UINT32        I_VERTICAL_INDEX;
  T_RAYT_RAY      S_RAY;
  T_RAYT_RGB      S_RAYT_RGB;
  T_UINT32        I_PIXEL;
  unsigned int frame_buffer;

  P_RAYT_WORLD = P_WORLD;

  frame_buffer = VC_get_frame_buffer();

  for (I_VERTICAL_INDEX = 0; I_VERTICAL_INDEX < I_NB_VERTICAL_PIXEL; I_VERTICAL_INDEX ++)
  {
    for (I_HORIZONTAL_INDEX = 0; I_HORIZONTAL_INDEX < I_NB_HORIZONTAL_PIXEL; I_HORIZONTAL_INDEX ++)
    {
      RAYT_GET_PRIMARY_RAY(I_HORIZONTAL_INDEX, I_VERTICAL_INDEX, I_NB_HORIZONTAL_PIXEL, I_NB_VERTICAL_PIXEL, &S_RAY);

      RAYT_TRACE(S_RAY, NULL_PTR,I_ITERATION,&S_RAYT_RGB);

      I_PIXEL = ((T_UINT32)0xFF) << 24;

      if (S_RAYT_RGB.F_RED < 1.0)
      {
        I_PIXEL |= ( (T_UINT32)( (T_UINT8)(255 * (S_RAYT_RGB.F_RED))) << 16);
      }
      else
      {
        I_PIXEL |= ( (T_UINT32)( (T_UINT8)(0xFF)) << 16);
      }

      if (S_RAYT_RGB.F_GREEN < 1.0)
      {
        I_PIXEL |= ( (T_UINT32)( (T_UINT8)(255 * (S_RAYT_RGB.F_GREEN))) << 8);
      }
      else
      {
        I_PIXEL |= ( (T_UINT32)( (T_UINT8)(0xFF)) << 8);
      }

      if (S_RAYT_RGB.F_BLUE < 1.0)
      {
        I_PIXEL |= ( (T_UINT32)( (T_UINT8)(255 * (S_RAYT_RGB.F_BLUE))));
      }
      else
      {
        I_PIXEL |= ( (T_UINT32)( (T_UINT8)(0xFF)));
      }

      *(unsigned int *)frame_buffer = I_PIXEL;
      frame_buffer+=4;
    }
  }
  return;
}
