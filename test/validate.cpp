#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>

int validate_xmf(char *filename, bool keep_validate_file) {
    xmlTextReaderPtr reader;
    int ret;
     
    xmlDocPtr doc = xmlReadFile(filename, NULL, XML_PARSE_DTDATTR | XML_PARSE_NOENT 
        | XML_PARSE_XINCLUDE );//| XML_PARSE_NOERROR);
    /*
     * apply the XInclude process, this should trigger the I/O just
     * registered.
     */
    bool includes = false;
    if (xmlXIncludeProcess(doc) <= 0) {
        fprintf(stderr, "XInclude processing failed. Are there any XInclude?\n");
        includes = true;
    }

    const char* tmp_file = "to_validate.xmf";
    xmlSaveFile(tmp_file, doc);

    /*
     * Free the document
     */
    xmlFreeDoc(doc);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();

    reader = xmlReaderForFile(tmp_file, NULL,
             XML_PARSE_DTDATTR | XML_PARSE_NOENT |
            XML_PARSE_DTDVALID | XML_PARSE_XINCLUDE); 

    if (reader != NULL) {
        //ret = xmlTextReaderRead(reader);
    
        /*
         * Once the document has been fully parsed check the validation results
         */
        if (xmlTextReaderIsValid(reader) != 1) {
            fprintf(stderr, "Document %s does not validate\n", filename);
            return 1;
        }
        
        // if (ret != 0) {
        //     fprintf(stderr, "%s : failed to parse\n", filename);
        // }
    } else {
        fprintf(stderr, "Unable to open %s\n", filename);
        return 1;
    }
    
    printf("Validation done\n");

    if(!keep_validate_file)
        remove(tmp_file);

    return 0;
}

int main(int argc, char** argv){

    return validate_xmf(argv[1], argc > 2);
}
