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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Add basic items
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());

    // Create an array and add it
    cJSON *hobbies = cJSON_CreateArray();
    if (!hobbies) { cJSON_Delete(root); return 2; }
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("reading"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("coding"));
    cJSON_AddItemToObject(root, "hobbies", hobbies);

    // Create nested object and add it
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 3; }
    cJSON_AddStringToObject(address, "street", "123 Main St");
    cJSON_AddStringToObject(address, "city", "Townsville");
    cJSON_AddItemToObject(root, "address", address);

    // Print an unformatted JSON string
    char *unformatted = cJSON_PrintUnformatted(root);
    if (!unformatted) { cJSON_Delete(root); return 4; }

    // Parse back the JSON string
    cJSON *parsed = cJSON_Parse(unformatted);
    if (!parsed) { free(unformatted); cJSON_Delete(root); return 5; }

    // Validate a string field (manual compare to avoid needing <string> or strcmp)
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!nameItem || !cJSON_IsString(nameItem) || !nameItem->valuestring) {
        cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root); return 6;
    }
    const char *expected_name = "example";
    const char *s = nameItem->valuestring;
    bool name_ok = true;
    for (int i = 0;; ++i) {
        if (expected_name[i] == '\0' && s[i] == '\0') break;
        if (expected_name[i] != s[i]) { name_ok = false; break; }
    }
    if (!name_ok) { cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root); return 7; }

    // Validate array size
    cJSON *hobbiesItem = cJSON_GetObjectItemCaseSensitive(parsed, "hobbies");
    if (!hobbiesItem || !cJSON_IsArray(hobbiesItem) || cJSON_GetArraySize(hobbiesItem) != 2) {
        cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root); return 8;
    }

    // Detach the age item from the original root and inspect it
    cJSON *age_item = cJSON_DetachItemFromObject(root, "age");
    if (!age_item || !cJSON_IsNumber(age_item) || age_item->valueint != 30) {
        if (age_item) cJSON_Delete(age_item);
        cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root); return 9;
    }

    // Replace the name in the original root
    cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("new_example"));

    // Duplicate the root (deep copy)
    cJSON *dup = cJSON_Duplicate(root, /*recurse=*/1);
    if (!dup) {
        cJSON_Delete(age_item);
        cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root);
        return 10;
    }

    // Print pretty, then minify a copy of it and parse to ensure minified JSON is valid
    char *pretty = cJSON_Print(root);
    if (!pretty) {
        cJSON_Delete(dup);
        cJSON_Delete(age_item);
        cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root);
        return 11;
    }

    // Make a mutable copy of the pretty string to minify (cJSON_Minify modifies in-place)
    size_t plen = std::strlen(pretty);
    char *mincopy = (char*)malloc(plen + 1);
    if (!mincopy) {
        free(pretty);
        cJSON_Delete(dup);
        cJSON_Delete(age_item);
        cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root);
        return 12;
    }
    std::memcpy(mincopy, pretty, plen + 1);
    cJSON_Minify(mincopy);

    cJSON *parsed_min = cJSON_Parse(mincopy);
    if (!parsed_min) {
        free(mincopy);
        free(pretty);
        cJSON_Delete(dup);
        cJSON_Delete(age_item);
        cJSON_Delete(parsed); free(unformatted); cJSON_Delete(root);
        return 13;
    }

    // Clean up everything
    cJSON_Delete(parsed_min);
    free(mincopy);
    free(pretty);

    cJSON_Delete(dup);
    cJSON_Delete(age_item); // we detached it earlier; now delete it
    cJSON_Delete(parsed);
    free(unformatted);
    cJSON_Delete(root);

    // If we reach here, the sequence of API calls worked logically
    return 66;
}