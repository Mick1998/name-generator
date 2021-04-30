#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <filesystem>
#include <fstream>

// Written by Diego Dasso Migotto - diegomigotto at hotmail dot com
namespace dasmig
{
    // The name generator generates names and surnames, allowing requests specifying gender and culture.
    // Internally it peaks from a list of popular names and surnames in each country.
    class ng
    {
        public:
            // Culture representing a country or a broader group (e.g danish, german)
            enum class culture
            {
                american,
                argentinian,
                australian,
                brazilian,
                british,
                bulgarian,
                canadian,
                chinese,
                danish,
                finnish,
                french,
                german,
                kazakh,
                mexican,
                norwegian,
                polish,
                portuguese,
                russian,
                spanish,
                swedish,
                turkish,
                ukrainian,
                any
            };

            // Simple gender enum to distinguish between male and female names.
            enum class gender
            {
                m,
                f,
                any
            };

            // Internal class used by name generator to allow for chained calls when building a full name.
            class name
            {
                public:
                    // Method to append a name to the generated name (useful for composed forenames).
                    name append_name() { _internal_string.append(L" ").append(ng::instance().get_name(_gender, _culture)); return *this; }

                    // Method to append a name to the generated name (useful for composed forenames).
                    name append_name(culture culture) { _internal_string.append(L" ").append(ng::instance().get_name(_gender, culture)); return *this; }

                    // Method to append a surname to the generated name.
                    name append_surname() { _internal_string.append(L" ").append(ng::instance().get_surname(_culture)); return *this; }

                    // Method to append a surname to the generated name.
                    name append_surname(culture culture) { _internal_string.append(L" ").append(ng::instance().get_surname(culture)); return *this; }
                    
                    // Operator string to allow for implicit conversion to string.
                    operator std::wstring() const { return _internal_string; }
                    
                    // Operator ostream streaming internal string.
                    friend std::wostream& operator<<(std::wostream& os, const name& name) { os << name._internal_string; return os; }
                    
                private:
                    // Private constructor, this is mostly a helper class to the name generator, not the intended API.
                    name(const std::wstring& name_str, gender gender, culture culture) : _internal_string(name_str), _gender(gender), _culture(culture) {}

                    // Internal string containing all name and appended surnames.
                    std::wstring _internal_string;

                    // Internal indicator of original gender of the first generated name.
                    gender _gender;

                    // Internal indicator of original culture of the first generated name.
                    culture _culture;
                
                // Allows name generator to construct names.
                friend class ng;
            };

            // Thread safe access to name generator singleton.
            static ng& instance() { static ng instance; return instance; }

            // Translates ISO 3166 2-letter country code to internal culture enum, unknown or unsupported code will be translated as any.
            static culture to_culture(const std::wstring& country_code) { return (_country_code_map.find(country_code) != _country_code_map.end()) ? _country_code_map.at(country_code) : culture::any; }

            // Generates a first name based on requested gender and culture.
            name get_name(gender gender = gender::any, culture culture = culture::any) const { return solver(true, gender, culture); };
            
            // Generates a surname based on requested culture.
            name get_surname(culture culture = culture::any) const { return solver(false, gender::any, culture); };       

            // Try loading every possible names file from the received resource path.
            void load(const std::filesystem::path& resource_path) 
            { 
                for (auto& f : std::filesystem::recursive_directory_iterator(resource_path))
                    if (f.is_regular_file() && (f.path().extension() == ".names"))
                        parse_file(f);
            };

        private:
            // Typedef to avoid type horror when defining a pointer to a container of names.
            typedef std::shared_ptr<std::vector<std::wstring>> name_container;

            // Maps ISO 3166 2-letter country codes to internal culture enum.
            static const inline std::map<std::wstring, culture> _country_code_map = {
                { L"ar" , culture::argentinian },
                { L"us" , culture::american },
                { L"au" , culture::australian },
                { L"br" , culture::brazilian },
                { L"gb" , culture::british },
                { L"bg" , culture::bulgarian },
                { L"ca" , culture::canadian },
                { L"cn" , culture::chinese },
                { L"dk" , culture::danish },
                { L"fi" , culture::finnish },
                { L"fr" , culture::french },
                { L"de" , culture::german },
                { L"kz" , culture::kazakh },
                { L"mx" , culture::mexican },
                { L"no" , culture::norwegian },
                { L"pl" , culture::polish },
                { L"pt" , culture::portuguese },
                { L"ru" , culture::russian },
                { L"es" , culture::spanish },
                { L"se" , culture::swedish },
                { L"tr" , culture::turkish },
                { L"ua" , culture::ukrainian }
            };

            // Default folder to look for names and surnames resources. 
            static const inline std::filesystem::path _default_resources_path = "resources";

            // Maps for accessing male name through culture.
            std::map<culture, name_container> _culture_indexed_m_names;

            // Maps for accessing female name through culture.
            std::map<culture, name_container> _culture_indexed_f_names;
            
            // Maps for accessing surname through culture.
            std::map<culture, name_container> _culture_indexed_surnames;

