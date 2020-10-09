#include <Core/Controls/TextInput.h>
#include <Core/OSWindow/OSWindow.h>
#include <codecvt>
#define LOG_FN "textInput"
#include <Core/Services/LogService.h>
#ifdef __ANDROID__
#define GLFW_KEY_ENTER 66
#define GLFW_KEY_RIGHT 22
#define GLFW_KEY_LEFT 21
#define GLFW_KEY_UP 500
#define GLFW_KEY_DOWN 500
#define GLFW_KEY_BACKSPACE 67
#define GLFW_KEY_END 500
#define GLFW_KEY_HOME 500
#endif


using String = std::u32string;
namespace controls {
	TextInput::TextInput(drawing::Rectangle rect, void* window) : BaseControl(rect, window) {
		#ifndef __ANDROID__
			ptr = (GLFWwindow*)((windowsys::oswindow*)window)->nativeWindowPointer();
			cursor = GLFW_IBEAM_CURSOR;
		#endif
		DrawingSetup();
		/*ge2 = new drawing::GraphicsElement(rect);
		ge2->Clear();*/
		lines.push_back(t_line());
		auto ii = ge->AddRectangle(drawing::Rectangle(), drawing::Color::Green(), true);
		ge->Disable(ii);
		line_rect_index.push_back(ii);
		::controls::
		w_timer* timer = new w_timer(500);
		timer->set_infinite(true);
		timer->set_tick([&](void*){
			if (!Focused() || typing) return;
			if (lptren){
				ge->Disable(lptrInd);
				Invalidate();
				lptren = false;
			}else{
				ge->Enable(lptrInd);
				Invalidate();
				lptren = true;
			}
		});
		((windowsys::oswindow*)window)->addTimer(timer);
		timer->start();
		ty_timer = new controls::w_timer(500);
		ty_timer->set_tick([&](void*){ typing = false; });
		((windowsys::oswindow*)window)->addTimer(ty_timer);
	}
	void TextInput::DrawingSetup() {
		ge->Clear();
		ge->AddRectangle(drawing::Rectangle(0.0f, 0.0f, rect.width, rect.height), bg, true);
		lptrInd = ge->AddLine(drawing::Point(0, 0), drawing::Point(0, 0), drawing::Color::Blue());
		ge->Disable(lptrInd);
	}
	void TextInput::Paint(int vpWidth, int vpHeight) {
		glViewport(rect.x, (vpHeight - rect.height) - rect.y, rect.width, rect.height);
		float py = m_pointer.y + font->Height() >= rect.height ? rect.height - font->Height() : m_pointer.y;
		float px = m_pointer.x + (m_pointer.x > rect.width ? (rect.width - (m_pointer.x + 10)) : 0);
		ge->ModifyLine(lptrInd, drawing::Point(px, py)
			, drawing::Point(px, py + font->Height() + 5), drawing::Color::Blue());
		ge->Draw(rect.width, rect.height);
		//ge2->Draw(rect.width, rect.height);
		if (e_text){
			tb->~TextBlock();
			delete tb;
			tb = new drawing::TextBlock(font->RenderTextBlock(Text()));
			e_text = false;
		}
		if (tb != NULL)
			tb->Render(m_pointer.x > rect.width ? (rect.width - (m_pointer.x + 10)) : 0, m_pointer.y + font->Height() >= rect.height ? font->Height() * m_pointer.line + 5 : ((rect.height / 21) - 1) * 21, rect.width, rect.height, drawing::Color::Red());
		glViewport(0, 0, vpWidth, vpHeight);

	}
	void TextInput::InsertText(String tex){

	}
	void TextInput::InsertChar(char32_t tex){
		auto line = &lines[ m_pointer.line ];
		line->insert(tex, m_pointer.index, font);
		m_pointer.index++;
		auto sz = font->MeasureText(line->content.substr(0, m_pointer.index));
		line->rect.height = font->Height();
		m_pointer.x = sz.x;
		m_pointer.y = m_pointer.line * font->Height();
		m_pointer.vIndex = ULLONG_MAX;
		e_text = true;
		Invalidate();
	}
	void TextInput::KeyCharPress(KeyArgs e) {
		InsertChar(e.Char);
		if (!lptren){
			ge->Enable(lptrInd);
			lptren = true;
		}
		typing = true;
		ty_timer->start();
	}
	void TextInput::LostFocus(){
		ge->Disable(lptrInd);
	}
	void TextInput::GotFocus(){
		//ltime = glfwGetTime();
	}
	void TextInput::KeyPress(KeyArgs e) {
		if (e.Key == GLFW_KEY_ENTER){
			if (m_pointer.line == lines.size() - 1){
				InsertChar('\n');
				lines.push_back(t_line());
				auto ii = ge->AddRectangle(drawing::Rectangle(), drawing::Color::Green(), true);
				ge->Disable(ii);
				line_rect_index.push_back(ii);
			}
			else {
				std::vector<t_line> tmp;
				for (size_t i = 0; i < lines.size(); i++)
				{
					if (i == m_pointer.line){
						t_line tl;
						auto ln = &lines[i];
						if (m_pointer.index < ln->content.size() - 1){
							auto st = ln->content.substr(m_pointer.index, ln->content.size() - 1 - m_pointer.index);
							tl.content += st;
							ln->remove(m_pointer.index, ln->content.size() - 1 - m_pointer.index, font);
						}			
						tmp.push_back(t_line(*ln));
						tl.insert('\n', tl.content.size(), font);
						tmp.push_back(tl);
					}
					else{
						tmp.push_back(t_line(lines[i]));
					}
				}
				lines = tmp;
			}
			m_pointer.line++;
			m_pointer.index = 0;
			m_pointer.x = 0;
			m_pointer.y = m_pointer.line * font->Height();
			e_text = true;
			m_pointer.vIndex = ULLONG_MAX;
			Invalidate();
		}
		else if (e.Key == GLFW_KEY_LEFT){
			if (m_pointer.index > 0) m_pointer.index--;
			else if (m_pointer.index == 0 && m_pointer.line > 0) {
				m_pointer.index = lines[m_pointer.line - 1].content.size() - 1;
				m_pointer.line--;
			}
			auto sz = font->MeasureText(lines[m_pointer.line].content.substr(0, m_pointer.index));
			m_pointer.x = sz.x;
			m_pointer.y = m_pointer.line * font->Height();
			m_pointer.vIndex = ULLONG_MAX;
			Invalidate();
		}
		else if (e.Key == GLFW_KEY_RIGHT){
			auto ln = &lines[m_pointer.line];
			if (m_pointer.index < ln->content.size() - 1 || (m_pointer.index < ln->content.size() && m_pointer.line == lines.size() - 1)){
				m_pointer.index++;
			}
			else if (m_pointer.index == ln->content.size() - 1 && lines.size() - 1 > m_pointer.line){
				m_pointer.index = 0;
				m_pointer.line++;
			}
			auto sz = font->MeasureText(ln->content.substr(0, m_pointer.index));
			m_pointer.x = sz.x;
			m_pointer.y = m_pointer.line * font->Height();
			m_pointer.vIndex = ULLONG_MAX;
			Invalidate();
		}
		else if (e.Key == GLFW_KEY_UP){
			if (m_pointer.vIndex == ULLONG_MAX) m_pointer.vIndex = m_pointer.index;
			if (m_pointer.line > 0) {
				m_pointer.line--;
				auto ln = &lines[ m_pointer.line ];
				if (ln->content.size() <= m_pointer.vIndex) m_pointer.index = ln->content.size() - (m_pointer.line == lines.size() - 1 ? 0 : 1);
				else if (ln->content.size() > m_pointer.vIndex) m_pointer.index = m_pointer.vIndex;
				auto sz = font->MeasureText(ln->content.substr(0, m_pointer.index));
				m_pointer.x = sz.x;
				m_pointer.y = m_pointer.line * font->Height();
				Invalidate();
			}
		}
		else if (e.Key == GLFW_KEY_DOWN){
			if (m_pointer.vIndex == ULLONG_MAX) m_pointer.vIndex = m_pointer.index;
			if (m_pointer.line < lines.size() - 1) {
				m_pointer.line++;
				auto ln = &lines[ m_pointer.line ];
				if (ln->content.size() <= m_pointer.vIndex) m_pointer.index = ln->content.size() - (m_pointer.line == lines.size() - 1 ? 0 : 1);
				else if (ln->content.size() > m_pointer.vIndex) m_pointer.index = m_pointer.vIndex;
				auto sz = font->MeasureText(ln->content.substr(0, m_pointer.index));
				m_pointer.x = sz.x;
				m_pointer.y = m_pointer.line * font->Height();
				Invalidate();
			}
		}
		else if (e.Key == GLFW_KEY_BACKSPACE){
			auto ln = &lines[m_pointer.line];
			if (m_pointer.index > 0) {
				ln->remove(m_pointer.index - 1, 1, font);
				m_pointer.index--;
				e_text = true;
				
			}
			else if (m_pointer.index == 0 && m_pointer.line > 0){
				if (ln->content.size() > 0){
					auto ln2 = &lines[m_pointer.line - 1];
					ln2->remove(ln2->content.size() - 1, 1, font);					
					m_pointer.index = ln2->content.size();
					ln2->content += ln->content;
					lines.erase(lines.begin() + m_pointer.line);
					m_pointer.line--;
					e_text = true;
				}
				else{
					auto ln2 = &lines[m_pointer.line - 1];
					ln2->remove(ln2->content.size() - 1, 1, font);
					lines.erase(lines.begin() + m_pointer.line);
					m_pointer.line--;
					m_pointer.index = ln2->content.size();
					e_text = true;
				}
			}
			ln = &lines[m_pointer.line];
			auto sz = font->MeasureText(ln->content.substr(0, m_pointer.index));
			m_pointer.x = sz.x;
			m_pointer.y = m_pointer.line * font->Height();
			m_pointer.vIndex = ULLONG_MAX;
			Invalidate();
		}
		else if (e.Key == GLFW_KEY_END){
			auto ln = &lines[m_pointer.line];
			if (m_pointer.index != ln->content.size() - 1){
				m_pointer.index = ln->content.size() - 1;
				auto sz = font->MeasureText(ln->content.substr(0, m_pointer.index));
				m_pointer.x = sz.x;
				m_pointer.y = m_pointer.line * font->Height();
				m_pointer.vIndex = ULLONG_MAX;
				Invalidate();
			}
		}
		else if (e.Key == GLFW_KEY_HOME){
			auto ln = &lines[m_pointer.line];
			if (m_pointer.index != 0){
				m_pointer.index = 0;
				auto sz = font->MeasureText(ln->content.substr(0, m_pointer.index));
				m_pointer.x = sz.x;
				m_pointer.y = m_pointer.line * font->Height();
				m_pointer.vIndex = ULLONG_MAX;
				Invalidate();
			}
		}
	}
	void TextInput::MouseUp(windowsys::MouseArgs e) {
		double x = e.x() - rect.x;
		double y = e.y() - rect.y;
		int ind = -1;
		if (x >= down.x - 10 && x <= down.x + 10)
		{
			for(auto ln : lines){
				ind++;
				double lny = ind * font->Height();
				if (lny <= y && lny + ln.rect.height >= y){
					auto pind = ln.content.getCharIndex(x);
					Log_WriteLine("Estimated clicked char index ~= " + std::to_string(pind) + " at line : " + std::to_string(ind + 1), LogType::Debug, false);
					m_pointer.line = ind;
					m_pointer.index = pind;
					m_pointer.x = ln.content.width(0, m_pointer.index);
					m_pointer.y = m_pointer.line * font->Height();
					m_pointer.vIndex = ULLONG_MAX;
					break;
				}
			}
		}
		/*else {
			size_t sline = 0, eline = 0, sind = 0, eind = 0;
			bool sbool = false, ebool = false;
			double dx = down.x - rect.x, dy = down.y - rect.y;
			for (size_t i = 0; i < lines.size(); i++)
			{
				if (dy > i * font->Height() && dy < i * font->Height() + lines[i].rect.height){
					sline = i;
					sind = lines[i].content.getCharIndex(dx);
					sbool = true;
				}
				if (y > i * font->Height() && y < i * font->Height() + lines[i].rect.height){
					eline = i;
					eind = lines[i].content.getCharIndex(x);
					ebool = true;
				}
				if (sbool && ebool) break;
			}
			Log_WriteLine("Estimated highlight text index's from (" + std::to_string(sind) + " at " 
				+ std::to_string(sline) + ") To (" + std::to_string(eind) + " at " + std::to_string(eline) + ")", LogType::Debug, true);
			if (sline == eline){
				auto ln = &lines[sline];
				auto sw = ln->content.width(0, sind), ew = ln->content.width(0, eind);
				ge->ModifyRectangle(2 + sline, drawing::Rectangle(sw, 0, ew - sw, ln->rect.height + 2), drawing::Color::Green(), true);
				ge->Enable(2 + sline);
			}
		}*/
	}
	void TextInput::MouseMove(windowsys::MouseArgs e){
		double x = e.x() - rect.x;
		double y = e.y() - rect.y;
		if (e.key() == windowsys::mouseKey::left && e.action() == windowsys::mouseKeyAction::press) {
			ge->Disable(2, lines.size());
			size_t eind = 0, exs = 0;
			for (size_t i = 0; i < lines.size(); i++)
			{
				if (y > i * font->Height() && y < i * font->Height() + lines[i].rect.height){
					m_selector.endLine = i;
					eind = lines[i].content.getCharIndex(x);
					exs = lines[i].content.width(0, eind);
					break;
				}
			}
			/*Log_WriteLine("Estimated highlight text index's from (" + std::to_string(sind) + " at " 
				+ std::to_string(sline) + ") To (" + std::to_string(eind) + " at " + std::to_string(eline) + ")", LogType::Debug, true);*/
			if (m_selector.startLine == m_selector.endLine && m_selector.endIndex != eind){
				m_selector.endIndex = m_selector.endLine;
				m_selector.endX = exs;
				auto ln = &lines[m_selector.endLine];
				ge->ModifyRectangle(2 + m_selector.endLine, drawing::Rectangle(m_selector.startX, 0, m_selector.endX - m_selector.startX, ln->rect.height + 2), drawing::Color::Green(), true);
				ge->Enable(2 + m_selector.endLine);
			}
			else {
				m_selector.endIndex = eind;
				m_selector.endX = exs;
				size_t sl, sx, el, ex;
				if (m_selector.startLine > m_selector.endLine){
					sl = m_selector.endLine;
					sx = m_selector.endX;
					el = m_selector.startLine;
					ex = m_selector.startX;
				}
				else {
				    el = m_selector.endLine;
					ex = m_selector.endX;
					sl = m_selector.startLine;
					sx = m_selector.startX;
				}
				for (size_t i = sl; i <= el; i++)
				{
					if (i == sl) {
						ge->ModifyRectangle(2 + sl, drawing::Rectangle(sx, sl * font->Height(), lines[sl].rect.width - sx, font->Height()), drawing::Color::Green(), true);
						ge->Enable(2 + sl);
					}
					else if (i == el) {
						ge->ModifyRectangle(2 + el, drawing::Rectangle(0, el * font->Height(), ex, font->Height()), drawing::Color::Green(), true);
						ge->Enable(2 + el);
					}
					else{
						ge->ModifyRectangle(2 + i, drawing::Rectangle(0, i * font->Height(), lines[i].rect.width, font->Height()), drawing::Color::Green(), true);
						ge->Enable(2 + i);
					}
				}
				
			}
			Invalidate();
		}
	}
	void TextInput::MouseDown(windowsys::MouseArgs e) {
		down = drawing::PointD(e.x(), e.y());
		
		double dx = down.x - rect.x, dy = down.y - rect.y;
		for (size_t i = 0; i < lines.size(); i++)
		{
			if (dy > i * font->Height() && dy < i * font->Height() + lines[i].rect.height){
				m_selector.startLine = i;
				m_selector.startIndex = lines[i].content.getCharIndex(dx);
				m_selector.startX = lines[i].content.width(0, m_selector.startIndex);
				break;
			}
		}
        #ifdef __ANDROID__
            windowsys::call_android_show_im();
        #endif
	}
	void TextInput::set_Font(drawing::Font* font) {
		this->font = font;
		tb = new drawing::TextBlock(font->RenderTextBlock(Text()));	
	}
	String TextInput::Text(){
		String tmp;
		for(auto x : lines){
			tmp += x.content.str;
		}
		return tmp;
	}
	//asdadsd
}