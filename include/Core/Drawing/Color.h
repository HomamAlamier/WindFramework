#pragma once
namespace drawing {
	struct Color
	{
	public:
		float R, G, B, A = 1;
		Color() {
			R = 0;
			G = 0;
			B = 0;
		}
		Color(int R, int G, int B, int A = 255) {
			this->R = R / 255.0f;
			this->G = G / 255.0f;
			this->B = B / 255.0f;
			this->A = A / 255.0f;
			if (this->R > 1) this->R = 1;
			if (this->R < 0) this->R = 0;
			if (this->G > 1) this->G = 1;
			if (this->G < 0) this->G = 0;
			if (this->B > 1) this->B = 1;
			if (this->B < 0) this->B = 0;
			if (this->A > 1) this->A = 1;
			if (this->A < 0) this->A = 0;
		}
		Color operator+(Color _c){
			return Color(R + _c.R, G + _c.G, B + _c.B, A + _c.A);
		}
		Color operator-(Color _c){
			return Color(R - _c.R, G - _c.G, B - _c.B, A - _c.A);
		}
		bool operator>(Color _c){
			return R + G + B + A > _c.R + _c.G + _c.B + _c.A;
		}
		bool operator<(Color _c){
			return R + G + B + A < _c.R + _c.G + _c.B + _c.A;
		}
		static Color Max(Color c1, Color c2){
			if (c1 > c2) return c1;
			else if (c1 < c2) return c2;
			else return c1;
		}
		static Color Min(Color c1, Color c2){
			if (c1 > c2) return c2;
			else if (c1 < c2) return c1;
			else return c1;
		}
		static Color White() { return Color(255, 255, 255); }
		static Color Black() { return Color(0, 0, 0); }
		static Color Red() { return Color(255, 0, 0); }
		static Color Green() { return Color(0, 255, 0); }
		static Color Blue() { return Color(0, 0, 255); }
		static Color Transparent() { return Color(0, 0, 0, 0); }
		static Color Translate(Color from, Color to, float t) {
			Color c;
			if (from.R != to.R) c.R = from.R > to.R ? from.R - t : from.R + t;
			if (from.G != to.G) c.G = from.G > to.G ? from.G - t : from.G + t;
			if (from.B != to.B) c.B = from.B > to.B ? from.B - t : from.B + t;
			if (from.A != to.A) c.A = from.A > to.A ? from.A - t : from.A + t;
			if (c.R > 1.0f) c.R = 1.0f;
			if (c.R < 0.0f) c.R = 0.0f;
			if (c.G > 1.0f) c.G = 1.0f;
			if (c.G < 0.0f) c.G = 0.0f;
			if (c.B > 1.0f) c.B = 1.0f;
			if (c.B < 0.0f) c.B = 0.0f;
			if (c.A > 1.0f) c.A = 1.0f;
			if (c.A < 0.0f) c.A = 0.0f;
			return c;
		}
	};
}