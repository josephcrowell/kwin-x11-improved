# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_kwin_scripts_x11\")

kcmutils_add_qml_kcm(kcm_kwin_scripts_x11 INSTALL_NAMESPACE plasma/kcms/systemsettings)

target_sources(kcm_kwin_scripts_x11 PRIVATE
    module.cpp
    kwinscriptsdata.cpp
)

target_link_libraries(kcm_kwin_scripts_x11 PRIVATE
    Qt::DBus

    KF6::I18n
    KF6::KCMUtilsCore
    KF6::KCMUtils
    KF6::Package
    KF6::KCMUtilsQuick
)

install(FILES kwinscripts.knsrc DESTINATION ${KDE_INSTALL_KNSRCDIR} RENAME kwinscripts-x11.knsrc)
