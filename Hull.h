#ifndef HULLO_H
#define HULLO_H

#include <vector>
/*

https://github.com/superwills/Hullinator
Hull.h - Convex hull creation from point cloud
Accompanies my answer at http://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection/60225#60225
version 1.0.0, Aug 4, 2013 319p

Copyright (C) 2013 William Sherif

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

William Sherif
will.sherif@gmail.com

*/

#include "Vectorf.h"
#include "Intersectable.h"
#include <set>
using namespace std;



// indexed edge
struct IEdge
{
	int a, b;
	IEdge(int ia, int ib) : a(ia), b(ib) {
		if (a == b) warning("Degenerate edge %d==%d", a, b);
	}

	// Verts describe the same edge
	bool same(const IEdge& e) const {
		// a---b
		// b---a
		return (a == e.a && b == e.b) || (a == e.b && b == e.a);
	}
	void flip() { ::swap(a, b); }
};

struct ITri
{
	int a, b, c;

	ITri(int ia, int ib, int ic) : a(ia), b(ib), c(ic){}
	bool hasEdge(const IEdge& edge) const {
		// edge.a and edge.b must BOTH be in a,b,c
		// assuming the edge is not degenerate
		return (edge.a == a || edge.a == b || edge.a == c) &&
			(edge.b == a || edge.b == b || edge.b == c);
	}

	// Do I have AT LEAST ONE of your edges
	bool sharesEdge(const ITri& o) const {
		return hasEdge(IEdge(o.a, o.b)) ||
			hasEdge(IEdge(o.b, o.c)) ||
			hasEdge(IEdge(o.c, o.a));
	}
};

// The interface of this class is really Vector3f.  You pass in Vector3f's to
// specify the point cloud via addPtToBound(Vector3f), then you call hull.expandToContainAllPts().
// After you are all done, the hull's points are in hull.finalPts and hull.finalNormals.
//
// You may rework this class to hide its internals more deeply if you wish.

// Convex hull def'n
struct Hull
{
	vector<Vector3f> verts;

	// indices of the actual hull.  in creating the convex hull no new vertices are added,
	// only _different points_ of the original point cloud's points may be used in specifying the hull.
	vector<int> indices;

	//AABB aabb ; // used to find pts closest to AABB corners.
	vector<int> remIndices; // candidate pts for the hull when being constructed.

	// Keep a transformed set, for each frame.  if you are a drifter then
	// the transformedPts set should be just re-transformed.  For pilots,
	// the transformedPts grouping 
	// The UNIQUE set of hull points
	vector<Vector3f> finalPts, transformedPts;

	// The group of triangles representing the final convex hull.
	vector<Triangle> finalTris;

	// So, the same hull will be hit multiple times.
	vector<PrecomputedTriangle> transformedTris;

	// The group of distinct normals on the final convex hull.
	// if two normals are __very similar__, (two coplanar tris) then they
	// are considered as 1 normal.  That means one less axis to test in SAT testing,
	// since SAT only uses face normals and not actual Triangle position in space.
	vector<Vector3f> finalNormals, transformedNormals;

	// This is the distance that is tolerable for pts to be conisdered inside
	// the hull while they are really outside it.
	float tolerance;

	// the extreme pts go in the hull to start
	//Vector3f N[3]={ HUGE,HUGE,HUGE }, P[3]={-HUGE,-HUGE,-HUGE} ; // mins, maxes

	AABB aabb;

	// The indices of the minimum and maximum vertices,
	// in each of the xyz axes independently.
	int mins[3], maxes[3];

	// ids of vertices CLOSEST TO these corners of the AABB.
	vector<int> extremeCorners;

	// Has to be BUNCH of values b/c there could be a # of values on the perimeter
	//map< int, vector<Vector3f> > Ns,Ps ; // i used these for finding the AVERAGE pts.

	Hull() {
		defaults();
	}

	Hull(const vector<Vector3f> &initialPts)
	{
		defaults();

		// I make sure to eliminate pts that are within a tolerence of each other (NO DUPES)
		for (int i = 0; i < initialPts.size(); i++)
			addPtToBound(initialPts[i]);

		solve();
	}

	void defaults(){
		tolerance = 0.5; // THE BIGGER YOU SET THIS, THE MORE LIKELY
		// THE HULL IS TO "IGNORE" OUTLIERS OF THE HULL AT ANY STAGE.
		// IF A POINT IS LESS THAN `TOLERANCE` UNITS AWAY FROM THE HULL AT
		// ANY STAGE, IT IS CONSIDERED "INSIDE" THE HULL AND ITS DISCARDED WITHOUT
		// ACTUALLY BOUNDING IT.  YOU CAN RAISE THIS NUMBER TO COME UP WITH
		// MORE COARSE (FEWER TRIS) BUT STILL KIND OF ACCURATE HULLS.
	}

	// clear out all old information
	void clear()
	{
		verts.clear();  indices.clear();  remIndices.clear();
		finalPts.clear();  finalNormals.clear();  finalTris.clear();
		aabb = AABB();
	}

	// THE PUBLIC INTERFACE:
	// You want to bind this pt too.
	void addPtToBound(const Vector3f& pt) {
		// I'll only add it if its unique. insert each point only once.
		// If we get past this for loop the pt will be added.
		for (int i = 0; i < verts.size(); i++)
			if (verts[i].isNear(pt))
				return;//we had pt already
		verts.push_back(pt); // is a UNIQUE vertex of the submesh.
	}

	void solve()
	{
		initFromExtremePts();
		expandToContainAllPts();
	}

private:
	// Finds the initial set of extreme points for the hull.
	void findExtreme()
	{
		for (int i = 0; i < 3; i++)  mins[i] = maxes[i] = 0;
		indices.clear();
		remIndices.clear();
		aabb = AABB();
		// find the extreme pts
		for (int i = 0; i < verts.size(); i++)
		{
			aabb.bound(verts[i]);
			remIndices.push_back(i); // add the index to the indices to process.
			// Look for 6 pts that minimize and maximize x,y,z axes
			// find the 6 pts (may not be distinct) with:
			//   smallest x (mins[0]), smallest y (mins[1]), sm z (mins[2]), largest x maxes[0], lg y, lg z.
			for (int axis = 0; axis < 3; axis++)
			{
				if (verts[i].elts[axis] < verts[mins[axis]].elts[axis]) // this is the new minimum point FOR THAT AXIS
					mins[axis] = i;
				if (verts[i].elts[axis] > verts[maxes[axis]].elts[axis]) // this is the new maximum point FOR THAT AXIS
					maxes[axis] = i;
			}
		}

		// The tolerance should be related to the extreme points
	}

