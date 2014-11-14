#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <string.h>

xmlDocPtr doc;
xmlNodePtr cur;
xmlChar *val;

typedef struct path_t *Path;
struct path_t {
    const char *comp;
    char wildcard;
    Path next;
};

Path createComps(const char *property);

void locate(const Path path);

void iterateCur(); ///< move to next non-text entry
void enterCur();   ///< go into the first non-text child of cur
void requireName(const char *name);
void getValue();
void freeValue();

int main(int argc, const char **argv)
{
    if (argc != 3) {
	fprintf(stderr, "syntax: %s <plist> <property to read>\n"
		"The property may be a path of keys; the keys must then correspond to dicts\n"
		"The path may contain the special path component \"*\", which does two things: (1) it prints out every component found, and (2) it matches every component found\n", argv[0]);
	return 1;
    }

    const char *filename = argv[1];
    const char *property = argv[2];
    const Path  path = createComps(property);
    
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

    locate(path);

    // clean up
    freeValue();
    xmlFreeDoc(doc);

    return 0;
}

void locate(const Path path)
{
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *) "key"))) {
	    // this is a key entry
	    getValue();

	    if (path->wildcard || !xmlStrcmp(val, (const xmlChar *)path->comp)) {
		// got the property; we expect
		// <key>property</key>
		// <valtype>sought after value</valtype>
		// where "valtype" can be e.g. "string", "integer", etc. we ignore valtype and always get strings
		// if this is a wildcard, we print it to stdout
		if (path->wildcard) {
		    getValue();
		    printf("*%s\n", val);
		}
		iterateCur();
		if (cur == NULL) {
		    fprintf(stderr, "missing xml element after <key>\n");
		    exit(1);
		}

		if (path->next) {
		    // this is a part of a path; cur must be a dict type
		    xmlNodePtr bookmark = cur;
		    requireName("dict");
		    enterCur();
		    locate(path->next);
		    cur = bookmark;
		} else {
		    // this is a leaf
		    getValue();
		    printf("%s\n", val);
		}
	    }
	}
	iterateCur();
    }
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

Path createComps(const char *property)
{
    Path path = NULL;
    Path tail = NULL;
    int pos = 0;
    int len = strlen(property);

    for (int i = 0; i <= len; i++) {
	if (i == len || property[i] == '/') {
	    if (i == pos) {
		// if we got '/foo/bar' and we're at the first '/' we want to just bump the starting pos and ignore
		pos++;
	    } else {
		// otherwise we want to add the path
		if (path == NULL) {
		    // this is the first path
		    path = tail = malloc(sizeof(struct path_t));
		} else {
		    tail->next = malloc(sizeof(struct path_t));
		    tail = tail->next;
		}
		tail->comp = strndup(&property[pos], i - pos);
		tail->next = NULL;
		tail->wildcard = !strcmp(tail->comp, "*");
		pos = i + 1;
	    }
	}
    }

    return path;
}
