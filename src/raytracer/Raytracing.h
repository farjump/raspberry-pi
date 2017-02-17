typedef char                T_CHAR;
typedef short               T_SHORT;
typedef long                T_LONG;
typedef int                 T_INT;
typedef long long           T_LONG2;
typedef char                T_INT8;
typedef short               T_INT16;
typedef long                T_INT32;



#define NULL_CHAR           (T_CHAR)0
#define NULL_SHORT          (T_SHORT)0
#define NULL_LONG           (T_LONG)0
#define NULL_INT            (T_INT)0
#define NULL_LONG2          (T_LONG2)0
#define NULL_INT8           (T_INT8)0
#define NULL_INT16          (T_INT16)0
#define NULL_INT32          (T_INT32)0


typedef unsigned char       T_UCHAR;
typedef unsigned short      T_USHORT;
typedef unsigned long       T_ULONG;
typedef unsigned int        T_UINT;
typedef unsigned long long  T_ULONG2;
typedef unsigned char       T_UINT8;
typedef unsigned short      T_UINT16;
typedef unsigned long       T_UINT32;

#define NULL_UCHAR          (T_UCHAR)0
#define NULL_USHORT         (T_USHORT)0
#define NULL_ULONG          (T_ULONG)0
#define NULL_UINT           (T_UINT)0
#define NULL_ULONG2         (T_ULONG2)0
#define NULL_UINT8          (T_UINT8)0
#define NULL_UINT16         (T_UINT16)0
#define NULL_UINT32         (T_UINT32)0

typedef float               T_FLOAT;
typedef double              T_DOUBLE;
typedef float               T_FLOAT32;
typedef double              T_FLOAT64;

#define NULL_FLOAT          (T_FLOAT)0.0
#define NULL_DOUBLE         (T_DOUBLE)0.0

typedef unsigned long       T_ADDR;

#define NULL_ADDR           (T_ADDR)0x00000000

typedef void *              T_PTR;

#define NULL_PTR       (T_PTR)0x00000000

typedef enum
{
  K_FALSE   = 0,
  K_TRUE    = 1
}T_BOOLEAN;


typedef struct
{
    T_UINT8   I_RED;
    T_UINT8   I_GREEN;
    T_UINT8   I_BLUE;
} T_RGB;

typedef struct
{
    T_FLOAT  F_X;
    T_FLOAT  F_Y;
} T_2D;

typedef struct
{
    T_FLOAT  F_X;
    T_FLOAT  F_Y;
    T_FLOAT  F_Z;
} T_3D;

typedef struct
{
    T_FLOAT  F_X;
    T_FLOAT  F_Y;
    T_FLOAT  F_H;
} T_2DH;

typedef struct
{
    T_FLOAT  F_X;
    T_FLOAT  F_Y;
    T_FLOAT  F_Z;
    T_FLOAT  F_H;
} T_3DH;


typedef struct
{
    T_3D     S_ORIGIN;
    T_3D     S_VECTOR;
} T_RAYT_RAY;



typedef struct
{
    T_FLOAT  F_RED;
    T_FLOAT  F_GREEN;
    T_FLOAT  F_BLUE;
} T_RAYT_RGB;

typedef struct
{
    T_RAYT_RGB    S_A;
    T_RAYT_RGB    S_D;
    T_RAYT_RGB    S_R;
    T_RAYT_RGB    S_P;
    T_UINT32      I_PHONG_NUMBER;
} T_RAYT_PROPERTY;

typedef struct
{
    T_3D   S_ORIGIN;
    T_3D   S_HORIZONTAL_VECTOR;
    T_3D   S_VERTICAL_VECTOR;
} T_RAYT_WINDOW;

typedef struct
{
    T_3D         S_ORIGIN;
    T_RAYT_RGB   S_RGB;
} T_RAYT_LIGHT;

typedef struct
{
    T_3D              S_ORIGIN;
    T_FLOAT           F_RADIUS;
    T_RAYT_PROPERTY   S_PROPERTY;
} T_RAYT_SPHERE;

typedef struct
{
    T_3D     S_ORIGIN;
    T_3D     S_VECTOR;
    T_RGB    S_COLOR;
} T_RAYT_PLAN;

typedef struct
{
    T_3D     S_ORIGIN;
    T_3D     S_VECTOR;
    T_RGB    S_COLOR0;
    T_RGB    S_COLOR1;
    T_FLOAT  F_SQUARE_SIZE;
} T_RAYT_CHESS_PLAN;

typedef struct
{
    T_3D                 S_EYE;
    T_RAYT_WINDOW        S_WINDOW;

    T_RAYT_RGB           S_AMBIANT_LIGHT;

    T_UINT32             I_NB_LIGHT;
    T_RAYT_LIGHT        *P_LIGHT;

    T_UINT32             I_NB_SPHERE;
    T_RAYT_SPHERE       *P_SPHERE;

    T_UINT32             I_NB_PLAN;
    T_RAYT_PLAN         *P_PLAN;

    T_UINT32             I_NB_CHESS_PLAN;
    T_RAYT_CHESS_PLAN   *P_CHESS_PLAN;
} T_RAYT_WORLD;


/* private function */

void RAYT_GET_PRIMARY_RAY(
  T_UINT32      I_HORIZONTAL_PIXEL,
  T_UINT32      I_VERTICAL_PIXEL,
  T_UINT32      I_NB_HORIZONTAL_PIXEL,
  T_UINT32      I_NB_VERTICAL_PIXEL,
  T_RAYT_RAY   *P_RAY);

void RAYT_GET_REFLECTED_VECTOR(
  T_3D      S_VECTOR,
  T_3D      S_NORMAL_VECTOR,
  T_3D     *P_REFLECTED_VECTOR);

void RAYT_NORMALIZE_VECTOR(
  T_3D     S_VECTOR,
  T_3D    *P_NORMALIZED_VECTOR);


void RAYT_GET_FIRST_INTERSECTION_WITH_SPHERE(
  T_RAYT_RAY        *P_RAY,
  T_RAYT_SPHERE     *P_SPHERE,
  T_UINT32          *P_NB_INTERSECTION,
  T_FLOAT           *P_INTERSECTION_PARAM,
  T_3D              *P_INTERSECTION,
  T_3D              *P_NORMAL_VECTOR,
  T_RAYT_PROPERTY  **P_PROPERTY);


void RAYT_TRACE(
  T_RAYT_RAY     S_RAY,
  T_PTR          P_OBJECT,
  T_UINT32       I_ITERATION,
  T_RAYT_RGB    *P_RGB);

void RAYT_GET_3D(
  T_RAYT_RAY  *P_RAY,
  T_FLOAT      F_PARAMETER,
  T_3D        *P_3D);


void RAYT_GET_RGB_FROM_LIGHT(
  T_PTR           P_OBJECT,
  T_3D           *P_ORIGIN,
  T_RAYT_LIGHT   *P_LIGHT,
  T_3D           *P_VECTOR,
  T_RAYT_RGB     *P_RGB);



/* Public function */

void RAYT_RENDER(
  T_RAYT_WORLD  * P_WORLD,
  T_UINT32        I_ITERATION,
  T_UINT32        I_NB_HORIZONTAL_PIXEL,
  T_UINT32        I_NB_VERTICAL_PIXEL);


extern T_RAYT_WORLD   * P_RAYT_WORLD;