	void initFrom4Tet()
	{
		//warning( "Degenerate tri added to hull `%s`", name.c_str() ) ;

		// Instead use a tetrahedral seed mesh from 4 opposite AABB corners
		// (a tetrahedron from the points marked with *, and if those are not distinct,
		// then from the points marked with .)
		//
		//   *----.
		//  /|   /|
		// .-.--* *
		// |/   |/
		// *----.

		// Use the AABB, and find the 4 pts that are nearest the pxpypz corners.. etc.
		aabb.recomputeCorners();

		// Find the 8 vertices closest to the extreme corners
		// If you look in recomputeCorners(), you will see the indices used here
		struct Corner{
			int aabbCornerIndex;
			int closestIndexSoFar;
			float dist2;
			Corner(int iAABBCornerIndex) {
				aabbCornerIndex = iAABBCornerIndex;
				// Init these 2 with vals that will be overwritten 1st iteration
				closestIndexSoFar = 0;
				dist2 = HUGE;
			}
		};
		vector<Corner> corners;
		for (int i = 0; i < aabb.corners.size(); i++)
			corners.push_back(Corner(i));

		// check if the selected vertex for pxpypz is indeed THE CLOSEST ONE to pxpypz or no
		for (int c = 0; c < corners.size(); c++)
		{
			for (int i = 0; i < verts.size(); i++)
			{
				float dist2 = distance2(verts[i], aabb.corners[corners[c].aabbCornerIndex]);
				if (dist2 < corners[c].dist2)
				{
					corners[c].dist2 = dist2;
					corners[c].closestIndexSoFar = i;
				}
			}
		}
		// 1 nxnypz, 2 nxpynz, 4 pxnynz, 7 pxpypz
		// From a diagram of the AABB cube,
		//   2----G
		//  /|   /|
		// D-A--7 4
		// |/   |/
		// 1----F
		// Faces
		extremeCorners.resize(8);
		for (int i = 0; i < aabb.corners.size(); i++)
			extremeCorners[i] = corners[i].closestIndexSoFar;

		// see if the index values of the extreme corners are unique
		set<int> uniqueExtremeCorners;
		uniqueExtremeCorners.insert(extremeCorners[1]);
		uniqueExtremeCorners.insert(extremeCorners[2]);
		uniqueExtremeCorners.insert(extremeCorners[4]);
		uniqueExtremeCorners.insert(extremeCorners[7]);

		if (uniqueExtremeCorners.size() != 4)
		{
			// Even if this happens, the convex hull still might come out correctly.
			warning("Extreme corners bad, only %d of them, %d %d %d %d", uniqueExtremeCorners.size(),
				extremeCorners[1], extremeCorners[2], extremeCorners[4], extremeCorners[7]);
		}

		// wind: 1,4,7 | 1,7,2 | 1,2,4 | 4,2,7
		addTri(extremeCorners[1], extremeCorners[4], extremeCorners[7]);
		addTri(extremeCorners[1], extremeCorners[7], extremeCorners[2]);
		addTri(extremeCorners[1], extremeCorners[2], extremeCorners[4]);
		addTri(extremeCorners[4], extremeCorners[2], extremeCorners[7]);
	}

	void initFrom6AxisExtremes()
	{
		// IF THE 6 EXTREME POINTS ARE NOT UNIQUE, then
		// the initial mesh could be malformed if we don't take precaution.

		// Those 6 extreme pts form the initial hull mesh.
		// it's a really crude approximation, but if no points lie that far outside,
		// it could actually be a pretty good approx.

		// (can use 6 extreme points, but I found it was more likely that
		// single-axis extremes were more likely to be the same point,
		// or backwards faces could get generated, which is rather disasterous for Quickhull.
		// 8 tri  version
		int *P = maxes, *N = mins; // alias names
		addTri(P[2], P[0], P[1]);
		addTri(P[2], P[1], N[0]);
		addTri(P[2], N[0], N[1]);
		addTri(P[2], N[1], P[0]);

		addTri(N[2], P[1], P[0]);
		addTri(N[2], N[0], P[1]);
		addTri(N[2], N[1], N[0]);
		addTri(N[2], P[0], N[1]);
	}

	void initFromExtremePts()
	{
		findExtreme();
		//initFrom6AxisExtremes() ;
		initFrom4Tet();

		// Check the hull is ok
		if (!convexityTest())
		{
			error("BAD SEED HULL. Your hull will malfunction because the initial tetrahedron got fucked up. "
				"Hullinator apologizes, but you have to give me better spread-out points.");

			drawDebugExtremePts();
		}
	}

	// This is the test to make sure you indeed HAVE A convex polygon.
	// basically no face can have its normal pointing towards ANY other point
	// in the shape.  if it does, a face is BACKWARDS.
	bool convexityTest() const
	{
		for (int i = 0; i < indices.size(); i += 3)
		{
			Plane plane(verts[indices[i]], verts[indices[i + 1]], verts[indices[i + 2]]);

			for (int j = 0; j < indices.size(); j++)
			{
				if (i == j) { j += 2; skip; } // don't test your verts against the tri you came out of.
				if (plane.distanceToPoint(verts[indices[j]]) > tolerance)
				{
					warning("Your convex polygon is not convex.");
					addPermDebugPoint(verts[indices[j]], Red);
					addPermDebugTriLine(verts[indices[i]], verts[indices[i + 1]], verts[indices[i + 2]], Red);
					return 0;
				}
			}
		}
		return 1;
	}

	// THIS IS THE ONLY WAY TO ADD A TRIANGLE TO THE HULL.
	bool addTri(int ia, int ib, int ic) {
		if (ia == ib || ia == ic || ib == ic){
			//warning( "Degenerate tri %d %d %d", ia, ib, ic ) ;
			return 0; // degenerate
		}

		// There you have a triangle added.
		indices.push_back(ia);    indices.push_back(ib);    indices.push_back(ic);
		//addDebugLine( debugPASS1 + tri.triCentroid(), debugPASS1 + tri.triCentroid() + tri.plane.normal*0.1f, Yellow ) ;
		return 1;
	}

