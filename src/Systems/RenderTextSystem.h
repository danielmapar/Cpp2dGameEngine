#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../Components/TextLabelComponent.h"
#include <SDL2/SDL.h>

class RenderTextSystem: public System {
    public:
        RenderTextSystem() {
            RequireComponent<TextLabelComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const auto textlabel = entity.GetComponent<TextLabelComponent>();
                
                SDL_Surface* surface = TTF_RenderText_Blended(
                    assetStore->GetFont(textlabel.assetId),
                    textlabel.text.c_str(),
                    textlabel.color
                );
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);

                int labelWidth = 0;
                int labelHeight = 0;

                SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

                SDL_Rect dstRect = {
                    static_cast<int>(textlabel.position.x - (textlabel.isFixed ? 0 : camera.x)),
                    static_cast<int>(textlabel.position.y - (textlabel.isFixed ? 0 : camera.y)),
                    labelWidth,
                    labelHeight
                };

                SDL_RenderCopy(renderer, texture, NULL, &dstRect);

                SDL_DestroyTexture(texture);
            }
        }
};

#endif
