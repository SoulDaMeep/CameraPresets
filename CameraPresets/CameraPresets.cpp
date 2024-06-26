#include "pch.h"

#include "CameraPresets.h"


BAKKESMOD_PLUGIN(CameraPresets, "Change Camera Settings with ease", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr < CVarManagerWrapper > _globalCvarManager;

/// \TODO FormatCamera()

void CameraPresets::onLoad() {
    _globalCvarManager = cvarManager;

    cvarManager->setBind("F1", "togglemenu " + GetMenuName());

    CurlRequest proReq;
    proReq.url = "https://raw.githubusercontent.com/SoulDaMeep/CameraPresets/master/data/CameraPresets/CameraPresetsPros.txt";
    HttpWrapper::SendCurlRequest(proReq, [this](int code, std::string result)
        {
            LOG("[CameraPresets] Repo-Req (Pros): {}", code);
            if (code != 200) return;

            SaveToFile(result, gameWrapper->GetDataFolder() / "CameraPresets" / "CameraPresetsPros.txt");

    });
    CurlRequest fsReq;
    fsReq.url = "https://raw.githubusercontent.com/SoulDaMeep/CameraPresets/master/data/CameraPresets/CameraPresetsFS.txt";
    HttpWrapper::SendCurlRequest(fsReq, [this](int code, std::string result) {
        LOG("[CameraPresets] Repo-Req (FS): {}", code);
        if (code != 200) return;

        SaveToFile(result, gameWrapper->GetDataFolder() / "CameraPresets" / "CameraPresetsFS.txt");
    });
    CurlRequest CCReq;
    CCReq.url = "https://raw.githubusercontent.com/SoulDaMeep/CameraPresets/master/data/CameraPresets/CameraPresetsCC.txt";
    HttpWrapper::SendCurlRequest(CCReq, [this](int code, std::string result)
        {
            LOG("[CameraPresets] Repo-Req (CC): {}", code);
            if (code != 200) return;

            SaveToFile(result, gameWrapper->GetDataFolder() / "CameraPresets" / "CameraPresetsCC.txt");

        });

    ProfileCameraSettings settings = gameWrapper->GetSettings().GetCameraSettings();
    PlayerCameraSettings.FOV = settings.FOV;
    PlayerCameraSettings.Distance = settings.Distance;
    PlayerCameraSettings.Angle = settings.Pitch;
    PlayerCameraSettings.Height = settings.Height;
    PlayerCameraSettings.Stiffness = settings.Stiffness;
    PlayerCameraSettings.SwivelSpeed = settings.SwivelSpeed;
    PlayerCameraSettings.TransitionSpeed = settings.TransitionSpeed;
    LoadSave();

}
void CameraPresets::LoadSave() {
    std::fstream inputFile;
    std::string data;
    inputFile.open(gameWrapper->GetDataFolder() / "CameraPresets" / "CameraPresets_Save.txt", std::ios::in);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string linePlayerName;

            data += line + "\n";
        }
        inputFile.close();
    }
    SaveToFile(data, gameWrapper->GetDataFolder() / "cameras_rlcs.data");
}

void CameraPresets::FRenamePlayer(std::string playername, std::string nPlayername) {
    std::fstream inputFile(gameWrapper->GetDataFolder() / "cameras_rlcs.data", std::ios::in);
    std::string data;

    if (inputFile.is_open()) {
        std::string line;

        while (std::getline(inputFile, line)) {
            // Tokenize the line using spaces
            std::istringstream iss(line);
            std::string linePlayerName;
            
            // Extract the first token
            if (iss >> linePlayerName) {
                LOG("lpn: {} | pn {}", linePlayerName, playername);
                if (linePlayerName != playername) {
                    data += line + "\n";
                }
                else {
                    std::string rol;
                    std::getline(iss, rol);
                    LOG("FOUND lpn: {} | pn {}", linePlayerName, playername);
                    LOG("line: {}", rol);
                    LOG("new line: {}", nPlayername + rol);
                    data += nPlayername + rol + "\n";
                    settingsChanged = true;
                }
            }
        }
        inputFile.close();
        SaveToFile(data, gameWrapper->GetDataFolder() / "cameras_rlcs.data");
    }
    else {
        LOG("[CameraPresets] Could not open file {}", "cameras_rlcs.data");
    }
}
std::vector<CameraPresets::CP_CameraSettings> CameraPresets::GetProPreset(std::string substring, const char* file) {
    std::fstream inputFile(gameWrapper->GetDataFolder() / "CameraPresets" / file, std::ios::in);
    std::vector<CP_CameraSettings> t_Cameras;
    if (inputFile.is_open()) {
        std::string line;
        
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string t_PlayerName;
            std::vector < std::string > t_Values;
            std::string word;
            while (std::getline(iss, word, '#')) {
                t_Values.push_back(word);
            }

            std::string temp;
            for (char c : t_Values[0]) {
                temp += std::tolower(c);
            }
            std::string subTemp;
            for (char c : substring) {
                subTemp += std::tolower(c);
            }

            CP_CameraSettings t_TempCamera;
            if (temp.find(subTemp) != std::string::npos) {
                t_TempCamera.name = t_Values[0];
                t_TempCamera.FOV = std::stoi(t_Values[1]);
                t_TempCamera.Distance = std::stoi(t_Values[6]);
                t_TempCamera.Height = std::stoi(t_Values[2]);
                t_TempCamera.Angle = std::stoi(t_Values[3]);
                t_TempCamera.Stiffness = std::stof(std::format("{}", std::stof(t_Values[4])));
                t_TempCamera.SwivelSpeed = std::stof(std::format("{}", std::stof(t_Values[7])));
                t_Values[5].pop_back();
                t_TempCamera.TransitionSpeed = std::stof(std::format("{}", std::stof(t_Values[5])));
                t_Cameras.push_back(t_TempCamera);
            }
        }

        inputFile.close();
    }
    else {
        LOG("[CameraPresets] Could not open file {}", file);
    }
    return t_Cameras;
}
std::string CameraPresets::CreateSettingString(CP_CameraSettings camera) {
    std::ostringstream formattedString;
    formattedString << camera.name << " " <<
        camera.FOV << " " <<
        camera.Height << " " <<
        camera.Angle << " " <<
        camera.Stiffness << " " <<
        camera.TransitionSpeed << " " <<
        camera.Distance << " " <<
        camera.SwivelSpeed << "\n";
    return formattedString.str();
}