	// You have to find ALL faces that can "see" pt,
	// otherwise the polyhedron could become concave again.
	// expand the hull to include pt.
	void expandToInclude(int pti)
	{
		// 3 new tris imposed, then triIndex needs to be DELETED,
		vector<int> facesThatPtCanSee; // a single index that looks up into indices
		// is only needed to specify a tri.
		// the triangle is 3 verts, but the other 2 can be gotten by i+1, and i+2.
		// we go through this backwards, so that in removal, the indices of indices marked for removal
		// aren't going to be wrong AS WE'RE REMOVING THEM
		for (int i = (int)indices.size() - 3; i >= 0; i -= 3)
		{
			Plane plane(verts[indices[i]], verts[indices[i + 1]], verts[indices[i + 2]]);
			if (plane.distanceToPoint(verts[pti]) >= 0.f)
				facesThatPtCanSee.push_back(i); // keep the index into the index array. the other 2 are implied by
			// being the next 2 in the index array.
		}

		// So, ALL THOSE FACES MUST BE REMOVED.
		// the perimeter is the set of edges that only occur ONCE.
		vector<IEdge> perimeter; // the set of edges that see 
		for (int i = 0; i < facesThatPtCanSee.size(); i++) // Each ONE is a tri.
		{
			int si = facesThatPtCanSee[i]; /// starting index in indices for the tri.
			int i1 = indices[si + 0], i2 = indices[si + 1], i3 = indices[si + 2];
			//info( "Face starting at indices[%d], indices are (%d %d %d)", si, i1, i2, i3 ) ;

			// We have to analyze 3 edges at a time, but its not importnat they're distinctly named
			IEdge edges[3] = { IEdge(i1, i2), IEdge(i2, i3), IEdge(i3, i1) };

			// If any of the edges ONLY OCCURS ONCE in the facesThatPtCanSee set, then it is a horizon edge.
			for (int edgeNo = 0; edgeNo < 3; edgeNo++)
			{
				IEdge &edge = edges[edgeNo]; // This is the edge we're trying to see if is unique the selected set
				bool isHorizonEdge = 1; // assume its unique

				// Next loop tries to eliminate the possibility that it is a horizon edge
				// by checking for another occurrence of that same edge in the faceTahtPtCanSee group.
				for (int j = 0; isHorizonEdge &&  // we still belive it could be a horizon edge
					j < facesThatPtCanSee.size(); j++)
				{
					if (j == i) skip;  // Don't check the triangle i came from, to see if there is an edge repeat.

					// at each face in the faces that pt can see set, we get the 3 edges of the face in jedges.
					int jsi = facesThatPtCanSee[j];
					int j1 = indices[jsi + 0], j2 = indices[jsi + 1], j3 = indices[jsi + 2];

					// Now see if the edge we're checking is the same as any of the 3 edges of this face.
					IEdge jedges[3] = { IEdge(j1, j2), IEdge(j2, j3), IEdge(j3, j1) }; // 3 edges of the face
					for (int jEdgeNo = 0; jEdgeNo < 3; jEdgeNo++)
					{
						if (edge.same(jedges[jEdgeNo])) {
							isHorizonEdge = 0; // its the same. 
							//info( "Edge %d %d same as %d %d", edge.a,edge.b, jedges[jEdgeNo].a,jedges[jEdgeNo].b ) ;
							break;
						}
					}
				}

				if (isHorizonEdge)  perimeter.push_back(edge);
			}
		}


		//info( "pti could see %d faces, perimeter has %d edges", facesThatPtCanSee.size(), perimeter.size() ) ;
		// we then walk the PERIMETER
		// since the tris were all wound ccw, yo actuall just wind tris in ANY ORDER,
		// fan them out from (newpoint, edge.a, edge.b ) ;
		for (int i = 0; i < perimeter.size(); i++)
			addTri(pti, perimeter[i].a, perimeter[i].b);

		// REMOVE ALL OF THE OLD FACES
		// can either delete or just _don't copy_
		//vector<int> newIndices ;
		for (int i = 0; i < facesThatPtCanSee.size(); i++) // Each ONE is a tri.
		{
			// delete __3__ entries from indices starting from facesThatPtCanSee[i]
			indices.erase(indices.begin() + facesThatPtCanSee[i],
				indices.begin() + facesThatPtCanSee[i] + 3); // say deletes 9,10 and 11
		}
	}

	void getFinalPts()
	{
		// Quickly filter the nonunique indices
		set<int> uniqueIndices(indices.begin(), indices.end());
		for (set<int>::iterator iter = uniqueIndices.begin(); iter != uniqueIndices.end(); ++iter)
			finalPts.push_back(verts[*iter]);

		for (int i = 0; i < indices.size(); i += 3)
		{
			// Now keep the normals, for SAT tests.
			Triangle tri(verts[indices[i]], verts[indices[i + 1]], verts[indices[i + 2]]);
			finalTris.push_back(tri);

			// If we don't already have a normal like that,
			bool had = 0;
			for (int j = 0; j < finalNormals.size(); j++)
				// For SAT testing, lack of overlap in direction `normal` will
				// also be lack of overlap in direction `-normal`, no need to check both.
				if (finalNormals[j].isNear(tri.plane.normal) ||
					finalNormals[j].isNear(-tri.plane.normal)) {
				had = 1;
				break;
				}
			if (!had)
				finalNormals.push_back(tri.plane.normal);
		}



		// identity-transform save copies of finalNormals etc.
		transformedPts = finalPts;
		for (Triangle& tri : finalTris)
			transformedTris.push_back(tri);
		transformedNormals = finalNormals;
	}

