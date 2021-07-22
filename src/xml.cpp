#include <xml.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <string>

#if (__cplusplus == 201703L)
    char * strdup(const char *str) {
        size_t len = strlen(str);
        char *x = (char *)malloc(len+1); /* 1 for the null terminator */
        if(!x) return NULL; /* malloc could not allocate memory */
        memcpy(x,str,len+1); /* copy the string into the new buffer */
        return x;
    }
#endif

using std::cerr;
using std::endl;

int ends_with(const char* haystack, const char* needle)
{
    int h_len = strlen(haystack);
    int n_len = strlen(needle);

    if (h_len < n_len)
        return false;

    for (int i = 0; i < n_len; i++) {
        if (haystack[h_len - n_len + i] != needle[i])
            return false;
    }

    return true;
}

void XMLAttribute_free(XMLAttribute* attr)
{
    free(attr->key);
    free(attr->value);
}

void XMLAttributeList_init(XMLAttributeList* list)
{
    list->heap_size = 1;
    list->size = 0;
    list->data = (XMLAttribute*) malloc(sizeof(XMLAttribute) * list->heap_size);
}

void XMLAttributeList_add(XMLAttributeList* list, XMLAttribute* attr)
{
    while (list->size >= list->heap_size) {
        list->heap_size *= 2;
        list->data = (XMLAttribute*) realloc(list->data, sizeof(XMLAttribute) * list->heap_size);
    }

    list->data[list->size++] = *attr;
}

void XMLNodeList_init(XMLNodeList* list)
{
    list->heap_size = 1;
    list->size = 0;
    list->data = (XMLNode**) malloc(sizeof(XMLNode*) * list->heap_size);
}

void XMLNodeList_add(XMLNodeList* list, XMLNode* node)
{
    while (list->size >= list->heap_size) {
        list->heap_size *= 2;
        list->data = (XMLNode**) realloc(list->data, sizeof(XMLNode*) * list->heap_size);
    }

    list->data[list->size++] = node;
}

XMLNode* XMLNodeList_at(XMLNodeList* list, int index)
{
    return list->data[index];
}

void XMLNodeList_free(XMLNodeList* list)
{
    free(list);
}

XMLNode* XMLNode_new(XMLNode* parent)
{
    XMLNode* node = (XMLNode*) malloc(sizeof(XMLNode));
    node->parent = parent;
    node->tag = NULL;
    node->inner_text = NULL;
    XMLAttributeList_init(&node->attributes);
    XMLNodeList_init(&node->children);
    if (parent)
        XMLNodeList_add(&parent->children, node);
    return node;
}

void XMLNode_free(XMLNode* node)
{
    if (node->tag)
        free(node->tag);
    if (node->inner_text)
        free(node->inner_text);
    for (int i = 0; i < node->attributes.size; i++)
        XMLAttribute_free(&node->attributes.data[i]);
    free(node);
}

XMLNode* XMLNode_child(XMLNode* parent, int index)
{
    return parent->children.data[index];
}

XMLNodeList* XMLNode_children(XMLNode* parent, const char* tag)
{
    XMLNodeList* list = (XMLNodeList*) malloc(sizeof(XMLNodeList));
    XMLNodeList_init(list);

    for (int i = 0; i < parent->children.size; i++) {
        XMLNode* child = parent->children.data[i];
        if (!strcmp(child->tag, tag))
            XMLNodeList_add(list, child);
    }

    return list;
}

char* XMLNode_attr_val(XMLNode* node, const char* key)
{
    for (int i = 0; i < node->attributes.size; i++) {
        XMLAttribute attr = node->attributes.data[i];
        if (!strcmp(attr.key, key))
            return attr.value;
    }
    return NULL;
}

XMLAttribute* XMLNode_attr(XMLNode* node, char* key)
{
    for (int i = 0; i < node->attributes.size; i++) {
        XMLAttribute* attr = &node->attributes.data[i];
        if (!strcmp(attr->key, key))
            return attr;
    }
    return NULL;
}

enum _TagType
{
    TAG_START,
    TAG_INLINE
};
typedef enum _TagType TagType;

