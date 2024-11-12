#pragma once

#include <QtCore/qglobal.h>

#   ifdef APP_LIB_EXPORTS
#       define APP_LIB_EXPORT Q_DECL_EXPORT
#   else
#       ifdef APP_LIB_IMPORTS
#           define APP_LIB_EXPORT Q_DECL_IMPORT
#       else
#           define APP_LIB_EXPORT
#       endif
#   endif