	void expandToContainAllPts()
	{
		// repeatedly:
		// 1) trim inside pts
		// 2) find furthest out pt
		// 3) poke face
		while (remIndices.size())
		{
			// 1) delete any points INSIDE the hull
			// any pts inside that hull (ALL on +halfspace)
			removeRemPtsInside();

			// If you just engulfed all the remaining pts with the last expansion, you're done.
			if (!remIndices.size())  break;

			// 2) get the FURTHEST point
			// the point with the BIGGEST, (MINIMUM distance to the hull)
			Vector3f closestPtOnTri;
			// Assume first pt is the right answer,
			int fi = 0; // furthestI. I use an INDEX into remIndices to explicitly delete it after.
			float furthestDistance = 0;
			for (int i = 0; i < remIndices.size(); i++)
			{
				Vector3f ptOnTri;
				float dist = distanceToClosestTriIB(remIndices[i], ptOnTri);
				if (dist == HUGE) {
					//error( "Point %d is INSIDE the hull.",remIndices[i] ) ;
					//addDebugPoint( debugPASS1, Blue ) ;
					skip;
				}
				else if (dist > furthestDistance)
				{
					fi = i;
					furthestDistance = dist;
					closestPtOnTri = ptOnTri;
				}
			}

			//info( "Point %d has furthest distance with %f units", remIndices[fi], furthestDistance ) ;
			//addDebugLine( debugPASS1 + verts[furthestOut], debugPASS1 + closestPtOnTri, Orange ) ;
			// 3) EXPAND THE HULL
			expandToInclude(remIndices[fi]);

			// REMOVE that furthest pt
			remIndices.erase(remIndices.begin() + fi);  // although removeInside() ;shoudl remove,
			// technically "inside" the hull,
			// floating point error is bound to fuck this up soon or later. so we manually ensure its removed.
		}

		getFinalPts();
	}

	// Tri normals point OUTSIDE the hull, so,
	// if you are on the + side of any tri you are NOT INSIDE
	// Because we're trying to simplify the hull, EXTREMELY CLOSE POINTS
	// are considered OUTSIDE
	bool insideIB(int pti) const {
		for (int i = 0; i < (int)indices.size(); i += 3)
		{
			// Reconstructing the plane IS expensive, but this code isn't optimized there.
			Plane plane(verts[indices[i]], verts[indices[i + 1]], verts[indices[i + 2]]);

			// if the dist below is -, it means the pt is INSIDE according to this face.
			// if it is +, it means it is DEFINITELY OUTSIDE THE SHAPE since it is outside at least 1 face.
			if (plane.distanceToPoint(verts[pti]) > tolerance) // ITS NOT INSIDE.  The larger tolerance is, the more willing
				// I am to say "Ok, you're still inside" even though you are slightly out of line / slightly outside.
				// this eps is important for points you just added to the hull.
				return 0;
		}
		return 1; // you are inside all the planes
	}

	// Remove all remaining candidate pts from remIndices that
	// are now INSIDE THE HULL.
	void removeRemPtsInside()
	{
		// 0 1 2 3 4 5 6 7 8
		// delete 4, 5, 6, 8
		// 
		for (int i = (int)remIndices.size() - 1; i >= 0; --i)
			if (insideIB(remIndices[i]))
			{
			//info( "Removing %d", remIndices[i] ) ;
			remIndices.erase(remIndices.begin() + i);
			}
	}

	// Get you the distance to the CLOSEST triangle in the hull.
	// The point is outside already, now, get me the SMALLEST distance,
	// This operates on the INDEX BUFFER, ie it is to be used
	// DURING HULL CONSTRUCTION, before finalTris is complete.
	// So you shouldn't use this outside this class.  It is marked IB
	// because it uses the index buffer, constructing Triangle objects for EACH TRI.
	// Because of the possibility of face removal tris aren't kept (they could be)
	float distanceToClosestTriIB(int pti, Vector3f& closestPtOnTri) const {
		float minDist = HUGE;
		for (int i = 0; i < indices.size(); i += 3)
		{
			Triangle tri(verts[indices[i]], verts[indices[i + 1]], verts[indices[i + 2]]);

			Vector3f ptOnTri;
			float dist = tri.distanceToPoint(verts[pti], ptOnTri);
			if (dist < minDist) {
				minDist = dist;
				closestPtOnTri = ptOnTri;
			}
		}

		//addDebugPoint( debugPASS1 + p, Red ) ;
		return minDist;
	}








	///////////////////////////
	// INTERSECTION ROUTINES //
public:
	// The default is to re-transform from the original point set.
	void transform(const Matrix4f& matrix) {
		// The containers are already the right size (in getFinalPts).
		for (int i = 0; i < finalTris.size(); i++)
			transformedTris[i] = matrix * finalTris[i];

		for (int i = 0; i < finalNormals.size(); i++)
			transformedNormals[i] = matrix.upper3x3(finalNormals[i]);

		for (int i = 0; i < finalPts.size(); i++)
			transformedPts[i] = matrix * finalPts[i];
	}

	// This transforms the transformed pts from where the transformed last were,
	void transformTransformed(const Matrix4f& matrix) {
		for (PrecomputedTriangle &tri : transformedTris)
			tri = matrix * tri;

		for (Vector3f& pt : transformedNormals)
			pt = matrix.upper3x3(pt);

		for (Vector3f& pt : transformedPts)
			pt = matrix * pt;
	}

	void transformTransformed(const Matrix3f& rot) {
		// The containers are already the right size (in getFinalPts).
		for (PrecomputedTriangle &tri : transformedTris)
			tri = rot * tri;
		for (Vector3f& pt : transformedNormals)
			pt = rot*pt;
		for (Vector3f& pt : transformedPts)
			pt = rot*pt;
	}

	void translateTransformed(const Vector3f& trans) {
		for (PrecomputedTriangle &tri : transformedTris)
			tri = tri + trans;
		for (Vector3f& pt : transformedPts)
			pt += trans;
	}

	// Used by drifters, who "untranslate" each vertex so
	// the hull is back at the origin, rotate the hull, then translate it back out again.
	void untranslateRotateTranslate(const Vector3f& untrans, const Matrix3f& rot)
	{
		for (Vector3f& pt : transformedPts)
		{
			pt -= untrans;    // 1. untranslate
			pt = rot*pt;      // 2. Then the rotate
			pt += untrans;    // Move them back
		}

		// These only rotate
		for (Vector3f& pt : transformedNormals)
			pt = rot*pt;

		for (PrecomputedTriangle &tri : transformedTris)
		{
			Vector3f pa = rot*(tri.a - untrans) + untrans;
			Vector3f pb = rot*(tri.b - untrans) + untrans;
			Vector3f pc = rot*(tri.c - untrans) + untrans;

			tri = PrecomputedTriangle(pa, pb, pc);
		}
	}

	// You can ask me if some random pt is inside the hull or not after hull formation completed
	bool inside(const Vector3f& pt) const {
		for (int i = 0; i < transformedTris.size(); i++)
			if (transformedTris[i].plane.distanceToPoint(pt) > tolerance)
				return 0;
		return 1; // you are inside all the planes
	}

