#include "SoundLoader.h"


std::unordered_map<std::string, Mix_Chunk*> SoundLoader::umapSoundsLoaded;




Mix_Chunk* SoundLoader::loadSound(std::string filename) {
    if (filename != "") {
        auto found = umapSoundsLoaded.find(filename);

        if (found != umapSoundsLoaded.end()) {
            //The sound was already loaded so return it.
            return found->second;
        }
        else {
            //Setup the relative filepath to the sounds folder using the input filename.
            std::string filepath = "Data/Sounds/" + filename;

            //Try to create a mix_Chunk using the filepath.
            Mix_Chunk* mix_Chunk = Mix_LoadWAV(filepath.c_str());
            if (mix_Chunk != nullptr) {

                //Add the mix_Chunk to the map of loaded mix_Chunks to keep track of it and for clean-up purposes.
                umapSoundsLoaded[filename] = mix_Chunk;

                return mix_Chunk;
            }
        }
    }

    return nullptr;
}



void SoundLoader::deallocateSounds() {
	//Stop the sounds first
    Mix_HaltChannel(-1);

    //Destroy all the sounds
    while (umapSoundsLoaded.empty() == false) {
        auto it = umapSoundsLoaded.begin();
        if (it->second != nullptr)
            Mix_FreeChunk(it->second);

        umapSoundsLoaded.erase(it);
    }
}