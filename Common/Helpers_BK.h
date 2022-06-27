#pragma once

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>

#include "imgui.h"

#include <cmath>

#include <random>
#include <vector>

namespace SK
{
	static std::default_random_engine g_oGen;
	static std::uniform_real_distribution<Scalar> g_oDist(Consts::t_1, Consts::t1);

	inline
	Vec2	Rotate(Vec2 const v, Scalar const fCos0, Scalar const fSin0)
	{
		return {v.x * fCos0 - v.y * fSin0, v.x * fSin0 + v.y * fCos0 };
	}
	inline
	Vec2	Rotate(Vec2 const v, Scalar const fAngleRad)
	{
		Scalar const fCos0 = Func::Cos(fAngleRad);
		Scalar const fSin0 = Func::Sin(fAngleRad);

		return Rotate(v, fCos0, fSin0);
	}

	inline
	Scalar	LengthSqr(Vec2 const v)
	{
		return glm::dot(v, v);
	}

	inline
	Scalar	LengthSqr(Vec2 const v0, Vec2 const v1)
	{
		Vec2 const v = v0 - v1;
		return glm::dot(v, v);
	}

	inline
	Vec2	ImToVec2(ImVec2 const vImVec2)
	{
		return Vec2(static_cast<Scalar>(vImVec2.x * g_fDrawScale), static_cast<Scalar>(vImVec2.y * g_fDrawScale));
	}

	inline
	ImVec2	VecToIm(Vec2 const vVec2)
	{
		return ImVec2(static_cast<f32>(vVec2.x * g_fDrawScale), static_cast<f32>(vVec2.y * g_fDrawScale));
	}

	inline
	ImVec2	VecToImDraw(Vec2 const vVec2)
	{
		return ImVec2(static_cast<f32>(vVec2.x * g_fDrawScale), static_cast<f32>(-vVec2.y * g_fDrawScale));
	}

	inline
	ImVec2	VecToImDraw(Scalar const x, Scalar const y)
	{
		return ImVec2(static_cast<f32>(x * g_fDrawScale), static_cast<f32>(-y * g_fDrawScale));
	}

	inline
	ImVec2	ImToDraw(ImVec2 const v)
	{
		return ImVec2(v.x * g_fDrawScale, -v.y * g_fDrawScale);
	}

	inline
	Scalar	Random(Scalar const fMin, Scalar const fMax)
	{
		return Func::ReScale(g_oDist(g_oGen), Consts::t_1, Consts::t1, fMin, fMax);
	}

	inline
	Vec2	RandomPointOnCircle(Vec2 const vCenter, Scalar const fRadius)
	{
		Vec2 vU(fRadius * Func::Sqrt(Func::ReScale(g_oDist(g_oGen), Consts::t_1, Consts::t1, Consts::t0, Consts::t1)),
				Consts::tTau * g_oDist(g_oGen));
		return Vec2(vCenter.x + Func::Cos(vU.y), vCenter.y + Func::Sin(vU.y));
	}

	inline
	Vec2	RandomPointOnCircleRaw() // Rad [0, 1], Angle [0, Tau]
	{
		Vec2 vU(Func::Sqrt(Func::ReScale(g_oDist(g_oGen), Consts::t_1, Consts::t1, Consts::t0, Consts::t1)),
				Consts::tTau * g_oDist(g_oGen));
		return vU;
	}

	inline
	Scalar	InverseLerp(Scalar const fA, Scalar const fB, Scalar const fLerp)
	{
		return (fA - fLerp) / (fA - fB);
	}

	inline
	Vec3	Lerp(Vec3 const& a, Vec3 const& b, Scalar const t)
	{
		return a * (Consts::t1 - t) + b * t;
	}

	inline
	Vec3	HSV2RGB(Vec3 const& c)
	{
		Vec4 const K = Vec4(Consts::t1, Consts::t2_3, Consts::t1_3, Consts::t3);
		Vec3 p = glm::abs(glm::fract(c.xxx + K.xyz) * Consts::t6 - K.www);
		Vec3 r = glm::clamp(p - K.xxx, Consts::t0, Consts::t1);
		return c.z * Lerp(K.xxx, r, c.y);
	}

	inline
	ImU32	RandomColor()
	{
		Scalar const fHue = Func::Lerp(Consts::t0, Consts::t1_3, Random(Consts::t0, Consts::t1));
		Vec3 const vHSVColor = Vec3(fHue, Consts::t1, Consts::t1);
		Vec3 const vRGB = glm::clamp(HSV2RGB(vHSVColor), Consts::t0, Consts::t1);

		return IM_COL32(static_cast<UInt32>(Func::Round(vRGB.x * 255.0f)),
			static_cast<UInt32>(Func::Round(vRGB.y * 255.0f)),
			static_cast<UInt32>(Func::Round(vRGB.z * 255.0f)),
			255);
	}

	inline
	Scalar	Det(Scalar const a, Scalar const b, Scalar const c, Scalar const d)
	{
		return a * d - b * c;
	}