void CameraPresets::SaveToFile(std::string data, std::filesystem::path path) {
    std::fstream outfile;

    outfile.open(path, std::ios::out);

    if (outfile.is_open()) {
        outfile << data;
        outfile.close();
    }
    else {
        LOG("[CameraPresets] Could not open file {}", path.string());
    }
}

void CameraPresets::DeletePlayerFromFile(std::string playername, std::filesystem::path path) {

    std::fstream inputFile(path, std::ios::in);
    std::string data;

    if (inputFile.is_open()) {
        std::string line;

        while (std::getline(inputFile, line)) {

            std::istringstream iss(line);
            std::string linePlayerName;

            if (iss >> linePlayerName) {
                if (linePlayerName != playername) {
                    data += line + "\n";
                }
            }
        }
        inputFile.close();
        SaveToFile(data, path);
    }
    else {
        LOG("[CameraPresets] Could not open file {}", path.string());
    }
}

bool CameraPresets::isValidCode(std::string code) {
    // this regex made me want to
    const char* reg = R"(^([a-zA-Z0-9_.^!-]+)#(\d+)#(\d+)#(-?\d+(\.\d+)?)#(\d+(\.\d+)?)#(\d+(\.\d+)?)#(\d+)#(\d+(\.\d+)?)\|)";      
    std::regex pattern(reg);
    std::smatch matches;
    if (std::regex_match(code, matches, pattern)) {
        return true;
    }
    return false;
}

CameraPresets::CP_CameraSettings CameraPresets::parseCode(const std::string& input) {
    CP_CameraSettings settings;

    std::istringstream iss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, '#')) {
        tokens.push_back(token);
    }

    if (tokens.size() == 8) {
        settings.name = tokens[0];
        settings.FOV = std::stoi(tokens[1]);
        settings.Distance = std::stoi(tokens[6]);
        settings.Height = std::stoi(tokens[2]);
        settings.Angle = std::stoi(tokens[3]);
        settings.Stiffness = std::stof(std::format("{}", std::stof(tokens[4])));
        settings.SwivelSpeed = std::stof(std::format("{}", std::stof(tokens[7])));
        settings.TransitionSpeed = std::stof(std::format("{}", std::stof(tokens[5])));
        settings.code = input;
    }
    else {
        LOG("[CameraPresets] Input failed");
    }

    return settings;
}

void CameraPresets::DumpSave(std::string data) {
    std::fstream outfile;

    outfile.open(gameWrapper->GetDataFolder() / "CameraPresets" / "CameraPresets_Save.txt", std::ios::out);
    if (outfile.is_open()) {
        outfile << data;
        outfile.close();
    }
    else {
        LOG("[CameraPresets] Could not open file {}", "cameras_rlcs.data");
    }
}


void CameraPresets::GetAllCodes(std::string inputcode) {

    std::istringstream iss(inputcode);
    std::vector<std::string> separatedValues;

    std::string token;
    while (std::getline(iss, token, '|')) {
        separatedValues.push_back(token);
    }

    for (const auto& value : separatedValues) {
        if (isValidCode(value + "|")) {
            LOG("Valid {}", value + "|");
            CP_CameraSettings camera = parseCode(value + "|");
            LOG("Parsed {}", camera.name);
            CP_ImportedCode impCode = {camera, false};
            
            ImportedCodes.push_back(impCode);
        }
        else {
            LOG("[CameraPresets] Not Valid {}", value);
        }
    }
}

void CameraPresets::onUnload() {
    std::fstream inputFile;
    std::string data;
    inputFile.open(gameWrapper->GetDataFolder() / "cameras_rlcs.data", std::ios::in);
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string linePlayerName;

            data += line + "\n";
        }
        inputFile.close();
    }
    DumpSave(data);
}