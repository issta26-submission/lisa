#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // include headers inside the function so this single function remains self-contained
    // (including headers inside a block is legal; they are processed by the preprocessor).
    #include <cstring>
    #include <cstdlib>
    #include "cJSON.h"

    cJSON *root = nullptr;
    cJSON *parsed = nullptr;
    cJSON *dup = nullptr;
    char *printed = nullptr;
    char *minified = nullptr;
    bool ok = true;

    // 1) Create root object and populate it with different types
    root = cJSON_CreateObject();
    if (!root) return -1;

    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);
    cJSON_AddTrueToObject(root, "is_student");

    // 2) Create an array and add it to root
    cJSON *hobbies = cJSON_CreateArray();
    if (!hobbies) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("reading"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("coding"));
    cJSON_AddItemToObject(root, "hobbies", hobbies);

    // 3) Create a nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return -1; }
    cJSON_AddStringToObject(address, "city", "NY");
    cJSON_AddNumberToObject(address, "zip", 10001);
    cJSON_AddItemToObject(root, "address", address);

    // 4) Print (pretty) the JSON to a string
    printed = cJSON_Print(root);
    if (!printed) { cJSON_Delete(root); return -1; }

    // 5) Parse the printed JSON back into a new cJSON structure
    parsed = cJSON_Parse(printed);
    if (!parsed) {
        free(printed);
        cJSON_Delete(root);
        return -1;
    }

    // 6) Validate some fields from the parsed JSON
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!(name && cJSON_IsString(name) && strcmp(name->valuestring, "example") == 0)) {
        ok = false;
    }

    cJSON *harr = cJSON_GetObjectItemCaseSensitive(parsed, "hobbies");
    if (!(harr && cJSON_IsArray(harr) && cJSON_GetArraySize(harr) == 2)) {
        ok = false;
    }

    // 7) Duplicate (deep copy) the parsed object
    dup = cJSON_Duplicate(parsed, 1); // 1 = recursive/deep copy
    if (!dup) ok = false;

    // 8) Modify the duplicate: replace the "name" value and detach "age"
    if (dup) {
        cJSON_ReplaceItemInObject(dup, "name", cJSON_CreateString("changed"));
        cJSON *detached_age = cJSON_DetachItemFromObject(dup, "age"); // detach from dup
        if (detached_age) {
            // reattach detached item under a different key in the original root
            cJSON_AddItemToObject(root, "detached_age", detached_age);
        } else {
            ok = false;
        }
    }

    // 9) Demonstrate minification: get an unformatted string and minify it in-place
    minified = cJSON_Print(root);
    if (minified) {
        cJSON_Minify(minified); // minifies the string in place
        free(minified);
        minified = nullptr;
    } else {
        ok = false;
    }

    // 10) Cleanup all created structures and memory
    free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);

    if (!ok) return -1;
    return 66;
}