	inline
	Bool	LineLineIntersect(	Scalar const x1, Scalar const y1, // Line 1 start
								Scalar const x2, Scalar const y2, // Line 1 end
								Scalar const x3, Scalar const y3, // Line 2 start
								Scalar const x4, Scalar const y4, // Line 2 end
								Scalar& ixOut, Scalar& iyOut)
	{
		Scalar const detL1 = Det(x1, y1, x2, y2);
		Scalar const detL2 = Det(x3, y3, x4, y4);
		Scalar const x1mx2 = x1 - x2;
		Scalar const x3mx4 = x3 - x4;
		Scalar const y1my2 = y1 - y2;
		Scalar const y3my4 = y3 - y4;

		Scalar const denom = Det(x1mx2, y1my2, x3mx4, y3my4);
		if (denom == Consts::t0) // Lines don't seem to cross
		{
			ixOut = SK::SKNaN<Scalar>;
			iyOut = SK::SKNaN<Scalar>;
			return false;
		}

		Scalar const xnom = Det(detL1, x1mx2, detL2, x3mx4);
		Scalar const ynom = Det(detL1, y1my2, detL2, y3my4);
		ixOut = xnom / denom;
		iyOut = ynom / denom;
		if (!std::isfinite(ixOut) || !std::isfinite(iyOut)) // Probably a numerical issue
			return false;

		return true; //All OK
	}

	inline
	Bool	LineLineIntersect(Vec2& vResult, Vec2 const vLA0, Vec2 const vLA1, Vec2 const vLB0, Vec2 const vLB1)
	{
		return LineLineIntersect(vLA0.x, vLA0.y, vLA1.x, vLA1.y,
								 vLB0.x, vLB0.y, vLB1.x, vLB1.y,
								 vResult.x, vResult.y);
	}

	inline
	std::vector<Vec2> SegmentCircleIntersects(Vec2 const& p1, Vec2 const& p2, Vec2 const& cp, Scalar r, Bool const segment)
	{
		auto sq = [](Scalar const x) {
			return x * x;
		};
		std::vector<Vec2> res;
		Scalar x0 = cp.x;
		Scalar y0 = cp.y;
		Scalar x1 = p1.x;
		Scalar y1 = p1.y;
		Scalar x2 = p2.x;
		Scalar y2 = p2.y;
		Scalar A = y2 - y1;
		Scalar B = x1 - x2;
		Scalar C = x2 * y1 - x1 * y2;
		Scalar a = sq(A) + sq(B);
		Scalar b, c;
		Bool bnz = true;
		constexpr Scalar eps = static_cast<Scalar>(1e-14);
		if (Func::Abs(B) >= eps) {
			b = Consts::t2 * (A * C + A * B * y0 - sq(B) * x0);
			c = sq(C) + Consts::t2 * B * C * y0 - sq(B) * (sq(r) - sq(x0) - sq(y0));
		}
		else {
			b = Consts::t2 * (B * C + A * B * x0 - sq(A) * y0);
			c = sq(C) + Consts::t2 * A * C * x0 - sq(A) * (sq(r) - sq(x0) - sq(y0));
			bnz = false;
		}
		auto d = sq(b) - Consts::t4 * a * c; // discriminant
		if (d < Consts::t0)
		{
			return res;
		}

		// checks whether a point is within a segment
		auto within = [x1, y1, x2, y2, &sq, eps](Scalar const x, Scalar const y) {
			auto d1 = Func::Sqrt(sq(x2 - x1) + sq(y2 - y1));	// distance between end-points
			auto d2 = Func::Sqrt(sq(x - x1) + sq(y - y1));		// distance from point to one end
			auto d3 = Func::Sqrt(sq(x2 - x) + sq(y2 - y));		// distance from point to other end
			auto delta = d1 - d2 - d3;
			return Func::Abs(delta) < eps;	// true if delta is less than a small tolerance
		};

		auto fx = [A, B, C](Scalar const x) {
			return -(A * x + C) / B;
		};

		auto fy = [A, B, C](Scalar const y) {
			return -(B * y + C) / A;
		};

		auto rxy = [segment, &res, within](Scalar const x, Scalar const y) {
			if (!segment || within(x, y)) {
				res.emplace_back(x, y);
			}
		};

		Scalar x, y;
		if (d == Consts::t0) {
			// line is tangent to circle, so just one intersect at most
			if (bnz) {
				x = -b / (Consts::t2 * a);
				y = fx(x);
				rxy(x, y);
			}
			else {
				y = -b / (Consts::t2 * a);
				x = fy(y);
				rxy(x, y);
			}
		}
		else {
			// two intersects at most
			d = Func::Sqrt(d);
			if (bnz) {
				x = (-b + d) / (Consts::t2 * a);
				y = fx(x);
				rxy(x, y);
				x = (-b - d) / (Consts::t2 * a);
				y = fx(x);
				rxy(x, y);
			}
			else {
				y = (-b + d) / (Consts::t2 * a);
				x = fy(y);
				rxy(x, y);
				y = (-b - d) / (Consts::t2 * a);
				x = fy(y);
				rxy(x, y);
			}
		}

		return res;
	}

	inline
	Vec2	PlanePointIntersection(Vec2 const rayVector, Vec2 const rayPoint, Vec2 const planeNormal, Vec2 const planePoint)
	{
		Vec2 const diff = rayPoint - planePoint;
		Scalar const prod1 = glm::dot(diff, planeNormal);
		Scalar const prod2 = glm::dot(rayVector, planeNormal);
		Scalar const prod3 = prod1 / prod2;
		return rayPoint - rayVector * prod3;
	}
}
