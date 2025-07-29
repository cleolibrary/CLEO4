#include "stdafx.h"
#include "OpcodeInfoDatabase.h"
#include <algorithm>
#include <fstream>

using namespace std;
using namespace simdjson;

bool OpcodeInfoDatabase::LoadCommands(const char* filepath)
{
    m_commands.clear();

    dom::parser parser;
    dom::element root;

    auto error = parser.load(filepath).get(root);
    if (error)
    {
        TRACE("Failed to parse commands database '%s' file. Code %d", filepath, error);
        return false;
    }

    const char* version;
    if (root["meta"]["version"].get_c_str().get(version))
    {
        TRACE("Invalid commands database '%s' file.", filepath);
        return false;
    }

    dom::array ext;
    if (root["extensions"].get_array().get(ext))
    {
        TRACE("Invalid commands database '%s' file.", filepath);
        return false;
    }

    for (auto& e : ext)
    {
        const char* extensionName;
        if (e["name"].get_c_str().get(extensionName))
        {
            continue; // invalid extension
        }

        dom::array commands;
        if (e["commands"].get_array().get(commands))
        {
            continue; // invalid extension
        }

        for (auto& c : commands)
        {
            // unsupported?
            bool unsupported;
            if (!c["attrs"]["is_unsupported"].get_bool().get(unsupported) && unsupported)
            {
                continue; // command defined as unsupported
            }

            // opcode
            const char* commandId;
            if (c["id"].get_c_str().get(commandId))
            {
                continue; // invalid command
            }
            auto idLong = stoul(commandId, nullptr, 16);
            if (idLong > 0x7FFF)
            {
                continue; // opcode out of bounds
            }
            auto id = (uint16_t)idLong;

            // command already present?
            if (m_commands.count(id))
            {
                m_commands[id].extension.push_back('/');
                m_commands[id].extension.append(extensionName);
                continue;
            }

            // name
            const char* commandName;
            if (c["name"].get_c_str().get(commandName))
            {
                continue; // invalid command
            }

            // store
            Command& command  = m_commands[id];
            command.extension = extensionName;
            command.id        = id;
            command.name      = commandName;
            command.nameLower = commandName;
            CLEO::StringToLower(command.nameLower);

            // operator
            const char* commandOperator = nullptr;
            if (!c["operator"].get_c_str().get(commandOperator))
            {
                command.oper = commandOperator;
            }

            // attributes
            dom::element attr;
            if (!c["attrs"].get(attr))
            {
                bool state;
                if (!attr["is_condition"].get_bool().get(state))
                {
                    command.isCondition = state;
                }
                if (!attr["is_constructor"].get_bool().get(state))
                {
                    command.isConstructor = state;
                }
                if (!attr["is_nop"].get_bool().get(state))
                {
                    command.isNop = state;
                }
            }

            // command params
            auto LoadParams = [&](const char* obj, bool isInput) {
                dom::array args;
                if (!c[obj].get_array().get(args))
                {
                    for (auto& p : args)
                    {
                        const char* argName;
                        if (!p["name"].get_c_str().get(argName))
                        {
                            auto& arg = command.arguments.emplace_back();
                            if (isInput) command.inputArguments++;

                            arg.name = argName;

                            const char* argType;
                            if (!p["type"].get_c_str().get(argType))
                            {
                                arg.typeName      = argType;
                                arg.typeNameLower = argType;
                                CLEO::StringToLower(arg.typeNameLower);
                                arg.type = TypeFromName(arg.typeName);
                            }

                            const char* argSource;
                            if (!p["source"].get_c_str().get(argSource))
                            {
                                arg.source = argSource;
                                CLEO::StringToLower(arg.source);
                            }
                        }
                    }
                }
            };
            LoadParams("input", true);
            LoadParams("output", false);
        }
    }

    TRACE("Commands database version %s loaded from '%s'", version, filepath);
    return true;
}

bool OpcodeInfoDatabase::LoadEnums(const char* filepath)
{
    m_enums.clear();

    dom::parser parser;
    dom::object root;

    auto error = parser.load(filepath).get(root);
    if (error)
    {
        TRACE("Failed to parse enums database '%s' file. Code %d", filepath, error);
        return false;
    }

    for (auto& enumPair : root)
    {
        auto name = std::string(enumPair.key);

        Enum currEnum = Enum();
        currEnum.name = name;

        CLEO::StringToLower(name);
        if (m_enums.find(name) != m_enums.end())
        {
            TRACE(
                "Failed to parse enums database '%s' file. Enum '%s' already defined!", filepath, currEnum.name.c_str()
            );
            return false;
        }

        int nextValue = 0;
        dom::object values;
        if (!enumPair.value.get_object().get(values))
        {
            for (auto& valPair : values)
            {
                auto valName = std::string(valPair.key);

                if (valPair.value.is_null())
                {
                    if (!currEnum.IsEmpty() && !currEnum.IsNumeric())
                    {
                        TRACE(
                            "Failed to parse enums database '%s' file. Enum '%s' contains mixed type values!", filepath,
                            currEnum.name.c_str()
                        );
                        return false;
                    }

                    currEnum.valuesNum[nextValue] = std::move(valName);
                    nextValue++;
                    continue;
                }

                int64_t num;
                if (!valPair.value.get_int64().get(num))
                {
                    if (!currEnum.IsEmpty() && !currEnum.IsNumeric())
                    {
                        TRACE(
                            "Failed to parse enums database '%s' file. Enum '%s' contains mixed type values!", filepath,
                            currEnum.name.c_str()
                        );
                        return false;
                    }

                    nextValue                     = (int)num;
                    currEnum.valuesNum[nextValue] = std::move(valName);
                    nextValue++;
                    continue;
                }

                std::string_view txt;
                if (!valPair.value.get_string().get(txt))
                {
                    if (!currEnum.IsEmpty() && currEnum.IsNumeric())
                    {
                        TRACE(
                            "Failed to parse enums database '%s' file. Enum '%s' contains mixed type values!", filepath,
                            currEnum.name.c_str()
                        );
                        return false;
                    }

                    currEnum.valuesTxt[std::string(txt)] = std::move(valName);
                    continue;
                }

                TRACE(
                    "Failed to parse enums database '%s' file. Enum '%s' contains invalid value type!", filepath,
                    currEnum.name.c_str()
                );
                return false;
            }
        }

        if (!currEnum.IsEmpty())
        {
            m_enums.emplace(std::move(name), std::move(currEnum));
        }
    }

    TRACE("Database of %d enums loaded from '%s'", m_enums.size(), filepath);
    return true;
}

