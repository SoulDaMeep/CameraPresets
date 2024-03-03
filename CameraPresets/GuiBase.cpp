#include "pch.h"
#include "GuiBase.h"
#include "CameraPresets.h"

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
        _globalCvarManager->executeCommand("togglemenu CameraPresets");
    });
    ImGui::Text("F5 is Window Bind");
}

void CameraPresets::RenderWindow() {
    std::fstream inputFile(CameraFolder, std::ios::in);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string playerName;
            std::vector <std::string> values;
            // Extract the player name
            iss >> playerName;

            // Tokenize the rest of the line using spaces and store in a vector
            std::string word;
            while (iss >> word) values.push_back(word);

            // dont continuously add the names by checking if they are already in the cameras vector

            bool playerExists = false;
            for (const auto& existingCamera : cameras) {
                if (existingCamera.name == playerName) {
                    playerExists = true;
                    break;
                }
            }

            if (!playerExists) {
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
        LOG("Could not open file");
    }

    if (settingsChanged) {
        std::string data;
        for (CP_CameraSettings camera : cameras) {
            LOG("{}", camera.name);
            data += CreateSettingString(camera);
        }
        SaveToFile(data);
        settingsChanged = false;
    }
    
    ImGui::SetNextWindowSize(ImVec2{900, 400});
    ImGui::Begin("CameraPresets", nullptr, ImGuiWindowFlags_MenuBar);
    ImGui::BeginTabBar("Tab");
    if(ImGui::BeginTabItem("-Presets-")) {
        ImGui::BeginChild("left pane", ImVec2(200, -ImGui::GetFrameHeightWithSpacing()), true);
        ImGui::InputText("Search", &SearchName);
        for (int i = 0; i < cameras.size(); i++)
        {
            auto& camera = cameras[i];
            if (camera.name.find(SearchName) != std::string::npos) {
                ImGui::PushID(i);
                const bool this_popup_open = ImGui::IsPopupOpen("item context menu");
                if (ImGui::Selectable(camera.name.c_str(), selected == i || this_popup_open)) {
                    selected = i;
                }
                if (ImGui::BeginPopupContextItem("item context menu")) {
                    ImGui::EndPopup();
                }
                ImGui::PopID();
            }
        }

        ImGui::EndChild();
        ImGui::SameLine();
        if (selected < cameras.size()) {
            if (oldSelected != selected) {
                CopyCodeMessage = false;
                oldSelected = selected;
            }
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text(cameras.at(selected).name.c_str());
            ImGui::Separator();


            
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
            } //FOV HEIGHT ANGLE STIFFNESS TRANSITIONSPEED DISTANCE SWIVELSPEED

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
        
            ImGui::InputTextWithHint("", cc.code.c_str(), &InputCode);

            ImGui::SameLine();
            if (ImGui::Button("Copy Code")) {
                ImGui::SetClipboardText(cc.code.c_str());
                CopyCodeMessage = true;
                gameWrapper->Execute([this](GameWrapper* gw) {
                    gameWrapper->SetTimeout([this](GameWrapper* gw) {
                        CopyCodeMessage = false;
                    }, 2);
                    // clear copy code message after 2 seconds
                });
            }
            if(CopyCodeMessage) {
                ImGui::Text("You Copied The Code!");
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }

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
        if (ImGui::Button("Clear All Presets")) {
            DeleteWindow = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            if(!cameras.empty()){
                DeletePlayerFromFile(cameras.at(selected).name);
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
    if (ImGui::BeginTabItem("-Help/Info-")) {
        ImGui::Text("CameraPresets is a plugin that adds functionality and a user friendly GUI to the original outdated pro settings implemented by bakkesmod");
        ImGui::Text("If you are a pro that wants their name added to the list please contact @souldameep on discord and I will add you in the next update");
        ImGui::Separator();
        ImGui::Text("Codes: ");
        ImGui::BulletText("Guide: NAME#FOV#HEIGHT#ANGLE#STIFFNESS#TRANSITIONSPEED#DISTANCE#SWIVELSPEED | \nExample: Squishy#110#90# -5#0.5#1.00#270#6.00");
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();

    if (DeleteWindow) {
        ImGui::PushID("DeleteWindow");
        ImGui::Begin("Delete ALL Presets", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Are you sure you want to delete all your presets");
        ImGui::Text("These changes CANNOT be undone");
        if (ImGui::Button("Cancel")) {
            DeleteWindow = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete All")) {
            DeleteWindow = false;
            for (CP_CameraSettings cam : cameras) {
                DeletePlayerFromFile(cam.name);
            }
            cameras.clear();
            settingsChanged = true;
        }
        ImGui::End();
        ImGui::PopID();
    }

    if (CreatePreset) {
        ImGui::PushID("CreatePreset");
        ImGui::Begin("Create Preset", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        if(InputNameError) ImGui::Text("Please input a proper name (no spaces)");

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
            bool containsSpace = false;
            for (char c : PresetName) {
                if (c == ' ') {
                    containsSpace = true;
                    break;
                }
            }

            if (!PresetName.empty() && !containsSpace) {
                cameras.push_back(tempCamera);
                PresetName.clear();
                CreatePreset = false;
                settingsChanged = true;
                InputNameError = false;
            }
            else {
                InputNameError = true;
            }
        }
        ImGui::SameLine();

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
        ImGui::Separator();

        if (ImGui::InputText("Pro Name", &ProPlayerSearch)) {
            ProPlayerCameras.clear();
            ProPlayerCameras = GetProPreset(ProPlayerSearch);
            if (ProPlayerCameras.size() > 10) {
                ProPlayerCameras.resize(10);
            }
        }
        ImGui::PushID("ProPlayerList");
        for (CP_CameraSettings cam : ProPlayerCameras) {
            if (ImGui::Button("Add")) {
                cameras.push_back(cam);
                CreatePreset = false;
                settingsChanged = true;
                InputNameError = false;
                ProPlayerSearch.clear();
                ProPlayerCameras.clear();
            }
            ImGui::SameLine();
            ImGui::Text(cam.name.c_str());
            ImGui::PopID();
        }
        ImGui::Separator();
        ImGui::InputText("Enter Code", &CodeAdder);
        ImGui::SameLine();
        if (ImGui::Button("Add Code")) {
            GetAllCodes(CodeAdder);
        }
        if (!ImportedCodes.empty()) {
            for (CP_CameraSettings cam : ImportedCodes) {
                if (ImGui::TreeNode(cam.name.c_str())) {
                    ImGui::TextUnformatted(("FOV: " + std::to_string(cam.FOV)).c_str());
                    ImGui::TextUnformatted(("Distance: " + std::to_string(cam.Distance)).c_str());
                    ImGui::TextUnformatted(("Height: " + std::to_string(cam.Height)).c_str());
                    ImGui::TextUnformatted(("Stiffness: " + std::format("{}", cam.Stiffness)).c_str());
                    ImGui::TextUnformatted(("SwivelSpeed: " + std::format("{}", cam.SwivelSpeed)).c_str());
                    ImGui::TextUnformatted(("TransitionSpeed: " + std::format("{}", cam.TransitionSpeed)).c_str());
                    ImGui::TreePop();
                }
            }
        }
        if (ImGui::Button("Add All")) {

            for (CP_CameraSettings cam : ImportedCodes) {
                cameras.push_back(cam);
            }
            settingsChanged = true;
            ImportedCodes.clear();
            ImportCode = false;
            CodeAdder = "";
        }
        ImGui::Separator();
        if (ImGui::Button("Close")) {
            ProPlayerCameras.clear();
            CreatePreset = false;
            InputNameError = false;
            PresetName.clear();
        }
        ImGui::End();
    }
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
    if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    RenderWindow();

    ImGui::End();

    if (!isWindowOpen_) {
        _globalCvarManager->executeCommand("togglemenu " + GetMenuName());
    }
}