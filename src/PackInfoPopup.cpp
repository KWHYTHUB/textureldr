#include "PackInfoPopup.hpp"
#include <Sapphire/loader/Loader.hpp>
#include <Sapphire/loader/Dirs.hpp>
#include <Sapphire/binding/GameManager.hpp>

class WackyBypassFont : public CCLabelBMFont {
protected:
    void setFntFile(ghc::filesystem::path fnt) {
        auto conf = FNTConfigLoadFile(fnt.string().c_str());
        m_sFntFile = fnt.string();
        CC_SAFE_RETAIN(conf);
        CC_SAFE_RELEASE(m_pConfiguration);
        m_pConfiguration = conf;
        conf->m_sAtlasName = fnt.replace_extension("png").string();

        this->setTexture(
            CCTextureCache::sharedTextureCache()->addImage(conf->getAtlasName(), false)
        );
        this->createFontChars();
    }

public:
    static CCLabelBMFont* create(
        const char* text,
        ghc::filesystem::path const& fnt
    ) {
        auto label = CCLabelBMFont::create();
        static_cast<WackyBypassFont*>(label)->setFntFile(fnt);
        label->setString(text);
        return label;
    }
};

ghc::filesystem::path PackInfoPopup::getPathInPack(const char* filename) const {
    std::string suffix = "";
    switch (CCDirector::get()->getLoadedTextureQuality()) {
        case kTextureQualityHigh: {
            suffix = "-uhd";
        } break;

        case kTextureQualityMedium: {
            suffix = "-hd";
        } break;

        default: break;
    }

    auto fname = ghc::filesystem::path(filename);
    fname.replace_filename(
        fname.stem().string() + suffix + fname.extension().string()
    );

    if (ghc::filesystem::exists(m_pack->getPath() / fname)) {
        return m_pack->getPath() / fname;
    } else {
        return dirs::getGameDir() / "Resources" / fname;
    }
}

bool PackInfoPopup::setup(std::shared_ptr<Pack> pack) {
    m_pack = pack;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto title = WackyBypassFont::create(
        pack->getDisplayName().c_str(),
        this->getPathInPack("goldFont.fnt")
    );
    title->setPosition(
        winSize.width / 2,
        winSize.height / 2 + m_size.height / 2 - 30.f
    );
    m_mainLayer->addChild(title);

    // bigFont.fnt

    auto bigFontLabel = WackyBypassFont::create(
        "This is what a Label looks like!",
        this->getPathInPack("bigFont.fnt")
    );
    bigFontLabel->limitLabelWidth(m_size.width, .5f, .1f);
    bigFontLabel->setPosition(winSize / 2 + CCPoint { 0.f, 10.f });
    m_mainLayer->addChild(bigFontLabel);

    // GJ_button_01.png

    auto defaultBtnLabel = WackyBypassFont::create(
        "Button!",
        this->getPathInPack("goldFont.fnt")
    );
    defaultBtnLabel->setScale(.8f);

    auto defaultBtnSize =
        defaultBtnLabel->getScaledContentSize() + CCSize { 20.f, 15.f };

    auto defaultBtnSpr = CCScale9Sprite::create(
        this->getPathInPack("GJ_button_01.png").string().c_str(),
        { 0, 0, 40, 40 }
    );
    defaultBtnSpr->setContentSize(defaultBtnSize);
    defaultBtnSpr->setScale(.85f);
    defaultBtnSpr->addChild(defaultBtnLabel);

    defaultBtnLabel->setPosition(defaultBtnSize / 2);

    auto defaultBtn = CCMenuItemSpriteExtra::create(
        defaultBtnSpr, this, nullptr
    );
    defaultBtn->setPosition(-60.f, -50.f);
    m_buttonMenu->addChild(defaultBtn);

    // GJ_button_02.png

    auto altBtnLabel = WackyBypassFont::create(
        "Button!",
        this->getPathInPack("goldFont.fnt")
    );
    altBtnLabel->setScale(.8f);

    auto altBtnSize =
        altBtnLabel->getScaledContentSize() + CCSize { 20.f, 15.f };

    auto altBtnSpr = CCScale9Sprite::create(
        this->getPathInPack("GJ_button_02.png").string().c_str(),
        { 0, 0, 40, 40 }
    );
    altBtnSpr->setContentSize(altBtnSize);
    altBtnSpr->setScale(.85f);
    altBtnSpr->addChild(altBtnLabel);

    altBtnLabel->setPosition(altBtnSize / 2);

    auto altBtn = CCMenuItemSpriteExtra::create(
        altBtnSpr, this, nullptr
    );
    altBtn->setPosition(60.f, -50.f);
    m_buttonMenu->addChild(altBtn);

    return true;
}

PackInfoPopup* PackInfoPopup::create(std::shared_ptr<Pack> pack) {
    auto ret = new PackInfoPopup;
    if (ret) {
        ret->m_pack = pack;
        if (ret->init(
            320.f, 200.f, pack,
            ret->getPathInPack("GJ_square01.png").string().c_str()
        )) {
            ret->autorelease();
            return ret;
        }
        delete ret;
    }
    return nullptr;
}
