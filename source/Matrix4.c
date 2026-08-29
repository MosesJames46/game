#include "../headers/Matrix4.h"
#include "../headers/game_math.h"
#include "../headers/game_utility.h"
#include "../headers/Camera.h"
#include "../headers/game_data.h"
#include <math.h>
#include <stdio.h>

Mat4x4 mat4x4_mul(Mat4x4 A, Mat4x4 B){
    Mat4x4 C;
    //indices i and j are for accessing the rows and columns of the matrix.
    //k is used to obtain the value from the respective row and column.
    //We only select a new row only after every column as been accessed.
    //We select new columns only after every value in the previous column has been accessed.
    //k walks through rows in A and columns in B.
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            float result = 0;
            for(int k = 0; k < 4; k++){
                result += A.m[i][k] * B.m[k][j];
            }
            C.m[i][j] = result;
        }
    }
    return C;
}

void mat4x4_init(Mat4x4* A){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            A->m[i][j] = 0;
        }
    }
}

Mat4x4 mat4x4_identity(){
    Mat4x4 I;
    mat4x4_init(&I);
    I.m[0][0]= 1;I.m[1][1] = 1;I.m[2][2] = 1;I.m[3][3] = 1;
    return I;
}

Mat4x4 mat4x4_rotate(vec3 axis){
    const float radian = 3.14159f / 180.f;
    
    //Multiply radian value by the value to rotate by for each component of axis.
    float xd = radian * axis.x;
    Mat4x4 X = mat4x4_identity();
    X.m[0][0] = 1;
    X.m[1][1] = cosf(xd);
    X.m[1][2] = -sinf(xd);
    X.m[2][1] = sinf(xd);
    X.m[2][2] = cosf(xd);
    
     
    float yd = radian * axis.y;
    Mat4x4 Y = mat4x4_identity();;
    Y.m[0][0] = cosf(yd);
    Y.m[0][2] = sinf(yd);
    Y.m[1][1] = 1;
    Y.m[2][0] = -sinf(yd);
    Y.m[2][2] = cosf(yd);

    float zd = radian * axis.z;
    Mat4x4 Z = mat4x4_identity();
    Z.m[0][0] = cosf(zd);
    Z.m[0][1] = -sinf(zd);
    Z.m[1][0] = sinf(zd);
    Z.m[1][1] = cosf(zd);
    Z.m[2][2] = 1;

    Mat4x4 R = mat4x4_mul(Y, Z);
    R = mat4x4_mul(X, R);
    return R;
}

vec4 mat4x4_mulv(Mat4x4 A, vec4 a){
    vec4 b;
    float br[4] = {0};
    for (int i = 0; i < 4; i++){
        float result = 0;
        result += A.m[i][0] * a.x;
        result += A.m[i][1] * a.y;
        result += A.m[i][2] * a.z;
        result += A.m[i][3] * a.w;
        br[i] = result;
    }

    b.x = br[0];
    b.y = br[1];
    b.z = br[2];
    b.w = br[3];

    return b;
}

vec3 mat4x4_mulv3(Mat4x4 A, vec3 a){
    vec3 b;
    float br[4] = {0};
    for (int i = 0; i < 4; i++){
        float result = 0;
        result += A.m[i][0] * a.x;
        result += A.m[i][1] * a.y;
        result += A.m[i][2] * a.z;
        result += A.m[i][3] * 1;
        br[i] = result;
    }

    b.x = br[0];
    b.y = br[1];
    b.z = br[2];

    return b;
}


void mat4x4_print(Mat4x4 A){
    for (int i = 0; i < 3; i++){
        for (int j =0; j < 3; j++){
            printf("%f", A.m[i][j]);
        }
        printf("\n");
    }
}


Mat4x4 mat4x4_cofactor(Mat4x4 A){
    Mat4x4 temp;
    int mp_i, mp_j, ap_i, ap_j;
    //The 
    mp_i = mp_j = 0;
    int sign = 1;

    int co_i, co_j;
    co_i = co_j = 0;
    //This quad nested for loop finds the row column that is to be excluded.
    //This will be i and j. It iterates through the input matrix to only input values that are
    //not on the row i and column j. This will be row + col - 1, since there will be overlap when
    //highlighting the row/column value. Once we find the value in the matrix, we place values into
    //the det matrix using mp_i/mp_j pointers. 
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            sign = (((i + j) % 2) == 0 )? 1 : -1; 
            Mat3x3 det_matrix;
            mp_i = 0;
            for(int k = 0; k < 4; k++){
                mp_j = 0;
                int l;
                for(l = 0; l < 4; l++){
                    //If Skip M_ij
                    if (l == j || i == k) continue;
                    //Only assign values to the determinant matrix when on correct cofactor
                    det_matrix.m[mp_i][mp_j] = A.m[k][l];
                    mp_j++;
                }
                if (k == i || l == j) continue;
                mp_i++;
            }
            //Assign the determinant of our new matrix to the cofactor matrix
            temp.m[i][j] = mat3x3_determinant(det_matrix) * sign;
        }
    }
    return temp;
}

