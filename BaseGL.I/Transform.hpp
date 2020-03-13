#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <glm/glm.hpp>

class Transform {
public:

    /*
     * Constructors
     */
    Transform(void);
    Transform(glm::vec3, float, float, float, float);

    /*
     * Getters
     */
    glm::vec3 get_translation_vector(void);
    float get_rotation_x(void);
    float get_rotation_y(void);
    float get_rotation_z(void);
    float get_scale(void);

    /*
     * Setters
     */
    void set_translation_vector(glm::vec3);
    void set_rotation_x(float);
    void set_rotation_y(float);
    void set_rotation_z(float);
    void set_scale(float);
    
    /*
     * Transform Matrixes
     */
    glm::mat4 translate();
    glm::mat4 rotate_x();
    glm::mat4 rotate_y();
    glm::mat4 rotate_z();
    glm::mat4 scale();
    glm::mat4 computeTransformationMatrix();

    /*
    * Printing
    */
    static void printm(glm::mat4);
    static void printv(glm::vec3);
    static void printv(glm::vec4);

private:

    /*
     * Attributes
     */
    glm::vec3 translation_vector;
    float rotation_x, rotation_y, rotation_z;
    float scale_factor;

};

#endif //_TRANSFORM_H