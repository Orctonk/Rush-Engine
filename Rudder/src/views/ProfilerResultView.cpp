#include "Rudderpch.h"
#include "ProfilerResultView.h"

#include "widgets/FileBrowser.h" 

#include <cstring>
#include <stack>

ProfilerResultView::ProfilerResultView(){
    m_CurFile = "None";
    m_FirstDisplayedTime = 1000000;
    m_DisplayedDuration = 5000000;
    enabled = false;
}

ProfilerResultView::~ProfilerResultView(){

}

void ProfilerResultView::OnImguiRender(){
    if(!enabled) return;

    if(ImGui::Begin("Profiler result",&enabled)){
        m_Focused = ImGui::IsWindowFocused();
        ImGui::Text("Current File: ");
        ImGui::SameLine();
        ImGui::Text(m_CurFile.c_str());
        
        static FileBrowser profBrowser;
        if(ImGui::Button("Browse")){
            profBrowser.Open();
            profBrowser.SetTitle("Select profiler result...");
        }
        profBrowser.Render();
        if(profBrowser.Finished()){
            m_CurFile = profBrowser.GetSelectedFile();
            LoadProfileResult(m_CurFile);
        }
        ImGui::Separator();

        float width = ImGui::GetWindowContentRegionWidth();
        int level = 0;
        auto p = ImGui::GetCursorScreenPos();
        auto drawList = ImGui::GetWindowDrawList();
        std::stack<uint64_t> ends;

        const float scale = m_DisplayedDuration / width;

        for(Sample &s : m_Samples){
            if(s.start + s.dur < m_FirstDisplayedTime || (s.dur/scale) < 1.0f)
                continue;
            if(s.start > m_FirstDisplayedTime + m_DisplayedDuration)
                break;

            uint64_t sEnd = s.start + s.dur;
            float xOff = (((float)s.start) - m_FirstDisplayedTime)/scale;
            while(!ends.empty() && ends.top() < sEnd){
                ends.pop();
                level--;
            }
            auto hash = std::hash<std::string>{}(m_SampleNameMap[s.id]);
            uint32_t col = (uint32_t)(0x000000FF | hash);
            drawList->AddRectFilled({p.x + xOff,p.y + level * 22},{p.x + xOff + (s.dur/scale),p.y + 20 * (level + 1) + 2 * level},col);
            const char *rem;
            ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(),(s.dur/scale),0.0f,m_SampleNameMap[s.id].c_str(),NULL,&rem);
            drawList->AddText({p.x + xOff,p.y + level * 22},0xFFFFFFFF,m_SampleNameMap[s.id].c_str(),rem);
            ends.push(sEnd);
            level++;
        }
    }
    ImGui::End();
}

void ProfilerResultView::OnEvent(Rush::Event &e){
    e.Dispatch<Rush::KeyboardPressEvent>(RUSH_BIND_FN(ProfilerResultView::KeyPressHandle));
    e.Dispatch<Rush::KeyboardRepeatEvent>(RUSH_BIND_FN(ProfilerResultView::KeyRepeatHandle));
    e.Dispatch<Rush::MouseScrollEvent>(RUSH_BIND_FN(ProfilerResultView::ScrollHandle));
}

void ProfilerResultView::LoadProfileResult(std::string path){
    m_SampleNameMap.clear();
    m_Samples.clear();
    std::ifstream profile;
    profile.open(path);

    char buf[1024];
    std::unordered_map<std::string, uint32_t> idMap;

    profile.getline(buf,1024,'{');

    bool done = false;
    uint32_t curId = 0;
    while(!done){
        Sample s;
        // TODO: switch to more robust JSON parser
        profile.getline(buf,1024,'{');
        profile.get(buf,9);
        profile.getline(buf,1024,'"');
        if(idMap.find(std::string(buf)) != idMap.end()){
            s.id = idMap[buf];
        }
        else{
            s.id = curId;
            idMap[std::string(buf)] = curId;
            m_SampleNameMap[curId] = std::string(buf);
            curId++;
        }

        profile.get(buf,8);
        profile.getline(buf,1024,',');
        s.start = atoi(buf);
        profile.getline(buf,1024,':');
        profile.getline(buf,1024,'}');
        s.dur = atoi(buf);
        profile.get(buf,2);
        done = buf[0] == ']';
        if(buf[0] == '\0'){
            RUSH_LOG_ERROR("Attempting to open incomplete or invalid profiler file!");
            done = true;
        }
        m_Samples.push_back(s);
    }
    profile.close();
    std::sort(m_Samples.begin(),m_Samples.end(),[](const Sample &a, const Sample &b){
        return a.start < b.start;
    });
}

bool ProfilerResultView::KeyPressHandle(Rush::KeyboardPressEvent &e){
    if(!m_Focused)
        return false;
    
    switch(e.keycode){
        case RUSH_KEY_W:
            m_FirstDisplayedTime += m_DisplayedDuration * 0.05f;
            m_DisplayedDuration *= 0.9f;
            return true;
        case RUSH_KEY_S:
            m_FirstDisplayedTime -= m_DisplayedDuration * 0.05f;
            m_DisplayedDuration /= 0.9f;
            return true;

        case RUSH_KEY_D:
            m_FirstDisplayedTime += m_DisplayedDuration * 0.1f;
            return true;

        case RUSH_KEY_A:
            m_FirstDisplayedTime += m_DisplayedDuration * -0.1f;
            return true;
        default:
            return false;
    }
}

bool ProfilerResultView::KeyRepeatHandle(Rush::KeyboardRepeatEvent &e){
    if(!m_Focused)
        return false;
    
    switch(e.keycode){
        case RUSH_KEY_W:
            m_FirstDisplayedTime += m_DisplayedDuration * 0.05f;
            m_DisplayedDuration *= 0.9f;
            return true;
        case RUSH_KEY_S:
            m_FirstDisplayedTime -= m_DisplayedDuration * 0.05f;
            m_DisplayedDuration /= 0.9f;
            return true;

        case RUSH_KEY_D:
            m_FirstDisplayedTime += m_DisplayedDuration * 0.1f;
            return true;

        case RUSH_KEY_A:
            m_FirstDisplayedTime += m_DisplayedDuration * -0.1f;
            return true;
        default:
            return false;
    }
}

bool ProfilerResultView::ScrollHandle(Rush::MouseScrollEvent &e){
    if(!m_Focused)
        return false;

    if(e.delta >= 0)
        for(int i = 0; i < e.delta; i++){
            m_FirstDisplayedTime += m_DisplayedDuration * 0.1f;
            m_DisplayedDuration *= 0.8f;
        }
    else
        for(int i = 0; i > e.delta; i--){
            m_FirstDisplayedTime -= m_DisplayedDuration * 0.1f;
            m_DisplayedDuration /= 0.8f;
        }

    return true;
}