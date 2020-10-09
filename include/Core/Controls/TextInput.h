#ifndef TEXTINPUT_H
#define TEXTINPUT_H


#include <Core/Controls/BaseControl.h>
#include <Core/Controls/w_Timer.h>
#include <Core/Drawing/Font.h>
namespace controls {
	struct t_pointer{
		size_t index = 0;
		size_t vIndex = 0;
		size_t line = 0;
		float x = 0.0f;
		float y = 0.0f;
	};
	struct t_selector{
		size_t startLine = 0;
		size_t endLine = 0;
		size_t startIndex = 0;
		size_t endIndex = 0;
		size_t startX = 0;
		size_t endX = 0;
	};
	struct StringContainer{
		std::u32string str;
		std::vector<drawing::f_Char*> metrics;
		drawing::Font* fnt = NULL;
		void updateMetrics(drawing::Font* fnt){
			metrics.clear();
			if (fnt == NULL) return;
			for(auto ch : str){
				metrics.push_back(fnt->GetCharMetrics(ch));
			}
			this->fnt = fnt;
		}
		void insert(size_t pos, char32_t ch){
			str.insert(str.begin() + pos, 1, ch);
			if (fnt != NULL) metrics.push_back(fnt->GetCharMetrics(ch));
		}
		void remove(size_t pos, size_t len){
			str.erase(str.begin() + pos, str.begin() + pos + len);
			if (fnt != NULL) metrics.erase(metrics.begin() + pos, metrics.begin() + pos + len);
		}
		float width(){
			float x = 0;
			for(auto m : metrics)
				x += m->rect.width + 1;
			return x;
		}
		float width(size_t index){
			return metrics[index]->rect.width;
		}
		float width(size_t pos, size_t len){
			float x = 0;
			for (size_t i = pos; i < pos + len; i++)
			{
				x += metrics[i]->rect.width + 1;
			}
			return x;
		}
		size_t getCharIndex(double x){	
			size_t xx = 0;
			for (size_t i = 0; i < metrics.size(); i++)
			{
				auto m = metrics[i];
				auto w = m->rect.width + m->bearing.x;
				if (xx + (w / 2) > x) return i;
				else if (xx + w > x) return i + 1;
				xx += w;
			}
			return str.length();
		}
		void operator+=(StringContainer sc){
			str += sc.str;
			if (fnt != NULL) 
				for(auto ch : sc.str)
					metrics.push_back(fnt->GetCharMetrics(ch));
		}
		void operator+=(std::u32string s){
			str += s;
			if (fnt != NULL) 
				for(auto ch : s)
					metrics.push_back(fnt->GetCharMetrics(ch));
		}
		void operator+=(char32_t s){
			str += s;
			if (fnt != NULL) 
				metrics.push_back(fnt->GetCharMetrics(s));
		}
		char32_t operator[](int ind) { return str[ind]; }
		size_t size() { return str.size(); }
		std::u32string substr(size_t pos, size_t len) { return str.substr(pos, len); }
		drawing::Font* getFont() { return fnt; }
		void setFont(drawing::Font* ptr) { fnt = ptr; }
	};
	struct t_line {
		StringContainer content;
		drawing::Rectangle rect;
		void insert(char32_t ch, size_t index, drawing::Font* fnt){
			content.setFont(fnt);
			content.insert(index, ch);
			rect.width = content.width();
		}
		void remove(size_t index, size_t count, drawing::Font* fnt){
			content.setFont(fnt);
			content.remove(index, count);
			rect.width = content.width();
		}
	};
	class TextInput : public BaseControl {
		void* ptr;
		//std::u32string text;
		bool e_text = false, lptren = false, typing = false;
		drawing::TextBlock* tb;
		drawing::PointD down;
		std::vector<t_line> lines;
		std::vector<size_t> line_rect_index;
		t_pointer m_pointer;
		t_selector m_selector;
		size_t lineStartIndex = 0, lptrInd = 0;
		double ltime = 0;
		controls::w_timer* ty_timer;
		drawing::GraphicsElement* ge2;
	public:
		TextInput(drawing::Rectangle rect, void* window);
		void DrawingSetup();
		void LostFocus();
		void GotFocus();
		void KeyCharPress(KeyArgs e);
		void KeyPress(KeyArgs e);
		void MouseDown(windowsys::MouseArgs e);
		void MouseUp(windowsys::MouseArgs e);
		void MouseMove(windowsys::MouseArgs e);
		void Paint(int vpWidth, int vpHeight);
		void InsertText(std::u32string tex);
		void InsertChar(char32_t ch);
		std::u32string Text();
		void set_Font(drawing::Font* font);
	};
}
#endif // !TEXTINPUT_H