bool OpcodeInfoDatabase::HasCommand(uint16_t opcode) const
{
    return m_commands.count(opcode);
}

bool OpcodeInfoDatabase::HasCommand(const char* name) const
{
    return GetCommand(name) != nullptr;
}

const OpcodeInfoDatabase::Command* OpcodeInfoDatabase::GetCommand(uint16_t opcode) const
{
    return HasCommand(opcode) ? &m_commands.at(opcode) : nullptr;
}

const OpcodeInfoDatabase::Command* OpcodeInfoDatabase::GetCommand(const char* name) const
{
    for (auto& [opcode, command] : m_commands)
    {
        if (command.NameEqual(name))
        {
            return &command;
        }
    }

    return nullptr;
}

const char* OpcodeInfoDatabase::GetExtensionName(uint16_t opcode) const
{
    auto command = GetCommand(opcode);
    return command ? command->extension.c_str() : nullptr;
}

const char* OpcodeInfoDatabase::GetExtensionName(const char* name) const
{
    auto command = GetCommand(name);
    return command ? command->extension.c_str() : nullptr;
}

uint16_t OpcodeInfoDatabase::GetOpcode(const char* name) const
{
    auto command = GetCommand(name);
    return command ? command->id : 0xFFFF;
}

const char* OpcodeInfoDatabase::GetCommandName(uint16_t opcode) const
{
    auto command = GetCommand(opcode);
    return command ? command->name.c_str() : nullptr;
}

const char* OpcodeInfoDatabase::GetArgumentName(uint16_t opcode, size_t paramIdx) const
{
    auto command = GetCommand(opcode);

    if (command == nullptr || paramIdx >= command->arguments.size())
    {
        return "";
    }

    return command->arguments[paramIdx].name.c_str();
}

std::string OpcodeInfoDatabase::GetExtensionMissingMessage(uint16_t opcode) const
{
    auto extensionName = GetExtensionName(opcode);
    if (extensionName == nullptr)
    {
        return {};
    }

    return CLEO::StringPrintf("CLEO extension plugin \"%s\" is missing!", extensionName);
}

bool OpcodeInfoDatabase::HasEnum(const char* name) const
{
    return m_enums.count(name);
}

const OpcodeInfoDatabase::Enum* OpcodeInfoDatabase::GetEnum(const char* name) const
{
    return HasEnum(name) ? &m_enums.at(name) : nullptr;
}

OpcodeInfoDatabase::CommandArgumentType OpcodeInfoDatabase::TypeFromName(const std::string_view& name)
{
    auto str = name.data();
    auto len = name.length();

    if (!_strnicmp(str, "any", len)) return CommandArgumentType::Any;
    if (!_strnicmp(str, "arguments", len)) return CommandArgumentType::Arguments;
    if (!_strnicmp(str, "bool", len)) return CommandArgumentType::Bool;
    if (!_strnicmp(str, "float", len)) return CommandArgumentType::Float;
    if (!_strnicmp(str, "gxt_key", len)) return CommandArgumentType::GxtKey;
    if (!_strnicmp(str, "int", len)) return CommandArgumentType::Int;
    if (!_strnicmp(str, "label", len)) return CommandArgumentType::Label;
    if (!_strnicmp(str, "model_any", len)) return CommandArgumentType::ModelAny;
    if (!_strnicmp(str, "model_char", len)) return CommandArgumentType::ModelChar;
    if (!_strnicmp(str, "model_object", len)) return CommandArgumentType::ModelObject;
    if (!_strnicmp(str, "model_vehicle", len)) return CommandArgumentType::ModelVehicle;
    if (!_strnicmp(str, "script_id", len)) return CommandArgumentType::ScriptId;
    if (!_strnicmp(str, "string", len)) return CommandArgumentType::String;
    if (!_strnicmp(str, "string128", len)) return CommandArgumentType::String128;
    if (!_strnicmp(str, "zone_key", len)) return CommandArgumentType::ZoneKey;
    return CommandArgumentType::Other;
}