	float distanceToClosestPointOnHull(const Vector3f& pt, Vector3f& closestPtOnHull) const {
		float minDist = HUGE;
		for (int i = 0; i < transformedTris.size(); i++)
		{
			Vector3f ptOnTri;
			float dist = transformedTris[i].distanceToPoint(pt, ptOnTri);
			if (dist < minDist) {
				minDist = dist;
				closestPtOnHull = ptOnTri;
			}
		}
		return minDist;
	}

	Vector3f closestPointTo(const Vector3f& pt) const {
		Vector3f closestPtOnHull;
		distanceToClosestPointOnHull(pt, closestPtOnHull);
		return closestPtOnHull;
	}

	bool intersectsTri(const PrecomputedTriangle& tri) const {
		float meMin, meMax, oMin, oMax;

		// Start with tri normal.
		SATtest(tri.plane.normal, transformedPts, meMin, meMax);
		SATtest(tri.plane.normal, tri.a, oMin, oMax); //Only need to test 1 pt from tri, since all 3 will collapse to same pt.

		if (!overlaps(meMin, meMax, oMin, oMax)) {
			//addDebugLine( tri.plane.normal*meMin, tri.plane.normal*meMax, Red ) ;
			//addDebugPoint( tri.plane.normal*oMin, Blue ) ;
			return 0;
		}

		// Now test the hull's normals against tri's pts
		//vector<Vector3f> triPts ;
		//triPts.push_back( tri.a ) ;  triPts.push_back( tri.b ) ;  triPts.push_back( tri.c ) ;
		for (int i = 0; i < transformedNormals.size(); i++)
		{
			SATtest(transformedNormals[i], transformedPts, meMin, meMax);
			SATtest(transformedNormals[i], &tri.a, 3, oMin, oMax);

			if (!overlaps(meMin, meMax, oMin, oMax)) {
				//addDebugLine( finalNormals[i]*meMin, finalNormals[i]*meMax, Red ) ;
				//addDebugLine( finalNormals[i]*oMin, finalNormals[i]*oMax, Blue ) ;
				return 0;
			}
		}

		// Cross every triangle edge axis with every hull edge axis
		// Fixes https://github.com/superwills/Hullinator/issues/1
		for (int i = 0; i < transformedNormals.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Vector3f axis = transformedNormals[i].cross(tri.edges[j]);
				if (axis.allzero()) skip;
				axis.normalize();

				SATtest(axis, transformedPts, meMin, meMax);
				SATtest(axis, &tri.a, 3, oMin, oMax); // use all 3 tri verts.

				if (!overlaps(meMin, meMax, oMin, oMax))
					return 0;
			}
		}

