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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return -1;
    }

    // Add simple members
    if (!cJSON_AddStringToObject(root, "name", "example")) {
        cJSON_Delete(root);
        return -2;
    }
    if (!cJSON_AddNumberToObject(root, "age", 30)) {
        cJSON_Delete(root);
        return -3;
    }

    // Create an array and add elements
    cJSON *items = cJSON_CreateArray();
    if (!items) {
        cJSON_Delete(root);
        return -4;
    }
    cJSON *one = cJSON_CreateString("one");
    if (!one) {
        cJSON_Delete(items);
        cJSON_Delete(root);
        return -5;
    }
    cJSON_AddItemToArray(items, one);

    cJSON *two = cJSON_CreateNumber(2);
    if (!two) {
        cJSON_Delete(items);
        cJSON_Delete(root);
        return -6;
    }
    cJSON_AddItemToArray(items, two);

    // Attach array to root
    cJSON_AddItemToObject(root, "items", items);

    // Add a boolean (true) item
    cJSON *active = cJSON_CreateTrue();
    if (!active) {
        cJSON_Delete(root);
        return -7;
    }
    cJSON_AddItemToObject(root, "active", active);

    // Serialize to string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) {
        cJSON_Delete(root);
        return -8;
    }

    // Parse the printed string back into a new cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) {
        free(printed);
        cJSON_Delete(root);
        return -9;
    }

    // Validate some values in the parsed object
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || !name->valuestring || strcmp(name->valuestring, "example") != 0) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -10;
    }

    cJSON *par_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!cJSON_IsArray(par_items)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -11;
    }
    cJSON *first = cJSON_GetArrayItem(par_items, 0);
    if (!cJSON_IsString(first) || !first->valuestring || strcmp(first->valuestring, "one") != 0) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -12;
    }

    // Replace the "name" item in the parsed object
    cJSON *new_name = cJSON_CreateString("changed");
    if (!new_name) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -13;
    }
    cJSON_ReplaceItemInObject(parsed, "name", new_name);

    // Detach and delete the "age" item from parsed (demonstrates detach)
    cJSON *detached_age = cJSON_DetachItemFromObject(parsed, "age");
    if (detached_age) {
        cJSON_Delete(detached_age);
    }

    // Delete "active" item from parsed
    cJSON_DeleteItemFromObject(parsed, "active");

    // Optionally re-serialize parsed to ensure structure is valid (and free)
    char *printed2 = cJSON_Print(parsed);
    if (printed2) {
        free(printed2);
    } else {
        // If serialization fails, still continue to clean up and report failure
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -14;
    }

    // Clean up
    cJSON_Delete(parsed);
    free(printed);
    cJSON_Delete(root);

    // Success
    return 66;
}