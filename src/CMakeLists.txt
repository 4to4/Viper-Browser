include_directories(${CMAKE_CURRENT_BINARY_DIR}) 
 
set(viper_src
    AdBlock/AdBlockButton.cpp
    AdBlock/AdBlockFilter.cpp
    AdBlock/AdBlockFilterParser.cpp
    AdBlock/AdBlockLog.cpp
    AdBlock/AdBlockLogDisplay.cpp
    AdBlock/AdBlockLogTableModel.cpp
    AdBlock/AdBlockManager.cpp
    AdBlock/AdBlockModel.cpp
    AdBlock/AdBlockSubscribeDialog.cpp
    AdBlock/AdBlockSubscription.cpp
    AdBlock/AdBlockWidget.cpp
    AdBlock/CustomFilterEditor.cpp
    AutoFill/AutoFill.cpp
    AutoFill/AutoFillCredentialsView.cpp
    AutoFill/AutoFillBridge.cpp
    AutoFill/AutoFillDialog.cpp
    Bookmarks/BookmarkExporter.cpp
    Bookmarks/BookmarkFolderModel.cpp
    Bookmarks/BookmarkImporter.cpp
    Bookmarks/BookmarkManager.cpp
    Bookmarks/BookmarkMenu.cpp
    Bookmarks/BookmarkNode.cpp
    Bookmarks/BookmarkTableModel.cpp
    Bookmarks/BookmarkWidget.cpp
    Cookies/CookieJar.cpp
    Cookies/CookieModifyDialog.cpp
    Cookies/CookieTableModel.cpp
    Cookies/CookieWidget.cpp
    Cookies/DetailedCookieTableModel.cpp
    Credentials/CredentialStore.cpp
    Downloads/DownloadItem.cpp
    Downloads/DownloadManager.cpp
    Downloads/InternalDownloadItem.cpp
    Extensions/ExtStorage.cpp
    Extensions/FaviconStoreBridge.cpp
    Highlighters/HTMLHighlighter.cpp
    Highlighters/JavaScriptHighlighter.cpp
    History/ClearHistoryDialog.cpp
    History/HistoryManager.cpp
    History/HistoryMenu.cpp
    History/HistoryTableModel.cpp
    History/HistoryWidget.cpp
    Network/AuthDialog.cpp
    Network/BlockedSchemeHandler.cpp
    Network/CertificateGeneralTab.cpp
    Network/CertificateViewer.cpp
    Network/HttpRequest.cpp
    Network/NetworkAccessManager.cpp
    Network/RequestInterceptor.cpp
    Network/SchemeRegistry.cpp
    Network/SecurityInfoDialog.cpp
    Network/SecurityManager.cpp
    Network/ViperNetworkReply.cpp
    Network/ViperSchemeHandler.cpp
    Preferences/AddSearchEngineDialog.cpp
    Preferences/ContentTab.cpp
    Preferences/ExemptThirdPartyCookieDialog.cpp
    Preferences/GeneralTab.cpp
    Preferences/Preferences.cpp
    Preferences/PrivacyTab.cpp
    Preferences/SearchTab.cpp
    URLSuggestion/URLSuggestionItemDelegate.cpp
    URLSuggestion/URLSuggestionListModel.cpp
    URLSuggestion/URLSuggestionWidget.cpp
    URLSuggestion/URLSuggestionWorker.cpp
    UserAgents/AddUserAgentDialog.cpp
    UserAgents/UserAgentManager.cpp
    UserAgents/UserAgentMenu.cpp
    UserAgents/UserAgentsWindow.cpp
    UserScripts/AddUserScriptDialog.cpp
    UserScripts/UserScript.cpp
    UserScripts/UserScriptEditor.cpp
    UserScripts/UserScriptManager.cpp
    UserScripts/UserScriptModel.cpp
    UserScripts/UserScriptWidget.cpp
    UserScripts/WebEngineScriptAdapter.cpp
    Web/URL.cpp
    Web/WebActionProxy.cpp
    Web/WebDialog.cpp
    Web/WebHistory.cpp
    Web/WebHitTestResult.cpp
    Web/WebPage.cpp
    Web/WebView.cpp
    Web/WebWidget.cpp
    Widgets/BookmarkBar.cpp
    Widgets/BookmarkDialog.cpp
    Widgets/CheckableTableView.cpp
    Widgets/CodeEditor.cpp
    Widgets/FileChooserLineEdit.cpp
    Widgets/FindTextWidget.cpp
    Window/BrowserTabBar.cpp
    Window/BrowserTabWidget.cpp
    Window/NavigationToolBar.cpp
    Window/SearchEngineLineEdit.cpp
    Window/URLLineEdit.cpp
    BrowserApplication.cpp
    BrowserScripts.cpp
    CommonUtil.cpp
    DatabaseWorker.cpp
    FaviconStore.cpp
    MainWindow.cpp
    SearchEngineManager.cpp
    SessionManager.cpp
    Settings.cpp
)

if (KF5Wallet_FOUND)
  set(viper_src ${viper_src} Credentials/CredentialStoreKWallet.cpp)
endif()

qt5_add_resources(viper_qrc application.qrc)

qt5_wrap_ui(viper_ui
    AdBlock/AdBlockLogDisplay.ui
    AdBlock/AdBlockSubscribeDialog.ui
    AdBlock/AdBlockWidget.ui
    AdBlock/CustomFilterEditor.ui
    AutoFill/AutoFillCredentialsView.ui
    AutoFill/AutoFillDialog.ui
    Bookmarks/BookmarkWidget.ui
    Cookies/CookieModifyDialog.ui
    Cookies/CookieWidget.ui
    Downloads/DownloadItem.ui
    Downloads/DownloadManager.ui
    History/ClearHistoryDialog.ui
    History/HistoryWidget.ui
    Network/AuthDialog.ui
    Network/CertificateGeneralTab.ui
    Network/CertificateViewer.ui
    Network/SecurityInfoDialog.ui
    Preferences/AddSearchEngineDialog.ui
    Preferences/ContentTab.ui
    Preferences/ExemptThirdPartyCookieDialog.ui
    Preferences/GeneralTab.ui
    Preferences/Preferences.ui
    Preferences/PrivacyTab.ui
    Preferences/SearchTab.ui
    UserAgents/AddUserAgentDialog.ui
    UserAgents/UserAgentsWindow.ui
    UserScripts/AddUserScriptDialog.ui
    UserScripts/UserScriptEditor.ui
    UserScripts/UserScriptWidget.ui
    Widgets/BookmarkDialog.ui
    Widgets/FindTextWidget.ui
    MainWindow.ui
)

set(viper_core_deps
    ${viper_src}
    ${viper_ui}
)

add_library(viper-core STATIC ${viper_core_deps}) 
target_link_libraries(viper-core
    Qt5::Core
    Qt5::Concurrent
    Qt5::Gui
    Qt5::Network
    Qt5::PrintSupport
    Qt5::Sql
    Qt5::Svg
    Qt5::QuickWidgets
    Qt5::Widgets
    Qt5::WebChannel
    Qt5::WebEngineWidgets
)

if (KF5Wallet_FOUND)
    target_link_libraries(viper-core KF5::Wallet)
endif()

set(viper_browser_src
    ${viper_qrc}
    main.cpp
)

add_executable(viper-browser ${viper_browser_src})

target_link_libraries(viper-browser
    viper-core
)

