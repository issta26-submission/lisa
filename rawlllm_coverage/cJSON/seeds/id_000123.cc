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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create the root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 1;
    }

    // Add a string value: "name": "example"
    cJSON *name = cJSON_CreateString("example");
    if (!name) {
        cJSON_Delete(root);
        return 2;
    }
    cJSON_AddItemToObject(root, "name", name); // root now owns name

    // Add a number value: "age": 30
    cJSON *age = cJSON_CreateNumber(30);
    if (!age) {
        cJSON_Delete(root);
        return 3;
    }
    cJSON_AddItemToObject(root, "age", age);

    // Add a boolean value: "active": true
    cJSON *active = cJSON_CreateBool(1);
    if (!active) {
        cJSON_Delete(root);
        return 4;
    }
    cJSON_AddItemToObject(root, "active", active);

    // Create and add an array: "scores": [10, 20, 30]
    cJSON *scores = cJSON_CreateArray();
    if (!scores) {
        cJSON_Delete(root);
        return 5;
    }
    cJSON_AddItemToObject(root, "scores", scores);
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(30));

    // Create and add a nested object: "address": { "street": "...", "city": "..." }
    cJSON *address = cJSON_CreateObject();
    if (!address) {
        cJSON_Delete(root);
        return 6;
    }
    cJSON_AddItemToObject(address, "street", cJSON_CreateString("123 Example Ave"));
    cJSON_AddItemToObject(address, "city", cJSON_CreateString("Sampletown"));
    cJSON_AddItemToObject(root, "address", address);

    // Retrieve "name" and do a basic validation (ensure it's a string and starts with 'e')
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!name_item || !cJSON_IsString(name_item) || !name_item->valuestring || name_item->valuestring[0] != 'e') {
        cJSON_Delete(root);
        return 7;
    }

    // Replace "age" with a new value: 31
    cJSON *new_age = cJSON_CreateNumber(31);
    if (!new_age) {
        cJSON_Delete(root);
        return 8;
    }
    cJSON_ReplaceItemInObject(root, "age", new_age); // replaces old age, takes ownership of new_age

    // Detach "active" from root and reattach it under a new "meta" object
    cJSON *detached_active = cJSON_DetachItemFromObject(root, "active");
    cJSON *meta = cJSON_CreateObject();
    if (!meta) {
        // If we fail to create meta, put detached_active back to root to avoid leak
        if (detached_active) {
            cJSON_AddItemToObject(root, "active", detached_active);
        }
        cJSON_Delete(root);
        return 9;
    }
    if (detached_active) {
        cJSON_AddItemToObject(meta, "active", detached_active);
    } else {
        // if detach returned NULL, ensure meta still exists (no active inside)
    }
    cJSON_AddItemToObject(root, "meta", meta);

    // Produce an unformatted JSON string (caller owns the returned buffer)
    char *json_str = cJSON_PrintUnformatted(root);
    if (!json_str) {
        cJSON_Delete(root);
        return 10;
    }
    // Example usage: we won't print here, but free the allocated string
    free(json_str);

    // Demonstrate deep-copy (duplicate) of the root and then delete the copy
    cJSON *copy = cJSON_Duplicate(root, 1); // 1 means recursive/deep copy
    if (!copy) {
        cJSON_Delete(root);
        return 11;
    }
    cJSON_Delete(copy);

    // Clean up original root and all children
    cJSON_Delete(root);

    // On success, return 66 as requested
    return 66;
}