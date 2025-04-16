/*
    SPDX-FileCopyrightText: 2019 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kcm.h"
#include "desktopeffectsdata.h"
#include "effectsfilterproxymodel.h"
#include "effectsmodel.h"

#include <KLocalizedString>
#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(DesktopEffectsKCMFactory,
                           "kcm_kwin_effects_x11.json",
                           registerPlugin<KWin::DesktopEffectsKCM>();
                           registerPlugin<KWin::DesktopEffectsData>();)

namespace KWin
{

DesktopEffectsKCM::DesktopEffectsKCM(QObject *parent, const KPluginMetaData &metaData)
    : KQuickConfigModule(parent, metaData)
    , m_model(new EffectsModel(this))
{
    qmlRegisterType<EffectsFilterProxyModel>("org.kde.private.kcms.kwin.effects", 1, 0, "EffectsFilterProxyModel");

    setButtons(Apply | Default | Help);

    connect(m_model, &EffectsModel::dataChanged, this, &DesktopEffectsKCM::updateNeedsSave);
    connect(m_model, &EffectsModel::loaded, this, &DesktopEffectsKCM::updateNeedsSave);

    // These are handled in kcm_animations
    m_model->setExcludeExclusiveGroups({"toplevel-open-close-animation", "maximize", "minimize", "fullscreen", "show-desktop", "desktop-animations"});
    m_model->setExcludeEffects({"fadingpopups", "slidingpopups", "login", "logout"});
}

DesktopEffectsKCM::~DesktopEffectsKCM()
{
}

QAbstractItemModel *DesktopEffectsKCM::effectsModel() const
{
    return m_model;
}

void DesktopEffectsKCM::load()
{
    m_model->load();
    setNeedsSave(false);
}

void DesktopEffectsKCM::save()
{
    m_model->save();
    setNeedsSave(false);
}

void DesktopEffectsKCM::defaults()
{
    m_model->defaults();
    updateNeedsSave();
}

void DesktopEffectsKCM::onGHNSEntriesChanged()
{
    m_model->load(EffectsModel::LoadOptions::KeepDirty);
}

void DesktopEffectsKCM::configure(const QString &pluginId, QQuickItem *context) const
{
    const QModelIndex index = m_model->findByPluginId(pluginId);
    m_model->requestConfigure(index, context);
}

void DesktopEffectsKCM::updateNeedsSave()
{
    setNeedsSave(m_model->needsSave());
    setRepresentsDefaults(m_model->isDefaults());
}

} // namespace KWin

#include "kcm.moc"

#include "moc_kcm.cpp"
