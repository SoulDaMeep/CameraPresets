#include "pch.h"
#include "GuiBase.h"
#include "CameraPresets.h"




namespace fs = std::filesystem;
std::string SettingsWindowBase::GetPluginName() {
    return "CameraPresets";
}

void SettingsWindowBase::SetImGuiContext(uintptr_t ctx) {
    ImGui::SetCurrentContext(reinterpret_cast <ImGuiContext*> (ctx));
}

std::string PluginWindowBase::GetMenuName() {
    return "CameraPresets";
}

std::string PluginWindowBase::GetMenuTitle() {
    return menuTitle_;
}

void CameraPresets::RenderSettings() {

    if (ImGui::Button("Open Presets Window")) gameWrapper->Execute([this](GameWrapper* gw) {
        _globalCvarManager->executeCommand("togglemenu " + GetMenuName());
    });

    if (ImGui::IsItemHovered()) {
        
        ImGui::SetTooltip("Bind: F1");
        
    }

}

void CameraPresets::RenderWindow() {



    // Window styling to make it xtruh kewl


    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiStyle& CameraPresetsStyle = ImGui::GetStyle();
    backup = style;
    ImVec4* colors = CameraPresetsStyle.Colors;



    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);

    colors[ImGuiCol_Border] = ImVec4{17.f / 255.f, 167.f / 255.f, 140.f/ 255.f, 0.20f};
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);

    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    
    colors[ImGuiCol_TitleBg] = primary;
    colors[ImGuiCol_TitleBgActive] = accent;
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_SliderGrab] = primary;
    colors[ImGuiCol_SliderGrabActive] = accent;

    colors[ImGuiCol_Button] = primary;
    colors[ImGuiCol_ButtonHovered] = hover;
    colors[ImGuiCol_ButtonActive] = accent;

    colors[ImGuiCol_Header] = primary;
    colors[ImGuiCol_HeaderHovered] = hover;
    colors[ImGuiCol_HeaderActive] = accent;

    colors[ImGuiCol_Separator] =    primary;
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);

    colors[ImGuiCol_Tab] = primary;
    colors[ImGuiCol_TabHovered] = hover;
    colors[ImGuiCol_TabActive] = accent;
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

    CameraPresetsStyle.FrameRounding = 4.0f;
    CameraPresetsStyle.GrabRounding = 2.0f;
    CameraPresetsStyle.TabRounding = 4.0f;
    CameraPresetsStyle.WindowRounding = 6.0f;
    CameraPresetsStyle.ScrollbarRounding = 3.0f;
    CameraPresetsStyle.ItemSpacing = ImVec2(8.0f, 6.0f);
    CameraPresetsStyle.WindowPadding = ImVec2(10.0f, 10.0f);
    CameraPresetsStyle.FramePadding = ImVec2(6.0f, 4.0f);
    CameraPresetsStyle.ScrollbarSize = 14.0f;
    CameraPresetsStyle.TabBorderSize = 0.0f;
    CameraPresetsStyle.WindowBorderSize = 0.0f;

    style = CameraPresetsStyle;
    
    /// \Notes takes player from file and turns them into CP_CameraPreset struct

    std::fstream inputFile(gameWrapper->GetDataFolder() / "cameras_rlcs.data", std::ios::in);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string playerName;

            std::vector <std::string> values;
            iss >> playerName;

            if (playerName.empty()) break;

            std::string word;

            while (iss >> word) values.push_back(word);

            bool playerExists = false;
            for (const auto& existingCamera : cameras) {
                if (existingCamera.name == playerName) {
                    playerExists = true;
                    break;
                }
            }

            if(!playerExists){
                CP_CameraSettings camera = {
                    playerName,
                    std::stoi(values[0]),
                    std::stoi(values[5]),
                    std::stoi(values[1]),
                    std::stoi(values[2]),
                    std::stof(values[3]),
                    std::stof(values[6]),
                    std::stof(values[4])
                };
                cameras.push_back(camera);
            }
        }
        inputFile.close();
    }
    else {
        LOG("[CameraPresets] Could not open file {}", "cameras_rlcs.data");
    }

    // if there are 2 of the same name, give one of them a number
    std::unordered_map<std::string, int> cams;

    for (CP_CameraSettings& cam : cameras) {
        std::string originalName = cam.name;
        while (cams.find(cam.name) != cams.end()) {
            cam.name = originalName + std::to_string(++cams[originalName]-1);
        }
        cams[originalName]++;
    }

    if (settingsChanged) {
        std::string data;
        for (CP_CameraSettings camera : cameras) {
            data += CreateSettingString(camera);
        }
        SaveToFile(data, gameWrapper->GetDataFolder() / "cameras_rlcs.data");
        DumpSave(data);
        settingsChanged = false;
    }

    ImGui::BeginTabBar("Tab");
    if(ImGui::BeginTabItem("Presets")) {
        /////////////////////////////////////////////////////////Left Panel of Presets/////////////////////////////////////////////////////////
        ImGui::BeginChild("left pane", ImVec2(250, -ImGui::GetFrameHeightWithSpacing()), true);
        ImGui::Columns(3, "MovementButtons", true);
        ImGui::SetColumnWidth(0, 175);
        ImGui::InputText("Search", &SearchName);
        ImGui::NextColumn();
        ImGui::SetColumnWidth(1, 35);
       
        if (!HideMovementButtons) {
            // if the preset is at the top, dont allow button up
            //swap lower and current index
            //decrease index to keep at same preset index
            if (selected > 0 && ImGui::ArrowButton(("up" + std::to_string(selected)).c_str(), ImGuiDir_Up)) {
                std::swap(cameras[selected], cameras[selected - 1]);
                settingsChanged = true;
                selected -= 1;
            }
        }
        ImGui::NextColumn();
        ImGui::SetColumnWidth(2, 35);

        if (!HideMovementButtons) {
            // if the preset is at the bottom, dont allow button down
            //swap lower and current index
            //increase index to keep at same preset index
            if (selected < cameras.size() - 1 && ImGui::ArrowButton(("down" + std::to_string(selected)).c_str(), ImGuiDir_Down)) {
                std::swap(cameras[selected], cameras[selected + 1]);
                settingsChanged = true;
                selected += 1;
            }
        }
        ImGui::Columns(1);

        ////////////////////////////////////////////Show Camera Presets in Side panel////////////////////////////////////////////
        if(cameras.empty()) HideMovementButtons = true;

        if(RenameBuffer.empty() && !cameras.empty()) RenameBuffer = cameras.at(selected).name;

        for (int i = 0; i < cameras.size(); i++)
        {
            auto& camera = cameras[i];

            //lowercase searching
            std::string TempName;
            for (char c : camera.name) {
                TempName += std::tolower(c);
            }

            if (SearchName.empty()) HideMovementButtons = false;
            else HideMovementButtons = true;

            
            if (TempName.find(SearchName) != std::string::npos) {
                ImGui::PushID(i);

                if (ImGui::Selectable(camera.name.c_str(), selected == i)) {
                    selected = i;
                    RenameBuffer = camera.name;
                }

                ImGui::SetNextWindowSize(ImVec2{275, 112});
                if (ImGui::BeginPopupContextWindow("Something"))
                {
                    ImGui::SeparatorText("Edit Name");
                    ImGui::InputText("##edit", &RenameBuffer);

                    if (InputNameError == 2) {
                        ImGui::TextColored(ImVec4{1, 1, 0, 1}, "Invalid Name: ( 20 Chars ) [ a-z A-z _.-^! ]");
                    }
                    else if (InputNameError == 3) {
                        ImGui::TextColored(ImVec4{1, 1, 0, 1}, "Name already exists!");
                    }
                    else ImGui::NewLine();
                    
                    if (ImGui::Button("Save")) {
                        const char* reg = R"(^[a-zA-Z0-9_.^!-]+$)";
                        std::regex pattern(reg);

                        if(cams.count(RenameBuffer) >= 1 || RenameBuffer == camera.name) {
                            InputNameError = 3;
                            LOG("input name error 3");
                        }
                        else if (std::regex_match(RenameBuffer, pattern) && RenameBuffer.length() <= 20) {
                            FRenamePlayer(camera.name, RenameBuffer);
                            InputNameError = 0;
                            ImGui::CloseCurrentPopup();
                        }
                        else InputNameError = 2;
                        
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
                ImGui::PopID();
            }
        }
        ImGui::EndChild();
        ////////////////////////////////////////////Editable Sliders////////////////////////////////////////////
        if (selected < cameras.size()) {
            ImGui::SameLine();
            if (oldSelected != selected) {
                CopyCodeMessage = false;
                oldSelected = selected;
            }
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, 225), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); // Leave room for 1 line below us

            ImGui::Text(cameras.at(selected).name.c_str());
            ImGui::Separator();
            ImGui::BeginChild("sliders", ImVec2(0, 225));

            if (ImGui::SliderInt("FOV", &cameras.at(selected).FOV, 60, 110)) {
                settingsChanged = true;
            }
            if (ImGui::SliderInt("Distance", &cameras.at(selected).Distance, 100, 400)) {
                cameras.at(selected).Distance = (cameras.at(selected).Distance / 10) * 10; // clamp between 10
                settingsChanged = true;

            }
            if (ImGui::SliderInt("Height", &cameras.at(selected).Height, 40, 200)) {
                cameras.at(selected).Height = (cameras.at(selected).Height / 10) * 10; // clamp between 10
                settingsChanged = true;
            }
            ImGui::SliderInt("Angle", &cameras.at(selected).Angle, -15, 0);
            if (ImGui::SliderFloat("Stiffness", &cameras.at(selected).Stiffness, 0.0f, 1.0f, "%.2f")) {
                cameras.at(selected).Stiffness = roundf(cameras.at(selected).Stiffness * 20) / 20;
                settingsChanged = true;

            }
            if (ImGui::SliderFloat("SwivelSpeed", &cameras.at(selected).SwivelSpeed, 1.0f, 10.0f, "%.2f")) {
                cameras.at(selected).SwivelSpeed = roundf(cameras.at(selected).SwivelSpeed * 10.0f) / 10.0f;
                settingsChanged = true;
            }
            if (ImGui::SliderFloat("TransitionSpeed", &cameras.at(selected).TransitionSpeed, 1.0f, 2.0f, "%.2f")) {
                cameras.at(selected).TransitionSpeed = roundf(cameras.at(selected).TransitionSpeed * 10.0f) / 10.0f;
                settingsChanged = true;
            }
            ImGui::EndChild();
            ImGui::EndChild();
            ////////////////////////////////////////////Copy Code Text////////////////////////////////////////////
            CP_CameraSettings cc = cameras.at(selected);
            std::string code = cc.name + '#'
                + std::to_string(cc.FOV) + '#'
                + std::to_string(cc.Height) + '#'
                + std::to_string(cc.Angle) + '#'
                + std::format("{}", cc.Stiffness) + '#'
                + std::format("{}", cc.TransitionSpeed) + '#'
                + std::to_string(cc.Distance) + '#'
                + std::format("{}", cc.SwivelSpeed) + '|';
            cc.code = code;

            ///////////////////Copy Code Button///////////////////
            if (ImGui::Button(("Copy: " + cc.code).c_str())) {
                ImGui::SetClipboardText(cc.code.c_str());
                CopyCodeMessage = true;
                gameWrapper->Execute([this](GameWrapper* gw) {
                    gameWrapper->SetTimeout([this](GameWrapper* gw) {
                        CopyCodeMessage = false;
                        }, 2);
                    // clear copy code message after 2 seconds
                });
            }
            if (CopyCodeMessage) ImGui::Text("Copied Code!");

            ImGui::EndGroup();

        }
        ////////////////////////////////////////////Add Preset Window////////////////////////////////////////////
        if (ImGui::Button("Add Camera Preset")) {
            gameWrapper->Execute([this](GameWrapper* gw) {
                ProfileCameraSettings settings = gameWrapper->GetSettings().GetCameraSettings();

                tempCamera.FOV = settings.FOV;
                tempCamera.Distance = settings.Distance;
                tempCamera.Angle = settings.Pitch;
                tempCamera.Height = settings.Height;
                tempCamera.Stiffness = settings.Stiffness;
                tempCamera.SwivelSpeed = settings.SwivelSpeed;
                tempCamera.TransitionSpeed = settings.TransitionSpeed;

            });
            CreatePreset = true;
        }

        ImGui::SameLine();
        ////////////////////////////////////////////Clear Button////////////////////////////////////////////
        if (ImGui::Button("Clear All Presets")) DeleteWindow = true;

        ImGui::SameLine();

        ////////////////////////////////////////////Delete Button////////////////////////////////////////////
        if (ImGui::Button("Delete")) {
            if (!cameras.empty()) {
                DeletePlayerFromFile(cameras.at(selected).name, gameWrapper->GetDataFolder() / "cameras_rlcs.data");
                std::string selectedName = cameras.at(selected).name;
                auto position = std::find_if(cameras.begin(), cameras.end(), [selectedName](const CP_CameraSettings& camera) {
                    return camera.name == selectedName;
                    });

                if (position != cameras.end()) {
                    cameras.erase(position);
                }

                //update settings
                settingsChanged = true;
            }
        }
        ImGui::EndTabItem();
    }
    ////////////////////////////////////////////Help / Info ////////////////////////////////////////////
    if (ImGui::BeginTabItem("Help/Info")) {
        ImGui::Text("CameraPresets is a plugin that adds functionality and a user friendly GUI to the original outdated pro settings implemented by bakkesmod");
        ImGui::Text("If you are a pro that wants their preset added to the list, upset because of a missing pro preset or if you found a bug, please contact me");
        ImGui::Text("Please do not manually edit external files associated to this plugin. Your game may crash on startup.");
        ImGui::SeparatorText("Codes");
        ImGui::Text("Guide: NAME#FOV#HEIGHT#ANGLE#STIFFNESS#TRANSITIONSPEED#DISTANCE#SWIVELSPEED | \nExample: Squishy#110#90# -5#0.5#1.00#270#6.00");
        ImGui::SeparatorText("Honorable Mentions:");
        ImGui::Text("- Kandda. - Concept and Thumbnail artist");
        ImGui::Text("- +left - Camera list lookup");
        ImGui::Text("- Liquipedia - Player Data");
        ImGui::SeparatorText("Play Testers");
        ImGui::Text("- sslowdev");
        ImGui::Text("- Rivques");
        ImGui::Text("- Trxxy");
        ImGui::SeparatorText("Socials");
        
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
        if (ImGui::Button("Github: @souldameep")) {
            system("start https://github.com/SoulDaMeep");
        }
        ImGui::SameLine();
        if(ImGui::Button("Discord Server")) {
            system("Start https://discord.gg/DuA6Efj54T");
        }
        
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();



    ////////////////////////////////////////////Delete Window Popup////////////////////////////////////////////
    if (DeleteWindow) {
        ImGui::Begin("Delete ALL Presets", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Are you sure you want to delete all your presets");
        ImGui::Text("These changes CANNOT be undone");
        if (ImGui::Button("Cancel")) DeleteWindow = false;
        
        ImGui::SameLine();
        if (ImGui::Button("Delete All")) {
            DeleteWindow = false;
            for (CP_CameraSettings cam : cameras) {
                DeletePlayerFromFile(cam.name, "cameras_rlcs.data");
            }
            cameras.clear();
            settingsChanged = true;
        }
        ImGui::End();
    }

    ////////////////////////////////////////////Create Preset Window////////////////////////////////////////////
    if (CreatePreset) {
        ImGui::Begin("Create Preset", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::BeginTabBar("CreateMenu");
        ////////////////////////////////////////////Create: Custom Preset////////////////////////////////////////////
        if (ImGui::BeginTabItem("Custom")) {
            
            ImGui::BeginChild("Input", ImVec2{350, (InputNameError == 1 ? 280.0f : 260.0f) }, true, ImGuiWindowFlags_NoScrollbar);

            if (InputNameError == 1) {
                ImGui::TextColored(ImVec4{ 1, 0, 0, 1 }, "Invalid Name:");
                ImGui::SameLine();
                ImGui::TextColored(ImVec4{ 1, 1, 0, 1 }, "( 20 Chars ) [ a-z A-z _.-^! ]");
            }
            if(ImGui::InputText("Name", &PresetName)) {
                 tempCamera.name = PresetName;
            }
            ImGui::SliderInt("FOV", &tempCamera.FOV, 60, 110);
            if (ImGui::SliderInt("Dist", &tempCamera.Distance, 100, 400)) {
                tempCamera.Distance = (tempCamera.Distance / 10) * 10; // clamp between 10
            }
            if (ImGui::SliderInt("Hei", &tempCamera.Height, 40, 200)) {
                tempCamera.Height = (tempCamera.Height / 10) * 10; // clamp between 10
            }
            ImGui::SliderInt("Ang", &tempCamera.Angle, -15, 0);
            if (ImGui::SliderFloat("Sti", &tempCamera.Stiffness, 0.0f, 1.0f, "%.2f")) {
                tempCamera.Stiffness = roundf(tempCamera.Stiffness * 20) / 20;
            }
            if (ImGui::SliderFloat("SwiSp", &tempCamera.SwivelSpeed, 1.0f, 10.0f, "%.2f")) {
                tempCamera.SwivelSpeed = roundf(tempCamera.SwivelSpeed * 10.0f) / 10.0f;
            }
            if (ImGui::SliderFloat("TraSp", &tempCamera.TransitionSpeed, 1.0f, 2.0f, "%.2f")) {
                tempCamera.TransitionSpeed = roundf(tempCamera.TransitionSpeed * 10.0f) / 10.0f;
            }
            if (ImGui::Button("Add Preset")) {
                
                const char* reg = R"(^[a-zA-Z0-9_.^!-]+$)";
                std::regex pattern(reg);
                if (std::regex_match(PresetName, pattern)&&PresetName.length() <= 20) {
                    cameras.push_back(tempCamera);
                    PresetName.clear();
                    settingsChanged = true;
                    InputNameError = 0;
                } else InputNameError = 1;
            }


            ImGui::SameLine();
            ///////////////////Copy Current Button///////////////////
            if (ImGui::Button("Copy Current")) {
                gameWrapper->Execute([this](GameWrapper* gw) {
                    ProfileCameraSettings settings = gameWrapper->GetSettings().GetCameraSettings();

                    tempCamera.FOV = settings.FOV;
                    tempCamera.Distance = settings.Distance;
                    tempCamera.Angle = settings.Pitch;
                    tempCamera.Height = settings.Height;
                    tempCamera.Stiffness = settings.Stiffness;
                    tempCamera.SwivelSpeed = settings.SwivelSpeed;
                    tempCamera.TransitionSpeed = settings.TransitionSpeed;
                });
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Pros")) {
            ////////////////////////////////////////////Create: Pros Tab////////////////////////////////////////////
            ImGui::BeginChild("CacheProPlayers", ImVec2{350, 70}, true, ImGuiWindowFlags_NoScrollbar);
            ImGui::Text("Add Pro Player Presets: ");
            if (ImGui::InputTextWithHint("Pro Name", "Enter Pro Player's name", &ProPlayerSearch)) {
                ProPlayerCameras.clear();

                ProPlayerCameras = GetProPreset(ProPlayerSearch, "CameraPresetsPros.txt");

                if (ProPlayerCameras.size() > 10) ProPlayerCameras.resize(10);
                
            }
            if(ProPlayerSearch.empty()) ProPlayerCameras.clear();
            ImGui::EndChild();
            ImVec2 size = {350, (max(ProPlayerCameras.size(), 1) * 27.5f) + ImGui::CalcTextSize("P").y * 2};
        
            ImGui::BeginChild("PlayerList", size, true, ImGuiWindowFlags_NoScrollbar);
            for (CP_CameraSettings cam : ProPlayerCameras) {
                ImGui::PushID(cam.name.c_str());

                if (ImGui::Button("Add")) {
                    cameras.push_back(cam);
                    settingsChanged = true;
                    InputNameError = false;
                    ProPlayerSearch.clear();
                    ProPlayerCameras.clear();
                }
                ImGui::SameLine();
                ImGui::Text(cam.name.c_str());
                ImGui::PopID();
            }
            if(!ProPlayerCameras.empty()) ImGui::Text("Provided by Liquipedia under CC-BY-SA 3.0");
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        ////////////////////////////////////////////Create: Freestylers Tab////////////////////////////////////////////
        if (ImGui::BeginTabItem("Freestylers")) {

            ImGui::BeginChild("CacheFreestylePlayers", ImVec2{ 350, 70 }, true, ImGuiWindowFlags_NoScrollbar);
            ImGui::Text("Add Freestyler Presets: ");


            if (ImGui::InputTextWithHint("FS Name", "Enter Freestyler's name", &FreestylePlayerSearch)) {
                FreestylePlayerCameras.clear();

                FreestylePlayerCameras = GetProPreset(FreestylePlayerSearch, "CameraPresetsFS.txt");

                if (FreestylePlayerCameras.size() > 10) FreestylePlayerCameras.resize(10);
                
                if(FreestylePlayerSearch.empty()) FreestylePlayerCameras.clear();
                
            }
            if(FreestylePlayerSearch.empty()) FreestylePlayerCameras.clear();
            ImGui::EndChild();
            ImVec2 size = { 350, (max(FreestylePlayerCameras.size(), 1) * 27.5f) + ImGui::CalcTextSize("P").y * 3.35f }; // P is tallest Char

            ImGui::BeginChild("PlayerList", size, true, ImGuiWindowFlags_NoScrollbar);
            for (CP_CameraSettings cam : FreestylePlayerCameras) {
                ImGui::PushID(cam.name.c_str());

                if (ImGui::Button("Add")) {
                    cameras.push_back(cam);
                    settingsChanged = true;
                    InputNameError = false;
                    FreestylePlayerSearch.clear();
                    FreestylePlayerCameras.clear();
                }
                ImGui::SameLine();
                ImGui::Text(cam.name.c_str());
                ImGui::PopID();
            }
            if (!FreestylePlayerCameras.empty()) ImGui::Text("Looking for Freestylers to add to the list!\nPlease message me if youd like to be added.");
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Content Creators")) {
            ImGui::BeginChild("CacheCCPlayers", ImVec2{ 350, 70 }, true, ImGuiWindowFlags_NoScrollbar);
            ImGui::Text("Add Content Creater Presets: ");

            if (ImGui::InputTextWithHint("CC Name", "Enter Content Creaters's name", &CCPlayerSearch)) {
                CCPlayerCameras.clear();

                CCPlayerCameras = GetProPreset(FreestylePlayerSearch, "CameraPresetsCC.txt");

                if (CCPlayerCameras.size() > 10) CCPlayerCameras.resize(10);

                if (CCPlayerSearch.empty()) CCPlayerCameras.clear();

            }
            if (CCPlayerSearch.empty()) CCPlayerCameras.clear();
            ImGui::EndChild();
            ImVec2 size = { 350, (max(CCPlayerCameras.size(), 1) * 27.5f) + ImGui::CalcTextSize("P").y * 3.35f }; // P is tallest Char

            ImGui::BeginChild("PlayerList", size, true, ImGuiWindowFlags_NoScrollbar);
            for (CP_CameraSettings cam : CCPlayerCameras) {
                ImGui::PushID(cam.name.c_str());

                if (ImGui::Button("Add")) {
                    cameras.push_back(cam);
                    settingsChanged = true;
                    InputNameError = false;
                    CCPlayerSearch.clear();
                    CCPlayerCameras.clear();
                }
                ImGui::SameLine();
                ImGui::Text(cam.name.c_str());
                ImGui::PopID();
            }
            if (!CCPlayerCameras.empty()) ImGui::Text("Looking for Content Creaters to add to the list!\nPlease message me if youd like to be added.");
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Codes")) {
            ////////////////////////////////////////////Create: Codes////////////////////////////////////////////
            ImGui::BeginChild("Codes", ImVec2{350, 50}, true, ImGuiWindowFlags_NoScrollbar);
            ImGui::Columns(2, "AddCodes");
            if (ImGui::Button("Add Code")) {
                GetAllCodes(CodeAdder);
                for (auto thing : ImportedCodes) {
                    LOG("Thing: {}", thing.camera_settings.name);
                }
                CodeAdder.clear();
            }
            ImGui::SetColumnWidth(0, 90);
            ImGui::NextColumn();
            ImGui::InputText("Enter Code", &CodeAdder);
            ImGui::SetColumnWidth(1, 350-90);
            ImGui::Columns(1);
            ImGui::EndChild();
            if (!ImportedCodes.empty()) {
                int i = 0;
                std::unordered_map<std::string, int> cameraNames;
                for (auto it = ImportedCodes.begin(); it != ImportedCodes.end();) {
                    CP_CameraSettings& cam = it->camera_settings;
                    std::string name = cam.name;

                    // Append the count to the name if it's not unique
                    cameraNames[name]++;
                    if (cameraNames[name] > 1) {
                        cam.name = name + std::to_string(cameraNames[name]-1);
                    }
                    
                     ///\Note if theres more of the same name then get the amount of the same name and subtract
                     ///\Note Ex: Name, Name1, Name2, Name3
                     ///\Note      (1)  (2)-1  (3)-1  (4)-1
                    

                    // Keep the id different from the others by including the index of the name.
                    ImGui::PushID((name + std::to_string(i)).c_str());

                    ImGui::BeginChild("Border", it->is_open ? ImVec2{ 350, 160.0f } : ImVec2{ 350, 45.0f }, true);

                    if (ImGui::Button("Remove")) {
                        it = ImportedCodes.erase(it);  // Erase and update iterator
                        ImGui::PopID();
                        ImGui::EndChild();
                        continue;
                    }

                    ImGui::SameLine();
                    it->is_open = ImGui::TreeNode(name.c_str());

                    if (it->is_open) {
                        ImGui::TextUnformatted(("FOV: " + std::to_string(cam.FOV)).c_str());
                        ImGui::TextUnformatted(("Distance: " + std::to_string(cam.Distance)).c_str());
                        ImGui::TextUnformatted(("Height: " + std::to_string(cam.Height)).c_str());
                        ImGui::TextUnformatted(("Stiffness: " + std::format("{}", cam.Stiffness)).c_str());
                        ImGui::TextUnformatted(("SwivelSpeed: " + std::format("{}", cam.SwivelSpeed)).c_str());
                        ImGui::TextUnformatted(("TransitionSpeed: " + std::format("{}", cam.TransitionSpeed)).c_str());
                        ImGui::TreePop();
                    }

                    ImGui::EndChild();
                    ImGui::PopID();

                    // Battle of the i's idk its 2:30am
                    i++;
                    ++it;

                }
            }
            if (ImGui::Button("Add All")) {

                for (CP_ImportedCode code : ImportedCodes) {
                    cameras.push_back(code.camera_settings);
                    settingsChanged = true;
                }
                settingsChanged = true;
                ImportedCodes.clear();
                ImportCode = false;
                CodeAdder = "";
            }
            ImGui::EndTabItem();
        }
        ImGui::Spacing();
        ImGui::Separator();
        
        if (ImGui::Button("Close")) {
            ProPlayerCameras.clear();
            CreatePreset = false;
            InputNameError = false;
            PresetName.clear();
        }
        ImGui::EndTabBar();
        ImGui::End();
    }
    style = backup;
}
void PluginWindowBase::SetImGuiContext(uintptr_t ctx) {
    ImGui::SetCurrentContext(reinterpret_cast <ImGuiContext*> (ctx));
}

bool PluginWindowBase::ShouldBlockInput() {
    return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

bool PluginWindowBase::IsActiveOverlay() {
    return true;
}

void PluginWindowBase::OnOpen() {
    isWindowOpen_ = true;
}

void PluginWindowBase::OnClose() {
    isWindowOpen_ = false;
}

void PluginWindowBase::Render() {
    ImGui::SetNextWindowSize(ImVec2 {900, 400});
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4{ 9 / 255.f, 204.f / 255.f, 162.f / 255.f, 0.70f });
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4{ 17.f / 255.f, 167.f / 255.f, 140.f / 255.f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.50f, 0.50f, 0.50f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.19f, 0.19f, 0.19f, 0.92f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{9 / 255.f, 204.f / 255.f, 162.f / 255.f, 0.70f});
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.00f, 0.00f, 0.00f, 0.24f));
    if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_NoResize)) {
        // Early out if the window is collapsed, as an optimization.
        LOG("CameraPresets Collapse");
        ImGui::End();
        return;
    }

    RenderWindow();

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();


    if (!isWindowOpen_) {
        _globalCvarManager->executeCommand("togglemenu " + GetMenuName());
    }
}