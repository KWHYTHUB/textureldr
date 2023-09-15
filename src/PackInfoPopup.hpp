#pragma once

#include <Sapphire/ui/Popup.hpp>
#include "Pack.hpp"

using namespace sapphire::prelude;

class PackInfoPopup : public Popup<std::shared_ptr<Pack>> {
protected:
    std::shared_ptr<Pack> m_pack;

    bool setup(std::shared_ptr<Pack> pack) override;

    ghc::filesystem::path getPathInPack(const char* filename) const;

public:
    static PackInfoPopup* create(std::shared_ptr<Pack> pack);
};
