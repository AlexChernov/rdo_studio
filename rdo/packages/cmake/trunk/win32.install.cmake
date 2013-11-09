IF(MSVC_IDE)
	SET(CONFIG_DIRECTORY_DEBUG   ${EXECUTABLE_OUTPUT_PATH}/Debug  )
	SET(CONFIG_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH}/Release)
ELSE()
	SET(CONFIG_DIRECTORY_DEBUG   ${EXECUTABLE_OUTPUT_PATH})
	SET(CONFIG_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})
ENDIF()

SET(QT_PLUGINS            plugins)
SET(QT_PLUGINS_PLATFORMS  ${QT_PLUGINS}/platforms)
SET(QT_PLUGINS_SQLDRIVERS ${QT_PLUGINS}/sqldrivers)

SET(QT_LIBRARY_DIR             $ENV{QTDIR}/bin                     )
SET(QT_PLUGINS_PLATFORMS_DIR   $ENV{QTDIR}/${QT_PLUGINS_PLATFORMS} )
SET(QT_PLUGINS_SQLDRIVERS_DIR  $ENV{QTDIR}/${QT_PLUGINS_SQLDRIVERS})

FIND_FILE(DLL_ICUUC       icuuc51.dll        PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_ICUIN       icuin51.dll        PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_ICUDT       icudt51.dll        PATHS ${QT_LIBRARY_DIR})
#FIND_FILE(DLL_D3DCOMPILER D3DCompiler_43.dll PATHS ${QT_LIBRARY_DIR})
SET(DLL_D_AND_R
	${DLL_ICUUC}
	${DLL_ICUIN}
	${DLL_ICUDT}
#	${DLL_D3DCOMPILER}
)

FIND_FILE(DLL_GLES libEGL.dll    PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_EGL  libGLESv2.dll PATHS ${QT_LIBRARY_DIR})
SET(CONFIG_DLL_OPTIONAL
	${DLL_GLES}
	${DLL_EGL}
)

FIND_FILE(DLL_QT_CORE_R     Qt5Core.dll     PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_QT_CORE_D     Qt5Cored.dll    PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_QT_CUI_R      Qt5Gui.dll      PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_QT_CUI_D      Qt5Guid.dll     PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_QT_WIDGETS_R  Qt5Widgets.dll  PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_QT_WIDGETS_D  Qt5Widgetsd.dll PATHS ${QT_LIBRARY_DIR})

FIND_FILE(DLL_PLUGINS_QWINDOWS_R qwindows.dll  PATHS ${QT_PLUGINS_PLATFORMS_DIR})
FIND_FILE(DLL_PLUGINS_QWINDOWS_D qwindowsd.dll PATHS ${QT_PLUGINS_PLATFORMS_DIR})
FIND_FILE(DLL_PLUGINS_QMINIMAL_R qminimal.dll  PATHS ${QT_PLUGINS_PLATFORMS_DIR})
FIND_FILE(DLL_PLUGINS_QMINIMAL_D qminimald.dll PATHS ${QT_PLUGINS_PLATFORMS_DIR})

IF (MSVC90)
	FIND_FILE(DLL_MSVCR_D msvcp90d.dll)
	FIND_FILE(DLL_MSVCP_D msvcr90d.dll)
	FIND_FILE(DLL_MSVCR_R msvcp90.dll)
	FIND_FILE(DLL_MSVCP_R msvcr90.dll)
ELSE()
	FIND_FILE(DLL_MSVCP_D msvcp100d.dll)
	FIND_FILE(DLL_MSVCR_D msvcr100d.dll)
	FIND_FILE(DLL_MSVCP_R msvcp100.dll)
	FIND_FILE(DLL_MSVCR_R msvcr100.dll)
ENDIF()

SET(CONFIG_DLL_D
	${DLL_D_AND_R}
	${DLL_QT_CORE_D}
	${DLL_QT_CUI_D}
	${DLL_QT_WIDGETS_D}
	${DLL_MSVCR_D}
	${DLL_MSVCP_D}
)
SET(CONFIG_PLUGINS_PLATFORM_D
	${DLL_PLUGINS_QWINDOWS_D}
	${DLL_PLUGINS_QMINIMAL_D}
)
FILE(COPY ${CONFIG_DLL_D}              DESTINATION ${CONFIG_DIRECTORY_DEBUG})
FILE(COPY ${CONFIG_PLUGINS_PLATFORM_D} DESTINATION ${CONFIG_DIRECTORY_DEBUG}/${QT_PLUGINS_PLATFORMS})

SET(CONFIG_DLL_R
	${DLL_D_AND_R}
	${DLL_QT_CORE_R}
	${DLL_QT_CUI_R}
	${DLL_QT_WIDGETS_R}
	${DLL_MSVCR_R}
	${DLL_MSVCP_R}
)
SET(CONFIG_PLUGINS_PLATFORM_R
	${DLL_PLUGINS_QWINDOWS_R}
	${DLL_PLUGINS_QMINIMAL_R}
)
FILE(COPY ${CONFIG_DLL_R}              DESTINATION ${CONFIG_DIRECTORY_RELEASE})
FILE(COPY ${CONFIG_PLUGINS_PLATFORM_R} DESTINATION ${CONFIG_DIRECTORY_RELEASE}/${QT_PLUGINS_PLATFORMS})

FOREACH(DLL_FILE_NAME ${CONFIG_DLL_OPTIONAL})
	IF(DLL_FILE_NAME)
		FILE(COPY ${DLL_FILE_NAME} DESTINATION ${CONFIG_DIRECTORY_DEBUG})
		FILE(COPY ${DLL_FILE_NAME} DESTINATION ${CONFIG_DIRECTORY_RELEASE})
	ENDIF()
ENDFOREACH()

CONFIGURE_FILE(${CMAKE_MODULE_PATH}/win32.install.iss.cmake ${CONFIG_DIRECTORY_RELEASE}/win32.install.iss)
