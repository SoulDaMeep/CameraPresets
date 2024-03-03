#include "pch.h"

#include "CameraPresets.h"


BAKKESMOD_PLUGIN(CameraPresets, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr < CVarManagerWrapper > _globalCvarManager;

void CameraPresets::onLoad() {
    _globalCvarManager = cvarManager;

    ProfileCameraSettings settings = gameWrapper->GetSettings().GetCameraSettings();
    PlayerCameraSettings.FOV = settings.FOV;
    PlayerCameraSettings.Distance = settings.Distance;
    PlayerCameraSettings.Angle = settings.Pitch;
    PlayerCameraSettings.Height = settings.Height;
    PlayerCameraSettings.Stiffness = settings.Stiffness;
    PlayerCameraSettings.SwivelSpeed = settings.SwivelSpeed;
    PlayerCameraSettings.TransitionSpeed = settings.TransitionSpeed;

}
std::vector<CameraPresets::CP_CameraSettings> CameraPresets::GetProPreset(std::string substring) {

    std::fstream inputFile(gameWrapper->GetDataFolder() / "CameraPresets.txt", std::ios::in);
    std::vector<CP_CameraSettings> t_Cameras;
    if (inputFile.is_open()) {
        LOG("Sttream is open");
        std::string line;

        while (std::getline(inputFile, line)) {
            LOG("in file");
            std::istringstream iss(line);
            std::string t_PlayerName;
            std::vector < std::string > t_Values;
            std::string word;
            while (std::getline(iss, word, '#')) {
                t_Values.push_back(word);
            }
            

            CP_CameraSettings t_TempCamera;
            if (t_Values[0].find(substring) != std::string::npos) {
                for (std::string val : t_Values) {
                    LOG("{}", val);
                }
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
        LOG("Could not open file");
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

void CameraPresets::SaveToFile(std::string data) {
    std::fstream outfile;

    outfile.open(gameWrapper->GetDataFolder() / "cameras_rlcs.data", std::ios::out);

    if (outfile.is_open()) {
        outfile << data;
        outfile.close();
    }
    else {
        LOG("Could not open file for writing or smth");
    }
}

void CameraPresets::SaveDataToFile(std::string playerName, std::vector < std::string > values) {
    std::fstream inputFile(gameWrapper->GetDataFolder() / "cameras_rlcs.data", std::ios::in);
    std::string data;

    if (inputFile.is_open()) {

        std::string line;

        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string t_PlayerName;
            std::vector < std::string > t_Values;

            // In file
            // Squishy 110 100 -3 1.00 270 4.00

            // Extract the player name from current line
            iss >> t_PlayerName;
            if (t_PlayerName == " ") break;

            // Tokenize the rest of the line using spaces and store in a vector
            std::string word;
            while (iss >> word) {
                t_Values.push_back(word);
            }
            // Ex: t_Playername = Squishy
            // Ex: t_Values = [110 100 -3, 1.00 270 4.00] 
            // 
            // 
            // Check if the current line contains the target player name
            if (t_PlayerName == playerName) {
                t_Values[0] = values[0];
                t_Values[5] = values[5];
                t_Values[1] = values[1];
                t_Values[2] = values[2];
                t_Values[3] = std::format("{}", std::stof(values[3]));
                t_Values[6] = std::format("{}", std::stof(values[4]));
                t_Values[4] = std::format("{}", std::stof(values[6]));
            }
            data += t_PlayerName;
            for (const auto& value : t_Values) {
                data += "   " + value;
            }
            data += "\n";
        }
        SaveToFile(data);
    }
    else {
        LOG("Could not open file");
    }
    inputFile.close();
}

void CameraPresets::DeletePlayerFromFile(std::string playername) {

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
                // Check if the token (player name) matches exactly
                if (linePlayerName != playername) {
                    // If it doesn't match, append the line to the data string
                    data += line + "\n";
                }
            }
        }
        inputFile.close();
        SaveToFile(data);
    }
    else {
        LOG("Could not open file");
    }
}
// Squishy 110 100 270 -3 1.00 4.00
// Turbo 110 90 270 -4 1.00 7.00
// Scrub 110 100 260 -3 2.00 4.00
// Squishy 110 100 270 -3 1.00 4.00
// Turbo 110 90 270 -4 1.00 7.00
// Scrub 110 100 260 -3 2.00 4.00

bool CameraPresets::isValidCode(std::string code) {
    // this regex made me want to
    
    const char* reg = R"(^([a-zA-Z0-9_.^!-]+)#(\d+)#(\d+)#(-?\d+(\.\d+)?)#(\d+(\.\d+)?)#(\d+(\.\d+)?)#(\d+)#(\d+(\.\d+)?)\|)";      
                       //^[A-Z]?['a-z']\w*#\d*#\d*#\-?\d*#\d([.])?\d*?#\d([.])?\d*#\d*\d([.])?\d*#\d([.])?\d?\|
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
        settings.Stiffness = std::stof(tokens[4]);
        settings.SwivelSpeed = std::stof(tokens[7]);
        settings.TransitionSpeed = std::stof(tokens[5]);
        settings.code = input;
    }
    else {
        LOG("Input failed");
    }

    return settings;
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
            ImportedCodes.push_back(camera);
        }
        else {
            LOG("Not Valid {}", value);
        }
    }
}