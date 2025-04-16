add_definitions(-DTRANSLATION_DOMAIN=\"kcm_animations_x11\")

set(kcm_animations_x11_PART_SRCS
    kcm.cpp
    animationsdata.cpp
    effectssubsetmodel.cpp
)

kcmutils_add_qml_kcm(kcm_animations_x11 SOURCES ${kcm_animations_x11_PART_SRCS})

kconfig_add_kcfg_files(kcm_animations_x11 animationskdeglobalssettings.kcfgc GENERATE_MOC)

target_link_libraries(kcm_animations_x11 PRIVATE
    Qt::DBus
    Qt::Quick
    KF6::ConfigGui
    KF6::KCMUtils
    KF6::I18n
    KF6::KCMUtilsQuick
    KF6::Service
    kcmkwincommon
)
