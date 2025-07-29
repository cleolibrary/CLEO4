#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class OpcodeInfoDatabase
{
  public:
    enum class CommandArgumentType : BYTE
    {
        Other, // enum or class
        Any,
        Arguments,
        Bool,
        Float,
        GxtKey,
        Int,
        Label,
        ModelAny,
        ModelChar,
        ModelObject,
        ModelVehicle,
        ScriptId,
        String,
        String128,
        ZoneKey
    };

    struct CommandArgument
    {
        std::string name          = "?";
        CommandArgumentType type  = CommandArgumentType::Any;
        std::string typeName      = "any";
        std::string typeNameLower = "any"; // same but lower case
        std::string source        = "any"; // any, any_var, etc.
    };

    struct Command
    {
        std::string extension = "?"; // CLEO plugin name

        uint16_t id           = -1; // opcode
        std::string name      = "?";
        std::string nameLower = "?"; // same but lower case

        std::string oper   = ""; // operator
        bool isCondition   = false;
        bool isConstructor = false;
        bool isNop         = false;

        std::vector<CommandArgument> arguments; // input and output params
        size_t inputArguments = 0;              // count

        bool NameEqual(const char* name) const { return _strcmpi(name, this->name.c_str()) == 0; }
        bool IsComparison() const
        {
            if (!isCondition || oper.empty()) return false;
            return oper == ">" || oper == ">=" || oper == "==" || "<" || oper == "<=" || oper == "<>";
        }
    };

    struct Enum
    {
        std::string name = "?";
        std::map<int, std::string> valuesNum;         // num value - name
        std::map<std::string, std::string> valuesTxt; // txt value - name

        bool IsNumeric() const { return valuesTxt.empty(); }
        bool IsEmpty() const { return valuesNum.empty() && valuesTxt.empty(); }
        const char* GetEntryName(int key) const { return valuesNum.count(key) ? valuesNum.at(key).c_str() : nullptr; }
        const char* GetEntryName(const char* key) const
        {
            return valuesTxt.count(key) ? valuesTxt.at(key).c_str() : nullptr;
        }
    };

    OpcodeInfoDatabase() = default;

    bool LoadCommands(const char* filepath); // mode json file
    bool LoadEnums(const char* filepath);    // enums.json file

    bool HasCommand(uint16_t opcode) const;
    bool HasCommand(const char* name) const;

    const Command* GetCommand(uint16_t opcode) const;
    const Command* GetCommand(const char* name) const;

    const char* GetExtensionName(uint16_t opcode) const;         // nullptr if not found
    const char* GetExtensionName(const char* commandName) const; // nullptr if not found

    uint16_t GetOpcode(const char* commandName) const; // 0xFFFF if not found
    const char* GetCommandName(uint16_t opcode) const; // nullptr if not found

    const char* GetArgumentName(uint16_t opcode, size_t paramIdx) const; // nullptr if not found

    std::string GetExtensionMissingMessage(
        uint16_t opcode
    ) const; // extension "x" missing message if known, empty text otherwise

    bool HasEnum(const char* name) const;        // lower case enum name
    const Enum* GetEnum(const char* name) const; // lower case enum name

  private:
    std::unordered_map<uint16_t, Command> m_commands;
    std::unordered_map<std::string, Enum> m_enums;

    static CommandArgumentType TypeFromName(const std::string_view& name);
};