static TagType parse_attrs(char* buf, int* i, char* lex, int* lexi, XMLNode* curr_node){
    
    XMLAttribute curr_attr = {0, 0};
    while (buf[*i] != '>') {
        lex[(*lexi)++] = buf[(*i)++];

        // Tag name
        if (buf[*i] == ' ' && !curr_node->tag) {
            lex[*lexi] = '\0';
            curr_node->tag = strdup(lex);
            *lexi = 0;
            (*i)++;
            continue;
        }

        // Usually ignore spaces
        if (lex[*lexi-1] == ' ') {
            (*lexi)--;
        }

        // Attribute key
        if (buf[*i] == '=') {
            lex[*lexi] = '\0';
            curr_attr.key = strdup(lex);
            *lexi = 0;
            continue;
        }

        // Attribute value
        if (buf[*i] == '\"') {
            if (!curr_attr.key) {
                #ifdef XML_ERRS
                    std::cerr << "littlexml ERROR :: Value has no key" << std::endl;
                #endif
                return TAG_START;
            }

            *lexi = 0;
            (*i)++;

            while (buf[*i] != '\"')
                lex[(*lexi)++] = buf[(*i)++];
            lex[*lexi] = '\0';
            curr_attr.value = strdup(lex);
            XMLAttributeList_add(&curr_node->attributes, &curr_attr);
            curr_attr.key = NULL;
            curr_attr.value = NULL;
            *lexi = 0;
            (*i)++;
            continue;
        }

        // Inline node
        if (buf[*i - 1] == '/' && buf[*i] == '>') {
            lex[*lexi] = '\0';
            if (!curr_node->tag)
                curr_node->tag = strdup(lex);
            (*i)++;
            return TAG_INLINE;
        }
    }

    return TAG_START;
}

int XMLDocument_load(XMLDocument* doc, const char* path){

    #ifdef XML_LOGS
        std::cout << std::endl;
        std::cout << "\tINFO :: littlexml : load \"" << path << "\" xml file : \"XMLDocument_load(XMLDocument* doc, const char* path)\"" << std::endl;
    #endif

    FILE* file = fopen(path, "r");
    if (!file) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    #ifdef XML_LOGS
        std::cout << "\tINFO :: littlexml : file buffer size : " << size + 1 << " chars : " << sizeof(char) * size + 1 << "o" << std::endl;
    #endif
    char* buf = (char*) malloc(sizeof(char) * size + 1);
    fread(buf, 1, size, file);
    fclose(file);
    buf[size] = '\0';

    doc->root = XMLNode_new(NULL);

    char lex[256];
    int lexi = 0;
    int i = 0;

    XMLNode* curr_node = doc->root;

    #ifdef XML_LOGS
        std::cout << "\tINFO :: littlexml : file found, reading" << std::endl;
        std::cout << "\tINFO :: littlexml : reading logs : " << std::endl << std::endl;
    #endif

    while (buf[i] != '\0'){
        if (buf[i] == '<') {
            #ifdef XML_LOGS
                std::cout << "\tINFO :: littlexml : new node" << std::endl;
            #endif
            lex[lexi] = '\0';

            // Inner text
            if (lexi > 0) {
                if (!curr_node) {
                    #ifdef XML_OUT_LOG
                        std::cout << "\tINFO :: littlexml : file reading : failure; result (false)" << std::endl;
                    #endif

                    #ifdef XML_ERRS
                        std::cerr << "littlexml ERROR : Text outside of document" << std::endl;
                    #endif
                    return false;
                }

                curr_node->inner_text = strdup(lex);
                lexi = 0;
            }

            // End of node
            if (buf[i + 1] == '/') {
                #ifdef XML_LOGS
                    std::cout << "\tINFO :: littlexml : node end found : node tag : ";
                #endif

                i += 2;
                while (buf[i] != '>')
                    lex[lexi++] = buf[i++];
                lex[lexi] = '\0';

                if (!curr_node) {
                    #ifdef XML_LOGS
                        std::cout << "\"None\"" << std::endl;
                    #endif
                    #ifdef XML_ERRS
                        std::cerr << "\tINFO :: littlexml ERROR : Already at the root" << endl;
                    #endif

                    return false;
                }

                #ifdef XML_LOGS
                    std::cout << "\"" << curr_node->tag << "\"" << std::endl;
                #endif

                if (strcmp(curr_node->tag, lex)) {
                    #ifdef XML_LOGS
                        std::cerr << "littlexml ERROR : Mismatched tags (" << curr_node->tag << " != " << lex << ")" << std::endl;
                    #endif
                    return false;
                }

                curr_node = curr_node->parent;
                i++;
                continue;
            }

            // Special nodes
            if (buf[i + 1] == '!') {
                while (buf[i] != ' ' && buf[i] != '>')
                    lex[lexi++] = buf[i++];
                lex[lexi] = '\0';

                // Comments
                if (!strcmp(lex, "<!--")) {
                    #ifdef XML_LOGS
                        std::cout << "\tINFO :: littlexml : comment found" << std::endl;
                    #endif

                    lex[lexi] = '\0';
                    while (!ends_with(lex, "-->")) {
                        lex[lexi++] = buf[i++];
                        lex[lexi] = '\0';
                    }
                    continue;
                }
            }

            // Declaration tags
            if (buf[i + 1] == '?') {
                while (buf[i] != ' ' && buf[i] != '>')
                    lex[lexi++] = buf[i++];
                lex[lexi] = '\0';

                // This is the XML declaration
                if (!strcmp(lex, "<?xml")) {
                    #ifdef XML_LOGS
                        std::cout << "\tINFO :: littlexml : xml declaration found" << std::endl;
                    #endif

                    lexi = 0;
                    XMLNode* desc = XMLNode_new(NULL);
                    parse_attrs(buf, &i, lex, &lexi, desc);

                    doc->version = XMLNode_attr_val(desc, "version");
                    doc->encoding = XMLNode_attr_val(desc, "encoding");
                    continue;
                }
            }

            // Set current node
            curr_node = XMLNode_new(curr_node);

            // Start tag
            i++;
            #ifdef XML_LOGS
                std::cout << "\tINFO :: littlexml : pars node's attributes : \"TagType parse_attrs(char* buf, int* i, char* lex, int* lexi, XMLNode* curr_node)\"" << std::endl;
            #endif

            if (parse_attrs(buf, &i, lex, &lexi, curr_node) == TAG_INLINE) {
                curr_node = curr_node->parent;
                i++;
                continue;
            }

            // Set tag name if none
            lex[lexi] = '\0';
            if (!curr_node->tag)
                curr_node->tag = strdup(lex);

            // Reset lexer
            lexi = 0;
            i++;
            continue;
        } else {
            lex[lexi++] = buf[i++];
        }
    }
    
    
    #ifdef XML_OUT_LOG
        std::cout << "\tINFO :: littlexml : file reading : success; result (true)" << std::endl << std::endl;
    #endif

    return true;
}