            // Initialize random generator, no complicated processes.
            ng() { load(_default_resources_path); };

            // We don't manage any resource, all should gracefully deallocate by itself.
            ~ng() {};

            // Contains logic to retrieve a random name/surname from the containers. 
            name solver(bool is_name, gender requested_gender, culture requested_culture) const 
            {
                // Utilized in case gender or culture is any.
                std::random_device random_device;

                // Distribution of possible culture values.  
                std::uniform_int_distribution<std::size_t> culture_range(0, static_cast<std::size_t>(culture::any) - 1);

                // Distribution of possible gender values.  
                std::uniform_int_distribution<std::size_t> gender_range(0, static_cast<std::size_t>(gender::any) - 1);

                // Randomly select a culture if necessary.
                if (requested_culture == culture::any)
                {
                    requested_culture = static_cast<culture>(culture_range(random_device));
                }

                // Randomly select gender if necessary.
                if (requested_gender == gender::any)
                {
                    requested_gender = static_cast<gender>(gender_range(random_device));
                }

                // Decides what container it will access.
                if (is_name)
                {
                    // Decides what container it will access.
                    switch (requested_gender)
                    {
                        case gender::m:
                        {
                            // Distribution of possible names.
                            std::uniform_int_distribution<std::size_t> names_range(0, _culture_indexed_m_names.at(requested_culture)->size() - 1);
                            // Randomly select a name of the requested gender and culture.
                            return name(_culture_indexed_m_names.at(requested_culture)->at(names_range(random_device)), requested_gender, requested_culture);
                        }
                        case gender::f:
                        {
                            // Distribution of possible names.
                            std::uniform_int_distribution<std::size_t> names_range(0, _culture_indexed_f_names.at(requested_culture)->size() - 1);
                            // Randomly select a name of the requested gender and culture.
                            return name(_culture_indexed_f_names.at(requested_culture)->at(names_range(random_device)), requested_gender, requested_culture);
                        }
                        default:
                        {
                            // Distribution of possible names.
                            std::uniform_int_distribution<std::size_t> names_range(0, _culture_indexed_m_names.at(requested_culture)->size() - 1);
                            // Randomly select a name of the requested gender and culture.
                            return name(_culture_indexed_m_names.at(requested_culture)->at(names_range(random_device)), requested_gender, requested_culture);
                        }
                    }
                }
                else
                {
                    // Distribution of possible surnames.
                    std::uniform_int_distribution<std::size_t> surnames_range(0, _culture_indexed_surnames.at(requested_culture)->size() - 1);
                    // Randomly select a surname of the requested culture.
                    return name(_culture_indexed_surnames.at(requested_culture)->at(surnames_range(random_device)), requested_gender, requested_culture);
                }
            };

            // Translates possible gender strings to gender enum.
            static gender to_gender(const std::wstring& gender_string) 
            {
                static const std::map<std::wstring, gender> gender_map = {
                    { L"m", gender::m },
                    { L"f", gender::f },
                    { L"male", gender::m },
                    { L"female", gender::f }
                };

                return (gender_map.find(gender_string) != gender_map.end()) ? gender_map.at(gender_string) : gender::any; 
            }

            // Try parsing the names file and index it into our container.
            void parse_file(const std::filesystem::path& file) 
            {
                // Expected names file format is <ISO 3166 2-letter country code>, <m(ale)|f(emale)|s(urname)>, list of names.
                std::wifstream tentative_file(file);

                // If managed to open the file proceed.
                if (tentative_file.is_open())
                {
                    // Expected delimiter character.
                    const wchar_t delimiter('\n');

                    // Line being read from the file.
                    std::wstring file_line;

                    // Culture read from file header.
                    culture culture_read = culture::any;

                    // Gender read from file header.
                    gender gender_read = gender::any;

                    // List of parsed names.
                    name_container names_read = std::make_shared<std::vector<std::wstring>>();

                    // Retrieves culture from file being read.
                    if (std::getline(tentative_file, file_line, delimiter))
                    {
                        culture_read = to_culture(file_line);
                    }

                    // We can't continue without a valid culture.
                    if (culture_read != culture::any)
                    {
                        // Retrieves gender, if unable to parse a gender it should be a surname file.
                        if (std::getline(tentative_file, file_line, delimiter))
                        {
                            gender_read = to_gender(file_line);
                        }
                            
                        // Retrieves list of names.
                        while (std::getline(tentative_file, file_line, delimiter))
                        {
                            names_read->push_back(file_line);
                        }

                        // Correctly index container.
                        switch (gender_read)
                        {
                            case gender::m:
                            {
                                _culture_indexed_m_names.emplace(culture_read, names_read);
                                break;
                            }
                            case gender::f:
                            {
                                _culture_indexed_f_names.emplace(culture_read, names_read);
                                break;
                            }
                            case gender::any:
                            default:
                            {
                                _culture_indexed_surnames.emplace(culture_read, names_read);
                                break;
                            }
                        }
                    }
                }
            }

    };

}