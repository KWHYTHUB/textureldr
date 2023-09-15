#include "PackNode.hpp"
#include <Sapphire/binding/ButtonSprite.hpp>
#include <Sapphire/binding/CCMenuItemToggler.hpp>
#include "PackManager.hpp"
#include "PackSelectLayer.hpp"
#include "PackInfoPopup.hpp"
#include "DragThingy.hpp"

bool PackNode::init(
    PackSelectLayer* layer,
    std::shared_ptr<Pack> pack,
    float width
) {
    if (!CCNode::init())
        return false;

    constexpr float HEIGHT = PackNode::HEIGHT;
    constexpr float SPACE_FOR_MENU = 50.f;
    constexpr float MOVE_OFFSET = 20.f;
    constexpr float SPACE_FOR_LOGO = HEIGHT;
    constexpr float PADDING = 5.f;

    float labelWidth = width - SPACE_FOR_MENU - SPACE_FOR_LOGO;
    float menuPosX = width - SPACE_FOR_MENU + MOVE_OFFSET;
    
    m_pack = pack;
    m_layer = layer;

    this->setID("PackNode");

    this->setContentSize({ width, HEIGHT });

    auto menu = CCMenu::create();
    menu->setPosition(menuPosX, HEIGHT / 2);
    menu->setID("pack-button-menu");
    
    auto logo = CCSprite::createWithSpriteFrameName("sapphire.loader/no-logo.png");
    logo->setPosition({ SPACE_FOR_LOGO / 2 + PADDING, HEIGHT / 2 });
    limitNodeSize(logo, { HEIGHT - PADDING * 2, HEIGHT - PADDING * 2 }, 1.f, .1f);
    this->addChild(logo);

    auto nameLabel = CCLabelBMFont::create(
        m_pack->getDisplayName().c_str(), "bigFont.fnt"
    );
    nameLabel->limitLabelWidth(labelWidth, .65f, .1f);

    auto nameButton = CCMenuItemSpriteExtra::create(
        nameLabel, this, menu_selector(PackNode::onView)
    );
    nameButton->setPosition(
        PADDING + SPACE_FOR_LOGO + nameLabel->getScaledContentSize().width / 2 - menuPosX,
        0
    );
    nameButton->setID("pack-name-button");
    menu->addChild(nameButton);

    auto applyArrowSpr = CCSprite::createWithSpriteFrameName("edit_addCBtn_001.png");
    applyArrowSpr->setScale(.45f);

    DragThingy* dragHandle = DragThingy::create(
        [=] { m_layer->startDragging(this); },
        [=] (CCPoint offset) { m_layer->moveDrag(offset); },
        [=] { m_layer->stopDrag(); }
    );
    applyArrowSpr->setAnchorPoint(ccp(0, 0));
    dragHandle->addChild(applyArrowSpr);
    dragHandle->setContentSize(applyArrowSpr->getScaledContentSize());
    dragHandle->setID("apply-pack-button");
    dragHandle->setPosition(width - MOVE_OFFSET, HEIGHT / 2.f);
    this->addChild(dragHandle);

    this->addChild(menu);

    return true;
}

void PackNode::onView(CCObject*) {
    PackInfoPopup::create(m_pack)->show();
}

PackNode* PackNode::create(
    PackSelectLayer* layer,
    std::shared_ptr<Pack> pack,
    float width
) {
    auto ret = new PackNode;
    if (ret && ret->init(layer, pack, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
