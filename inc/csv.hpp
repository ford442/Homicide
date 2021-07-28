#ifndef __CSV__HPP__
    #define __CSV__HPP__

    #include <iostream>
    #include <string>
    #include <list>

    namespace CSV{

        class Document;

        class Node{
            public:
                Node();
                Node(Document *doc);
                ~Node();

                std::string load(std::string str);

                std::string get_tag(void) const;
                std::string get_value(void) const;
                Document *get_doc(void) const;
            
            private:
                std::string tag;
                std::string value;

                Document *doc;
        };

        class Document{
            public:
                Document();
                Document(std::string path);
                ~Document();

                bool load(std::string path);
                bool reload(void);

                enum CSV_Debug_level{
                    CSV_Debug_none,
                    CSV_Debug_1,
                    CSV_Debug_2,
                    CSV_Debug_3
                };

                void set_Debug(CSV_Debug_level dbg_level);
                void set_custom_Debug(bool errors, bool logs, bool warnings);

                // return the last error (and pop it)
                std::string get_error(void) const;
                std::string get_error(const int id) const;
                std::list<std::string> get_errors(void) const;

                bool is_error(void) const;

                std::string search(std::string tag);
                std::list<Node> get_nodes();

            private:
                std::list<Node> nodes;
                std::string path;

                bool is_printing_errors;
                bool is_printing_logs;
                bool is_printing_warnings;

                std::list<std::string> errors;
                std::list<std::string> warnings;
        };
    }

    #define CSV_DEBUG_MAX CSV::Document::CSV_Debug_3
    #define CSV_DEBUG_MIN CSV::Document::CSV_Debug_none

#endif