static void node_out(FILE* file, XMLNode* node, int indent, int times)
{
    for (int i = 0; i < node->children.size; i++) {
        XMLNode* child = node->children.data[i];

        if (times > 0)
            fprintf(file, "%0*s", indent * times, " ");
        
        fprintf(file, "<%s", child->tag);
        for (int i = 0; i < child->attributes.size; i++) {
            XMLAttribute attr = child->attributes.data[i];
            if (!attr.value || !strcmp(attr.value, ""))
                continue;
            fprintf(file, " %s=\"%s\"", attr.key, attr.value);
        }

        if (child->children.size == 0 && !child->inner_text)
            fprintf(file, " />\n");
        else {
            fprintf(file, ">");
            if (child->children.size == 0)
                fprintf(file, "%s</%s>\n", child->inner_text, child->tag);
            else {
                fprintf(file, "\n");
                node_out(file, child, indent, times + 1);
                if (times > 0)
                    fprintf(file, "%0*s", indent * times, " ");
                fprintf(file, "</%s>\n", child->tag);
            }
        }
    }
}

int XMLDocument_write(XMLDocument* doc, const char* path, int indent){
    FILE* file = fopen(path, "w");
    if (!file) {
        return false;
    }

    fprintf(
        file, "<?xml version=\"%s\" encoding=\"%s\" ?>\n",
        (doc->version) ? doc->version : "1.0",
        (doc->encoding) ? doc->encoding : "UTF-8"
    );
    node_out(file, doc->root, indent, 0);
    fclose(file);

    return true;
}

void XMLDocument_free(XMLDocument* doc)
{
    XMLNode_free(doc->root);
}