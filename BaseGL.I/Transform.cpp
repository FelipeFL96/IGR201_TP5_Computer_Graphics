#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdio.h>
#include "Transform.hpp"

using namespace std;

/*
 * Constructors
 */

Transform::Transform() :
    translation_vector(glm::vec3(0.0f, 0.0f, 0.0f)),
    rotation_x(0.0f), rotation_y(0.0f), rotation_z(0.0f),
    scale_factor(1.0f)
{}

Transform::Transform(glm::vec3 translation_vector, float rotation_x, float rotation_y, float rotation_z, float scale_factor) :
    translation_vector(translation_vector), 
    rotation_x(rotation_x), rotation_y(rotation_y), rotation_z(rotation_z), 
    scale_factor(scale_factor)
{}

/*
 * Getters
 */
glm::vec3 Transform::get_translation_vector(void) { return translation_vector; }
float Transform::get_rotation_x(void) { return rotation_x; }
float Transform::get_rotation_y(void) { return rotation_x; }
float Transform::get_rotation_z(void) { return rotation_y; }
float Transform::get_scale(void) { return scale_factor; }

/*
 * Setters
 */
void Transform::set_translation_vector(glm::vec3 new_translation_vector) { translation_vector = new_translation_vector; }
void Transform::set_rotation_x(float new_rotation_x) { rotation_x = new_rotation_x; }
void Transform::set_rotation_y(float new_rotation_x) { rotation_x = new_rotation_x; }
void Transform::set_rotation_z(float new_rotation_y) { rotation_y = new_rotation_y; }
void Transform::set_scale(float new_scale_factor) { scale_factor = new_scale_factor; }

/*
 * Transform Matrixes
 */
glm::mat4 Transform::translate() {
    glm::mat4 translation_matrix(1.0f);
    translation_matrix = glm::translate(translation_matrix, translation_vector);
    return translation_matrix;
}

glm::mat4 Transform::rotate_x() {
    glm::mat4 rotation_matrix(1.0f);
    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_x), glm::vec3(1.0f, 0.0f, 0.0f));
    return rotation_matrix;
}

glm::mat4 Transform::rotate_y() {
    glm::vec4 vec(1.0f, 3.0f, 2.0f, 1.0f);
    glm::mat4 rotation_matrix(1.0f);
    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_y), glm::vec3(0.0f, 1.0f, 0.0f));
    return rotation_matrix;
}

glm::mat4 Transform::rotate_z() {
    glm::vec4 vec(1.0f, 3.0f, 2.0f, 1.0f);
    glm::mat4 rotation_matrix(1.0f);
    rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_z), glm::vec3(0.0f, 0.0f, 1.0f));
    return rotation_matrix;
}

glm::mat4 Transform::scale() {
    glm::mat4 scaling_matrix(1.0f);
    scaling_matrix = glm::scale(scaling_matrix, glm::vec3(scale_factor, scale_factor, scale_factor));
    return scaling_matrix;
}

glm::mat4 Transform::computeTransformationMatrix() {
    return translate() * scale() * rotate_x() * rotate_y() * rotate_z();
}

/*
 * Printing
 */

void Transform::printm(glm::mat4 mat) {
    cout.precision(3);
    cout << "|\t" << mat[0][0] << "\t" << mat[1][0] << "\t" << mat[2][0] << "\t" << mat[3][0] << "\t|\n"
         << "|\t" << mat[0][1] << "\t" << mat[1][1] << "\t" << mat[2][1] << "\t" << mat[3][1] << "\t|\n"
         << "|\t" << mat[0][2] << "\t" << mat[1][2] << "\t" << mat[2][2] << "\t" << mat[3][2] << "\t|\n"
         << "|\t" << mat[0][3] << "\t" << mat[1][3] << "\t" << mat[2][3] << "\t" << mat[3][3] << "\t|\n"
         << endl;
}

void Transform::printv(glm::vec3 vec) {
    cout.precision(3);
    cout << "[ " << vec[0] << " " << vec[1] << " " << vec[2] << " ]" << endl;
}

void Transform::printv(glm::vec4 vec) {
    cout.precision(3);
    cout << "[ " << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << " ]" << endl;
}