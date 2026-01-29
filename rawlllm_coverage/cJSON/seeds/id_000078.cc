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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Basic items
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30);
    cJSON *active = cJSON_CreateTrue();
    if (!name || !age || !active) {
        cJSON_Delete(root);
        return -1;
    }

    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "age", age);
    cJSON_AddItemToObject(root, "active", active);

    // Nested object (address)
    cJSON *address = cJSON_CreateObject();
    if (!address) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddItemToObject(address, "city", cJSON_CreateString("Wonderland"));
    cJSON_AddItemToObject(address, "zip", cJSON_CreateNumber(12345));
    cJSON_AddItemToObject(root, "address", address);

    // Array of phone numbers
    cJSON *phones = cJSON_CreateArray();
    if (!phones) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddItemToArray(phones, cJSON_CreateString("123-4567"));
    cJSON_AddItemToArray(phones, cJSON_CreateString("987-6543"));
    cJSON_AddItemToObject(root, "phones", phones);

    // Mixed items array
    cJSON *items = cJSON_CreateArray();
    if (!items) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2));
    cJSON *childObj = cJSON_CreateObject();
    cJSON_AddItemToObject(childObj, "type", cJSON_CreateString("widget"));
    cJSON_AddItemToObject(childObj, "count", cJSON_CreateNumber(3));
    cJSON_AddItemToArray(items, childObj);
    cJSON_AddItemToObject(root, "items", items);

    // Read some values back safely
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (cJSON_IsString(got_name) && got_name->valuestring) {
        // Just demonstrating reading; we won't print in some embedded contexts
        // printf("Name: %s\n", got_name->valuestring);
    } else {
        cJSON_Delete(root);
        return -1;
    }

    cJSON *got_phones = cJSON_GetObjectItemCaseSensitive(root, "phones");
    if (!cJSON_IsArray(got_phones)) {
        cJSON_Delete(root);
        return -1;
    }
    int phones_count = cJSON_GetArraySize(got_phones);
    for (int i = 0; i < phones_count; ++i) {
        cJSON *p = cJSON_GetArrayItem(got_phones, i);
        if (!cJSON_IsString(p)) {
            cJSON_Delete(root);
            return -1;
        }
    }

    // Detach the "age" item and reattach it under a different key (demonstrate detach/attach)
    cJSON *detached_age = cJSON_DetachItemFromObject(root, "age"); // ownership transferred to us
    if (detached_age) {
        cJSON_AddItemToObject(root, "previous_age", detached_age); // reattach under new key
    } else {
        cJSON_Delete(root);
        return -1;
    }

    // Replace the "active" boolean with false to demonstrate replace API
    cJSON_ReplaceItemInObject(root, "active", cJSON_CreateFalse());

    // Serialize to string
    char *printed = cJSON_Print(root);
    if (printed) {
        // Optionally print; then free the printed string
        // printf("%s\n", printed);
        free(printed); // cJSON uses malloc internally, so free() is appropriate
    } else {
        cJSON_Delete(root);
        return -1;
    }

    // Demonstrate parsing back from string
    char *roundtrip = cJSON_Print(root);
    if (!roundtrip) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON *parsed = cJSON_Parse(roundtrip);
    free(roundtrip);
    if (!parsed) {
        cJSON_Delete(root);
        return -1;
    }
    // Validate parsed structure: check name again
    cJSON *parsed_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(parsed_name) || !parsed_name->valuestring) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Clean up parsed and the original tree
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // Success
    return 66;
}