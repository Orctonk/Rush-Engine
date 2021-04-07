#include "CollisionRoutines.h"

#include "../SphereCollider.h"
#include "../BoxCollider.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_access.hpp>

#define RUSH_EPSILON 0.002f

void calcTAndBTAxes(int normal, int* tangent, int* bitangent) {
	if(normal == 0){
		*tangent = 1;
		*bitangent = 2;
	} else if (normal == 1) {
		*tangent = 0;
		*bitangent = 2;
	} else {
		*tangent = 0;
		*bitangent = 1;
	}
}

int sutherlandHodgman(glm::vec2 points[4], const glm::vec2 &max, glm::vec2 out[8]) {
	glm::vec2 inp[8];
	int outi = 0;
	for (int i = 0; i < 4; i++) inp[i] = points[i];
	int inpsize = 4;
	float t;

	for (int i = 0; i < inpsize; i++) {
		int lp = ((i - 1) + inpsize) % inpsize;
		if (glm::epsilonEqual(inp[i].y, inp[lp].y, RUSH_EPSILON))
			t = 0.0f;
		else 
			t = (-max.y-inp[lp].y) / (inp[i].y - inp[lp].y);
		if (inp[i].y > -max.y) {
			if (inp[lp].y < -max.y) {
				RUSH_ASSERT(t <= 1 && t >= 0);
				out[outi++] = glm::vec2(inp[i].x * t + inp[lp].x * (1 - t), -max.y);
			}
			out[outi++] = inp[i];
		} else if (inp[lp].y > -max.y)
			out[outi++] = glm::vec2(inp[i].x * t + inp[lp].x * (1 - t), -max.y);
	}
	inpsize = outi;
	for (int i = 0; i < outi; i++) inp[i] = out[i];
	outi = 0;
	for (int i = 0; i < inpsize; i++) {
		int lp = ((i - 1) + inpsize) % inpsize;
		if (glm::epsilonEqual(inp[i].x, inp[lp].x, RUSH_EPSILON))
			t = 0.0f;
		else
			t = (-max.x-inp[lp].x) / (inp[i].x - inp[lp].x);
		if (inp[i].x > -max.x){
			if (inp[lp].x < -max.x) {
				RUSH_ASSERT(t <= 1 && t >= 0);
				out[outi++] = glm::vec2(-max.x, inp[i].y * t + inp[lp].y * (1 - t));
			}
			out[outi++] = inp[i];
		}
		else if (inp[lp].x > -max.x)
			out[outi++] = glm::vec2(-max.x, inp[i].y * t + inp[lp].y * (1 - t));
	}
	inpsize = outi;
	for (int i = 0; i < outi; i++) inp[i] = out[i];
	outi = 0;
	for (int i = 0; i < inpsize; i++) {
		int lp = ((i - 1) + inpsize) % inpsize;
		if (glm::epsilonEqual(inp[i].y, inp[lp].y, RUSH_EPSILON))
			t = 0.0f;
		else
			t = (max.y-inp[lp].y) / (inp[i].y - inp[lp].y);
		if (inp[i].y < max.y) {
			if (inp[lp].y > max.y) {
				RUSH_ASSERT(t <= 1 && t >= 0);
				out[outi++] = glm::vec2(inp[i].x * t + inp[lp].x * (1 - t), max.y);
			}
			out[outi++] = inp[i];
		}
		else if (inp[lp].y < max.y)
			out[outi++] = glm::vec2(inp[i].x * t + inp[lp].x * (1 - t), max.y);
	}
	inpsize = outi;
	for (int i = 0; i < outi; i++) inp[i] = out[i];
	outi = 0;
	for (int i = 0; i < inpsize; i++) {
		int lp = ((i - 1) + inpsize) % inpsize;
		if (glm::epsilonEqual(inp[i].x, inp[lp].x, RUSH_EPSILON))
			t = 0.0f;
		else
			t = (max.x-inp[lp].x) / (inp[i].x - inp[lp].x);
		if (inp[i].x < max.x) {
			if (inp[lp].x > max.x) {
				RUSH_ASSERT(t <= 1 && t >= 0);
				out[outi++] = glm::vec2(max.x, inp[i].y * t + inp[lp].y * (1 - t));
			}
			out[outi++] = inp[i];
		}
		else if (inp[lp].x < max.x)
			out[outi++] = glm::vec2(max.x, inp[i].y * t + inp[lp].y * (1 - t));
	}
	RUSH_ASSERT(outi <= 8);
	return outi;
}

