#include <Sapphire/loader/Mod.hpp>
#include <Sapphire/binding/CCMenuItemSpriteExtra.hpp>
#include "PackSelectLayer.hpp"
#include <Sapphire/modify/MenuLayer.hpp>
#include <Sapphire/modify/IDManager.hpp>
#include <Sapphire/ui/BasedButtonSprite.hpp>
#include "PackManager.hpp"

using namespace sapphire::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;
        
        NodeIDs::provideFor(this);

        auto menu = this->getChildByID("right-side-menu");

        menu->addChild(CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
            this, menu_selector(MyMenuLayer::onTextureLdr)
        ));
        menu->updateLayout();

        return true;
    }

    void onTextureLdr(CCObject*) {
        PackSelectLayer::scene();
    }
};

