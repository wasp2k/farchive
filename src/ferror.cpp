#include "ferror.h"

/* ------------------------------------------------------------------------- */

ferror::ferror() : m_lastError( ferror::UNDEFINED )
{
}

/* ------------------------------------------------------------------------- */

ferror::ferror(ferror::ERRORCODE lastError) : m_lastError( lastError )
{
}

/* ------------------------------------------------------------------------- */