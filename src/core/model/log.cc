/*
 * Copyright (c) 2006,2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "log.h"

#include "assert.h"
#include "environment-variable.h"
#include "fatal-error.h"

#include "ns3/core-config.h"

#include <cstring> // strlen
#include <iostream>
#include <list>
#include <stdexcept>
#include <utility>

/**
 * \file
 * \ingroup logging
 * ns3::LogComponent and related implementations.
 */

namespace ns3
{

/**
 * \ingroup logging
 * The Log TimePrinter.
 * This is private to the logging implementation.
 */
static TimePrinter g_logTimePrinter = nullptr;
/**
 * \ingroup logging
 * The Log NodePrinter.
 */
static NodePrinter g_logNodePrinter = nullptr;

/**
 * \ingroup logging
 * Handler for \c print-list token in NS_LOG
 * to print the list of log components.
 * This is private to the logging implementation.
 */
class PrintList
{
  public:
    PrintList(); //<! Constructor, prints the list and exits.
};

/**
 * Invoke handler for \c print-list in NS_LOG environment variable.
 * This is private to the logging implementation.
 */
static PrintList g_printList;

/* static */
LogComponent::ComponentList*
LogComponent::GetComponentList()
{
    static LogComponent::ComponentList components;
    return &components;
}

PrintList::PrintList()
{
    auto [found, value] = EnvironmentVariable::Get("NS_LOG", "print-list", ":");
    if (found)
    {
        LogComponentPrintList();
        exit(0);
    }
}

LogComponent::LogComponent(const std::string& name,
                           const std::string& file,
                           const LogLevel mask /* = 0 */)
    : m_levels(0),
      m_mask(mask),
      m_name(name),
      m_file(file)
{
    EnvVarCheck();

    LogComponent::ComponentList* components = GetComponentList();
    for (LogComponent::ComponentList::const_iterator i = components->begin();
         i != components->end();
         i++)
    {
        if (i->first == name)
        {
            NS_FATAL_ERROR("Log component \"" << name << "\" has already been registered once.");
        }
    }
    components->insert(std::make_pair(name, this));
}

LogComponent&
GetLogComponent(const std::string name)
{
    LogComponent::ComponentList* components = LogComponent::GetComponentList();
    LogComponent* ret;

    try
    {
        ret = components->at(name);
    }
    catch (std::out_of_range&)
    {
        NS_FATAL_ERROR("Log component \"" << name << "\" does not exist.");
    }
    return *ret;
}

void
LogComponent::EnvVarCheck()
{
    auto [found, value] = EnvironmentVariable::Get("NS_LOG", m_name, ":");
    if (!found)
    {
        std::tie(found, value) = EnvironmentVariable::Get("NS_LOG", "*", ":");
    }
    if (!found)
    {
        std::tie(found, value) = EnvironmentVariable::Get("NS_LOG", "***", ":");
    }

    if (!found || value == "")
    {
        return;
    }

    // Got a value, might have flags
    int level = 0;
    std::string::size_type cur_lev;
    std::string::size_type next_lev{0};
    bool pre_pipe{true}; // before the first '|', enables positional 'all', '*'
    do
    {
        cur_lev = next_lev;
        next_lev = value.find('|', cur_lev);
        std::string lev = value.substr(cur_lev, next_lev - cur_lev);

        if (lev == "error")
        {
            level |= LOG_ERROR;
        }
        else if (lev == "warn")
        {
            level |= LOG_WARN;
        }
        else if (lev == "debug")
        {
            level |= LOG_DEBUG;
        }
        else if (lev == "info")
        {
            level |= LOG_INFO;
        }
        else if (lev == "function")
        {
            level |= LOG_FUNCTION;
        }
        else if (lev == "logic")
        {
            level |= LOG_LOGIC;
        }
        else if (pre_pipe && (lev == "all" || lev == "*"))
        {
            level |= LOG_LEVEL_ALL;
        }
        else if (lev == "prefix_func" || lev == "func")
        {
            level |= LOG_PREFIX_FUNC;
        }
        else if (lev == "prefix_time" || lev == "time")
        {
            level |= LOG_PREFIX_TIME;
        }
        else if (lev == "prefix_node" || lev == "node")
        {
            level |= LOG_PREFIX_NODE;
        }
        else if (lev == "prefix_level" || lev == "level")
        {
            level |= LOG_PREFIX_LEVEL;
        }
        else if (lev == "prefix_all" || (!pre_pipe && (lev == "all" || lev == "*")))
        {
            level |= LOG_PREFIX_ALL;
        }
        else if (lev == "level_error")
        {
            level |= LOG_LEVEL_ERROR;
        }
        else if (lev == "level_warn")
        {
            level |= LOG_LEVEL_WARN;
        }
        else if (lev == "level_debug")
        {
            level |= LOG_LEVEL_DEBUG;
        }
        else if (lev == "level_info")
        {
            level |= LOG_LEVEL_INFO;
        }
        else if (lev == "level_function")
        {
            level |= LOG_LEVEL_FUNCTION;
        }
        else if (lev == "level_logic")
        {
            level |= LOG_LEVEL_LOGIC;
        }
        else if (lev == "level_all")
        {
            level |= LOG_LEVEL_ALL;
        }
        else if (lev == "**")
        {
            level |= LOG_LEVEL_ALL | LOG_PREFIX_ALL;
        }

        pre_pipe = false;
    } while (next_lev != std::string::npos);

    Enable((LogLevel)level);
}

bool
LogComponent::IsEnabled(const LogLevel level) const
{
    //  LogComponentEnableEnvVar ();
    return (level & m_levels) ? 1 : 0;
}

bool
LogComponent::IsNoneEnabled() const
{
    return m_levels == 0;
}

void
LogComponent::SetMask(const LogLevel level)
{
    m_mask |= level;
}

void
LogComponent::Enable(const LogLevel level)
{
    m_levels |= (level & ~m_mask);
}

void
LogComponent::Disable(const LogLevel level)
{
    m_levels &= ~level;
}

const char*
LogComponent::Name() const
{
    return m_name.c_str();
}

std::string
LogComponent::File() const
{
    return m_file;
}

/* static */
std::string
LogComponent::GetLevelLabel(const LogLevel level)
{
    if (level == LOG_ERROR)
    {
        return "ERROR";
    }
    else if (level == LOG_WARN)
    {
        // whitespace left at the end for alignment
        return "WARN ";
    }
    else if (level == LOG_DEBUG)
    {
        return "DEBUG";
    }
    else if (level == LOG_INFO)
    {
        // whitespace left at the end for alignment
        return "INFO ";
    }
    else if (level == LOG_FUNCTION)
    {
        return "FUNCT";
    }
    else if (level == LOG_LOGIC)
    {
        return "LOGIC";
    }
    else
    {
        return "unknown";
    }
}

void
LogComponentEnable(const char* name, LogLevel level)
{
    LogComponent::ComponentList* components = LogComponent::GetComponentList();
    LogComponent::ComponentList::const_iterator i;
    for (i = components->begin(); i != components->end(); i++)
    {
        if (i->first == name)
        {
            i->second->Enable(level);
            return;
        }
    }
    if (i == components->end())
    {
        // nothing matched
        LogComponentPrintList();
        NS_FATAL_ERROR("Logging component \""
                       << name << "\" not found. See above for a list of available log components");
    }
}

void
LogComponentEnableAll(LogLevel level)
{
    LogComponent::ComponentList* components = LogComponent::GetComponentList();
    for (LogComponent::ComponentList::const_iterator i = components->begin();
         i != components->end();
         i++)
    {
        i->second->Enable(level);
    }
}

void
LogComponentDisable(const char* name, LogLevel level)
{
    LogComponent::ComponentList* components = LogComponent::GetComponentList();
    for (LogComponent::ComponentList::const_iterator i = components->begin();
         i != components->end();
         i++)
    {
        if (i->first == name)
        {
            i->second->Disable(level);
            break;
        }
    }
}

void
LogComponentDisableAll(LogLevel level)
{
    LogComponent::ComponentList* components = LogComponent::GetComponentList();
    for (LogComponent::ComponentList::const_iterator i = components->begin();
         i != components->end();
         i++)
    {
        i->second->Disable(level);
    }
}

void
LogComponentPrintList()
{
    LogComponent::ComponentList* components = LogComponent::GetComponentList();
    for (LogComponent::ComponentList::const_iterator i = components->begin();
         i != components->end();
         i++)
    {
        std::cout << i->first << "=";
        if (i->second->IsNoneEnabled())
        {
            std::cout << "0" << std::endl;
            continue;
        }
        if (i->second->IsEnabled(LOG_LEVEL_ALL))
        {
            std::cout << "all";
        }
        else
        {
            if (i->second->IsEnabled(LOG_ERROR))
            {
                std::cout << "error";
            }
            if (i->second->IsEnabled(LOG_WARN))
            {
                std::cout << "|warn";
            }
            if (i->second->IsEnabled(LOG_DEBUG))
            {
                std::cout << "|debug";
            }
            if (i->second->IsEnabled(LOG_INFO))
            {
                std::cout << "|info";
            }
            if (i->second->IsEnabled(LOG_FUNCTION))
            {
                std::cout << "|function";
            }
            if (i->second->IsEnabled(LOG_LOGIC))
            {
                std::cout << "|logic";
            }
        }
        if (i->second->IsEnabled(LOG_PREFIX_ALL))
        {
            std::cout << "|prefix_all";
        }
        else
        {
            if (i->second->IsEnabled(LOG_PREFIX_FUNC))
            {
                std::cout << "|func";
            }
            if (i->second->IsEnabled(LOG_PREFIX_TIME))
            {
                std::cout << "|time";
            }
            if (i->second->IsEnabled(LOG_PREFIX_NODE))
            {
                std::cout << "|node";
            }
            if (i->second->IsEnabled(LOG_PREFIX_LEVEL))
            {
                std::cout << "|level";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * \ingroup logging
 * Check if a log component exists.
 * This is private to the logging implementation.
 *
 * \param [in] componentName The putative log component name.
 * \returns \c true if \c componentName exists.
 */
static bool
ComponentExists(std::string componentName)
{
    const char* name = componentName.c_str();
    LogComponent::ComponentList* components = LogComponent::GetComponentList();
    LogComponent::ComponentList::const_iterator i;
    for (i = components->begin(); i != components->end(); i++)
    {
        if (i->first == name)
        {
            return true;
        }
    }
    NS_ASSERT(i == components->end());
    // nothing matched
    return false;
}

/**
 * \ingroup logging
 * Parse the \c NS_LOG environment variable.
 * This is private to the logging implementation.
 */
static void
CheckEnvironmentVariables()
{
    auto dict = EnvironmentVariable::GetDictionary("NS_LOG", ":")->GetStore();

    for (auto& [component, value] : dict)
    {
        if (value == "")
        {
            if (ComponentExists(component) || component == "*" || component == "***")
            {
                return;
            }
            else
            {
                LogComponentPrintList();
                NS_FATAL_ERROR(
                    "Invalid or unregistered component name \""
                    << component
                    << "\" in env variable NS_LOG, see above for a list of valid components");
            }
        }
        else
        {
            if (ComponentExists(component) || component == "*")
            {
                std::string::size_type cur_lev;
                std::string::size_type next_lev{0};
                do
                {
                    cur_lev = next_lev;
                    next_lev = value.find('|', cur_lev);
                    std::string lev = value.substr(cur_lev, next_lev - cur_lev);
                    if (lev == "error" || lev == "warn" || lev == "debug" || lev == "info" ||
                        lev == "function" || lev == "logic" || lev == "all" ||
                        lev == "prefix_func" || lev == "func" || lev == "prefix_time" ||
                        lev == "time" || lev == "prefix_node" || lev == "node" ||
                        lev == "prefix_level" || lev == "level" || lev == "prefix_all" ||
                        lev == "level_error" || lev == "level_warn" || lev == "level_debug" ||
                        lev == "level_info" || lev == "level_function" || lev == "level_logic" ||
                        lev == "level_all" || lev == "*" || lev == "**")
                    {
                        continue;
                    }
                    else
                    {
                        NS_FATAL_ERROR("Invalid log level \""
                                       << lev << "\" in env variable NS_LOG for component name "
                                       << component);
                    }
                } while (next_lev != std::string::npos);
            }
            else
            {
                LogComponentPrintList();
                NS_FATAL_ERROR(
                    "Invalid or unregistered component name \""
                    << component
                    << "\" in env variable NS_LOG, see above for a list of valid components");
            }
        } // else

    } // for
}

void
LogSetTimePrinter(TimePrinter printer)
{
    g_logTimePrinter = printer;
    /** \internal
     *  This is the only place where we are more or less sure that all log variables
     * are registered. See \bugid{1082} for details.
     */
    CheckEnvironmentVariables();
}

TimePrinter
LogGetTimePrinter()
{
    return g_logTimePrinter;
}

void
LogSetNodePrinter(NodePrinter printer)
{
    g_logNodePrinter = printer;
}

NodePrinter
LogGetNodePrinter()
{
    return g_logNodePrinter;
}

ParameterLogger::ParameterLogger(std::ostream& os)
    : m_os(os)
{
}

void
ParameterLogger::CommaRest()
{
    if (m_first)
    {
        m_first = false;
    }
    else
    {
        m_os << ", ";
    }
}

template <>
ParameterLogger&
ParameterLogger::operator<< <std::string>(const std::string& param)
{
    CommaRest();
    m_os << "\"" << param << "\"";
    return *this;
}

ParameterLogger&
ParameterLogger::operator<<(const char* param)
{
    (*this) << std::string(param);
    return *this;
}

template <>
ParameterLogger&
ParameterLogger::operator<< <int8_t>(const int8_t param)
{
    (*this) << static_cast<int16_t>(param);
    return *this;
}

template <>
ParameterLogger&
ParameterLogger::operator<< <uint8_t>(const uint8_t param)
{
    (*this) << static_cast<uint16_t>(param);
    return *this;
}

} // namespace ns3