bool BoxSphereCollision(const BaseCollider& c1, const BaseCollider& c2, const TransformComponent& t1, const TransformComponent& t2, Manifold* m) {
	bool col = SphereBoxCollision(c2, c1, t2, t1, m);
	m->collisionNormal = -m->collisionNormal;
	return col;
}

bool BoxBoxCollision(const BaseCollider& c1, const BaseCollider& c2, const TransformComponent& t1, const TransformComponent& t2, Manifold* m) {
	BoxCollider& b1 = (BoxCollider&)c1;
	BoxCollider& b2 = (BoxCollider&)c2;
	glm::vec3 p1 = b1.center + t1.GetTranslation();
	glm::vec3 p2 = b2.center + t2.GetTranslation();
	glm::vec3 pr = p2 - p1;
	
	glm::vec3 ex1 = b1.size / 2.0f;
	glm::vec3 ex2 = b2.size / 2.0f;

	glm::mat3 rot1 = glm::toMat3(t1.GetRotation());
	glm::mat3 rot2 = glm::toMat3(t2.GetRotation());

	glm::mat3 orthTest = rot1 * glm::transpose(rot1);
	
	glm::mat3 rotr = glm::transpose(rot1) * rot2;
	glm::mat3 absrotr;
	absrotr[0] = glm::abs(rotr[0]);
	absrotr[1] = glm::abs(rotr[1]);
	absrotr[2] = glm::abs(rotr[2]);

	glm::vec3 prrot = pr * rot1;

	int face = 0;
	float pen = 0.0f;
	float leastPen = -FLT_MAX;
	glm::vec3 normal = glm::vec3(0.0f);
	bool invNormal = false;

	// B1 X axis separation test
	pen = glm::abs(prrot.x) - (ex1.x + glm::dot(ex2, absrotr[0]));
	if (pen > 0.0f) return false; 
	if (pen > leastPen) {
		leastPen = pen;
		normal = t1.GetRight();
		invNormal = prrot.x < 0.0f;
		face = 1;
	}

	// B1 Y axis separation test
	pen = glm::abs(prrot.y) - (ex1.y + glm::dot(ex2, absrotr[1]));
	if (pen > 0.0f) return false; 
	if (pen > leastPen) {
		leastPen = pen;
		normal = t1.GetUp();
		invNormal = prrot.y < 0.0f;
		face = 2;
	}

	// B1 Z axis separation test
	pen = glm::abs(prrot.z) - (ex1.z + glm::dot(ex2, absrotr[2]));
	if (pen > 0.0f) return false; 
	if (pen > leastPen) {
		leastPen = pen;
		normal = t1.GetBack();
		invNormal = prrot.z < 0.0f;
		face = 3;
	}

	glm::vec3 prrot2 = pr * rot2;

	// B2 X axis separation test
	pen = glm::abs(prrot2.x) - (ex2.x + glm::dot(ex1, glm::row(absrotr, 0)));
	if (pen > 0.0f) return false; 
	if (pen > leastPen) {
		leastPen = pen;
		normal = t2.GetRight();
		invNormal = prrot2.x < 0.0f;
		face = 4;
	}

	// B2 Y axis separation test
	pen = glm::abs(prrot2.y) - (ex2.y + glm::dot(ex1, glm::row(absrotr, 1)));
	if (pen > 0.0f) return false;
	if (pen > leastPen) {
		leastPen = pen;
		normal = t2.GetUp();
		invNormal = prrot2.y < 0.0f;
		face = 5;
	}

	// B2 Z axis separation test
	pen = glm::abs(prrot2.z) - (ex2.z + glm::dot(ex1, glm::row(absrotr, 2)));
	if (pen > 0.0f) return false;
	if (pen > leastPen) {
		leastPen = pen;
		normal = t2.GetBack();
		invNormal = prrot2.z < 0.0f;
		face = 6;
	}

	rotr = glm::transpose(rotr);
	absrotr = glm::transpose(absrotr);
	absrotr += 1.0e-5f;

	glm::vec3 edgeNormal;
	float edgeNormalLen;
	float refD;

	// B1.X x B2
	refD = prrot.z * rotr[1][0] - prrot.y * rotr[2][0];
	pen = glm::abs(refD) - (ex1.y * absrotr[2][0] + ex1.z * absrotr[1][0] + ex2.y * absrotr[0][2] + ex2.z * absrotr[0][1]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { 0.0f, -rotr[2][0],rotr[1][0] };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 7;
	}
	refD = prrot.z * rotr[1][1] - prrot.y * rotr[2][1];
	pen = glm::abs(refD) - (ex1.y * absrotr[2][1] + ex1.z * absrotr[1][1] + ex2.x * absrotr[0][2] + ex2.z * absrotr[0][0]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { 0.0f, -rotr[2][1],rotr[1][1] };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 8;
	}
	refD = prrot.z * rotr[1][2] - prrot.y * rotr[2][2];
	pen = glm::abs(refD) - (ex1.y * absrotr[2][2] + ex1.z * absrotr[1][2] + ex2.x * absrotr[0][1] + ex2.y * absrotr[0][0]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { 0.0f, -rotr[2][2],rotr[1][2] };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 9;
	}

	// B1.Y x B2
	refD = prrot.x * rotr[2][0] - prrot.z * rotr[0][0];
	pen = glm::abs(refD) - (ex1.x * absrotr[2][0] + ex1.z * absrotr[0][0] + ex2.y * absrotr[1][2] + ex2.z * absrotr[1][1]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { rotr[2][0], 0.0f ,-rotr[0][0] };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 10;
	}
	refD = prrot.x * rotr[2][1] - prrot.z * rotr[0][1];
	pen = glm::abs(refD) - (ex1.x * absrotr[2][1] + ex1.z * absrotr[0][1] + ex2.x * absrotr[1][2] + ex2.z * absrotr[1][0]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { rotr[2][1], 0.0f ,-rotr[0][1] };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 11;
	}
	refD = prrot.x * rotr[2][2] - prrot.z * rotr[0][2];
	pen = glm::abs(refD) - (ex1.x * absrotr[2][2] + ex1.z * absrotr[0][2] + ex2.x * absrotr[1][1] + ex2.y * absrotr[1][0]);
	if (pen > 0.02f) 
		return false;
	edgeNormal = { rotr[2][2], 0.0f ,-rotr[0][2] };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 12;
	}

	// B1.Z x B2
	refD = prrot.y * rotr[0][0] - prrot.x * rotr[1][0];
	pen = glm::abs(refD) - (ex1.x * absrotr[1][0] + ex1.y * absrotr[0][0] + ex2.y * absrotr[2][2] + ex2.z * absrotr[2][1]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { -rotr[1][0],rotr[0][0],0.0f };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 13;
	}
	refD = prrot.y * rotr[0][1] - prrot.x * rotr[1][1];
	pen = glm::abs(refD) - (ex1.x * absrotr[1][1] + ex1.y * absrotr[0][1] + ex2.x * absrotr[2][2] + ex2.z * absrotr[2][0]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { -rotr[1][1],rotr[0][1],0.0f };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 14;
	}
	refD = prrot.y * rotr[0][2] - prrot.x * rotr[1][2];
	pen = glm::abs(refD) - (ex1.x * absrotr[1][2] + ex1.y * absrotr[0][2] + ex2.x * absrotr[2][1] + ex2.y * absrotr[2][0]);
	if (pen > RUSH_EPSILON) 
		return false;
	edgeNormal = { -rotr[1][2],rotr[0][2],0.0f };
	edgeNormalLen = glm::length(edgeNormal);
	pen /= edgeNormalLen;
	if (edgeNormalLen > RUSH_EPSILON && pen > leastPen) {
		leastPen = pen;
		normal = edgeNormal / edgeNormalLen;
		invNormal = refD < 0.0f;
		face = 15;
	}

	if (face > 6) normal = rot1 * normal;

	if (invNormal) normal = -normal;

	m->penetrationDepth = -leastPen;
	m->collisionNormal = normal;

	if (face > 6) {
		glm::vec3 sign = glm::sign(normal * rot1);
		p1 += rot1 * (sign * ex1);

		sign = -glm::sign(normal * rot2);
		p2 += rot2 * (sign * ex2);

		glm::vec3 aEdge = rot1[(face - 7) / 3];
		glm::vec3 bEdge = rot2[(face - 7) % 3];

		pr = p2 - p1;
		float edgeAngle = glm::dot(aEdge, bEdge);
		float prOnE1 = glm::dot(aEdge, pr);
		float prOnE2 = -glm::dot(bEdge, pr);
		float ang = 1.0f - edgeAngle * edgeAngle;
		float e1Fac, e2Fac;
		if (ang < RUSH_EPSILON)
			e1Fac = e2Fac = 0.0f;
		else {
			float angInv = 1.0f / ang;
			e1Fac = (prOnE1 + edgeAngle * prOnE2) * angInv;
			e2Fac = (edgeAngle * prOnE1 + prOnE2) * angInv;
		}

		p1 += aEdge * e1Fac;
		p2 += bEdge * e2Fac;

		m->collisionNormal = normal;
		m->numContacts = 1;
		m->contactPoints[0].depth = leastPen;
		m->contactPoints[0].position = (p1 + p2) * 0.5f;

		return true;
	}

	glm::mat3 refRot, incRot;
	glm::vec3 refPos, incPos;
	glm::vec3 refEx, incEx;
	glm::vec3 refN;
	int refNAxis, refTAxis,refBTAxis;
	if (face <= 3) { // Reference face is on B1
		refRot = rot1;
		refPos = p1;
		refEx = ex1;
		refNAxis = face - 1;
		refN = normal;
		incRot = rot2;
		incPos = p2;
		incEx = ex2;
	} else { // Reference face is on B2
		refRot = rot2;
		refPos = p2;
		refEx = ex2;
		refNAxis = face - 4;
		refN = -normal;
		incRot = rot1;
		incPos = p1;
		incEx = ex1;
	}

	calcTAndBTAxes(refNAxis, &refTAxis, &refBTAxis);

	glm::vec3 incBasisLen = refN * incRot;
	glm::vec3 incBasisLenAbs = glm::abs(incBasisLen);
	
	int incNAxis, incTAxis, incBTAxis;

	if (incBasisLenAbs.x > incBasisLenAbs.y) {
		if (incBasisLenAbs.x > incBasisLenAbs.z)
			incNAxis = 0;
		else
			incNAxis = 2;
	} else {
		if (incBasisLenAbs.y > incBasisLenAbs.z)
			incNAxis = 1;
		else 
			incNAxis = 2;
	}
	
	calcTAndBTAxes(incNAxis, &incTAxis, &incBTAxis);

	glm::vec3 incC;
	if (incBasisLen[incNAxis] < 0.0f)
		incC = incPos - refPos + incEx[incNAxis] * incRot[incNAxis];
	else 
		incC = incPos - refPos - incEx[incNAxis] * incRot[incNAxis];

	glm::vec2 incFace[4];
	glm::vec2 c;
	c[0] = glm::dot(incC, refRot[refTAxis]);
	c[1] = glm::dot(incC, refRot[refBTAxis]);
	
	glm::mat2 incToRef = { rotr[incTAxis][refTAxis], rotr[incBTAxis][refTAxis] , rotr[incTAxis][refBTAxis] ,rotr[incBTAxis][refBTAxis] };

	glm::vec2 st = incToRef[0] * incEx[incTAxis];
	glm::vec2 sb = incToRef[1] * incEx[incBTAxis];

	incFace[0] = c + st + sb;
	incFace[1] = c + st - sb;
	incFace[2] = c - st - sb;
	incFace[3] = c - st + sb;

	glm::vec2 refRect = { refEx[refTAxis], refEx[refBTAxis] };

	glm::vec2 clipped[8];
	int clipC = sutherlandHodgman(incFace, refRect, clipped);

	glm::mat2 refToInc = glm::inverse(incToRef);

	glm::vec4 contacts[8];
	int numContacts = 0;

	for (int i = 0; i < clipC; i++) {
		glm::vec2 cp = clipped[i] - c;
		cp = refToInc * cp;
		glm::vec3 contactPoint = incC + cp.x * incRot[incTAxis] + cp.y * incRot[incBTAxis];

		contacts[numContacts] = { contactPoint, refEx[refNAxis] - glm::dot(refN,contactPoint) };
		if (contacts[numContacts].w >= 0) {
			numContacts++;
		}
	}

	if (numContacts < 1) return false;

	if (numContacts <= 4) {
		for (int i = 0; i < numContacts; i++) {
			m->contactPoints[i].position = glm::vec3(contacts[i]) + refPos - m->collisionNormal * contacts[i].w;
			m->contactPoints[i].depth = -contacts[i].w;
		}
		m->numContacts = numContacts;
		return true;
	}
	else {
		for (int i = 0; i < 4; i++) {
			m->contactPoints[i].position = glm::vec3(contacts[i]) + refPos;
			m->contactPoints[i].depth = -contacts[i].w;
		}
		m->numContacts = 4;
		return true;
	}
}