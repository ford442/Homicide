#ifndef __CSV__HPP__
    #define __CSV__HPP__

    #include <iostream>
    #include <string>

    namespace CSV{
        
        class Node{
            public:
                Node();
                ~Node();
            
            private:
                std::string tag;
                std::string value;
        }

        class Document{
            public:
                Document();
                ~Document();
            
            private:

        };
    }

#endif