		return 1;
	}

	// TO PUSH THE TRI OUT OF THE HULL, TRANSLATE THE TRI BY +PENETRATION
	// TO PUSH THE HULL BACK, TRANSLATE THE HULL BY -PENETRATION
	bool intersectsTri(const PrecomputedTriangle& tri, Vector3f& penetration) const {
		float minOverlap = HUGE;
		Vector3f axisOfMinOverlap;

		float meMin, meMax, oMin, oMax, lowerLim, upperLim;
		SATtest(tri.plane.normal, transformedPts, meMin, meMax);
		SATtest(tri.plane.normal, tri.a, oMin, oMax); //Only need to test 1 pt from tri, since all 3 will collapse to same pt.

		// Because the tri is going to appear as a POINT in the test, 
		// we have to use `maxOverlaps`.  See the comments in `maxOverlaps`
		// for how it detects overlaps differently than plain `overlaps`
		if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim)) {
			//addDebugLine( tri.plane.normal*meMin, tri.plane.normal*meMax, Red ) ;
			//addDebugPoint( tri.plane.normal*oMin, Blue ) ;
			return 0;
		}

		// These by default are the maxes then
		minOverlap = upperLim - lowerLim;
		axisOfMinOverlap = tri.plane.normal;

		// Now test the hull's normals against tri's pts
		//vector<Vector3f> triPts ;
		//triPts.push_back( tri.a ) ;  triPts.push_back( tri.b ) ;  triPts.push_back( tri.c ) ;
		for (int i = 0; i < transformedNormals.size(); i++)
		{
			SATtest(transformedNormals[i], transformedPts, meMin, meMax);
			SATtest(transformedNormals[i], &tri.a, 3, oMin, oMax); // use all 3 tri verts.

			if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim)) {
				//addDebugLine( transformedNormals[i]*meMin, transformedNormals[i]*meMax, Red ) ;
				//addDebugLine( transformedNormals[i]*oMin, transformedNormals[i]*oMax, Blue ) ;
				return 0;
			}

			float overlap = upperLim - lowerLim;
			if (overlap < minOverlap) {
				axisOfMinOverlap = transformedNormals[i];
				minOverlap = overlap;
			}
		}

		// cross product axes.  You get pretty good coverage
		// without testing these...  the fewest hits happen over here.
		// Bug https://github.com/superwills/Hullinator/issues/1 was due to this part
		// being SKIPPED

		// Cross every triangle edge axis with every hull edge axis
		for (int i = 0; i < transformedNormals.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Vector3f axis = transformedNormals[i].cross(tri.edges[j]);
				if (axis.allzero()) skip;
				axis.normalize();

				SATtest(axis, transformedPts, meMin, meMax);
				SATtest(axis, &tri.a, 3, oMin, oMax); // use all 3 tri verts.

				if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim))
					return 0;

				float overlap = upperLim - lowerLim;
				if (overlap < minOverlap) {
					axisOfMinOverlap = axis;
					minOverlap = overlap;
				}
			}
		}

		penetration = axisOfMinOverlap*minOverlap;
		return 1;

	}

	// The contact point is also generated.
	bool intersectsTri(const PrecomputedTriangle& tri, Vector3f& penetration, Vector3f &contact) const {
		float minOverlap = HUGE;
		Vector3f axisOfMinOverlap;
		const PrecomputedTriangle *pTriMinOverlap;

		// 1. Test the tri.
		float meMin, meMax, oMin, oMax, lowerLim, upperLim;
		SATtest(tri.plane.normal, transformedPts, meMin, meMax);
		SATtest(tri.plane.normal, tri.a, oMin, oMax); //Only need to test 1 pt from tri, since all 3 will collapse to same pt.

		if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim))
			return 0;

		// These by default are the maxes then
		minOverlap = upperLim - lowerLim;
		axisOfMinOverlap = tri.plane.normal;
		pTriMinOverlap = &tri;

		// 2. Test the hull's tris
		for (int i = 0; i < transformedTris.size(); i++)
		{
			SATtest(transformedTris[i].plane.normal, transformedPts, meMin, meMax);
			SATtest(transformedTris[i].plane.normal, &tri.a, 3, oMin, oMax); // use all 3 tri verts.

			if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim)) {
				//addDebugLine( transformedNormals[i]*meMin, transformedNormals[i]*meMax, Red ) ;
				//addDebugLine( transformedNormals[i]*oMin, transformedNormals[i]*oMax, Blue ) ;
				return 0;
			}

			float overlap = upperLim - lowerLim;
			if (overlap < minOverlap) {
				axisOfMinOverlap = transformedTris[i].plane.normal;
				minOverlap = overlap;
				pTriMinOverlap = &transformedTris[i];
			}
		}

		// 3. Test the CROSS of every triangle edge axis with every hull edge axis
		for (int i = 0; i < transformedTris.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Vector3f axis = transformedTris[i].plane.normal.cross(tri.edges[j]);
				if (axis.allzero()) skip;
				axis.normalize();

				SATtest(axis, transformedPts, meMin, meMax);
				SATtest(axis, &tri.a, 3, oMin, oMax); // use all 3 tri verts.

				if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim))
					return 0;

				float overlap = upperLim - lowerLim;
				if (overlap < minOverlap) {
					axisOfMinOverlap = axis;
					minOverlap = overlap;

					// This is a cross product between edges.
					//pTriMinOverlap=&transformedTris[i];
					pTriMinOverlap = 0;
				}
			}

			// hull edges against tri normal.
			for (int j = 0; j < 3; j++)
			{
				Vector3f axis = transformedTris[i].edges[j].cross(tri.plane.normal);
				if (axis.allzero()) skip;
				axis.normalize();

				SATtest(axis, transformedPts, meMin, meMax);
				SATtest(axis, &tri.a, 3, oMin, oMax); // use all 3 tri verts.

				if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim))
					return 0;

				float overlap = upperLim - lowerLim;
				if (overlap < minOverlap) {
					axisOfMinOverlap = axis;
					minOverlap = overlap;

					// This is a cross product between edges.
					//pTriMinOverlap=&transformedTris[i];
					pTriMinOverlap = 0;
				}
			}
		}


		penetration = axisOfMinOverlap*minOverlap;

		// Umm... You take your axis of minimal overlap, project the first polygon on it,
		// find the point(s) with the biggest dot product results, then do the same with
		// the second but find the points with the smallest dot product results.
		// Those are the points you're looking for, more or less.
		//  https://www.allegro.cc/forums/thread/596835
		// That produces a pointon the hull that is CLOSE to the tri.
		// For now I'm calling it the contact point, but this could be a lot more exact.

		// This Code Zealot post: http://www.codezealot.org/archives/394

		///*
		// On the convex hull we only want the pt of min pene
		// The pt generated is VERY reasonable.
		int meSmall = 0, meLarge = 0;
		SATGetPtsWithExtremeDots(axisOfMinOverlap, transformedPts, meSmall, meLarge);
		////addDebugPoint( transformedPts[meSmall], DarkGreen ) ;
		////addDebugPoint( transformedPts[meLarge], Green ) ;
		contact = transformedPts[meSmall];
		//*/


		// On the tri, max.
		//int oSmall=0,oLarge=0;
		//SATGetPtsWithExtremeDots( axisOfMinOverlap, &tri.a, 3, oSmall, oLarge ) ;
		//addDebugPoint( (&tri.a)[oSmall], Vector4f(0.5,0.5,0,1) ) ;
		//addDebugPoint( (&tri.a)[oLarge], Yellow ) ;

		if (pTriMinOverlap)
		{
			Vector3f off = pTriMinOverlap->plane.normal*0.01f;
			addDebugTriSolid(pTriMinOverlap->a + off, pTriMinOverlap->b + off, pTriMinOverlap->c + off, Green);
		}

		return 1;
	}


	bool intersectsSphere(const Vector3f& center, float r, Vector3f& closestPtOnHull) const {
		return distanceToClosestPointOnHull(center, closestPtOnHull) <= r;
	}

	inline bool intersectsSphere(const Vector3f& center, float r) const {
		Vector3f closestPtOnHull;
		return intersectsSphere(center, r, closestPtOnHull);
	}

	bool intersectsHull(const Hull& o) const {
		// Get the normals for one of the shapes,
		float meMin, meMax, oMin, oMax;

		for (int i = 0; i < transformedNormals.size(); i++)
		{
			SATtest(transformedNormals[i], transformedPts, meMin, meMax);
			SATtest(transformedNormals[i], o.transformedPts, oMin, oMax);
			if (!overlaps(meMin, meMax, oMin, oMax))
				return 0; // NO OVERLAP IN AT LEAST 1 AXIS, SO NO INTERSECTION
			// otherwise, go on with the next test
		}

		// TEST SHAPE2.normals as well
		for (int i = 0; i < o.transformedNormals.size(); i++)
		{
			SATtest(o.transformedNormals[i], transformedPts, meMin, meMax);
			SATtest(o.transformedNormals[i], o.transformedPts, oMin, oMax);
			if (!overlaps(meMin, meMax, oMin, oMax))
				return 0; // NO OVERLAP IN AT LEAST 1 AXIS, SO NO INTERSECTION
		}

		// 3. For accurate results, need test CROSSES
		// 3. Test the CROSS of every triangle edge axis with every hull edge axis
		for (int i = 0; i < transformedTris.size(); i++)
		{
			for (int j = 0; j < o.transformedTris.size(); j++)
			{
				for (int edgeNo = 0; edgeNo < 3; edgeNo++)
				{
					Vector3f axis = transformedTris[i].plane.normal.cross(o.transformedTris[j].edges[edgeNo]);
					if (axis.allzero()) skip;
					axis.normalize();

					SATtest(axis, transformedPts, meMin, meMax);
					SATtest(axis, o.transformedPts, oMin, oMax);

					if (!overlaps(meMin, meMax, oMin, oMax))
						return 0;
				}



				for (int edgeNo = 0; edgeNo < 3; edgeNo++)
				{
					Vector3f axis = transformedTris[i].edges[edgeNo].cross(o.transformedTris[j].plane.normal);
					if (axis.allzero()) skip;
					axis.normalize();

					SATtest(axis, transformedPts, meMin, meMax);
					SATtest(axis, o.transformedPts, oMin, oMax);

					if (!overlaps(meMin, meMax, oMin, oMax))
						return 0;
				}
			}
		}

		// if overlap occurred in ALL AXES, then they do intersect
		return 1;
	}


	// With separation vector.
	bool intersectsHull(const Hull& o, Vector3f &penetration, Vector3f &contact1, Vector3f &contact2) const {

		// Vars for penetration distance.
		Vector3f axisOfMinOverlap;
		float minOverlap = HUGE;
		const PrecomputedTriangle *pTriMinOverlap;

		float meMin, meMax, oMin, oMax, lowerLim, upperLim;

		for (int i = 0; i < transformedNormals.size(); i++)
		{
			SATtest(transformedNormals[i], transformedPts, meMin, meMax);
			SATtest(transformedNormals[i], o.transformedPts, oMin, oMax);

			if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim)) {
				//addDebugLine( transformedNormals[i]*meMin, transformedNormals[i]*meMax, Red ) ;
				//addDebugLine( transformedNormals[i]*oMin, transformedNormals[i]*oMax, Blue ) ;
				return 0; // NO OVERLAP IN AT LEAST 1 AXIS, SO NO INTERSECTION
			}

			float overlap = upperLim - lowerLim;
			if (overlap < minOverlap) {
				axisOfMinOverlap = transformedTris[i].plane.normal;
				minOverlap = overlap;
				pTriMinOverlap = &transformedTris[i];
			}
		}

		// TEST SHAPE2.normals as well
		for (int i = 0; i < o.transformedNormals.size(); i++)
		{
			SATtest(o.transformedNormals[i], transformedPts, meMin, meMax);
			SATtest(o.transformedNormals[i], o.transformedPts, oMin, oMax);
			if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim)) {
				//addDebugLine( transformedNormals[i]*meMin, transformedNormals[i]*meMax, Red ) ;
				//addDebugLine( transformedNormals[i]*oMin, transformedNormals[i]*oMax, Blue ) ;
				return 0; // NO OVERLAP IN AT LEAST 1 AXIS, SO NO INTERSECTION
			}

			float overlap = upperLim - lowerLim;
			if (overlap < minOverlap) {
				axisOfMinOverlap = transformedTris[i].plane.normal;
				minOverlap = overlap;
				pTriMinOverlap = &transformedTris[i];
			}
		}


		// 3. For accurate results, need test CROSSES
		// 3. Test the CROSS of every triangle edge axis with every hull edge axis
		for (int i = 0; i < transformedTris.size(); i++)
		{
			for (int j = 0; j < o.transformedTris.size(); j++)
			{
				// tt's normals against o.tt's edges
				for (int edgeNo = 0; edgeNo < 3; edgeNo++)
				{
					Vector3f axis = transformedTris[i].plane.normal.cross(o.transformedTris[j].edges[edgeNo]);
					if (axis.allzero()) skip;
					axis.normalize();

					SATtest(axis, transformedPts, meMin, meMax);
					SATtest(axis, o.transformedPts, oMin, oMax);

					if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim))
						return 0;

					float overlap = upperLim - lowerLim;
					if (overlap < minOverlap) {
						axisOfMinOverlap = axis;
						minOverlap = overlap;
						// This is a cross product between edges.
						//pTriMinOverlap=&transformedTris[i];
						pTriMinOverlap = 0;
					}
				}

				// and tt's edges against o.tt's normals
				for (int edgeNo = 0; edgeNo < 3; edgeNo++)
				{
					Vector3f axis = transformedTris[i].edges[edgeNo].cross(o.transformedTris[j].plane.normal);
					if (axis.allzero()) skip;
					axis.normalize();

					SATtest(axis, transformedPts, meMin, meMax);
					SATtest(axis, o.transformedPts, oMin, oMax);

					if (!maxOverlaps(meMin, meMax, oMin, oMax, lowerLim, upperLim))
						return 0;

					float overlap = upperLim - lowerLim;
					if (overlap < minOverlap) {
						axisOfMinOverlap = axis;
						minOverlap = overlap;
						pTriMinOverlap = 0;
					}
				}
			}
		}


		penetration = axisOfMinOverlap*minOverlap;

		// On the convex hull we only want the pt of min pene
		// The pt generated is VERY reasonable.
		int meSmall = 0, meLarge = 0;
		SATGetPtsWithExtremeDots(axisOfMinOverlap, transformedPts, meSmall, meLarge);
		////addDebugPoint( transformedPts[meSmall], DarkGreen ) ;
		////addDebugPoint( transformedPts[meLarge], Green ) ;
		contact1 = transformedPts[meSmall];

		SATGetPtsWithExtremeDots(axisOfMinOverlap, o.transformedPts, meSmall, meLarge);
		contact2 = o.transformedPts[meLarge];

		// if overlap occurred in ALL AXES, then they do intersect
		return 1;
	}

	// SAT test
	bool intersectsAABB(const AABB& aabb) const {
		// SAT:
		// 3 cube axes
		float meMin, meMax, oMin, oMax;

		for (int axis = 0; axis < 3; axis++)
		{
			// because we're projecting points in 3 space TO THE PRINCIPAL AXES,
			// the span of the projection of all the vertices is just going to be
			// the min/max in each priniciple axis direction.
			meMin = HUGE, meMax = -HUGE; // init these here
			for (int j = 0; j < transformedPts.size(); j++)
			{
				if (transformedPts[j].elts[axis] < meMin)  meMin = transformedPts[j].elts[axis];
				if (transformedPts[j].elts[axis] > meMax)  meMax = transformedPts[j].elts[axis];
			}

			// we "cheat" here and just pick out the correct index for the aabb.
			// its axis aligned!
			oMin = aabb.min.elts[axis];
			oMax = aabb.max.elts[axis];
			if (!overlaps(meMin, meMax, oMin, oMax))
				return 0;
		}

		// Ok, if the cheaper (aabb) tests didn't fail,
		// then test the hull's planes
		for (int i = 0; i < transformedNormals.size(); i++)
		{
			SATtest(transformedNormals[i], transformedPts, meMin, meMax);
			SATtest(transformedNormals[i], aabb.corners, oMin, oMax);
			if (!overlaps(meMin, meMax, oMin, oMax))
				return 0;
		}

		return 1;
	}

	bool intersectsSphere(const Sphere& sphere, Vector3f& closestPtOnHull) const {
		// Get the closest point on the hull to the sphere.
		// that point must be within r. This is MUCH MORE EXPENSIVE than the simple boolean test below.
		return distanceToClosestPointOnHull(sphere.c, closestPtOnHull) <= sphere.r;
	}

	bool intersectsSphere(const Sphere& sphere) const {
		// Can use an INSIDE test, similar to sphere frustum.
		// OR check i'm within sphere.r of each plane
		for (int i = 0; i < transformedTris.size(); i++)
		{
			float dist = transformedTris[i].plane.distanceToPoint(sphere.c);

			// If the sphere is way outside one of the planes, it doesn't hit the hull.
			if (dist > sphere.r)  return 0;
		}

		return 1; // sphere hits the hull or is inside the hull.
	}

	// rtcd pg 199
	// this is much more efficient than a ray-tri intn on each
	// possible BECAUSE its a convex hull
	bool intersectsRay(const Ray& ray, float &t1, float &t2) const {
		// assume intn is .. whole ray.
		t1 = 0.f, t2 = ray.len;

		// Test EVERY tri..
		for (int i = 0; i < transformedTris.size(); i++)
		{
			// solve t for reaching the plane.
			// The t for reaching the plane would be (-plane.d - normal•ray.start)/(normal • ray.dir)
			float den = transformedTris[i].plane.normal.dot(ray.dir);
			float dist = -transformedTris[i].plane.d - transformedTris[i].plane.normal.dot(ray.start);

			// If the ray is //l to the plane, but it runs AWAY from the plane,
			// then you'll never hit the convex polyhedron with this ray.
			if (den == 0.f) {
				if (dist > 0.f) {
					return 0;
				}
			}
			else // ray not parallel to this plane,
			{
				float t = dist / den;

				// The denominator being < 0 means the plane normal
				// FACES the ray (dot product of plane normal and ray dir is -ve)
				if (den < 0.f)
				{
					// use the FURTHEST BACK plane hit on there
					if (t > t1) t1 = t;
				}
				else
				{
					// den > 0.f, so ray runs in same direction as
					// plane normal ie the plane is hit from the back
					// want SMALLEST t for there.
					if (t < t2) t2 = t;
				}

				// if t1 (which tracks the FURTHEST forward facing plane intn found)
				// exceeds t2 (NEAREST back facing plane intn), its a miss b/c the location on the ray
				// of a hit was further on a forward facing plane
				// than on a back facing plane.  Does it make sense to hit
				// a "back facing side" of a (convex) rock BEFORE hitting its "front facing side"? No!
				if (t1 > t2) return 0;
			}
		}

		return 1;
	}

	inline bool intersectsRay(const Ray& ray, Vector3f& closerPt, Vector3f& furtherPt) const {
		float t1, t2;
		bool hit = intersectsRay(ray, t1, t2);
		closerPt = ray.at(t1), furtherPt = ray.at(t2);
		return hit;
	}

	// Only want closest pt
	inline bool intersectsRay(const Ray& ray, Vector3f& closerPt) const {
		float t1, t2;
		bool hit = intersectsRay(ray, t1, t2);
		closerPt = ray.at(t1);
		return hit;
	}

	// Hit or not, don't care where
	inline bool intersectsRay(const Ray& ray) const {
		float t1, t2;
		return intersectsRay(ray, t1, t2);
	}






	// Debug functions  
