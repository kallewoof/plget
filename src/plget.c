#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>

xmlDocPtr doc;
xmlNodePtr cur;
xmlChar *val;

void iterateCur(); ///< move to next non-text entry
void enterCur();   ///< go into the first non-text child of cur
void requireName(const char *name);
void getValue();
void freeValue();

int main(int argc, const char **argv)
{
    if (argc != 3) {
	fprintf(stderr, "syntax: %s <plist> <property to read>\n", argv[0]);
	return 1;
    }

    const char *filename = argv[1];
    const char *property = argv[2];
    
    doc = xmlParseFile(filename);
    val = NULL;

    if (doc == NULL) {
	fprintf(stderr, "Document not parsed successfully.\n");
	return 1;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
	fprintf(stderr, "empty document\n");
	xmlFreeDoc(doc);
	return 1;
    }

    requireName("plist");
    enterCur(); // plist -> dict
    requireName("dict");

    // the property to get is the "key" value for the plist so we try to find that
    cur = cur->xmlChildrenNode;
    
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "key"))) {
	    // this is a key entry
	    getValue();
	    if (!xmlStrcmp(val, (const xmlChar *)property)) {
		// got the property; we expect
		// <key>property</key>
		// <valtype>sought after value</valtype>
		// where "valtype" can be e.g. "string", "integer", etc. we ignore valtype and always get strings
		iterateCur();
		if (cur == NULL) {
		    fprintf(stderr, "missing xml element after <key>\n");
		    return 1;
		}
		getValue();
		printf("%s", val);
		freeValue();
		xmlFreeDoc(doc);
		return 0;
	    }
	}
	iterateCur();
    }

    // clean up
    freeValue();
    xmlFreeDoc(doc);

    return 0;
}

void enterCur()
{
    cur = cur->xmlChildrenNode;
    while (cur && cur->next && !xmlStrcmp(cur->name, (const xmlChar *)"text")) {
	cur = cur->next;
    }
}

void iterateCur()
{
    cur = cur->next;
    while (cur && cur->next && !xmlStrcmp(cur->name, (const xmlChar *)"text")) {
	cur = cur->next;
    }
}

void requireName(const char *name)
{
    if (xmlStrcmp(cur->name, (const xmlChar *) name)) {
	fprintf(stderr, "expected %s; got %s\n", name, cur->name);
	xmlFreeDoc(doc);
	exit(1);
    }
}

void freeValue()
{
    if (val != NULL) {
	xmlFree(val);
	val = NULL;
    }
}

void getValue()
{
    if (val != NULL) xmlFree(val);
    val = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
}
