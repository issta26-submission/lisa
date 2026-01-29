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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add some basic members (string, number, boolean)
    cJSON_AddStringToObject(root, "name", "cJSON Test");
    cJSON_AddNumberToObject(root, "version", 1.7);
    cJSON_AddTrueToObject(root, "ok"); // helper macro to add a boolean true

    // Create an array and add elements
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToObject(root, "items", arr);

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return -3; }
    cJSON_AddStringToObject(nested, "nestedKey", "nestedValue");
    cJSON_AddItemToObject(root, "nested", nested);

    // Print/unformatted JSON to a string
    char *json_str = cJSON_PrintUnformatted(root);
    if (!json_str) { cJSON_Delete(root); return -4; }

    // Parse the printed JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(json_str);
    // free the printed string (cJSON_Print* uses heap allocation)
    free(json_str);

    if (!parsed) { cJSON_Delete(root); return -5; }

    // Access members from the parsed object and validate types
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -6;
    }

    cJSON *version = cJSON_GetObjectItemCaseSensitive(parsed, "version");
    if (!cJSON_IsNumber(version)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -7;
    }

    cJSON *ok = cJSON_GetObjectItemCaseSensitive(parsed, "ok");
    if (!(ok && (ok->type & cJSON_True || ok->type & cJSON_False || cJSON_IsBool(ok)))) {
        // Some cJSON builds expose boolean type via cJSON_True/cJSON_False bits;
        // cJSON_IsBool macro may also be available. We accept any reasonable boolean representation.
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -8;
    }

    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!items) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -9;
    }

    int items_count = cJSON_GetArraySize(items);
    if (items_count != 2) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -10;
    }

    // Check array element types
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    if (!cJSON_IsString(first) || !cJSON_IsNumber(second)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -11;
    }

    // All checks passed; clean up and return success code 66
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66;
}