void mat4x4_transpose(Mat4x4* A){
    swapf(&A->m[0][1], &A->m[1][0]);
    swapf(&A->m[2][0], &A->m[0][2]);
    swapf(&A->m[2][1], &A->m[1][2]);
    swapf(&A->m[3][0], &A->m[0][3]);
    swapf(&A->m[1][3], &A->m[3][1]);
    swapf(&A->m[3][2], &A->m[2][3]);
}

float mat4x4_determinant(Mat4x4 A){
    //tracks the temporary pointers
    int t_i, t_j;
    int sign;
    float r = 0;
    
    //The determinant of a 4x4 matrix is calculate by taking to coefficients of the first row 
    //multiplying these values by the determinant of the minor obtained by removing the corresponding column
    //and applying to correct sign.
    for (int i = 0; i < 4; i++){
        Mat3x3 temp;
        t_i = 0;
        for (int j = 0; j < 4; j++){
            t_j = 0;
            int k;
            for (k = 0; k < 4; k++){
                //The index i represents our column.
                if (j == 0 || k == i) continue;
                temp.m[t_i][t_j++] = A.m[j][k];
            }
            if (j == 0 || k == i) continue;
            ++t_i;
        }
        //mat3x3_print_mat3x3(temp);
        sign = ((i % 2) == 0) ? 1 : -1;
        r += sign * A.m[0][i] * mat3x3_determinant(temp);
    }
    return r;
}

Mat4x4 mat4x4_inverse(Mat4x4 A){
    float determinant = mat4x4_determinant(A);
    //printf("Determinant: %f\n", determinant);
    A = mat4x4_cofactor(A);
    
    mat4x4_transpose(&A);
    //mat4x4_print_mat4x4(A);
    determinant = 1.f / determinant;

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            A.m[i][j] *= determinant;
        }
    }
    return A;
}

Mat4x4 mat4x4_scale(float s){
    Mat4x4 I = mat4x4_identity();
    I.m[0][0] = s;
    I.m[1][1] = s;
    I.m[2][2] = s;
    return I;
}


Mat4x4 mat4x4_translate(vec3 x){
    //Defines how far to translate the object.
    Mat4x4 m = mat4x4_identity();
    m.m[0][3] = x.x;
    m.m[1][3] = x.y;
    m.m[2][3] = x.z;
    //mat4x4_print_mat4x4(m);
    return m;
}

Mat4x4 mat4x4_create_matrixv3(vec3 a, vec3 b, vec3 c){
    Mat4x4 C;
    C.m[0][0] = a.x; C.m[1][0] = b.x; C.m[2][0] = c.x; C.m[3][0] = 0;
    C.m[0][1] = a.y; C.m[1][1] = b.y; C.m[2][1] = c.y; C.m[3][1] = 0;
    C.m[0][2] = a.z; C.m[1][2] = b.z; C.m[2][2] = c.z; C.m[3][2] = 0;
    C.m[0][3] = 0; C.m[1][3] = 0; C.m[2][3] = 0; C.m[3][3] = 1.f;
    return C;
}

Mat4x4 mat4x4_create_matrixv4(vec4 a, vec4 b, vec4 c){
    Mat4x4 C;
    C.m[0][0] = a.x; C.m[1][0] = b.x; C.m[2][0] = c.x; C.m[3][0] = 0;
    C.m[0][1] = a.y; C.m[1][1] = b.y; C.m[2][1] = c.y; C.m[3][1] = 0;
    C.m[0][2] = a.z; C.m[1][2] = b.z; C.m[2][2] = c.z; C.m[3][2] = 0;
    C.m[0][3] = a.w; C.m[1][3] = b.w; C.m[2][3] = c.w; C.m[3][3] = 1.f;

    return C;
}