public:
	void drawDebugLines(const Vector4f& color) const {
		for (int i = 0; i < indices.size(); i += 3)
			addDebugTriLine(verts[indices[i]], verts[indices[i + 1]], verts[indices[i + 2]], color);
	}

	void drawDebug(const Vector3f& o, const Vector4f& color) const {
		//for( int i = 0 ; i < indices.size() ; i+=3 )
		//  addDebugTriSolid( verts[indices[i]], verts[indices[i+1]], verts[indices[i+2]], color ) ;
		for (int i = 0; i < transformedTris.size(); i++)
		{
			addDebugTriSolid(o + transformedTris[i].a, o + transformedTris[i].b, o + transformedTris[i].c, color);
		}
	}

	void drawDebug(const Vector4f& color) const {
		//for( int i = 0 ; i < indices.size() ; i+=3 )
		//  addDebugTriSolid( verts[indices[i]], verts[indices[i+1]], verts[indices[i+2]], color ) ;
		for (int i = 0; i < transformedTris.size(); i++)
		{
			addDebugTriSolid(transformedTris[i], color);
		}
	}

	void drawDebugExtremePts() const {
		// SHOW THE ORIGINAL EXTREME POINTS
		/*
		for( int axis = 0 ; axis < 3 ; axis++ )
		{
		Vector4f color( !axis, (axis==1)?1.f:0.f, (axis==2)?1.f:0.f, 1.f ) ;
		addDebugPoint( verts[mins[axis]], color*0.5 ) ;
		addDebugPoint( verts[maxes[axis]], color ) ;
		}
		//*/

		// Show the points closest to the AABB's corners
		for (int i = 0; i < extremeCorners.size(); i++)
			addDebugPoint(verts[extremeCorners[i]], Vector4f(i & 1, i & 2, i & 4, 1.f));

	}

	void drawDebugOriginalPts() const
	{
		for (int i = 0; i < verts.size(); i++)
			addDebugPoint(verts[i], insideIB(i) ? Green : Red);
	}

	void drawDebugFaceNormals() const
	{
		for (int i = 0; i < transformedTris.size(); i++)
		{
			Vector3f c = transformedTris[i].centroid;
			addDebugLine(c, c + transformedTris[i].plane.normal, Yellow);
		}
	}

	void drawDebugRemainingPts(const Vector3f& o, const Vector4f& color) const {
		// DISPLAY THE remIndices, AND A LINE TO THEIR CLOSEST FUCKING HULL FACE
		for (int i = 0; i < remIndices.size(); i++)
		{
			const Vector3f& pt = verts[remIndices[i]];
			if (insideIB(remIndices[i]))
				; // addDebugPoint( o + pt, Green ) ;
			else
			{
				Vector3f ptOnTri;
				distanceToClosestTriIB(remIndices[i], ptOnTri);
				addDebugPoint(o + pt, Red);
				addDebugLine(o + pt, o + ptOnTri, Red);
			}
		}
	}
};


#endif
