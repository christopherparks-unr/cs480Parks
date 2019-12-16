#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "object.h"
#include <bullet/btBulletDynamicsCommon.h>


struct Physics
{
	btBroadphaseInterface *bp;
	btDefaultCollisionConfiguration *cc;
	btCollisionDispatcher *dp;
	btSequentialImpulseConstraintSolver *s;
	btDiscreteDynamicsWorld *dw;

	std::vector<btCollisionShape*> vector_of_shapes;
	std::vector<Object*> vector_of_objects;

	void init()
	{
		bp = new btDbvtBroadphase();
		cc = new btDefaultCollisionConfiguration();
		dp = new btCollisionDispatcher(cc);
		s = new btSequentialImpulseConstraintSolver();
		dw = new btDiscreteDynamicsWorld(dp, bp, s, cc);
		dw->setGravity(btVector3(0.0,-2.00,0.0));
		vector_of_shapes.clear();
		vector_of_objects.clear();

		std::cout << "Successfully initialized DynObjects" << std::endl;
	}


	btRigidBody* add_object(Object* obj_to_add) {
		btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
		assert(obj_to_add);
		vector_of_objects.push_back(obj_to_add);
		btVector3 triArray[3];
		btTriangleMesh *objTriMesh = new btTriangleMesh();
		for(int i = 0; i < (int)obj_to_add->Vertices.size(); i += 3)
		{
			for (int j = 0; j < 3; j++) {
				glm::vec3 location = obj_to_add->Vertices[i+j].vertex;
				triArray[j] = btVector3(location[0],location[1],location[2]);
			}
			objTriMesh->addTriangle(triArray[0],triArray[1],triArray[2]);
		}
		btCollisionShape *shape = new btBvhTriangleMeshShape(objTriMesh, true);
		vector_of_shapes.push_back(shape);
	
		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
		btScalar mass(0.0);
		btVector3 inertia(0,0,0);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		rigidBody->setFriction(0.2);
		rigidBody->setRestitution(obj_to_add->restitution);
		dw->addRigidBody(rigidBody);
		return rigidBody;
	}

	btRigidBody* add_sphere(Object* obj_to_add, float m, float r) {
		btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
		vector_of_objects.push_back(obj_to_add);
		btCollisionShape *shape = new btSphereShape(btScalar(r));
		vector_of_shapes.push_back(shape);

		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
		btScalar mass(m);
		btVector3 inertia(0,0,0);
		shape->calculateLocalInertia(mass,inertia);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		rigidBody->setFriction(0.0);
		rigidBody->setRestitution(obj_to_add->restitution);
		dw->addRigidBody(rigidBody);
		return rigidBody;
	}

	btRigidBody* add_box(Object* obj_to_add, float m, btVector3 size) {
		btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
		vector_of_objects.push_back(obj_to_add);
		btCollisionShape *shape = nullptr;
		shape = new btBoxShape(size);
		vector_of_shapes.push_back(shape);

		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
		btScalar mass(m);
		btVector3 inertia(0,0,0);
		shape->calculateLocalInertia(mass,inertia);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		rigidBody->setFriction(0.5);
		rigidBody->setRestitution(obj_to_add->restitution);
		dw->addRigidBody(rigidBody);
		return rigidBody;
	}

	btRigidBody* add_cylinder(Object* obj_to_add, float m, btVector3 idk) {
		btVector3 offset = btVector3(obj_to_add->x_offset, obj_to_add->y_offset, obj_to_add->z_offset);
		vector_of_objects.push_back(obj_to_add);
		btCollisionShape *shape = nullptr;
		shape = new btCylinderShape(idk);
		vector_of_shapes.push_back(shape);

		btDefaultMotionState *shapeMotionState = NULL;
		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), offset));
		btScalar mass(m);
		btVector3 inertia(0,0,0);
		shape->calculateLocalInertia(mass,inertia);
		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
		rigidBody->setFriction(0.5);
		rigidBody->setRestitution(obj_to_add->restitution);
		dw->addRigidBody(rigidBody);
		return rigidBody;
	}

	void destruct()
	{
		delete bp;
		delete cc;
		delete dp;
		delete s;
		delete dw;
		bp = nullptr;
		cc = nullptr;
		dp = nullptr;
		s = nullptr;
		dw = nullptr;
		
	}
};

#endif  /* PHYSICS_H */