void mat4x4_print_mat4x4(Mat4x4 A){
    for (int  i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            printf("%f ", A.m[i][j]);
        }
        printf("\n");
    }
}

Mat4x4 mat4x4_lookat(vec3 eye, vec3 center, vec3 up){
    vec3 forward = vec3_normalize(vec3_distance(eye, center));
    vec3 right = vec3_normalize(vec3_cross_product(up, forward));
    up = vec3_normalize(vec3_cross_product(forward, right));

    Mat4x4 m = mat4x4_create_matrixv3(right, up, forward);
    return m;
}

Mat4x4 mat4x4_diablo_viewport(){
    Mat4x4 I = mat4x4_identity();
    float width = game_data.width / 2.f;
    float height = game_data.height / 2.f;
    I.m[0][0] = width;
    I.m[0][3] = width;

    I.m[1][1] = height;
    I.m[1][3] = height;

    return I;
}

Mat4x4 mat4x4_view(vec3 camera_position, vec3 look_at, vec3 up){
    struct Camera c;
    camera_lookat(&c, camera_position, look_at, up);
    //The view matrix is the inverse of the camera matrix. The camera matrix is the tranformation
    //used to obtain the camera's position in the world. If we translated from the origin to the point,
    //we need to inverse that transformation.

    // vec3_print_vector3(c.right);
    // vec3_print_vector3(c.up);
    // vec3_print_vector3(c.forward);
    //The following are placed this way because the normally would go into the matrix as a column vector.
    //Because our values are normalized however, we can perform an inverse of the camera orientation by transposing.
    Mat4x4 m = mat4x4_identity();
    m.m[0][0] = c.right.x;
    m.m[0][1] = c.right.y;
    m.m[0][2] = c.right.z;

    m.m[1][0] = c.up.x;
    m.m[1][1] = c.up.y;
    m.m[1][2] = c.up.z;

    /*
        When a point relative to the camera is applied to the view matrix it is finding
        the points position relative to the camers orientation. If our camera is pointing
        forward in the negative z-axis in the world, then we want to find how far along the
        camera's forward axis the point's z value lies. Typically if the camera moved from the origin
        to it's position, it's translation is M * c where M is the translation matrix, and c is the camera.
        To go from the camera's position to the origin, these values are negated.
    */
    m.m[2][0] = -c.forward.x;
    m.m[2][1] = -c.forward.y;
    m.m[2][2] = -c.forward.z;
    m.m[3][3] = 1;

    /*
        If we were to take the Orientation of the Camera and the Translation Matrix and perform matrix multiplication, we'd be multiplying the camera's oreientation to it's 
        translation. The problem is what happens computationally. Every row to column is basically just the dot product of the row vector of the Orientation matrix and the column
        of the translation matrix. Even more so, the first portion is just the Orientation to an identity matrix because the translation matrix has 1 in the diagonals and the 
        homogenous coordinate and the affine transformation. 

        Simply put, when we perform the matrix transformation we get the same result for the orientation but append the translation to the last column vector. Since 
        we only need the translation, dot product those values. 

        We perform negation because we want to place the camera at the center of the world negating the translation is essentially taking us back to the 
        original starting position of the camera.
    */
    m.m[0][3] = -vec3_dot(c.right, c.position);
    m.m[1][3] = -vec3_dot(c.up, c.position);
    m.m[2][3] = vec3_dot(c.forward, c.position);

    //mat4x4_print_mat4x4(m);

    Mat4x4 view = m;

    return view;
}

Mat4x4 mat4x4_perspective_projection(float fov, float aspect, float znear, float zfar){

    /*
        https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/perspective-matrix-in-practice.html?utm_source=chatgpt.com
    */

    /*
        Using: 3D Computer Graphics | Deriving the Perspective Projection Matrix as a ref.
        
        This matrix is used after our Camera transformation using the view matrix.
        We want to take our coordiantes in view space and project these values onto the screen.
        Our goal is to convert view space/camera space coordiantes to clip space coordinates.

        We start by imagining our camera as a frustum. This frutsum starts at the camera and extends
        into the world. Of course because the frustum is 3D there is a left, right, top, bottom, near, and
        far plane associated with each side respectively. If we observe this frustum from the right
        side, we limit the 3D frustum to a 2D triangle. This triangle gives us the side view, where
        the depth ranges from the camera to the far plane and the angle is given by theta. Our goal in 
        this perspective is to find where along the defined near plane, the area given by determining how close
        to start to the camera, our object will pass through if we were to draw a line from the camera
        to the object. 
        https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix//opengl-perspective-projection-matrix.html
        The link above shows a decent picture of what we're trying to accomplish. We can replicate this 
        same result by looking at our frustum form the top havving our z extend outward and our x being our
        upward axis.


        Finding Where Our Point Intersects with the Near Plane:
        Something to notice when finding this intersection starting from the camera and going to the near
        plane where the objects point intersects, is that if we know where the object's point is within
        the frustum we can use the law of similar triangles to determine the ratio. Since these ratios
        are the same, we can then solve for where along the y_n (near plane y axis) our point
        will be. Again keep in mind we are in view coordinates still at this moment.

        Normalizing the Coordinate:
        We want to now normalize the view coordinate value. Typically to do this, you take the full width/height
        of your plane and divide where your point is along the respective axis by that width/height.
        Remember though, we are solving for where the point intersects on the near plane along the 
        vertical/horizontal axis or x_n and y_n. We know our ratio is:
        x_n / z_n = x_e / z_e => x_n = (x_e * z_n) / z_e. the n stands for near. Our z_n is our defined
        near plane distance from the camera. We can set this to be any number. The only problem with this
        is that we still do not have our values normalized. To do this, we need to divide each value in
        eye coordinates by the width/height of the screen. So now our equation is : 
        x_n = (x_e * z_n) / (width * z_e).
        Now comes the obtaining of the height within the near plane. If looking at our frustum
        from the x-axis side, we get our triangle view again. Focusing on the near plane portion
        (go to about 19:45 within the video), we want to determine the height/top portion.
        We can solve for this using trig. We konw the tangent is the ratio between the height/width, 
        of a triangle. Our triangle's angle is only using half. Remember we do not use the full triangle
        because the intersection is always above the halfway point. Therefore we can obtain the tangent
        of half the angle which wil return to us the exact same ratio if we were to perform height / width.
        If we then perform algebra on the following equation: tan(theta/2) = t / n (t = top and n = near),
        we can solve for t: tan(theta / 2) * n = t.


        The idea is to imagine that our camera, now being centered at the origin, has normalized coordiantes
        ranging from [-1, 1]. These values extend from the right, left, top, bottom, near, and far.


    */
    Mat4x4 perspective_mat = mat4x4_identity();
    //The pi / 180 is used because our values going into tanf function are 
    //not in radians so we have to convert them.

    /*
        Another thing to be sure of is our understanding of fov. The FOV represents how much we see in the
        scene through the camera. If we look at our camera from the side at the x axis, we will see
        the y axis pointing up and z axis pointing out.
    */
    float f = 1.f / tanf((fov / 2.f) * ( 3.14159f / 180.f));
    float s = f / aspect;

    perspective_mat.m[0][0] = s;
    perspective_mat.m[1][1] = f;

    perspective_mat.m[2][2] = -(zfar + znear) / (zfar - znear);
    perspective_mat.m[2][3] = -(2 * zfar * znear) / (zfar - znear);
    perspective_mat.m[3][2] = -1;
    perspective_mat.m[3][3] = 0;

    //mat4x4_print_mat4x4(perspective_mat);

    return perspective_mat;
}

vec4 mat4x4_clipping_check(vec4 v){
    /*
        The following is taken from scratch pixel: https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/perspective-matrix-in-practice.html?utm_source=chatgpt.com

        When clipping coordinates, we want the final result to be between [-1, 1]. When vertices are tranformed into clip space,
        the result we're looking for is -1 <= x_clip / w_clip <= 1. This is equivalent to -w_clip <= x_clip <= w_clip. 
        The following code represents this with each component.
    */
    float pos_w = v.w;
    float neg_w = -pos_w;

    if (neg_w > v.x ){
        v.x = neg_w;
    }else if (v.x > pos_w){
        v.x = pos_w;
    }

    if (neg_w > v.y ){
        v.y = neg_w;
    }else if (v.y > pos_w){
        v.y = pos_w;
    }

    if (neg_w > v.z ){
        v.z = neg_w;
    }else if (v.z > pos_w){
        v.z = pos_w;
    }

    return v;
}
 
vec4 mat4x4_perspective_divide(vec4 v){

    if (v.w != 0){
        v.x /= v.w;
        v.y /= v.w;
        v.z /= v.w;
    }
    return v;
 }